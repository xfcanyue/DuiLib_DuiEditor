#include "StdAfx.h"
#include "UIGrid.h"
#include "UIGridCell.h"
#include "UIGridRowUI.h"

namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
CGridCellInnerCheckBoxUI::CGridCellInnerCheckBoxUI()
{
	m_pOwner = NULL;
}

bool CGridCellInnerCheckBoxUI::Activate()
{
	if( !CControlUI::Activate() ) return false;
	if( !m_sGroupName.IsEmpty() ) Selected(true);
	else Selected(!IsSelected());
	return true;
}

bool CGridCellInnerCheckBoxUI::IsSelected() const
{
	if(!m_pOwner) return false;
	CGridCellUI *pCellUI = (CGridCellUI *)m_pOwner;
	CGridUI *pGrid = (CGridUI *)pCellUI->GetOwner();
	if(!pGrid) return false;
	TCellData *pCellData = pGrid->GetCellData(pCellUI->GetRow(), pCellUI->GetCol());
	if(!pCellData) return false;
	return pCellData->IsCheckBoxCheck() == TRUE;
}

void CGridCellInnerCheckBoxUI::Selected(bool bSelected, bool bTriggerEvent)
{
	if(IsSelected() == bSelected) return;

	if( bSelected ) m_uButtonState |= UISTATE_SELECTED;
	else m_uButtonState &= ~UISTATE_SELECTED;

	if(!m_pOwner) return;
	CGridCellUI *pCellUI = (CGridCellUI *)m_pOwner;
	CGridUI *pGrid = (CGridUI *)pCellUI->GetOwner();
	if(!pGrid) return;
	TCellData *pCellData = pGrid->GetCellData(pCellUI->GetRow(), pCellUI->GetCol());
	if(!pCellData) return;

	pCellData->SetCheckBoxCheck(bSelected);
	if(GetManager())
		GetManager()->SendNotify(pGrid, DUI_MSGTYPE_SELECTCHANGED, pCellUI->GetRow(), pCellUI->GetCol());

	BOOL bCheck = pCellData->IsCheckBoxCheck();
	if(pCellUI->IsFixedRow())
	{
		//如果点击固定行的checkbox, 操作整列的checkbox
		for (int i=0; i<pGrid->GetRowCount(); i++)
		{
			TCellData *pCellData = pGrid->GetCellData(i, pCellUI->GetCol());
			if(pCellData && pGrid->GetCellType(pCellUI->GetRow(), pCellUI->GetCol()) == celltypeCheckBox)
			{
				pCellData->SetCheckBoxCheck(bCheck);

				if(GetManager())
					GetManager()->SendNotify(pGrid, DUI_MSGTYPE_SELECTCHANGED, i, pCellUI->GetCol());
			}
		}
	}

	pGrid->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CGridCellUI)

CGridCellUI::CGridCellUI() : m_pOwner(NULL)
{
	SetInset(CDuiRect(1,1,1,1));
	m_iChildAlign = DT_CENTER;
	m_iChildVAlign = DT_VCENTER;

	m_row = -1;
	m_col = -1;
	m_pInnerControl = NULL;
	m_nSeparatorType = 0;

	::ZeroMemory(&m_rcMerge, sizeof(m_rcMerge));
	m_bMergeWithOther = false;
	m_celltype = celltypeText;
}

CGridCellUI::~CGridCellUI(void)
{
}

LPCTSTR CGridCellUI::GetClass() const
{
	return DUI_CTR_GRIDCELL;
}

UINT CGridCellUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP | UIFLAG_SETCURSOR;
}

LPVOID CGridCellUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDCELL) == 0 ) return static_cast<CGridCellUI*>(this);
	return __super::GetInterface(pstrName);
}

CDuiString CGridCellUI::GetText() const
{
	if(!GetOwner()) return __super::GetText();

	TCellData *pCellData = GetOwner()->GetCellData(m_row, m_col);
	if(pCellData)
	{
		return pCellData->GetText();
	}
	return __super::GetText();
}

void CGridCellUI::SetText(LPCTSTR pstrText)
{
	if(!GetOwner()) return __super::SetText(pstrText);

	CGridUI *pGrid = (CGridUI *)GetOwner();
	TCellData *pCellData = pGrid->GetCellData(m_row, m_col);
	if(pCellData)
	{
		pCellData->SetText(pstrText);
		return;
	}

	__super::SetText(pstrText);
}

