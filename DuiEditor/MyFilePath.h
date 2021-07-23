#pragma once
class CMyFilePath
{
public:
	CMyFilePath(void);
	~CMyFilePath(void);

	//创建多层目录
	static BOOL CreateMuiltDirectory(LPCTSTR lpszPathName);

	//规范化，整理路径，去掉 ".\" 和 "..\"
	static CString Canonicalize(LPCTSTR lpszPathName);

	//验证路径是否为有效路径。 
	static BOOL IsValidPath(LPCTSTR lpszPathName);

	//文件或目录是否存在
	static BOOL IsFileExist(LPCTSTR lpszPathName);

	//去除文件名，得到目录。 路径末尾包含"\"
	static CString GetPathName(LPCTSTR lpszPathName);

	//去除目录，得到文件名
	static CString GetFileName(LPCTSTR lpszPathName);

	//去除文件路径中的文件扩展名
	static CString RemoveFileExt(LPCTSTR lpszPathName);

	//分割路径，
	//假设 lpszPathFrom = c:/a/b/1.txt, lpszPathTo = c:/a/
	//返回结果为 b/1.txt
	static CString SplitPath(LPCTSTR lpszPathName, LPCTSTR lpszPathBase);
};

