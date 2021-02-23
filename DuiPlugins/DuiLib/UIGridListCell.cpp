#include "StdAfx.h"
#include "UIGridList.h"
#include "UIGridListCell.h"
#include "UIGridListRowUI.h"

namespace DuiLib
{

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CGridListCellUI)

CGridListCellUI::CGridListCellUI() : m_pOwner(NULL), m_bSelected(FALSE),
	m_uTextStyle(DT_CENTER | DT_VCENTER | DT_SINGLELINE), m_dwTextColor(0), 
	m_dwDisabledTextColor(0),
	m_iFont(-1),
	m_bShowHtml(false)
{
	SetInset(CDuiRect(1,1,1,1));
	m_iChildAlign = DT_CENTER;
	m_iChildVAlign = DT_VCENTER;

	::ZeroMemory(&m_rcCellPos, sizeof(m_rcCellPos));
	::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
	::ZeroMemory(&m_rcMerge, sizeof(m_rcMerge));
	m_bMergeWithOther = false;
	m_nSeparatorType = 0;
	m_celltype = celltypeText;
	m_pInnerControl = NULL;
}

CGridListCellUI::~CGridListCellUI(void)
{
}

LPCTSTR CGridListCellUI::GetClass() const
{
	return DUI_CTR_GRIDLISTCELL;
}

UINT CGridListCellUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP | UIFLAG_SETCURSOR;
}

LPVOID CGridListCellUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDLISTCELL) == 0 ) return static_cast<CGridListCellUI*>(this);
	return __super::GetInterface(pstrName);
}

int CGridListCellUI::GetRowIndex()
{
	CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
	CGridListRowUI *pRow = (CGridListRowUI *)GetParent();
	if(pRow->GetInterface(DUI_CTR_GRIDLISTROW))
		return pRow->GetRowIndex();
	return -1;
}

int CGridListCellUI::GetColIndex()
{
	CGridListRowUI *pRow = (CGridListRowUI *)GetParent();
	if(pRow) return pRow->GetItemIndex(this);
	return -1;
}

bool CGridListCellUI::IsFixed()
{
	CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
	CGridListRowUI *pRow = (CGridListRowUI *)GetParent();
	if(pRow->GetParent()->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		return true;
	}

	if(GetColIndex() < GetOwner()->GetFixedColumnCount())
	{
		return true;
	}

	return false;
}

bool CGridListCellUI::IsFixedRow()
{
	CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
	CGridListRowUI *pRow = (CGridListRowUI *)GetParent();
	if(pRow->GetParent()->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		return true;
	}
	return false;
}

bool CGridListCellUI::IsFixedColumn()
{
	if(GetColIndex() < GetOwner()->GetFixedColumnCount())
	{
		return true;
	}

	return false;
}

void CGridListCellUI::Selected(BOOL bSelect)
{
	if(m_bSelected = bSelect) return;
	m_bSelected = bSelect;
	Invalidate();
}

bool CGridListCellUI::IsMerged()
{
	if(m_rcMerge.left == m_rcMerge.right && m_rcMerge.top == m_rcMerge.bottom)
		return false;
	return true;
}

void CGridListCellUI::SetMergeRange(const RECT &rcRange)
{
	m_rcMerge = rcRange;
}

void CGridListCellUI::SetMergedWithOther(bool bMerge)
{
	m_bMergeWithOther = bMerge;
}

bool CGridListCellUI::IsMergedWidthOthers()
{
	return m_bMergeWithOther;
}


RECT CGridListCellUI::GetCellPos()
{
	if(!GetOwner()) return GetPos();

	if(IsMerged())
	{
		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		CDuiRect rcItem;
		for (int i=m_rcMerge.left; i<=m_rcMerge.right; i++)
		{
			for (int j=m_rcMerge.top; j<=m_rcMerge.bottom; j++)
			{
				CGridListCellUI *pCell = pGrid->GetCell(i,j);
				if(pCell)
				{
					if(rcItem.IsNull()) rcItem = pCell->GetPos();
					else rcItem.Join(pCell->GetPos());
				}
			}
		}
		return rcItem;
	}
	else if(IsMergedWidthOthers())
	{
		return CDuiRect(0,0,0,0);
	}
	return GetPos();
}

