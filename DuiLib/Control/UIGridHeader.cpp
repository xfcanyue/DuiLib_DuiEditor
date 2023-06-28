#include "StdAfx.h"
#include "UIGridHeader.h"

namespace DuiLib
{


IMPLEMENT_DUICONTROL(CGridHeaderUI)


CGridHeaderUI::CGridHeaderUI(void) : m_pOwner(NULL)
{
}


CGridHeaderUI::~CGridHeaderUI(void)
{
}

LPCTSTR CGridHeaderUI::GetClass() const
{
	return _T("GridHeaderUI");
}

UINT CGridHeaderUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPVOID CGridHeaderUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, DUI_CTR_GRIDHEADER) == 0 ) return static_cast<CGridHeaderUI*>(this);
	return CVerticalLayoutUI::GetInterface(pstrName);
}

bool CGridHeaderUI::Add(CControlUI* pControl)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDROW))
		return false;

	CGridRowUI *pRow = static_cast<CGridRowUI *>(pControl);
	pRow->SetOwner(GetOwner());

	return CVerticalLayoutUI::Add(pControl);
}

bool CGridHeaderUI::AddAt(CControlUI* pControl, int iIndex)
{
	if(!pControl->GetInterface(DUI_CTR_GRIDROW))
		return false;

	CGridRowUI *pRow = static_cast<CGridRowUI *>(pControl);
	pRow->SetOwner(GetOwner());

	return CVerticalLayoutUI::AddAt(pControl, iIndex);
}

void CGridHeaderUI::DoInit()
{
}

void CGridHeaderUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CVerticalLayoutUI::SetPos(rc, bNeedInvalidate);
}

SIZE CGridHeaderUI::EstimateSize(SIZE szAvailable)
{
	return CVerticalLayoutUI::EstimateSize(szAvailable);
}

}
