#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

namespace DuiLib{

CUIFrameWnd::CUIFrameWnd(void) : m_pApplication(NULL)
{
	
}

CUIFrameWnd::~CUIFrameWnd(void)
{
	
}

void CUIFrameWnd::OnFinalMessage( HWND hWnd )
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		delete pForm;
	}
	m_listForm.Empty();

	__super::OnFinalMessage(hWnd);	
}

void CUIFrameWnd::AttachVirtualForm(CUIForm *pForm)
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm1 = (CUIForm *)m_listForm.GetAt(i);
		if(pForm1 == pForm)
			return;
	}

	pForm->SetFrameWnd(this);
	m_listForm.Add(pForm);
}

void CUIFrameWnd::DetachVirtualForm(CUIForm *pForm)
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm1 = (CUIForm *)m_listForm.GetAt(i);
		if(pForm1 == pForm)
		{
			m_listForm.Remove(i);
			return;
		}
	}
}

void CUIFrameWnd::ShowFullScreen() //全屏显示
{
	int screenX = ::GetSystemMetrics(SM_CXSCREEN);//获取整个屏幕右下角X坐标   
	int screenY = ::GetSystemMetrics(SM_CYSCREEN);//屏幕Y坐标 
	MoveWindow(GetHWND(), 0, 0, screenX, screenY, TRUE);
	//::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, screenX, screenY, SWP_SHOWWINDOW);
}

LRESULT CUIFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:			lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:			lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:		lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:		lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:		lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO:	lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:		lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
	case WM_SIZE:			
		lRes = OnSize(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_CHAR:		lRes = OnChar(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:		lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:		lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_SETFOCUS:		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_RBUTTONUP:		lRes = OnRButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_RBUTTONDOWN:	lRes = OnRButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEHOVER:	lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	if(m_pApplication && uMsg == m_pApplication->m_UIAPP_SINGLEAPPLICATION_MSG)
	{
		::ShowWindow(GetHWND(), SW_SHOW);
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		::SetForegroundWindow(GetHWND());
		return 1;
	}

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	//菜单命令
	lRes = HandleMenuCommandMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == UIMSG_INSERT_MSG)
	{
		bHandled = TRUE;
		CMsgWndUI *pMsgWindow = (CMsgWndUI *)wParam;
		GetManager()->SendNotify(pMsgWindow, _T("CMsgWndUI::InsertMsg"), wParam, lParam);
		return 0;
	}
	else if (uMsg == UIMSG_GRID_NOTIFY)
	{
		bHandled = TRUE;
		CGridUI* pGrid = (CGridUI*)wParam;
		GetManager()->SendNotify(pGrid, _T("CGridUI::OnGridNotify"), wParam, lParam);
		return 0;
	}

	if(uMsg == UIMSG_CREATE_MENU)
	{
		CDuiString *pstring = (CDuiString *)wParam;
		CreateMenu(pstring->GetData());
		delete pstring;
		return 0;
	}

	if(uMsg == UIMSG_CONTROL_ACTION)
	{
		TUIAction *act = (TUIAction *)wParam;
		ASSERT(act);
		UIAction(act, false);
		return 0;
	}

	if(uMsg == UIMSG_CONTROL_ACTION_ASYNC)
	{
		TUIAction *act = (TUIAction *)wParam;
		ASSERT(act);
		UIAction(act, true);
		delete act;
	}

	//当你的窗口移动到DPI不同的显示器上时，会收到 WM_DPICHANGED 消息。
	//直接修改当前显示设置改动dpi，不会收到此消息
	if (uMsg == WM_DPICHANGED)
	{
		//wParam 的 HIWORD 包含窗口的新 dpi 的 Y 轴值。wParam 的 LOWORD 包含窗口的新 DPI 的 X 轴值。
		//例如，96、120、144 或 192。对于 Windows 应用，X 轴和 Y 轴的值是相同的。
		m_pm.SetDPI(LOWORD(wParam));
		m_pm.ResetDPIAssets();
		bHandled = TRUE;
		return 0;
	}

	if(OnCustomMessage(uMsg, wParam, lParam))
	{
		bHandled = TRUE;
		return 0;
	}

	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		if(pForm->OnCustomMessage(uMsg, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}
	}

	return 0;
}

