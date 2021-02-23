#include "StdAfx.h"
#include "UIGridListHeader.h"

namespace DuiLib
{


IMPLEMENT_DUICONTROL(CGridListHeaderUI)


CGridListHeaderUI::CGridListHeaderUI(void) : m_pOwner(NULL)
{
}


CGridListHeaderUI::~CGridListHeaderUI(void)
{
}

LPCTSTR CGridListHeaderUI::GetClass() const
{
	return DUI_CTR_GRIDLISTHEADER;
}

UINT CGridListHeaderUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridListHeaderUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDLISTHEADER) == 0 ) return static_cast<CGridListHeaderUI*>(this);
	return __super::GetInterface(pstrName);
}

bool CGridListHeaderUI::Add(CControlUI* pControl)
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
			pGrid->m_nRowCount++;
			pGrid->Refresh();
		}
	}
	return bAdd;
}

bool CGridListHeaderUI::AddAt(CControlUI* pControl, int iIndex)
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
			pGrid->m_nRowCount++;
			pGrid->Refresh();
		}
	}
	return bAdd;
}

void CGridListHeaderUI::DoInit()
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

void CGridListHeaderUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	__super::SetPos(rc, bNeedInvalidate);
}

SIZE CGridListHeaderUI::EstimateSize(SIZE szAvailable)
{
	SIZE sz = {0,0};

	if(GetOwner())
	{
		for (int i=0; i<GetCount(); i++)
		{
			sz.cy += GetOwner()->GetRowHeight(i);
		}

		for (int i=0; i<GetOwner()->GetColumnCount(); i++)
		{
			sz.cx += GetOwner()->GetColumnWidth(i);
		}
	}

//	sz.cx = max(sz.cx, szAvailable.cx);
	if(sz.cy == 0 && GetOwner()) 
		sz.cy = GetOwner()->GetDefRowHeight();

	return sz;
}

}