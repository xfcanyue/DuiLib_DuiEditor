#include "stdafx.h"
#include "UITabCtrl.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CTabCtrlUI)

	CTabCtrlUI::CTabCtrlUI()
		: m_bActiveMouseOn(false)
	{
	}

	CTabCtrlUI::~CTabCtrlUI()
	{
		
	}

	LPCTSTR CTabCtrlUI::GetClass() const
	{
		return _T("TabCtrlUI");
	}

	LPVOID CTabCtrlUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_TABCTRL) == 0 ) 
			return static_cast<COptionUI*>(this);
		return COptionUI::GetInterface(pstrName);
	}

	bool CTabCtrlUI::Activate()
	{
		if( !COptionUI::Activate() ) return false;
		
		BindTriggerTabSel();
		return true;
	}

	void CTabCtrlUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if(event.Type == UIEVENT_MOUSEENTER)
		{
			if( IsActiveMouseOn() && ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) 
			{
				SetCaptureState(true);
				SetPushedState(true);
				Invalidate();
				Activate();
				return;
			}
		}

		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsActiveMouseOn() && IsCaptureState() ) 
			{
				SetCaptureState(false);
				SetPushedState(false);
				Invalidate();
				return;	
			}
		}
		__super::DoEvent(event);
	}

	bool CTabCtrlUI::IsSelected() const
	{
		LPCTSTR pstrName = m_sBindTabLayoutName;
		if(pstrName == NULL || (m_iBindTabIndex < 0 && m_sBindTabIndexName.IsEmpty()))
		{
			return __super::IsSelected();
		}

		CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
		if(!pTabLayout) return __super::IsSelected();

		if(m_iBindTabIndex >= 0 && pTabLayout->GetCurSel() == m_iBindTabIndex)
			return true;

		if(!m_sBindTabIndexName.IsEmpty() && m_sBindTabIndexName == pTabLayout->GetCurForm())
			return true;

		return false;
	}

	bool CTabCtrlUI::IsSelectedState() const
	{
		return IsSelected();
	}

	void CTabCtrlUI::BindTriggerTabSel( int _SetSelectIndex /*= -1*/ )
	{
		LPCTSTR pstrName = GetBindTabLayoutName();

		if(GetBindTabLayoutIndex() >= 0 || _SetSelectIndex >= 0 )
		{
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			int sel = _SetSelectIndex >=0?_SetSelectIndex : GetBindTabLayoutIndex();
			if(pTabLayout->GetCurSel() == sel)
				return;
			pTabLayout->SelectItem(sel);
			Selected(true);
		}
		else if(!m_sBindTabIndexName.IsEmpty())
		{
			CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
			if(!pTabLayout) return;
			CControlUI *pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sBindTabIndexName));
			if(pControl)
			{
				pTabLayout->SelectItem(pControl);
				Selected(true);
			}
		}
	}

	void CTabCtrlUI::SetActiveMouseOn(bool bActive) { m_bActiveMouseOn = bActive; }
	bool CTabCtrlUI::IsActiveMouseOn() { return m_bActiveMouseOn; }

	void CTabCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("activemouseon")) == 0 ) 
			SetActiveMouseOn(_tcsicmp(pstrValue, _T("true")) == 0);
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}