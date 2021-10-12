#include "StdAfx.h"
#include "UIGrid.h"

namespace DuiLib
{


IMPLEMENT_DUICONTROL(CGridUI)

CGridUI::CGridUI(void)
{
	for (int i=0; i<MAX_GRID_COLUMN_COUNT; i++)
	{
		m_mapColumnWidth[i] = 0;
		m_mapColWidthFixed[i] = 0;
		m_mapColumnCellType[i] = celltypeText;
		m_mapColumnSort[i] = TRUE;
	}

	m_nColCount = 0;
	m_nRowCount = 0;
	SetInset(CDuiRect(1,1,1,1));
	Add(m_pHeader = new CGridHeaderUI);
	Add(m_pBody = new CGridBodyUI);
	m_pHeader->SetOwner(this);
	m_pHeader->SetAutoCalcHeight(true);
	m_pBody->SetOwner(this);

	m_nSortCol = -1;
	m_bSortAscending = TRUE;
	m_pfnCompare = CGridUI::pfnCellTextCompare;

	m_pCellLButtonDown = NULL;
	m_pHotRow = NULL;
	m_pHotCell = NULL;
	m_nSeparatorType = 0;
}


CGridUI::~CGridUI(void)
{
	ClearSelectedRows();
	ClearSelectedCells();
}

LPCTSTR CGridUI::GetClass() const
{
	return DUI_CTR_GRID;
}

UINT CGridUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRID) == 0 ) return static_cast<CGridUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridUI::Refresh(bool bNeedUpdate)
{
	if(bNeedUpdate) NeedUpdate();
	else Invalidate();
}

bool CGridUI::Add(CControlUI* pControl)
{
	if(pControl->GetInterface(DUI_CTR_GRIDHEADER))
	{
		if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
		{
			__super::Remove(m_pHeader);
		}
		m_pHeader = static_cast<CGridHeaderUI*>(pControl);
		m_pHeader->SetOwner(this);
		m_pHeader->SetAutoCalcHeight(true);
		return __super::AddAt(pControl, 0);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDBODY))
	{
		if(m_pBody != pControl && m_pBody->GetCount() == 0)
		{
			__super::Remove(m_pBody);
		}
		m_pBody = static_cast<CGridBodyUI*>(pControl);
		m_pBody->SetOwner(this);
		return __super::AddAt(pControl, 1);
	}
	return false;
// 	if(pControl->GetInterface(DUI_CTR_GRIDROW))
// 	{
// 		CGridRowUI *pRow = static_cast<CGridRowUI *>(pControl);
// 		return m_pBody->Add(pControl);
// 	}
// 	
// 	return __super::Add(pControl);
}

bool CGridUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(pControl->GetInterface(DUI_CTR_GRIDHEADER))
	{
		if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
		{
			__super::Remove(m_pHeader);
		}
		m_pHeader = static_cast<CGridHeaderUI*>(pControl);
		m_pHeader->SetOwner(this);
		m_pHeader->SetAutoCalcHeight(true);
		return __super::AddAt(pControl, 0);
	}

	if(pControl->GetInterface(DUI_CTR_GRIDBODY))
	{
		if(m_pBody != pControl && m_pBody->GetCount() == 0)
		{
			__super::Remove(m_pBody);
		}
		m_pBody = static_cast<CGridBodyUI*>(pControl);
		m_pBody->SetOwner(this);
		return __super::AddAt(pControl, 1);
	}
	return false;
// 	if(pControl->GetInterface(DUI_CTR_GRIDROW))
// 	{
// 		CGridRowUI *pRow = static_cast<CGridRowUI *>(pControl);
// 		return m_pBody->AddAt(pControl, iIndex);
// 	}
// 
// 	return __super::AddAt(pControl, iIndex);
}

int CGridUI::InsertRow(int nIndex)
{
	if(IsVirtualGrid() && nIndex >= GetFixedRowCount()) 
	{
		m_nRowCount++;
		return nIndex;
	}

	//固定行需要插入实际控件
	if(nIndex >=0 && nIndex < GetFixedRowCount())
	{
		CGridRowUI *pRow = new CGridRowUI();
		pRow->SetOwner(this);
		m_pHeader->AddAt(pRow, nIndex);

		for (int i=0; i<GetColumnCount(); i++)
		{
			CGridCellUI *pCell = new CGridCellUI;
			pCell->SetOwner(this);
			pRow->Add(pCell);
		}

		m_RowData.InsertAt(nIndex, AllocRowData());
		m_nRowCount++;
		Refresh(true);
		return nIndex;	
	}

	if(nIndex >= 0)
	{
		m_RowData.InsertAt(nIndex, AllocRowData());
		m_nRowCount++;
		Refresh(true);
		return nIndex;
	}
	else
	{
		m_RowData.Add(AllocRowData());
		m_nRowCount++;
		Refresh(true);
		return m_RowData.GetSize()-1;
	}	

	return -1;
}

bool CGridUI::DeleteRow(int nIndex)
{
	if(nIndex < GetFixedRowCount())
	{
		m_pHeader->RemoveAt(nIndex);
	}

	SelectRow(nIndex, FALSE);

	for (int i=0; i<GetColumnCount(); i++)
	{
		SelectCell(nIndex, i, FALSE);
		if(GetFocusCell().row == nIndex && GetFocusCell().col == i)
			SetFocusCell(-1,-1);
	}

	TRowData *pRowData = (TRowData *)m_RowData.GetAt(nIndex);
	if(pRowData)
	{
		m_RowData.Remove(nIndex);
		FreeRowData(pRowData);
	}
	m_nRowCount--;
	Refresh(true);
	return true;
}

int CGridUI::InsertColumn(int nIndex)
{
	m_nColCount++;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pHeader->GetItemAt(i);
		CGridCellUI *pCellUI = new CGridCellUI();

		if(nIndex >= 0)
			pRow->AddAt(pCellUI, nIndex);
		else
			pRow->Add(pCellUI);

		pCellUI->SetOwner(this);
		if(nIndex >= 0)
			pCellUI->SetCellIndex(i, nIndex);
		else
			pCellUI->SetCellIndex(i, m_nColCount-1);
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pBody->GetItemAt(i);
		CGridCellUI *pCell = new CGridCellUI();

		if(nIndex >= 0)
			pRow->AddAt(pCell, nIndex);
		else
			pRow->Add(pCell);

		pCell->SetOwner(this);
	}

	for (int i=0; i<m_RowData.GetSize(); i++)
	{
		TRowData *pRowData = (TRowData *)m_RowData.GetAt(i);
		if(nIndex >= 0)
			pRowData->m_cells.InsertAt(nIndex, AllocCellData());
		else
			pRowData->m_cells.Add(AllocCellData());
	}

	if(nIndex >= 0) return nIndex;	
	Refresh(true);
	return m_nColCount;
}

