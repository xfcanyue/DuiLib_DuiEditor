#include "StdAfx.h"
#include "UIGridHeader.h"

namespace DuiLib {
//add by liqs99 ±Ì∏Ò¿‡
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CGridHeaderUI)
CGridHeaderUI::CGridHeaderUI(void) : m_pOwner(NULL)
{
}


CGridHeaderUI::~CGridHeaderUI(void)
{
}

LPCTSTR CGridHeaderUI::GetClass() const
{
	return _T("GridRowUI");
}

UINT CGridHeaderUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridHeaderUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridHeader")) == 0 ) return static_cast<CGridHeaderUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridHeaderUI::SetOwner(CControlUI *pOwnder)
{
	m_pOwner = pOwnder;
}

CControlUI *CGridHeaderUI::GetOwner()
{
	return m_pOwner;
}

SIZE CGridHeaderUI::EstimateSize(SIZE szAvailable)
{
	//return __super::EstimateSize(szAvailable);
	SIZE cXY = {0, m_cxyFixed.cy};
	if( cXY.cy == 0 && m_pManager != NULL ) 
	{
		for( int it = 0; it < m_items.GetSize(); it++ ) 
		{
			int cy = MAX(cXY.cy,static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy);
			int nMin = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 6;
			cy = MAX(cy,nMin);
			cXY.cy += cy;
		}
	}

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		cXY.cx +=  static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cx;
	}

	return cXY;
}

void CGridHeaderUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	rc = m_rcItem;

	// Adjust for inset
	rc.left += m_rcInset.left;
	rc.top += m_rcInset.top;
	rc.right -= m_rcInset.right;
	rc.bottom -= m_rcInset.bottom;

	// Determine the minimum size
	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) 
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();

	int cxNeeded = 0;
	int nAdjustables = 0;
	int cyFixed = 0;
	int nEstimateNum = 0;
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		if( sz.cy == 0 ) {
			nAdjustables++;
		}
		else {
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
		}
		cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;

		RECT rcPadding = pControl->GetPadding();
		sz.cx = MAX(sz.cx, 0);
		if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
		if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		cxNeeded = MAX(cxNeeded, sz.cx);
		nEstimateNum++;
	}
	cyFixed += (nEstimateNum - 1) * m_iChildPadding;


	// Place elements
	int cyNeeded = 0;
	int cyExpand = 0;
	if( nAdjustables > 0 ) cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
	// Position the elements
	SIZE szRemaining = szAvailable;
	int iPosY = rc.top;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
		iPosY -= m_pVerticalScrollBar->GetScrollPos();
	}
	int iPosX = rc.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}
	int iAdjustable = 0;
	int cyFixedRemaining = cyFixed;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it2);
			continue;
		}

		RECT rcPadding = pControl->GetPadding();
		szRemaining.cy -= rcPadding.top;
		SIZE sz = pControl->EstimateSize(szRemaining);
		if( sz.cy == 0 ) {
			iAdjustable++;
			sz.cy = cyExpand;
			// Distribute remaining to last element (usually round-off left-overs)
			if( iAdjustable == nAdjustables ) {
				sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
			} 
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
		}
		else {
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
			cyFixedRemaining -= sz.cy;
		}

		sz.cx = MAX(cxNeeded, szAvailable.cx - rcPadding.left - rcPadding.right);

		if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
		if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

		RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top + rcPadding.bottom };
		pControl->SetPos(rcCtrl);

		iPosY += sz.cy + m_iChildPadding + rcPadding.top + rcPadding.bottom;
		cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
		szRemaining.cy -= sz.cy + m_iChildPadding + rcPadding.bottom;
	}
	cyNeeded += (nEstimateNum - 1) * m_iChildPadding;

	if( m_pHorizontalScrollBar != NULL ) {
		if( cxNeeded > rc.right - rc.left ) {
			if( m_pHorizontalScrollBar->IsVisible() ) {
				m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
			}
			else {
				m_pHorizontalScrollBar->SetVisible(true);
				m_pHorizontalScrollBar->SetScrollRange(cxNeeded - (rc.right - rc.left));
				m_pHorizontalScrollBar->SetScrollPos(0);
				rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
			}
		}
		else {
			if( m_pHorizontalScrollBar->IsVisible() ) {
				m_pHorizontalScrollBar->SetVisible(false);
				m_pHorizontalScrollBar->SetScrollRange(0);
				m_pHorizontalScrollBar->SetScrollPos(0);
				rc.bottom += m_pHorizontalScrollBar->GetFixedHeight();
			}
		}
	}

	// Process the scrollbar
	ProcessScrollBar(rc, cxNeeded, cyNeeded);
}


void CGridHeaderUI::PaintBorder(HDC hDC)
{
	CGridCtrlUI *pOwner = (CGridCtrlUI *)GetOwner();
	if(!pOwner || pOwner->GetGridInfo()->dwLineColor == 0) return;

	RECT rcBorder = m_rcItem;

	int width = rcBorder.left + m_rcInset.right - m_rcInset.left;
	int count = m_items.GetSize();
	for( int it1 = 0; it1 < count; it1++ ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
		RECT rc = pControl->GetPos();

		//top
		rcBorder		= rc;
		rcBorder.bottom	= rcBorder.top;
		CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(pOwner->GetGridInfo()->dwLineColor));

		//bottom
		if(it1 == count-1)
		{
			rcBorder		= rc;
			rcBorder.bottom -= 1;
			rcBorder.top	= rcBorder.bottom;
			CRenderEngine::DrawLine(hDC, rcBorder,1, GetAdjustColor(pOwner->GetGridInfo()->dwLineColor));
		}

		//left
		rcBorder		= rc;
		rcBorder.right	= rcBorder.left;
		CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(pOwner->GetGridInfo()->dwLineColor));

		//right
		rcBorder		= rc;
		rcBorder.right	-= 1;
		rcBorder.left	= rcBorder.right;
		CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(pOwner->GetGridInfo()->dwLineColor));
	}
	
}

}//namespace