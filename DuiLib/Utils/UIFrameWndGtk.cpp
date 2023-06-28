#include "StdAfx.h"

#ifdef DUILIB_GTK
namespace DuiLib{

CUIFrameWndGtk::CUIFrameWndGtk(void)
{
	
}

CUIFrameWndGtk::~CUIFrameWndGtk(void)
{
	
}

CPaintManagerUI *CUIFrameWndGtk::GetManager()
{
	return &m_pm;
}


void CUIFrameWndGtk::OnFinalMessage(UIWND hWnd)
{
	for (int i = 0; i < m_listForm.GetSize(); i++)
	{
		CUIForm* pForm = (CUIForm*)m_listForm.GetAt(i);
		delete pForm;
	}
	m_listForm.Empty();

	GetManager()->RemovePreMessageFilter(this);
	GetManager()->RemoveNotifier(this);
	GetManager()->ReapObjects(GetManager()->GetRoot());
}


LRESULT CUIFrameWndGtk::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& bHandled)
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

LRESULT CUIFrameWndGtk::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	if (GetManager()->MessageHandler(uMsg, wParam, lParam, lRes))
		return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIFrameWndGtk::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == UIMSG_INSERT_MSG)
	{
#ifdef DUILIB_WIN32
		bHandled = TRUE;
		CMsgWndUI *pMsgWindow = (CMsgWndUI *)wParam;
		GetManager()->SendNotify(pMsgWindow, _T("CMsgWndUI::InsertMsg"), wParam, lParam);
#endif
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

	//当你的窗口移动到DPI不同的显示器上时，会收到 WM_DPICHANGED 消息。
	//直接修改当前显示设置改动dpi，不会收到此消息
// 	if (uMsg == WM_DPICHANGED)
// 	{
// 		//wParam 的 HIWORD 包含窗口的新 dpi 的 Y 轴值。wParam 的 LOWORD 包含窗口的新 DPI 的 X 轴值。
// 		//例如，96、120、144 或 192。对于 Windows 应用，X 轴和 Y 轴的值是相同的。
// 		GetManager()->SetDPI(LOWORD(wParam));
// 		GetManager()->ResetDPIAssets();
// 		bHandled = TRUE;
// 		return 0;
// 	}

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

LRESULT CUIFrameWndGtk::HandleMenuCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CUIFrameWndGtk::Notify(TNotifyUI& msg)
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
			//SetWindowMinimize();
		}
		else if( sCtrlName == _T("windowmaxbtn"))
		{ 
			//SetWindowMaximized();
		}
		else if( sCtrlName == _T("windowrestorebtn"))
		{ 
			//SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		}
	}

	else if(msg.sType == DUI_MSGTYPE_WINDOWINIT)
	{

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

LRESULT CUIFrameWndGtk::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUIFrameWndGtk::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIFrameWndGtk::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 调整窗口样式
// 	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
// 	styleValue &= ~WS_CAPTION;
// 	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

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
		pRoot = builder.Create(GetSkinFile().GetData(), NULL, this, GetManager());
	}

	if (pRoot == NULL) 
	{
#ifdef WIN32
		CDuiString sError = _T("Loading resource failed :");
		sError += GetSkinFile();
		MessageBox(NULL, sError, _T("Duilib"), MB_OK | MB_ICONERROR);
		ExitProcess(1);
#endif
		return 0;
	}
	GetManager()->AttachDialog(pRoot);
	// 添加Notify事件接口
	GetManager()->AddNotifier(this);

	// 窗口初始化完毕
	__InitWindow();
	return 0;
}

