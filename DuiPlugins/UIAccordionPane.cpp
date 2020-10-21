#include "stdafx.h"
#include "UIAccordionPane.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CAccordionPaneUI)

	CAccordionPaneUI::CAccordionPaneUI() : CUIAnimation( this ), m_bPaneVisible(true)
	{
		m_pHeader = new CHorizontalLayoutUI;
		m_pHeader->SetFixedHeight(30);
		m_pHeader->SetChildVAlign(DT_VCENTER);
		__super::Add(m_pHeader);

		m_pHeader->Add(m_pHeaderIcon = new CLabelUI);
		m_pHeader->Add(m_pHeaderLabel = new CLabelUI);
		m_pHeader->Add(m_pHeaderButton = new COptionUI);

		m_pBody = new CVerticalLayoutUI;
		__super::Add(m_pBody);

// 		m_pHeaderIcon->SetBkColor(0xFFFF0000);
// 		m_pHeaderLabel->SetBkColor(0xFF00FF00);
// 		m_pHeaderButton->SetBkColor(0xFF0000FF);
//		m_pBody->SetBkColor(0xFF00005A);
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

	void CAccordionPaneUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		if( event.Type == UIEVENT_BUTTONUP) 
		{
			if( IsEnabled() ) 
			{
				SetPaneVisible(!IsPaneVisible());
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
// 		int iStepLen = ( m_rcItemOld.bottom - m_rcItemOld.top - m_pHeader->GetFixedHeight() ) / nTotalFrame;
// 
// 		if(m_bPaneVisible)
// 			m_rcCurPos.bottom += iStepLen;
// 		else
// 			m_rcCurPos.bottom -= iStepLen;
// 		SetPos(m_rcCurPos);
	}

	void CAccordionPaneUI::OnAnimationStop(INT nAnimationID) 
	{
		//NeedParentUpdate();
	}

	SIZE CAccordionPaneUI::EstimateSize(SIZE szAvailable)
	{
		if(!IsPaneVisible())
		{
			SIZE sz = m_cxyFixed;
			sz.cy = m_pHeader->GetFixedHeight();
			return sz;
		}
		return __super::EstimateSize(szAvailable);
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

	void CAccordionPaneUI::SetPaneVisible(bool bVisible)
	{
		if(m_bPaneVisible == bVisible)
			return;
		m_bPaneVisible = bVisible;

		m_pBody->SetVisible(bVisible);
		NeedParentUpdate();
		return;


		m_rcItemOld = m_rcItem;

		m_rcCurPos.left		= m_rcItem.left;
		m_rcCurPos.right	= m_rcItem.right;
		m_rcCurPos.top		= m_rcItem.top;
		m_rcCurPos.bottom	= m_rcItem.bottom;

		//StopAnimation( TAB_ANIMATION_ID );
		//StartAnimation( TAB_ANIMATION_ELLAPSE, TAB_ANIMATION_FRAME_COUNT, TAB_ANIMATION_ID );
	}

	bool CAccordionPaneUI::IsPaneVisible()
	{
		return m_bPaneVisible;
	}
	void CAccordionPaneUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("headerheight")) == 0 ) 
		{
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
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}