#include "StdAfx.h"
#include "UIGridList.h"

#include "../ExcelFile/ExcelFile.h"
#include "../ExcelFile/ExcelApplication.h"
#include "../ExcelFile/ExcelBook.h"
#include "../ExcelFile/ExcelBooks.h"
#include "../ExcelFile/ExcelSheet.h"
#include "../ExcelFile/ExcelSheets.h"
#include "../ExcelFile/ExcelRange.h"
#include "../ExcelFile/ExcelRangeEx.h"

namespace DuiLib
{

//返回单元格引用，如果非法参数，就返回这个
static CGridListCellUI grid_cell_null;

IMPLEMENT_DUICONTROL(CGridListUI)

CGridListUI::CGridListUI(void)
{
	memset(m_mapColumnWidth, 0, MAX_GRID_COLUMN_COUNT*sizeof(int));
	memset(m_mapColumnWidthFixed, 0, MAX_GRID_COLUMN_COUNT*sizeof(int));
	for (int i=0; i<MAX_GRID_COLUMN_COUNT; i++)
		m_mapColumnCellType[i] = celltypeText;
	m_nRowCount = 0;
	SetInset(CDuiRect(1,1,1,1));
	SetBkColor(0xffffffff);
	Add(m_pHeader = new CGridListHeaderUI);
	Add(m_pBody = new CGridListBodyUI);
	m_pHeader->SetOwner(this);
	m_pBody->SetOwner(this);

	m_cellFocus.row = -1;
	m_cellFocus.col = -1;
	m_iFocusRow = -1;
	m_iteratorRow = 0;
	m_iteratorCell = 0;

	m_nSortCol = -1;
	m_bSortAscending = TRUE;
	m_pfnCompare = CGridListUI::pfnCellTextCompare;
}


CGridListUI::~CGridListUI(void)
{
	//ClearSelectedRows();
	//ClearSelectedCells();
}

LPCTSTR CGridListUI::GetClass() const
{
	return DUI_CTR_GRIDLIST;
}

UINT CGridListUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridListUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDLIST) == 0 ) return static_cast<CGridListUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridListUI::DoInit()
{
	EnableScrollBar(true, true);
	m_pHorizontalScrollBar->SetScrollRange(0);

	if(GetFixedColumnCount() > 0 && GetLeftFixedColWidth() > 0)
	{
		SetColumnWidth(0, GetLeftFixedColWidth());
	}

	m_nRowCount = m_pHeader->GetCount() + m_pBody->GetCount();

	if(IsVirtualGrid())
	{
		ResetGridBody();
	}
}

void CGridListUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if(!GetCellFromPt(event.ptMouse))
		{
			ClearSelectedCells();
			ClearSelectedRows();
		}
		if(IsEnableSelectRect())
		{
			m_uButtonState |= UISTATE_CAPTURED;
			m_rcTracker.left = event.ptMouse.x;
			m_rcTracker.top = event.ptMouse.y;
			m_rcTracker.right = event.ptMouse.x;
			m_rcTracker.bottom = event.ptMouse.y;
		}
		Invalidate();
		return;
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			m_uButtonState &= ~UISTATE_CAPTURED;
		if(!m_rcTracker.IsNull())
		{
			//rect is in a single cell, ignore it
			CGridListCellUI *pCell1 = GetCell(m_rcTracker.left, m_rcTracker.top);
			CGridListCellUI *pCell2 = GetCell(m_rcTracker.right, m_rcTracker.bottom);
			if(pCell1 && pCell2 && pCell1 == pCell2)
			{
				m_rcTracker.Empty();
				Invalidate();
			}
			else
			{
				m_rcTracker.Normalize();
				SelectRect(m_rcTracker);
				m_rcTracker.Empty();
				Invalidate();
			}
		}
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK);
		}
		return;
	}

	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0  && !m_rcTracker.IsNull()) 
		{
			m_rcTracker.right = event.ptMouse.x;
			m_rcTracker.bottom = event.ptMouse.y;
			Invalidate();
		}
		return;
	}

	if(event.Type == UIEVENT_RBUTTONUP)
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			if(!GetCellFromPt(event.ptMouse))
			{
				ClearSelectedCells();
				ClearSelectedRows();
			}

			if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_RCLICK);
		}
		return;
	}

	if(event.Type == UIEVENT_DBLCLICK)
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_DBCLICK);
		}
		return;
	}

	__super::DoEvent(event);
}

void CGridListUI::Notify(TNotifyUI& msg)
{

}

void CGridListUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	rc = m_rcItem;

	// Adjust for inset
	RECT m_rcInset = CVerticalLayoutUI::m_rcInset;
	if(GetManager())
		GetManager()->GetDPIObj()->Scale(&m_rcInset);
	rc.left += m_rcInset.left;
	rc.top += m_rcInset.top;
	rc.right -= m_rcInset.right;
	rc.bottom -= m_rcInset.bottom;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	if( m_items.GetSize() == 0) {
		ProcessScrollBar(rc, 0, 0);
		return;
	}

	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
	int iPosX = rc.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) 
	{
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}
	
	if(IsExpandColumnToFit() && GetColumnCount() > 0)
	{
		int cxFixed = 0;
		for (int i=0; i<GetColumnCount(); i++)
		{
			if(GetFixedColumnCount() > 0 && GetLeftFixedColWidth() > 0 && i == 0)
			{
				cxFixed += GetLeftFixedColWidth();
				continue;
			}
			if(i < MAX_GRID_COLUMN_COUNT)
			{
				cxFixed += m_mapColumnWidthFixed[i];
			}
		}
		int nAverage = ( szAvailable.cx - cxFixed ) / GetColumnCount();
		if(GetFixedColumnCount() > 0 && GetLeftFixedColWidth() > 0)
		{
			nAverage = ( szAvailable.cx - cxFixed ) / (GetColumnCount() - 1);
		}
		for (int i=0; i<GetColumnCount(); i++)
		{
			if(GetFixedColumnCount() > 0 && GetLeftFixedColWidth() > 0 && i == 0)
			{
				SetColumnWidth(i, GetLeftFixedColWidth());
				continue;
			}
			if(i < MAX_GRID_COLUMN_COUNT)
			{
				SetColumnWidth(i, nAverage + m_mapColumnWidthFixed[i]);
			}
		}
	}
	
	SIZE szHeader = m_pHeader->EstimateSize(szAvailable);
	RECT rcCtrl = {iPosX, rc.top, iPosX + szHeader.cx, rc.top + szHeader.cy };
	m_pHeader->SetPos(rcCtrl);

	szAvailable.cy -= szHeader.cy;
	SIZE szBody = m_pBody->EstimateSize(szAvailable);
	RECT rcCtrl2 = {iPosX, rc.top+szHeader.cy, iPosX + szHeader.cx, rc.bottom};
	m_pBody->SetPos(rcCtrl2);

	if(IsVirtualGrid())
	{
		BuildVirtualRow();
	}

	int cxNeeded = max(szHeader.cx, szBody.cx);
	int cyNeeded = szHeader.cy + szBody.cy;
	if(IsVirtualGrid())
		cyNeeded =  (GetRowCount() - GetFixedRowCount()) * GetDefRowHeight();

	ProcessScrollBar(rc, cxNeeded, cyNeeded);
	if(!IsVirtualGrid())
		m_pBody->GetVerticalScrollBar()->SetScrollRange(cyNeeded);

	if(IsVirtualGrid())
		ResetVirtualOrder(false);
}