void CGridUI::DeleteColumn(int nIndex)
{
	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pHeader->GetItemAt(i);
		pRow->RemoveAt(nIndex);
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pBody->GetItemAt(i);
		pRow->RemoveAt(nIndex);
	}

	for (int i=0; i<m_RowData.GetSize(); i++)
	{
		TRowData *pRowData = (TRowData *)m_RowData.GetAt(i);
		TCellData *pCellData = (TCellData *)pRowData->m_cells.GetAt(nIndex);

		SelectCell(i,nIndex,FALSE);
		if(GetFocusCell().row == i && GetFocusCell().col == nIndex)
			SetFocusCell(-1, -1);

		pRowData->m_cells.Remove(nIndex);
		FreeCellData(pCellData);
	}
	m_nColCount--;
	Refresh(true);
}

void CGridUI::ResetGrid()
{
	m_pHeader->RemoveAll();
	m_pBody->RemoveAll();
}

void CGridUI::ResetGridBody()
{
	while (GetRowCount() > GetFixedRowCount())
	{
		DeleteRow(GetRowCount()-1);
	}
}

BOOL CGridUI::SetRowCount(int rows)
{
	BOOL bResult = TRUE;
	ASSERT(rows >= 0);


// 	if(IsVirtualGrid())
// 	{
// 		m_nRowCount = rows;
// 		Invalidate();
// 	}
// 	else
// 	{
		if (rows == GetRowCount())
			return bResult;

		int addedRows = rows - GetRowCount();
		if(addedRows > 0)
		{
			while(GetRowCount() < rows) 
				InsertRow();
		}
		else
		{
			while(GetRowCount() > 0 && GetRowCount() > rows) 
				if(!DeleteRow(GetRowCount()-1)) 
					break;
		}
//	}
	return bResult;
}

int CGridUI::GetRowCount()
{
	if(IsVirtualGrid()) return m_nRowCount;
	return m_RowData.GetSize();
}


BOOL CGridUI::SetColumnCount(int cols)
{
	BOOL bResult = TRUE;
	if(cols <= 0) return FALSE;
	if (cols == GetColumnCount())
		return bResult;

	int addedCols = cols - GetColumnCount();
	if(addedCols > 0)
	{
		while(GetColumnCount() < cols) 
		{
			InsertColumn();
		}
	}
	else
	{
		while(GetColumnCount() > 0 && GetColumnCount() > cols) 
		{
			DeleteColumn(GetColumnCount()-1);
		}
	}

	return bResult;
}

int CGridUI::GetColumnCount()
{
	return m_nColCount;
}

BOOL CGridUI::SetFixedRowCount(int rows)
{
	BOOL bResult = TRUE;
	ASSERT(rows >= 0);
	if (rows == m_pHeader->GetCount())
		return bResult;

	int addedRows = rows - m_pHeader->GetCount();
	if(addedRows > 0)
	{
		while(m_pHeader->GetCount() < rows) 
		{
			CGridRowUI *pRow = new CGridRowUI();
			pRow->SetOwner(this);
			if(m_pHeader->Add(pRow))
			{
				m_RowData.InsertAt(m_pHeader->GetCount()-1, AllocRowData());
				m_nRowCount++;

				for (int i=0; i<GetColumnCount(); i++)
				{
					CGridCellUI *pCellUI = new CGridCellUI;
					pCellUI->SetOwner(this);
					pCellUI->SetCellIndex(m_pHeader->GetCount()-1, i);
					pRow->Add(pCellUI);
				}
			}
		}
	}
	else
	{
		while(m_pHeader->GetCount() > rows) 
		{
			int nDeleteRow = m_pHeader->GetCount()-1;
			TRowData *pRowData = (TRowData *)m_RowData.GetAt(nDeleteRow);
			if(!pRowData) return false;

			for (int i=0; i<GetColumnCount(); i++)
			{
				SelectCell(nDeleteRow, i, FALSE);
				if(GetFocusCell().row == nDeleteRow && GetFocusCell().col == i)
					SetFocusCell(-1,-1);
			}
			m_RowData.Remove(nDeleteRow);
			FreeRowData(pRowData);
			m_nRowCount--;

			m_pHeader->RemoveAt(m_pHeader->GetCount()-1);

		}
	}

	return bResult;
}

int CGridUI::GetFixedRowCount()
{
	return m_pHeader->GetCount();
}

BOOL CGridUI::SetRowHeight(int row, int height)
{
	TRowData *pRowData = GetRowData(row);
	if(!pRowData) return FALSE;

	pRowData->SetHeight(height);
	return TRUE;
}

int CGridUI::GetRowHeight(int row)
{
	TRowData *pRowData = GetRowData(row);
	if(!pRowData) return GetDefRowHeight();
	
	if(pRowData->GetHeight() <= 0) return GetDefRowHeight();
	return pRowData->GetHeight();
}

BOOL CGridUI::SetColumnWidth(int col, int width)
{
	if(col<0 || col >= MAX_GRID_COLUMN_COUNT) return FALSE;
	m_mapColWidthFixed[col] = width;
	return TRUE;
}

int CGridUI::GetColumnWidth(int col)
{
	if(col<0 || col >= MAX_GRID_COLUMN_COUNT) return 0;
	if(m_mapColWidthFixed[col] > 0) return m_mapColWidthFixed[col];
	if(m_mapColumnWidth[col] > 0) return m_mapColumnWidth[col];
	return GetDefColWidth();
}

void CGridUI::SetCellType(int col, GridCellType cellType)
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		m_mapColumnCellType[col] = cellType;
	}
}

GridCellType CGridUI::GetCellType(int col)
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		return m_mapColumnCellType[col];
	}
	return celltypeText;
}

void CGridUI::SetCellType(int row, int col, GridCellType cellType)
{
	INT64 key = (((INT64)row) << 32) + col;
	m_mapCellType[key] = cellType;

	CGridCellUI *pCellUI = GetCellUI(row,col);
	if(pCellUI)
	{
		if(cellType == celltypeCheckBox || cellType == celltypePicture)
			pCellUI->CreateInnerControl();
	}
}