bool CGridCellUI::IsMergedWithOthers()
{
	if(!GetOwner()) return false;
	TCellData *pCellData = GetOwner()->GetCellData(m_row, m_col);
	if(pCellData)
	{
		return pCellData->IsMergedWithOthers();
	}
	return m_bMergeWithOther;
}

void CGridCellUI::SetCellIndex(int row, int col) { m_row = row; m_col = col; }

void CGridCellUI::InitCell()
{
	if(!GetOwner()) return;
	CGridUI *pGrid = (CGridUI *)GetOwner();

	TCellData *pCellData = pGrid->GetCellData(m_row, m_col);
	if(!pCellData) return;

	pCellData->SetText(m_sText);
	if(m_rcMerge.left != m_rcMerge.right || m_rcMerge.top != m_rcMerge.bottom)
	{
		pGrid->MergeCells(m_rcMerge.left, m_rcMerge.top, m_rcMerge.right, m_rcMerge.bottom);
	}
	pCellData->SetMergedWidthOthers(m_bMergeWithOther);

	if(m_celltype != celltypeText)
		pGrid->SetCellType(m_row, m_col, m_celltype);
}

BOOL CGridCellUI::IsFixedRow() const 
{ 
	if(!GetOwner()) return false;
	CGridUI *pGrid = (CGridUI *)GetOwner();
	return pGrid->IsFixedRow(m_row);
}

BOOL CGridCellUI::IsFixedCol() const
{
	if(!GetOwner()) return false;
	CGridUI *pGrid = (CGridUI *)GetOwner();
	return pGrid->IsFixedCol(m_col);
}

void CGridCellUI::SetFixedWidth(int cx)
{
	//使用GetExtraParent()一直找到CGridUI，如果是固定列，设置列宽。 
	//cell ==> row ==> header ==> grid
	if(GetExtraParent() && GetExtraParent()->GetInterface(DUI_CTR_GRIDROW)) 
	{
		CGridRowUI *pRow = (CGridRowUI *)GetExtraParent();
		if(pRow->GetExtraParent() && pRow->GetExtraParent()->GetInterface(DUI_CTR_GRIDHEADER))
		{
			CGridHeaderUI *pHeader = (CGridHeaderUI *)pRow->GetExtraParent();
			if(pHeader->GetExtraParent() && pHeader->GetExtraParent()->GetInterface(DUI_CTR_GRID)) 
			{
				//终于找到你啦
				CGridUI *pGrid = (CGridUI *)pHeader->GetExtraParent();
				int col = pRow->GetItemIndex(this);
				pGrid->SetColumnWidth(col, cx);
				return;	
			}
		}	
	}
	__super::SetFixedWidth(cx);
}

bool CGridCellUI::IsSelected() const
{
	if(!GetOwner())
		return false;
	if(GetOwner()->IsSelectedCell(m_row, m_col))
		return true;
	return false;
}

void CGridCellUI::Selected(bool bSelected, bool bTriggerEvent)
{

}

bool CGridCellUI::IsFocused() const
{
	if(!GetOwner())
		return false;
	if(GetOwner()->GetFocusCell().row == m_row && GetOwner()->GetFocusCell().col == m_col)
		return true;
	return false;
}

bool CGridCellUI::IsHot() const
{
	if(!GetOwner() || GetOwner()->IsListMode()) return false;
	CGridUI *pGrid = (CGridUI *)GetOwner();
	if(pGrid->GetHotCell() == this)
		return true;
	return false;
}

RECT CGridCellUI::GetCellPos()
{
	if(!GetOwner()) return GetPos();
	CGridUI *pGrid = (CGridUI *)GetOwner();

	if(pGrid->IsMergedCell(m_row, m_col))
	{
		TGridMergeRange rcMerge = pGrid->GetCellMergeRange(m_row, m_col);
		CDuiRect rcItem;
		for (int i=rcMerge.begin_row; i<=rcMerge.end_row; i++)
		{
			for (int j=rcMerge.begin_col; j<=rcMerge.end_col; j++)
			{
				CGridCellUI *pCell = pGrid->GetCellUI(i,j);
				if(pCell)
				{
					if(rcItem.IsNull()) rcItem = pCell->GetPos();
					else rcItem.Join(pCell->GetPos());
				}
			}
		}
		return rcItem;
	}
	else if(IsMergedWithOthers())
	{
		return CDuiRect(0,0,0,0);
	}
	return __super::GetPos();
}

