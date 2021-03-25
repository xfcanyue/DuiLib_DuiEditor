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
	m_nTestFrom = 0;
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

	CString strAppName;
#ifdef UILIB_STATIC
#	ifdef _UNICODE
#		ifdef _DEBUG
			strAppName = _T("DuiPreviewer_Sd.exe");	
#		else
			strAppName = _T("DuiPreviewer.exe");	
#		endif
#	else
#		ifdef _DEBUG
			strAppName = _T("DuiPreviewer_ASd.exe");	
#		else
			strAppName = _T("DuiPreviewer_AS.exe");	
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
			strAppName = _T("DuiPreviewerd.exe");	
#		else
			strAppName = _T("DuiPreviewer.exe");	
#		endif
#	else
#		ifdef _DEBUG
			strAppName = _T("DuiPreviewer_Ad.exe");	
#		else
			strAppName = _T("DuiPreviewer_A.exe");	
#		endif
#	endif
#endif

	CString strCmd;
	if(m_nTestFrom == 0)
	{
		CString strPath, strFileName;
		strPath = g_proj.GetProjectPath();
		strFileName = g_proj.GetStartupFile();
		if(strPath.IsEmpty() || strFileName.IsEmpty())
		{
			strPath = m_pDoc->GetSkinPath();
			strFileName = m_pDoc->GetSkinFileName();
		}
		strCmd.Format(_T("%s%s \"-f %s%s\" \"-o %d\""), 
			g_strAppPath, strAppName, strPath, strFileName, hChildStdoutWr);
	}
	else if(m_nTestFrom == 1)
	{
		strCmd.Format(_T("%s%s \"-f %s\" \"-o %d\""), 
			g_strAppPath, strAppName, m_strSpacialFile, hChildStdoutWr);
	}
	else
	{
		strCmd = g_cfg.strAttachTestCommand;
	}

	InsertMsg(_T("command: " + strCmd));
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