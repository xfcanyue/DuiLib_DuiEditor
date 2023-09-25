#include "StdAfx.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CTrackerUI)

CTrackerUI::CTrackerUI() : m_iCursor(0), m_uButtonState(0)
{
	m_rcInset = CDuiRect(4,4,4,4);
	m_dwBorderColor = 0xFF000000;
	m_nBorderStyle = PS_DOT;
	m_rcBorderSize = CDuiRect(1,1,1,1);
	m_bMouseDown = false;
	m_ptLastMouse.x = m_ptLastMouse.y = 0;
}

LPCTSTR CTrackerUI::GetClass() const
{
	return DUI_CTR_TRACKER;
}

LPVOID CTrackerUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_TRACKER) == 0 ) return static_cast<CTrackerUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

UINT CTrackerUI::GetControlFlags() const
{
	return UIFLAG_SETCURSOR;
}

RECT CTrackerUI::GetRect()
{
	return m_rcNewPos;
}

RECT CTrackerUI::GetSizerRect(int iIndex)
{
	LONG lMiddleX = (m_rcItem.left + m_rcItem.right) / 2;
	LONG lMiddleY = (m_rcItem.top + m_rcItem.bottom) / 2;
	LONG SIZER_WIDTH = m_rcInset.left*2;
	switch( iIndex ) {
	case 0:
		return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.top + SIZER_WIDTH);
	case 1:
		return CDuiRect(lMiddleX - SIZER_WIDTH/2, m_rcItem.top, lMiddleX + SIZER_WIDTH/2, m_rcItem.top + SIZER_WIDTH);
	case 2:
		return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
	case 3:
		return CDuiRect(m_rcItem.left, lMiddleY - SIZER_WIDTH/2, m_rcItem.left + SIZER_WIDTH, lMiddleY + SIZER_WIDTH/2);
	case 4:
		break;
	case 5:
		return CDuiRect(m_rcItem.right - SIZER_WIDTH, lMiddleY - SIZER_WIDTH/2, m_rcItem.right, lMiddleY + SIZER_WIDTH/2);
	case 6:
		return CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
	case 7:
		return CDuiRect(lMiddleX - SIZER_WIDTH/2, m_rcItem.bottom - SIZER_WIDTH, lMiddleX + SIZER_WIDTH/2, m_rcItem.bottom);
	case 8:
		return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
	default:
		break;
	}
	return CDuiRect();
}

int CTrackerUI::GetSizerCursor(POINT& pt, int& iCursor)
{
	LONG SIZER_WIDTH = m_rcInset.left*2;
	LONG SIZER_TO_ROOT = 20;	
	RECT rcRoot = m_pManager->GetRoot()->GetPos();

	iCursor = -1;
	for( int i = 8; i >= 0; --i ) {
		CDuiRect rc = GetSizerRect(i);
		if( rc.PtInRect(pt) ) {
			iCursor = i;
			break;
		}
	}
	if (iCursor < 0 && rcRoot.bottom - m_rcItem.bottom < SIZER_TO_ROOT) {
		CDuiRect rc = CDuiRect(m_rcItem.left, m_rcItem.bottom-SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
		if(rc.PtInRect(pt) ) {
			iCursor = 7;
		}
	}
	if (iCursor < 0 && rcRoot.right - m_rcItem.right < SIZER_TO_ROOT) {
		CDuiRect rc = CDuiRect(m_rcItem.right-SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
		if(rc.PtInRect(pt) ) {
			iCursor = 5;
		}
	}
	if (iCursor < 0 && m_rcItem.left - rcRoot.left < SIZER_TO_ROOT) {
		CDuiRect rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left+SIZER_WIDTH, m_rcItem.bottom);
		if(rc.PtInRect(pt) ) {
			iCursor = 3;
		}
	}
	if (iCursor < 0 && m_rcItem.top - rcRoot.top < SIZER_TO_ROOT) {
		CDuiRect rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top+SIZER_WIDTH);
		if(rc.PtInRect(pt) ) {
			iCursor = 1;
		}
	}
	if (iCursor < 0) iCursor = 4;
	switch( iCursor ) {
	case 0:
		return DUI_SIZENWSE;
	case 1:
		return DUI_SIZENS;
	case 2:
		return DUI_SIZENESW;
	case 3:
		return DUI_SIZEWE;
	case 4:
		return DUI_SIZEALL;
	case 5:
		return DUI_SIZEWE;
	case 6:
		return DUI_SIZENESW;
	case 7:
		return DUI_SIZENS;
	case 8:
		return DUI_SIZENWSE;
	default:
		return DUI_SIZEALL;
	}

	return DUI_SIZEALL;
}

