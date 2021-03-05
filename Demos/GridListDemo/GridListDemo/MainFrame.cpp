#include "StdAfx.h"
#include "MainFrame.h"

CMainFrame::CMainFrame(void)
{
	m_bVirtualGrid = FALSE;
	m_nRows = 100;
	m_nCols = 1;
	m_nFixedRows = 1;
	m_nFixedCols = 1;
}


CMainFrame::~CMainFrame(void)
{
}


void CMainFrame::InitWindow()
{
	UI_BINDCONTROL(CGridListUI, m_pGrid, _T("grid_main"));

	m_pGrid->SetRowCount(12);
	for (int i=0; i<m_pGrid->GetRowCount(); i++)
	{
		for (int j=1; j<m_pGrid->GetColumnCount(); j++)
		{
			CString temp; 
			temp.Format(_T("%02d,%02d"), i, j);
			m_pGrid->Cell(i,j).SetText(temp);
		}
	}

	ddxSetManager(GetManager());
	ddxCheckBox(_T("chk_virtual_grid"), m_bVirtualGrid);
	ddxText(_T("edit_rows"), m_nRows);
	ddxText(_T("edit_cols"), m_nCols);
	ddxText(_T("edit_fixed_rows"), m_nFixedRows);
	ddxText(_T("edit_fixed_cols"), m_nFixedCols);
	ddxCheckBox(_T("chk_list_mode"), m_bListMode);
	ddxCheckBox(_T("chk_single_row_selection"), m_bSingleRowSelection);
	ddxCheckBox(_T("chk_view_list_no"), m_bViewListNumber);
	ddxCheckBox(_T("chk_view_grid_row_line"), m_bViewRowLine);
	ddxCheckBox(_T("chk_view_grid_col_line"), m_bViewColLine);
	ddxCheckBox(_T("chk_select_rect"), m_bSelectRect);

	
	GetGridPara();
	m_pGrid->Cell(1,1).SetCellType(celltypeCheckBox);
	m_pGrid->SetColumnCellType(1, celltypeCheckBox);
	m_pGrid->SetColumnCellType(2, celltypeEdit);
	m_pGrid->SetColumnCellType(3, celltypeCombo);
	m_pGrid->SetColumnCellType(4, celltypeDateTime);
	m_pGrid->SetColumnCellType(5, celltypeDate);
	m_pGrid->SetColumnCellType(6, celltypeTime);

	InsertMsgUiV(_T("sizeof the grid element: gridlist=%d, gridheader=%d, gridbody=%d, gridrow=%d, gridcell=%d"),
				sizeof(CGridListUI), sizeof(CGridListHeaderUI), sizeof(CGridListBodyUI), sizeof(CGridListRowUI), sizeof(CGridListCellUI));

	InsertMsgUiV(_T("size, CHorizontalLayoutUI=%d, CVerticalLayoutUI=%d"), sizeof(CHorizontalLayoutUI), sizeof(CVerticalLayoutUI));
}