void CGridListUI::SetScrollPos(SIZE szPos, bool bMsg)
{
	int cx = 0;
	int cy = 0;

	//just scroll body's VerticalScrollBar
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
	{
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		m_pBody->SetScrollPos(szPos, false);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if(IsVirtualGrid())
		ResetVirtualOrder(false);

	if( cx == 0 && cy == 0 ) return;

	RECT rcPos;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;

		rcPos = pControl->GetPos();
		rcPos.left -= cx;
		rcPos.right -= cx;
//  		rcPos.top -= cy;	//don't scroll grid's VerticalScrollBar
//  		rcPos.bottom -= cy;
		pControl->SetPos(rcPos);
	}

	Invalidate();

	if(m_pVerticalScrollBar)
	{
		// 发送滚动消息
		if( m_pManager != NULL && bMsg ) {
			int nPage = (m_pVerticalScrollBar->GetScrollPos() + m_pVerticalScrollBar->GetLineSize()) / m_pVerticalScrollBar->GetLineSize();
			m_pManager->SendNotify(this, DUI_MSGTYPE_SCROLL, (WPARAM)nPage);
		}
	}
}

void CGridListUI::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
{
	while (m_pHorizontalScrollBar)
	{
		// Scroll needed
		if (cxRequired > rc.right - rc.left && !m_pHorizontalScrollBar->IsVisible())
		{
			m_pHorizontalScrollBar->SetVisible(true);
			m_pHorizontalScrollBar->SetScrollRange(cxRequired - (rc.right - rc.left));
			m_pHorizontalScrollBar->SetScrollPos(0);
			SetPos(m_rcItem);
			break;
		}

		// No scrollbar required
		if (!m_pHorizontalScrollBar->IsVisible()) break;

		// Scroll not needed anymore?
		int cxScroll = cxRequired - (rc.right - rc.left);
		if (cxScroll <= 0)
		{
			m_pHorizontalScrollBar->SetVisible(false);
			m_pHorizontalScrollBar->SetScrollPos(0);
			m_pHorizontalScrollBar->SetScrollRange(0);
			SetPos(m_rcItem);
		}
		else
		{
			RECT rcScrollBarPos = { rc.left, rc.bottom, rc.right, rc.bottom + m_pHorizontalScrollBar->GetFixedHeight() };
			m_pHorizontalScrollBar->SetPos(rcScrollBarPos);

			if (m_pHorizontalScrollBar->GetScrollRange() != cxScroll) 
			{
				int iScrollPos = m_pHorizontalScrollBar->GetScrollPos();
				m_pHorizontalScrollBar->SetScrollRange(::abs(cxScroll)); // if scrollpos>range then scrollpos=range
				if(iScrollPos > m_pHorizontalScrollBar->GetScrollPos()) 
				{
					SetPos(m_rcItem);
				}
			}
		}
		break;
	}

	RECT rcBody = m_pBody->GetPos();
	int nBodyHeight = 0;
	if(IsVirtualGrid())
		nBodyHeight = m_pBody->GetCount() * GetDefRowHeight();
	else
		nBodyHeight = rcBody.bottom - rcBody.top;
	while (m_pVerticalScrollBar)
	{
		// Scroll needed
		if (cyRequired > nBodyHeight && !m_pVerticalScrollBar->IsVisible()) 
		{
			m_pVerticalScrollBar->SetVisible(true);
			m_pVerticalScrollBar->SetScrollRange(cyRequired - nBodyHeight);
			m_pVerticalScrollBar->SetScrollPos(0);
			SetPos(m_rcItem);
			break;
		}

		// No scrollbar required
		if (!m_pVerticalScrollBar->IsVisible()) break;

		// Scroll not needed anymore?
		int cyScroll = cyRequired - nBodyHeight;
		if (cyScroll <= 0) 
		{
			m_pVerticalScrollBar->SetVisible(false);
			m_pVerticalScrollBar->SetScrollPos(0);
			m_pVerticalScrollBar->SetScrollRange(0);
			SetPos(m_rcItem);
			break;
		}

		RECT rcScrollBarPos = { rc.right, m_rcItem.top, rc.right + m_pVerticalScrollBar->GetFixedWidth(), m_rcItem.bottom };
		m_pVerticalScrollBar->SetPos(rcScrollBarPos);

		if (m_pVerticalScrollBar->GetScrollRange() != cyScroll)
		{
			int iScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollRange(::abs(cyScroll)); // if scrollpos>range then scrollpos=range
			if(iScrollPos > m_pVerticalScrollBar->GetScrollPos()) 
			{
				SetPos(m_rcItem);
			}
		}
		break;
	}
}

void CGridListUI::BuildVirtualRow()
{
	RECT rcBody = m_pBody->GetPos();
	int nNeedRows = (rcBody.bottom - rcBody.top) / GetDefRowHeight();
	if((rcBody.bottom - rcBody.top) % GetDefRowHeight() > 0)
		nNeedRows++;
	
	nNeedRows = min(m_nRowCount - GetFixedRowCount(), nNeedRows);

	int addedRows = nNeedRows - m_pBody->GetCount();
	if(addedRows > 0)
	{
		while(m_pBody->GetCount() < nNeedRows) 
		{
			CGridListRowUI *pRow = new CGridListRowUI();
			if(m_pBody->Add(pRow))
			{
				for (int i=0; i<GetColumnCount(); i++)
				{
					CGridListCellUI *pCell = new CGridListCellUI;
					pRow->Add(pCell);
				}
			}
		}
	}
	else
	{
		while(m_pBody->GetCount() > 0 && m_pBody->GetCount() > nNeedRows) 
		{
			m_pBody->RemoveAt(m_pBody->GetCount()-1);
		}
	}
}