void CGridListCellUI::SetCellType(GridListCellType celltype)
{
	if(m_celltype == celltype) return;
	m_celltype = celltype;

	DeleteInnerControl();

	switch (celltype)
	{
	case celltypeText:
		break;
	case celltypeEdit:
		break;
	case celltypeCheckBox:
		CreateInnerControl();
		break;
	case celltypeCombo:
		break;
	case celltypeDateTime:
		break;
	case celltypeDate:
		break;
	case celltypeTime:
		break;
	case celltypeContainer:
		break;
	}

	Invalidate();
}

GridListCellType CGridListCellUI::GetCellType()
{
	GridListCellType cellType = m_celltype;
	if(GetOwner() && cellType == celltypeText)
	{
		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		if(pGrid)
		{
			cellType = pGrid->GetColumnCellType(GetColIndex());
		}
	}
	return cellType;
}

CDuiString CGridListCellUI::GetText() const
{
	return __super::GetText();
}

void CGridListCellUI::SetText(LPCTSTR pstrText)
{
	__super::SetText(pstrText);
}

int  CGridListCellUI::GetText_to_int()
{
	return __super::GetText_to_int();
}

void CGridListCellUI::SetText(int n)
{
	__super::SetText(n);
}

void CGridListCellUI::SetTextStyle(UINT uStyle)
{
	m_uTextStyle = uStyle;
	Invalidate();
}

UINT CGridListCellUI::GetTextStyle() const
{
	return m_uTextStyle;
}

RECT CGridListCellUI::GetTextPadding() const
{
	return m_rcTextPadding;
}

void CGridListCellUI::SetTextPadding(RECT rc)
{
	m_rcTextPadding = rc;
	Invalidate();
}

void CGridListCellUI::CreateInnerControl()
{
	if(m_pInnerControl) return;

	GridListCellType cellType = GetCellType();

	switch (cellType)
	{
	case celltypeText:
		{
			
		}
		break;
	case celltypeEdit:
		{
			CRichEditUI *pControl = new CRichEditUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridListCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleEdit());
			pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;

			if(GetOwner())
			{
				CGridListUI *pGrid = (CGridListUI *)GetOwner();
				if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_STARTEDIT, GetRowIndex(), GetColIndex());
			}
		}
		break;
	case celltypeCheckBox:
		{
			CCheckBoxUI *pControl = new CCheckBoxUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridListCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleCheckBox());
			//pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeCombo:
		{
			CComboExUI *pControl = new CComboExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			pControl->OnNotify += MakeDelegate(this, &CGridListCellUI::OnNotifyInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleCombo());
			pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeDateTime:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleDateTime());
			pControl->SetFormatStyle(UIDTS_DATETIME);
			pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeDate:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleDate());
			pControl->SetFormatStyle(UIDTS_DATE);
			pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeTime:
		{
			CDateTimeExUI *pControl = new CDateTimeExUI;
			pControl->OnEvent += MakeDelegate(this, &CGridListCellUI::OnEventInnerControl);
			if(GetOwner())
				pControl->ApplyAttributeList(GetOwner()->GetStyleTime());
			pControl->SetFormatStyle(UIDTS_TIME);
			pControl->SetText(GetText());
			Add(pControl);
			m_pInnerControl = pControl;
		}
		break;
	case celltypeContainer:
		break;
	}
	Invalidate();
}

void CGridListCellUI::DeleteInnerControl()
{
	if(m_pInnerControl)
	{
		Remove(m_pInnerControl);
		m_pInnerControl = NULL;
	}
}

CControlUI *CGridListCellUI::GetInnerControl() const
{
	return m_pInnerControl;
}

bool CGridListCellUI::OnEventInnerControl(void* param)
{
	TEventUI *pEvent = (TEventUI *)param;

	if( pEvent->Type == UIEVENT_KILLFOCUS ) 
	{
		OnCellKillFocus();
	}
	return true;
}