void CGridCellUI::CreateInnerControl()
{
	if(!GetOwner()) return;
	CGridUI *pGrid = (CGridUI *)GetOwner();

	GridCellType cellType = pGrid->GetCellType(m_row, m_col);

	//如果类型和已存在的不同，删除重新创建
	if(m_pInnerControl != NULL) 
	{
		if( (cellType == celltypeText)																			||
			(cellType == celltypeEdit		&& m_pInnerControl->GetInterface(DUI_CTR_RICHEDIT)		== NULL)	||
			(cellType == celltypeCheckBox	&& m_pInnerControl->GetInterface(DUI_CTR_CHECKBOX)		== NULL)	||
			(cellType == celltypeCombo		&& m_pInnerControl->GetInterface(DUI_CTR_COMBOEX)		== NULL)	||
			(cellType == celltypeDateTime	&& m_pInnerControl->GetInterface(DUI_CTR_DATETIMEEX)	== NULL)	||
			(cellType == celltypeDate		&& m_pInnerControl->GetInterface(DUI_CTR_DATETIMEEX)	== NULL)	||
			(cellType == celltypeTime		&& m_pInnerControl->GetInterface(DUI_CTR_DATETIMEEX)	== NULL)	||
			(cellType == celltypePicture	&& m_pInnerControl->GetInterface(DUI_CTR_PICTURE)		== NULL)	)
		{
			DeleteInnerControl();
		}
	}

	if(m_pInnerControl != NULL) return;

	TCellData *pCellData = GetOwner()->GetCellData(m_row, m_col);
	if(!pCellData) return;

	switch (cellType)
	{
	case celltypeText:
		{

		}
		break;
	case celltypeEdit:
		{
			CRichEditUI *pControl = new CRichEditUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleEdit());
			pControl->SetText(pCellData->GetText());
			Add(pControl);
			m_pInnerControl = pControl;

			if(GetOwner())
			{
				CGridUI *pGrid = (CGridUI *)GetOwner();
				if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_STARTEDIT, m_row, m_col);
			}
		}
		break;
	case celltypeCheckBox:
		{
			CGridCellInnerCheckBoxUI *pControl = new CGridCellInnerCheckBoxUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleCheckBox());
			pControl->SetOwner(this);
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeCombo:
		{
			CComboExUI *pControl = new CComboExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleCombo());
			pControl->SetText(pCellData->GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeDateTime:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleDateTime());
			pControl->SetFormatStyle(UIDTS_DATETIME);
			pControl->SetText(pCellData->GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeDate:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleDate());
			pControl->SetFormatStyle(UIDTS_DATE);
			pControl->SetText(pCellData->GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeTime:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleTime());
			pControl->SetFormatStyle(UIDTS_TIME);
			pControl->SetText(pCellData->GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypePicture:
		{
			CPictureUI *pControl = new CPictureUI;
			pControl->OnEvent += MakeDelegate(this, &CGridCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStylePicture());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeContainer:
		break;
	}
	Invalidate();
}

void CGridCellUI::DeleteInnerControl()
{
	if(m_pInnerControl)
	{
		Remove(m_pInnerControl);
		m_pInnerControl = NULL;
	}
}

CControlUI *CGridCellUI::GetInnerControl() const { return m_pInnerControl; }

bool CGridCellUI::OnEventInnerControl(void* param)
{
	TEventUI *pEvent = (TEventUI *)param;

	if( pEvent->Type == UIEVENT_KILLFOCUS ) 
	{
		OnCellKillFocus();
	}
	return true;
}

