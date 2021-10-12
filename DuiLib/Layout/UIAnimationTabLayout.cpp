#include "StdAfx.h"
#include "UIAnimationTabLayout.h"

namespace DuiLib {
	IMPLEMENT_DUICONTROL(CAnimationTabLayoutUI)

	CAnimationTabLayoutUI::CAnimationTabLayoutUI() : 
		m_nPositiveDirection( 1 ),
		m_pCurrentControl( NULL ),
		m_bControlVisibleFlag( false )
	{
		SetAnimation(DuiAnim_horizontal);
		SetFrameCount(24);
		SetFrameDelay(5);
	}

	LPCTSTR CAnimationTabLayoutUI::GetClass() const
	{
		return _T("AnimationTabLayoutUI");
	}

	LPVOID CAnimationTabLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("AnimationTabLayout")) == 0 ) 
			return static_cast<CAnimationTabLayoutUI*>(this);
		return CTabLayoutUI::GetInterface(pstrName);
	}

	bool CAnimationTabLayoutUI::SelectItem( int iIndex )
	{
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
		if( iIndex == m_iCurSel ) return true;
		if( iIndex > m_iCurSel ) m_nPositiveDirection = -1;
		if( iIndex < m_iCurSel ) m_nPositiveDirection = 1;

		int iOldSel = m_iCurSel;
		m_iCurSel = iIndex;
		for( int it = 0; it < m_items.GetSize(); it++ ) {
			if( it == iIndex ) {
				GetItemAt(it)->SetVisible(true);
				GetItemAt(it)->SetFocus();
				m_bControlVisibleFlag = false;
				m_pCurrentControl = static_cast<CControlUI*>(m_items[it]);
				
			}
			else GetItemAt(it)->SetVisible(false);
		}

		NeedParentUpdate();
		if( NULL != m_pCurrentControl ) m_pCurrentControl->SetVisible( false );
		AnimationSwitch();

		if( m_pManager != NULL ) {
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, iOldSel);

			CControlUI *pActive = GetItemAt(m_iCurSel);
			if(pActive && !pActive->GetName().IsEmpty()) m_pManager->SendNotify(pActive, DUI_MSGTYPE_TABACTIVEFORM,  m_iCurSel, m_iOldSel);
			CControlUI *pNoActive = GetItemAt(m_iOldSel);
			if(pNoActive && !pNoActive->GetName().IsEmpty()) m_pManager->SendNotify(pNoActive, DUI_MSGTYPE_TABNOACTIVEFORM,  m_iCurSel, m_iOldSel);
		}
		return true;
	}

	void CAnimationTabLayoutUI::AnimationSwitch()
	{
		m_rcItemOld = m_rcItem;
		if( GetAnimation() == DuiAnim_horizontal )
		{
			m_rcCurPos.top = m_rcItem.top;
			m_rcCurPos.bottom = m_rcItem.bottom;
			m_rcCurPos.left = m_rcItem.left - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection + m_nPositiveDirection;
			m_rcCurPos.right = m_rcItem.right - ( m_rcItem.right - m_rcItem.left ) * m_nPositiveDirection+ m_nPositiveDirection;		
		}
		else if(GetAnimation() == DuiAnim_vertical)
		{
			m_rcCurPos.left = m_rcItem.left;
			m_rcCurPos.right = m_rcItem.right;
			m_rcCurPos.top = m_rcItem.top - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;
			m_rcCurPos.bottom = m_rcItem.bottom - ( m_rcItem.bottom - m_rcItem.top ) * m_nPositiveDirection;		
		}

		StopAnimation( ANIMATION_ID_TAB );
		StartAnimation( GetFrameDelay(), GetFrameCount(), ANIMATION_ID_TAB );
	}

	void CAnimationTabLayoutUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_TIMER ) 
		{
			OnTimer(  event.wParam );
		}
		__super::DoEvent( event );
	}

	void CAnimationTabLayoutUI::OnTimer( int nTimerID )
	{
		OnAnimationElapse( nTimerID );
	}

	void CAnimationTabLayoutUI::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
	{
		if( !m_bControlVisibleFlag ) {
			m_bControlVisibleFlag = true;
			m_pCurrentControl->SetVisible( true );
		}

		int iStepLen = 0;
		if( GetAnimation() == DuiAnim_horizontal )
		{
			iStepLen = ( m_rcItemOld.right - m_rcItemOld.left ) * m_nPositiveDirection / nTotalFrame;
			if( nCurFrame != nTotalFrame )
			{
				m_rcCurPos.left = m_rcCurPos.left + iStepLen;
				m_rcCurPos.right = m_rcCurPos.right +iStepLen;			
			}
			else
			{
				m_rcItem = m_rcCurPos = m_rcItemOld;
			}
		}
		else if( GetAnimation() == DuiAnim_vertical )
		{
			iStepLen = ( m_rcItemOld.bottom - m_rcItemOld.top ) * m_nPositiveDirection / nTotalFrame;
			if( nCurFrame != nTotalFrame )
			{
				m_rcCurPos.top = m_rcCurPos.top + iStepLen;
				m_rcCurPos.bottom = m_rcCurPos.bottom +iStepLen;			
			}
			else
			{
				m_rcItem = m_rcCurPos = m_rcItemOld;	
			}	
		}
		SetPos(m_rcCurPos);
	}

	void CAnimationTabLayoutUI::OnAnimationStop(int nAnimationID) 
	{
		SetPos(m_rcItemOld);
		NeedParentUpdate();
	}

	void CAnimationTabLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;

		for( int it = 0; it < m_items.GetSize(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) {
				SetFloatPos(it);
				continue;
			}

			if( it != m_iCurSel ) continue;

			RECT rcPadding = pControl->GetPadding();
			rc.left += rcPadding.left;
			rc.top += rcPadding.top;
			rc.right -= rcPadding.right;
			rc.bottom -= rcPadding.bottom;

			SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

			SIZE sz = pControl->EstimateSize(szAvailable);
			if( sz.cx == 0 ) {
				sz.cx = MAX(0, szAvailable.cx);
			}
			if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
			if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();

			if(sz.cy == 0) {
				sz.cy = MAX(0, szAvailable.cy);
			}
			if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
			if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();

			RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy};
			pControl->SetPos(rcCtrl);
		}
	}

	void CAnimationTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		__super::SetAttribute(pstrName, pstrValue);
	}
} // namespace DuiLib