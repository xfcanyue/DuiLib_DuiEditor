#include "StdAfx.h"
#include "UIGridTextRow.h"


namespace DuiLib {
//add by liqs99 ±Ì∏Ò¿‡
/////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DUICONTROL(CGridTextRowUI)
CGridTextRowUI::CGridTextRowUI(void)
{
	for (int i=0; i<DUI_GRID_MAX_COLUMN; i++)
	{
		m_arrCell[i].SetOwner(this);
	}
}


CGridTextRowUI::~CGridTextRowUI(void)
{
}

LPCTSTR CGridTextRowUI::GetClass() const
{
	return _T("GridTextRowUI");
}

UINT CGridTextRowUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridTextRowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridTextRow")) == 0 ) return static_cast<CGridRowUI*>(this);
	return __super::GetInterface(pstrName);
}

CGridCellUI *CGridTextRowUI::GetCell(int col)
{
	if(col<0 || col >= DUI_GRID_MAX_COLUMN) return NULL;
	return &m_arrCell[col];
}

SIZE CGridTextRowUI::EstimateSize(SIZE szAvailable)
{
	SIZE cXY = {0, m_cxyFixed.cy};
	if( cXY.cy == 0 && m_pManager != NULL ) {
		for( int it = 0; it < m_items.GetSize(); it++ ) {
			cXY.cy = MAX(cXY.cy,static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cy);
		}
		int nMin = m_pManager->GetDefaultFontInfo()->tm.tmHeight + 6;
		cXY.cy = MAX(cXY.cy,nMin);
	}

	for( int it = 0; it < m_items.GetSize(); it++ ) {
		cXY.cx +=  static_cast<CControlUI*>(m_items[it])->EstimateSize(szAvailable).cx;
	}

	return cXY;
}

bool CGridTextRowUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	__super::DoPaint(hDC, rcPaint, pStopControl);

	CGridCtrlUI *pGrid = (CGridCtrlUI *)GetOwner();
	int rowcount = pGrid->GetGridInfo()->nRowCount - pGrid->GetGridInfo()->nFixedRowCount;
	int colCount = pGrid->GetGridInfo()->nColumnCount;
	for (int i=0; i<rowcount; i++)
	{
		RECT rcItem = m_rcItem;
		for (int i=0; i<colCount; i++)
		{
			rcItem.left = pGrid->GetGridInfo()->rcColumn[i].left;
			rcItem.right = pGrid->GetGridInfo()->rcColumn[i].right;
			m_arrCell[i].DrawCellText(hDC, rcItem);
		}
	}
	
	return true;
}

} //namespace
