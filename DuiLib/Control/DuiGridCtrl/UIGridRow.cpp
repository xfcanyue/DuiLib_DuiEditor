#include "StdAfx.h"
#include "UIGridRow.h"


namespace DuiLib {
//add by liqs99 表格类
/////////////////////////////////////////////////////////////////////////////////////

CGridRowUI::CGridRowUI(void) : m_pOwner(NULL), m_bDragable(true)
{
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
	return UIFLAG_TABSTOP;
}

LPVOID CGridRowUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("GridRow")) == 0 ) return static_cast<CGridRowUI*>(this);
	return __super::GetInterface(pstrName);
}

void CGridRowUI::SetOwner(CControlUI *pOwnder)
{
	m_pOwner = pOwnder;
}

CControlUI *CGridRowUI::GetOwner()
{
	return m_pOwner;
}

void CGridRowUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) 
	{
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
	}

// 	if( event.Type == UIEVENT_SETCURSOR )
// 	{
// 		RECT rcSeparator = GetThumbRect();
// 		if (m_iSepWidth>=0)//111024 by cddjr, 增加分隔符区域，方便用户拖动
// 			rcSeparator.left-=4;
// 		else
// 			rcSeparator.right+=4;
// 		if( IsEnabled() && m_bDragable && ::PtInRect(&rcSeparator, event.ptMouse) ) 
// 		{
// 			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
// 			return;
// 		}
// 	}

	__super::DoEvent(event);
}

} //namespace