GridCellType CGridUI::GetCellType(int row, int col)
{
	INT64 key = (((INT64)row) << 32) + col;
	std::map<INT64, GridCellType>::iterator it = m_mapCellType.find(key);
	if(it != m_mapCellType.end())
		return it->second;

	//固定行的格子类型需要在设计器或者代码中设定，不直接引用列类型。
	if(IsFixedRow(row))
		return celltypeText;

	return GetCellType(col);
}

void CGridUI::SetVirtualGrid(BOOL bVirtual)
{
	if(m_bVirtualGrid == bVirtual) return;
	ResetGridBody();
	m_bVirtualGrid = bVirtual;
	m_nRowCount = GetFixedRowCount();
	NeedUpdate();
}

void CGridUI::SetVirtualRowCount(int nRows)
{
	SetRowCount(nRows + GetFixedRowCount());
}

void CGridUI::SetRowTag(int row, UINT_PTR tag)
{
	TRowData *pRowData = GetRowData(row);
	if(pRowData)
	{
		pRowData->SetTag(tag);
	}
}

UINT_PTR CGridUI::GetRowTag(int row)
{
	TRowData *pRowData = GetRowData(row);
	if(pRowData)
	{
		return pRowData->GetTag();
	}
	return NULL;
}

int CGridUI::FindRowFromRowTag(UINT_PTR tag)
{
	for (int i=0; i<m_RowData.GetSize(); i++)
	{
		TRowData *pRowData = (TRowData *)m_RowData.GetAt(i);
		if(pRowData->GetTag() == tag)
			return i;
	}
	return -1;
}

BOOL CGridUI::IsMergedCell(int row, int col)
{
	if(row == 0 && col == 0)
	{
		int x = 0; x++;
	}
	INT64 key = (((INT64)row) << 32) + col;
	std::map<INT64, TGridMergeRange>::iterator it = m_mapCellMergeRange.find(key);
	if(it != m_mapCellMergeRange.end())
		return TRUE;

	return FALSE;
}

TGridMergeRange CGridUI::GetCellMergeRange(int row, int col)
{
	TGridMergeRange rc = {0};
	INT64 key = (((INT64)row) << 32) + col;
	std::map<INT64, TGridMergeRange>::iterator it = m_mapCellMergeRange.find(key);
	if(it != m_mapCellMergeRange.end())
		return it->second;
	return rc;
}

void CGridUI::MergeCells(int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
	INT64 key = (((INT64)nStartRow) << 32) + nStartCol;

	//同一个格子，删除合并
	if(nStartRow == nEndRow && nStartCol == nEndCol)
	{
		TGridMergeRange rcRange = {0};
		std::map<INT64, TGridMergeRange>::iterator it = m_mapCellMergeRange.find(key);
		if(it != m_mapCellMergeRange.end())
		{
			rcRange = it->second;
			m_mapCellMergeRange.erase(it);

			for (int i=rcRange.begin_row; i<=rcRange.end_row; i++)
			{
				for (int j=rcRange.begin_col; j<=rcRange.end_col; j++)
				{
					if(i==rcRange.begin_row && j==rcRange.begin_col)
						continue;

					TCellData *pCellMerged = GetCellData(i, j);
					if(pCellMerged)
						pCellMerged->SetMergedWidthOthers(false);
				}
			}
		}
	}
	else
	{
		TGridMergeRange range;
		range.begin_row = nStartRow;
		range.end_row = nEndRow;
		range.begin_col = nStartCol;
		range.end_col = nEndCol;
		m_mapCellMergeRange[key] = range;	

		for (int i=nStartRow; i<=nEndRow; i++)
		{
			for (int j=nStartCol; j<=nEndCol; j++)
			{
				if(i==nStartRow && j==nStartCol)
					continue;

				TCellData *pCellMerged = GetCellData(i, j);
				if(pCellMerged)
					pCellMerged->SetMergedWidthOthers(true);
			}
		}
	}
	NeedUpdate();
}


CGridCellUI *CGridUI::GetCellUI(int row, int col)
{
	CGridCellUI *pCell = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)m_pHeader->GetItemAt(i);
		for (int j=0; j<pRowUI->GetCount(); j++)
		{
			CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
			if(pCellUI->GetRow() == row && pCellUI->GetCol() == col)
				return pCellUI;
		}
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)m_pBody->GetItemAt(i);
		for (int j=0; j<pRowUI->GetCount(); j++)
		{
			CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
			if(pCellUI->GetRow() == row && pCellUI->GetCol() == col)
				return pCellUI;
		}
	}

	return pCell;
}

CGridCellUI *CGridUI::GetCellUI(const TCellID &cellID)
{
	return GetCellUI(cellID.row, cellID.col);
}

CGridCellUI *CGridUI::GetCellUI(TCellData *pCellData)
{
	CGridCellUI *pCell = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)m_pHeader->GetItemAt(i);
		for (int j=0; j<pRowUI->GetCount(); j++)
		{
			CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
			if(GetCellData(pCellUI->GetRow(), pCellUI->GetCol()) == pCellData)
				return pCellUI;
		}
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)m_pBody->GetItemAt(i);
		for (int j=0; j<pRowUI->GetCount(); j++)
		{
			CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
			if(GetCellData(pCellUI->GetRow(), pCellUI->GetCol()) == pCellData)
				return pCellUI;
		}
	}

	return pCell;
}

CGridCellUI *CGridUI::GetCellUIFromPoint(const POINT &pt)
{
	CGridCellUI *pCellUI = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pHeader->GetItemAt(i);
		if(::PtInRect(&pRow->GetPos(), pt))
		{
			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridCellUI *pCell = (CGridCellUI *)pRow->GetItemAt(j);
				if(::PtInRect(&pCell->GetPos(), pt)) 
				{
					pCellUI = pCell;
					goto OUT_FOR;
				}
			}
		}
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pBody->GetItemAt(i);
		if(::PtInRect(&pRow->GetPos(), pt))
		{
			for (int j=0; j<pRow->GetCount(); j++)
			{
				CGridCellUI *pCell = (CGridCellUI *)pRow->GetItemAt(j);
				if(::PtInRect(&pCell->GetPos(), pt)) 
				{
					pCellUI = pCell;
					goto OUT_FOR;
				}
			}
		}
	}

	return NULL;

