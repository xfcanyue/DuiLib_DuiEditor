#include "StdAfx.h"
#include "UIGridFixedRow.h"

namespace DuiLib {
//add by liqs99 ±Ì∏Ò¿‡
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CGridFixedRowUI)
CGridFixedRowUI::CGridFixedRowUI() : m_pOwner(NULL)
{
}


CGridFixedRowUI::~CGridFixedRowUI()
{
}

LPCTSTR CGridFixedRowUI::GetClass() const
{
	return _T("GridFixedRowUI");
}

UINT CGridFixedRowUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridFixedRowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridFixedRow")) == 0 ) return static_cast<CGridFixedRowUI*>(this);
	return __super::GetInterface(pstrName);
}

bool CGridFixedRowUI::Add(CControlUI* pControl)
{	
	if(pControl->GetInterface(_T("GridFixedCell")) != NULL)
	{
		CGridFixedCellUI *pCell = (CGridFixedCellUI *)pControl;
		pCell->SetOwner(GetOwner());
		return __super::Add(pControl);
	}
	//return false;
	return __super::Add(pControl);
}

CGridCellUI *CGridFixedRowUI::GetCell(int col)
{
	return (CGridCellUI *)(CGridFixedCellUI *)GetItemAt(col);
}

SIZE CGridFixedRowUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0, m_cxyFixed.cy};
	if( cXY.cy == 0 && m_pManager != NULL ) {
		for( int it = 0; it < m_items.GetSize(); it++ ) {
			cXY.cy = MAX(cXY.cy,static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy);
		}
		int nMin = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 12;
		cXY.cy = MAX(cXY.cy,nMin);
	}

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		cXY.cx +=  static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cx;
	}

	return cXY;
}

void CGridFixedRowUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
	rc = m_rcItem;

	// Adjust for inset
	rc.left += m_rcInset.left;
	rc.top += m_rcInset.top;
	rc.right -= m_rcInset.right;
	rc.bottom -= m_rcInset.bottom;

	if( m_items.GetSize() == 0) {
		return;
	}


	// Determine the width of elements that are sizeable
	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

	int nAdjustables = 0;
	int cxFixed = 0;
	int nEstimateNum = 0;
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;
		SIZE sz = pControl->EstimateSize(szAvailable);
		if( sz.cx == 0 ) {
			nAdjustables++;
		}
		else {
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		}
		cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
		nEstimateNum++;
	}
	cxFixed += (nEstimateNum - 1) * m_iChildPadding;

	int cxExpand = 0;
	int cxNeeded = 0;
	if( nAdjustables > 0 ) cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
	// Position the elements
	SIZE szRemaining = szAvailable;
	int iPosX = rc.left;

	int iAdjustable = 0;
	int cxFixedRemaining = cxFixed;

	int nHeaderWidth = GetWidth();

	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);

		SIZE sz = {0,0};
		CGridFixedCellUI* pHeaderItem = (CGridFixedCellUI*)pControl;
		if (pHeaderItem && pHeaderItem->GetScale() != 0)
		{
			sz.cx = int(nHeaderWidth * (float)pHeaderItem->GetScale() / 100);
		}
		else
		{
			sz = pControl->EstimateSize(szRemaining);
		}

		if( sz.cx == 0 ) 
		{
			iAdjustable++;
			sz.cx = cxExpand;
			// Distribute remaining to last element (usually round-off left-overs)
			if( iAdjustable == nAdjustables ) 
			{
				sz.cx = MAX(0, szRemaining.cx - cxFixedRemaining);
			}
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
		}
		else 
		{
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

			cxFixedRemaining -= sz.cx;
		}

		sz.cy = pControl->GetFixedHeight();
		if( sz.cy == 0 ) sz.cy = rc.bottom - rc.top;
		if( sz.cy < 0 ) sz.cy = 0;
		if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
		if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

		RECT rcCtrl = { iPosX, rc.top, iPosX + sz.cx, rc.top + sz.cy};
		pControl->SetPos(rcCtrl);
		iPosX += sz.cx + m_iChildPadding;
		cxNeeded += sz.cx;
		szRemaining.cx -= sz.cx + m_iChildPadding;

		CGridCtrlUI *pOwner = (CGridCtrlUI *)GetOwner();
		pOwner->GetGridInfo()->rcColumn[it2] = rcCtrl;
		//pOwner->NeedUpdate();
	}
}


void CGridFixedRowUI::PaintBkColor(HDC hDC)
{
	return;
	DWORD dwBackColor = 0;
	dwBackColor = m_dwBackColor;

	CGridCtrlUI *pOwner = (CGridCtrlUI *)GetOwner();
	if(dwBackColor==0 && pOwner)
	{
		dwBackColor = pOwner->GetGridInfo()->dwHeaderBkColor;
	}

	if( dwBackColor != 0 ) 
	{
		bool bVer = (m_sGradient.CompareNoCase(_T("hor")) != 0);
		if( m_dwBackColor2 != 0 ) 
		{
			if( m_dwBackColor3 != 0 ) 
			{
				RECT rc = m_rcItem;
				rc.bottom = (rc.bottom + rc.top) / 2;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 8);
				rc.top = rc.bottom;
				rc.bottom = m_rcItem.bottom;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), bVer, 8);
			}
			else 
			{
				CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 16);
			}
		}
		else if( dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(dwBackColor));
		else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(dwBackColor));
	}	
	
}

void CGridFixedRowUI::PaintBorder(HDC hDC)
{
	CGridCtrlUI *pOwner = (CGridCtrlUI *)GetOwner();
	if(pOwner && pOwner->GetGridInfo()->dwLineColor != 0)
	{
		RECT rcBorder;

		rcBorder  = m_rcItem;
		int width = rcBorder.left + m_rcInset.right - m_rcInset.left;
		for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
			RECT rc = pControl->GetPos();
			width += rc.right - rc.left +  m_rcInset.right - m_rcInset.left;
			rcBorder.right  = width - 1;
			rcBorder.left	= rcBorder.right;
			CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(pOwner->GetGridInfo()->dwLineColor));
		}

		return;
	}
	
	__super::PaintBorder(hDC);
}

}