void CGridListUI::ResetVirtualOrder(bool bForceReset)
{
	SIZE szPos = GetScrollPos();
	RECT rcBody = m_pBody->GetPos();
	int maxPosY = (GetRowCount() - GetFixedRowCount()) * GetDefRowHeight() - m_pBody->GetCount() * GetDefRowHeight();

	//InsertMsgUiV(_T("szPos.cy=%d, maxPosY=%d"), szPos.cy, maxPosY);

	if(szPos.cy >= 0 && szPos.cy <= maxPosY)
	{
		BOOL bNofity = FALSE;
		int startrow = szPos.cy / GetDefRowHeight() + GetFixedRowCount();
		for (int i = 0; i<m_pBody->GetCount(); i++)
		{
			CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);
			if(pRow->GetRowIndex() != startrow+i)
			{
				pRow->SetRowIndex(startrow + i);
				bNofity = TRUE;
			}
		}
		//InsertMsgUiV(_T("DrawRange=%d,%d"), startrow, startrow+m_pBody->GetCount()-1);

		//Notification window for grid filling  通知窗口进行表格填充
		if(bNofity || bForceReset)
		{
			GetManager()->SendNotify(this, DUI_MSGTYPE_DRAWITEM, startrow, startrow+m_pBody->GetCount()-1);
			Refresh();
		}
	}
	Invalidate();
}

bool CGridListUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	bool bPaint = __super::DoPaint(hDC, rcPaint, pStopControl);

	if(bPaint) //paint merge cells here
	{
		for (int i=0; i<m_pHeader->GetCount(); i++)
		{
			CGridListRowUI *pRow = (CGridListRowUI *)m_pHeader->GetItemAt(i);
			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
				if(pCell->IsMerged())
				{
					PaintMergeCells(hDC, pCell);
				}
			}
		}

		for (int i=0; i<m_pBody->GetCount(); i++)
		{
			CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);

			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
				if(pCell->IsMerged())
				{
					PaintMergeCells(hDC, pCell);
				}
			}
		}
	}

	if(bPaint && !m_rcTracker.IsNull())
	{
		CDuiRect rcRect = m_rcTracker;
		rcRect.Normalize();
		CRenderEngine::DrawRect(hDC, rcRect, 2, GetAdjustColor(GetDragRectColor()), PS_SOLID);
	}
	return bPaint;
}

void CGridListUI::PaintBorder(HDC hDC)
{
	if(GetLineColor() != 0)
	{
		CRenderEngine::DrawRect(hDC, m_rcItem, 1, GetAdjustColor(GetLineColor()), PS_SOLID);
	}
}

void CGridListUI::PaintMergeCells(HDC hDC, CGridListCellUI *pCell)
{
	pCell->PaintBkColor(hDC);
	pCell->PaintBkImage(hDC);
	pCell->PaintStatusImage(hDC);
	pCell->PaintForeColor(hDC);
	pCell->PaintForeImage(hDC);
	pCell->PaintText(hDC);
	pCell->PaintBorder(hDC);
}

bool CGridListUI::Add(CControlUI* pControl)
{
	if(pControl->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
		{
			__super::Remove(m_pHeader);
		}
		m_pHeader = static_cast<CGridListHeaderUI*>(pControl);
		return __super::AddAt(pControl, 0);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDLISTBODY))
	{
		if(m_pBody != pControl && m_pBody->GetCount() == 0)
		{
			__super::Remove(m_pBody);
		}
		m_pBody = static_cast<CGridListBodyUI*>(pControl);
		return __super::AddAt(pControl, 1);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDLISTROW))
	{
		CGridListRowUI *pRow = static_cast<CGridListRowUI *>(pControl);
		return m_pBody->Add(pControl);
	}
	
	return __super::Add(pControl);
}

bool CGridListUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(pControl->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
		{
			__super::Remove(m_pHeader);
		}
		m_pHeader = static_cast<CGridListHeaderUI*>(pControl);
		return __super::AddAt(pControl, 0);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDLISTBODY))
	{
		if(m_pBody != pControl && m_pBody->GetCount() == 0)
		{
			__super::Remove(m_pBody);
		}
		m_pBody = static_cast<CGridListBodyUI*>(pControl);
		return __super::AddAt(pControl, 1);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDLISTROW))
	{
		CGridListRowUI *pRow = static_cast<CGridListRowUI *>(pControl);
		return m_pBody->AddAt(pControl, iIndex);
	}

	return __super::AddAt(pControl, iIndex);
}

int CGridListUI::InsertRow(bool bFixed, int nIndex)
{
	if(IsVirtualGrid() && !bFixed) return -1;
	if(bFixed)
	{
		if(nIndex >= 0)
		{
			CGridListRowUI *pRow = new CGridListRowUI();
			pRow->SetOwner(this);
			if(m_pHeader->AddAt(pRow, nIndex))
			{
				for (int i=0; i<GetColumnCount(); i++)
				{
					CGridListCellUI *pCell = new CGridListCellUI;
					pCell->SetOwner(this);
					pRow->Add(pCell);
				}
				return nIndex;	
			}
		}
		else
		{
			CGridListRowUI *pRow = new CGridListRowUI();
			pRow->SetOwner(this);
			if(m_pHeader->Add(pRow))
			{
				for (int i=0; i<GetColumnCount(); i++)
				{
					CGridListCellUI *pCell = new CGridListCellUI;
					pCell->SetOwner(this);
					pRow->Add(pCell);
				}
				return GetCount()-1;
			}
		}
	}

	if(nIndex >= 0)
	{
		CGridListRowUI *pRow = new CGridListRowUI();
		pRow->SetOwner(this);
		if(m_pBody->AddAt(pRow, nIndex))
		{
			for (int i=0; i<GetColumnCount(); i++)
			{
				CGridListCellUI *pCell = new CGridListCellUI;
				pCell->SetOwner(this);
				pRow->Add(pCell);
			}
			return nIndex + m_pHeader->GetCount();	
		}
	}
	else
	{
		CGridListRowUI *pRow = new CGridListRowUI();
		pRow->SetOwner(this);
		if(m_pBody->Add(pRow))
		{
			for (int i=0; i<GetColumnCount(); i++)
			{
				CGridListCellUI *pCell = new CGridListCellUI;
				pCell->SetOwner(this);
				pRow->Add(pCell);
			}
			return GetRowCount()-1;
		}
	}
	return -1;
}

