#include "StdAfx.h"
#include "FormGridList.h"


CFormGridList::CFormGridList(void)
{
	
}


CFormGridList::~CFormGridList(void)
{
}

void CFormGridList::InitWindow()
{
	UI_BINDCONTROL(CGridUI, m_pGrid, _T("grid_main"));

	m_pGrid->SetCellType(1, celltypeCheckBox);
	m_pGrid->SetCellType(2, celltypeText);
	m_pGrid->SetCellType(3, celltypeEdit);
	m_pGrid->SetCellType(4, celltypeCombo);
	m_pGrid->SetCellType(5, celltypeDateTime);
	m_pGrid->SetCellType(6, celltypeDate);
	m_pGrid->SetCellType(7, celltypeTime);
	m_pGrid->SetCellType(8, celltypeContainer);
	m_pGrid->SetCellType(9, celltypePicture);

	m_pGrid->SetRowCount(1 * 20000);
	for (int i=m_pGrid->GetFixedRowCount(); i<m_pGrid->GetRowCount(); i++)
	{
		for (int j=1; j<m_pGrid->GetColumnCount(); j++)
		{
			CString temp; 
			temp.Format(_T("%02d,%02d"), i, j);
			m_pGrid->Cell(i,j).SetText(temp);
		}
	}

	m_pGrid->SetColumnSort(1, FALSE);
	m_pGrid->SetVirtualGrid(TRUE);
	m_pGrid->SetRowCount(10 * 10000);
}

void CFormGridList::OnNotifyClick(TNotifyUI& msg)
{
	if(IsControl(msg, _T("grid_innerbutton_modify")))
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
}

void CFormGridList::OnNotifyInitCell(TNotifyUI& msg)
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

void CFormGridList::OnNotifyDrawItem(TNotifyUI& msg)
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
							pic->SetFixedWidth(32);
							pic->SetFixedHeight(32);
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
							pic->SetFixedWidth(32);
							pic->SetFixedHeight(32);
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
