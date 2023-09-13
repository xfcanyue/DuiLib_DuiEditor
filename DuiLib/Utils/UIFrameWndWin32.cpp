#pragma once
#include "StdAfx.h"

#ifdef DUILIB_WIN32
namespace DuiLib{

CUIFrameWndWin32::CUIFrameWndWin32(void)
{
	
}

CUIFrameWndWin32::~CUIFrameWndWin32(void)
{
	
}

CPaintManagerUI *CUIFrameWndWin32::GetManager()
{
	return &m_pm;
}

void CUIFrameWndWin32::OnFinalMessage( HWND hWnd )
{
	for (int i=0; i<m_listForm.GetSize(); i++)
	{
		CUIForm *pForm = (CUIForm *)m_listForm.GetAt(i);
		delete pForm;
	}
	m_listForm.Empty();

	GetManager()->RemovePreMessageFilter(this);
	GetManager()->RemoveNotifier(this);
	GetManager()->ReapObjects(GetManager()->GetRoot());
}

UIWND CUIFrameWndWin32::Create(UIWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy)
{
	UIWND hWnd = CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy);
	if(hWnd != NULL)
	{
		GetManager()->SetDPI(GetManager()->GetDPIObj()->GetMainMonitorDPI());
	}
	return hWnd;
}

UINT CUIFrameWndWin32::GetClassStyle() const
{
	return CS_DBLCLKS;
}


LRESULT CUIFrameWndWin32::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			{
				LRESULT lResult = ResponseDefaultKeyEvent(wParam);
				if(lResult == S_OK)
				{
					bHandled = true;
					return S_OK;
				}
			}
		default:
			break;
		}
	}
	return S_FALSE;
}

LRESULT CUIFrameWndWin32::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	if (GetManager()->MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIFrameWndWin32::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
		GetManager()->SetDPI(LOWORD(wParam));
		GetManager()->ResetDPIAssets();
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

LRESULT CUIFrameWndWin32::HandleMenuCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CUIFrameWndWin32::Notify(TNotifyUI& msg)
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
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if( sCtrlName == _T("windowmaxbtn"))
		{ 
			::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
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
			CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowrestorebtn")));
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

	CUIFrameWndBase::Notify(msg);
}

LRESULT CUIFrameWndWin32::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

#if defined(WIN32) && !defined(UNDER_CE)
LRESULT CUIFrameWndWin32::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUIFrameWndWin32::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndWin32::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CUIFrameWndWin32::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if (!::IsZoomed(*this))
	{
		RECT rcSizeBox = GetManager()->GetSizeBox();
		if (pt.y < rcClient.top + rcSizeBox.top)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			return HTTOP;
		}
		else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
		{
			if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
		if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
	}

	RECT rcCaption = GetManager()->GetCaptionRect();
	rcCaption = GetManager()->GetDPIObj()->ScaleRect(rcCaption);
	if (-1 == rcCaption.bottom)
	{
		rcCaption.bottom = rcClient.bottom;
	}

	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
	{
		CControlUI* pControl = GetManager()->FindControl(pt);
		if (IsInStaticControl(pControl))
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CUIFrameWndWin32::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO Monitor = {};
	Monitor.cbSize = sizeof(Monitor);
	::GetMonitorInfo(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &Monitor);
	CDuiRect rcWork = Monitor.rcWork;
	if( Monitor.dwFlags != MONITORINFOF_PRIMARY ) {
		rcWork.Offset(-rcWork.left, -rcWork.top);
	}

	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right - rcWork.left;
	lpMMI->ptMaxSize.y = rcWork.bottom - rcWork.top;
	lpMMI->ptMaxTrackSize.x = GetManager()->GetMaxInfo().cx == 0?rcWork.right - rcWork.left:GetManager()->GetMaxInfo().cx;
	lpMMI->ptMaxTrackSize.y = GetManager()->GetMaxInfo().cy == 0?rcWork.bottom - rcWork.top:GetManager()->GetMaxInfo().cy;
	lpMMI->ptMinTrackSize.x = GetManager()->GetMinInfo().cx;
	lpMMI->ptMinTrackSize.y = GetManager()->GetMinInfo().cy;

	bHandled = TRUE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
#endif

LRESULT CUIFrameWndWin32::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = GetManager()->GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
			CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(GetManager()->FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	return lRes;
}

LRESULT CUIFrameWndWin32::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 调整窗口样式
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	// 关联UI管理器
	GetManager()->Init(GetHWND(), GetManagerName(), this);
	// 注册PreMessage回调
	GetManager()->AddPreMessageFilter(this);

	// 创建主窗口
	CControlUI* pRoot=NULL;
	CDialogBuilder builder;
	CDuiString sSkinType = GetSkinType();
	if (!sSkinType.IsEmpty()) {
		STRINGorID xml(_ttoi(GetSkinFile().GetData()));
		pRoot = builder.Create(xml, sSkinType, this, GetManager());
	}
	else {
		pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, GetManager());
	}

	if (pRoot == NULL) {
		CDuiString sError = _T("Loading resource failed :");
		sError += GetSkinFile();
		MessageBox(NULL, sError, _T("Duilib") ,MB_OK|MB_ICONERROR);
		ExitProcess(1);
		return 0;
	}
	GetManager()->AttachDialog(pRoot);
	// 添加Notify事件接口
	GetManager()->AddNotifier(this);

	// 窗口初始化完毕
	__InitWindow();
	return 0;
}

LRESULT CUIFrameWndWin32::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


LRESULT CUIFrameWndWin32::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndWin32::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LONG CUIFrameWndWin32::GetStyle()
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;

	return styleValue;
}

void CUIFrameWndWin32::UIAction(TUIAction *act, bool bAsync)
{
	if(act->action == UIACTION_Close)
	{
		Close(act->wParam);
		return;
	}

	CUIFrameWndBase::UIAction(act, bAsync);
}

} //namespace DuiLib{
#endif //#ifdef DUILIB_WIN32