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
	UI_BINDCONTROL(CGridUI, m_pGrid, _T("grid_main"));

	m_pGrid->SetRowCount(10);
	for (int i=m_pGrid->GetFixedRowCount(); i<m_pGrid->GetRowCount(); i++)
	{
		for (int j=1; j<m_pGrid->GetColumnCount(); j++)
		{
			if(j == 9)
			{
				m_pGrid->Cell(i,j).SetText(_T("images\\ico.jpg"));
			}
			else
			{
				m_pGrid->Cell(i,j).SetTextV(_T("%02d,%02d"), i, j);
			}
		}
	}
// 	m_pGrid->SetVirtualGrid(TRUE);
// 	m_pGrid->SetRowCount(10 * 10000);

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
	
	GetGridPara();
	m_pGrid->SetCellType(1, celltypeCheckBox);
	//m_pGrid->SetCellType(2, celltypeText);
	m_pGrid->SetCellType(3, celltypeEdit);
	m_pGrid->SetCellType(4, celltypeCombo);
	m_pGrid->SetCellType(5, celltypeDateTime);
	m_pGrid->SetCellType(6, celltypeDate);
	m_pGrid->SetCellType(7, celltypeTime);
	m_pGrid->SetCellType(8, celltypeContainer);
	m_pGrid->SetCellType(9, celltypePicture);

	//剔除不允许排序的列
	m_pGrid->SetColumnSort(1, FALSE);
	m_pGrid->SetColumnSort(8, FALSE);
	m_pGrid->SetColumnSort(9, FALSE);

	m_pGrid->MergeCells(0,0,1,0);
	m_pGrid->MergeCells(0,1,1,1);
	m_pGrid->MergeCells(0,2,1,2);
	m_pGrid->MergeCells(0,3,1,3);
	m_pGrid->MergeCells(0,4,1,4);
	m_pGrid->MergeCells(0,8,1,8);
	m_pGrid->MergeCells(0,9,1,9);

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
	else if(IsControl(msg, _T("btn_save_xml")))
	{
		UIGrid_SaveXmlFile(m_pGrid);
	}
	else if(IsControl(msg, _T("btn_load_xml")))
	{
		UIGrid_LoadXmlFile(m_pGrid);
		GetGridPara();
	}
	else if(IsControl(msg, _T("btn_save_excel")))
	{
		UIGrid_SaveExcelFile(m_pGrid);
	}
	else if(IsControl(msg, _T("btn_load_excel")))
	{
		UIGrid_LoadExcelFile(m_pGrid);
		GetGridPara();
	}
	else if(IsControl(msg, _T("btn_cell_type")))
	{
		CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_cell_type"));
		int nType = pCombo->GetCurSel();

		TCellID cellID = m_pGrid->GetFocusCell();
		m_pGrid->SetCellType(cellID.row, cellID.col, GridCellType(nType));
	}
	else if(IsControl(msg, _T("btn_column_type")))
	{
		CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_column_type"));
		int nType = pCombo->GetCurSel();
		TCellID cell = m_pGrid->GetFocusCell();
		if(cell.IsValid())
		{
			m_pGrid->SetCellType(cell.col, GridCellType(nType));
		}
	}
	else if(IsControl(msg, _T("btn_add_row")))
	{
		int row = m_pGrid->InsertRow();
		for (int j=1; j<m_pGrid->GetColumnCount(); j++)
		{
			if(j == 9)
			{
				m_pGrid->Cell(row,j).SetText(_T("images\\ico.jpg"));
			}
			else
			{
				m_pGrid->Cell(row,j).SetTextV(_T("%02d,%02d"), row, j);
			}
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
		TCellID cell = m_pGrid->GetFocusCell();
		if(cell.IsValid())
		{
			m_pGrid->DeleteColumn(cell.col);
		}
	}
	else if(IsControl(msg, _T("grid_innerbutton_modify")))
	{
		if(msg.pSender->GetParent() && msg.pSender->GetParent()->GetInterface(DUI_CTR_GRIDCELL))
		{
			CGridCellUI *pCellUI = (CGridCellUI *)msg.pSender->GetParent();
			CString temp;
			temp.Format(_T("Click modify button, row=%d, col=%d"), pCellUI->GetRow(), pCellUI->GetCol());
			MessageBox(GetManager()->GetPaintWindow(), temp, _T("Msg"), MB_OK);
		}
	}
	else if(IsControl(msg, _T("grid_innerbutton_delete")))
	{
		if(msg.pSender->GetParent() && msg.pSender->GetParent()->GetInterface(DUI_CTR_GRIDCELL))
		{
			CGridCellUI *pCellUI = (CGridCellUI *)msg.pSender->GetParent();
			CString temp;
			temp.Format(_T("Click delete button, row=%d, col=%d"), pCellUI->GetRow(), pCellUI->GetCol());
			MessageBox(GetManager()->GetPaintWindow(), temp, _T("Msg"), MB_OK);
		}
	}
	else if(IsControl(msg, _T("grid_main")))
	{
		TCellID cellID = m_pGrid->GetFocusCell();
		if(cellID.IsValid())
		{
			CComboExUI *pCombo = (CComboExUI *)FindControl(_T("cb_cell_type"));
			GridCellType cellType = m_pGrid->GetCellType(cellID.row, cellID.col);
			pCombo->SelectItem(cellType);

			CComboExUI *pComboCol = (CComboExUI *)FindControl(_T("cb_column_type"));
			GridCellType cellColumn = m_pGrid->GetCellType(cellID.col);
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

		InsertMsgUiV(_T("DrawRange=%d,%d"), lo, hi);

		int sort_col = m_pGrid->GetSortColumn();
		BOOL bAscending = m_pGrid->GetSortAscending();

		//fill grid content
		for (int i=lo; i<=hi; i++)
		{
			for (int j=1; j<m_pGrid->GetColumnCount(); j++)
			{
				CGridCellUI *pCellUI = m_pGrid->GetCellUI(i,j);
				if(sort_col > 0 && !bAscending)
				{
					if(j == 9)
					{
						CControlUI *pInnerControl = pCellUI->GetInnerControl();
						if(pInnerControl && pInnerControl->GetInterface(DUI_CTR_PICTURE))
						{
							CPictureUI *pic = (CPictureUI *)pInnerControl;
							pic->LoadFile(_T("images\\ico.jpg"));
						}
					}
					else
					{
						pCellUI->SetTextV(_T("%d,%d"), m_pGrid->GetRowCount()-i, j);
					}
				}
				else
				{
					if(j == 9)
					{
						CControlUI *pInnerControl = pCellUI->GetInnerControl();
						if(pInnerControl && pInnerControl->GetInterface(DUI_CTR_PICTURE))
						{
							CPictureUI *pic = (CPictureUI *)pInnerControl;
							pic->LoadFile(_T("images\\ico.jpg"));
						}
					}
					else
					{
						pCellUI->SetTextV(_T("%d,%d"), i, j);
					}
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
			//这时候可以对本地数据进行排序, 接着会触发OnNotifyDrawItem刷新显示。
			m_pGrid->Refresh(true);
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
		TCellData *pCellData = m_pGrid->GetCellData(msg.wParam, msg.lParam);
		CDuiString s;
		s.Format(_T("checkbox cell selectchanged, row=%d, col=%d, %s"), msg.wParam, msg.lParam, pCellData->IsCheckBoxCheck() ? _T("Select") : _T("UnSelect"));
		InsertMsgUI(s);
	}
}

void CMainFrame::OnNotifyPreDropDown(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		CGridCellUI *pCellUI = m_pGrid->GetCellUI(msg.wParam, msg.lParam);
		CComboExUI *pCombo = (CComboExUI *)pCellUI->GetInnerControl();

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
		TCellData *pCellData = m_pGrid->GetCellData(msg.wParam, msg.lParam);

		CDuiString s;
		s.Format(_T("edit cell text changed, row=%d, col=%d, text=%s"), msg.wParam, msg.lParam, pCellData->GetText());
		InsertMsgUI(s);
	}
}


void CMainFrame::OnNotifyInitCell(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_main")))
	{
		int row = msg.wParam;
		int col = msg.lParam;
		CGridCellUI *pCellUI = m_pGrid->GetCellUI(row, col);
		if(!pCellUI) return;

		//在表格第8列，插入2个按钮
		if(col == 8)
		{
			pCellUI->SetChildPadding(10);

			CButtonUI *pButtonModify = new CButtonUI;
			pCellUI->Add(pButtonModify);
			pButtonModify->ApplyAttributeList(_T("style_button"));
			pButtonModify->SetForeImage(_T("images\\edtico.png"));
			pButtonModify->SetFixedWidth(16);
			pButtonModify->SetFixedHeight(16);
			pButtonModify->SetName(_T("grid_innerbutton_modify"));

			CButtonUI *pButtonDelete = new CButtonUI;
			pCellUI->Add(pButtonDelete);
			pButtonDelete->ApplyAttributeList(_T("style_button"));
			pButtonDelete->SetForeImage(_T("images\\delico.png"));
			pButtonDelete->SetFixedWidth(16);
			pButtonDelete->SetFixedHeight(16);
			pButtonDelete->SetName(_T("grid_innerbutton_delete"));	
		}
	}
}