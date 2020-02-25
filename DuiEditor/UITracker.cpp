#include "StdAfx.h"
#include "DuiEditor.h"
#include "UITracker.h"

/////////////////////////////////////////////////////////////////////////////
CUITracker::CUITracker()
{
	m_hMoveHandleBitmap=(HBITMAP)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BITMAP_MOVEHANDLE), IMAGE_BITMAP,0,0,0);
	BITMAP bm; 
	GetObject(m_hMoveHandleBitmap,sizeof(BITMAP),(LPBYTE)&bm);
	m_nMoveHandleSize=bm.bmWidth;

	m_bFocus = FALSE;
}

CUITracker::CUITracker(LPCRECT lpSrcRect, UINT nStyle)
	: CTracker(lpSrcRect, nStyle)
{	
	m_hMoveHandleBitmap=(HBITMAP)::LoadImage(::AfxGetResourceHandle(),MAKEINTRESOURCE(IDB_BITMAP_MOVEHANDLE), IMAGE_BITMAP,0,0,0);
	BITMAP bm; 
	GetObject(m_hMoveHandleBitmap,sizeof(BITMAP),(LPBYTE)&bm);
	m_nMoveHandleSize=bm.bmWidth;

	m_bFocus = FALSE;
}

CUITracker::~CUITracker()
{
}

void CUITracker::Draw(CDC* pDC) const
{
	__super::Draw(pDC);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	// draw resize handles
// 	if(m_bFocus)
// 	{
// 		if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
// 		{
// 			UINT mask = GetHandleMask();
// 			for (int i = 0; i < 8; ++i)
// 			{
// 				if (mask & (1<<i))
// 				{
// 					GetHandleRect((TrackerHit)i, &rect);
// 					pDC->FillSolidRect(rect, RGB(0,0,0));
// 				}
// 			}
// 		}
// 	}
	
	//draw move handle
	if(g_duiProp.IsBaseFromContainer(m_node.name()) && m_bDrawMoveHandle)
	{
		CRect rect = m_rect;
		GetMoveHandleRect(&rect);
		CDC hCloneDC;
		hCloneDC.CreateCompatibleDC(pDC);
		HBITMAP hOldBitmap=(HBITMAP)hCloneDC.SelectObject(m_hMoveHandleBitmap);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &hCloneDC, 0, 0, SRCCOPY);
		hCloneDC.SelectObject(hOldBitmap);
		::DeleteDC(hCloneDC);
	}
}

void CUITracker::GetMoveHandleRect(CRect* pHandleRect) const
{
	CRect rcTopLeft;
	CRect rcTop;
	GetHandleRect(hitTopLeft,&rcTopLeft);
	GetHandleRect(hitTop,&rcTop);

	CRect rect;
	int size=m_nMoveHandleSize/2;
	int offset=rcTopLeft.right+10+size;
	rect.left=offset>rcTop.left?rcTopLeft.right:offset;
	rect.left-=size;
	rect.right=rect.left+m_nMoveHandleSize;
	rect.top=rcTopLeft.bottom-size;
	rect.bottom=rect.top+m_nMoveHandleSize;

	*pHandleRect=rect;
}

BOOL CUITracker::SetCursor(CWnd* pWnd, UINT nHitTest, const CPoint &ptDPtoLP, const CSize &szOffset) const
{
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);
	point-=ptDPtoLP;
	point.Offset(szOffset);

	return __super::SetCursor(pWnd, nHitTest, point);
}

int CUITracker::HitTestHandles(CPoint point) const
{
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	BOOL bIsContainer = g_duiProp.IsBaseFromContainer(m_node.name());
	if(bIsContainer)
	{
		GetMoveHandleRect(&rect);
		if(rect.PtInRect(point))
			return hitMiddle;
	}

	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}

	if(g_duiProp.IsWindowForm(m_node.name()) || bIsContainer)
	{
		return hitNothing;
	}

	return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CUITracker::TrackHandle(int nHandle, CWnd* pWnd, CDC* pDCClipTo)
{
	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= 8);   // handle 8 is inside the rect

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;
	
	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// save original width & height in pixels
	int nWidth = m_rect.Width();
	int nHeight = m_rect.Height();

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	CRect rectSave = m_rect;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	// find out what x/y coords we are supposed to modify
	int *px, *py;
	int xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDC* pDrawDC;
	if (pDCClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pDCClipTo;
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ENSURE_VALID(pDrawDC);

	CRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
			// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			rectOld = m_rect;
			// handle resize cases (and part of move)
			if (px != NULL)
				*px = GET_X_LPARAM(msg.lParam) - xDiff;
			if (py != NULL)
				*py = GET_Y_LPARAM(msg.lParam) - yDiff;

			// handle move case
			if (nHandle == hitMiddle)
			{
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
			}
			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);

			// 			{
			// 				CString temp;
			// 				temp.Format(_T("m_rect=%d,%d,%d,%d,"), m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
			// 				InsertMsg(temp);
			// 			}

			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
					DrawTrackerRect(&rectOld, pDrawDC);
				}
				//OnChangedRect(rectOld);
				if (msg.message != WM_LBUTTONUP)
					bMoved = TRUE;
			}
			if (m_bFinalErase)
				goto ExitLoop;

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
				DrawTrackerRect(&m_rect, pDrawDC);
			}
			break;

			// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTrackerRect(&m_rect, pDrawDC);
			}
			m_rect = rectSave;
			goto ExitLoop;

			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pDCClipTo == NULL)
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if(bMoved)
	{
		OnChangedRect(rectOld);
	}
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}