OUT_FOR:
	//谁把我合并了？

	//实表模式下，单元格会保存是否被合并的信息，没有被合并直接返回当前格子
	TCellData *pCellData = GetCellData(pCellUI->GetRow(), pCellUI->GetCol());
	if(pCellData)
	{
		if(!pCellData->IsMergedWithOthers())
			return pCellUI;
	}

	TCellID cellID = {pCellUI->GetRow(), pCellUI->GetCol()};
	std::map<INT64, TGridMergeRange>::iterator it;
	for (it=m_mapCellMergeRange.begin(); it!=m_mapCellMergeRange.end(); it++)
	{
		if((cellID.row >= it->second.begin_row && cellID.row <= it->second.end_row)		&&
			(cellID.col >= it->second.begin_col && cellID.col <= it->second.end_col)	)
		{
			pCellUI = GetCellUI(it->second.begin_row, it->second.begin_col);
			break;
		}
	}

	return pCellUI;
}

CGridRowUI *CGridUI::GetRowUI(int row)
{
	CGridRowUI *pRowUI = NULL;

	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		pRowUI = (CGridRowUI *)m_pHeader->GetItemAt(i);
		if(pRowUI->GetRow() == row)
			return pRowUI;
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		pRowUI = (CGridRowUI *)m_pBody->GetItemAt(i);
		if(pRowUI->GetRow() == row)
			return pRowUI;
	}

	return NULL;
}

BOOL CGridUI::IsFixedRow(int row)
{
	if(row >= 0 && row < GetFixedRowCount())
		return TRUE;
	return FALSE;
}

BOOL CGridUI::IsFixedCol(int col)
{
	if(col >= 0 && col < GetFixedColumnCount())
		return TRUE;
	return FALSE;
}

void CGridUI::SetFocusCell(int row, int col) 
{ 
	if(m_FocusCell.IsValid()) 
	{
		CGridCellUI *pCellUI = GetCellUI(m_FocusCell);
		if(pCellUI)
			pCellUI->OnCellKillFocus();
	}

	m_FocusCell.row = row;
	m_FocusCell.col = col;

	if(m_FocusCell.IsValid()) 
	{
		CGridCellUI *pCellUI = GetCellUI(m_FocusCell);
		if(pCellUI)
			pCellUI->OnCellSetFocus();
	}
}

void CGridUI::SetFocusCell(const TCellID &cellID)
{
	m_FocusCell.row = cellID.row;
	m_FocusCell.col = cellID.col;
}

const TCellID &CGridUI::GetFocusCell() const { return m_FocusCell; }

void CGridUI::SetColumnSort(int col, BOOL bSort)
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		m_mapColumnSort[col] = bSort;
	}
}

BOOL CGridUI::IsColumnSort(int col) const
{
	if(col >= 0 && col <= MAX_GRID_COLUMN_COUNT)
	{
		return m_mapColumnSort[col];
	}
	return FALSE;
}

void CGridUI::SetSortCallbackFun(PFNLVCOMPARE pfnCompare) { m_pfnCompare = pfnCompare; }
PFNLVCOMPARE CGridUI::GetSortCallbackFun() const { return m_pfnCompare; }

void CGridUI::SortItems(int col)
{
	if(!IsHeaderSort() || !IsColumnSort(col)) return;

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
	
	OnSortItem(col, bAscending);
	Refresh(true);
}

void CGridUI::OnSortItem(int col, BOOL bAscending)
{
	if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_SORTITEM, (WPARAM)col, (LPARAM)bAscending);
}

