#include "StdAfx.h"

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//

	class CComboWnd : public CWindowWnd, public INotifyUI
	{
	public:
		void Init(CComboUI* pOwner);
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void Notify(TNotifyUI& msg) override;

		void EnsureVisible(int iIndex);
		void Scroll(int dx, int dy);

#if(_WIN32_WINNT >= 0x0501)
		virtual UINT GetClassStyle() const;
#endif
		bool IsHitItem(POINT ptMouse);
	public:
		CPaintManagerUI m_pm;
		CComboUI* m_pOwner;
		CVerticalLayoutUI* m_pLayout;
		int m_iOldSel;
		bool m_bHitItem;
	};

	void CComboWnd::Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("windowinit"))
		{
			EnsureVisible(m_iOldSel);
		}
		else if(msg.sType == _T("click")) {
			// 测试代码
			CDuiString sName = msg.pSender->GetName();
			CControlUI* pCtrl = msg.pSender;
			while(pCtrl != NULL) {
				IListItemUI* pListItem = (IListItemUI*)pCtrl->GetInterface(DUI_CTR_LISTITEM);
				if(pListItem != NULL ) {
					break;
				}
				pCtrl = pCtrl->GetParent();
			}
		}
	}

	void CComboWnd::Init(CComboUI* pOwner)
	{
		m_bHitItem = false;
		m_pOwner = pOwner;
		m_pLayout = NULL;
		m_iOldSel = m_pOwner->GetCurSel();

		// Position the popup window in absolute space
		SIZE szDrop = m_pOwner->GetDropBoxSize();
		RECT rcOwner = pOwner->GetPos();
		RECT rc = rcOwner;
		rc.top = rc.bottom;		// 父窗口left、bottom位置作为弹出窗口起点
		rc.bottom = rc.top + szDrop.cy;	// 计算弹出窗口高度
		if( szDrop.cx > 0 ) rc.right = rc.left + szDrop.cx;	// 计算弹出窗口宽度

		SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
		int cyFixed = 0;
		for( int it = 0; it < pOwner->GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(pOwner->GetItemAt(it));
			if( !pControl->IsVisible() ) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cyFixed += sz.cy;
		}
		cyFixed += 4;
		rc.bottom = rc.top + MIN(cyFixed, szDrop.cy);

		::MapWindowRect(pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);

		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		if( rc.bottom > rcWork.bottom ) {
			rc.left = rcOwner.left;
			rc.right = rcOwner.right;
			if( szDrop.cx > 0 ) rc.right = rc.left + szDrop.cx;
			rc.top = rcOwner.top - MIN(cyFixed, szDrop.cy);
			rc.bottom = rcOwner.top;
			::MapWindowRect(pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
		}

		Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW, rc);
		// HACK: Don't deselect the parent's caption
		HWND hWndParent = m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);
		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	LPCTSTR CComboWnd::GetWindowClassName() const
	{
		return _T("ComboWnd");
	}

	void CComboWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->m_pWindow = NULL;
		m_pOwner->SetPushedState(false);
		m_pOwner->Invalidate();
		delete this;
	}

	bool CComboWnd::IsHitItem(POINT ptMouse)
	{
		CControlUI* pControl = m_pm.FindControl(ptMouse);
		if(pControl != NULL) {
			LPVOID pInterface = pControl->GetInterface(DUI_CTR_SCROLLBAR);
			if(pInterface) return false;

			while(pControl != NULL) {
				IListItemUI* pListItem = (IListItemUI*)pControl->GetInterface(DUI_CTR_LISTITEM);
				if(pListItem != NULL ) {
					return true;
				}
				pControl = pControl->GetParent();
			}
		}
		
		return false;
	}

	LRESULT CComboWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if( uMsg == WM_CREATE ) {
			m_pm.SetForceUseSharedRes(true);
			m_pm.Init(m_hWnd);
			// The trick is to add the items to the new container. Their owner gets
			// reassigned by this operation - which is why it is important to reassign
			// the items back to the righfull owner/manager when the window closes.
			m_pLayout = new CVerticalLayoutUI;
			m_pLayout->SetManager(&m_pm, NULL, true);
			LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(_T("VerticalLayout"));
			if( pDefaultAttributes ) {
				m_pLayout->ApplyAttributeList(pDefaultAttributes);
			}
			m_pLayout->SetInset(CDuiRect(2, 1, 2, 1));
			m_pLayout->SetBkColor(0xFFFFFFFF);
			m_pLayout->SetBorderColor(0xFFC6C7D2);
			m_pLayout->SetBorderSize(1);
			m_pLayout->SetAutoDestroy(false);
			m_pLayout->EnableScrollBar();
			m_pLayout->ApplyAttributeList(m_pOwner->GetDropBoxAttributeList());
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
				m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
			}
			CShadowUI *pShadow = m_pOwner->GetManager()->GetShadow();
			pShadow->CopyShadow(m_pm.GetShadow());
			m_pm.GetShadow()->ShowShadow(m_pOwner->IsShowShadow());
			m_pm.AttachDialog(m_pLayout);
			m_pm.AddNotifier(this);
			return 0;
		}
		else if( uMsg == WM_CLOSE ) {
			m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
			RECT rcNull = { 0 };
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) static_cast<CControlUI*>(m_pOwner->GetItemAt(i))->SetPos(rcNull);
			m_pOwner->SetFocus();
		}
		else if( uMsg == WM_LBUTTONDOWN ) {
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			m_bHitItem = IsHitItem(pt);
		}
		else if( uMsg == WM_LBUTTONUP ) {
			POINT pt = { 0 };
			::GetCursorPos(&pt);
			::ScreenToClient(m_pm.GetPaintWindow(), &pt);
			if(m_bHitItem && IsHitItem(pt)) {
				PostMessage(WM_KILLFOCUS);
			}
			m_bHitItem = false;
		}
		else if( uMsg == WM_KEYDOWN ) {
			switch( wParam ) {
			case VK_ESCAPE:
				m_pOwner->SelectItem(m_iOldSel, true);
				EnsureVisible(m_iOldSel);
			case VK_RETURN:
				PostMessage(WM_KILLFOCUS);
				break;
			default:
				TEventUI event;
				event.Type = UIEVENT_KEYDOWN;
				event.chKey = (TCHAR)wParam;
				m_pOwner->DoEvent(event);
				EnsureVisible(m_pOwner->GetCurSel());
				return 0;
			}
		}
		else if( uMsg == WM_MOUSEWHEEL ) {
			int zDelta = (int) (short) HIWORD(wParam);
			TEventUI event = { 0 };
			event.Type = UIEVENT_SCROLLWHEEL;
			event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, 0);
			event.lParam = lParam;
			event.dwTimestamp = ::GetTickCount();
			if(m_pOwner->m_ListInfo.IsScrollSelect()) {
				m_pOwner->DoEvent(event);
				EnsureVisible(m_pOwner->GetCurSel());
				return 0;
			}
			else {
				m_pLayout->DoEvent(event);
				return 0;
			}
		}
		else if( uMsg == WM_KILLFOCUS ) {
			if( m_hWnd != (HWND) wParam ) PostMessage(WM_CLOSE);
		}

		LRESULT lRes = 0;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	void CComboWnd::EnsureVisible(int iIndex)
	{
		if( m_pOwner->GetCurSel() < 0 ) return;
		m_pLayout->FindSelectable(m_pOwner->GetCurSel(), false);
		RECT rcItem = m_pLayout->GetItemAt(iIndex)->GetPos();
		RECT rcList = m_pLayout->GetPos();
		CScrollBarUI* pHorizontalScrollBar = m_pLayout->GetHorizontalScrollBar();
		if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();
		int iPos = m_pLayout->GetScrollPos().cy;
		if( rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom ) return;
		int dx = 0;
		if( rcItem.top < rcList.top ) dx = rcItem.top - rcList.top;
		if( rcItem.bottom > rcList.bottom ) dx = rcItem.bottom - rcList.bottom;
		Scroll(0, dx);
	}

	void CComboWnd::Scroll(int dx, int dy)
	{
		if( dx == 0 && dy == 0 ) return;
		SIZE sz = m_pLayout->GetScrollPos();
		m_pLayout->SetScrollPos(CDuiSize(sz.cx + dx, sz.cy + dy));
	}

