#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#pragma once

namespace DuiLib
{
	template< typename T = CControlUI>
	class TemplateButtonUI : public TemplateLabelUI<T>
	{
	public:
		TemplateButtonUI() 
		{
			m_uTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER;
		}
		virtual ~TemplateButtonUI() {}

		virtual UINT GetControlFlags() const override
		{
			return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
		}

		virtual bool Activate() override
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

		virtual void DoEvent(TEventUI& event) override
		{
			if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
				if( m_pParent != NULL ) m_pParent->DoEvent(event);
				else __super::DoEvent(event);
				return;
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
				if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) 
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
					if( ::PtInRect(&m_rcItem, event.ptMouse) ) 
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
					if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
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
			__super::DoEvent(event);
		}

		virtual void SwitchPaneVisible()
		{
			if(m_sSwitchControlVisible.IsEmpty()) 
				return;

			CControlUI* pControl = GetManager()->FindControl(m_sSwitchControlVisible);
			if(!pControl) return;
			pControl->SetPaneVisible(!pControl->IsPaneVisible());
		}

		virtual void SetSwitchPaneVisible(LPCTSTR ControlName)
		{
			m_sSwitchControlVisible = ControlName;
		}

		virtual void BindTabIndex(int _BindTabIndex)
		{
			if( _BindTabIndex >= 0)
				m_iBindTabIndex	= _BindTabIndex;
		}

		virtual void BindTabIndexName(LPCTSTR _BindTabIndexName)
		{
			m_sBindTabIndexName = _BindTabIndexName;
		}

		virtual void BindTabLayoutName(LPCTSTR _TabLayoutName)
		{
			if(_TabLayoutName)
				m_sBindTabLayoutName = _TabLayoutName;
		}

		virtual void BindTriggerTabSel(int _SetSelectIndex = -1)
		{
			CDuiString pstrName = GetBindTabLayoutName();

			if(GetBindTabLayoutIndex() >= 0 || _SetSelectIndex >= 0 )
			{
				CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
				if(!pTabLayout) return;
				pTabLayout->SelectItem(_SetSelectIndex >=0?_SetSelectIndex:GetBindTabLayoutIndex());
			}
			else if(!m_sBindTabIndexName.IsEmpty())
			{
				CTabLayoutUI* pTabLayout = static_cast<CTabLayoutUI*>(GetManager()->FindControl(pstrName));
				if(!pTabLayout) return;
				CControlUI *pControl = static_cast<CControlUI*>(GetManager()->FindControl(m_sBindTabIndexName));
				if(pControl)
					pTabLayout->SelectItem(pControl);
			}
		}

		virtual void RemoveBindTabIndex()
		{
			m_iBindTabIndex	= -1;
			m_sBindTabLayoutName.Empty();
		}

		virtual int	 GetBindTabLayoutIndex()
		{
			return m_iBindTabIndex;
		}

		virtual CDuiString GetBindTabLayoutIndexName()
		{
			return m_sBindTabIndexName;
		}

		virtual CDuiString GetBindTabLayoutName()
		{
			return m_sBindTabLayoutName;
		}

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override
		{
			if( _tcsicmp(pstrName, _T("switchpanevisible")) == 0 ) SetSwitchPaneVisible(pstrValue);
			else if( _tcsicmp(pstrName, _T("bindtabindex")) == 0 ) BindTabIndex(_ttoi(pstrValue));
			else if( _tcsicmp(pstrName, _T("bindtabindexname")) == 0 ) BindTabIndexName(pstrValue);
			else if( _tcsicmp(pstrName, _T("bindtablayoutname")) == 0 ) BindTabLayoutName(pstrValue);	
			else __super::SetAttribute(pstrName, pstrValue);
		}

	protected:
		CDuiString m_sSwitchControlVisible;

		int			m_iBindTabIndex;
		CDuiString	m_sBindTabIndexName;
		CDuiString	m_sBindTabLayoutName;
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CButtonHLayoutUI : public TemplateButtonUI<CHorizontalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CButtonHLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CButtonVLayoutUI : public TemplateButtonUI<CVerticalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CButtonVLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CButtonLayoutUI : public TemplateButtonUI<CDynamicLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(CButtonLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API CButtonUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CButtonUI)

	public:
		CButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void DoEvent(TEventUI& event);

		virtual void SwitchPaneVisible();
		virtual void SetSwitchPaneVisible(LPCTSTR ControlName);

		virtual void BindTabIndex(int _BindTabIndex);
		virtual void BindTabIndexName(LPCTSTR _BindTabIndexName);
		virtual void BindTabLayoutName(LPCTSTR _TabLayoutName);
		virtual void BindTriggerTabSel(int _SetSelectIndex = -1);
		virtual void RemoveBindTabIndex();
		virtual int	 GetBindTabLayoutIndex();
		virtual CDuiString GetBindTabLayoutIndexName();
		virtual LPCTSTR GetBindTabLayoutName();

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

//		virtual void PaintText(UIRender *pRender) override;
	protected:
		CDuiString m_sSwitchControlVisible;

		int			m_iBindTabIndex;
		CDuiString	m_sBindTabIndexName;
		CDuiString	m_sBindTabLayoutName;
	};

}	// namespace DuiLib

#endif // __UIBUTTON_H__