bool CGridCellUI::OnNotifyInnerControl(void* param)
{
	TNotifyUI *pNotify = (TNotifyUI *)param;
	CGridUI *pGrid = (CGridUI *)GetOwner();
	if(!pGrid) return true;

	GridCellType cellType = pGrid->GetCellType(m_row, m_col);
	
	if(cellType == celltypeCombo)
	{
		if(pNotify->sType == DUI_MSGTYPE_PREDROPDOWN)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_PREDROPDOWN, m_row, m_col);
		}
		else if(pNotify->sType == DUI_MSGTYPE_DROPDOWN)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_DROPDOWN, m_row, m_col);
		}
		else if(pNotify->sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ITEMSELECT, m_row, m_col);
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ENDEDIT, m_row, m_col);
		}
	}

	return true;
}

void CGridCellUI::OnCellSetFocus()
{
	CreateInnerControl();
}

void CGridCellUI::OnCellKillFocus()
{
	CGridUI *pGrid = (CGridUI *)GetOwner();
	if(!pGrid) return;

	if(!m_pInnerControl) return;

	GridCellType celltype = pGrid->GetCellType(m_row, m_col);
	if(celltype == celltypeEdit ||
		celltype == celltypeCombo || 
		celltype == celltypeDateTime || 
		celltype == celltypeDate|| 
		celltype == celltypeTime)
	{
		pGrid->Cell(m_row,m_col).SetText(m_pInnerControl->GetText());
		DeleteInnerControl();
	}

	if(celltype == celltypeEdit || 
		celltype == celltypeDateTime || 
		celltype == celltypeDate|| 
		celltype == celltypeTime)
	{
		if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ENDEDIT, m_row, m_col);
	}	
}

void CGridCellUI::DoInit()
{

}

void CGridCellUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	return __super::SetPos(rc, bNeedInvalidate);
}

SIZE CGridCellUI::EstimateSize(SIZE szAvailable)
{
	if(!GetOwner())
		return __super::EstimateSize(szAvailable);

	SIZE sz = {0,0};
	sz.cx = GetOwner()->GetColumnWidth(m_col);
	if(sz.cx > szAvailable.cx) sz.cx = szAvailable.cx;
	return sz;
}

void CGridCellUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

	if(!GetOwner()) return __super::DoEvent(event);
	CGridUI *pGrid = (CGridUI *)GetOwner();
	return pGrid->DoEvent(event);

	__super::DoEvent(event);
}

bool CGridCellUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if(!GetOwner()) 
		return __super::DoPaint(hDC, rcPaint, pStopControl);
	CGridUI *pGrid = (CGridUI *)GetOwner();
	TCellData *pCellData = pGrid->GetCellData(m_row, m_col);
	m_rcPaint2  = GetCellPos();
	if(pCellData)
	{
		//we must draw merge cells in parent grid
		if(pGrid->IsMergedCell(m_row, m_col) || pCellData->IsMergedWithOthers())
			return true;
	}
	return __super::DoPaint(hDC, rcPaint, pStopControl);
}

void CGridCellUI::PaintBkColor(HDC hDC)
{
	if(IsSelected() || IsFocused())
	{
		if(m_dwSelectedBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwSelectedBkColor));
			return;
		}
	}
	else if( !IsEnabled() ) {
		if(m_dwDisabledBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwDisabledBkColor));
			return;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if(m_dwPushedBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwPushedBkColor));
			return;
		}
	}
	else if( IsHot() != 0 ) {
		if(m_dwHotBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwHotBkColor));
			return;
		}
	}

	if( m_dwBackColor != 0 ) {
		bool bVer = (m_sGradient.CompareNoCase(_T("hor")) != 0);
		if( m_dwBackColor2 != 0 ) {
			if( m_dwBackColor3 != 0 ) {
				RECT rc = m_rcPaint2;
				rc.bottom = (rc.bottom + rc.top) / 2;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 8);
				rc.top = rc.bottom;
				rc.bottom = m_rcPaint2.bottom;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), bVer, 8);
			}
			else {
				CRenderEngine::DrawGradient(hDC, m_rcPaint2, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 16);
			}
		}
		else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwBackColor));
		else CRenderEngine::DrawColor(hDC, m_rcPaint2, GetAdjustColor(m_dwBackColor));
	}
}

void CGridCellUI::PaintBkImage(HDC hDC)
{
	if( m_sBkImage.IsEmpty() ) return;
	CRenderEngine::DrawImageString(hDC, m_pManager, m_rcPaint2, m_rcPaint2, m_sBkImage.GetData(), NULL, m_instance);
}

void CGridCellUI::PaintStatusImage(HDC hDC)
{
	
}

