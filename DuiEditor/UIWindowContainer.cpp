#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIWindowContainer.h"


CUIWindowContainer::CUIWindowContainer(void)
{
}


CUIWindowContainer::~CUIWindowContainer(void)
{
}

void CUIWindowContainer::ResizeWindow()
{
	CControlUI* pRoot = GetManager()->GetRoot();

#if defined(WIN32) && !defined(UNDER_CE)
	MONITORINFO oMonitor = {}; 
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
#else
	CDuiRect rcWork;
	GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
	SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
	szAvailable = pRoot->EstimateSize(szAvailable);
	GetManager()->SetInitSize(szAvailable.cx, szAvailable.cy);

	SIZE szInit = GetManager()->GetInitSize();
	CDuiRect rc;
	CDuiPoint point;
	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + szInit.cx;
	rc.bottom = rc.top + szInit.cy;

	int nWidth = rc.GetWidth();
	int nHeight = rc.GetHeight();

	MoveWindow(rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
}

void CUIWindowContainer::OnChangeRect()
{
	//InsertMsg(_T("CUIWindowContainer::OnChangeRect()"));
	HWND hWnd = ::GetParent(GetHWND());
	if(::IsWindow(hWnd))
	{
// 		CRect rcControl = m_pControl->GetPos();
// 		CRect rc = m_tracker.m_rect;
// 		rc.OffsetRect(-rcControl.left, -rcControl.top);
// 		::SendMessage(hWnd, WM_CHANGE_RECT, (WPARAM)&rc, 0);

		::SendMessage(hWnd, WM_CHANGE_RECT, (WPARAM)&m_tracker.m_rect, 0);

// 		CRect rcControl = m_pControl->GetPos();
// 		CRect rc;
// 		rc.IntersectRect(rcControl, m_tracker.m_rect);
// 		::SendMessage(hWnd, WM_CHANGE_RECT, (WPARAM)&rc, 0);
	}
}

LRESULT CUIWindowContainer::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_SETFOCUS:		
		lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_LBUTTONUP:		lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
	case WM_LBUTTONDOWN:	lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:		lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEHOVER:		lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	case WM_PAINT:			lRes = OnPaint(uMsg, wParam, lParam, bHandled);	break;
	case WM_SETCURSOR:		lRes = OnSetCursor(uMsg, wParam, lParam, bHandled);	break;
	default:				bHandled = FALSE; break;
	}
	if (bHandled) return lRes;

	lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
	if (bHandled) return lRes;

	if(uMsg == WM_PAINT || uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN || uMsg == WM_MOUSEMOVE ||
		uMsg == WM_MOUSEHOVER || uMsg == WM_TIMER || uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS)
	{
		if (GetManager()->MessageHandler(uMsg, wParam, lParam, lRes))
			return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUIWindowContainer::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_tracker.m_nHandleSize = 6;
	m_tracker.m_nStyle = CUITracker::dottedLine|CUITracker::resizeInside;

	//////////////////////////////////////////////////////////////////////////
	// 调整窗口样式
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	GetManager()->Init(m_hWnd);
	GetManager()->AddPreMessageFilter(this);
	
	//先在根部放一个containner, 设计器内部把它当成窗口对象。
	CVerticalLayoutUI *p = new CVerticalLayoutUI;
	GetManager()->AttachDialog(p);
	GetManager()->AddNotifier(this);

	ResizeWindow();

	GetManager()->GetDPIObj()->SetDPIAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	GetManager()->SetDPI(CDPI::GetMainMonitorDPI());

	return 0;
}

LRESULT CUIWindowContainer::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	GetManager()->MessageHandler(uMsg, wParam, lParam, lRes);

	HDC hDC = ::GetDC(m_hWnd);
	CDC *pDC = CDC::FromHandle(hDC);

	m_tracker.Draw(pDC);

	::ReleaseDC(m_hWnd, hDC);
	return 0;
}

LRESULT CUIWindowContainer::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetFocus(GetSafeHwnd());

	CPoint point;
	::GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);
	CWnd *pWnd = CWnd::FromHandle(m_hWnd);
	BOOL bNewRect = FALSE;

	int htTest = m_tracker.HitTest(point);
	if(htTest > 0 )
	{
		if(m_tracker.Track(pWnd, point, TRUE))
		{
			m_tracker.m_rect.NormalizeRect();
			switch (htTest)
			{
			case CRectTracker::hitTopLeft:
				//break;
			case CRectTracker::hitTopRight:
				//break;
			case CRectTracker::hitBottomRight:
				//break; 
			case CRectTracker::hitBottomLeft:
				//break;
			case CRectTracker::hitTop:
				//break;
			case CRectTracker::hitRight:
				//break; 
			case CRectTracker::hitBottom:
				//break;
			case CRectTracker::hitLeft:
				//break; 
			case CRectTracker::hitMiddle: //移动
				bNewRect = TRUE;
				break;
			}		
		}
	}
	else
	{
		CRectTracker tracker;
		if(tracker.TrackRubberBand(pWnd, point))
		{
			tracker.m_rect.NormalizeRect();
			CRect rc;
			CRect rcControl = m_pControl->GetPos();
			rc.IntersectRect(tracker.m_rect, rcControl);
			m_tracker.m_rect = rc;
			bNewRect = TRUE;
		}
	}

	if(bNewRect)
	{
		OnChangeRect();
	}

	Invalidate();
	return 0;
}

LRESULT CUIWindowContainer::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUIWindowContainer::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT nHittest = LOWORD(lParam); 
	CWnd *pWnd = CWnd::FromHandle(m_hWnd);
	if(m_tracker.SetCursor(pWnd, nHittest))
		return 0;

	::SetCursor(LoadCursor(NULL, IDC_ARROW));
	return 0;
}

LRESULT CUIWindowContainer::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnKeyDown(uMsg, wParam, lParam, bHandled);

	switch(wParam)
	{
	case VK_UP:
		m_tracker.m_rect.OffsetRect(0,-1);
		OnChangeRect();
		break;
	case VK_DOWN:
		m_tracker.m_rect.OffsetRect(0,1);
		OnChangeRect();
		break;
	case VK_LEFT:
		m_tracker.m_rect.OffsetRect(-1,0);
		OnChangeRect();
		break;
	case VK_RIGHT:
		m_tracker.m_rect.OffsetRect(1,0);
		OnChangeRect();
		break;
	}

	return 0;
}