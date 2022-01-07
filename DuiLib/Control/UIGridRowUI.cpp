#include "StdAfx.h"
#include "UIGridRowUI.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CGridRowUI)

CGridRowUI::CGridRowUI(void) : m_pOwner(NULL)
{
// 	SetInset(CDuiRect(1,1,1,1));
// 	SetBorderSize(1);
// 	SetBkColor(UIRGB(255,0,0));
	m_row = -1;
}

CGridRowUI::~CGridRowUI(void)
{
}

LPCTSTR CGridRowUI::GetClass() const
{
	return _T("GridRowUI");
}

UINT CGridRowUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP | UIFLAG_SETCURSOR;
}

LPVOID CGridRowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDROW) == 0 ) return static_cast<CGridRowUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridRowUI::SetRowIndex(int nIndex)
{
	m_row = nIndex;
}

int CGridRowUI::GetRow()
{
	return m_row;
}

bool CGridRowUI::IsSelected() const
{
	if(GetOwner() && GetOwner()->IsSelectedRow(m_row))
		return true;
	return false;
}

void CGridRowUI::Selected(bool bSelected, bool bTriggerEvent)
{

}

bool CGridRowUI::IsHotState() const
{
	if(!GetOwner() || !GetOwner()->IsListMode()) return false;
	CGridUI *pGrid = (CGridUI *)GetOwner();
	if(pGrid->GetHotRow() == this)
		return true;
	return false;
}

bool CGridRowUI::Add(CControlUI* pControl)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDCELL))
		return false;

	CGridCellUI *pCell = static_cast<CGridCellUI *>(pControl);
	pCell->SetOwner(GetOwner());

	return __super::Add(pControl); 
}

bool CGridRowUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDCELL))
		return false;

	CGridCellUI *pCell = static_cast<CGridCellUI *>(pControl);
	pCell->SetOwner(GetOwner());

	return __super::AddAt(pControl, iIndex);
}

void CGridRowUI::DoInit()
{
	
}

void CGridRowUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
}

SIZE CGridRowUI::EstimateSize(SIZE szAvailable)
{
	SIZE sz = {0};
	if(GetOwner())
	{
		CGridUI *pGrid = (CGridUI *)GetOwner();
		sz.cy = pGrid->GetRowHeight(m_row);
		return sz;
	}
	return __super::EstimateSize(szAvailable);
}

void CGridRowUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else __super::DoEvent(event);
		return;
	}

	__super::DoEvent(event);
}

void CGridRowUI::PaintBkColor(UIRender *pRender)
{
	DWORD dwBackColor = 0;

	if(dwBackColor == 0 && !IsEnabled() && GetDisabledBkColor() != 0) 
		dwBackColor = GetDisabledBkColor();

	if(dwBackColor == 0 && (IsSelected() || IsFocused()) && GetSelectBkColor() != 0)
		dwBackColor = GetSelectBkColor();

	if(dwBackColor == 0 && IsPushedState() && GetPushedBkColor() != 0)
		dwBackColor = GetPushedBkColor();

	if(dwBackColor == 0 && IsHotState() && GetHotBkColor() != 0)
		dwBackColor = m_dwHotBkColor;

	if(dwBackColor == 0)
		dwBackColor = GetBkColor();

	if(dwBackColor == 0) return;

	pRender->DrawBackColor(m_rcItem, CDuiSize(0,0), 
		GetAdjustColor(dwBackColor), 
		GetAdjustColor(GetBkColor2()), 
		GetAdjustColor(GetBkColor3()), 
		GetGradient());
}

}