bool CGridListUI::DeleteRow(int nIndex)
{
	if(IsVirtualGrid()) return false;
	if(nIndex < m_pHeader->GetCount())
		return false; //m_pHeader->RemoveAt(nIndex);
	return m_pBody->RemoveAt(nIndex-m_pHeader->GetCount());
}

int CGridListUI::InsertColumn(int nIndex)
{
	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pHeader->GetItemAt(i);
		CGridListCellUI *pCell = new CGridListCellUI();
		pCell->SetOwner(this);

		if(nIndex >= 0)
			pRow->AddAt(pCell, nIndex);
		else
			pRow->Add(pCell);
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);
		CGridListCellUI *pCell = new CGridListCellUI();
		pCell->SetOwner(this);

		if(nIndex >= 0)
			pRow->AddAt(pCell, nIndex);
		else
			pRow->Add(pCell);
	}

	return nIndex >= 0 ? nIndex : GetColumnCount()-1;	
}

void CGridListUI::DeleteColumn(int nIndex)
{
	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pHeader->GetItemAt(i);
		pRow->RemoveAt(nIndex);
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);
		pRow->RemoveAt(nIndex);
	}
}

CGridListHeaderUI *CGridListUI::GetHeader()
{
	return m_pHeader;
}

CGridListBodyUI *CGridListUI::GetBody()
{
	return m_pBody;
}

CGridListRowUI *CGridListUI::GetRow(int nRow)
{
	if (nRow < 0) return NULL;

	int nFixedRowCount = m_pHeader->GetCount();
	if(nRow < nFixedRowCount)
	{
		return static_cast<CGridListRowUI *>(m_pHeader->GetItemAt(nRow));
	}
	else if(IsVirtualGrid())
	{
		for (int i=0; i<m_pBody->GetCount(); i++)
		{
			CGridListRowUI *pRow = static_cast<CGridListRowUI *>(m_pBody->GetItemAt(i));
			if(pRow->GetRowIndex() == nRow)
				return pRow;
		}
	}
	else
		return static_cast<CGridListRowUI *>(m_pBody->GetItemAt(nRow-nFixedRowCount));

	return NULL;
}

CGridListCellUI *CGridListUI::GetCell(int nRow, int nCol)
{
	if (nRow < 0 || nCol < 0) 
		return NULL;

	CGridListRowUI * pRow = GetRow(nRow);
	if(!pRow) return NULL;

	return (CGridListCellUI *)pRow->GetItemAt(nCol);
}

CGridListCellUI &CGridListUI::Cell(int nRow, int nCol)
{
	CGridListCellUI *pCell = GetCell(nRow, nCol);
	if(pCell) return *pCell;
	return grid_cell_null;
}

CGridListCellUI *CGridListUI::GetCellFromPt(POINT pt)
{
	CGridListCellUI *pCell = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pHeader->GetItemAt(i);
		if(::PtInRect(&pRow->GetPos(), pt))
		{
			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
				if(::PtInRect(&pCell->GetPos(), pt)) 
					return pCell;
			}
			break;
		}
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);
		if(::PtInRect(&pRow->GetPos(), pt))
		{
			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
				if(::PtInRect(&pCell->GetPos(), pt)) 
					return pCell;
			}
			break;
		}
	}

	return pCell;
}

POINT CGridListUI::FindWhoMergeMe(CGridListCellUI *pMe)
{
	POINT ptCell = {-1,-1};
	POINT ptMe = {pMe->GetRowIndex(), pMe->GetColIndex()};
	CGridListCellUI *pCell = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pHeader->GetItemAt(i);
		for (int j=0; j<pRow->GetCount(); j++)
		{
			CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
			if(pCell->IsMerged() && ::PtInRect(&pCell->GetMergeRange(), ptMe)) 
			{
				ptCell.x = pCell->GetRowIndex();
				ptCell.y = pCell->GetColIndex();
				return ptCell;
			}
		}		
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridListRowUI *pRow = (CGridListRowUI *)m_pBody->GetItemAt(i);
		for (int j=0; j<pRow->GetCount(); j++)
		{
			CGridListCellUI *pCell = (CGridListCellUI *)pRow->GetItemAt(j);
			if(pCell->IsMerged() && ::PtInRect(&pCell->GetMergeRange(), ptMe)) 
			{
				ptCell.x = pCell->GetRowIndex();
				ptCell.y = pCell->GetColIndex();
				return ptCell;
			}
		}	
	}

	return ptCell;
}

void CGridListUI::ResetGrid()
{
	m_pHeader->RemoveAll();
	m_pBody->RemoveAll();
}

void CGridListUI::ResetGridBody()
{
	m_pBody->RemoveAll();
}

BOOL CGridListUI::SetRowCount(int nRows)
{
	BOOL bResult = TRUE;
	ASSERT(nRows >= 0);

	m_nRowCount = nRows;

	if(IsVirtualGrid())
	{
		ResetGridBody();
		Invalidate();
	}
	else
	{
		if (nRows == GetRowCount())
			return bResult;

		int addedRows = nRows - GetRowCount();
		if(addedRows > 0)
		{
			while(GetRowCount() < nRows) InsertRow();
		}
		else
		{
			while(GetRowCount() > 0 && GetRowCount() > nRows) 
				if(!DeleteRow(GetRowCount()-1)) break;
		}
	}
	return bResult;
}

int CGridListUI::GetRowCount()
{
	if(IsVirtualGrid()) return m_nRowCount;
	return m_pBody->GetCount() + m_pHeader->GetCount();
}


BOOL CGridListUI::SetColumnCount(int nCols)
{
	BOOL bResult = TRUE;
	if(nCols <= 0) return FALSE;
	if (nCols == GetColumnCount())
		return bResult;

	int addedCols = nCols - GetColumnCount();
	if(addedCols > 0)
	{
		while(GetColumnCount() < nCols) 
			InsertColumn();
	}
	else
	{
		while(GetColumnCount() > 0 && GetColumnCount() > nCols) 
			DeleteColumn(GetColumnCount()-1);
	}

	return bResult;
}

int CGridListUI::GetColumnCount()
{
	CGridListRowUI *pRow = GetRow(0);
	if(!pRow) return 0;
	return pRow->GetCount();
}

BOOL CGridListUI::SetFixedRowCount(int nRows)
{
	BOOL bResult = TRUE;
	ASSERT(nRows >= 0);
	if (nRows == m_pHeader->GetCount())
		return bResult;

	int nColCount = GetColumnCount();

	int addedRows = nRows - m_pHeader->GetCount();
	if(addedRows > 0)
	{
		while(m_pHeader->GetCount() < nRows) 
		{
			CGridListRowUI *pRow = new CGridListRowUI();
			pRow->SetOwner(this);
			if(m_pHeader->Add(pRow))
			{
				for (int i=0; i<GetColumnCount(); i++)
				{
					CGridListCellUI *pCell = new CGridListCellUI;
					pCell->SetOwner(this);
					pRow->Add(pCell);
				}
			}
		}
	}
	else
	{
		while(m_pHeader->GetCount() > nRows) 
			m_pHeader->RemoveAt(m_pHeader->GetCount()-1);
	}

	return bResult;
}

