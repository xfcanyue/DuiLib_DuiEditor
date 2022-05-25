#include "StdAfx.h"
#include "UIFile.h"


///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	CUIFile::CUIFile()
	{
		m_pData = NULL; 
		m_dwSize = 0;
	}

	CUIFile::~CUIFile()
	{
		Empty();
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

	//////////////////////////////////////////////////////////////////////////
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
} // namespace DuiLib
