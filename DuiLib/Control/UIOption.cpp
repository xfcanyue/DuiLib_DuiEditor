#include "StdAfx.h"
#include "UIOption.h"

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(COptionHLayoutUI)

	LPCTSTR COptionHLayoutUI::GetClass() const
	{
		return _T("OptionHLayoutUI");
	}

	LPVOID COptionHLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_OPTION_HLAYOUT) == 0 ) return static_cast<COptionHLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(COptionVLayoutUI)

	LPCTSTR COptionVLayoutUI::GetClass() const
	{
		return _T("OptionVLayoutUI");
	}

	LPVOID COptionVLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_OPTION_VLAYOUT) == 0 ) return static_cast<COptionVLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(COptionLayoutUI)

	LPCTSTR COptionLayoutUI::GetClass() const
	{
		return _T("OptionLayoutUI");
	}

	LPVOID COptionLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_OPTION_LAYOUT) == 0 ) return static_cast<COptionLayoutUI*>(this);
		return __super::GetInterface(pstrName);
	}

	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(COptionUI)
	COptionUI::COptionUI() : m_bSelected(false)
	{
	}

	COptionUI::~COptionUI()
	{
		if( !m_sGroupName.IsEmpty() && m_pManager ) m_pManager->RemoveOptionGroup(m_sGroupName, this);
	}
	 
	LPCTSTR COptionUI::GetClass() const
	{
		return _T("OptionUI");
	}

	LPVOID COptionUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_OPTION) == 0 ) return static_cast<COptionUI*>(this);
		return CButtonUI::GetInterface(pstrName);
	}

	void COptionUI::DoInit() 
	{
		if(!m_sGroupName.IsEmpty() ) 
		{
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
		}
	}

// 	void COptionUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
// 	{
// 		CControlUI::SetManager(pManager, pParent, bInit);
// 		if( bInit && !m_sGroupName.IsEmpty() ) {
// 			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
// 		}
// 	}

	LPCTSTR COptionUI::GetGroup() const
	{
		return m_sGroupName;
	}

	void COptionUI::SetGroup(LPCTSTR pStrGroupName)
	{
		if( pStrGroupName == NULL ) {
			if( m_sGroupName.IsEmpty() ) return;
			m_sGroupName.Empty();
		}
		else {
			if( m_sGroupName == pStrGroupName ) return;
			if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
			m_sGroupName = pStrGroupName;
		}

		if( !m_sGroupName.IsEmpty() ) {
			if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
		}
		else {
			if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
		}

		Selected(IsSelected());
	}

	bool COptionUI::IsSelected() const
	{
		//return m_bSelected;
		if(m_sSwitchControlVisible.IsEmpty())
			return m_bSelected;

		CControlUI* pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sSwitchControlVisible.GetData()));
		if(!pControl) return m_bSelected;
		return pControl->IsVisible();
	}

	void COptionUI::Selected(bool bSelected, bool bTriggerEvent)
	{
		if(IsSelected() == bSelected) return;

		m_bSelected = bSelected;
		SetSelectedState(m_bSelected);

		if( m_pManager != NULL ) 
		{
			if( !m_sGroupName.IsEmpty() ) 
			{
				if( m_bSelected ) 
				{
					CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
					for( int i = 0; i < aOptionGroup->GetSize(); i++ ) 
					{
						CControlUI* pControlx = static_cast<CControlUI*>(aOptionGroup->GetAt(i));
						if(pControlx->GetInterface(DUI_CTR_OPTION))
						{
							COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
							if( pControl != this ) {
								pControl->Selected(false);
								pControl->SwitchPaneVisible();
							}
						}
					}
				}
			}
			if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
		}

		Invalidate();
	}

	bool COptionUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( !m_sGroupName.IsEmpty() ) Selected(true);
		else Selected(!IsSelected());

		if( m_pManager != NULL )
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
			BindTriggerTabSel();
			SwitchPaneVisible();
		}

		return true;
	}

	void COptionUI::SwitchPaneVisible()
	{
		if(m_sSwitchControlVisible.IsEmpty()) 
			return;

		CControlUI* pControl = GetManager()->FindControl(m_sSwitchControlVisible);
		if(!pControl) return;
		bool bVisible = pControl->IsPaneVisible();
		if(m_bSelected && !bVisible)
		{
			pControl->SetPaneVisible(true);
		}
		else if(!m_bSelected && bVisible)
		{
			pControl->SetPaneVisible(false);
		}
	}

	void COptionUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("group")) == 0 ) SetGroup(pstrValue);
		else if( _tcsicmp(pstrName, _T("selected")) == 0 ) Selected(_tcsicmp(pstrValue, _T("true")) == 0);
		else __super::SetAttribute(pstrName, pstrValue);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CCheckBoxUI)

	CCheckBoxUI::CCheckBoxUI() : m_bAutoCheck(FALSE)
	{

	}

	LPCTSTR CCheckBoxUI::GetClass() const
	{
		return _T("CheckBoxUI");
	}
	LPVOID CCheckBoxUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_CHECKBOX) == 0 ) return static_cast<CCheckBoxUI*>(this);
		return COptionUI::GetInterface(pstrName);
	}

	void CCheckBoxUI::SetCheck(bool bCheck)
	{
		Selected(bCheck);
	}

	bool  CCheckBoxUI::GetCheck() const
	{
		return IsSelected();
	}

	void CCheckBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("EnableAutoCheck")) == 0 ) SetAutoCheck(_tcsicmp(pstrValue, _T("true")) == 0);
		
		COptionUI::SetAttribute(pstrName, pstrValue);
	}
	void CCheckBoxUI::SetAutoCheck(bool bEnable)
	{
		m_bAutoCheck = bEnable;
	}
	void CCheckBoxUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else COptionUI::DoEvent(event);
			return;
		}
		if( m_bAutoCheck && (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK))
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() )
			{
				SetCheck(!GetCheck()); 
				m_pManager->SendNotify(this, DUI_MSGTYPE_CHECKCLICK, 0, 0);
				Invalidate();
			}
			return;
		}
		COptionUI::DoEvent(event);
	}
	void CCheckBoxUI::Selected(bool bSelected, bool bTriggerEvent)
	{
		if( m_bSelected == bSelected ) return;
		m_bSelected = bSelected;
		SetSelectedState(m_bSelected);

		if( m_pManager != NULL ) {
			if( !m_sGroupName.IsEmpty() ) {
				if( m_bSelected ) {
					CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
					for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
						COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
						if( pControl != this ) {
							pControl->Selected(FALSE);
						}
					}
					if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, IsSelected(), 0);
				}
			}
			else {
				if (bTriggerEvent) m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED, IsSelected(), 0);
			}
		}

		Invalidate();
	}
}