int CGridListUI::GetFixedRowCount()
{
	return m_pHeader->GetCount();
}

BOOL CGridListUI::SetRowHeight(int nRow, int height)
{
	CGridListRowUI *pRow = GetRow(nRow);
	if(!pRow) return FALSE;
	pRow->SetFixedHeight(height);
	return TRUE;
}

int CGridListUI::GetRowHeight(int nRow)
{
	CGridListRowUI *pRow = GetRow(nRow);
	if(!pRow) return -1;
	if(pRow->GetFixedHeight() != 0) return pRow->GetFixedHeight();
	return GetDefRowHeight();
}

BOOL CGridListUI::SetColumnWidth(int nCol, int width)
{
	if(nCol<0 || nCol >= MAX_GRID_COLUMN_COUNT) return FALSE;
	m_mapColumnWidth[nCol] = width;
	m_mapColumnWidthFixed[nCol] = width - GetDefColWidth();
	return TRUE;
}

int CGridListUI::GetColumnWidth(int nCol)
{
	if(nCol<0 || nCol >= MAX_GRID_COLUMN_COUNT) return 0;

	return m_mapColumnWidth[nCol] > 0 ? m_mapColumnWidth[nCol] : GetDefColWidth();
}

void CGridListUI::ClearSelectedRows()
{
	for (int i=0; i<m_aSelectedRow.GetSize(); i++)
	{
		int row = (int)m_aSelectedRow.GetAt(i);
		CGridListRowUI *pRow = GetRow(row);
		if(pRow)
			pRow->Selected(FALSE);
	}
	m_aSelectedRow.Empty();
	m_iFocusRow = -1;
}

void CGridListUI::SelectRow(int row, bool bClearSelectionIfCurSelected)
{
	if(IsSelectedRow(row) && IsSingleRowSelection()) return;
	CGridListRowUI *pRow = GetRow(row);
	if(!pRow) return;
	if(pRow->IsFixedRow()) return;
	
	if(IsSingleRowSelection())
		ClearSelectedRows();

	if(bClearSelectionIfCurSelected)
	{
		if(::GetKeyState(VK_CONTROL) >= 0)
			ClearSelectedRows();
	}

	if(!bClearSelectionIfCurSelected && !pRow->IsSelected())
		ClearSelectedRows();

	m_aSelectedRow.Add((LPVOID)pRow->GetRowIndex());
	pRow->Selected(TRUE);
	SetFocusRow(row);
	pRow->Invalidate();
}

BOOL CGridListUI::IsSelectedRow(int row)
{
	CGridListRowUI *pRow = GetRow(row);
	if(pRow) return pRow->IsSelected();
	return FALSE;
}

void CGridListUI::ClearSelectedCells()
{
	for (int i=0; i<m_aSelectedCellX.GetSize(); i++)
	{
		int row = (int)m_aSelectedCellX.GetAt(i);
		int col = (int)m_aSelectedCellY.GetAt(i);
		CGridListCellUI *pCell = GetCell(row, col);
		if(pCell)
		{
			pCell->Selected(FALSE);
		}
	}
	m_aSelectedCellX.Empty();
	m_aSelectedCellY.Empty();
	SetFocusCell(-1,-1);
}

void CGridListUI::SelectCell(int row, int col, bool bClearSelectionIfCurSelected)
{
	CGridListCellUI *pCell = GetCell(row, col);
	if(!pCell) return;

	if(IsListMode())
	{
		SelectRow(row, bClearSelectionIfCurSelected);
	}
	else if(pCell->IsFixedColumn() && !pCell->IsFixedRow())
	{
		SelectRow(row, bClearSelectionIfCurSelected);
	}
	else
	{
		ClearSelectedRows();
	}

	if(pCell->IsFixed()) return;

	if(IsListMode())
		ClearSelectedCells();

	if(bClearSelectionIfCurSelected)
	{
		if(::GetKeyState(VK_CONTROL) >= 0)
			ClearSelectedCells();
	}

	if(!bClearSelectionIfCurSelected && !pCell->IsSelected())
		ClearSelectedCells();
	
	m_aSelectedCellX.Add((LPVOID)row);
	m_aSelectedCellY.Add((LPVOID)col);
	pCell->Selected(TRUE);
	SetFocusCell(row, col);
	pCell->Invalidate();
}

BOOL CGridListUI::IsSelectedCell(int row, int col)
{
	CGridListCellUI *pCell = GetCell(row, col);
	if(pCell) return pCell->IsSelected();
	return FALSE;
}

void CGridListUI::SetFocusCell(int row, int col)
{
	CGridListCellUI *pCell = NULL;
	pCell = GetCell(m_cellFocus.row, m_cellFocus.col);
	if(pCell) 
	{
		pCell->OnCellKillFocus();
	}

	m_cellFocus.row = row;
	m_cellFocus.col = col;

	pCell = GetCell(m_cellFocus.row, m_cellFocus.col);
	if(pCell) 
		pCell->CreateInnerControl();

// 	if(GetManager() && row>=0 && col>=0)
// 	{
// 		GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, row, col);
// 	}
}

TGridCell CGridListUI::GetFocusCell() const
{
	return m_cellFocus;
}

void CGridListUI::SetFocusRow(int row)
{
	m_iFocusRow = row;
}

int CGridListUI::GetFocusRow() const
{
	return m_iFocusRow;
}

