#include "StdAfx.h"
#include "TreeDeviceUI.h"


IMPLEMENT_DUICONTROL(CTreeDeviceUI)

CTreeDeviceUI::CTreeDeviceUI(void)
{
}


CTreeDeviceUI::~CTreeDeviceUI(void)
{
}

LPCTSTR CTreeDeviceUI::GetClass() const
{
	return _T("TreelDeviceUI");
}

LPVOID CTreeDeviceUI::GetInterface( LPCTSTR pstrName )
{
	if( _tcsicmp(pstrName, _T("TreelDeviceUI")) == 0 ) return static_cast<CTreeDeviceUI*>(this);
	return CTreeUI::GetInterface(pstrName);
}

void CTreeDeviceUI::OnDeleteNode(TNodeData *pNodeData)
{
	tagDeviceNodeTag *pTag = (tagDeviceNodeTag *)pNodeData->GetTag();
	if(pTag)
	{
		::DeleteObject(pTag->hIcon);
		delete pTag;
	}
}

bool CTreeDeviceUI::OnPaintItemForeImage(UIRender *pRender, CControlUI *pItem)
{
	CTreeInnerCheckBoxUI *pCheckBox = (CTreeInnerCheckBoxUI *)pItem;
	if(pCheckBox->GetType() != CTreeInnerCheckBoxUI::BTN_ICON)
		return false;

	CTreeItemUI *pItemUI = (CTreeItemUI *)pCheckBox->GetOwner();
	if(!pItemUI) 
		return false;

	TNodeData *pNodeData = pItemUI->GetNodeData();
	tagDeviceNodeTag *pTag = (tagDeviceNodeTag *)pNodeData->GetTag();
	HICON hIcon = NULL;
	if(pTag)
	{
		hIcon = pTag->hIcon;
	}
	if(hIcon == NULL) return false;

	CDuiRect rc = pItem->GetPos();
	::DrawIconEx(pRender->GetDC(), rc.left, rc.top, hIcon, rc.GetWidth(), rc.GetHeight(), 0, NULL, DI_NORMAL);
	return true;
}