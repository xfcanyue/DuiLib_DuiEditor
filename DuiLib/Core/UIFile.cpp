#include "StdAfx.h"
#include "UIFile.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef WIN32
#include "../Utils/unzip.cpp"
#endif
///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	CUIFile::CUIFile()
	{
		m_pData = NULL; 
		m_dwSize = 0;
		m_fp = NULL;
		m_charset = CUIFile::FILE_ANSI;
	}

	CUIFile::~CUIFile()
	{
		Empty();
		Close();
	}

	LPBYTE CUIFile::GetData() const
	{
		return m_pData;
	}

	DWORD  CUIFile::GetSize() const
	{
		return m_dwSize;
	}

	void CUIFile::Empty()
	{
		if(m_pData != NULL) { delete []m_pData; m_pData = NULL; }
		m_dwSize = 0U;
	}

	BOOL CUIFile::LoadFile(const STRINGorID &bitmap, LPCTSTR type, HINSTANCE instance)
	{
		Empty();

		//读取顺序  zip文件 ==> skin文件夹 ==> 资源 ==> 文件绝对路径
		if (type == NULL)
		{
			if (__LoadFromZip(bitmap, type, instance))
				return TRUE;
			if (__LoadFromSkinPath(bitmap, type, instance))
				return TRUE;
		}
		else
		{
#ifdef DUILIB_WIN32
			if (__LoadFromResource(bitmap, type, instance))
				return TRUE;
#endif
		}

		return __LoadFromDiskPath(bitmap.m_lpstr);
	}


	BOOL CUIFile::LoadFile(LPCTSTR pStrImage, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(pStrImage), type, instance);
	}

#ifdef WIN32
	BOOL CUIFile::LoadFile(UINT nID, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(nID), type, instance);
	}
#endif

	BOOL CUIFile::__LoadFromSkinPath(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance)
	{
		CDuiString sFile = CPaintManagerUI::GetResourcePath();	
		sFile += bitmap.m_lpstr;
#ifndef DUILIB_WIN32
		sFile.Replace(_T("\\"), _T("/"));
#endif
		return __LoadFromDiskPath(sFile);
	}

	BOOL CUIFile::__LoadFromZip(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance)
	{
		if (CPaintManagerUI::GetResourceZip().IsEmpty())
			return FALSE;

#ifdef WIN32
		HZIP hz = NULL;
		do 
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			sFile += CPaintManagerUI::GetResourceZip();
			if (CPaintManagerUI::IsCachedResourceZip())  //zip资源
			{
				hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
			}
			else //zip文件
			{
				CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
				UISTRING_CONVERSION;
				hz = OpenZip(sFile.GetData(), UIT2A(sFilePwd.GetData()));
			}
			if (hz == NULL) break;
			ZIPENTRY ze;
			int i = 0;
			CDuiString key = bitmap.m_lpstr;
			key.Replace(_T("\\"), _T("/"));
			if (FindZipItem(hz, key, true, &i, &ze) != 0) break;
			m_dwSize = ze.unc_size;
			if (m_dwSize == 0) break;
			m_pData = new BYTE[m_dwSize + 1];
			m_pData[m_dwSize] = '\0';
			int res = UnzipItem(hz, i, m_pData, m_dwSize);
			if (res != 0x00000000 && res != 0x00000600)
			{
				Empty();
			}
		} while (0);

		if (!CPaintManagerUI::IsCachedResourceZip())
			CloseZip(hz);
#endif

		return m_dwSize > 0;
	}

#ifdef WIN32
	BOOL CUIFile::__LoadFromResource(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance)
	{
		HINSTANCE dllinstance = NULL;
		if (instance)
		{
			dllinstance = instance;
		}
		else
		{
			dllinstance = CPaintManagerUI::GetResourceDll();
		}

		HRSRC hResource = ::FindResource(dllinstance, bitmap.m_lpstr, type);
		if (hResource == NULL) return FALSE;
		HGLOBAL hGlobal = ::LoadResource(dllinstance, hResource);
		if (hGlobal == NULL)
		{
			FreeResource(hResource);
			return FALSE;
		}

		m_dwSize = ::SizeofResource(dllinstance, hResource);
		if (m_dwSize == 0)
		{
			FreeResource(hResource);
			return FALSE;
		}
		m_pData = new BYTE[m_dwSize + 1];
		m_pData[m_dwSize] = '\0';
		::CopyMemory(m_pData, (LPBYTE)::LockResource(hGlobal), m_dwSize);
		::FreeResource(hResource);

		return m_dwSize > 0;
	}
