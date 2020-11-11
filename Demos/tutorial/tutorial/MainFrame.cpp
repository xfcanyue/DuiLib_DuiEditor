#include "StdAfx.h"
#include "MainFrame.h"

#include "Dialog1.h"
#include "FormAdvanceControl.h"

CMainFrame::CMainFrame(void)
{
	AttachVirtualForm(new CFormAdvanceControl);
	m_bCheckmenu_cmd_2 = TRUE;
	m_bCheckmenu_cmd_22 = TRUE;
}


CMainFrame::~CMainFrame(void)
{
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	CString className = msg.pSender->GetClass();
	CString ControlName = msg.pSender->GetName();
	CString EventName = msg.sType;

	if(ControlName != _T("msgwnd_frame"))
	{
		CString strText;
		strText.Format(_T("Notify: class='%s' name='%s' msg='%s'"), className, ControlName, EventName);
		InsertMsgUI(strText);
	}

	__super::Notify(msg);
}

void CMainFrame::InitWindow()
{
	//绑定控件指针
	UI_BINDCONTROL(CImageBoxUI, m_pImageBox, _T("imagebox_1"));
	UI_BINDCONTROL(CProgressUI, m_pProgress, _T("progress_1"));

	//进度条
	//m_pProgress->SetTimer(1, 200);

	CDuiString strText;
	strText += _T("\r\n我");
	strText += _T("\r\n我是");
	strText += _T("\r\n我是字");
	strText += _T("\r\n我是字幕");
	strText += _T("\r\n我是字幕，");
	strText += _T("\r\n我是字幕，我");
	strText += _T("\r\n我是字幕，我是");
	strText += _T("\r\n我是字幕，我是字");
	strText += _T("\r\n我是字幕，我是字幕");
	CControlUI *pRoll1 = FindControl(_T("rolltext_1"));
	CControlUI *pRoll2 = FindControl(_T("rolltext_2"));
	CControlUI *pRoll3 = FindControl(_T("rolltext_3"));
	CControlUI *pRoll4 = FindControl(_T("rolltext_4"));
	pRoll1->SetText(pRoll1->GetText() + strText);
	pRoll2->SetText(pRoll2->GetText() + strText);
	pRoll3->SetText(pRoll3->GetText() + strText);
	pRoll4->SetText(pRoll4->GetText() + strText);
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//响应菜单命令
bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	CString str;
	str.Format(_T("你点击了菜单: name=%s, text=%s"), cmd->szName, cmd->szText);
	InsertMsgUI(str);

	if(IsMenuCommand(cmd, _T("menu_cmd_2")))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_2 = !m_bCheckmenu_cmd_2;
	}
	if(IsMenuCommand(cmd, _T("menu_cmd_22")))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_22 = !m_bCheckmenu_cmd_22;
	}
	return false;
}

/*
	//更新菜单状态的另一种方式
*/
bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	if(IsMenuCommand(cmdUI, _T("menu_cmd_22")))
	{
		cmdUI->SetCheck(m_bCheckmenu_cmd_2 == TRUE);
		return true;
	}
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{
	if(IsControl(msg, _T("btn_showhide_top_pane")))
		OnShowHideTopPane();

	//使用UI_COMMAND消息映射
	UI_COMMAND(_T("btn_showhide_bottom_pane"), OnShowHideBottomPane);

	if(IsControl(msg, _T("btn_showhide_left_pane")))
	{
		CAnimationPaneUI *pPane;
		UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_3"));
		pPane->SetPaneVisible(!pPane->IsPaneVisible());
	}
	
	if(IsControl(msg, _T("btn_showhide_right_pane")))
	{
		CAnimationPaneUI *pPane;
		UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_4"));
		pPane->SetPaneVisible(!pPane->IsPaneVisible());
	}

	if(IsControl(msg, _T("btn_domodal")))
	{
		CDialog1 dlg;
		dlg.DoModal(this);
	}

	if(IsControl(msg, _T("btn_show_dialog")))
	{
		CDialog1 *pDlg = new CDialog1;
		pDlg->ShowDialog(this);
	}

	if(IsControl(msg, _T("windowmenubtn")))
	{
		//设置菜单选中信息
		CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(&m_MenuInfos);

		//设置状态
		CMenuWnd::SetMenuItemInfo(_T("menu_cmd_2"), m_bCheckmenu_cmd_2 == TRUE);

		CreateMenu(_T("Menu.xml"));
	}
}

void CMainFrame::OnNotifyTimer(TNotifyUI& msg)
{
	if(msg.pSender == m_pProgress)
	{
		int nNewValue = m_pProgress->GetValue() + 1;
		if(nNewValue > m_pProgress->GetMaxValue()) nNewValue = m_pProgress->GetMinValue();
		m_pProgress->SetValue(nNewValue);
	}
}

void CMainFrame::OnShowHideTopPane()
{
	CAnimationPaneUI *pPane;
	UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_1"));
	pPane->SetPaneVisible(!pPane->IsPaneVisible());
}

void CMainFrame::OnShowHideBottomPane()
{
	CAnimationPaneUI *pPane;
	UI_BINDCONTROL(CAnimationPaneUI, pPane, _T("pane_2"));
	pPane->SetPaneVisible(!pPane->IsPaneVisible());
}