void CGridListUI::SelectRect(const RECT &rc)
{
	if(rc.left == rc.right && rc.top == rc.bottom) return;
	if(::GetKeyState(VK_CONTROL) >= 0)
	{
		ClearSelectedRows();
		ClearSelectedCells();
	}

	if(IsListMode())
	{
		POINT pt = {rc.right, rc.bottom};
		CGridListCellUI *pCell = GetCellFromPt(pt);
		if(pCell && !pCell->IsFixed())
		{
			int row = pCell->GetRowIndex();
			int col = pCell->GetColIndex();
			m_aSelectedCellX.Add((LPVOID)row);
			m_aSelectedCellY.Add((LPVOID)col);
			pCell->Selected(TRUE);
			SetFocusCell(row, col);
		}
	}

	CDuiRect rcIntersect;
	int rowcount = GetRowCount();
	for (int row=0; row < rowcount; row++)
	{
		CGridListRowUI *pRow = GetRow(row);
		if(!pRow) continue;
		if(pRow->IsFixedRow()) continue;
		if(rcIntersect.IntersectRect(rc, pRow->GetPos()))
		{
			if(IsListMode())
			{
				if(IsSingleRowSelection())
					ClearSelectedRows();
				m_aSelectedRow.Add((LPVOID)pRow->GetRowIndex());
				pRow->Selected(TRUE);
				SetFocusRow(row);
			}
			else
			{
				int colcount = GetColumnCount();
				for (int col=0; col < colcount; col++)
				{
					CGridListCellUI *pCell = GetCell(row, col);
					if(!pCell) continue;
					if(pCell->IsFixed()) continue;
					if(rcIntersect.IntersectRect(rc, pCell->GetPos()))
					{
						m_aSelectedCellX.Add((LPVOID)pCell->GetRowIndex());
						m_aSelectedCellY.Add((LPVOID)pCell->GetColIndex());
						pCell->Selected(TRUE);
						SetFocusCell(row, col);
					}
				}
			}
		}
	}
	Invalidate();
	Refresh();
}

int CGridListUI::GetSelectRowCount()
{
	return m_aSelectedRow.GetSize();
}

int CGridListUI::GetSelectRow()
{
	m_iteratorRow = 0;
	CGridListRowUI *pRow = NULL;
	while (!pRow)
	{
		if(m_iteratorRow >= m_aSelectedRow.GetSize())
			break;
		pRow = GetRow((int)m_aSelectedRow.GetAt(m_iteratorRow));
	}

	if(pRow)
	{
		return pRow->GetRowIndex();
	}
	return -1;
}

int CGridListUI::GetNextSelectRow()
{
	m_iteratorRow++;
	CGridListRowUI *pRow = NULL;
	while (!pRow)
	{
		if(m_iteratorRow >= m_aSelectedRow.GetSize())
			break;
		pRow = GetRow((int)m_aSelectedRow.GetAt(m_iteratorRow));
	}
	if(pRow)
	{
		return pRow->GetRowIndex();
	}
	return -1;
}

int CGridListUI::GetSelectCellCount()
{
	return m_aSelectedCellX.GetSize();
}

CGridListCellUI *CGridListUI::GetSelectCell()
{
	m_iteratorCell = 0;
	CGridListCellUI *pCell = NULL;
	while (!pCell)
	{
		if(m_iteratorCell >= m_aSelectedCellX.GetSize())
			break;
		pCell = GetCell((int)m_aSelectedCellX.GetAt(m_iteratorCell), (int)m_aSelectedCellY.GetAt(m_iteratorCell));
	}
	return pCell;
}

CGridListCellUI *CGridListUI::GetNextSelectCell()
{
	m_iteratorCell++;
	CGridListCellUI *pCell = NULL;
	while (!pCell)
	{
		if(m_iteratorCell >= m_aSelectedCellX.GetSize())
			break;
		pCell = GetCell((int)m_aSelectedCellX.GetAt(m_iteratorCell), (int)m_aSelectedCellY.GetAt(m_iteratorCell));
	}
	return pCell;
}

void CGridListUI::MergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
	BOOL bFirst = TRUE;
	for (int i=nStartRow; i<=nEndRow; i++)
	{
		for (int j=nStartCol; j<=nEndCol; j++)
		{
			if(bFirst)
			{
				CGridListCellUI *pCell = GetCell(i,j);
				if(pCell)
					pCell->SetMergeRange(CDuiRect(nStartRow, nStartCol, nEndRow, nEndCol));
				bFirst = FALSE;
			}
			else
			{
				CGridListCellUI *pCell = GetCell(i,j);
				if(pCell)
					pCell->SetMergedWithOther(true);
			}
		}
	}
}

void CGridListUI::SortItems(int col)
{
	if(!IsHeaderSort()) return;

	ClearSelectedRows();
	ClearSelectedCells();

	BOOL bAscending = !m_bSortAscending;
	if(!IsVirtualGrid())
	{
		int lo = GetFixedRowCount();
		int hi = -1;
		if(m_nSortCol != col) bAscending = TRUE;
		SortItems(m_pfnCompare, col, bAscending, 0, GetFixedRowCount(), -1);
	}
	m_nSortCol = col;
	m_bSortAscending = bAscending;

	if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_SORTITEM);
}

BOOL CGridListUI::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, int low, int high)
{
	if (nCol >= GetColumnCount())
		return FALSE;

	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;

	if (hi <= lo)
		return FALSE;

	//LPARAM midItem = GetItemData((lo + hi)/2, nCol);
	LPARAM pMidCell = (LPARAM) GetCell((lo + hi)/2, nCol);

	// loop through the list until indices cross
	while (lo <= hi)
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if (bAscending)
			while (lo < high  && pfnCompare( (LPARAM)GetCell(lo, nCol), (LPARAM) pMidCell, bAscending) < 0)
				++lo;
		else
			while (lo < high && pfnCompare((LPARAM)GetCell(lo, nCol), pMidCell, bAscending) > 0)
				++lo;

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if (bAscending)
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, bAscending) > 0)
				--hi;
		else
			while (hi > low && pfnCompare((LPARAM)GetCell(hi, nCol), pMidCell, bAscending) < 0)
				--hi;

		// If the indexes have not crossed, swap if the items are not equal	
		if (lo <= hi)
		{
			if (pfnCompare((LPARAM)GetCell(lo, nCol), (LPARAM)GetCell(hi, nCol), bAscending) != 0)
			{
				CGridListRowUI *pRow1 = GetRow(lo);
				CGridListRowUI *pRow2 = GetRow(hi);
				int fixdrows = GetFixedRowCount();
				int nIndex1 = lo - fixdrows;
				int nIndex2 = hi - fixdrows;
				m_pBody->SetItemIndex(pRow1, nIndex2);
				m_pBody->SetItemIndex(pRow2, nIndex1);
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if (low < hi)
		SortItems(pfnCompare, nCol, bAscending, data, low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if (lo < high)
		SortItems(pfnCompare, nCol, bAscending, data, lo, high);

	return TRUE;
}

int CALLBACK CGridListUI::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNUSED_ALWAYS(lParamSort);

	CGridListCellUI* pCell1 = (CGridListCellUI*) lParam1;
	CGridListCellUI* pCell2 = (CGridListCellUI*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	BOOL bAscending = (BOOL)lParamSort;
	CDuiString text1 = pCell1->GetText();
	CDuiString text2 = pCell2->GetText();
	return _tcscmp(pCell1->GetText(), pCell2->GetText());
}

void CGridListUI::Refresh()
{
	NeedParentUpdate();
}

void CGridListUI::SetVirtualGrid(BOOL bVirtual)
{
	ResetGridBody();
	m_bVirtualGrid = bVirtual;
}

void CGridListUI::SetColumnCellType(int col, GridListCellType cellType)
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		m_mapColumnCellType[col] = cellType;

		if(col < GetColumnCount())
		{
			for (int i=GetFixedRowCount(); i<GetRowCount(); i++)
			{
				CGridListCellUI *pCell = GetCell(i, col);
				if(pCell)
				{
					pCell->SetCellType(GridListCellType(cellType));
				}
			}
		}
	}
}

