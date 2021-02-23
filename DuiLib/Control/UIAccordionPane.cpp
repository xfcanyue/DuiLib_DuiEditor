#include "stdafx.h"
#include "UIAccordionPane.h"

namespace DuiLib
{
	class CAccordionPaneUI::CPaneCheckBox : public COptionUI
	{
	public:
		CPaneCheckBox(CAccordionPaneUI *pOwner) : m_pOwner(pOwner)
		{
		}

		LPVOID CPaneCheckBox::GetInterface(LPCTSTR pstrName)
		{
			if( _tcsicmp(pstrName, _T("PaneCheckBox")) == 0 ) return static_cast<CPaneCheckBox*>(this);
			return __super::GetInterface(pstrName);
		}
		
		virtual bool Activate()
		{
			if( !CControlUI::Activate() ) return false;
			m_pOwner->SetPaneVisible(!m_pOwner->IsPaneVisible(), true);
			return true;
		}
		/*
		virtual bool IsSelected() const
		{
			return m_pOwner->IsPaneVisible();
		}
		*/
		virtual void Selected(bool bSelected, bool bTriggerEvent)
		{
			if(m_bSelected == bSelected) return;

			m_bSelected = bSelected;
			m_pOwner->SetPaneVisible(m_bSelected);
			if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
			else m_uButtonState &= ~UISTATE_SELECTED;

			if( m_pManager != NULL ) {
				if( !m_sGroupName.IsEmpty() ) {
					if( bSelected ) {
						CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
						for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
							COptionUI* pControl = static_cast<COptionUI*>(aOptionGroup->GetAt(i));
							if( pControl != this ) 
							{
								if(pControl->IsSelected())
								{
									pControl->Selected(false, true);
									CPaneCheckBox *pPane = (CPaneCheckBox *)pControl->GetInterface(_T("PaneCheckBox"));
									if(pPane)
									{
										pPane->m_pOwner->SetPaneVisible(false, bTriggerEvent);
									}
								}
							}
						}
					}
				}
			}
		}
		
