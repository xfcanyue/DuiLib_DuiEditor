#include "StdAfx.h"
#include "UIGridListBody.h"
#include "UIGridList.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CGridListBodyUI)

CGridListBodyUI::CGridListBodyUI(void) : m_pOwner(NULL)
{
	EnableScrollBar(true,false);
	m_pVerticalScrollBar->SetFixedWidth(0);

}


CGridListBodyUI::~CGridListBodyUI(void)
{
}

LPCTSTR CGridListBodyUI::GetClass() const
{
	return DUI_CTR_GRIDLISTBODY;
}

UINT CGridListBodyUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridListBodyUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDLISTBODY) == 0 ) return static_cast<CGridListBodyUI*>(this);
	return __super::GetInterface(pstrName);
}

bool CGridListBodyUI::Add(CControlUI* pControl)
{
	if(pControl->GetInterface(DUI_CTR_GRIDLISTROW))
	{
		CGridListRowUI *pRow = static_cast<CGridListRowUI *>(pControl);
	}

	bool bAdd = __super::Add(pControl);
	if(bAdd)
	{
		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		if(pGrid)
		{
			if(!pGrid->IsVirtualGrid()) pGrid->m_nRowCount++;
			pGrid->Refresh();
		}
	}
	return bAdd;
}

bool CGridListBodyUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(pControl->GetInterface(DUI_CTR_GRIDLISTROW))
	{
		CGridListRowUI *pRow = static_cast<CGridListRowUI *>(pControl);
	}

	bool bAdd = __super::AddAt(pControl, iIndex);
	if(bAdd)
	{
		CGridListUI *pGrid = (CGridListUI *)GetOwner();
		if(pGrid)
		{
			if(!pGrid->IsVirtualGrid()) pGrid->m_nRowCount++;
			pGrid->Refresh();
		}
	}
	return bAdd;
}

void CGridListBodyUI::DoInit()
{
	//在xml中布局的这里处理一下
	if(!GetOwner())
	{
		CControlUI *pControl1 = GetParent();
		if(pControl1 && pControl1->GetInterface(DUI_CTR_GRIDLIST))
		{
			CGridListUI *pGrid = static_cast<CGridListUI *>(pControl1);
			SetOwner(pGrid);
		}
	}
}

void CGridListBodyUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	return __super::SetPos(rc, bNeedInvalidate);
}

SIZE CGridListBodyUI::EstimateSize(SIZE szAvailable)
{
	//if(GetOwner() && GetOwner()->IsVirtualGrid())
	//	return __super::EstimateSize(szAvailable);

	SIZE sz = {0,0};

	for (int i=0; i<GetCount(); i++)
	{
		sz.cy += GetOwner()->GetRowHeight(i);
	}

	if(GetOwner())
	{
		for (int i=0; i<GetOwner()->GetColumnCount(); i++)
		{
			sz.cx += GetOwner()->GetColumnWidth(i);
		}
	}

	//sz.cx = max(sz.cx, szAvailable.cx);
	if(sz.cy == 0) sz.cy = max(sz.cy, szAvailable.cy);

	return sz;
}

void CGridListBodyUI::LineUp()
{
	if(!GetOwner()) return __super::LineUp();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->LineUp();
}

void CGridListBodyUI::LineDown()
{
	if(!GetOwner()) return __super::LineDown();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->LineDown();
}

void CGridListBodyUI::PageUp()
{
	if(!GetOwner()) return __super::PageUp();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->PageUp();
}

void CGridListBodyUI::PageDown()
{
	if(!GetOwner()) return __super::PageDown();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->PageDown();
}

void CGridListBodyUI::HomeUp()
{
	if(!GetOwner()) return __super::HomeUp();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->HomeUp();
}

void CGridListBodyUI::EndDown()
{
	if(!GetOwner()) return __super::EndDown();
	CGridListUI *pGrid = (CGridListUI *)GetOwner();
	pGrid->EndDown();
}

}