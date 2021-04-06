#include "StdAfx.h"
#include "UIGridListRowUI.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CGridListRowUI)

CGridListRowUI::CGridListRowUI(void) : m_pOwner(NULL), m_bSelected(FALSE)
{
// 	SetInset(CDuiRect(1,1,1,1));
// 	SetBorderSize(1);
// 	SetBorderColor(UIRGB(255,0,0));
	m_nRowIndex = -1;
}

CGridListRowUI::~CGridListRowUI(void)
{
}

LPCTSTR CGridListRowUI::GetClass() const
{
	return DUI_CTR_GRIDLISTROW;
}

UINT CGridListRowUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP | UIFLAG_SETCURSOR;
}

LPVOID CGridListRowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDLISTROW) == 0 ) return static_cast<CGridListRowUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridListRowUI::SetRowIndex(int nIndex)
{
	m_nRowIndex = nIndex;
}

int CGridListRowUI::GetRowIndex()
{
	CGridListUI *pGrid = static_cast<CGridListUI *>(GetOwner());
	CControlUI *pControl = GetParent();
	if(pControl->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		return pGrid->GetHeader()->GetItemIndex(this);
	}
	else if(pGrid->IsVirtualGrid())
	{
		return m_nRowIndex;
	}
	else if(pControl->GetInterface(DUI_CTR_GRIDLISTBODY))
	{
		return pGrid->GetBody()->GetItemIndex(this) + pGrid->GetHeader()->GetCount();
	}

	return -1;
}

bool CGridListRowUI::IsFixedRow()
{
	if(GetParent()->GetInterface(DUI_CTR_GRIDLISTHEADER))
	{
		return true;
	}
	return false;
}

bool CGridListRowUI::Add(CControlUI* pControl)
{
	if( pControl == NULL) return false;
	if(!m_items.Add(pControl)) return false;

	if( m_pManager != NULL ) m_pManager->InitControls(pControl, this);
	if( IsVisible() ) NeedUpdate();
	else pControl->SetInternVisible(false);
	return true; 

// 	if(pControl->GetInterface(DUI_CTR_GRIDLISTCELL))
// 	{
// 		CGridListCellUI *pCell = static_cast<CGridListCellUI *>(pControl);
// 	}
// 	bool bAdd = __super::Add(pControl);
// 	if(bAdd)
// 	{
// 		Invalidate();
// 	}
// 	return bAdd;
}

bool CGridListRowUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(pControl->GetInterface(DUI_CTR_GRIDLISTROW))
	{
		CGridListCellUI *pCell = static_cast<CGridListCellUI *>(pControl);
		pCell->SetOwner(GetOwner());
	}
	bool bAdd = __super::AddAt(pControl, iIndex);
	if(bAdd)
	{
		Invalidate();
	}
	return bAdd;
}

void CGridListRowUI::Selected(BOOL bSelect)
{
	if(m_bSelected = bSelect) return;
	m_bSelected = bSelect;
	Invalidate();
}

void CGridListRowUI::DoInit()
{
	//在xml中布局的这里处理一下
	if(!GetOwner())
	{
		CControlUI *pControl1 = GetParent();
		if(pControl1 && (pControl1->GetInterface(DUI_CTR_GRIDLISTHEADER) || pControl1->GetInterface(DUI_CTR_GRIDLISTBODY) ))
		{
			CControlUI *pControl2 = pControl1->GetParent();
			if(pControl2 && pControl2->GetInterface(DUI_CTR_GRIDLIST))
			{
				CGridListUI *pGrid = static_cast<CGridListUI *>(pControl2);
				SetOwner(pGrid);
			}
		}
	}
}

void CGridListRowUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	return __super::SetPos(rc, bNeedInvalidate);
}

SIZE CGridListRowUI::EstimateSize(SIZE szAvailable)
{
	SIZE sz = GetManager()->GetDPIObj()->Scale(m_cxyFixed);

	if(sz.cy == 0 && GetOwner()) 
		sz.cy = GetManager()->GetDPIObj()->Scale(GetOwner()->GetDefRowHeight());

	return sz;
}

void CGridListRowUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

// 	if( event.Type == UIEVENT_BUTTONDOWN )
// 	{
// 		CGridListUI *pGrid = (CGridListUI *)GetOwner();
// 		if(pGrid && pGrid->IsListMode())
// 		{
// 			pGrid->SelectRow(GetRowIndex());
// 			pGrid->Invalidate();
// 		}
// 	}
// 	else if( event.Type == UIEVENT_RBUTTONDOWN )
// 	{
// 		CGridListUI *pGrid = (CGridListUI *)GetOwner();
// 		if(pGrid && pGrid->IsListMode())
// 		{
// 			pGrid->SelectRow(GetRowIndex());
// 			pGrid->Invalidate();
// 		}
// 	}

	__super::DoEvent(event);
}

void CGridListRowUI::PaintBkColor(HDC hDC)
{
	DWORD dwBackColor = m_dwBackColor;
	if(!GetOwner() || dwBackColor != 0) return __super::PaintBkColor(hDC);
	CGridListUI *pGrid = (CGridListUI *)GetOwner();

	if(IsSelected())
	{
		dwBackColor = pGrid->GetRowSelectedBkColor();
	}
	else
	{
		
	}

	if(dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(dwBackColor));
	else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(dwBackColor));
}

void CGridListRowUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if(_tcsicmp(pstrName, _T("height")) == 0 || _tcsicmp(pstrName, _T("pos")) == 0)
	{
		__super::SetAttribute(pstrName, pstrValue);
		if(GetOwner()) GetOwner()->Refresh();
	}
	else
		return __super::SetAttribute(pstrName, pstrValue);
}

}