#include "StdAfx.h"
#include "MyFilePath.h"


CMyFilePath::CMyFilePath(void)
{
}


CMyFilePath::~CMyFilePath(void)
{
}

BOOL CMyFilePath::CreateMuiltDirectory(LPCTSTR lpszPathName)
{
	LPCTSTR pstr = lpszPathName;

	CString path;
	while( *pstr != _T('\0') )
	{
		path += *pstr;
		if(*pstr == _T('\\'))
		{
			if(!PathIsDirectory(path))
			{
				if(!CreateDirectory(path, NULL))
					return FALSE;
			}
		}

		pstr++;
	}

	if(!PathIsDirectory(path))
	{
		if(!CreateDirectory(path, NULL))
			return FALSE;
	}
	return TRUE;
}

CString CMyFilePath::Canonicalize(LPCTSTR lpszPathName)
{
	CString strPath;
	TCHAR newPath[MAX_PATH];
	memset(newPath, 0, MAX_PATH * sizeof(TCHAR));
	BOOL bRet = PathCanonicalize(newPath, lpszPathName);
	if(bRet)
	{
		PathAddBackslash(newPath);
		strPath = LSW2T(newPath);
	}
	return strPath;
}

BOOL CMyFilePath::IsValidPath(LPCTSTR lpszPathName)
{
	return PathIsDirectory(lpszPathName);
}

BOOL CMyFilePath::IsFileExist(LPCTSTR lpszPathName)
{
	return PathFileExists(lpszPathName);
}

CString CMyFilePath::GetPathName(LPCTSTR lpszPathName)
{
	CString strPath;
	//PathCchRemoveFileSpec win8以上支持
	TCHAR newPath[MAX_PATH];
	_tcscpy_s(newPath, MAX_PATH, lpszPathName);
	if(PathRemoveFileSpec(newPath))
	{
		PathAddBackslash(newPath);
		strPath = newPath;
	}
	return strPath;
}

CString CMyFilePath::GetFileName(LPCTSTR lpszPathName)
{
	CString strFile;
	TCHAR newPath[MAX_PATH];
	memset(newPath, 0, MAX_PATH * sizeof(TCHAR));
	_tcscpy_s(newPath, MAX_PATH, lpszPathName);
	PathStripPath(newPath);
	strFile = newPath;
	return strFile;
}

CString CMyFilePath::RemoveFileExt(LPCTSTR lpszPathName)
{
	CString strExt;
	TCHAR newPath[MAX_PATH];
	memset(newPath, 0, MAX_PATH * sizeof(TCHAR));
	_tcscpy_s(newPath, MAX_PATH, lpszPathName);
	PathRemoveExtension(newPath);
	strExt = newPath;
	return strExt;
}

CString CMyFilePath::SplitPath(LPCTSTR lpszPathName, LPCTSTR lpszPathBase)
{
	CString strNewPath;
	CString PathName = lpszPathName;
	CString PathBase = lpszPathBase;

	if(PathName.Find(PathBase) < 0)
		return strNewPath;

	strNewPath = PathName.Right(PathName.GetLength() - PathBase.GetLength());
	return strNewPath;
}