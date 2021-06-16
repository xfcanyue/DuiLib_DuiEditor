#include "StdAfx.h"
#include "IGridUI.h"

namespace DuiLib
{

static CStdControlPool<TCellData> poolcell;
static CStdControlPool<TRowData> poolrow;

//////////////////////////////////////////////////////////////////////////
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

static TRowData grid_row_null;
TRowData &IGridUI::Row(int row)
{
	TRowData *pRowData = GetRowData(row);
	if(!pRowData) return grid_row_null;
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

static TCellData grid_cell_null;
TCellData &IGridUI::Cell(int row, int col)
{
	TCellData *pCellData = GetCellData(row, col);
	if(!pCellData) return grid_cell_null;
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

void IGridUI::SelectRow(int row, BOOL bSelected)
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

void IGridUI::SelectCell(int row, int col, BOOL bSelected)
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
	return m_aSelectedRows.size();
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
	return m_aSelectedCells.size();
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