GridListCellType CGridListUI::GetColumnCellType(int col)
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		return m_mapColumnCellType[col];
	}
	return celltypeText;
}

static void grid_to_xml(xml_document &xml, CGridListUI *pGrid)
{
	xml_node root = xml.append_child(_T("GridList"));
	if(!pGrid->GetName().IsEmpty())
	{
		root.append_attribute(_T("name")).set_value(pGrid->GetName());
	}
	root.append_attribute(_T("rowcount")).set_value(pGrid->GetRowCount());
	root.append_attribute(_T("colcount")).set_value(pGrid->GetColumnCount());
	root.append_attribute(_T("rowheight")).set_value(pGrid->GetDefRowHeight());
	root.append_attribute(_T("colwidth")).set_value(pGrid->GetDefColWidth());

	for (int i=0; i<pGrid->GetRowCount(); i++)
	{
		CGridListRowUI *pRow = pGrid->GetRow(i);
		if(!pRow) continue;

		xml_node nodeRow = root.append_child(_T("Row"));;
		if(pRow->IsFixedRow())
			nodeRow.append_attribute(_T("header")).set_value(true);

		if(pGrid->GetRowHeight(i) != pGrid->GetDefRowHeight())
		{
			nodeRow.append_attribute(_T("height")).set_value(pGrid->GetRowHeight(i));
		}

		for (int j=0; j<pGrid->GetColumnCount(); j++)
		{
			CGridListCellUI *pCell = pGrid->GetCell(i,j);
			if(!pCell) continue;

			xml_node nodeCell = nodeRow.append_child(_T("Cell"));
			nodeCell.text().set(pCell->GetText());
			if(pCell->IsMerged())
			{
				CDuiRect rc = pCell->GetMergeRange();
				nodeCell.append_attribute(_T("merge")).set_value(rc.ToString());
			}
			if(pCell->IsMergedWidthOthers())
				nodeCell.append_attribute(_T("mergedwidthothers")).set_value(pCell->IsMergedWidthOthers());

		}
	}
}

static bool xml_to_grid(xml_document &xml, CGridListUI *pGrid)
{
	xml_node root = xml.child(_T("GridList"));
	if(!root) return false;

// 	if(root.attribute(_T("rowcount")))
// 	{
// 		pGrid->SetRowCount(root.attribute(_T("rowcount")).as_int());
// 	}
// 	if(root.attribute(_T("colcount")))
// 	{
// 		pGrid->SetColumnCount(root.attribute(_T("colcount")).as_int());
// 	}
	if(root.attribute(_T("rowheight")))
	{
		pGrid->SetDefRowHeight(root.attribute(_T("rowheight")).as_int());
	}
	if(root.attribute(_T("colwidth")))
	{
		pGrid->SetDefColWidth(root.attribute(_T("colwidth")).as_int());
	}

	for (xml_node nodeRow = root.child(_T("Row")); nodeRow; nodeRow=nodeRow.next_sibling(_T("Row")))
	{
		CGridListRowUI *pRow = new CGridListRowUI;
		bool bHeader = nodeRow.attribute(_T("header")).as_bool();
		if(bHeader)
			pGrid->GetHeader()->Add(pRow);
		else
			pGrid->GetBody()->Add(pRow);

		for (xml_node nodeCell = nodeRow.child(_T("Cell")); nodeCell; nodeCell=nodeCell.next_sibling(_T("Cell")))
		{
			CGridListCellUI *pCell = new CGridListCellUI;
			pRow->Add(pCell);

			if(nodeCell.attribute(_T("merge")))
			{
				CDuiRect rc(nodeCell.attribute(_T("merge")).as_string());
				if(!rc.IsNull())
				{
					rc.Normalize();
					pCell->SetMergeRange(rc);
				}
			}

			if(nodeCell.attribute(_T("mergedwidthothers")))
			{
				pCell->SetMergedWithOther(nodeCell.attribute(_T("mergedwidthothers")).as_bool());
			}

			pCell->SetText(nodeCell.text().get());
		}
	}
	return true;
}

bool CGridListUI::LoadXmlString(LPCTSTR lpstring)
{
	if(IsVirtualGrid()) return false;

	return false;
}

CDuiString CGridListUI::SaveXmlString()
{
	CDuiString strXml;
	if(IsVirtualGrid()) return strXml;


	return strXml;
}

bool  CGridListUI::LoadXmlFile(LPCTSTR filename, bool bOpenFileDialog)
{
	if(IsVirtualGrid()) return false;
	ResetGrid();

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("XML(*.xml)|*.xml|All Files (*.*)|*.*||");
		CFileDialog fileDlg(TRUE,_T("xml"), _T(""),  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return false;
		strFileName = fileDlg.GetPathName();
	}
	else
	{
		strFileName = filename;
	}

	xml_document xml;
	xml_parse_result ret = xml.load_file(strFileName);
	if(ret.status != pugi::status_ok) return false;
	return xml_to_grid(xml, this);
}

bool  CGridListUI::SaveXmlFile(LPCTSTR filename, bool bOpenFileDialog)
{
	if(IsVirtualGrid()) return false;

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("XML(*.xml)|*.xml|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xml"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return false;
		strFileName = fileDlg.GetPathName();
	}
	else
	{
		strFileName = filename;
	}

	xml_document xml;
	grid_to_xml(xml, this);
	return xml.save_file(strFileName);
}