bool CGridListCellUI::OnNotifyInnerControl(void* param)
{
	TNotifyUI *pNotify = (TNotifyUI *)param;
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	if(!pGrid) return true;

	if(GetCellType() == celltypeEdit)
	{
		if(pNotify->sType == DUI_MSGTYPE_TEXTCHANGED)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_TEXTCHANGED, GetRowIndex(), GetColIndex());
		}
	}

	if(GetCellType() == celltypeCombo)
	{
		if(pNotify->sType == DUI_MSGTYPE_PREDROPDOWN)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_PREDROPDOWN, GetRowIndex(), GetColIndex());
		}
		else if(pNotify->sType == DUI_MSGTYPE_DROPDOWN)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_DROPDOWN, GetRowIndex(), GetColIndex());
		}
		else if(pNotify->sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ITEMSELECT, GetRowIndex(), GetColIndex());
			if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ENDEDIT, GetRowIndex(), GetColIndex());
		}
	}

	return true;
}

void CGridListCellUI::OnCellKillFocus()
{
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	if(!pGrid) return;

	if(!m_pInnerControl) return;

	GridListCellType celltype = GetCellType();
	if(celltype == celltypeEdit ||
		celltype == celltypeCombo || 
		celltype == celltypeDateTime || 
		celltype == celltypeDate|| 
		celltype == celltypeTime)
	{
		SetText(m_pInnerControl->GetText());
		DeleteInnerControl();
	}

	if(celltype == celltypeEdit || 
		celltype == celltypeDateTime || 
		celltype == celltypeDate|| 
		celltype == celltypeTime)
	{
		if(GetManager()) GetManager()->SendNotify(pGrid, DUI_MSGTYPE_ENDEDIT, GetRowIndex(), GetColIndex());
	}	
}

void CGridListCellUI::OnClickCheckBox()
{
	CCheckBoxUI *pCheckBox = (CCheckBoxUI *)m_pInnerControl;
	pCheckBox->Selected(!pCheckBox->IsSelected(), false);

	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	if(!pGrid) return;

	if(GetManager())
		GetManager()->SendNotify(pGrid, DUI_MSGTYPE_SELECTCHANGED, GetRowIndex(), GetColIndex());

	if(IsFixedRow())
	{
		bool bCheck = pCheckBox->IsSelected();
		int col = GetColIndex();
		//如果点击固定行的checkbox, 操作整列的checkbox
		for (int i=pGrid->GetFixedRowCount(); i<pGrid->GetRowCount(); i++)
		{
			CGridListCellUI *pCell = pGrid->GetCell(i, col);
			if(pCell->GetCellType() == celltypeCheckBox)
			{
				pCheckBox = (CCheckBoxUI *)pCell->m_pInnerControl;
				pCheckBox->Selected(bCheck, false);

				if(GetManager())
					GetManager()->SendNotify(pGrid, DUI_MSGTYPE_SELECTCHANGED, GetRowIndex(), GetColIndex());
			}
		}
	}
}

void CGridListCellUI::DoInit()
{
	//在xml中布局的这里处理一下
	if(!GetOwner())
	{
		CControlUI *pControl1 = GetParent();
		if(pControl1 && pControl1->GetInterface(DUI_CTR_GRIDLISTROW))
		{
			CControlUI *pControl2 = pControl1->GetParent();
			if(pControl2 && (pControl2->GetInterface(DUI_CTR_GRIDLISTHEADER) || pControl2->GetInterface(DUI_CTR_GRIDLISTBODY)) )
			{
				CControlUI *pControl3 = pControl2->GetParent();
				if(pControl3 && pControl3->GetInterface(DUI_CTR_GRIDLIST))
				{
					CGridListUI *pGrid = static_cast<CGridListUI *>(pControl3);
					SetOwner(pGrid);
				}
			}
		}
	}
	
	GridListCellType cellType = GetCellType();
	if(cellType == celltypeCheckBox)
	{
		CreateInnerControl();
	}
}

SIZE CGridListCellUI::EstimateSize(SIZE szAvailable)
{
	SIZE sz = {0,0};

	if(GetOwner())
		sz.cx = GetOwner()->GetColumnWidth(GetColIndex());

	return sz;
}

