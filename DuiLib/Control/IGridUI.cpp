#include "StdAfx.h"
#include "IGridUI.h"

namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
//
//
TCellData::TCellData()
{
	m_state = 0;
	m_tag = 0;
	m_dwTextColor = 0;
}

void TCellData::Select(BOOL bSelected) 
{   
	if(bSelected) m_state |= 0x01;
	else m_state &= static_cast<BYTE>(~0x01);
}

BOOL TCellData::IsSelected() const { return (m_state & 0x01) == 0x01; }

void TCellData::SetCheckBoxCheck(BOOL bSelected) 
{ 
	if(bSelected) m_state |= 0x02;
	else m_state &= static_cast<BYTE>(~0x02);
}

BOOL TCellData::IsCheckBoxCheck() const { return (m_state & 0x02) == 0x02; }

void TCellData::SetMergedWidthOthers(bool bMerged) 
{
	if(bMerged) m_state |= 0x04;
	else m_state &= static_cast<BYTE>(~0x04);
}
bool TCellData::IsMergedWithOthers() const { return (m_state & 0x04) == 0x04; }

CDuiString TCellData::GetText() const { return m_sText; }
int TCellData::GetTextN() const { return _ttoi(m_sText); }

void TCellData::SetText(LPCTSTR pstrText) { m_sText = pstrText; }
void TCellData::SetTextN(int n) { m_sText.Format(_T("%d"), n); }
void TCellData::SetTextV(LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	CDuiString s;
	s.InnerFormat(lpszFormat, argList);
	SetText(s);
	va_end(argList);
}

void TCellData::SetTag(UINT_PTR tag) { m_tag = tag; }
UINT_PTR TCellData::GetTag() const { return m_tag; }

void TCellData::SetTextColor(DWORD dwColor) { m_dwTextColor = dwColor; }
DWORD TCellData::GetTextColor() const { return m_dwTextColor; }

//////////////////////////////////////////////////////////////////////////
//
//
TRowData::TRowData()
{
	m_nHeight = 0;
	m_bSelected = FALSE;
	m_tag = 0;
}

void TRowData::SetHeight(int n) { m_nHeight = static_cast<short>(n); }
int  TRowData::GetHeight() const { return m_nHeight; }

void TRowData::Selected(BOOL bSelected) { m_bSelected = bSelected; }
BOOL TRowData::IsSelected() const { return m_bSelected; }

void TRowData::SetTag(UINT_PTR tag) { m_tag = tag; }
UINT_PTR TRowData::GetTag() const { return m_tag; }

TCellData *TRowData::GetCell(int col) { return (TCellData *)m_cells.GetAt(col); }

//////////////////////////////////////////////////////////////////////////
//
//
static CStdControlPool<TCellData> poolcell;
static CStdControlPool<TRowData> poolrow;

IGridUI::IGridUI(void)
{
	m_bVirtualGrid = FALSE;

	m_nDefRowHeight = 30;
	m_nDefColWidth = 80;
	m_nFixedColCount = 1;
	m_bViewListNo = TRUE;

	m_bDrawRowLine = TRUE;
	m_bDrawColumnLine = TRUE;
	m_dwLineColor = 0xff808080;

	m_bEnableSizeColumn = TRUE;
	m_bEnableSizeRow = TRUE;

	m_bAutoExpandLastColumn = FALSE;
	m_bAutoExpandColumnByText = FALSE;
	m_bAutoFitColumns = TRUE;
	m_bAutoFitRows = FALSE;

	m_bListMode = TRUE;
	m_bSingleRowSelection = TRUE;

	m_bHeaderSort = TRUE;
	m_szSortIconSize.cx = 16;
	m_szSortIconSize.cy = 16;

	//////////////////////////////////////////////////////////////////////////
	m_FocusCell.row = -1;
	m_FocusCell.col = -1;
}
	
IGridUI::~IGridUI(void)
{
	for (int i=0; i<m_RowData.GetSize(); i++)
	{
		TRowData *pRowData = (TRowData *)m_RowData[i];
		FreeRowData(pRowData);
	}
}

TRowData *IGridUI::AllocRowData()
{
	TRowData *pRowData = poolrow.Alloc();
	pRowData->m_nHeight = 0;
	pRowData->m_bSelected = FALSE;

	for (int i=0; i<GetColumnCount(); i++)
	{
		pRowData->m_cells.Add(AllocCellData());
	}

	return pRowData;
}

void IGridUI::FreeRowData(TRowData *pRowData)
{
	for (int j=0; j<pRowData->m_cells.GetSize(); j++)
	{
		FreeCellData((TCellData *)pRowData->m_cells[j]);
	}
	pRowData->m_cells.Empty();
	poolrow.Free(pRowData);
}

TCellData *IGridUI::AllocCellData()
{
	TCellData *pCellData = poolcell.Alloc();
	return pCellData;
}

void IGridUI::FreeCellData(TCellData *pCellData)
{
	poolcell.Free(pCellData);
}

TRowData *IGridUI::GetRowData(int row)
{
	return (TRowData *)m_RowData.GetAt(row);
}

