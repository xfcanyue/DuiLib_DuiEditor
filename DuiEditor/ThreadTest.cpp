// ThreadTestForm.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ThreadTest.h"

#include "ThreadPipe.h"
// CThreadTestForm

DWORD g_nThreadTestID = 0;
HANDLE g_hThreadTestHandle = NULL;
CThreadTest *g_pThreadTest = NULL;

IMPLEMENT_DYNCREATE(CThreadTest, CWinThread)

CThreadTest::CThreadTest()
{
}

CThreadTest::~CThreadTest()
{
}

BOOL CThreadTest::InitInstance()
{
	g_nThreadTestID = m_nThreadID;
	g_hThreadTestHandle = m_hThread;

	ZeroMemory( &m_piProcInfo, sizeof(PROCESS_INFORMATION) );
	ZeroMemory( &m_siStartInfo, sizeof(STARTUPINFO) );

	m_siStartInfo.cb = sizeof(STARTUPINFO); 
	m_siStartInfo.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;//STARTF_USESHOWWINDOW;
	m_siStartInfo.hStdOutput = hChildStdoutWr;//GetStdHandle(STD_OUTPUT_HANDLE);
	m_siStartInfo.wShowWindow=SW_SHOW;

	CString strPath, strFileName;
	strPath = g_proj.child(_T("Project")).attribute(_T("path")).as_string();
	strFileName = g_proj.child(_T("Project")).child(_T("Startup")).attribute(_T("file")).as_string();
	if(strPath.IsEmpty() || strFileName.IsEmpty())
	{
		strPath = m_pDoc->GetSkinPath();
		strFileName = m_pDoc->GetSkinFileName();
	}

	CString strCmd;
	if(!m_strSpacialFile.IsEmpty())
	{
		strCmd.Format(_T("%sDuiPreviewer.exe \"-f %s\" \"-o %d\""), 
			g_strAppPath, m_strSpacialFile, hChildStdoutWr);
	}
	else if(g_strAttachTestCommand.IsEmpty())
	{
		strCmd.Format(_T("%sDuiPreviewer.exe \"-f %s%s\" \"-o %d\""), 
			g_strAppPath, strPath, strFileName, hChildStdoutWr);
	}
	else
	{
		strCmd = g_strAttachTestCommand;
	}

	InsertMsg(_T("start duilib process."));

	BOOL bCreate = CreateProcess(NULL, 
		(LPTSTR)(LPCTSTR)strCmd, // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&m_siStartInfo,  // STARTUPINFO pointer 
		&m_piProcInfo);  // receives PROCESS_INFORMATION
	if(!bCreate)
	{
		InsertMsg(_T("启动测试窗体进程失败"));
		return FALSE;
	}

	WaitForSingleObject(m_piProcInfo.hProcess,INFINITE);  

	DWORD dwCode = 0;
	BOOL bRet = ::GetExitCodeProcess(m_piProcInfo.hProcess, &dwCode);

	CloseHandle(m_piProcInfo.hThread);
	CloseHandle(m_piProcInfo.hProcess);

	InsertMsg(_T("Exit duilib process."));
	return FALSE;
}

int CThreadTest::ExitInstance()
{
	g_nThreadTestID = 0;
	g_hThreadTestHandle = NULL;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadTest, CWinThread)
END_MESSAGE_MAP()


// CThreadTestForm 消息处理程序