void CGridCellUI::PaintForeColor(HDC hDC)
{

}

void CGridCellUI::PaintForeImage(HDC hDC)
{
	CGridUI *pGrid = (CGridUI *)GetOwner();
	if(!pGrid) return;

	if(GetCol() == pGrid->GetSortColumn() && IsFixedRow() && GetRow() == 0)
	{
		CDuiRect rcItem = GetCellPos();

		SIZE szIcon = pGrid->GetSortIconSize();

		rcItem.top++;
		rcItem.bottom--;
		rcItem.right -= 5;
		rcItem.left = rcItem.right - szIcon.cx;

		CDuiRect rcImage(0,0, szIcon.cx, szIcon.cy);
		rcImage.AlignRect(rcItem);

		if(pGrid->GetSortAscending())
			CRenderEngine::DrawImageString(hDC, GetManager(), rcImage, rcImage, pGrid->GetSortAscendingImage());
		else
			CRenderEngine::DrawImageString(hDC, GetManager(), rcImage, rcImage, pGrid->GetSortDescendingImage());
	}
}

void CGridCellUI::PaintText(HDC hDC)
{
	if(!GetOwner()) return __super::PaintText(hDC);
	CGridUI *pGrid = (CGridUI *)GetOwner();

	GridCellType cellType = pGrid->GetCellType(m_row, m_col);
	if(m_pInnerControl || cellType == celltypeContainer || cellType == celltypePicture) 
		return;

	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	CDuiString sText = GetText();

	RECT rcTextPadding = GetTextPadding();
	GetManager()->GetDPIObj()->Scale(&rcTextPadding);
	int nLinks = 0;
	RECT rc = m_rcPaint2;
	rc.left += rcTextPadding.left;
	rc.right -= rcTextPadding.right;
	rc.top += rcTextPadding.top;
	rc.bottom -= rcTextPadding.bottom;

	DWORD clrColor = IsEnabled()?m_dwTextColor:m_dwDisabledTextColor;
	if((IsSelected() || IsFocused()) && (GetSelectedTextColor() != 0))
	{
		clrColor = GetSelectedTextColor();
	}
	else if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedTextColor() != 0) )
	{
		clrColor = GetPushedTextColor();
	}
	else if( IsHot() && (GetHotTextColor() != 0) )
	{
		clrColor = GetHotTextColor();
	}
	else if( IsFocused() && (GetFocusedTextColor() != 0) )
	{
		clrColor = GetFocusedTextColor();
	}

	int iFont = GetFont();
	if((IsSelected() || IsFocused()) && (GetSelectedFont() != 0) )
	{
		iFont = GetSelectedFont();
	}
	else if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (GetPushedFont() != -1) )
	{
		iFont = GetPushedFont();
	}
	else if( IsHot() && (GetHotFont() != -1) )
	{
		iFont = GetHotFont();
	}
	else if( IsFocused() && (GetFocusedFont() != -1) )
	{
		iFont = GetFocusedFont();
	}

	if(IsFixedCol() && !IsFixedRow())
	{
		if(pGrid->GetFixedColumnCount() > 0 && pGrid->IsViewListNumber())
		{
			sText.Format(_T("%d"), m_row - pGrid->GetFixedRowCount() + 1);
			RECT rcText = {0};
			CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, m_dwTextColor, iFont, DT_CALCRECT | m_uTextStyle);
			rcText.right += rcTextPadding.left + rcTextPadding.right;
			rcText.bottom += rcTextPadding.top + rcTextPadding.bottom;
			if(pGrid->GetColumnWidth(m_col) < rcText.right - rcText.left + 10)
			{
				pGrid->SetColumnWidth(m_col, rcText.right - rcText.left + 10);
				pGrid->NeedUpdate();
			}
		}
	}
	else
	{
		if(pGrid->IsExpandColumnByText())
		{
			RECT rcText = {0};
			CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, m_dwTextColor, iFont, DT_CALCRECT | m_uTextStyle);
			rcText.right += rcTextPadding.left + rcTextPadding.right;
			rcText.bottom += rcTextPadding.top + rcTextPadding.bottom;
			if(pGrid->GetColumnWidth(m_col) < rcText.right - rcText.left + 10)
			{
				pGrid->SetColumnWidth(m_col, rcText.right - rcText.left + 10);
				pGrid->NeedUpdate();
			}
		}
	}
	if( sText.IsEmpty() ) return;


	if( m_bShowHtml )
		CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, clrColor, \
		NULL, NULL, nLinks, iFont, m_uTextStyle);
	else
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, clrColor, \
		iFont, m_uTextStyle);
}