BOOL CGridUI::SortItems(PFNLVCOMPARE pfnCompare, int col, BOOL bAscending, LPARAM data, int low, int high)
{
	if (col >= GetColumnCount())
		return FALSE;

	if (high == -1)
		high = GetRowCount() - 1;

	int lo = low;
	int hi = high;

	if (hi <= lo)
		return FALSE;

	//LPARAM midItem = GetItemData((lo + hi)/2, col);
	LPARAM pMidCell = (LPARAM) GetCellData((lo + hi)/2, col);

	// loop through the list until indices cross
	while (lo <= hi)
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if (bAscending)
			while (lo < high  && pfnCompare( (LPARAM)GetCellData(lo, col), (LPARAM) pMidCell, bAscending) < 0)
				++lo;
		else
			while (lo < high && pfnCompare((LPARAM)GetCellData(lo, col), pMidCell, bAscending) > 0)
				++lo;

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if (bAscending)
			while (hi > low && pfnCompare((LPARAM)GetCellData(hi, col), pMidCell, bAscending) > 0)
				--hi;
		else
			while (hi > low && pfnCompare((LPARAM)GetCellData(hi, col), pMidCell, bAscending) < 0)
				--hi;

		// If the indexes have not crossed, swap if the items are not equal	
		if (lo <= hi)
		{
			if (pfnCompare((LPARAM)GetCellData(lo, col), (LPARAM)GetCellData(hi, col), bAscending) != 0)
			{
				TRowData *pRow1 = GetRowData(lo);
				TRowData *pRow2 = GetRowData(hi);
				m_RowData.SetAt(lo, pRow2);
				m_RowData.SetAt(hi, pRow1);
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if (low < hi)
		SortItems(pfnCompare, col, bAscending, data, low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if (lo < high)
		SortItems(pfnCompare, col, bAscending, data, lo, high);

	return TRUE;
}

int CALLBACK CGridUI::pfnCellTextCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	TCellData* pCell1 = (TCellData*) lParam1;
	TCellData* pCell2 = (TCellData*) lParam2;
	if (!pCell1 || !pCell2) return 0;

	BOOL bAscending = (BOOL)lParamSort;
	CDuiString text1 = pCell1->GetText();
	CDuiString text2 = pCell2->GetText();
	return _tcscmp(pCell1->GetText(), pCell2->GetText());
}

//////////////////////////////////////////////////////////////////////////
void CGridUI::DoInit()
{
	EnableScrollBar(true, true);
	m_pHorizontalScrollBar->SetScrollRange(0);

	//左侧第一列是否固定宽度
	if(GetFixedColumnCount() > 0 && IsViewListNumber())
	{
		m_mapColWidthFixed[0] = 30;
	}

	//初始化数据行
	int nRows = m_pHeader->GetCount() + m_pBody->GetCount();

	m_nColCount = 0;
	//根据标题栏第一行判断有几列
	CGridRowUI *pHeaderRow = (CGridRowUI *)m_pHeader->GetItemAt(0);
	if(pHeaderRow) m_nColCount = pHeaderRow->GetCount();

	//初始化数据
	for (int i=0; i<nRows; i++)
	{
		TRowData *pRowData = AllocRowData();
		m_RowData.Add(pRowData);
		m_nRowCount++;
	}

	//初始化表格头里面的控件
	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pHeaderRow = (CGridRowUI *)m_pHeader->GetItemAt(i);
		pHeaderRow->SetOwner(this);
		pHeaderRow->SetRowIndex(i);
		pHeaderRow->ApplyAttributeList(GetStyleFixedRow());
		Row(i).SetHeight(pHeaderRow->GetFixedHeight());
		for (int j=0; j<pHeaderRow->GetCount(); j++)
		{
			CGridCellUI *pCellUI = (CGridCellUI *)pHeaderRow->GetItemAt(j);
			pCellUI->SetOwner(this);
			pCellUI->SetCellIndex(i, j);
			pCellUI->ApplyAttributeList(GetStyleFixedCell());
			pCellUI->InitCell();
			pCellUI->CreateInnerControl();
		}
	}

	ResetGridBody();
}

void CGridUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETFOCUS ) 
	{
		Invalidate();
	}
	else if( event.Type == UIEVENT_KILLFOCUS ) 
	{
		Invalidate();
	}
	else if( event.Type == UIEVENT_BUTTONDOWN )
	{
		m_pCellLButtonDown = GetCellUIFromPoint(event.ptMouse);
		if(m_pCellLButtonDown == NULL)
		{
			ClearSelectedCells();
			ClearSelectedRows();
		}

		m_uButtonState |= UISTATE_CAPTURED;

		if(OnSizeColumnOrRow(event))
			return;


		//列表模式允许多行选择时，或者非列表模式时，可以进行拖选
		if((IsListMode() && !IsSingleRowSelection()) || !IsListMode())
		{
			m_rcTracker.left = event.ptMouse.x;
			m_rcTracker.top = event.ptMouse.y;
			m_rcTracker.right = event.ptMouse.x;
			m_rcTracker.bottom = event.ptMouse.y;
		}

		if(m_pCellLButtonDown == NULL)
			return;

		int row = m_pCellLButtonDown->GetRow();
		int col = m_pCellLButtonDown->GetCol();
		if(IsFixedRow(row))
		{
			if(!IsFixedCol(col)) //排序
			{
				SortItems(col);
			}
		}
		else
		{
			if(IsListMode())
			{
				if(IsSingleRowSelection())  
				{	
					if(!IsSelectedRow(row))
						SelectRow(row);
					SetFocusCell(row, col);
				}
				else
				{
					//Ctrl键按下
					if(::GetKeyState(VK_CONTROL) < 0)
					{
						//当多行选中时，若是按住Ctrl键，点击已选中行时，取消选择
						if(IsSelectedRow(row))
						{
							SelectRow(row, FALSE);
							SetFocusCell(-1,-1);
						}
						else
						{
							SelectRow(row);
							SetFocusCell(row, col);
						}

					}
					else
					{
						//Ctrl键没有按下，首先清理选中行
						ClearSelectedRows();
						SelectRow(row);
						SetFocusCell(row, col);
					}
				}
			}
			else //单元格多选模式
			{
				if(IsFixedCol(col) && !IsFixedRow(row)) //点击左边固定列时，选中整行
				{
					if(::GetKeyState(VK_CONTROL) >= 0) //Ctrl键没有按下，先清理选择
						ClearSelectedCells();

					//选中整行，不包含固定列
					CGridRowUI *pRowUI = (CGridRowUI *)GetParent();
					if(pRowUI)
					{
						for (int i=1; i<pRowUI->GetCount(); i++)
						{
							CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(i);
							SelectCell(pCellUI->GetRow(), pCellUI->GetCol());
						}
					}
				}
				else
				{
					//Ctrl键按下
					if(::GetKeyState(VK_CONTROL) < 0)
					{
						//当多行选中时，若是按住Ctrl键，点击已选中单元格时，取消选择之
						if(IsSelectedCell(row, col))
							SelectCell(row, col, FALSE);
						else
							SelectCell(row, col);

					}
					else
					{
						//Ctrl键没有按下，首先清理选中单元格
						ClearSelectedCells();
						SelectCell(row, col);
					}
				}		
			}

		}

		Invalidate();
		return;
	}

	else if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
			m_uButtonState &= ~UISTATE_CAPTURED;

		if(!m_rcTracker.IsNull())
		{
			m_rcTracker.Normalize();

			//如果拖动框选在一个格子中，忽略掉。 rect is in a single cell, ignore it
			CGridCellUI *pCell1 = GetCellUIFromPoint(CDuiPoint(m_rcTracker.left, m_rcTracker.top));
			CGridCellUI *pCell2 = GetCellUIFromPoint(CDuiPoint(m_rcTracker.right, m_rcTracker.bottom));
			if(pCell1 && pCell2 && pCell1 != pCell2)
			{
				if(::GetKeyState(VK_CONTROL) >= 0) {
					ClearSelectedRows();
					ClearSelectedCells();
				}
			}

			CDuiRect rcIntersect;
			for (int i=0; i<m_pBody->GetCount(); i++)
			{
				CGridRowUI *pRowUI = (CGridRowUI *)m_pBody->GetItemAt(i);
				if(IsListMode())
				{
					if(rcIntersect.IntersectRect(m_rcTracker, pRowUI->GetPos()))
						SelectRow(i + GetFixedRowCount());
				}
				else
				{
					for (int j=GetFixedColumnCount(); j<pRowUI->GetCount(); j++)
					{
						CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
						if(rcIntersect.IntersectRect(m_rcTracker, pCellUI->GetPos()))
							SelectCell(i + GetFixedRowCount(), j);
					}
				}
			}
		}

		CGridCellUI *pCellUI = GetCellUIFromPoint(event.ptMouse);

		if(pCellUI && pCellUI == m_pCellLButtonDown)
		{
			if(GetCellType(pCellUI->GetRow(), pCellUI->GetCol()) == celltypeCheckBox)
			{
				CControlUI *pInnerControl = pCellUI->GetInnerControl();
				if(pInnerControl && pInnerControl->GetInterface(DUI_CTR_CHECKBOX))
				{
					CGridCellInnerCheckBoxUI *pCheckBox = (CGridCellInnerCheckBoxUI *)pInnerControl;
					if(pCheckBox->IsMouseEnabled() && pCheckBox->IsEnabled())
						pCheckBox->SetCheck(!pCheckBox->GetCheck());
				}
			}
		}

		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK);
		}

		m_rcTracker.Empty();
		m_pCellLButtonDown = NULL;
		Invalidate();
		return;
	}

	else if( event.Type == UIEVENT_MOUSEMOVE )
	{
		m_pHotCell = GetCellUIFromPoint(event.ptMouse);
		if(m_pHotCell)
		{
			m_pHotRow = GetRowUI(m_pHotCell->GetRow());
		}
		else
		{
			m_pHotRow = NULL;
		}

		if( (m_uButtonState & UISTATE_CAPTURED) != 0)
		{
			if(m_nSeparatorType == 1 && m_pCellLButtonDown)
			{
				int width = GetColumnWidth(m_pCellLButtonDown->GetCol());
				width -= m_ptLastMouse.x - event.ptMouse.x;
				if( width > 0 ) 
				{
					SetColumnWidth(m_pCellLButtonDown->GetCol(), width);
					m_ptLastMouse = event.ptMouse;
					NeedUpdate();
				}
				return;
			}

			if(m_nSeparatorType == 2 && m_pCellLButtonDown)
			{
				int height = GetRowHeight(m_pCellLButtonDown->GetRow());
				height -= m_ptLastMouse.y - event.ptMouse.y;
				if( height > 0 ) 
				{
					SetRowHeight(m_pCellLButtonDown->GetRow(), height);
					m_ptLastMouse = event.ptMouse;
					NeedUpdate();
				}
				return;
			}

			if((IsListMode() && !IsSingleRowSelection()) || !IsListMode())
			{
				m_rcTracker.right = event.ptMouse.x;
				m_rcTracker.bottom = event.ptMouse.y;
				Invalidate();
				return;
			}
		}
	}

	else if( event.Type == UIEVENT_RBUTTONDOWN )
	{
		CGridCellUI *pCellUI = GetCellUIFromPoint(event.ptMouse);
		if(pCellUI == NULL)
		{
			ClearSelectedCells();
			ClearSelectedRows();
			Refresh();
			return;
		}

		int row = pCellUI->GetRow();
		int col = pCellUI->GetCol();
		if(!IsFixedRow(row))
		{
			if(IsListMode())
			{
				if(!IsSelectedRow(row))
				{
					ClearSelectedRows();
					SelectRow(row);
				}
				SetFocusCell(row, col);
			}
			else
			{
				if(!IsSelectedCell(row, col))
				{
					ClearSelectedCells();
					SelectCell(row, col);
				}
			}
		}
	}

	else if(event.Type == UIEVENT_RBUTTONUP)
	{	
		return;
	}

	else if( event.Type == UIEVENT_SETCURSOR )
	{
		CGridCellUI *pCellUI = GetCellUIFromPoint(event.ptMouse);
		if(pCellUI)
		{
			RECT rcSeparatorWidth = pCellUI->GetCellPos();
			rcSeparatorWidth.left = rcSeparatorWidth.right - 8;

			RECT rcSeparatorHeight = pCellUI->GetCellPos();
			rcSeparatorHeight.top = rcSeparatorHeight.bottom - 4;

// 			RECT rc = pCellUI->GetCellPos();
// 			CMsgWndUI::InsertMsgV(_T("ptMouse.x = %d, ptMouse.y = %d, CellPos = %d,%d,%d,%d "), 
// 				event.ptMouse.x, event.ptMouse.y,
// 				rc.left, rc.top, rc.right, rc.bottom);

			if(IsEnableSizeColumn() && IsFixedRow(pCellUI->GetRow()) && ::PtInRect(&rcSeparatorWidth, event.ptMouse))
			{
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
				return;
			}
			else if(IsEnableSizeRow() && (IsFixedCol(pCellUI->GetCol()) || IsFixedRow(pCellUI->GetRow())) && ::PtInRect(&rcSeparatorHeight, event.ptMouse))
			{
				::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
				return;
			}
		}
	}

	else if(event.Type == UIEVENT_DBLCLICK)
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
		{
			//InsertMsgUI(_T("grid dbclick"));
			if(GetManager()) GetManager()->SendNotify(this, DUI_MSGTYPE_DBCLICK, 0, 0, true);
		}
		return;
	}

	__super::DoEvent(event);
}

