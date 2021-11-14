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
	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
				delete (CUIForm *)(*it);
		}
		m_listForm.clear();
	}

	__super::OnFinalMessage(hWnd);	
}

void CUIFrameWnd::AttachVirtualForm(CUIForm *pForm)
{
	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
			if(*it == pForm)
				return;
		}
	}

	pForm->SetFrameWnd(this);
	m_listForm.push_back(pForm);
}

void CUIFrameWnd::DetachVirtualForm(CUIForm *pForm)
{
	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
			if(*it == pForm)
			{
				m_listForm.erase(it);
				return;
			}
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

	if(OnCustomMessage(uMsg, wParam, lParam))
	{
		bHandled = TRUE;
		return 0;
	}

	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
			if((*it)->OnCustomMessage(uMsg, wParam, lParam))
			{
				bHandled = TRUE;
				return 0; 
			}
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

			if(m_listForm.size() > 0)
			{
				std::list<CUIForm *>::iterator it;
				for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
				{
					if((*it)->OnMenuCommand(pMenuCmd))
					{
						delete pMenuCmd;
						bHandled = TRUE;
						return 0; 
					}
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

			if(m_listForm.size() > 0)
			{
				std::list<CUIForm *>::iterator it;
				for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
				{
					if((*it)->OnMenuUpdateCommandUI(pCmdUI))
					{
						bHandled = TRUE;
						return 1; 
					}
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
		if(m_listForm.size() > 0)
		{
			std::list<CUIForm *>::iterator it;
			for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
			{
				CUIForm *pForm = (CUIForm *)(*it);
				if(pForm->IsForm(msg.pSender->GetName()))
				{
					pForm->OnActiveForm();
					return;
				}
			}
		}
	}

	else if(msg.sType == DUI_MSGTYPE_TABNOACTIVEFORM)
	{
		if(m_listForm.size() > 0)
		{
			std::list<CUIForm *>::iterator it;
			for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
			{
				CUIForm *pForm = (CUIForm *)(*it);
				if(pForm->IsForm(msg.pSender->GetName()))
				{
					pForm->OnHideForm();
					return;
				}
			}
		}
	}

	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
#ifdef _DEBUG
			CUIForm *pForm = (CUIForm *)(*it);
			pForm->Notify(msg);
#else
			(*it)->Notify(msg);
#endif
		}
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

	if(m_listForm.size() > 0)
	{
		std::list<CUIForm *>::iterator it;
		for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
		{
#ifdef _DEBUG
			CUIForm *pForm = (CUIForm *)(*it);
			pForm->__InitWindow();
			pForm->InitWindow();
#else
			(*it)->__InitWindow();
			(*it)->InitWindow();
#endif
		}
	}
	

	InitWindow();
}

} //namespace DuiLib{