#endif

	BOOL CUIFile::__LoadFromDiskPath(LPCTSTR sFilePath)
	{
		//直接去读取bitmap.m_lpstr指向的路径
		FILE *file = _tfopen(sFilePath, _T("rb+"));
		if(file==NULL) return FALSE;
		if(feof(file)) return FALSE;

		int pos = ftell(file);
		fseek(file, 0, SEEK_END);
		m_dwSize = ftell(file);
		fseek(file, pos, SEEK_SET);

		if (m_dwSize == 0)
		{
			fclose(file);
			return FALSE;
		}

		m_pData = new BYTE[m_dwSize + 1];
		memset(m_pData, 0, sizeof(BYTE)*(m_dwSize+1));
		int nRead = fread(m_pData, 1, m_dwSize, file);
		fclose(file);
		return m_dwSize > 0;
	}
	/*
	BOOL CUIFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags)
	{
		Close();

		CDuiString mode;
		if((nOpenFlags & modeRead) ==  modeRead)
		{
			mode = _T("r");
		}
		else if((nOpenFlags & modeWrite) ==  modeWrite)
		{
			mode = _T("w");
		}
		else if((nOpenFlags & modeReadWrite) ==  modeReadWrite)
		{
			mode = _T("r");
		}

		m_fp = _tfopen(lpszFileName, mode);
		return m_fp != NULL;
	}
	*/

	BOOL CUIFile::Open(LPCTSTR lpszFileName, LPCTSTR mode)
	{
		m_fp = _tfopen(lpszFileName, mode);
		return m_fp != NULL;
	}

	void CUIFile::Close()
	{
		if(m_fp) { fclose(m_fp); m_fp = NULL; }
	}

	UINT CUIFile::Read(void* lpBuf, UINT nCount)
	{
		return fread(lpBuf, 1, nCount, m_fp);
	}

	UINT CUIFile::Write(const void* lpBuf, UINT nCount)
	{
		return fwrite(lpBuf, 1, nCount, m_fp);
	}

	UINT CUIFile::WriteV(const char* lpszFormat, ...)
	{
		UINT nWrite = 0;
		UISTRING_CONVERSION;
		CDuiString sFormat = UIA2T(lpszFormat);

		CDuiString strText;
		va_list argList;
		va_start(argList, lpszFormat);
		strText.InnerFormat(sFormat, argList);
		va_end(argList);

		if(m_charset == CUIFile::FILE_UTF8)
		{
			const char* pText = UIT2UTF8(strText);
			nWrite = Write(pText, strlen(pText));
		}
		else if(m_charset == CUIFile::FILE_ANSI)
		{
			const char* pText = UIT2A(strText);
			nWrite = Write(pText, strlen(pText));
		}
		return nWrite;
	}

	UINT CUIFile::WriteV(const wchar_t* lpszFormat, ...)
	{
		UINT nWrite = 0;
		UISTRING_CONVERSION;
		CDuiString sFormat = UIW2T(lpszFormat);

		CDuiString strText;
		va_list argList;
		va_start(argList, lpszFormat);
		strText.InnerFormat(sFormat, argList);
		va_end(argList);

		if(m_charset == CUIFile::FILE_UTF8)
		{
			const char* pText = UIT2UTF8(strText);
			nWrite = Write(pText, strlen(pText));
		}
		else if(m_charset == CUIFile::FILE_ANSI)
		{
			const char* pText = UIT2A(strText);
			nWrite = Write(pText, strlen(pText));
		}
		return nWrite;
	}

	UINT CUIFile::GetFileLength() const
	{
		DWORD fsize;
		int pos = ftell(m_fp);
		fseek(m_fp, 0, SEEK_END);
		fsize = ftell(m_fp);
		fseek(m_fp, pos, SEEK_SET);
		return fsize;
	}

	int CUIFile::SeekToEnd()
	{
		return fseek(m_fp, 0, SEEK_END);
	}

	void CUIFile::SeekToBegin()
	{
		fseek(m_fp, 0, SEEK_SET);
	}

	int CUIFile::Seek(UINT lOff, UINT nFrom)
	{
		return fseek(m_fp, lOff, nFrom);
	}

	BOOL CUIFile::IsEOF()
	{
		return feof(m_fp) == 1;
	}

	void CUIFile::SetCharSet(CUIFile::CharSet charset)
	{
		m_charset = charset;
	}
