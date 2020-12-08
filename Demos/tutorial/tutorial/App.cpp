#include "StdAfx.h"
#include "App.h"
#include "MainFrame.h"

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

	//注册插件库
	DuiPluginsRegister();

	//语言包路径
	CLangManagerUI::SetLanguage(_T("Lang\\ChineseSimplified"), _T("chs"));

	//建立主窗口
	m_pMainWnd = new CMainFrame();
	if( m_pMainWnd == NULL ) return 0;
	m_pMainWnd->Create(NULL, GetAppName(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	m_pMainWnd->CenterWindow();

	return true;
}

int CApp::ExitInstance()
{
	return __super::ExitInstance();
}