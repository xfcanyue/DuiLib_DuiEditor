#pragma once

// CThreadTestForm
#include "DuiEditorDoc.h"

extern DWORD g_nThreadTestID;
extern HANDLE g_hThreadTestHandle;
class CThreadTest : public CWinThread
{
	DECLARE_DYNCREATE(CThreadTest)

public:
	CThreadTest();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadTest();

	CDuiEditorDoc *m_pDoc;
	CString m_strSpacialFile;

	PROCESS_INFORMATION m_piProcInfo; 
	STARTUPINFO m_siStartInfo; 

	// 0 = 工具栏运行
	// 1 = 文件树运行
	int m_nTestFrom;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CThreadTest *g_pThreadTest;