/*
	BOOL CUIFile::LoadFile(const STRINGorID &bitmap, LPCTSTR type, HINSTANCE instance)
	{
		Empty();

		//读取顺序  skin文件夹 ==> zip文件 ==> 资源 ==> 文件绝对路径
		do 
		{
			if( type == NULL )
			{
				CDuiString sFile = CPaintManagerUI::GetResourcePath();
				if( CPaintManagerUI::GetResourceZip().IsEmpty() ) //直接读文件夹
				{
					sFile += bitmap.m_lpstr;
					HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
						FILE_ATTRIBUTE_NORMAL, NULL);
					if( hFile == INVALID_HANDLE_VALUE ) break;
					m_dwSize = ::GetFileSize(hFile, NULL);
					if( m_dwSize == 0 ) break;

					DWORD dwRead = 0;
					m_pData = new BYTE[ m_dwSize + 1 ];
					m_pData[m_dwSize] = '\0';
					::ReadFile( hFile, m_pData, m_dwSize, &dwRead, NULL );
					::CloseHandle( hFile );

					if( dwRead != m_dwSize ) 
					{
						Empty();
						break;
					}
				}
				else //读zip
				{
					sFile += CPaintManagerUI::GetResourceZip();
					HZIP hz = NULL;
					if (CPaintManagerUI::IsCachedResourceZip())  //zip资源
					{
						hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
					}
					else //zip文件
					{
						CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
						UISTRING_CONVERSION;
						hz = OpenZip(sFile.GetData(), UIT2A(sFilePwd.GetData()));
					}
					if( hz == NULL ) break;
					ZIPENTRY ze; 
					int i = 0; 
					CDuiString key = bitmap.m_lpstr;
					key.Replace(_T("\\"), _T("/")); 
					if( FindZipItem(hz, key, true, &i, &ze) != 0 ) break;
					m_dwSize = ze.unc_size;
					if( m_dwSize == 0 ) break;
					m_pData = new BYTE[ m_dwSize + 1];
					m_pData[m_dwSize] = '\0';
					int res = UnzipItem(hz, i, m_pData, m_dwSize);
					if( res != 0x00000000 && res != 0x00000600)
					{
						Empty();
						if( !CPaintManagerUI::IsCachedResourceZip() )
							CloseZip(hz);
						break;
					}
					if( !CPaintManagerUI::IsCachedResourceZip() )
						CloseZip(hz);
				}
			}
			else //读取资源id
			{
				HINSTANCE dllinstance = NULL;
				if (instance)
				{
					dllinstance = instance;
				}
				else
				{
					dllinstance = CPaintManagerUI::GetResourceDll();
				}
				HRSRC hResource = ::FindResource(dllinstance, bitmap.m_lpstr, type);
				if( hResource == NULL ) break;
				HGLOBAL hGlobal = ::LoadResource(dllinstance, hResource);
				if( hGlobal == NULL ) 
				{
					FreeResource(hResource);
					break;
				}

				m_dwSize = ::SizeofResource(dllinstance, hResource);
				if( m_dwSize == 0 ) break;
				m_pData = new BYTE[ m_dwSize + 1];
				m_pData[m_dwSize] = '\0';
				::CopyMemory(m_pData, (LPBYTE)::LockResource(hGlobal), m_dwSize);
				::FreeResource(hResource);
			}
		} while (0);

		while (!m_pData)
		{
			//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
			HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
				FILE_ATTRIBUTE_NORMAL, NULL);
			if( hFile == INVALID_HANDLE_VALUE ) break;
			m_dwSize = ::GetFileSize(hFile, NULL);
			if( m_dwSize == 0 ) break;

			DWORD dwRead = 0;
			m_pData = new BYTE[ m_dwSize + 1];
			m_pData[m_dwSize] = '\0';
			::ReadFile( hFile, m_pData, m_dwSize, &dwRead, NULL );
			::CloseHandle( hFile );

			if( dwRead != m_dwSize ) 
			{
				Empty();
			}
			break;
		}
		return m_dwSize > 0;
	}

	BOOL CUIFile::LoadFile(LPCTSTR pStrImage, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(pStrImage), type, instance);
	}

	BOOL CUIFile::LoadFile(UINT nID, LPCTSTR type, HINSTANCE instance)
	{
		return LoadFile(STRINGorID(nID), type, instance);
	}
*/
	//////////////////////////////////////////////////////////////////////////