LRESULT CUIFrameWndGtk::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GdkEventButton *ev = (GdkEventButton *)wParam;

	CDuiPoint ptEvent(ev->x, ev->y);

	CDuiRect rcClient;
	CPlatform::GetClientRect(GetHWND(), &rcClient);

	//改变窗口大小
	int sep = 4;
	CDuiRect rcLeft(rcClient.left-sep, rcClient.top, rcClient.left+sep, rcClient.bottom);
	CDuiRect rcTop(rcClient.left, rcClient.top-sep, rcClient.right, rcClient.top+sep);
	CDuiRect rcRight(rcClient.right-sep, rcClient.top, rcClient.right+sep, rcClient.bottom);
	CDuiRect rcBottom(rcClient.left, rcClient.bottom-sep, rcClient.right, rcClient.bottom+sep);
	CDuiRect rcLeftTop(rcClient.left-sep, rcClient.top-sep, rcClient.left+sep, rcClient.top+sep);
	CDuiRect rcLeftBottom(rcClient.left-sep, rcClient.bottom-sep, rcClient.left+sep, rcClient.bottom+sep);
	CDuiRect rcRightTop(rcClient.right-sep, rcClient.top-sep, rcClient.right+sep, rcClient.top+sep);
	CDuiRect rcRightBottom(rcClient.right-sep, rcClient.bottom-sep, rcClient.right+sep, rcClient.bottom+sep);

	if(rcLeft.PtInRect(ptEvent) || rcTop.PtInRect(ptEvent) || 
		rcRight.PtInRect(ptEvent) || rcBottom.PtInRect(ptEvent) ||
		rcLeftTop.PtInRect(ptEvent) || rcLeftBottom.PtInRect(ptEvent) ||
		rcRightTop.PtInRect(ptEvent) || rcRightBottom.PtInRect(ptEvent))
	{
		GdkWindowEdge gdkWindowEdge;

		if(rcLeft.PtInRect(ptEvent))		gdkWindowEdge = GDK_WINDOW_EDGE_WEST; 
		if(rcTop.PtInRect(ptEvent))			gdkWindowEdge = GDK_WINDOW_EDGE_NORTH; 
		if(rcRight.PtInRect(ptEvent))		gdkWindowEdge = GDK_WINDOW_EDGE_EAST; 
		if(rcBottom.PtInRect(ptEvent))		gdkWindowEdge = GDK_WINDOW_EDGE_SOUTH;
		if(rcLeftTop.PtInRect(ptEvent))		gdkWindowEdge = GDK_WINDOW_EDGE_NORTH_WEST; 
		if(rcLeftBottom.PtInRect(ptEvent))	gdkWindowEdge = GDK_WINDOW_EDGE_SOUTH_WEST;
		if(rcRightTop.PtInRect(ptEvent))	gdkWindowEdge = GDK_WINDOW_EDGE_NORTH_EAST;
		if(rcRightBottom.PtInRect(ptEvent))	gdkWindowEdge = GDK_WINDOW_EDGE_SOUTH_EAST;

		gtk_window_begin_resize_drag(GTK_WINDOW(m_hWnd), gdkWindowEdge, ev->button, 
			(gint)ev->x_root, (gint)ev->y_root, ev->time);
		bHandled = FALSE;
		return 0;
	}


	//拖动窗口
	CDuiRect rcCaption = GetManager()->GetCaptionRect();
	rcCaption = GetManager()->GetDPIObj()->ScaleRect(rcCaption);
	rcCaption.left = rcClient.left + rcCaption.left;
	rcCaption.right = rcClient.right - rcCaption.right;
	rcCaption.top = rcClient.top + rcCaption.top;
	if (rcCaption.bottom < 0)
	{
		rcCaption.bottom = rcClient.bottom;
	}
	else
	{
		rcCaption.bottom = rcClient.top + rcCaption.bottom;
	}

	if(rcCaption.PtInRect(ptEvent))
	{
		CControlUI* pControl = GetManager()->FindControl(CDuiPoint(ev->x, ev->y));
		if (IsInStaticControl(pControl))
		{
			gtk_window_begin_move_drag(GTK_WINDOW(m_hWnd), ev->button, 
				(gint)ev->x_root, (gint)ev->y_root, ev->time);
		}
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


LRESULT CUIFrameWndGtk::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUIFrameWndGtk::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	GdkEventMotion *ev = (GdkEventMotion *)wParam;

	CDuiPoint ptEvent(ev->x, ev->y);

	CDuiRect rcClient;
	CPlatform::GetClientRect(GetHWND(), &rcClient);

	//改变窗口大小
	int sep = 4;
	CDuiRect rcLeft(rcClient.left-sep, rcClient.top, rcClient.left+sep, rcClient.bottom);
	CDuiRect rcTop(rcClient.left, rcClient.top-sep, rcClient.right, rcClient.top+sep);
	CDuiRect rcRight(rcClient.right-sep, rcClient.top, rcClient.right+sep, rcClient.bottom);
	CDuiRect rcBottom(rcClient.left, rcClient.bottom-sep, rcClient.right, rcClient.bottom+sep);
	CDuiRect rcLeftTop(rcClient.left-sep, rcClient.top-sep, rcClient.left+sep, rcClient.top+sep);
	CDuiRect rcLeftBottom(rcClient.left-sep, rcClient.bottom-sep, rcClient.left+sep, rcClient.bottom+sep);
	CDuiRect rcRightTop(rcClient.right-sep, rcClient.top-sep, rcClient.right+sep, rcClient.top+sep);
	CDuiRect rcRightBottom(rcClient.right-sep, rcClient.bottom-sep, rcClient.right+sep, rcClient.bottom+sep);

	if(rcLeft.PtInRect(ptEvent) || rcTop.PtInRect(ptEvent) || 
		rcRight.PtInRect(ptEvent) || rcBottom.PtInRect(ptEvent) ||
		rcLeftTop.PtInRect(ptEvent) || rcLeftBottom.PtInRect(ptEvent) ||
		rcRightTop.PtInRect(ptEvent) || rcRightBottom.PtInRect(ptEvent))
	{
		if(rcLeft.PtInRect(ptEvent))				SetCursor(DUI_SIZEWE); 
		else if(rcTop.PtInRect(ptEvent))			SetCursor(DUI_SIZENS);
		else if(rcRight.PtInRect(ptEvent))			SetCursor(DUI_SIZEWE);
		else if(rcBottom.PtInRect(ptEvent))			SetCursor(DUI_SIZENS);
		else if(rcLeftTop.PtInRect(ptEvent))		SetCursor(DUI_SIZENWSE);
		else if(rcRightBottom.PtInRect(ptEvent))	SetCursor(DUI_SIZENWSE);
		else if(rcRightTop.PtInRect(ptEvent))		SetCursor(DUI_SIZENESW);
		else if(rcLeftBottom.PtInRect(ptEvent))		SetCursor(DUI_SIZENESW);

		bHandled = TRUE;
		return 0;
	}

	SetCursor(DUI_ARROW);
	bHandled = FALSE;
	return 0;
}

} //namespace DuiLib{
#endif //#ifdef DUILIB_GTK