void CGridListCellUI::DoEvent(TEventUI& event)
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
		if(SizeColumnOrRow(event))
			return;

		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		if(IsFixedRow() && !IsFixedColumn() && GetRowIndex() == 0 )
		{
			pGrid->SortItems(GetColIndex());
			Invalidate();
		}
		else
		{
			pGrid->SelectCell(GetRowIndex(), GetColIndex());
		}
	}
	else if( event.Type == UIEVENT_RBUTTONDOWN )
	{
		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		if(pGrid)
		{
			pGrid->SelectCell(GetRowIndex(), GetColIndex(), false);
		}
	}
	else if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			m_nSeparatorType = 0;
			m_uButtonState &= ~UISTATE_CAPTURED;
			if( GetParent() ) 
				GetParent()->NeedParentUpdate();
			return;
		}
		else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
		{
			m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
		}
		if(GetCellType() == celltypeCheckBox)
		{
			OnClickCheckBox();
		}
	}
	else if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) 
		{
			if(m_nSeparatorType == 1 && GetOwner())
			{
				CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
				int width = pGrid->GetColumnWidth(GetColIndex());
				width -= ptLastMouse.x - event.ptMouse.x;
				if( width > 0 ) 
				{
					pGrid->SetColumnWidth(GetColIndex(), width);
					ptLastMouse = event.ptMouse;
					pGrid->NeedUpdate();
				}
			}
			else if(m_nSeparatorType == 2 && GetOwner())
			{
				CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
				int height = pGrid->GetRowHeight(GetRowIndex());
				height -= ptLastMouse.y - event.ptMouse.y;
				if( height > 0 ) 
				{
					pGrid->SetRowHeight(GetRowIndex(), height);
					ptLastMouse = event.ptMouse;
					pGrid->NeedUpdate();
				}
			}
			return;
		}
	}
	else if( event.Type == UIEVENT_SETCURSOR )
	{
		CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
		if(pGrid)
		{
			RECT rcSeparatorWidth = GetPos();
			rcSeparatorWidth.left = rcSeparatorWidth.right - 8;
			if( IsEnabled() && GetOwner()->IsEnableSizeColumn() && ::PtInRect(&rcSeparatorWidth, event.ptMouse) ) 
			{
				if(IsFixedRow())
				{
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
				}
				else
				{
					if(pGrid->IsEnableSizeColumnInBody())
						::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
					else
						::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				}
				return;
			}

			RECT rcSeparatorHeight = GetPos();
			rcSeparatorHeight.top = rcSeparatorHeight.bottom - 4;
			if( IsEnabled() && GetOwner()->IsEnableSizeRow() && ::PtInRect(&rcSeparatorHeight, event.ptMouse) ) 
			{
				if(IsFixedColumn())
				{
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
				}
				else
				{
					if(pGrid->IsEnableSizeRowInBody())
						::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					else
						::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				}
				return;
			}
		}	
	}
	else if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	else if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	__super::DoEvent(event);
}

bool CGridListCellUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	//we must draw merge cells in parent grid
	if(IsMerged() || IsMergedWidthOthers())
		return true;
	return __super::DoPaint(hDC, rcPaint, pStopControl);
}

void CGridListCellUI::PaintBkColor(HDC hDC)
{
	DWORD dwBackColor = m_dwBackColor;
	if(!GetOwner() || dwBackColor != 0) return __super::PaintBkColor(hDC);
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	CGridListRowUI *pRow = pGrid->GetRow(GetRowIndex());
	TGridCell cellFocus = pGrid->GetFocusCell();

// 	if(IsMerged() && GetColIndex() == 1)
// 	{
// 		InsertMsgUiV(_T("%d, %d, %d, %d,     %d, %d"), m_rcMerge.left, m_rcMerge.top, m_rcMerge.right, m_rcMerge.bottom,
// 			ptFocusCell.x, ptFocusCell.y);
// 	}

	if(IsSelected())
	{
		if(IsFixed()) dwBackColor = GetOwner()->GetFixedCellSelectedBkColor();
		else dwBackColor = GetOwner()->GetCellSelectedBkColor();
	}
	else if(IsFixedColumn() && pGrid->IsSelectedRow(GetRowIndex()))
	{
		dwBackColor = GetOwner()->GetFixedCellSelectedBkColor();
	}
	else if(IsFixedRow() && cellFocus.row != GetRowIndex() && cellFocus.col == GetColIndex())
	{
		dwBackColor = GetOwner()->GetFixedCellSelectedBkColor();
	}
	else if(IsFixedRow() && IsMerged() && cellFocus.col >= m_rcMerge.top && cellFocus.col <= m_rcMerge.bottom)
	{
		dwBackColor = GetOwner()->GetFixedCellSelectedBkColor();
	}
	else
	{
		if(IsFixed()) dwBackColor = GetOwner()->GetFixedCellBkColor();
		else dwBackColor = GetOwner()->GetCellBkColor();
	}	

	RECT rcItem = GetCellPos();
	if(dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, rcItem, GetAdjustColor(dwBackColor));
	else CRenderEngine::DrawColor(hDC, rcItem, GetAdjustColor(dwBackColor));
}

