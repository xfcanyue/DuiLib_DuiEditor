#include "StdAfx.h"
#include "UITabLayout.h"

namespace DuiLib
{
	IMPLEMENT_DUICONTROL(CTabLayoutUI)
	CTabLayoutUI::CTabLayoutUI() : m_iCurSel(-1), m_iOldSel(-1), 
	m_pCurControl(NULL),m_pLastControl(NULL),m_nMoveDirection(1)
	{
	}

	LPCTSTR CTabLayoutUI::GetClass() const
	{
		return _T("TabLayoutUI");
	}

	LPVOID CTabLayoutUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_TABLAYOUT) == 0 ) return static_cast<CTabLayoutUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	bool CTabLayoutUI::Add(CControlUI* pControl)
	{
		bool ret = CContainerUI::Add(pControl);
		if( !ret ) return ret;

		if(m_iCurSel == -1 && pControl->IsVisible())
		{
			m_iCurSel = GetItemIndex(pControl);
		}
		else
		{
			pControl->SetVisible(false);
		}

		return ret;
	}

	bool CTabLayoutUI::AddAt(CControlUI* pControl, int iIndex)
	{
		bool ret = CContainerUI::AddAt(pControl, iIndex);
		if( !ret ) return ret;

		if(m_iCurSel == -1 && pControl->IsVisible())
		{
			m_iCurSel = GetItemIndex(pControl);
		}
		else if( m_iCurSel != -1 && iIndex <= m_iCurSel )
		{
			m_iCurSel += 1;
		}
		else
		{
			pControl->SetVisible(false);
		}

		return ret;
	}

	bool CTabLayoutUI::Remove(CControlUI* pControl, bool bDoNotDestroy )
	{
		if( pControl == NULL) return false;

		int index = GetItemIndex(pControl);
		bool ret = CContainerUI::Remove(pControl);
		if( !ret ) return false;

		if( m_iCurSel == index)
		{
			if( GetCount() > 0 )
			{
				m_iCurSel=0;
				GetItemAt(m_iCurSel)->SetVisible(true);
			}
			else
				m_iCurSel=-1;
			NeedParentUpdate();
		}
		else if( m_iCurSel > index )
		{
			m_iCurSel -= 1;
		}

		return ret;
	}

	void CTabLayoutUI::RemoveAll()
	{
		m_iCurSel = -1;
		CContainerUI::RemoveAll();
		NeedParentUpdate();
	}

	int CTabLayoutUI::GetCurSel() const
	{
		return m_iCurSel;
	}

	bool CTabLayoutUI::SelectItem(int iIndex)
	{
		if( iIndex < 0 || iIndex >= m_items.GetSize() ) return false;
		if( iIndex == m_iCurSel ) return true;

		if(m_animation == DuiAnim_horizontal || m_animation == DuiAnim_vertical)
		{
			if( iIndex > m_iCurSel ) m_nMoveDirection = -1;
			if( iIndex < m_iCurSel ) m_nMoveDirection = 1;

			m_pCurControl = NULL;
			m_pLastControl = NULL;

			m_iOldSel = m_iCurSel;
			m_iCurSel = iIndex;
			for( int it = 0; it < m_items.GetSize(); it++ )
			{
				if( it == iIndex ) 
				{
					m_pCurControl = static_cast<CControlUI*>(m_items[it]);
				}
				else if(it == m_iOldSel)
				{
					m_pLastControl = static_cast<CControlUI*>(m_items[it]);
				}
				else GetItemAt(it)->SetVisible(false);
			}

			StopAnimation(ANIMATION_ID_TAB);
			StartAnimation(GetFrameDelay(), GetFrameCount(), ANIMATION_ID_TAB);
			return true;
		}

		m_iOldSel = m_iCurSel;
		m_iCurSel = iIndex;
		for( int it = 0; it < m_items.GetSize(); it++ )
		{
			if( it == iIndex ) {
				GetItemAt(it)->SetVisible(true);
				GetItemAt(it)->SetFocus();
				SetPos(m_rcItem);
				m_pCurControl = static_cast<CControlUI*>(m_items[it]);
			}
			else GetItemAt(it)->SetVisible(false);
		}
		NeedParentUpdate();

		if( m_pManager != NULL ) {
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, m_iOldSel);

			CControlUI *pActive = GetItemAt(m_iCurSel);
			if(pActive && !pActive->GetName().IsEmpty()) m_pManager->SendNotify(pActive, DUI_MSGTYPE_TABACTIVEFORM,  m_iCurSel, m_iOldSel);
			CControlUI *pNoActive = GetItemAt(m_iOldSel);
			if(pNoActive && !pNoActive->GetName().IsEmpty()) m_pManager->SendNotify(pNoActive, DUI_MSGTYPE_TABNOACTIVEFORM,  m_iCurSel, m_iOldSel);
		}
		return true;
	}

	bool CTabLayoutUI::SelectItem( CControlUI* pControl )
	{
		int iIndex=GetItemIndex(pControl);
		if (iIndex==-1)
			return false;
		else
			return SelectItem(iIndex);
	}

	bool CTabLayoutUI::SelectForm(LPCTSTR lpstrFormName)
	{
		CControlUI *pControl = FindSubControl(lpstrFormName);
		if(!pControl || pControl->GetName().IsEmpty()) return false;

		CControlUI *pActive = GetItemAt(m_iCurSel);
		if(pControl == pActive)
		{
			if(pActive && !pActive->GetName().IsEmpty()) m_pManager->SendNotify(pActive, DUI_MSGTYPE_TABACTIVEFORM,  m_iCurSel, m_iOldSel);
			CControlUI *pNoActive = GetItemAt(m_iOldSel);
			if(pNoActive && !pNoActive->GetName().IsEmpty()) m_pManager->SendNotify(pNoActive, DUI_MSGTYPE_TABNOACTIVEFORM,  m_iCurSel, m_iOldSel);
			return true;
		}

		return SelectItem(pControl);
	}

	CDuiString CTabLayoutUI::GetCurForm()
	{
		CControlUI *pActive = GetItemAt(m_iCurSel);
		if(!pActive) return _T("");
		return pActive->GetName();
	}

	void CTabLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("selectedid")) == 0 ) SelectItem(_ttoi(pstrValue));
		return CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CTabLayoutUI::DoInit()
	{
// 		if( m_pManager != NULL ) {
// 			CControlUI *pActive = GetItemAt(m_iCurSel);
// 			if(pActive && !pActive->GetName().IsEmpty()) m_pManager->SendNotify(pActive, DUI_MSGTYPE_TABACTIVEFORM,  m_iCurSel, -1);
// 		}
	}

	void CTabLayoutUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		// Adjust for inset
		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;

		if(IsAnimationRunning(ANIMATION_ID_TAB))
		{
			//CMsgWndUI::InsertMsgV(_T("总帧数: %d, 当前帧: %d"), GetFrameCount(), GetCurrentFrame(ANIMATION_ID_TAB));

			for( int it = 0; it < m_items.GetSize(); it++ ) 
			{
				CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
				if( !pControl->IsVisible() ) continue;
				if( pControl->IsFloat() ) {
					SetFloatPos(it);
					continue;
				}

				if( it == m_iCurSel )
				{
// 					RECT rcPadding = pControl->GetPadding();
// 					rc.left += rcPadding.left;
// 					rc.top += rcPadding.top;
// 					rc.right -= rcPadding.right;
// 					rc.bottom -= rcPadding.bottom;

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

					int nHoriPer = (rcCtrl.right - rcCtrl.left) / GetFrameCount() * GetCurrentFrame(ANIMATION_ID_TAB);
					int nVertPer = (rcCtrl.bottom - rcCtrl.top) / GetFrameCount() * GetCurrentFrame(ANIMATION_ID_TAB);

					RECT rcItem = rcCtrl;
					if(m_animation == DuiAnim_horizontal)
					{
						if(m_nMoveDirection == 1) //从右向左边滚动
						{
							rcItem.left = rcCtrl.right - nHoriPer;
							rcItem.right = rcItem.left + (rcCtrl.right - rcCtrl.left);
						}
						else //从左向右滚动
						{
							rcItem.right = rcCtrl.left + nHoriPer;
							rcItem.left = rcItem.right - (rcCtrl.right - rcCtrl.left);
						}
					}
					else if(m_animation == DuiAnim_vertical) //从下往上滚动
					{
						if(m_nMoveDirection == 1)
						{
							rcItem.top = rcCtrl.bottom - nVertPer;
							rcItem.bottom = rcItem.top + (rcCtrl.bottom - rcCtrl.top);
						}
						else //从上往下滚动
						{
							rcItem.bottom = rcCtrl.top + nVertPer;
							rcItem.top = rcItem.top - (rcCtrl.bottom - rcCtrl.top);
						}
					}
					RECT rcPadding = pControl->GetPadding();
					rcItem.left += rcPadding.left;
					rcItem.top += rcPadding.top;
					rcItem.right -= rcPadding.right;
					rcItem.bottom -= rcPadding.bottom;
					//CMsgWndUI::InsertMsgV(_T("Curr: %d,%d,%d,%d"), rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
					pControl->SetPos(rcItem);
				}
				else if(it == m_iOldSel)
				{
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
					int nHoriPer = (rcCtrl.right - rcCtrl.left) / GetFrameCount() * GetCurrentFrame(ANIMATION_ID_TAB);
					int nVertPer = (rcCtrl.bottom - rcCtrl.top) / GetFrameCount() * GetCurrentFrame(ANIMATION_ID_TAB);

					RECT rcItem = rcCtrl;
					if(m_animation == DuiAnim_horizontal)
					{
						if(m_nMoveDirection == 1)
						{
							rcItem.left = rcCtrl.left - nHoriPer;
							rcItem.right = rcCtrl.right - nHoriPer;
						}
						else
						{
							rcItem.left = rcCtrl.left + nHoriPer;
							rcItem.right = rcCtrl.right + nHoriPer;
						}
					}
					else if(m_animation == DuiAnim_vertical)
					{
						if(m_nMoveDirection == 1)
						{
							rcItem.top = rcCtrl.top - nVertPer;
							rcItem.bottom = rcCtrl.bottom - nVertPer;
						}
						else
						{
							rcItem.top = rcCtrl.top + nVertPer;
							rcItem.bottom = rcCtrl.bottom + nVertPer;
						}
					}

					//CMsgWndUI::InsertMsgV(_T("Last: %d,%d,%d,%d"), rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
					pControl->SetPos(rcItem);
				}
			}
			return;
		}

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

	void CTabLayoutUI::OnAnimationStart(int nAnimationID, BOOL bFirstLoop)
	{
		if(m_pCurControl) m_pCurControl->SetVisible(true);		
	}

	void CTabLayoutUI::OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID)
	{
		NeedParentUpdate();
	}

	void CTabLayoutUI::OnAnimationStop(int nAnimationID)
	{
		if(m_pLastControl) m_pLastControl->SetVisible(false);
		if(m_pCurControl) m_pCurControl->SetVisible(true);
		SetPos(m_rcItem);
		NeedParentUpdate();
		if( m_pManager != NULL ) {
			m_pManager->SetNextTabControl();
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT, m_iCurSel, m_iOldSel);

			CControlUI *pActive = GetItemAt(m_iCurSel);
			if(pActive && !pActive->GetName().IsEmpty()) m_pManager->SendNotify(pActive, DUI_MSGTYPE_TABACTIVEFORM,  m_iCurSel, m_iOldSel);
			CControlUI *pNoActive = GetItemAt(m_iOldSel);
			if(pNoActive && !pNoActive->GetName().IsEmpty()) m_pManager->SendNotify(pNoActive, DUI_MSGTYPE_TABNOACTIVEFORM,  m_iCurSel, m_iOldSel);
		}
	}
}
