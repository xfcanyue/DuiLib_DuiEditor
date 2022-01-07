#include "StdAfx.h"
#include "App.h"

CApp uiApp;

CApp::CApp(void)
{
}


CApp::~CApp(void)
{
}


bool CApp::InitInstance(HINSTANCE hInstance)
{
	if(!__super::InitInstance(hInstance))
		return false;

	//载入配置文件，如果配置文件不存在，保存时建立新的。
	//默认是程序目录下的 AppName.xml
	//szPathName不为空的话，需要指定路径和文件
	Config.LoadConfig();

	int x = sizeof(CControlUI);

	//建立主窗口
	m_pMainWnd = new CMainFrame();
	if( m_pMainWnd == NULL ) return false;
	m_pMainWnd->Create(NULL, GetAppName(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	m_pMainWnd->CenterWindow();

	return true;
}

int CApp::ExitInstance()
{
	//保存配置文件
	Config.SaveConfig();
	return __super::ExitInstance();
}