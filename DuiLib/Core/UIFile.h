#ifndef __UIFILE_H__
#define __UIFILE_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CUIFile
	{
	public:
		CUIFile();
		~CUIFile();

		LPBYTE GetData() const;
		DWORD  GetSize() const;
		void Empty();

		BOOL LoadFile(const STRINGorID &bitmap, LPCTSTR type=NULL, HINSTANCE instance=NULL);
		BOOL LoadFile(LPCTSTR pStrImage, LPCTSTR type=NULL, HINSTANCE instance=NULL);
		BOOL LoadFile(UINT nID, LPCTSTR type=NULL, HINSTANCE instance=NULL);

	private:
		LPBYTE m_pData; 
		DWORD m_dwSize;
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

		HANDLE m_hFind;
		void* m_pFoundInfo;
		void* m_pNextInfo;

		HANDLE m_hZip;
		ZIPENTRY m_zipEntry;
		ZIPENTRY m_zipEntryNext;
		int m_nZipIndex;
	};

} // namespace DuiLib

#endif // __UIFILE_H__