int CTrackerUI::GetCorIndex(int fx, int fy, int sx, int sy)
{
	int minx = (fx < sx) ? fx : sx;
	int maxx = (fx > sx) ? fx : sx;

	int miny = (fy < sy) ? fy : sy;
	int maxy = (fy > sy) ? fy : sy;

	if ((fx == minx) && (fy == miny))
		return 0;

	if ((fx == minx) && (fy == maxy))
		return 6;

	if ((fx == maxx) && (fy == maxy))
		return 8;

	return 2;
}

void CTrackerUI::PaintBorder(UIRender *pRender) 
{
	for (int i = 0; i < 9; ++i) 
	{
		pRender->DrawColor( GetSizerRect(i), CDuiSize(0, 0), m_dwBorderColor);
	}
// 	RECT rc = m_rcItem;
// 	rc.left += m_rcInset.left;
// 	rc.top += m_rcInset.left;
// 	rc.right -= m_rcInset.left;
// 	rc.bottom -= m_rcInset.left;
// 	pRender->DrawRect( rc, m_rcBorderSize.left, m_dwBorderColor, GetBorderStyle());
	CContainerUI::PaintBorder(pRender);
}

//拽块移动位置
void CTrackerUI::MoveRect(int cx, int cy)
{
	CDuiRect rcParent = m_pParent->GetPos();
	RECT rcCurPos = m_rcNewPos;

	//只能在区域移动
	if (cx >= 0) {
		if (rcCurPos.right + cx > rcParent.right) {
			cx = rcParent.right - rcCurPos.right;
		}
	}
	else {
		if (rcCurPos.left + cx < rcParent.left) {
			cx = rcParent.left - rcCurPos.left;
		}
	}

	if (cy >= 0) {
		if (rcCurPos.bottom + cy > rcParent.bottom) {
			cy = rcParent.bottom - rcCurPos.bottom;
		}
	}
	else {
		if (rcCurPos.top + cy < rcParent.top) {
			cy = rcParent.top - rcCurPos.top;
		}
	}

	rcCurPos.bottom += cy;
	rcCurPos.top += cy;
	rcCurPos.left += cx;
	rcCurPos.right += cx;

	//位置变化才发送消息
	if (cy || cx) {
		if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_SIZECHANGED);
	}
	m_rcNewPos = rcCurPos;
}

//拖拽修改大小
void CTrackerUI::SizeRect(int movex, int movey)
{
	CDuiRect rcParent = m_pParent->GetPos();
	RECT rcCurPos = m_rcNewPos;

	if (movex < rcParent.left)
		movex = rcParent.left;
	else if (movex > rcParent.right)
		movex = rcParent.right;

	if (movey < rcParent.top)
		movey = rcParent.top;
	else if (movey > rcParent.bottom)
		movey = rcParent.bottom;

	int otherx, othery;
	int firstx, firsty;

	switch (m_iCursor) {
	case 0:
		otherx = m_rcNewPos.right;
		othery = m_rcNewPos.bottom;
		firstx = movex;
		firsty = movey;

		m_iCursor = GetCorIndex(firstx, firsty, otherx, othery);
		break;
	case 2:
		otherx = m_rcNewPos.left;
		othery = m_rcNewPos.bottom;
		firstx = movex;
		firsty = movey;

		m_iCursor = GetCorIndex(firstx,firsty,otherx,othery);
		break;
	case 6:
		otherx = m_rcNewPos.right;
		othery = m_rcNewPos.top;
		firstx = movex;
		firsty = movey;
		m_iCursor = GetCorIndex(firstx, firsty, otherx, othery);
		break;
	case 8:
		otherx = m_rcNewPos.left;
		othery = m_rcNewPos.top;
		firstx = movex;
		firsty = movey;
		m_iCursor = GetCorIndex(firstx, firsty, otherx, othery);
		break;
	case 1:
		otherx = m_rcNewPos.right;
		othery = m_rcNewPos.bottom;
		firstx = m_rcNewPos.left;
		firsty = movey;
		if (firsty > othery)
			m_iCursor = 7;
		break;
	case 7:
		otherx = m_rcNewPos.left;
		othery = m_rcNewPos.top;
		firstx = m_rcNewPos.right;
		firsty = movey;
		if (firsty < othery)
			m_iCursor = 1;
		break;
	case 3:
		otherx = m_rcNewPos.right;
		othery = m_rcNewPos.bottom;
		firsty = m_rcNewPos.top;
		firstx = movex;
		if(firstx>otherx)
			m_iCursor = 5;
		break;
	case 5:
		otherx = m_rcNewPos.left;
		othery = m_rcNewPos.top;
		firsty = m_rcNewPos.bottom;
		firstx = movex;
		if (firstx < otherx)
			m_iCursor = 3;
		break;
	default:
		return;
	}

	rcCurPos.left = (firstx < otherx) ? firstx : otherx;
	rcCurPos.right = (firstx > otherx) ? firstx : otherx;

	rcCurPos.top = (firsty < othery) ? firsty : othery;
	rcCurPos.bottom = (firsty > othery) ? firsty : othery;

	//位置变化才发送消息
	if ((m_rcNewPos.left!= rcCurPos.left) || (m_rcNewPos.top != rcCurPos.top) || (m_rcNewPos.bottom != rcCurPos.bottom) || (m_rcNewPos.right != rcCurPos.right) ){
		m_rcNewPos = rcCurPos;
		if (m_pManager != NULL) m_pManager->SendNotify(this, DUI_MSGTYPE_SIZECHANGED);
	}
}