void CMainFrame::GetGridPara()
{
	m_bVirtualGrid = m_pGrid->IsVirtualGrid();
	m_nRows = m_pGrid->GetRowCount();
	m_nCols = m_pGrid->GetColumnCount();
	m_nFixedRows = m_pGrid->GetFixedRowCount();
	m_nFixedCols = m_pGrid->GetFixedColumnCount();
	m_bListMode = m_pGrid->IsListMode();
	m_bSingleRowSelection = m_pGrid->IsSingleRowSelection();
	m_bViewListNumber = m_pGrid->IsViewListNumber();
	m_bViewRowLine = m_pGrid->IsDrawRowLine();
	m_bViewColLine = m_pGrid->IsDrawColumnLine();
	m_bSelectRect = m_pGrid->IsEnableSelectRect();
	UpdateDataUI(false);
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{
	UpdateDataUI(true);

	if(IsControl(msg, _T("chk_virtual_grid")))
	{
		m_pGrid->SetVirtualGrid(m_bVirtualGrid);
		m_pGrid->SetRowCount(m_nRows);
	}
	else if(IsControl(msg, _T("btn_set_rows")))
	{
		m_pGrid->SetRowCount(m_nRows);
	}
	else if(IsControl(msg, _T("btn_set_cols")))
	{
		m_pGrid->SetColumnCount(m_nCols);
	}
	else if(IsControl(msg, _T("btn_set_fixed_rows")))
	{
		m_pGrid->SetFixedRowCount(m_nFixedRows);
	}
	else if(IsControl(msg, _T("btn_set_fixed_cols")))
	{
		m_pGrid->SetFixedColumnCount(m_nFixedCols);
	}
	else if(IsControl(msg, _T("chk_list_mode")))
	{
		m_pGrid->SetListMode(m_bListMode);
	}
	else if(IsControl(msg, _T("chk_single_row_selection")))
	{
		m_pGrid->SetSingleRowSelection(m_bSingleRowSelection);
	}
	else if(IsControl(msg, _T("chk_view_list_no")))
	{
		m_pGrid->SetViewListNumber(m_bViewListNumber);
	}
	else if(IsControl(msg, _T("chk_view_grid_row_line")))
	{
		m_pGrid->EnableDrawRowLine(m_bViewRowLine);
	}
	else if(IsControl(msg, _T("chk_view_grid_col_line")))
	{
		m_pGrid->EnableDrawColumnLine(m_bViewColLine);
	}
	else if(IsControl(msg, _T("chk_select_rect")))
	{
		m_pGrid->EnableSelectRect(m_bSelectRect);
	}
	else if(IsControl(msg, _T("btn_save_xml")))
	{
		m_pGrid->SaveXmlFile();
	}
	else if(IsControl(msg, _T("btn_load_xml")))
	{
		m_pGrid->LoadXmlFile();
		GetGridPara();
	}
	else if(IsControl(msg, _T("btn_save_excel")))
	{
		m_pGrid->SaveExcelFile();
	}
	else if(IsControl(msg, _T("btn_load_excel")))
	{
		m_pGrid->LoadExcelFile();
		GetGridPara();
	}
	else if(IsControl(msg, _T("btn_cell_type")))
	{
		CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_cell_type"));
		int nType = pCombo->GetCurSel();
		for (CGridListCellUI *pCell=m_pGrid->GetSelectCell(); pCell; pCell=m_pGrid->GetNextSelectCell())
		{
			pCell->SetCellType(GridListCellType(nType));
		}
	}
	else if(IsControl(msg, _T("btn_column_type")))
	{
		CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_column_type"));
		int nType = pCombo->GetCurSel();
		TGridCell cell = m_pGrid->GetFocusCell();
		if(cell.IsValid())
		{
			m_pGrid->SetColumnCellType(cell.col, GridListCellType(nType));
		}
	}
	else if(IsControl(msg, _T("btn_add_row")))
	{
		int row = m_pGrid->InsertRow();
		for (int j=1; j<m_pGrid->GetColumnCount(); j++)
		{
			CString temp; 
			temp.Format(_T("%02d,%02d"), row, j);
			m_pGrid->Cell(row,j).SetText(temp);
		}
	}
	else if(IsControl(msg, _T("btn_delete_row")))
	{
		int row = m_pGrid->GetSelectRow();
		if(row >= m_pGrid->GetFixedRowCount())
			m_pGrid->DeleteRow(row);
	}
	else if(IsControl(msg, _T("btn_add_column")))
	{
		m_pGrid->InsertColumn();
	}
	else if(IsControl(msg, _T("btn_delete_column")))
	{
		TGridCell cell = m_pGrid->GetFocusCell();
		if(cell.IsValid())
		{
			m_pGrid->DeleteColumn(cell.col);
		}
	}
	else if(IsControl(msg, _T("grid_main")))
	{
		TGridCell cell = m_pGrid->GetFocusCell();
		if(cell.IsValid())
		{
			CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_cell_type"));
			GridListCellType cellType = m_pGrid->GetCell(cell.row, cell.col)->GetCellType();
			pCombo->SelectItem(cellType);

			CComboExUI *pComboCol = (CComboExUI *)FindControl(_T("cb_column_type"));
			GridListCellType cellColumn = m_pGrid->GetColumnCellType(cell.col);
			pComboCol->SelectItem(cellColumn);
		}
	}
	m_pGrid->Refresh();

}

void CMainFrame::OnNotifyRClick(TNotifyUI& msg)
{
	CString temp;

	if(IsControl(msg, _T("grid_main")))
	{
		
	}
}

void CMainFrame::OnNotifyDrawItem(TNotifyUI& msg)
{
	//for virtual grid, 
	if(IsControl(msg, _T("grid_main")))
	{
		int lo = msg.wParam;	//begin row
		int hi = msg.lParam;	//end row

		//InsertMsgUiV(_T("DrawRange=%d,%d"), lo, hi);

		int sort_col = m_pGrid->GetSortColumn();
		BOOL bAscending = m_pGrid->GetSortAscending();

		//fill grid content
		for (int i=lo; i<=hi; i++)
		{
			for (int j=1; j<m_pGrid->GetColumnCount(); j++)
			{
				if(sort_col > 0 && !bAscending)
				{
					CDuiString s;
					s.Format(_T("%d,%d"), m_pGrid->GetRowCount()-i, j);
					m_pGrid->Cell(i,j).SetText(s);
				}
				else
				{
					CDuiString s;
					s.Format(_T("%d,%d"), i, j);
					m_pGrid->Cell(i,j).SetText(s);
				}
			}
		}
	}
}


void CMainFrame::OnNotifySortItem(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		//for virtual grid, when a sort message is received, we have to refresh the virtual order.
		//and then we receive a drawitem message to sort the local data
		if(m_pGrid->IsVirtualGrid())
		{
			m_pGrid->ResetVirtualOrder();
		}
	}
}