void CGridCellUI::PaintBorder(HDC hDC)
{
	if(!GetOwner()) return __super::PaintBorder(hDC);
	CGridUI *pGrid = (CGridUI *)GetOwner();

	DWORD dwColor = 0;
	if(IsSelected() || IsFocused())
		dwColor = GetSelectedBorderColor();

	//已选中单元格，或者 焦点单元格
	if(dwColor != 0)
	{
		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if(nBorderSize > 0)
		{
			if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 ) 
			{
				CRenderEngine::DrawRoundRect(hDC, m_rcPaint2, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(dwColor), m_nBorderStyle);
			}
			else
			{
				CRenderEngine::DrawRect(hDC, m_rcPaint2, nBorderSize, GetAdjustColor(dwColor), m_nBorderStyle);
			}
		}
		else
		{
			RECT rcBorder;

			if(rcBorderSize.left > 0){
				rcBorder		= m_rcPaint2;
				rcBorder.right	= rcBorder.left;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.left,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.top > 0){
				rcBorder		= m_rcPaint2;
				rcBorder.bottom	= rcBorder.top;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.top,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.right > 0){
				rcBorder		= m_rcPaint2;
				rcBorder.right -= 1;
				rcBorder.left	= rcBorder.right;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.right,GetAdjustColor(dwColor),m_nBorderStyle);
			}
			if(rcBorderSize.bottom > 0){
				rcBorder		= m_rcPaint2;
				rcBorder.bottom -= 1;
				rcBorder.top	= rcBorder.bottom;
				CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.bottom,GetAdjustColor(dwColor),m_nBorderStyle);
			}
		}
	}
	else
	{
		DWORD dwBorderColor = GetOwner()->GetLineColor();
		if(pGrid->IsDrawRowLine())
		{
			RECT rcBorder = m_rcPaint2;
			rcBorder.bottom -= 1;
			rcBorder.top = rcBorder.bottom;
			CRenderEngine::DrawLine(hDC, rcBorder, 1, GetAdjustColor(dwBorderColor));
		}

		if(pGrid->IsDrawColumnLine())
		{
			RECT rcBorder	= m_rcPaint2;
			rcBorder.right -= 1;
			rcBorder.left	= rcBorder.right;
			CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(dwBorderColor));
		}
	}
}

void CGridCellUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("celltype")) == 0 )
	{
		if(_tcsicmp(pstrValue, _T("text")) == 0)
			m_celltype = celltypeText;
		else if(_tcsicmp(pstrValue, _T("edit")) == 0)
			m_celltype = celltypeEdit;
		else if(_tcsicmp(pstrValue, _T("check")) == 0)
			m_celltype = celltypeCheckBox;
		else if(_tcsicmp(pstrValue, _T("combo")) == 0)
			m_celltype = celltypeCombo;
		else if(_tcsicmp(pstrValue, _T("datetime")) == 0)
			m_celltype = celltypeDateTime;
		else if(_tcsicmp(pstrValue, _T("date")) == 0)
			m_celltype = celltypeDate;
		else if(_tcsicmp(pstrValue, _T("time")) == 0)
			m_celltype = celltypeTime;
		else if(_tcsicmp(pstrValue, _T("picture")) == 0)
			m_celltype = celltypeContainer;
		else if(_tcsicmp(pstrValue, _T("container")) == 0)
			m_celltype = celltypeContainer;
	}
	else if( _tcsicmp(pstrName, _T("merge")) == 0 )
	{
		RECT rcRange;
		LPTSTR pstr = NULL;
		rcRange.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		rcRange.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcRange.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcRange.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		m_rcMerge = rcRange;
	}
	else if( _tcsicmp(pstrName, _T("mergedwithothers")) == 0 )
	{
		m_bMergeWithOther = _tcsicmp(pstrValue, _T("true")) == 0;
	}
	else 
		__super::SetAttribute(pstrName, pstrValue);
}

}