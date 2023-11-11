#include "StdAfx.h"
#include "App.h"

CApp uiApp;

CApp::CApp(void)
{
	m_hLibCfgmgr32 = NULL;
}


CApp::~CApp(void)
{
}


bool CApp::InitInstance(HINSTANCE hInstance)
{
	if(!__super::InitInstance(hInstance))
		return false;

	//注册插件库
	//DuiPluginsRegister();

	//注册脚本插件
	//DuiScriptRegister();

	REGIST_DUICONTROL(CTreeDeviceUI);

	//语言包路径
	//CLangManagerUI::SetLanguage(_T("Lang\\ChineseSimplified"), _T("chs"));

	m_hLibCfgmgr32 = ::LoadLibrary(_T("Cfgmgr32.dll"));

	//建立主窗口
	m_pMainWnd = new CMainFrame();
	if( m_pMainWnd == NULL ) return false;
	m_pMainWnd->Create(NULL, GetAppName(), UI_WNDSTYLE_FRAME, 0L, 0, 0, 0, 0);
	m_pMainWnd->CenterWindow();

	return true;
}

int CApp::ExitInstance()
{
	return __super::ExitInstance();
}