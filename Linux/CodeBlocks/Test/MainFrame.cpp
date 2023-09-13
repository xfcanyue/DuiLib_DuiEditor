//#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
    CFadeButtonUI *p = static_cast<CFadeButtonUI *>(GetManager()->FindControl(_T("windowmenubtn")));
    CDuiString s = p->GetText();
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

// bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
// {
// 	return false;
// }

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}