bool  CGridListUI::LoadExcelFile(LPCTSTR filename, bool bOpenFileDialog)
{
	if(IsVirtualGrid()) return false;
	ResetGrid();

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("Excel(*.xls)|*.xls|All Files (*.*)|*.*||");
		CFileDialog fileDlg(TRUE,_T("xls"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return FALSE;
		strFileName = fileDlg.GetPathName();
	}
	else
		strFileName = filename;

	CExcelFile f;
	if(!f.OpenFile(strFileName))	return false;
	f.LoadSheet(1);
	if(f.GetSheetCount() <= 0)	return false;

	bool bHeader = true;
	for (int xlsRow=1; xlsRow<=f.GetRowsCount(); xlsRow++)
	{
		CGridListRowUI *pRow = new CGridListRowUI;
		if(bHeader)
		{
			GetHeader()->Add(pRow);
			bHeader = false;
		}
		else
			GetBody()->Add(pRow);

		for (int xlsCol=1; xlsCol<=f.GetColumnsCount(); xlsCol++)
		{
			CGridListCellUI *pCell = new CGridListCellUI;
			pRow->Add(pCell);

			CString cellText;
			f.GetCell(xlsRow, xlsCol, cellText);
			pCell->SetText(cellText);
		}
	}

	return true;
}

bool  CGridListUI::SaveExcelFile(LPCTSTR filename, bool bOpenFileDialog)
{
	if(IsVirtualGrid()) return false;

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("Excel(*.xls)|*.xls|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xls"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return FALSE;
		strFileName = fileDlg.GetPathName();
	}
	else
		strFileName = filename;

	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	if(bFind)	::DeleteFile(strFileName);

	CExcelFile f;
	if(!f.CreateFile())	return false;
	f.LoadSheet(1);

	int rowcount = GetRowCount();
	int colcount = GetColumnCount();
	CString cellText;
	int xlsRow=1;
	int xlsCol=1;
	for (int row=0; row<rowcount; row++)
	{
		xlsCol=1;
		for (int col=0; col<colcount; col++)
		{
			CGridListCellUI *pCell = (CGridListCellUI *)GetCell(row,col);

			if(row==0 && col==0)
				f.SetColumnWidth(xlsCol, GetColumnWidth(col));
			if(col==0)
				f.SetRowHeight(xlsRow, GetRowHeight(row));

			if(pCell->IsMergedWidthOthers())	
			{
				xlsCol++;
				continue;
			}

			CExcelRangeEx rg;

			if(pCell->IsMerged())
			{
				const RECT &rc = pCell->GetMergeRange();
				rg.SetRange(f._sheet, rc.left, rc.top, rc.right, rc.bottom);
				rg.Merge();
			}
			else
				rg.SetRange(f._sheet, row, col, row, col);

			cellText = pCell->GetText();

			f.SetCell(xlsRow, xlsCol, cellText);
			rg.SetBorder();
			rg.SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			DWORD clr = 0;
			if(pCell->IsFixed())	
				clr = GetFixedCellBkColor();
			else 
				clr = GetCellBkColor();
			if(clr != 0)
				rg.SetBackColor(clr);

			xlsCol++;
		}
		xlsRow++;
	}

	f.SaveAs(strFileName);
	return true;
}
//////////////////////////////////////////////////////////////////////////
void CGridListUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("virtualgrid")) == 0 )
	{
		SetVirtualGrid(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("rowheight")) == 0 )
	{
		SetDefRowHeight(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("columnwith")) == 0 )
	{
		SetDefColWidth(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("fixedcolumncount")) == 0 )
	{
		SetFixedColumnCount(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("leftfixedcolumnwith")) == 0 )
	{
		SetLeftFixedColWidth(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("viewlistnumber")) == 0 )
	{
		SetViewListNumber(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("rowline")) == 0 )
	{
		EnableDrawRowLine(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("columnline")) == 0 )
	{
		EnableDrawColumnLine(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("linecolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetLineColor(clrColor);
	}	
	else if( _tcsicmp(pstrName, _T("selectrect")) == 0 )
	{
		EnableSelectRect(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("dragrectcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetDragRectColor(clrColor);
	}	
	else if( _tcsicmp(pstrName, _T("sizecolumn")) == 0 )	
	{
		EnableSizeColumn(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("sizerow")) == 0 )
	{
		EnableSizeRow(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("sizecolumninbody")) == 0 )	
	{
		EnableSizeColumnInBody(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("sizerowinbody")) == 0 )
	{
		EnableSizeRowInBody(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("expandcolumntofit")) == 0 )	
	{
		ExpandColumnToFit(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("expandrowtofit")) == 0 )
	{
		ExpandRowToFit(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("listmode")) == 0 )
	{
		SetListMode(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("singlerowselection")) == 0 )
	{
		SetSingleRowSelection(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("sortheader")) == 0 )
	{
		SetHeaderSort(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("ascendingimage")) == 0 ) SetSortAscendingImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("descendingimage")) == 0 ) SetSortDescendingImage(pstrValue);
	else if( _tcsicmp(pstrName, _T("sorticonsize")) == 0 )
	{
		SIZE sz = { 0 };
		LPTSTR pstr = NULL;
		sz.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		sz.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		SetSortIconSize(sz);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("rowselectedbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetRowSelectedBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("rowselectedtextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetRowSelectedTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("rowselectedfont")) == 0 )
	{
		SetRowSelectedFont(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("rowselectedbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetRowSelectedBorderColor(clrColor);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("cellbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("celltextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("cellfont")) == 0 )
	{
		SetCellFont(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("cellbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellBorderColor(clrColor);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("cellselectedbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellSelectedBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("cellselectedtextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellSelectedTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("cellselectedfont")) == 0 )
	{
		SetCellSelectedFont(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("cellselectedbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCellSelectedBorderColor(clrColor);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("fixedcellselectedbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellSelectedBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("fixedcellselectedtextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellSelectedTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("fixedcellselectedfont")) == 0 )
	{
		SetFixedCellSelectedFont(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("fixedcellselectedbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellSelectedBorderColor(clrColor);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("fixedcellbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("fixedcelltextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("fixedcellfont")) == 0 )
	{
		SetFixedCellFont(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("fixedcellbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetFixedCellBorderColor(clrColor);
	}
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("font")) == 0 ) { SetFont(_ttoi(pstrValue)); Invalidate(); }
	//////////////////////////////////////////////////////////////////////////
	else if( _tcsicmp(pstrName, _T("edit")) == 0 )
	{
		SetStyleEdit(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("checkbox")) == 0 )
	{
		SetStyleCheckBox(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("combo")) == 0 )
	{
		SetStyleCombo(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("datetime")) == 0 )
	{
		SetStyleDateTime(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("date")) == 0 )
	{
		SetStyleDate(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("time")) == 0 )
	{
		SetStyleTime(pstrValue);
	}
	else 
		__super::SetAttribute(pstrName, pstrValue);
}


}