void CGridListCellUI::PaintBorder(HDC hDC)
{
	if(!GetOwner() || m_dwBorderColor != 0) return __super::PaintBorder(hDC);

	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	DWORD dwBorderColor = GetOwner()->GetLineColor();
	RECT rcItem = GetCellPos();

	if(IsSelected())
	{
		dwBorderColor = pGrid->GetCellSelectedBorderColor();
		RECT rcBorder = m_rcItem;
		rcBorder.left++;
		rcBorder.top++;
		rcBorder.right--;
		rcBorder.bottom--;
		CRenderEngine::DrawRect(hDC, rcItem, 1, GetAdjustColor(pGrid->GetCellSelectedBorderColor()), PS_SOLID);
	}
	else
	{
		if(GetOwner()->IsDrawRowLine())
		{
			RECT rcBorder = rcItem;
			rcBorder.bottom -= 1;
			rcBorder.top = rcBorder.bottom;
			CRenderEngine::DrawLine(hDC, rcBorder, 1, GetAdjustColor(dwBorderColor));
		}

		if(GetOwner()->IsDrawColumnLine())
		{
			RECT rcBorder	= rcItem;
			rcBorder.right -= 1;
			rcBorder.left	= rcBorder.right;
			CRenderEngine::DrawLine(hDC,rcBorder,1,GetAdjustColor(dwBorderColor));
		}
	}	
}

void CGridListCellUI::PaintText(HDC hDC)
{
	if(!GetOwner()) return __super::PaintText(hDC);

	if(m_pInnerControl)
	{
		return;
	}

	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	RECT rc = GetCellPos();
	GetManager()->GetDPIObj()->Scale(&m_rcTextPadding);
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;

	CDuiString sText;
	if(GetOwner()->IsViewListNumber() && IsFixedColumn() && !IsFixedRow())
	{
		if(GetRowIndex() >= 0)
			sText.Format(_T("%d"), GetRowIndex() - GetOwner()->GetFixedRowCount() + 1);
	}
	else
	{
		sText = GetText();
	}
	if( sText.IsEmpty() ) return;
	int nLinks = 0;
	if( IsEnabled() ) {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwTextColor, \
			NULL, NULL, nLinks, m_iFont, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwTextColor, \
			m_iFont, m_uTextStyle);
	}
	else {
		if( m_bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			NULL, NULL, nLinks, m_iFont, m_uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			m_iFont, m_uTextStyle);
	}
}

void CGridListCellUI::PaintForeImage(HDC hDC)
{
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	if(!pGrid) return;

	if(GetColIndex() == pGrid->GetSortColumn() && IsFixedRow() && GetRowIndex() == 0)
	{
		CDuiRect rcItem = GetCellPos();

		rcItem.top++;
		rcItem.bottom--;
		rcItem.right++;
		rcItem.left = rcItem.right - (rcItem.bottom - rcItem.top);

		SIZE szIcon = pGrid->GetSortIconSize();
		CDuiRect rcImage(0,0, szIcon.cx, szIcon.cy);
		rcImage.AlignRect(rcItem);

		if(pGrid->GetSortAscending())
			CRenderEngine::DrawImageString(hDC, GetManager(), rcImage, rcImage, pGrid->GetSortAscendingImage());
		else
			CRenderEngine::DrawImageString(hDC, GetManager(), rcImage, rcImage, pGrid->GetSortDescendingImage());
	}
}