/*
	CUIFileFind::CUIFileFind()
	{
		m_bFindInZip = FALSE;
		m_hFind = INVALID_HANDLE_VALUE;
		m_pFoundInfo = NULL;
		m_pNextInfo = NULL;

		m_hZip = NULL;
		m_nZipIndex = 0;
	}

	CUIFileFind::~CUIFileFind()
	{
		FindClose();
	}

	BOOL CUIFileFind::FindFile(LPCTSTR lpFileName)
	{
		FindClose();

		//是否相对路径
		BOOL bRelativePath = PathIsRelative(lpFileName);
		if (bRelativePath)
		{
			int resType = CPaintManagerUI::GetResourceType();		
			if (resType == UILIB_FILE) //皮肤文件在磁盘文件
			{
				m_pNextInfo = new WIN32_FIND_DATA;

				m_strRoot = CPaintManagerUI::GetResourcePath() + lpFileName;
				TCHAR strDrive[_MAX_DRIVE], strDir[_MAX_DIR];
				_tsplitpath_s(m_strRoot, strDrive, _MAX_DRIVE, strDir, _MAX_DIR, NULL, 0, NULL, 0);
				_tmakepath_s(m_strRoot.GetBuffer(_MAX_PATH), _MAX_PATH, strDrive, strDir, NULL, NULL);

				m_hFind = ::FindFirstFile(CPaintManagerUI::GetResourcePath() + lpFileName, (LPWIN32_FIND_DATA)m_pNextInfo);
				if (m_hFind != INVALID_HANDLE_VALUE)
					return TRUE;
				delete m_pNextInfo;
				m_pNextInfo = NULL;
			}
			else if (resType == UILIB_ZIP) //ZIP文件
			{

			}
			else if (resType == UILIB_ZIPRESOURCE) //ZIP资源方式
			{
				m_strRoot = lpFileName;
				MakeFullPath(m_strRoot);
				m_strRoot.Replace(_T("\\"), _T("/"));

				m_hZip = CPaintManagerUI::GetResourceZipHandle();
				int i = 0;
				ZRESULT zRet = FindZipItem((HZIP)m_hZip, m_strRoot, true, &i, &m_zipEntry);
				if (zRet == ZR_OK)
				{
					m_bFindInZip = TRUE;
					return TRUE;
				}
			}

			//在程序目录下搜索
			m_pNextInfo = new WIN32_FIND_DATA; 
			m_strRoot = CPaintManagerUI::GetInstancePath() + lpFileName;

			m_hFind = ::FindFirstFile(CPaintManagerUI::GetInstancePath() + lpFileName, (LPWIN32_FIND_DATA)m_pNextInfo);
			if (m_hFind != INVALID_HANDLE_VALUE)
				return TRUE;
			delete m_pNextInfo;
			m_pNextInfo = NULL;
		}
		else
		{
			LPCTSTR pstr = _tfullpath(m_strRoot.GetBuffer(_MAX_PATH), lpFileName, _MAX_PATH);
			if (pstr != NULL)
			{
				m_pNextInfo = new WIN32_FIND_DATA;
				m_hFind = ::FindFirstFile(lpFileName, (LPWIN32_FIND_DATA)m_pNextInfo);
				return m_hFind != INVALID_HANDLE_VALUE;
				delete m_pNextInfo;
				m_pNextInfo = NULL;
			}
		}
		return FALSE;
	}

	BOOL CUIFileFind::FindNextFile()
	{
		if (m_bFindInZip)
		{
			ZRESULT zRet = GetZipItem((HZIP)m_hZip, m_nZipIndex, &m_zipEntryNext);
			m_nZipIndex++;
			return zRet == ZR_OK;
		}
		else
		{
			if (m_hFind == INVALID_HANDLE_VALUE)
				return FALSE;

			if (m_pFoundInfo == NULL)
				m_pFoundInfo = new WIN32_FIND_DATA;

			void* pTemp = m_pFoundInfo;
			m_pFoundInfo = m_pNextInfo;
			m_pNextInfo = pTemp;
			return ::FindNextFile(m_hFind, (LPWIN32_FIND_DATA)m_pNextInfo);
		}
	}

	void CUIFileFind::FindClose()
	{
		m_bFindInZip = FALSE;

		if (m_pFoundInfo != NULL)
		{
			delete m_pFoundInfo;
			m_pFoundInfo = NULL;
		}

		if (m_pNextInfo != NULL)
		{
			delete m_pNextInfo;
			m_pNextInfo = NULL;
		}

		if (m_hFind != INVALID_HANDLE_VALUE) { 
			::FindClose(m_hFind); 
			m_hFind = INVALID_HANDLE_VALUE; 
		}

		if (m_hZip != NULL && !CPaintManagerUI::IsCachedResourceZip()) { 
			CloseZip((HZIP)m_hZip); 
			m_hZip = NULL; 
		}

		m_nZipIndex = 0;
	}

	CDuiString CUIFileFind::GetFileName()
	{
		CDuiString sFile;
		if (m_bFindInZip)
		{
			sFile = m_zipEntryNext.name;
		}
		else
		{
			if (m_pFoundInfo != NULL)
				sFile = ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;
		}
		return sFile;
	}

	CDuiString CUIFileFind::GetFilePath()
	{
		CDuiString sFile;
		if (m_bFindInZip)
		{
			sFile = m_zipEntryNext.name;
		}
		else
		{
			if (m_pFoundInfo != NULL)
			{
				sFile = m_strRoot;
				sFile += ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;
			}
		}
		return sFile;
	}

// 	CDuiString CUIFileFind::GetFileTitleName()
// 	{
// 		CDuiString sFile;
// 		if (m_bFindInZip)
// 		{
// 			sFile = m_zipEntryNext.name;
// 		}
// 		else
// 		{
// 			if (m_pFoundInfo != NULL)
// 				sFile = ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;
// 		}
// 		return sFile;
// 	}
// 
// 	CDuiString CUIFileFind::GetFileExtName()
// 	{
// 		CDuiString sFile;
// 		if (m_bFindInZip)
// 		{
// 			sFile = m_zipEntryNext.name;
// 		}
// 		else
// 		{
// 			if (m_pFoundInfo != NULL)
// 				sFile = ((LPWIN32_FIND_DATA)m_pFoundInfo)->cFileName;
// 		}
// 		return sFile;
// 	}

	BOOL CUIFileFind::IsDots()
	{
		BOOL bResult = FALSE;
		if (m_bFindInZip)
		{
			
		}
		else
		{
			if (m_pFoundInfo != NULL && IsDirectory())
			{
				LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA)m_pFoundInfo;
				if (pFindData->cFileName[0] == '.')
				{
					if (pFindData->cFileName[1] == '\0' ||
						(pFindData->cFileName[1] == '.' &&
							pFindData->cFileName[2] == '\0'))
					{
						bResult = TRUE;
					}
				}
			}
		}
		return bResult;
	}

	BOOL CUIFileFind::IsDirectory()
	{
		BOOL bResult = FALSE;
		if (m_bFindInZip)
		{
			bResult = (m_zipEntryNext.attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
		}
		else
		{
			if (m_pFoundInfo != NULL)
				bResult = (((LPWIN32_FIND_DATA)m_pFoundInfo)->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
		}
		return bResult;
	}

	void CUIFileFind::MakeFullPath(CDuiString &sPath)
	{
		//有 "*" 就去掉。
		int n = sPath.ReverseFind(_T('\\'));
		CDuiString sTemp = sPath.Right(sPath.GetLength()-n-1);
		if (sTemp.Find(_T('*') >= 0))
		{
			sPath = sPath.Left(n + 1);
		}
		return;
	}
	*/
} // namespace DuiLib

