#include "StdAfx.h"
#include "UIButton.h"

namespace DuiLib
{

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CButtonHLayoutUI)

	LPCTSTR CButtonHLayoutUI::GetClass() const
	{
		return _T("ButtonHLayoutUI");
	}

	LPVOID CButtonHLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON_HLAYOUT) == 0 ) return static_cast<CButtonHLayoutUI*>(this);
		return CHorizontalLayoutUI::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CButtonVLayoutUI)

	LPCTSTR CButtonVLayoutUI::GetClass() const
	{
		return _T("ButtonVLayoutUI");
	}

	LPVOID CButtonVLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON_VLAYOUT) == 0 ) return static_cast<CButtonVLayoutUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CButtonLayoutUI)

	LPCTSTR CButtonLayoutUI::GetClass() const
	{
		return _T("ButtonVLayoutUI");
	}

	LPVOID CButtonLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON_LAYOUT) == 0 ) return static_cast<CButtonLayoutUI*>(this);
		return CDynamicLayoutUI::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CButtonUI)

	CButtonUI::CButtonUI()
	{
		m_iBindTabIndex = -1;
		m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	}

	LPCTSTR CButtonUI::GetClass() const
	{
		return _T("ButtonUI");
	}

	LPVOID CButtonUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_BUTTON) == 0 ) return static_cast<CButtonUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CButtonUI::GetControlFlags() const
	{
		return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
	}

	void CButtonUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
					Activate();
					return;
				}
			}
		}		
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			//if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) 
			if( m_rcItem.PtInRect(event.ptMouse) && IsEnabled() ) 
			{
				SetCaptureState(true);
				SetPushedState(true);
				Invalidate();
			}
			return;
		}	
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			if( IsCaptureState() ) 
			{
				//if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
				if( m_rcItem.PtInRect(event.ptMouse) ) 
					SetPushedState(true);
				else 
					SetPushedState(false);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( IsCaptureState() ) 
			{
				SetCaptureState(false);
				SetPushedState(false);
				Invalidate();
				//if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();	
				if( m_rcItem.PtInRect(event.ptMouse) ) Activate();				
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			if( IsContextMenuUsed() ) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				SetHotState(true);
				Invalidate();
			}
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				SetHotState(false);
				Invalidate();
			}
		}
		CLabelUI::DoEvent(event);
	}

	bool CButtonUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL )
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			BindTriggerTabSel();

			SwitchPaneVisible();
		}
		return true;
	}

	void CButtonUI::SwitchPaneVisible()
	{
		if(m_sSwitchControlVisible.IsEmpty()) 
			return;

		CControlUI* pControl = GetManager()->FindControl(m_sSwitchControlVisible);
		if(!pControl) return;
		pControl->SetPaneVisible(!pControl->IsPaneVisible());
	}

	void CButtonUI::SetSwitchPaneVisible(LPCTSTR ControlName)
	{
		m_sSwitchControlVisible = ControlName;
	}

	void CButtonUI::BindTabIndex(int _BindTabIndex )
	{
		if( _BindTabIndex >= 0)
			m_iBindTabIndex	= _BindTabIndex;
	}

	void CButtonUI::BindTabIndexName(LPCTSTR _BindTabIndexName)
	{
		m_sBindTabIndexName = _BindTabIndexName;
	}

	void CButtonUI::BindTabLayoutName( LPCTSTR _TabLayoutName )
	{
		if(_TabLayoutName)
			m_sBindTabLayoutName = _TabLayoutName;
	}

	void CButtonUI::BindTriggerTabSel( int _SetSelectIndex /*= -1*/ )
	{
		LPCTSTR pstrName = GetBindTabLayoutName();
// 		if(pstrName == NULL || (GetBindTabLayoutIndex() < 0 && _SetSelectIndex < 0))
// 			return;

		if(GetBindTabLayoutIndex() >= 0 || _SetSelectIndex >= 0 )
		{
			CTabLayoutUI* pTabLayout = dynamic_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			pTabLayout->SelectItem(_SetSelectIndex >=0?_SetSelectIndex:GetBindTabLayoutIndex());
		}
		else if(!m_sBindTabIndexName.IsEmpty())
		{
			CTabLayoutUI* pTabLayout = dynamic_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			CControlUI *pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sBindTabIndexName));
			if(pControl)
				pTabLayout->SelectItem(pControl);
		}
	}

	void CButtonUI::RemoveBindTabIndex()
	{
		m_iBindTabIndex	= -1;
		m_sBindTabLayoutName.Empty();
	}

	int CButtonUI::GetBindTabLayoutIndex()
	{
		return m_iBindTabIndex;
	}

	CDuiString CButtonUI::GetBindTabLayoutIndexName()
	{
		return m_sBindTabIndexName;
	}

	LPCTSTR CButtonUI::GetBindTabLayoutName()
	{
		return m_sBindTabLayoutName;
	}

	void CButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("switchpanevisible")) == 0 ) SetSwitchPaneVisible(pstrValue);
		else if( _tcsicmp(pstrName, _T("bindtabindex")) == 0 ) BindTabIndex(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("bindtabindexname")) == 0 ) BindTabIndexName(pstrValue);
		else if( _tcsicmp(pstrName, _T("bindtablayoutname")) == 0 ) BindTabLayoutName(pstrValue);	
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}



// 	void CButtonUI::PaintText(UIRender *pRender)
// 	{
// 		return CLabelUI::PaintText(pRender);
// 	}
}