bool CGridListCellUI::SizeColumnOrRow(TEventUI& event)
{
	RECT rcSeparatorWidth = GetPos();
	rcSeparatorWidth.left = rcSeparatorWidth.right - 8;
	RECT rcSeparatorHeight = GetPos();
	rcSeparatorHeight.top = rcSeparatorHeight.bottom - 4;
	if( ::PtInRect(&rcSeparatorWidth, event.ptMouse) && GetOwner()->IsEnableSizeColumn()) 
	{
		if(IsFixedRow())
		{
			m_nSeparatorType = 1;
			m_uButtonState |= UISTATE_CAPTURED;
			ptLastMouse = event.ptMouse;
			return true;
		}
		else
		{
			if(GetOwner()->IsEnableSizeColumnInBody())
			{
				m_nSeparatorType = 1;
				m_uButtonState |= UISTATE_CAPTURED;
				ptLastMouse = event.ptMouse;
				return true;
			}
		}
	}
	else if( IsEnabled() && GetOwner()->IsEnableSizeRow() && ::PtInRect(&rcSeparatorHeight, event.ptMouse) ) 
	{
		if(IsFixedColumn())
		{
			m_nSeparatorType = 2;
			m_uButtonState |= UISTATE_CAPTURED;
			ptLastMouse = event.ptMouse;
			return true;
		}
		else
		{
			if(GetOwner()->IsEnableSizeRowInBody())
			{
				m_nSeparatorType = 2;
				m_uButtonState |= UISTATE_CAPTURED;
				ptLastMouse = event.ptMouse;
				return true;
			}
		}
	}
	m_uButtonState &= ~UISTATE_CAPTURED;
	return false;
}

void CGridListCellUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("celltype")) == 0 )
	{
		if(_tcsicmp(pstrValue, _T("text")) == 0)
			SetCellType(celltypeText);
		else if(_tcsicmp(pstrValue, _T("edit")) == 0)
			SetCellType(celltypeEdit);
		else if(_tcsicmp(pstrValue, _T("check")) == 0)
			SetCellType(celltypeCheckBox);
		else if(_tcsicmp(pstrValue, _T("combo")) == 0)
			SetCellType(celltypeCombo);
		else if(_tcsicmp(pstrValue, _T("datetime")) == 0)
			SetCellType(celltypeDateTime);
		else if(_tcsicmp(pstrValue, _T("date")) == 0)
			SetCellType(celltypeDate);
		else if(_tcsicmp(pstrValue, _T("time")) == 0)
			SetCellType(celltypeTime);
		else if(_tcsicmp(pstrValue, _T("container")) == 0)
			SetCellType(celltypeContainer);
	}
	else if( _tcsicmp(pstrName, _T("align")) == 0 ) {
		if( _tcsstr(pstrValue, _T("left")) != NULL ) {
			m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
			m_uTextStyle |= DT_LEFT;
		}
		if( _tcsstr(pstrValue, _T("center")) != NULL ) {
			m_uTextStyle &= ~(DT_LEFT | DT_RIGHT );
			m_uTextStyle |= DT_CENTER;
		}
		if( _tcsstr(pstrValue, _T("right")) != NULL ) {
			m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
			m_uTextStyle |= DT_RIGHT;
		}
	}
	else if( _tcsicmp(pstrName, _T("valign")) == 0 ) {
		if( _tcsstr(pstrValue, _T("top")) != NULL ) {
			m_uTextStyle &= ~(DT_BOTTOM | DT_VCENTER | DT_WORDBREAK);
			m_uTextStyle |= (DT_TOP | DT_SINGLELINE);
		}
		if( _tcsstr(pstrValue, _T("vcenter")) != NULL ) {
			m_uTextStyle &= ~(DT_TOP | DT_BOTTOM | DT_WORDBREAK);            
			m_uTextStyle |= (DT_VCENTER | DT_SINGLELINE);
		}
		if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
			m_uTextStyle &= ~(DT_TOP | DT_VCENTER | DT_WORDBREAK);
			m_uTextStyle |= (DT_BOTTOM | DT_SINGLELINE);
		}
	}
	else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) {
		RECT rcTextPadding = { 0 };
		LPTSTR pstr = NULL;
		rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetTextPadding(rcTextPadding);
	}
	else if( _tcsicmp(pstrName, _T("merge")) == 0 )
	{
		RECT rcRange;
		LPTSTR pstr = NULL;
		rcRange.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
		rcRange.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		rcRange.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
		rcRange.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
		SetMergeRange(rcRange);
	}
	else if( _tcsicmp(pstrName, _T("mergedwithothers")) == 0 )
	{
		SetMergedWithOther(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else 
		__super::SetAttribute(pstrName, pstrValue);
}

}