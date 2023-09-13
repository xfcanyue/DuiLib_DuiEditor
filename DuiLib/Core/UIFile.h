#ifndef __UIFILE_H__
#define __UIFILE_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CUIFile
	{
	public:
// 		enum OpenFlags {
// 			modeRead =         (int) 0x00000,
// 			modeWrite =        (int) 0x00001,
// 			modeReadWrite =    (int) 0x00002,
// 			modeCreate =       (int) 0x01000,
// 			modeNoTruncate =   (int) 0x02000,	//如果文件存在，追加内容；如果文件不存在创建新文件。
// 			typeBinary =       (int) 0x08000,
// 		};
		enum CharSet {
			FILE_ANSI =  (int) 0x00000,
			FILE_UTF8 =  (int) 0x00001,
		};
	public:
		CUIFile();
		~CUIFile();

		LPBYTE GetData() const;
		DWORD  GetSize() const;
		void Empty();

		BOOL LoadFile(const STRINGorID &bitmap, LPCTSTR type=NULL, HINSTANCE instance=NULL);
		BOOL LoadFile(LPCTSTR pStrImage, LPCTSTR type=NULL, HINSTANCE instance=NULL);
#ifdef WIN32
		BOOL LoadFile(UINT nID, LPCTSTR type=NULL, HINSTANCE instance=NULL);
#endif

		//BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags);
		BOOL Open(LPCTSTR lpszFileName, LPCTSTR mode);
		void Close();
		UINT Read(void* lpBuf, UINT nCount);
		UINT Write(const void* lpBuf, UINT nCount);
		UINT WriteV(const char* lpszFormat, ...);
		UINT WriteV(const wchar_t* lpszFormat, ...);
		UINT WriteLine(const char* lpszFormat, ...);
		UINT WriteLine(const wchar_t* lpszFormat, ...);
		UINT GetFileLength() const;
		int SeekToEnd();
		void SeekToBegin();
		int Seek(UINT lOff, UINT nFrom);
		BOOL IsEOF();
		void SetCharSet(CUIFile::CharSet charset);
	protected:
		BOOL __LoadFromSkinPath(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance);
		BOOL __LoadFromZip(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance);
#ifdef WIN32
		BOOL __LoadFromResource(const STRINGorID& bitmap, LPCTSTR type, HINSTANCE instance);
#endif
		BOOL __LoadFromDiskPath(LPCTSTR sFilePath);
	private:
		LPBYTE m_pData; 
		DWORD m_dwSize;

		FILE *m_fp;
		UINT m_charset;

	// --- static functions --
	public:
		static BOOL CopyFile(LPCTSTR sSourceFilePathName, LPCTSTR sDestFilePathName, BOOL bFailIfExists); //拷贝文件
		static BOOL DeleteFile(LPCTSTR sFilePathName); //删除文件
		static BOOL IsFileExist(LPCTSTR sFilePathName);	//文件是否存在
		
		static BOOL CreateDirectory(LPCTSTR sPathName, BOOL bCreateMultiLevelDirectory = FALSE); //创建目录
		static BOOL RemoveDirectory(LPCTSTR sPathName, BOOL bDeleteNonEmptyDirectory = FALSE); //删除目录
		static BOOL IsDirectoryExist(LPCTSTR sPathName); //目录是否存在
	};
	
	class UILIB_API CUIFileFind
	{
	public:
		CUIFileFind();
		~CUIFileFind();

		BOOL FindFile(LPCTSTR lpFileName);
		BOOL FindNextFile();
		void FindClose();

		//获取找到的文件的名称（包括扩展名）
		CDuiString GetFileName();

		//获取找到的文件的完整路径。 包含路径、文件名、扩展名
		CDuiString GetFilePath();

// 		//获取找到的文件的名称（没有包括扩展名）
// 		CDuiString GetFileTitleName();
// 
// 		//获取找到的文件的扩展名
// 		CDuiString GetFileExtName();

		//确定找到的文件的名称是否具有名称 "." 或 "..."，以指示实际上是一个目录。
		BOOL IsDots();

		//确定找到的文件是否为目录
		BOOL IsDirectory();

	protected:
		void MakeFullPath(CDuiString& sPath);
	private:
		CDuiString m_strRoot;
		BOOL m_bFindInZip;

#ifdef WIN32
		HANDLE m_hFind;
		void* m_pFoundInfo;
		void* m_pNextInfo;
#elif defined __linux__
		DIR* m_dir;
		struct dirent* m_dirFoundInfo;
		struct dirent* m_dirNextInfo;

		struct dirent** m_scandirList;
		int m_scandirCount;
		int m_scandirIterator;
#elif defined __APPLE__
        DIR* m_dir;
        struct dirent* m_dirFoundInfo;
        struct dirent* m_dirNextInfo;

        struct dirent** m_scandirList;
        int m_scandirCount;
        int m_scandirIterator;
#endif
	};
	
} // namespace DuiLib

#endif // __UIFILE_H__
