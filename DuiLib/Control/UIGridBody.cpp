#include "StdAfx.h"
#include "UIGridBody.h"
#include "UIGrid.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CGridBodyUI)

CGridBodyUI::CGridBodyUI(void) : m_pOwner(NULL)
{
	EnableScrollBar(false,false);
	m_nCyNeeded = 0;
}


CGridBodyUI::~CGridBodyUI(void)
{
}

LPCTSTR CGridBodyUI::GetClass() const
{
	return DUI_CTR_GRIDBODY;
}

UINT CGridBodyUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridBodyUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDBODY) == 0 ) return static_cast<CGridBodyUI*>(this);
	return __super::GetInterface(pstrName);
}

bool CGridBodyUI::Add(CControlUI* pControl)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDROW))
		return false;

	CGridRowUI *pRowUI = static_cast<CGridRowUI *>(pControl);
	pRowUI->SetOwner(GetOwner());

	return __super::Add(pControl);
}

bool CGridBodyUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDROW))
		return false;

	CGridRowUI *pRowUI = static_cast<CGridRowUI *>(pControl);
	pRowUI->SetOwner(GetOwner());

	return __super::AddAt(pControl, iIndex);
}

void CGridBodyUI::DoInit()
{
}

void CGridBodyUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	//return __super::SetPos(rc, bNeedInvalidate);
	CControlUI::SetPos(rc, bNeedInvalidate);
	if(!GetOwner()) return;

	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->BuildRows(rc, bNeedInvalidate);
/*	
	CGridUI *pGrid = (CGridUI *)GetOwner();
	CDuiRect rcItem = m_rcItem;

	int iPosY = rc.top;
	if(pGrid->GetVerticalScrollBar() && pGrid->GetVerticalScrollBar()->IsVisible() && !pGrid->IsFitRows())
	{
		iPosY -= pGrid->GetVerticalScrollBar()->GetScrollPos();
	}

	//显示的第一行是哪个？
	int nBeginRow = pGrid->GetFixedRowCount();
	int nRows = pGrid->GetRowCount() - pGrid->GetFixedRowCount();
	for (int i=nBeginRow; i<nRows; i++)
	{
		if(iPosY + pGrid->GetRowHeight(i) > rc.top)
		{
			nBeginRow = i;
			break;
		}
		iPosY += pGrid->GetRowHeight(i);
	}

	//可以显示几行？
	int nNeedRows = 0;
	int nTempRow = nBeginRow;
	int iTempPosY = iPosY;
	while (iTempPosY < rcItem.bottom && nTempRow < pGrid->GetRowCount())
	{
		iTempPosY += pGrid->GetRowHeight(nTempRow);
		nNeedRows++;
		nTempRow++;
	}

	//创建实际的GridRowUI
	int addedRows = nNeedRows - GetCount();
	if(addedRows > 0)
	{
		while(GetCount() < nNeedRows) 
		{
			CGridRowUI *pRow = new CGridRowUI();
			pRow->SetOwner(GetOwner());
			Add(pRow);
			for (int i=0; i<pGrid->GetColumnCount(); i++)
			{
				CGridCellUI *pCell = new CGridCellUI;
				pRow->Add(pCell);
			}	
		}
	}
	else
	{
		while(GetCount() > 0 && GetCount() > nNeedRows) 
		{
			RemoveAt(GetCount()-1);
		}
	}
	
	//GridRowUI SetPos，初始化数据
	int nCurrentRow = nBeginRow;
	for (int i=0; i<GetCount(); i++)
	{
		CGridRowUI *pRowUI = (CGridRowUI *)GetItemAt(i);
		RECT rcRow = { rcItem.left, iPosY, rcItem.right, iPosY + pGrid->GetRowHeight(nCurrentRow) };
		//pRowUI->SetPos(rcRow, bNeedInvalidate);

		if(pRowUI->GetRow() != nCurrentRow)
		{
			pRowUI->SetRowIndex(nCurrentRow);
			for (int j=0; j<pRowUI->GetCount(); j++)
			{
				CGridCellUI *pCellUI = (CGridCellUI *)pRowUI->GetItemAt(j);
				pCellUI->SetCellIndex(nCurrentRow, j);

				GridCellType cellType = pGrid->GetCellType(pCellUI->GetRow(), pCellUI->GetCol());
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
						GetManager()->SendNotify(pGrid, DUI_MSGTYPE_INITCELL, nCurrentRow, j);
				}
			}
		}
		else
		{
			TRowData *pRowData = pGrid->GetRowData(nCurrentRow);
			int nIndentSize = 0;
			if(pRowData)
			{
				int nIndentCount = 0;
				TRowData *pTemp = pRowData;
				while (pTemp->m_pParent)
				{
					nIndentCount++;
					pTemp = pTemp->m_pParent;
				}
				nIndentSize = nIndentCount * pGrid->GetRowIndentWidth();
			}
			//pRowUI->SetPadding(CDuiRect(nIndentSize,0,0,0));
			rcRow.left += nIndentSize;
			pRowUI->SetPos(rcRow, bNeedInvalidate);
		}

		iPosY += pGrid->GetRowHeight(nCurrentRow);
		nCurrentRow++;
	}

	//虚表模式时，发送消息出去，让调用方填充CGridCellUI的数据
	if(pGrid->IsVirtualGrid())
	{
		GetManager()->SendNotify(pGrid, DUI_MSGTYPE_DRAWITEM, nBeginRow, nBeginRow+GetCount()-1);
	}
	*/
}

SIZE CGridBodyUI::EstimateSize(SIZE szAvailable)
{
	return __super::EstimateSize(szAvailable);
}

bool CGridBodyUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	return __super::DoPaint(hDC, rcPaint, pStopControl);
}

void CGridBodyUI::LineUp()
{
	if(!GetOwner()) return __super::LineUp();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->LineUp();
}

void CGridBodyUI::LineDown()
{
	if(!GetOwner()) return __super::LineDown();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->LineDown();
}

void CGridBodyUI::PageUp()
{
	if(!GetOwner()) return __super::PageUp();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->PageUp();
}

void CGridBodyUI::PageDown()
{
	if(!GetOwner()) return __super::PageDown();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->PageDown();
}

void CGridBodyUI::HomeUp()
{
	if(!GetOwner()) return __super::HomeUp();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->HomeUp();
}

void CGridBodyUI::EndDown()
{
	if(!GetOwner()) return __super::EndDown();
	CGridUI *pGrid = (CGridUI *)GetOwner();
	pGrid->EndDown();
}

}