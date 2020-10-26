#include "stdafx.h"
#include "UIAccordionPane.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CAccordionPaneUI)

	CAccordionPaneUI::CAccordionPaneUI() : CUIAnimation( this ), m_nHeaderHeight(30), m_animation_direction(0), m_bPaneVisible(true)
	{
		m_pHeader = new CHorizontalLayoutUI;
		m_pHeader->SetFixedHeight(m_nHeaderHeight);
		m_pHeader->SetChildVAlign(DT_VCENTER);
		__super::Add(m_pHeader);

		m_pHeader->Add(m_pHeaderIcon = new CLabelUI);
		m_pHeader->Add(m_pHeaderLabel = new CLabelUI);
		m_pHeader->Add(m_pHeaderButton = new COptionUI);

		m_pHeaderButton->OnEvent += MakeDelegate(this, &CAccordionPaneUI::OnToggleEvent);

		m_pBody = new CVerticalLayoutUI;
		__super::Add(m_pBody);
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
		
	}

	bool CAccordionPaneUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;

		SetPaneVisible(!IsPaneVisible());
		return true;
	}

	bool CAccordionPaneUI::OnToggleEvent(void *param)
	{
		TEventUI *pEvent = (TEventUI *)param;

		if( pEvent->Type == UIEVENT_KEYDOWN )
		{
			if (IsKeyboardEnabled()) {
				if( pEvent->chKey == VK_SPACE || pEvent->chKey == VK_RETURN ) {
					Activate();
				}
			}
		}	
		if( pEvent->Type == UIEVENT_BUTTONUP )
		{
			if( ::PtInRect(&m_rcItem, pEvent->ptMouse) ) 
			{
				Activate();
			}
		}
		return true;
	}

	void CAccordionPaneUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		if( event.Type == UIEVENT_BUTTONUP) 
		{
			if( ::PtInRect(&m_pHeader->GetPos(), event.ptMouse) ) 
			{
				Activate();
			}
		}
		__super::DoEvent( event );
	}

	void CAccordionPaneUI::OnTimer( int nTimerID )
	{
		OnAnimationElapse( nTimerID );
	}

	void CAccordionPaneUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		NeedParentUpdate();
	}

	void CAccordionPaneUI::OnAnimationStop(INT nAnimationID) 
	{
		NeedParentUpdate();
		if(IsPaneVisible())
			m_pHeaderButton->Selected(true, false);
		else
			m_pHeaderButton->Selected(false, false);
	}

	void CAccordionPaneUI::SetPaneVisible(bool bVisible, bool bAnimation)
	{
		if(m_bPaneVisible == bVisible)
			return;
		m_bPaneVisible = bVisible;
		if(!bAnimation)
		{
			NeedParentUpdate();
			return;
		}

		if(m_bPaneVisible)
		{
			if(GetParent())
			{
				GetParent()->CalcPos(this, m_rcItemOld);
			}
		}
		else
		{
			m_rcItemOld = m_rcItem;
		}

		StopAnimation( PANE_ANIMATION_ID );
		StartAnimation( PANE_ANIMATION_ELLAPSE, PANE_ANIMATION_FRAME_COUNT, PANE_ANIMATION_ID );
	}

	SIZE CAccordionPaneUI::EstimateSize(SIZE szAvailable)
	{
		if(IsAnimationRunning(PANE_ANIMATION_ID))
		{	
			SIZE sz;// = m_cxyFixed;
			sz.cx = m_rcItemOld.right - m_rcItemOld.left;
			sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
			if(IsPaneVisible())
			{
				sz.cy = m_pHeader->GetFixedHeight();
				sz.cy += (m_rcItemOld.bottom - m_rcItemOld.top - m_nHeaderHeight) / PANE_ANIMATION_FRAME_COUNT * GetCurrentFrame(PANE_ANIMATION_ID);
			}
			else
			{
				sz.cy -= (m_rcItemOld.bottom - m_rcItemOld.top - m_nHeaderHeight) / PANE_ANIMATION_FRAME_COUNT * GetCurrentFrame(PANE_ANIMATION_ID);
			}
			return sz;
		}
		if(!IsPaneVisible())
		{
			SIZE sz = m_cxyFixed;
			sz.cy = m_pHeader->GetFixedHeight();
			return sz;
		}
		return __super::EstimateSize(szAvailable);
	}

	int CAccordionPaneUI::GetFixedHeight() const
	{
		CAccordionPaneUI *pThis = const_cast<CAccordionPaneUI *>(this);
		if(pThis->IsAnimationRunning(PANE_ANIMATION_ID))
		{
			SIZE sz;// = m_cxyFixed;
			sz.cx = m_rcItemOld.right - m_rcItemOld.left;
			sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
			if(IsPaneVisible())
			{
				sz.cy = m_pHeader->GetFixedHeight();
				sz.cy += (m_rcItemOld.bottom - m_rcItemOld.top - m_nHeaderHeight) / PANE_ANIMATION_FRAME_COUNT * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
			}
			else
			{
				sz.cy -= (m_rcItemOld.bottom - m_rcItemOld.top - m_nHeaderHeight) / PANE_ANIMATION_FRAME_COUNT * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
			}		
			return sz.cy;
		}
		if(!pThis->IsPaneVisible())
		{
			return m_pHeader->GetFixedHeight();
		}
		return __super::GetFixedHeight();
	}

	CControlUI* CAccordionPaneUI::GetItemAt(int iIndex) const
	{
		return m_pBody->GetItemAt(iIndex);
	}

	int CAccordionPaneUI::GetItemIndex(CControlUI* pControl) const
	{
		return m_pBody->GetItemIndex(pControl);
	}

	bool CAccordionPaneUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		return m_pBody->SetItemIndex(pControl, iIndex);
	}

	int CAccordionPaneUI::GetCount() const
	{
		return m_pBody->GetCount();
	}

	bool CAccordionPaneUI::Add(CControlUI* pControl)
	{
		return m_pBody->Add(pControl);
	}

	bool CAccordionPaneUI::AddAt(CControlUI* pControl, int iIndex)
	{
		return m_pBody->AddAt(pControl, iIndex);
	}

	bool CAccordionPaneUI::Remove(CControlUI* pControl)
	{
		return m_pBody->Remove(pControl);
	}

	bool CAccordionPaneUI::RemoveAt(int iIndex)
	{
		return m_pBody->RemoveAt(iIndex);
	}

	void CAccordionPaneUI::RemoveAll()
	{
		m_pBody->RemoveAll();
	}

	CHorizontalLayoutUI *CAccordionPaneUI::GetPaneHeader()
	{
		return m_pHeader;
	}

	CVerticalLayoutUI *CAccordionPaneUI::GetPaneBody()
	{
		return m_pBody;
	}

	bool CAccordionPaneUI::IsPaneVisible() const
	{
		return m_bPaneVisible;
	}
	void CAccordionPaneUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("headerheight")) == 0 ) 
		{
			m_nHeaderHeight = _ttoi(pstrValue);
			m_pHeader->SetFixedHeight(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("headerstyle")) == 0 ) 
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
		else if( _tcsicmp(pstrName, _T("togglestyle")) == 0 ) 
		{
			m_pHeaderButton->ApplyAttributeList(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("bodystyle")) == 0 ) 
		{
			m_pBody->ApplyAttributeList(pstrValue);
		}
		if( _tcsicmp(pstrName, _T("animation_direction")) == 0)
		{
			if(_tcsicmp( pstrValue, _T("horizontal")) == 0)
				m_animation_direction = 1;
			else
				m_animation_direction = 0;
		}
		else if( _tcsicmp(pstrName, _T("panevisible")) == 0 ) 
		{
			SetPaneVisible(_tcsicmp(pstrValue, _T("true")) == 0, false);
		}
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}