void CTrackerUI::DoEvent(TEventUI& event) 
{
	if( event.Type == UIEVENT_SETCURSOR && IsEnabled())
	{
		int cursorx;
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) return;
		GetManager()->SetCursor(GetSizerCursor(event.ptMouse, cursorx));
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
	{
		m_bMouseDown = true;
		m_uButtonState |= UISTATE_CAPTURED;
		m_ptLastMouse = event.ptMouse;
		m_rcNewPos = m_rcItem;

		GetSizerCursor(event.ptMouse, m_iCursor);
		
		return;
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		m_bMouseDown = false;
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			m_uButtonState &= ~UISTATE_CAPTURED;
			//NeedParentUpdate();
		}
		return;
	}
	else if(( event.Type == UIEVENT_MOUSEMOVE )&&(m_bMouseDown))
	{
		if( (m_uButtonState & UISTATE_CAPTURED) == 0 ) return;

		LONG cx, cy;

		switch( m_iCursor ) {
		case 0:
		case 2:
		case 6:
		case 8:
			cx = event.ptMouse.x;
			cy = event.ptMouse.y;
			break;
		case 1:
		case 7:
			cx = 0;
			cy = event.ptMouse.y;
			break;
		case 3:
		case 5:
			cx = event.ptMouse.x;
			cy = 0;
			break;
		default:
			cx = event.ptMouse.x - m_ptLastMouse.x;
			cy = event.ptMouse.y - m_ptLastMouse.y;
			break;
		}

		m_ptLastMouse = event.ptMouse;
		//将当前拖拽块的位置 和 当前拖拽块的前一时刻的位置，刷新  
		CDuiRect rcInvalidate = m_rcNewPos;

		if (m_iCursor == 4)
			MoveRect(cx, cy);
		else
			SizeRect(cx, cy);

		this->SetPos(m_rcNewPos);
		rcInvalidate.Join(m_rcNewPos);
		if (m_pManager) m_pManager->Invalidate(rcInvalidate);
	}
	CContainerUI::DoEvent(event);
}

//
//void CCanvasContainerUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
//{
//	if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
//		CDuiRect rcParent = m_pParent->GetPos();
//		RECT rcUpdate = { 0 };
//		rcUpdate.left = m_rcNewPos.left < rcParent.left ? rcParent.left : m_rcNewPos.left;
//		rcUpdate.top = m_rcNewPos.top < rcParent.top ? rcParent.top : m_rcNewPos.top;
//		rcUpdate.right = m_rcNewPos.right > rcParent.right ? rcParent.right : m_rcNewPos.right;
//		rcUpdate.bottom = m_rcNewPos.bottom > rcParent.bottom ? rcParent.bottom : m_rcNewPos.bottom;
//		CRenderEngine::DrawColor(hDC, rcUpdate, 0x11000000);
//	}
//}

};