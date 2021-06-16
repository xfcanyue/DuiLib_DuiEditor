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
	return DUI_CTR_GRIDROW;
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

bool CGridRowUI::IsHot()
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
		else CHorizontalLayoutUI::DoEvent(event);
		return;
	}

	CHorizontalLayoutUI::DoEvent(event);
}

void CGridRowUI::PaintBkColor(HDC hDC)
{
	if(IsSelected() || IsFocused())
	{
		if(m_dwSelectedBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwSelectedBkColor));
			return;
		}
	}
	else if( !IsEnabled() ) {
		if(m_dwDisabledBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwDisabledBkColor));
			return;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if(m_dwPushedBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwPushedBkColor));
			return;
		}
	}
	else if( IsHot() != 0 ) {
		if(m_dwHotBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwHotBkColor));
			return;
		}
	}

	if( m_dwBackColor != 0 ) {
		bool bVer = (m_sGradient.CompareNoCase(_T("hor")) != 0);
		if( m_dwBackColor2 != 0 ) {
			if( m_dwBackColor3 != 0 ) {
				RECT rc = m_rcItem;
				rc.bottom = (rc.bottom + rc.top) / 2;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 8);
				rc.top = rc.bottom;
				rc.bottom = m_rcItem.bottom;
				CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), bVer, 8);
			}
			else {
				CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), bVer, 16);
			}
		}
		else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
		else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
	}
}

}