bool CGridUI::OnSizeColumnOrRow(TEventUI& event)
{
	if(m_pCellLButtonDown == NULL) return false;

	RECT rcSeparatorWidth = m_pCellLButtonDown->GetCellPos();
	rcSeparatorWidth.left = rcSeparatorWidth.right - 8;

	RECT rcSeparatorHeight = m_pCellLButtonDown->GetCellPos();
	rcSeparatorHeight.top = rcSeparatorHeight.bottom - 4;

	if( ::PtInRect(&rcSeparatorWidth, event.ptMouse) && IsFixedRow(m_pCellLButtonDown->GetRow()) && IsEnableSizeColumn()) 
	{
		m_nSeparatorType = 1;
		m_ptLastMouse = event.ptMouse;
		return true;
	}
	else if( IsEnabled() && IsEnableSizeRow() && (IsFixedCol(m_pCellLButtonDown->GetCol()) || IsFixedRow(m_pCellLButtonDown->GetRow())) && ::PtInRect(&rcSeparatorHeight, event.ptMouse) ) 
	{
		m_nSeparatorType = 2;
		m_ptLastMouse = event.ptMouse;
		return true;
	}

	m_nSeparatorType = 0;
	return false;
}

void CGridUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	rc = m_rcItem;

	// Adjust for inset
	RECT rcInset = GetInset();
	rc.left += rcInset.left;
	rc.top += rcInset.top;
	rc.right -= rcInset.right;
	rc.bottom -= rcInset.bottom;
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

	if( m_items.GetSize() == 0) {
		ProcessScrollBar(rc, 0, 0);
		return;
	}

	SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
	int iPosX = rc.left;
	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() && !IsFitColumns()) 
	{
		szAvailable.cx += m_pHorizontalScrollBar->GetScrollRange();
		iPosX -= m_pHorizontalScrollBar->GetScrollPos();
	}

	if(IsFitRows() && GetRowCount() > 0)
	{
		int rowCount = GetRowCount();
		int nAverage = (rc.bottom - rc.top -1) / rowCount;
		for (int i=0; i<rowCount; i++)
		{
			SetRowHeight(i, nAverage);
		}
	}

	//自动调整表格最后一列，使铺满表格
	if(IsExpandLastColumn() && GetColumnCount() > 0 && GetColumnCount() < MAX_GRID_COLUMN_COUNT)
	{
		int cxFixed = 0;
		int colCount = GetColumnCount();
		for (int i=0; i<colCount-1; i++)
		{
			cxFixed += GetColumnWidth(i);
		}

		//最后一列大于默认列宽才能扩展
		if(rc.right - rc.left - cxFixed > GetDefColWidth())
		{
			m_mapColWidthFixed[colCount-1] = rc.right - rc.left - cxFixed - 1;

		}
		else
			m_mapColWidthFixed[colCount-1] = GetDefColWidth();
	}
	//自动调整表格所有列，使铺满表格
	else if(IsFitColumns() && GetColumnCount() > 0 && GetColumnCount() < MAX_GRID_COLUMN_COUNT)
	{
		int cxFixed = 0;
		int nFixedCount = 0;
		for (int i=0; i<GetColumnCount(); i++)
		{
			if(m_mapColWidthFixed[i] > 0)
			{
				cxFixed += m_mapColWidthFixed[i];
				nFixedCount++;
			}
		}
		int nNoFixedCols = GetColumnCount() - nFixedCount;
		if(nNoFixedCols > 0)
		{
			int nAverage = ( szAvailable.cx - cxFixed ) / nNoFixedCols;
			for (int i=0; i<GetColumnCount(); i++)
			{
				if(m_mapColWidthFixed[i] == 0)
				{
					if(i == GetColumnCount() - 1)
						m_mapColumnWidth[i] = ( szAvailable.cx - cxFixed ) - nAverage * (nNoFixedCols - 1) ;
					else
						m_mapColumnWidth[i] = nAverage;	
				}
			}
		}
	}

	int nGridWidth = 0;
	for (int i=0; i<GetColumnCount(); i++)
	{
		nGridWidth += GetColumnWidth(i);
	}

	SIZE szHeader = m_pHeader->EstimateSize(szAvailable);
	RECT rcCtrl = {iPosX, rc.top, iPosX + nGridWidth, rc.top + szHeader.cy };
	m_pHeader->SetPos(rcCtrl);

	szAvailable.cy -= szHeader.cy;
	SIZE szBody = m_pBody->EstimateSize(szAvailable);
	RECT rcCtrl2 = {iPosX, rc.top+szHeader.cy, iPosX + nGridWidth, rc.bottom};
	m_pBody->SetPos(rcCtrl2);

	int cxNeeded = nGridWidth;
	int cyNeeded = m_pBody->GetCyNeeded();
	ProcessScrollBar(rc, cxNeeded, m_pBody->GetCyNeeded());
}