		CAccordionPaneUI *m_pOwner;
	};

	class CAccordionPaneUI::CAccordionBody : public CAnimationPaneUI
	{
	public:
		CAccordionBody(CAccordionPaneUI *pOwner) : m_pOwner(pOwner) {}

		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
		{
			//m_pOwner->OnAnimationStep(nTotalFrame, nCurFrame, nAnimationID);
			m_pOwner->NeedParentUpdate();
		}
		virtual void OnAnimationStop(INT nAnimationID)
		{
			//m_pOwner->OnAnimationStop(nAnimationID);
			m_pOwner->NeedParentUpdate();
		}

		CAccordionPaneUI *m_pOwner;
	};
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CAccordionPaneUI)

	CAccordionPaneUI::CAccordionPaneUI()
	{
		m_pHeader = new CHorizontalLayoutUI;
		m_pHeader->SetFixedHeight(30);
		m_pHeader->SetChildVAlign(DT_VCENTER);
		m_pHeader->SetChildPadding(10);
		m_pHeader->SetInset(CDuiRect(10,0,10,0));
		__super::Add(m_pHeader);

		m_pHeader->Add(m_pHeaderIcon = new CLabelUI);
		m_pHeaderIcon->SetFixedWidth(16);
		m_pHeaderIcon->SetFixedHeight(16);

		m_pHeader->Add(m_pHeaderLabel = new CLabelUI);
		m_pHeaderLabel->SetTextStyle(DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		m_pHeader->Add(m_pHeaderCheckBox = new CPaneCheckBox(this));
		m_pHeaderCheckBox->SetFixedWidth(12);
		m_pHeaderCheckBox->SetFixedHeight(12);

		m_pAnimPane = new CAccordionBody(this);
		m_pAnimPane->SetAnimationDirection(1);
		m_pAnimPane->SetInset(CDuiRect(1,1,1,1));
		__super::Add(m_pAnimPane);

		m_pBodyVerti = new CVerticalLayoutUI;
		m_pAnimPane->Add(m_pBodyVerti);
	}

	CAccordionPaneUI::~CAccordionPaneUI()
	{
		
	}

	LPCTSTR CAccordionPaneUI::GetClass() const
	{
		return DUI_CTR_ACCORDIONPANE;
	}

	LPVOID CAccordionPaneUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_ACCORDIONPANE) == 0 ) 
			return static_cast<CAccordionPaneUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CAccordionPaneUI::DoInit()
	{
		m_pHeaderCheckBox->Selected(IsPaneVisible(), false);
	}

	bool CAccordionPaneUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;

		//m_pHeaderCheckBox->Selected(IsPaneVisible(), true);
		SetPaneVisible(!IsPaneVisible());
		return true;
	}

	void CAccordionPaneUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_BUTTONUP) 
		{
			if( ::PtInRect(&m_pHeader->GetPos(), event.ptMouse) ) 
			{
				Activate();
			}
		}
		__super::DoEvent( event );
	}

	void CAccordionPaneUI::SetPaneVisible(bool bVisible, bool bAnimation)
	{
		bool bPaneVisible = m_pAnimPane->IsPaneVisible();
		if(bPaneVisible == bVisible)
			return;
		bPaneVisible = bVisible;

		if(!bAnimation || !IsVisible())
		{
			m_pAnimPane->SetPaneVisible(bPaneVisible, false);
			NeedParentUpdate();
			return;
		}

		if(bPaneVisible)
		{
			if(GetParent())
			{
				RECT rcCalcPos;
				GetParent()->CalcPos(this, rcCalcPos);
				SetCalcPos(rcCalcPos);
			}
		}

		m_pAnimPane->SetPaneVisible(bPaneVisible);
		m_pHeaderCheckBox->Selected(bPaneVisible, true);
	}

	SIZE CAccordionPaneUI::EstimateSize(SIZE szAvailable)
	{
		if(m_pAnimPane->IsAnimationRunning(PANE_ANIMATION_ID))
		{
			SIZE szRet = m_cxyFixed;
			szRet.cy = m_pHeader->GetFixedHeight() + m_pAnimPane->GetFixedHeight();
// #ifdef _DEBUG
// 			CStringA tmp;
// 			tmp.Format("szRet.cy=%d\n", szRet.cy);
// 			OutputDebugStringA(tmp);
// #endif
			return szRet;
		}
		
		if(!IsPaneVisible() && !GetParent()->IsCalPosNow())
		{
			RECT rc = m_pHeader->GetPos();
			SIZE szRet = {rc.right - rc.left, rc.bottom - rc.top};
			return szRet;
		}
		return __super::EstimateSize(szAvailable);
	}

	int CAccordionPaneUI::GetFixedHeight() const
	{
		CAccordionPaneUI *pThis = const_cast<CAccordionPaneUI *>(this);
		if(m_pAnimPane->IsAnimationRunning(PANE_ANIMATION_ID))
		{
			SIZE szRet = m_cxyFixed;
			szRet.cy = m_pHeader->GetFixedHeight() + m_pAnimPane->GetFixedHeight();
// #ifdef _DEBUG
// 			CStringA tmp;
// 			tmp.Format("cy=%d\n", szRet.cy);
// 			OutputDebugStringA(tmp);
// #endif
			return szRet.cy;
		}
		if(!IsPaneVisible() && !pThis->GetParent()->IsCalPosNow())
		{
			RECT rc = m_pHeader->GetPos();
			SIZE szRet = {rc.right - rc.left, rc.bottom - rc.top};
			return szRet.cy;
		}
		
		return __super::GetFixedHeight();
	}

	CControlUI* CAccordionPaneUI::GetItemAt(int iIndex) const
	{
		return m_pBodyVerti->GetItemAt(iIndex);
	}

	int CAccordionPaneUI::GetItemIndex(CControlUI* pControl) const
	{
		return m_pBodyVerti->GetItemIndex(pControl);
	}

	bool CAccordionPaneUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		return m_pBodyVerti->SetItemIndex(pControl, iIndex);
	}

	int CAccordionPaneUI::GetCount() const
	{
		return m_pBodyVerti->GetCount();
	}

	bool CAccordionPaneUI::Add(CControlUI* pControl)
	{
		return m_pBodyVerti->Add(pControl);
	}

	bool CAccordionPaneUI::AddAt(CControlUI* pControl, int iIndex)
	{
		return m_pBodyVerti->AddAt(pControl, iIndex);
	}

	bool CAccordionPaneUI::Remove(CControlUI* pControl)
	{
		return m_pBodyVerti->Remove(pControl);
	}

	bool CAccordionPaneUI::RemoveAt(int iIndex)
	{
		return m_pBodyVerti->RemoveAt(iIndex);
	}

	void CAccordionPaneUI::RemoveAll()
	{
		m_pBodyVerti->RemoveAll();
	}

	CHorizontalLayoutUI *CAccordionPaneUI::GetPaneHeader()
	{
		return m_pHeader;
	}

	CVerticalLayoutUI *CAccordionPaneUI::GetPaneBody()
	{
		return m_pBodyVerti;
	}

	bool CAccordionPaneUI::IsPaneVisible() const
	{
		return m_pAnimPane->IsPaneVisible();
	}

	void CAccordionPaneUI::SetFrameCount(int framecount) { m_pAnimPane->SetFrameCount(framecount); }
	int CAccordionPaneUI::GetFrameCount() const { return m_pAnimPane->GetFrameCount(); }
	void CAccordionPaneUI::SetFrameDelay(int nDelay) { m_pAnimPane->SetFrameDelay(nDelay); }
	int CAccordionPaneUI::GetFrameDelay() const { return m_pAnimPane->GetFrameDelay(); }

	void CAccordionPaneUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("headerstyle")) == 0 ) 
		{
			m_pHeader->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconstyle")) == 0 ) 
		{
			m_pHeaderIcon->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("textstyle")) == 0 ) 
		{
			m_pHeaderLabel->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("checkboxstyle")) == 0 ) 
		{
			m_pHeaderCheckBox->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("bodystyle")) == 0 ) 
		{
			m_pAnimPane->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("panevisible")) == 0 ) 
		{
			SetPaneVisible(_tcsicmp(pstrValue, _T("true")) == 0, false);
		}
		else if( _tcsicmp(pstrName, _T("framecount")) == 0 ) {
			m_pAnimPane->SetFrameCount(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("framedelay")) == 0 ) {
			m_pAnimPane->SetFrameDelay(_ttoi(pstrValue));
		}
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}