// GridListDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "GridListDemo.h"
#include "MainFrame.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	g_hInstance = hInstance;
	if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))
	{
		return 1;
	}

	//_CrtSetBreakAlloc(392);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	CPaintManagerUI::SetInstance(g_hInstance);
	g_strAppPath = CPaintManagerUI::GetInstancePath();

	CPaintManagerUI::SetResourcePath(g_strAppPath + _T("skin"));
	CPaintManagerUI::LoadPlugin(g_strAppPath + _T("DuiPlugins_ud.dll"));
	DuiPluginsRegister();

	// 创建主窗口
	CMainFrame* pMainWnd = new CMainFrame();
	if( pMainWnd == NULL ) return 0;
	pMainWnd->Create(NULL, _T("DemoGrid"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pMainWnd->CenterWindow();
	// 消息循环
	CPaintManagerUI::MessageLoop();
	// 销毁窗口
	delete pMainWnd;
	pMainWnd = NULL;

	CPaintManagerUI::Term();
	OleUninitialize();
	::CoUninitialize();
	return 0;
}