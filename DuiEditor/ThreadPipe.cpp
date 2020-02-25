// ThreadPipe.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ThreadPipe.h"


HANDLE hChildStdoutRd = NULL; 
HANDLE hChildStdoutWr = NULL; 
HANDLE g_hSaveStdout = NULL; 
// CThreadPipe
DWORD g_nThreadPipeID = 0;
HANDLE g_hThreadPipeHandle = NULL;

IMPLEMENT_DYNCREATE(CThreadPipe, CWinThread)

CThreadPipe::CThreadPipe()
{
}

CThreadPipe::~CThreadPipe()
{
}

BOOL CThreadPipe::InitInstance()
{
	g_nThreadPipeID = m_nThreadID;
	g_hThreadPipeHandle = m_hThread;

	DWORD dwRead = 0; 
	TCHAR chBuf[4096]; 

	for (;;)
	{
		ZeroMemory(chBuf, 4096 * sizeof(TCHAR));
		if (!ReadFile(hChildStdoutRd, chBuf, 4095, &dwRead, NULL) || dwRead == 0) 
			break;

		if(dwRead > 0)
			InsertMsg(chBuf);
	}
	return FALSE;
}

int CThreadPipe::ExitInstance()
{
	g_nThreadPipeID = 0;
	g_hThreadPipeHandle = NULL;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadPipe, CWinThread)
END_MESSAGE_MAP()


// CThreadPipe 消息处理程序
void CThreadPipe::CreatePipe()
{
	SECURITY_ATTRIBUTES saAttr; 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
	//创建输出管道 Create a pipe for the child process's STDOUT. 
	::CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0);
	//重定向输出
	SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr);

	//创建PIPE接收线程
	CThreadPipe *pThread = new CThreadPipe;
	pThread->CreateThread();
}

void CThreadPipe::ClosePipe()
{
	if(hChildStdoutWr!=NULL) { CloseHandle(hChildStdoutWr); hChildStdoutWr=NULL; } //必须先关闭写通道
	if(hChildStdoutRd!=NULL) { CloseHandle(hChildStdoutRd); hChildStdoutRd=NULL; }
	SetStdHandle(STD_OUTPUT_HANDLE, g_hSaveStdout);

	if(g_nThreadPipeID != 0)
	{
		::PostThreadMessage(g_nThreadPipeID, WM_QUIT, 0, 0);
		WaitForSingleObject(g_hThreadPipeHandle,INFINITE);
	}
}