LRESULT CUIFrameWnd::HandleMenuCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == UIMSG_MENUCLICK)
	{
		bHandled = TRUE;

		MenuCmd* pMenuCmd = (MenuCmd*)wParam;
		if(pMenuCmd)
		{
			if(OnMenuCommand(pMenuCmd))
			{
				delete pMenuCmd;
				bHandled = TRUE;
				return 0;
			}

			for (int i=0; i<m_listForm.GetSize(); i++)
			{
				CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
				if(pForm->OnMenuCommand(pMenuCmd))
				{
					delete pMenuCmd;
					bHandled = TRUE;
					return 0; 
				}
			}

			delete pMenuCmd;
			return 0;
		}
	}
	else if(uMsg == UIMSG_MENU_UPDATE_COMMAND_UI)
	{
		bHandled = TRUE;

		CMenuCmdUI* pCmdUI = (CMenuCmdUI *)wParam;
		if(pCmdUI)
		{
			if(OnMenuUpdateCommandUI(pCmdUI))
			{
				bHandled = TRUE;
				return 1;
			}

			for (int i=0; i<m_listForm.GetSize(); i++)
			{
				CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
				if(pForm->OnMenuUpdateCommandUI(pCmdUI))
				{
					bHandled = TRUE;
					return 1; 
				}
			}

			return 0;
		}
	}

	return 0;
}

LRESULT CUIFrameWnd::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWnd::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CUIFrameWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString sCtrlName = msg.pSender->GetName();
		if( sCtrlName == _T("windowclosebtn") )
		{
			Close(IDCANCEL);
		}
		else if( sCtrlName == _T("windowminbtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if( sCtrlName == _T("windowmaxbtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		}
		else if( sCtrlName == _T("windowrestorebtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		}
	}

	else if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{
#if defined(WIN32) && !defined(UNDER_CE)
		if( ::IsZoomed(*this) ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}	
#endif
	}
	
	else if(msg.sType == DUI_MSGTYPE_TABACTIVEFORM)
	{
		for (int i=0; i<m_listForm.GetSize(); i++)
		{
			CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
			if(pForm->IsForm(msg.pSender->GetName()))
			{
				pForm->OnActiveForm();
				return;
			}
		}
	}

	else if(msg.sType == DUI_MSGTYPE_TABNOACTIVEFORM)
	{
		for (int i=0; i<m_listForm.GetSize(); i++)
		{
			CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
			if(pForm->IsForm(msg.pSender->GetName()))
			{
				pForm->OnHideForm();
				return;
			}
		}
	}

	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		pForm->Notify(msg);
	}

	CUIFrmBase::Notify(msg);
}

LRESULT CUIFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	return lRes;
}

void CUIFrameWnd::__InitWindow()
{
	__super::__InitWindow();

	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		pForm->__InitWindow();
		pForm->InitWindow();
	}
	
	InitWindow();
}

void CUIFrameWnd::UIAction(TUIAction *act, bool bAsync)
{
	if(act->action == UIACTION_Close)
	{
		Close(act->wParam);
		return;
	}

	CControlUI *pControl = GetManager()->FindControl(act->sControlName);
	if(!pControl) return;

	if (act->action == UIACTION_SetText)
	{
		pControl->SetText((LPCTSTR)act->wParam);
		return;
	}

	if (act->action == UIACTION_SetValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetValue(act->wParam);
		}
		return;
	}

	if (act->action == UIACTION_SetMinValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetMinValue(act->wParam);
		}
		return;
	}

	if (act->action == UIACTION_SetMaxValue)
	{
		CProgressUI *pProgress = dynamic_cast<CProgressUI *>(pControl);
		if(pProgress)
		{
			pProgress->SetMaxValue(act->wParam);
		}
		return;
	}
}

} //namespace DuiLib{