void CGridUI::BuildRows(RECT rc, bool bNeedInvalidate)
{
	CDuiRect rcItem = rc;

	int iPosY = rc.top;
	if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible() && !IsFitRows())
	{
		iPosY -= GetVerticalScrollBar()->GetScrollPos();
	}

	//显示的第一行是哪个？
	int cyNeeded = 0;
	int nBeginRow = GetFixedRowCount();
	int nRows = GetRowCount();
	BOOL bFindBeginRow = FALSE;
	for (int i=nBeginRow; i<nRows; i++)
	{
		int nHeight = GetRowHeight(i);
		if(!bFindBeginRow)
		{
			if(iPosY + nHeight > rc.top)
			{
				nBeginRow = i;
				bFindBeginRow = TRUE;
			}
			else
			{
				iPosY += nHeight;
			}
		}
		cyNeeded += nHeight;
	}
	m_pBody->SetCyNeeded(cyNeeded);

	//可以显示几行？
	int nNeedRows = 0;
	int nTempRow = nBeginRow;
	int iTempPosY = iPosY;
	while (iTempPosY < rcItem.bottom && nTempRow < GetRowCount())
	{
		iTempPosY += GetRowHeight(nTempRow);
		nNeedRows++;
		nTempRow++;
	}

	//创建实际的GridRowUI
	int addedRows = nNeedRows - m_pBody->GetCount();
	if(addedRows > 0)
	{
		while(m_pBody->GetCount() < nNeedRows) 
		{
			CGridRowUI *pRow = new CGridRowUI();
			pRow->SetOwner(this);
			m_pBody->Add(pRow);
			pRow->ApplyAttributeList(GetStyleRow());
			for (int i=0; i<GetColumnCount(); i++)
			{
				CGridCellUI *pCell = new CGridCellUI;
				pRow->Add(pCell);
				if(i < GetFixedColumnCount())
					pCell->ApplyAttributeList(GetStyleFixedCell());
				else
					pCell->ApplyAttributeList(GetStyleCell());
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

	//GridRowUI SetPos，初始化数据
	int nCurrentRow = nBeginRow;
	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)m_pBody->GetItemAt(i);
		RECT rcRow = { rcItem.left, iPosY, rcItem.right, iPosY + GetRowHeight(nCurrentRow) };
		pRowUI->SetPos(rcRow, bNeedInvalidate);

		if(pRowUI->GetRow() != nCurrentRow)
		{
			pRowUI->SetRowIndex(nCurrentRow);
			for (int j=0; j<pRowUI->GetCount(); j++)
			{
				CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
				pCellUI->SetCellIndex(nCurrentRow, j);

				GridCellType cellType = GetCellType(pCellUI->GetRow(), pCellUI->GetCol());
				if(cellType == celltypeCheckBox)
					pCellUI->CreateInnerControl();
				else if(cellType == celltypePicture)
				{
					pCellUI->CreateInnerControl();
					CControlUI *pInnerControl = pCellUI->GetInnerControl();
					if(pInnerControl && pInnerControl->GetInterface(DUI_CTR_PICTURE))
					{
						CPictureUI *pic = (CPictureUI *)pInnerControl;
						CDuiString sText = pCellUI->GetText();
						pic->LoadFile(sText);
					}
				}
				else if(cellType == celltypeContainer)
				{
					//celltypeContainer类型时，发送消息出去，可以在消息响应中插入自定义控件
					if(pCellUI->GetCount() == 0)
						GetManager()->SendNotify(this, DUI_MSGTYPE_INITCELL, nCurrentRow, j);
				}
			}
			GetManager()->SendNotify(this, DUI_MSGTYPE_INITROWDATA, nCurrentRow, 0);
		}

		iPosY += GetRowHeight(nCurrentRow);
		nCurrentRow++;
	}

	//虚表模式时，发送消息出去，让调用方填充CGridCellUI的数据
	if(IsVirtualGrid())
	{
		OnDrawItem(nBeginRow, nBeginRow+m_pBody->GetCount()-1);
	}
}

