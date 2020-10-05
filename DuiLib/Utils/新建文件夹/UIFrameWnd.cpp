#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

//namespace DuiLib{

CUIFrameWnd::CUIFrameWnd(void)
{
	
}

CUIFrameWnd::~CUIFrameWnd(void)
{
}

void CUIFrameWnd::OnFinalMessage( HWND hWnd )
{
// 	std::list<CUIForm *>::iterator it;
// 	for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
// 	{
// 		CUIForm *pForm = (CUIForm *)(*it);
// 		RemoveVirtualWnd(pForm->GetWindowClassName());
// 	}
// 	m_listForm.clear();

	__super::OnFinalMessage(hWnd);	
}


CMenuWnd *CUIFrameWnd::CreateMenu(STRINGorID xml)
{
	CDuiPoint pt2;
	::GetCursorPos(&pt2);
	CMenuWnd *pMenuWnd = CMenuWnd::CreateMenu(NULL, xml, pt2,  GetManager());
	if(pMenuWnd) pMenuWnd->ResizeMenu();
	return pMenuWnd;
}

void CUIFrameWnd::AttachForm(CUIForm *pForm)
{
	//if(!AddVirtualWnd(pForm->GetWindowClassName(), pForm))
	//	return;

	std::list<CUIForm *>::iterator it;
	for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
	{
		if(*it == pForm)
			return;
	}

	pForm->SetFrameWnd(this);
	m_listForm.push_back(pForm);
}

void CUIFrameWnd::DetachForm(CUIForm *pForm)
{
	//RemoveVirtualWnd(pForm->GetWindowClassName());

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

void CUIFrameWnd::ShowFullScreen() //全屏显示
{
	int screenX = ::GetSystemMetrics(SM_CXSCREEN);//获取整个屏幕右下角X坐标   
	int screenY = ::GetSystemMetrics(SM_CYSCREEN);//屏幕Y坐标 
	::SetWindowPos(GetHWND(), HWND_TOPMOST, 0, 0, screenX, screenY, SWP_SHOWWINDOW);
}

LRESULT CUIFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int lRet = __super::OnCreate(uMsg, wParam, lParam, bHandled);

	std::list<CUIForm *>::iterator it;
	for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
	{
		(*it)->InitWindow();
	}

	return lRet;
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
	if(OnCustomMessage(uMsg, wParam, lParam))
	{
		bHandled = TRUE;
		return 0;
	}

	std::list<CUIForm *>::iterator it;
	for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
	{
		if((*it)->OnCustomMessage(uMsg, wParam, lParam))
		{
			bHandled = TRUE;
			return 0; 
		}
	}

	return 0;
}

LRESULT CUIFrameWnd::HandleMenuCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_MENUCLICK)
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

			delete pMenuCmd;
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

	std::list<CUIForm *>::iterator it;
	for (it=m_listForm.begin(); it!=m_listForm.end(); it++)
	{
		(*it)->Notify(msg);
	}

	__super::Notify(msg);
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

DUI_BEGIN_MESSAGE_MAP(CUIFrameWnd,CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,			OnNotifyClick)			//(_T("click"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RCLICK,			OnNotifyRClick)			//(_T("rclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DBCLICK,			OnNotifyDbClick)		//(_T("dbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MENU,			OnNotifyMenu)			//(_T("menu"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_LINK,			OnNotifyLink)			//(_T("link"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TIMER,			OnNotifyTimer)			//(_T("timer"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN,			OnNotifyReturn)			//(_T("return"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SCROLL,			OnNotifyScroll)			//(_T("scroll"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_DROPDOWN,		OnNotifyDropDown)		//(_T("dropdown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SETFOCUS,		OnNotifySetFocus)		//(_T("setfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_KILLFOCUS,		OnNotifyKillFocus)		//(_T("killfocus"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,		OnNotifyItemClick)		//(_T("itemclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMRCLICK,		OnNotifyItemRClick)		//(_T("itemrclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TABSELECT,		OnNotifyTabSelect)		//(_T("tabselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,		OnNotifyItemSelect)		//(_T("itemselect"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMEXPAND,		OnNotifyItemExpand)		//(_T("itemexpand"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWINIT,		OnNotifyWindowPrepare)		//(_T("windowinit"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_BUTTONDOWN,		OnNotifyButtonDown)		//(_T("buttondown"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSEENTER,		OnNotifyMouseEnter)		//(_T("mouseenter"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_MOUSELEAVE,		OnNotifyMouseLeave)		//(_T("mouseleave"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_TEXTCHANGED,		OnNotifyTextChanged)	//(_T("textchanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_HEADERCLICK,		OnNotifyHeaderClick)	//(_T("headerclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMDBCLICK,		OnNotifyItemDBClick)	//(_T("itemdbclick"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SHOWACTIVEX,		OnNotifyShowActivex)	//(_T("showactivex"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCOLLAPSE,	OnNotifyItemCollapse)	//(_T("itemcollapse"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE,	OnNotifyItemActivate)	//(_T("itemactivate"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED,	OnNotifyValueChanged)	//(_T("valuechanged"))
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,	OnNotifySelectChanged)	//(_T("selectchanged"))

	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_STARTEDIT,	OnNotityGridStartEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_ENDEDIT,	OnNotityGridEndEdit)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_STARTSELCHANGE,	OnNotityGridStartSelChange)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_ENDSELCHANGE,	OnNotityGridEndSelChange)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_DROPDOWNLIST,	OnNotityGridDropDownList)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_GRID_GETDISPINFO,	OnNotityGridGetDispInfo)
DUI_END_MESSAGE_MAP()






//}