void CMainFrame::OnNotifyItemSelect(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CDuiString s;
		s.Format(_T("combo cell select item, row=%d, col=%d"), msg.wParam, msg.lParam);
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifySelectChanged(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CGridListCellUI *pCell = m_pGrid->GetCell(msg.wParam, msg.lParam);
		CCheckBoxUI *pCheckBox = (CCheckBoxUI *)pCell->GetInnerControl();
		CDuiString s;
		s.Format(_T("checkbox cell selectchanged, row=%d, col=%d, %s"), msg.wParam, msg.lParam, pCheckBox->IsSelected() ? _T("Select") : _T("UnSelect"));
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifyPreDropDown(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CGridListCellUI *pCell = m_pGrid->GetCell(msg.wParam, msg.lParam);
		CComboExUI *pCombo = (CComboExUI *)pCell->GetInnerControl();

		CDuiString s;
		s.Format(_T("combo cell predropdown, row=%d, col=%d"), msg.wParam, msg.lParam);
		InsertMsgUI(s);

		pCombo->RemoveAll();
		for (int i=0; i<10; i++)
		{
			CString s;
			s.Format(_T("combo text %d"), i);
			pCombo->AddString(s);
		}
		
	}
}

void CMainFrame::OnNotifyDropDown(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CDuiString s;
		s.Format(_T("combo cell dropdown, row=%d, col=%d"), msg.wParam, msg.lParam);
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifyStartEdit(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CDuiString s;
		s.Format(_T("edit cell start edit, row=%d, col=%d"), msg.wParam, msg.lParam);
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifyEndEdit(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CDuiString s;
		s.Format(_T("edit cell end edit, row=%d, col=%d"), msg.wParam, msg.lParam);
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifyTextChanged(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CGridListCellUI *pCell = m_pGrid->GetCell(msg.wParam, msg.lParam);
		CRichEditUI *pRich = (CRichEditUI *)pCell->GetInnerControl();

		CDuiString s;
		s.Format(_T("edit cell text changed, row=%d, col=%d, text=%s"), msg.wParam, msg.lParam, pRich->GetText());
		InsertMsgUI(s);
	}
}