#if(_WIN32_WINNT >= 0x0501)
	UINT CComboWnd::GetClassStyle() const
	{
		if(m_pOwner->IsShowShadow()) {
			return __super::GetClassStyle();

		}
		else {
			return __super::GetClassStyle() | CS_DROPSHADOW;
		}
	}
#endif
	////////////////////////////////////////////////////////
	IMPLEMENT_DUICONTROL(CComboUI)

	CComboUI::CComboUI()
		: m_pWindow(NULL)
		, m_iCurSel(-1)
	{
		m_uTextStyle = DT_VCENTER | DT_SINGLELINE;
		m_szDropBox = CDuiSize(0, 150);
		::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
		m_rcTextPadding.left = 5;
		m_rcTextPadding.right = 5;
	}

	LPCTSTR CComboUI::GetClass() const
	{
		return _T("ComboUI");
	}

	LPVOID CComboUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_COMBO) == 0 ) return static_cast<CComboUI*>(this);
		if( _tcsicmp(pstrName, _T("IListOwner")) == 0 ) return static_cast<IListOwnerUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CComboUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP | UIFLAG_SETCURSOR;
	}

	void CComboUI::DoInit()
	{
	}

	UINT CComboUI::GetListType()
	{
		return LT_COMBO;
	}

	TListInfoUI* CComboUI::GetListInfo()
	{
		return &m_ListInfo;
	}

	int CComboUI::GetCurSel() const
	{
		return m_iCurSel;
	}

	bool CComboUI::SelectItem(LPCTSTR pstrText)
	{
		for( int it = 0; it < GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if( !pControl->IsVisible() ) continue;

			if(pControl->GetText() == pstrText)
			{
				SelectItem(it);
				return true;
			}
		}
		return false;
	}

	bool CComboUI::SelectItem(int iIndex, bool bTakeFocus)
	{
		if( iIndex == m_iCurSel ) return true;
		int iOldSel = m_iCurSel;
		if( m_iCurSel >= 0 ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
			if( !pControl ) return false;
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) pListItem->Select(false);
			m_iCurSel = -1;
		}
		if( iIndex < 0 ) return false;
		if( m_items.GetSize() == 0 ) return false;
		if( iIndex >= m_items.GetSize() ) iIndex = m_items.GetSize() - 1;
		CControlUI* pControl = static_cast<CControlUI*>(m_items[iIndex]);
		if( !pControl || !pControl->IsEnabled() ) return false;
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if( pListItem == NULL ) return false;
		m_iCurSel = iIndex;
		if( m_pWindow != NULL || bTakeFocus ) pControl->SetFocus();
		pListItem->Select(true);

		//modify by liqs99
		SetText(pControl->GetText());

		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, m_iCurSel, iOldSel, true);
		Invalidate();


		return true;
	}

	bool CComboUI::SelectMultiItem(int iIndex, bool bTakeFocus)
	{
		return SelectItem(iIndex, bTakeFocus);
	}
	
	bool CComboUI::UnSelectItem(int iIndex, bool bOthers)
	{
		return false;
	}
		
	bool CComboUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		int iOrginIndex = GetItemIndex(pControl);
		if( iOrginIndex == -1 ) return false;
		if( iOrginIndex == iIndex ) return true;

		IListItemUI* pSelectedListItem = NULL;
		if( m_iCurSel >= 0 ) pSelectedListItem = 
			static_cast<IListItemUI*>(GetItemAt(m_iCurSel)->GetInterface(_T("ListItem")));
		if( !CContainerUI::SetItemIndex(pControl, iIndex) ) return false;
		int iMinIndex = min(iOrginIndex, iIndex);
		int iMaxIndex = max(iOrginIndex, iIndex);
		for(int i = iMinIndex; i < iMaxIndex + 1; ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) {
				pListItem->SetIndex(i);
			}
		}
		if( m_iCurSel >= 0 && pSelectedListItem != NULL ) m_iCurSel = pSelectedListItem->GetIndex();
		return true;
	}

	bool CComboUI::Add(CControlUI* pControl)
	{
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if( pListItem != NULL ) 
		{
			pListItem->SetOwner(this);
			pListItem->SetIndex(m_items.GetSize());
			pControl->SetAutoCalcWidth(false);
		}
		return CContainerUI::Add(pControl);
	}

	bool CComboUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if (!CContainerUI::AddAt(pControl, iIndex)) return false;

		// The list items should know about us
		IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
		if( pListItem != NULL ) {
			pListItem->SetOwner(this);
			pListItem->SetIndex(iIndex);
			pControl->SetAutoCalcWidth(false);
		}

		for(int i = iIndex + 1; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) {
				pListItem->SetIndex(i);
			}
		}
		if( m_iCurSel >= iIndex ) m_iCurSel += 1;
		return true;
	}

	bool CComboUI::Remove(CControlUI* pControl, bool bDoNotDestroy )
	{
		int iIndex = GetItemIndex(pControl);
		if (iIndex == -1) return false;

		if (!CContainerUI::RemoveAt(iIndex)) return false;

		for(int i = iIndex; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) {
				pListItem->SetIndex(i);
			}
		}

		if( iIndex == m_iCurSel && m_iCurSel >= 0 ) {
			int iSel = m_iCurSel;
			m_iCurSel = -1;
			SelectItem(FindSelectable(iSel, false));
		}
		else if( iIndex < m_iCurSel ) m_iCurSel -= 1;
		return true;
	}

	bool CComboUI::RemoveAt(int iIndex, bool bDoNotDestroy )
	{
		if (!CContainerUI::RemoveAt(iIndex)) return false;

		for(int i = iIndex; i < GetCount(); ++i) {
			CControlUI* p = GetItemAt(i);
			IListItemUI* pListItem = static_cast<IListItemUI*>(p->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) pListItem->SetIndex(i);
		}

		if( iIndex == m_iCurSel && m_iCurSel >= 0 ) {
			int iSel = m_iCurSel;
			m_iCurSel = -1;
			SelectItem(FindSelectable(iSel, false));
		}
		else if( iIndex < m_iCurSel ) m_iCurSel -= 1;
		return true;
	}

	void CComboUI::RemoveAll()
	{
		m_iCurSel = -1;
		CContainerUI::RemoveAll();
	}

	void CComboUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN )
		{
			if( IsEnabled() ) {
				Activate();
				SetCaptureState(true);
				SetPushedState(true);
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( IsCaptureState() ) {
				SetCaptureState(false);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			return;
		}
		if( event.Type == UIEVENT_KEYDOWN )
		{
			switch( event.chKey ) {
			case VK_F4:
				Activate();
				return;
			case VK_UP:
				SelectItem(FindSelectable(m_iCurSel - 1, false));
				return;
			case VK_DOWN:
				SelectItem(FindSelectable(m_iCurSel + 1, true));
				return;
			case VK_PRIOR:
				SelectItem(FindSelectable(m_iCurSel - 1, false));
				return;
			case VK_NEXT:
				SelectItem(FindSelectable(m_iCurSel + 1, true));
				return;
			case VK_HOME:
				SelectItem(FindSelectable(0, false));
				return;
			case VK_END:
				SelectItem(FindSelectable(GetCount() - 1, true));
				return;
			}
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if(m_ListInfo.IsScrollSelect()) {
				bool bDownward = LOWORD(event.wParam) == SB_LINEDOWN;
				SelectItem(FindSelectable(m_iCurSel + (bDownward ? 1 : -1), bDownward));
			}
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
				if( !IsHotState()  )
					SetHotState(true);
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsHotState() ) {
				SetHotState(false);
				Invalidate();
			}
			return;
		}
		CControlUI::DoEvent(event);
	}

	SIZE CComboUI::EstimateSize(SIZE szAvailable)
	{
		if( IsAutoCalcHeight() ) return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontHeight(-1) + 12);
		return CControlUI::EstimateSize(szAvailable);
	}

	bool CComboUI::Activate()
	{
		if( !CControlUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_PREDROPDOWN);
		if( m_pWindow ) return true;
		m_pWindow = new CComboWnd();
		ASSERT(m_pWindow);
		m_pWindow->Init(this);
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_DROPDOWN);
		Invalidate();
		return true;
	}


	//modify by liqs99
