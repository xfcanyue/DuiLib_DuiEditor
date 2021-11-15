#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	//可以从设计器中生成这段代码。
	UI_BINDCONTROL(CEditUI, m_edit_text, _T("edit_text"));
	UI_BINDCONTROL(CRichEditUI, m_redit_text, _T("redit_text"));
	UI_BINDCONTROL(COptionUI, m_opt_check, _T("opt_check"));
	UI_BINDCONTROL(CComboExUI, m_cb_recv_type, _T("cb_recv_type"));

	//指定配置文件中，对应的父节点。
	CXmlNodeUI root = uiApp.Config.GetRoot().child_auto(_T("MainFrame"));

	//载入控件内容。 可以从设计器中生成这段代码。
	UIGetApp()->Config.LoadControl(root, m_edit_text, _T("default_text_1"));
	UIGetApp()->Config.LoadControl(root, m_redit_text, _T("default_text_2"));
	UIGetApp()->Config.LoadControl(root, m_opt_check, TRUE);
	UIGetApp()->Config.LoadControl(root, m_cb_recv_type, _T("AAA"), TRUE);

	//没有数据的话，写入几个做测试
	if(m_cb_recv_type->GetCount() <= 0)
	{
		m_cb_recv_type->AddString(_T("AAA"));
		m_cb_recv_type->AddString(_T("BBB"));
		m_cb_recv_type->AddString(_T("CCC"));
		m_cb_recv_type->AddString(_T("DDD"));
	}
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_CLOSE)
	{
		//指定配置文件中，对应的父节点。
		CXmlNodeUI root = uiApp.Config.GetRoot().child_auto(_T("MainFrame"));

		//保存控件内容。 可以从设计器中生成这段代码。
		UIGetApp()->Config.SaveControl(root, m_edit_text);
		UIGetApp()->Config.SaveControl(root, m_redit_text);
		UIGetApp()->Config.SaveControl(root, m_opt_check);
		UIGetApp()->Config.SaveControl(root, m_cb_recv_type, TRUE);
	}
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}