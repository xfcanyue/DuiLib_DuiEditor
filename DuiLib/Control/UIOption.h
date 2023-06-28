#ifndef __UIOPTION_H__
#define __UIOPTION_H__

#pragma once

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	template< typename T = CControlUI >
	class TemplateOptionUI : public TemplateButtonUI<T>
	{
	public:
		TemplateOptionUI() 
		{
			m_bSelected = false;
		}

		virtual ~TemplateOptionUI() 
		{
			if( !m_sGroupName.IsEmpty() && T::GetManager())
				T::GetManager()->RemoveOptionGroup(m_sGroupName, this);
		}

		virtual void DoInit() override
		{
			if(!m_sGroupName.IsEmpty() ) 
			{
				if (T::GetManager()) T::GetManager()->AddOptionGroup(m_sGroupName, this);
			}
		}

// 		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true) override
// 		{
// 			CControlUI::SetManager(pManager, pParent, bInit);
// 			if( bInit && !m_sGroupName.IsEmpty() ) {
// 				if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
// 			}
// 		}

		virtual bool Activate() override
		{
			if( !CControlUI::Activate() ) return false;
			if( !m_sGroupName.IsEmpty() ) Selected(true);
			else Selected(!IsSelected());

			if(T::GetManager() != NULL )
			{
				T::GetManager()->SendNotify(this, DUI_MSGTYPE_CLICK);
				this->BindTriggerTabSel();
				this->SwitchPaneVisible();
			}

			return true;
		}

		virtual void SwitchPaneVisible() override
		{
			if(this->m_sSwitchControlVisible.IsEmpty()) 
				return;

			CControlUI* pControl = T::GetManager()->FindControl(this->m_sSwitchControlVisible);
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

		CDuiString GetGroup() const
		{
			return m_sGroupName;
		}

		void SetGroup(LPCTSTR pStrGroupName = NULL)
		{
			if( pStrGroupName == NULL ) 
			{
				if( m_sGroupName.IsEmpty() ) return;
				m_sGroupName.Empty();
			}
			else 
			{
				if( m_sGroupName == pStrGroupName ) return;
				if (!m_sGroupName.IsEmpty() && T::GetManager()) T::GetManager()->RemoveOptionGroup(m_sGroupName, this);
				m_sGroupName = pStrGroupName;
			}

			if( !m_sGroupName.IsEmpty() ) 
			{
				if (T::GetManager()) T::GetManager()->AddOptionGroup(m_sGroupName, this);
			}
			else {
				if (T::GetManager()) T::GetManager()->RemoveOptionGroup(m_sGroupName, this);
			}

			Selected(IsSelected());
		}

		virtual bool IsSelected() const
		{
			if(this->m_sSwitchControlVisible.IsEmpty())
				return m_bSelected;

			CControlUI* pControl = static_cast<CControlUI*>(T::GetManager()->FindControl(this->m_sSwitchControlVisible.GetData()));
			if(!pControl) return m_bSelected;
			return pControl->IsVisible();
		}

		virtual void Selected(bool bSelected, bool bTriggerEvent=true)
		{
			if(IsSelected() == bSelected) return;

			m_bSelected = bSelected;
			T::SetSelectedState(m_bSelected);

			if(T::GetManager() != NULL ) 
			{
				if( !m_sGroupName.IsEmpty() ) 
				{
					if( m_bSelected ) 
					{
						CStdPtrArray* aOptionGroup = T::GetManager()->GetOptionGroup(m_sGroupName);
						for( int i = 0; i < aOptionGroup->GetSize(); i++ ) 
						{
							CControlUI* pControlx = static_cast<CControlUI*>(aOptionGroup->GetAt(i));
							if(pControlx->GetInterface(DUI_CTR_OPTION))
							{
								TemplateOptionUI<T> *pControl = static_cast<TemplateOptionUI<T>*>(aOptionGroup->GetAt(i));
								if( pControl != this ) {
									pControl->Selected(false);
									pControl->SwitchPaneVisible();
								}
							}
						}
					}
				}	
				if (bTriggerEvent) T::GetManager()->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			}

			T::Invalidate();
		}

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override
		{
			if( _tcsicmp(pstrName, _T("group")) == 0 ) SetGroup(pstrValue);
			else if( _tcsicmp(pstrName, _T("selected")) == 0 ) Selected(_tcsicmp(pstrValue, _T("true")) == 0);
			else T::SetAttribute(pstrName, pstrValue);
		}
	protected:
		bool			m_bSelected;
		CDuiString		m_sGroupName;
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API COptionHLayoutUI : public TemplateOptionUI<CHorizontalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(COptionHLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API COptionVLayoutUI : public TemplateOptionUI<CVerticalLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(COptionVLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API COptionLayoutUI : public TemplateOptionUI<CDynamicLayoutUI>
	{
	public:
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		DECLARE_DUICONTROL(COptionLayoutUI)
	};

	//////////////////////////////////////////////////////////////////////////
	class UILIB_API COptionUI : public CButtonUI
	{
		DECLARE_DUICONTROL(COptionUI)
	public:
		COptionUI();
		virtual ~COptionUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void DoInit() override;
		//void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

		virtual bool Activate() override;

		virtual void SwitchPaneVisible() override;

		LPCTSTR GetGroup() const;
		void SetGroup(LPCTSTR pStrGroupName = NULL);
		virtual bool IsSelected() const;
		virtual void Selected(bool bSelected, bool bTriggerEvent=true);

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
	protected:
		bool			m_bSelected;
		CDuiString		m_sGroupName;
	};

	class UILIB_API CCheckBoxUI : public COptionUI
	{
		DECLARE_DUICONTROL(CCheckBoxUI)

	public:
		CCheckBoxUI();
		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual void SetCheck(bool bCheck);
		virtual bool GetCheck() const;
	protected:
		bool m_bAutoCheck; 

	public:
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
		void SetAutoCheck(bool bEnable);
		virtual void DoEvent(TEventUI& event) override;
		virtual void Selected(bool bSelected, bool bTriggerEvent=true) override;
	};
} // namespace DuiLib

#endif // __UIOPTION_H__