// 	CDuiString CComboUI::GetText() const
// 	{
// 		if( m_iCurSel < 0 ) return _T("");
// 		CControlUI* pControl = static_cast<CControlUI*>(m_items[m_iCurSel]);
// 		return pControl->GetText();
// 	}

	CDuiString CComboUI::GetDropBoxAttributeList()
	{
		return m_sDropBoxAttributes;
	}

	void CComboUI::SetDropBoxAttributeList(LPCTSTR pstrList)
	{
		m_sDropBoxAttributes = pstrList;
	}

	SIZE CComboUI::GetDropBoxSize() const
	{
		return m_szDropBox;
	}

	void CComboUI::SetDropBoxSize(SIZE szDropBox)
	{
		m_szDropBox = szDropBox;
	}

	bool CComboUI::IsShowShadow()
	{
		return m_bShowShadow;
	}

	void CComboUI::SetShowShadow(bool bShow)
	{
		if( m_bShowShadow == bShow ) return;

		m_bShowShadow = bShow;
		Invalidate();
	}

	void CComboUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		if(!::EqualRect(&rc, &m_rcItem)) {
			// 隐藏下拉窗口
			if(m_pWindow && ::IsWindow(m_pWindow->GetHWND())) m_pWindow->Close();
			// 所有元素大小置为0
			RECT rcNull = { 0 };
			for( int i = 0; i < m_items.GetSize(); i++ ) static_cast<CControlUI*>(m_items[i])->SetPos(rcNull);
			// 调整位置
			CControlUI::SetPos(rc, bNeedInvalidate);
		}
	}

	void CComboUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
	}

	void CComboUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if(m_ListInfo.SetAttribute(pstrName, pstrValue)) 
			return;

		if( _tcsicmp(pstrName, _T("showshadow")) == 0 ) SetShowShadow(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("dropbox")) == 0 ) SetDropBoxAttributeList(pstrValue);
		else if( _tcsicmp(pstrName, _T("dropboxsize")) == 0)
		{
			SIZE szDropBoxSize = { 0 };
			LPTSTR pstr = NULL;
			szDropBoxSize.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			szDropBoxSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			SetDropBoxSize(szDropBoxSize);
		}
		else __super::SetAttribute(pstrName, pstrValue);
	}

} // namespace DuiLib
