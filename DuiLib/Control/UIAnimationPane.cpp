#include "stdafx.h"
#include "UIAnimationPane.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CAnimationPaneUI)

	CAnimationPaneUI::CAnimationPaneUI() : CUIAnimation( this ), m_animation(0), m_bPaneVisible(true)
	{
		m_nFrameCount = 24;
		m_nFrameDelay = 5;
	}

	CAnimationPaneUI::~CAnimationPaneUI()
	{		
	}

	LPCTSTR CAnimationPaneUI::GetClass() const
	{
		return DUI_CTR_ANIMATIONPANE;
	}

	LPVOID CAnimationPaneUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_ANIMATIONPANE) == 0 ) 
			return static_cast<CAnimationPaneUI*>(this);
		return __super::GetInterface(pstrName);
	}

	void CAnimationPaneUI::DoInit()
	{
		
	}

	void CAnimationPaneUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		__super::DoEvent( event );
	}

	void CAnimationPaneUI::OnTimer( int nTimerID )
	{
		OnAnimationElapse( nTimerID );
	}

	void CAnimationPaneUI::OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID)
	{
		NeedParentUpdate();
	}

	void CAnimationPaneUI::OnAnimationStop(INT nAnimationID) 
	{
		NeedParentUpdate();
	}

	void CAnimationPaneUI::SetPaneVisible(bool bVisible, bool bAnimation)
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
		StartAnimation( GetFrameDelay(), GetFrameCount(), PANE_ANIMATION_ID );
	}

	SIZE CAnimationPaneUI::EstimateSize(SIZE szAvailable)
	{
		if(m_animation == 1)
		{
			if(IsAnimationRunning(PANE_ANIMATION_ID))
			{	
				SIZE sz;// = m_cxyFixed;
				sz.cx = m_rcItemOld.right - m_rcItemOld.left;
				sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
				if(IsPaneVisible())
				{
					sz.cy = (m_rcItemOld.bottom - m_rcItemOld.top) / GetFrameCount() * GetCurrentFrame(PANE_ANIMATION_ID);
				}
				else
				{
					sz.cy -= (m_rcItemOld.bottom - m_rcItemOld.top) / GetFrameCount() * GetCurrentFrame(PANE_ANIMATION_ID);
					if(sz.cy <= 0) sz.cy = 1;
				}
				return sz;
			}
			if(!IsPaneVisible())
			{
				SIZE sz = m_cxyFixed;
				sz.cy = 1;
				return sz;
			}
		}
		else if(m_animation == 2)
		{
			if(IsAnimationRunning(PANE_ANIMATION_ID))
			{	
				SIZE sz;// = m_cxyFixed;
				sz.cx = m_rcItemOld.right - m_rcItemOld.left;
				sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
				if(IsPaneVisible())
				{
					sz.cx = (m_rcItemOld.right - m_rcItemOld.left) / GetFrameCount() * GetCurrentFrame(PANE_ANIMATION_ID);
				}
				else
				{
					sz.cx -= (m_rcItemOld.right - m_rcItemOld.left) / GetFrameCount() * GetCurrentFrame(PANE_ANIMATION_ID);
					if(sz.cx <= 0) sz.cx = 1;
				}
				return sz;
			}
			if(!IsPaneVisible())
			{
				SIZE sz = m_cxyFixed;
				sz.cx = 1;
				return sz;
			}
		}
		return __super::EstimateSize(szAvailable);
	}

	int CAnimationPaneUI::GetFixedHeight() const
	{
		if(m_animation == 1)
		{
			CAnimationPaneUI *pThis = const_cast<CAnimationPaneUI *>(this);
			if(pThis->IsAnimationRunning(PANE_ANIMATION_ID))
			{
				SIZE sz;// = m_cxyFixed;
				sz.cx = m_rcItemOld.right - m_rcItemOld.left;
				sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
				if(IsPaneVisible())
				{
					sz.cy = (m_rcItemOld.bottom - m_rcItemOld.top) / GetFrameCount() * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
				}
				else
				{
					sz.cy -= (m_rcItemOld.bottom - m_rcItemOld.top) / GetFrameCount() * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
					if(sz.cy <= 0) sz.cy = 1;
				}		
				return sz.cy;
			}
			if(!pThis->IsPaneVisible())
			{
				return 1;
			}
		}
		return __super::GetFixedHeight();
	}

	int CAnimationPaneUI::GetFixedWidth() const
	{
		if(m_animation == 2)
		{
			CAnimationPaneUI *pThis = const_cast<CAnimationPaneUI *>(this);
			if(pThis->IsAnimationRunning(PANE_ANIMATION_ID))
			{
				SIZE sz;
				sz.cx = m_rcItemOld.right - m_rcItemOld.left;
				sz.cy = m_rcItemOld.bottom - m_rcItemOld.top;
				if(IsPaneVisible())
				{
					sz.cx = (m_rcItemOld.right - m_rcItemOld.left) / GetFrameCount() * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
				}
				else
				{
					sz.cx -= (m_rcItemOld.right - m_rcItemOld.left) / GetFrameCount() * pThis->GetCurrentFrame(PANE_ANIMATION_ID);
					if(sz.cx <= 0) sz.cx = 1;
				}		
				return sz.cx;
			}
			if(!pThis->IsPaneVisible())
			{
				return 1;
			}
		}
		return __super::GetFixedWidth();
	}

	bool CAnimationPaneUI::IsPaneVisible() const
	{
		return m_bPaneVisible;
	}

	void CAnimationPaneUI::SetFrameCount(int framecount) { m_nFrameCount = framecount; }
	int CAnimationPaneUI::GetFrameCount() const { return m_nFrameCount; }
	void CAnimationPaneUI::SetFrameDelay(int nDelay) { m_nFrameDelay = nDelay; }
	int CAnimationPaneUI::GetFrameDelay() const { return m_nFrameDelay; }

	void CAnimationPaneUI::SetAnimationDirection(int nAnim)
	{
		m_animation = nAnim;
	}

	int CAnimationPaneUI::GetAnimationDirection()
	{
		return m_animation;
	}

	void CAnimationPaneUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("animation")) == 0)
		{
			if(_tcsicmp( pstrValue, _T("horizontal")) == 0)
				SetAnimationDirection(2);
			else if(_tcsicmp( pstrValue, _T("vertical")) == 0)
				SetAnimationDirection(1);
			else
				SetAnimationDirection(0);
		}
		else if( _tcsicmp(pstrName, _T("panevisible")) == 0 ) 
		{
			SetPaneVisible(_tcsicmp(pstrValue, _T("true")) == 0, false);
		}
		else if( _tcsicmp(pstrName, _T("framecount")) == 0 ) {
			SetFrameCount(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("framedelay")) == 0 ) {
			SetFrameDelay(_ttoi(pstrValue));
		}
		else 
			__super::SetAttribute(pstrName, pstrValue);
	}
}