void CGridUI::OnDrawItem(int nBeginRow, int nEndRow)
{
	GetManager()->SendNotify(this, DUI_MSGTYPE_DRAWITEM, nBeginRow, nEndRow);
}

void CGridUI::SetScrollPos(SIZE szPos, bool bMsg)
{
	int cx = 0;
	int cy = 0;

	//just scroll body's VerticalScrollBar
	if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
	{
		int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
		m_pVerticalScrollBar->SetScrollPos(szPos.cy);
		cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
		int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
		m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
		cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
	}

	if( cx == 0 && cy == 0 ) return;

	RECT rcPos;
	for( int it2 = 0; it2 < m_items.GetSize(); it2++ ) {
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it2]);
		if( !pControl->IsVisible() ) continue;
		if( pControl->IsFloat() ) continue;

		rcPos = pControl->GetPos();
		rcPos.left -= cx;
		rcPos.right -= cx;
		//  	rcPos.top -= cy;	//don't scroll grid's VerticalScrollBar
		//  	rcPos.bottom -= cy;
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

void CGridUI::ProcessScrollBar(RECT rc, int cxRequired, int cyRequired)
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
	int nBodyHeight = rcBody.bottom - rcBody.top;
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

bool CGridUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	bool bPaint = __super::DoPaint(hDC, rcPaint, pStopControl);
	if(!bPaint) return false;

	//paint merge cells here
	for (int i=0; i<m_pHeader->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pHeader->GetItemAt(i);
		for (int j=0; j<pRow->GetCount(); j++)
		{
			CGridCellUI *pCell = (CGridCellUI *)pRow->GetItemAt(j);
			if(IsMergedCell(pCell->GetRow(), pCell->GetCol()))
			{
				pCell->PaintBkColor(hDC);
				pCell->PaintBkImage(hDC);
				pCell->PaintStatusImage(hDC);
				pCell->PaintForeColor(hDC);
				pCell->PaintForeImage(hDC);
				pCell->PaintText(hDC);
				pCell->PaintBorder(hDC);

				if(pCell->GetInnerControl())
				{
					CDuiRect rcCell = pCell->GetCellPos();
					CDuiRect rcItem = pCell->GetInnerControl()->GetPos();

					POINT ptCenter = rcCell.CenterPoint();
					CDuiRect rcItemNew;
					rcItemNew.left = ptCenter.x - rcItem.GetWidth()/2;
					rcItemNew.right = ptCenter.x + rcItem.GetWidth()/2;
					rcItemNew.top = ptCenter.y - rcItem.GetHeight()/2;
					rcItemNew.bottom = ptCenter.y + rcItem.GetHeight()/2;

					if(rcItem.left == rcItemNew.left && 
						rcItem.right == rcItemNew.right && 
						rcItem.top == rcItemNew.top && 
						rcItem.bottom == rcItemNew.bottom )
					{
						pCell->GetInnerControl()->Paint(hDC, rcCell, pStopControl);
					}
					else
					{
						pCell->GetInnerControl()->SetPos(rcItemNew,false);
						pCell->GetInnerControl()->Paint(hDC, rcCell, pStopControl);
					}			
				}
			}
		}
	}

	for (int i=0; i<m_pBody->GetCount(); i++)
	{
		CGridRowUI *pRow = (CGridRowUI *)m_pBody->GetItemAt(i);

		for (int j=0; j<pRow->GetCount(); j++)
		{
			CGridCellUI *pCell = (CGridCellUI *)pRow->GetItemAt(j);
			if(IsMergedCell(pCell->GetRow(), pCell->GetCol()))
			{
				pCell->PaintBkColor(hDC);
				pCell->PaintBkImage(hDC);
				pCell->PaintStatusImage(hDC);
				pCell->PaintForeColor(hDC);
				pCell->PaintForeImage(hDC);
				pCell->PaintText(hDC);
				pCell->PaintBorder(hDC);
			}
		}
	}	

	if(!m_rcTracker.IsNull())
	{
		CDuiRect rcRect = m_rcTracker;
		rcRect.Normalize();
		CRenderEngine::DrawRect(hDC, rcRect, 1, GetAdjustColor(GetLineColor()), PS_SOLID);
	}

	RECT rcTemp = { 0 };
	if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
		if( m_pVerticalScrollBar == pStopControl ) return false;
		if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
			if( !m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
		}
	}
	PaintBorder(hDC);
	return bPaint;
}

void CGridUI::PaintBorder(HDC hDC)
{
	if(GetLineColor() != 0)
	{
		CRenderEngine::DrawRect(hDC, m_rcItem, 1, GetAdjustColor(GetLineColor()), PS_SOLID);
	}
}

void CGridUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("rowheight")) == 0 )
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
	else if( _tcsicmp(pstrName, _T("sizecolumn")) == 0 )	
	{
		EnableSizeColumn(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("sizerow")) == 0 )
	{
		EnableSizeRow(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("fitcolumnbytext")) == 0 )	
	{
		SetExpandColumnByText(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("fitlastcolumn")) == 0 )	
	{
		SetExpandLastColumn(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("fitcolumns")) == 0 )	
	{
		SetFitColumns(_tcsicmp(pstrValue, _T("true")) == 0);
	}	
	else if( _tcsicmp(pstrName, _T("fitrows")) == 0 )
	{
		SetFitRows(_tcsicmp(pstrValue, _T("true")) == 0);
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
	else if( _tcsicmp(pstrName, _T("fixedrow")) == 0 )
	{
		SetStyleFixedRow(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("fixedcell")) == 0 )
	{
		SetStyleFixedCell(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("row")) == 0 )
	{
		SetStyleRow(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("cell")) == 0 )
	{
		SetStyleCell(pstrValue);
	}
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
	else if( _tcsicmp(pstrName, _T("picture")) == 0 )
	{
		SetStylePicture(pstrValue);
	}
	else 
		__super::SetAttribute(pstrName, pstrValue);
}

}