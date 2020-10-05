#include "StdAfx.h"
#include "UIGridBody.h"

namespace DuiLib {
//add by liqs99 ±Ì∏Ò¿‡
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CGridBodyUI)
CGridBodyUI::CGridBodyUI(void)
{
}


CGridBodyUI::~CGridBodyUI(void)
{
}

LPCTSTR CGridBodyUI::GetClass() const
{
	return _T("GridRowUI");
}

UINT CGridBodyUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridBodyUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridBody")) == 0 ) return static_cast<CGridBodyUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridBodyUI::SetOwner(CControlUI *pOwnder)
{
	m_pOwner = pOwnder;
}

CControlUI *CGridBodyUI::GetOwner() const
{
	return m_pOwner;
}

int CGridBodyUI::GetScrollStepSize() const
{
	CGridCtrlUI *pOwner = (CGridCtrlUI *)GetOwner();
	if(pOwner != NULL) return pOwner->GetScrollStepSize();

	return CVerticalLayoutUI::GetScrollStepSize();
}

void CGridBodyUI::SetScrollPos(SIZE szPos, bool bMsg)
{
	int cx = 0;
	int cy = 0;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	RECT rcPos;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;

		rcPos = pControl->GetPos();
		rcPos.left -= cx;
		rcPos.right -= cx;
		rcPos.top -= cy;
		rcPos.bottom -= cy;
		pControl->SetPos(rcPos, true);
	}

	Invalidate();
	/*
	if( m_pOwner ) {
		CListHeaderUI* pHeader = m_pOwner->GetHeader();
		if( pHeader == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		pInfo->nColumns = MIN(pHeader->GetCount(), UILIST_MAX_COLUMNS);

		if( !pHeader->IsVisible() ) {
			for( int it = 0; it < pHeader->GetCount(); it++ ) {
				static_cast<CControlUI*>(pHeader->GetItemAt(it))->SetInternVisible(true);
			}
		}
		for( int i = 0; i < pInfo->nColumns; i++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(pHeader->GetItemAt(i));
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) continue;

			RECT rcPos = pControl->GetPos();
			rcPos.left -= cx;
			rcPos.right -= cx;
			pControl->SetPos(rcPos);
			pInfo->rcColumn[i] = pControl->GetPos();
		}
		if( !pHeader->IsVisible() ) {
			for( int it = 0; it < pHeader->GetCount(); it++ ) {
				static_cast<CControlUI*>(pHeader->GetItemAt(it))->SetInternVisible(false);
			}
		}
	}
	*/
}

void CGridBodyUI::SetPos(RECT rc, bool bNeedInvalidate)
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


void CGridBodyUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
		else CVerticalLayoutUI::DoEvent(event);
		return;
	}

	CVerticalLayoutUI::DoEvent(event);
}

}//namespace