TRowData &IGridUI::Row(int row)
{
	TRowData *pRowData = GetRowData(row);
//	if(!pRowData) return grid_row_null;
	return *pRowData;
}

TCellData *IGridUI::GetCellData(int row, int col)
{
	TRowData *pRowData = (TRowData *)m_RowData.GetAt(row);
	if(!pRowData) return NULL;

	return (TCellData *)pRowData->m_cells.GetAt(col);
}

TCellData *IGridUI::GetCellData(const TCellID &cell)
{
	return GetCellData(cell.row, cell.col);\
}

TCellData &IGridUI::Cell(int row, int col)
{
	TCellData *pCellData = GetCellData(row, col);
//	if(!pCellData) return grid_cell_null;
	return *pCellData;
}

TCellData &IGridUI::Cell(const TCellID &cell)
{
	return Cell(cell.row, cell.col);
}

void IGridUI::ClearSelectedRows()
{
	std::map<INT, INT>::iterator it;
	for (it=m_aSelectedRows.begin(); it!=m_aSelectedRows.end(); it++) 
	{
		if(!IsVirtualGrid())
		{
			Row(it->second).Selected(FALSE);
		}
	}
	m_aSelectedRows.clear();
	SetFocusCell(-1,-1);
}

void IGridUI::SelectRow(int row, BOOL bSelected, BOOL bTriggerEvent)
{
	if(IsSingleRowSelection()) //只允许单行选中时
	{
		if(IsSelectedRow(row) == bSelected)
			return;

		if(bSelected)
			ClearSelectedRows();
	}

	if(!IsVirtualGrid())
	{
		Row(row).Selected(bSelected);
	}

	if(bSelected)
	{
		m_aSelectedRows[row] = row;
	}
	else
	{
		std::map<INT, INT>::iterator it = m_aSelectedRows.find(row);
		if(it != m_aSelectedRows.end()) {
			m_aSelectedRows.erase(it);
		}
	}

	if (bTriggerEvent)
	{
		SendGridNotify(DUI_MSGTYPE_CLICK, row, 0);
	}

	Refresh();
}

BOOL IGridUI::IsSelectedRow(int row)
{
	if(!IsVirtualGrid())
		return Row(row).IsSelected();

	std::map<INT, INT>::iterator it = m_aSelectedRows.find(row);
	if(it != m_aSelectedRows.end()) {
		return TRUE;
	}
	return FALSE;
}

void IGridUI::ClearSelectedCells()
{
	std::map<INT64, TCellID>::iterator it;
	for (it=m_aSelectedCells.begin(); it!=m_aSelectedCells.end(); it++) 
	{
		if(!IsVirtualGrid())
		{
			Cell(it->second).Select(FALSE);
		}
	}
	m_aSelectedCells.clear();
	SetFocusCell(-1,-1);
}

void IGridUI::SelectCell(int row, int col, BOOL bSelected, BOOL bTriggerEvent)
{
	if(IsSelectedCell(row,col) == bSelected)
		return;

	if(!IsVirtualGrid())
	{
		Cell(row,col).Select(bSelected);
	}

	INT64 key = (((INT64)row) << 32) + col;
	if(bSelected)
	{
		TCellID cellID = {row, col};
		m_aSelectedCells[key] = cellID;
		SetFocusCell(row, col);
	}
	else
	{
		std::map<INT64, TCellID>::iterator it = m_aSelectedCells.find(key);
		if(it != m_aSelectedCells.end()){
			m_aSelectedCells.erase(it);
		}
		SetFocusCell(-1,-1);
	}	

	if (bTriggerEvent)
	{
		SendGridNotify(DUI_MSGTYPE_CLICK, row, col);
	}

	Refresh();
}

BOOL IGridUI::IsSelectedCell(int row, int col)
{
	if(!IsVirtualGrid())
		return Cell(row, col).IsSelected();

	INT64 key = (((INT64)row) << 32) + col;
	std::map<INT64, TCellID>::iterator it = m_aSelectedCells.find(key);
	if(it != m_aSelectedCells.end()){
		return TRUE;
	}
	return FALSE;
}


int IGridUI::GetSelectRowCount()
{
	return (int)m_aSelectedRows.size();
}

int IGridUI::GetSelectRow()
{
	m_iteratorRow = m_aSelectedRows.begin();
	if(m_iteratorRow == m_aSelectedRows.end())
		return -1;
	return m_iteratorRow->second;
}

int IGridUI::GetNextSelectRow()
{
	m_iteratorRow++;
	if(m_iteratorRow == m_aSelectedRows.end())
		return -1;
	return m_iteratorRow->second;
}

int IGridUI::GetSelectCellCount()
{
	return (int)m_aSelectedCells.size();
}

TCellID IGridUI::GetSelectCell()
{
	TCellID cellID = {-1,-1};
	m_iteratorCell = m_aSelectedCells.begin();
	if(m_iteratorCell == m_aSelectedCells.end())
		return cellID;

	return m_iteratorCell->second;
}

TCellID IGridUI::GetNextSelectCell()
{
	TCellID cellID = {-1,-1};
	m_iteratorCell++;
	if(m_iteratorCell == m_aSelectedCells.end())
		return cellID;

	return m_iteratorCell->second;
}

}
