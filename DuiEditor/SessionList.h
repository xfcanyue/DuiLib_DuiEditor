#pragma once

//CSessionList, 管理缓存文件
class CSessionList
{
public:
	CSessionList(void);
	~CSessionList(void);

	void Init();
	BOOL Save();

	//获取File节点
	xml_node GetSession(LPCTSTR lpszPathName);

	//返回真正需要打开文件的路径
	CString GetSessionFile(LPCTSTR lpszPathName);

	//返回backup属性
	CString GetSessionBackup(LPCTSTR lpszPathName);

	//删除File节点
	void DeleteSession(LPCTSTR lpszPathName);

	//打开文件
	void OpenSession(LPCTSTR lpszPathName);

	//正常关闭文件
	void CloseSession(LPCTSTR lpszPathName);

	void ProcessSessionFileList();
private:
	xml_document xml;
};

extern CSessionList g_session;