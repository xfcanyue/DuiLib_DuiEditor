#include "StdAfx.h"

#include "UIMenu.h"

namespace DuiLib {

	//modify by liqs99
	//1, lstrcpy 改为 _tcscpy
	/////////////////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CMenuUI)

	CMenuUI::CMenuUI():
	m_pWindow(NULL), m_nIconWidth(26), m_dwIconBkColor(0xFFE8EDEE)
	{
		m_ListInfo.dwDisabledBkColor = 0x00000000;

		if (GetHeader() != NULL)
			GetHeader()->SetVisible(false);
	}

	CMenuUI::~CMenuUI()
	{

	}

	LPCTSTR CMenuUI::GetClass() const
	{
		return _T("MenuUI");
	}

	LPVOID CMenuUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("Menu")) == 0 ) return static_cast<CMenuUI*>(this);
		return CListUI::GetInterface(pstrName);
	}

	UINT CMenuUI::GetListType()
	{
		return LT_MENU;
	}

	void CMenuUI::DoEvent(TEventUI& event)
	{
		return __super::DoEvent(event);
	}

	bool CMenuUI::Add(CControlUI* pControl)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		for (int i = 0; i < pMenuItem->GetCount(); ++i)
		{
			if (pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{
				(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
			}
		}
		return CListUI::Add(pControl);
	}

	bool CMenuUI::AddAt(CControlUI* pControl, int iIndex)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		for (int i = 0; i < pMenuItem->GetCount(); ++i)
		{
			if (pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{
				(static_cast<CMenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
			}
		}
		return CListUI::AddAt(pControl, iIndex);
	}

	int CMenuUI::GetItemIndex(CControlUI* pControl) const
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return -1;

		return __super::GetItemIndex(pControl);
	}

	bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex)
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		return __super::SetItemIndex(pControl, iIndex);
	}

	bool CMenuUI::Remove(CControlUI* pControl, bool bDoNotDestroy )
	{
		CMenuElementUI* pMenuItem = static_cast<CMenuElementUI*>(pControl->GetInterface(_T("MenuElement")));
		if (pMenuItem == NULL)
			return false;

		return __super::Remove(pControl);
	}

	SIZE CMenuUI::EstimateSize(SIZE szAvailable)
	{
		int cxFixed = 0;
		int cyFixed = 0;
		for( int it = 0; it < GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if( !pControl->IsVisible() ) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cyFixed += sz.cy;
			if( cxFixed < sz.cx )
				cxFixed = sz.cx;
		}

		for (int it = 0; it < GetCount(); it++) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if (!pControl->IsVisible()) continue;

			pControl->SetFixedWidth(MulDiv(cxFixed, 100, GetManager()->GetDPIObj()->GetScale()));
		}

		return CDuiSize(cxFixed, cyFixed);
	}

	void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("expandicon")) == 0){
			SetExpandIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("checkicon")) == 0){
			SetCheckIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconwidth")) == 0){
			SetIconWidth(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("iconbkcolor")) == 0){
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetIconBkColor(clrColor);
		}
		else
			CListUI::SetAttribute(pstrName, pstrValue);
	}

	void CMenuUI::SetExpandIcon(LPCTSTR strIcon) 
	{ 
		m_strExpandIcon = strIcon; 
	}
	CDuiString CMenuUI::GetExpandIcon() 
	{ 
		return m_strExpandIcon; 
	}
	void CMenuUI::SetCheckIcon(LPCTSTR strIcon) 
	{ 
		m_strCheckIcon = strIcon;
	}
	CDuiString CMenuUI::GetCheckIcon() 
	{ 
		return m_strCheckIcon; 
	}
	void CMenuUI::SetIconWidth(int nWidth)
	{
		m_nIconWidth = nWidth;
	}
	int CMenuUI::GetIconWidth()
	{
		return m_nIconWidth;
	}
	void CMenuUI::SetIconBkColor(DWORD dwColor)
	{
		m_dwIconBkColor = dwColor;
	}
	DWORD CMenuUI::GetIconBkColor()
	{
		return m_dwIconBkColor;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	//

	CMenuWnd::CMenuWnd():
	m_pOwner(NULL),
		m_pLayout(),
		m_xml(_T("")),
		isClosing(false),
		m_bAutoDestroy(false)
	{
		m_dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;
	}

	CMenuWnd::~CMenuWnd()
	{
		
	}

	void CMenuWnd::Close(UINT nRet)
	{
		ASSERT(::IsWindow(m_hWnd));
		if (!::IsWindow(m_hWnd)) return;
		PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
		isClosing = true;
	}


	BOOL CMenuWnd::Receive(ContextMenuParam param)
	{
		switch (param.wParam)
		{
		case 1:
			Close();
			break;
		case 2:
			{
				HWND hParent = GetParent(m_hWnd);
				while (hParent != NULL)
				{
					if (hParent == param.hWnd)
					{
						Close();
						break;
					}
					hParent = GetParent(hParent);
				}
			}
			break;
		default:
			break;
		}

		return TRUE;
	}

	CMenuWnd* CMenuWnd::CreateMenu(CMenuElementUI* pOwner, STRINGorID xml, POINT point, CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo /*= NULL*/, DWORD dwAlignment /*= eMenuAlignment_Left | eMenuAlignment_Top*/)
	{
		CMenuWnd* pMenu = new CMenuWnd;
		pMenu->Init(pOwner, xml, point, pMainPaintManager, pMenuCheckInfo, dwAlignment);
		return pMenu;
	}

	void CMenuWnd::DestroyMenu()
	{
		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			for(int i = 0; i < mCheckInfos->GetSize(); i++) {
				MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(mCheckInfos->GetAt(i));
				if(pItemInfo != NULL) {
					delete pItemInfo;
					pItemInfo = NULL;
				}
			}
			mCheckInfos->Resize(0);
			CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(NULL);
		}
	}
	
	MenuItemInfo* CMenuWnd::SetMenuItemInfo(LPCTSTR pstrName, bool bChecked)
	{
		if(pstrName == NULL || lstrlen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo == NULL) {
				pItemInfo = new MenuItemInfo;
				_tcscpy(pItemInfo->szName, pstrName);
				pItemInfo->bChecked = bChecked;
				mCheckInfos->Insert(pstrName, pItemInfo);
			}
			else {
				pItemInfo->bChecked = bChecked;
			}

			return pItemInfo;
		}
		return NULL;
	}

	void CMenuWnd::Init(CMenuElementUI* pOwner, STRINGorID xml, POINT point,
		CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo/* = NULL*/,
		DWORD dwAlignment/* = eMenuAlignment_Left | eMenuAlignment_Top*/)
	{

		m_BasedPoint = point;
		m_pOwner = pOwner;
		m_pLayout = NULL;
		m_xml = xml;
		m_dwAlignment = dwAlignment;

		// 如果是一级菜单的创建
		if (pOwner == NULL)
		{
			ASSERT(pMainPaintManager != NULL);
			CMenuWnd::GetGlobalContextMenuObserver().SetManger(pMainPaintManager);
			if (pMenuCheckInfo != NULL)
				CMenuWnd::GetGlobalContextMenuObserver().SetMenuCheckInfo(pMenuCheckInfo);
		}

		CMenuWnd::GetGlobalContextMenuObserver().AddReceiver(this);

		Create((m_pOwner == NULL) ? pMainPaintManager->GetPaintWindow() : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
		
		// HACK: Don't deselect the parent's caption
		HWND hWndParent = m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	LPCTSTR CMenuWnd::GetWindowClassName() const
	{
		return _T("DuiMenuWnd");
	}


	void CMenuWnd::Notify(TNotifyUI& msg)
	{
		if( CMenuWnd::GetGlobalContextMenuObserver().GetManager() != NULL) 
		{
			if( msg.sType == _T("click") || msg.sType == _T("valuechanged") ) 
			{
				CMenuWnd::GetGlobalContextMenuObserver().GetManager()->SendNotify(msg, false);
			}
		}

	}

	CControlUI* CMenuWnd::CreateControl( LPCTSTR pstrClassName )
	{
		if (_tcsicmp(pstrClassName, _T("Menu")) == 0)
		{
			return new CMenuUI();
		}
		else if (_tcsicmp(pstrClassName, _T("MenuElement")) == 0)
		{
			return new CMenuElementUI();
		}
		return NULL;
	}


	void CMenuWnd::OnFinalMessage(HWND hWnd)
	{
		RemoveObserver();
		if( m_pOwner != NULL ) {
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
				if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement"))) != NULL ) {
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
				}
			}
			m_pOwner->m_pWindow = NULL;
			m_pOwner->m_uButtonState &= ~ UISTATE_PUSHED;
			m_pOwner->Invalidate();

			// 内部创建的内部删除
			delete this;
		}
		else
		{
			if(m_bAutoDestroy)
			{
				DestroyMenu();
				// 外部创建的也删除
				delete this;
			}
		}
	}

	LRESULT CMenuWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bool bShowShadow = false;
		if( m_pOwner != NULL) {
			LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
			RECT rcClient;
			::GetClientRect(*this, &rcClient);
			::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
				rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

			m_pm.Init(m_hWnd);
			m_pm.GetDPIObj()->SetScale(m_pOwner->GetManager()->GetDPIObj()->GetDPI());
			// The trick is to add the items to the new container. Their owner gets
			// reassigned by this operation - which is why it is important to reassign
			// the items back to the righfull owner/manager when the window closes.
			m_pLayout = new CMenuUI();
			m_pm.SetForceUseSharedRes(true);
			m_pLayout->SetManager(&m_pm, NULL, true);
			LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(_T("Menu"));
			if( pDefaultAttributes ) {
				m_pLayout->ApplyAttributeList(pDefaultAttributes);
			}
			m_pLayout->GetList()->SetAutoDestroy(false);

			for( int i = 0; i < m_pOwner->GetCount(); i++ ) 
			{
				if(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
				{
					CMenuElementUI *pElementUI = static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i));
					pElementUI->SetOwner(m_pLayout);
					m_pLayout->Add(static_cast<CControlUI*>(pElementUI));
					pElementUI->OnUpdateCommandUI();
				}
			}

			CShadowUI *pShadow = m_pOwner->GetManager()->GetShadow();
			pShadow->CopyShadow(m_pm.GetShadow());
			bShowShadow = m_pm.GetShadow()->IsShowShadow();
			m_pm.GetShadow()->ShowShadow(false);
			m_pm.SetLayered(m_pOwner->GetManager()->IsLayered());
			m_pm.AttachDialog(m_pLayout);
			m_pm.AddNotifier(this);

			ResizeSubMenu();
		}
		else {
			m_pm.Init(m_hWnd);
			m_pm.GetDPIObj()->SetScale(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetDPIObj()->GetDPI());
			CDialogBuilder builder;

			CControlUI* pRoot = builder.Create(m_xml,UINT(0), this, &m_pm);
			bShowShadow = m_pm.GetShadow()->IsShowShadow();
			m_pm.GetShadow()->ShowShadow(false);
			m_pm.AttachDialog(pRoot);
			m_pm.AddNotifier(this);

			if(GetMenuUI())
			{
				for (int i=0; i<GetMenuUI()->GetCount(); i++)
				{
					CMenuElementUI *pElementUI = static_cast<CMenuElementUI*>(GetMenuUI()->GetItemAt(i));
					pElementUI->OnUpdateCommandUI();
				}
			}

			ResizeMenu();
		}
		GetMenuUI()->m_pWindow = this;
		m_pm.GetShadow()->ShowShadow(bShowShadow);
		m_pm.GetShadow()->Create(&m_pm);
		return 0;
	}

	CMenuUI* CMenuWnd::GetMenuUI()
	{
		return dynamic_cast<CMenuUI*>(m_pm.GetRoot());
	}

	void CMenuWnd::ResizeMenu()
	{
		CControlUI* pRoot = m_pm.GetRoot();

#if defined(WIN32) && !defined(UNDER_CE)
		MONITORINFO oMonitor = {}; 
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
#else
		CDuiRect rcWork;
		GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
		SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
		szAvailable = pRoot->EstimateSize(szAvailable);

		//szAvailable.cx += 20; //add by liqs99 菜单右边显示不全

		m_pm.SetInitSize(szAvailable.cx, szAvailable.cy);

		//必须是Menu标签作为xml的根节点
		CMenuUI *pMenuRoot = static_cast<CMenuUI*>(pRoot);
		ASSERT(pMenuRoot);

		SIZE szInit = m_pm.GetInitSize();
		CDuiRect rc;
		CDuiPoint point = m_BasedPoint;
		rc.left = point.x;
		rc.top = point.y;
		rc.right = rc.left + szInit.cx;
		rc.bottom = rc.top + szInit.cy;

		int nWidth = rc.GetWidth();
		int nHeight = rc.GetHeight();

		if (m_dwAlignment & eMenuAlignment_Right)
		{
			rc.right = point.x;
			rc.left = rc.right - nWidth;
		}

		if (m_dwAlignment & eMenuAlignment_Bottom)
		{
			rc.bottom = point.y;
			rc.top = rc.bottom - nHeight;
		}

		//////////////////////////////////////////////////////////////////////////add by liqs99
		//调整在屏幕中的位置，主要是触碰边界
		//int screenX = GetSystemMetrics(SM_CXSCREEN);//获取整个屏幕右下角X坐标   
		//int screenY = GetSystemMetrics(SM_CYSCREEN);//屏幕Y坐标 
		if(rc.right > rcWork.GetWidth())
		{
			rc.left -= rc.right - rcWork.GetWidth();
			rc.right -= rc.right - rcWork.GetWidth();
		}
		if(rc.bottom > rcWork.GetHeight())
		{
			rc.top = point.y - rc.GetHeight();
			rc.bottom = point.y;
		}
		//////////////////////////////////////////////////////////////////////////

		SetForegroundWindow(m_hWnd);
		MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
		SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight() + pMenuRoot->GetInset().bottom + pMenuRoot->GetInset().top, SWP_SHOWWINDOW);
	}

	void CMenuWnd::ResizeSubMenu()
	{
		// Position the popup window in absolute space
		RECT rcOwner = m_pOwner->GetPos();
		RECT rc = rcOwner;

		int cxFixed = 0;
		int cyFixed = 0;

#if defined(WIN32) && !defined(UNDER_CE)
		MONITORINFO oMonitor = {}; 
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
#else
		CDuiRect rcWork;
		GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
		SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

		for( int it = 0; it < m_pOwner->GetCount(); it++ ) {
			if(m_pOwner->GetItemAt(it)->GetInterface(_T("MenuElement")) != NULL ){
				CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->GetItemAt(it));
				SIZE sz = pControl->EstimateSize(szAvailable);
				cyFixed += sz.cy;
				if( cxFixed < sz.cx ) cxFixed = sz.cx;
			}
		}

		RECT rcWindow;
		GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

		rc.top = rcOwner.top;
		rc.bottom = rc.top + cyFixed;
		::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
		rc.left = rcWindow.right;
		rc.right = rc.left + cxFixed;
		rc.right += 2;

		bool bReachBottom = false;
		bool bReachRight = false;
		LONG chRightAlgin = 0;
		LONG chBottomAlgin = 0;

		RECT rcPreWindow = {0};
		MenuObserverImpl::Iterator iterator(CMenuWnd::GetGlobalContextMenuObserver());
		MenuMenuReceiverImplBase* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if( pContextMenu != NULL ) {
				GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

				bReachRight = rcPreWindow.left >= rcWindow.right;
				bReachBottom = rcPreWindow.top >= rcWindow.bottom;
				if( pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow() ||  bReachBottom || bReachRight )
					break;
			}
			pReceiver = iterator.next();
		}

		if (bReachBottom)
		{
			rc.bottom = rcWindow.top;
			rc.top = rc.bottom - cyFixed;
		}

		if (bReachRight)
		{
			rc.right = rcWindow.left;
			rc.left = rc.right - cxFixed;
		}

		if( rc.bottom > rcWork.bottom )
		{
			rc.bottom = rc.top;
			rc.top = rc.bottom - cyFixed;
		}

		if (rc.right > rcWork.right)
		{
			rc.right = rcWindow.left;
			rc.left = rc.right - cxFixed;
		}

		if( rc.top < rcWork.top )
		{
			rc.top = rcOwner.top;
			rc.bottom = rc.top + cyFixed;
		}

		if (rc.left < rcWork.left)
		{
			rc.left = rcWindow.right;
			rc.right = rc.left + cxFixed;
		}

		MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + m_pLayout->GetInset().top + m_pLayout->GetInset().bottom, FALSE);
	}

	void CMenuWnd::setDPI(int DPI) {
		m_pm.SetDPI(DPI);
	}


	LRESULT CMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HWND hFocusWnd = (HWND)wParam;

		BOOL bInMenuWindowList = FALSE;
		ContextMenuParam param;
		param.hWnd = GetHWND();

		MenuObserverImpl::Iterator iterator(CMenuWnd::GetGlobalContextMenuObserver());
		MenuMenuReceiverImplBase* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWnd* pContextMenu = dynamic_cast<CMenuWnd*>(pReceiver);
			if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
				bInMenuWindowList = TRUE;
				break;
			}
			pReceiver = iterator.next();
		}

		if( !bInMenuWindowList ) {
			param.wParam = 1;
			CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
			return 0;
		}
		return 0;
	}
	LRESULT CMenuWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		SIZE szRoundCorner = m_pm.GetRoundCorner();
		if( !::IsIconic(*this) ) {
			CDuiRect rcWnd;
			::GetWindowRect(*this, &rcWnd);
			rcWnd.Offset(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
			::SetWindowRgn(*this, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
		bHandled = FALSE;
		return 0;
	}

	LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg )
		{
		case WM_CREATE:       
			lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_KILLFOCUS:       
			lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE || wParam == VK_LEFT)
				Close();
			break;
		case WM_SIZE:
			lRes = OnSize(uMsg, wParam, lParam, bHandled);
			break;
		case WM_CLOSE:
			if( m_pOwner != NULL )
			{
				m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
				m_pOwner->SetPos(m_pOwner->GetPos());
				m_pOwner->SetFocus();
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return 0L;
			break;
		default:
			bHandled = FALSE;
			break;
		}

		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	IMPLEMENT_DUICONTROL(CMenuElementUI)

	CMenuElementUI::CMenuElementUI():
	m_pWindow(NULL),
		m_bDrawLine(false),
		m_dwLineColor(DEFAULT_LINE_COLOR),
		m_bCheckItem(false), m_bCheck(false),
		m_bShowExplandIcon(false)
	{
		m_cxyFixed.cy = ITEM_DEFAULT_HEIGHT;
		m_cxyFixed.cx = ITEM_DEFAULT_WIDTH;
		m_szIconSize.cy = ITEM_DEFAULT_ICON_SIZE;
		m_szIconSize.cx = ITEM_DEFAULT_ICON_SIZE;

		m_rcLinePadding.top = m_rcLinePadding.bottom = 0;
		m_rcLinePadding.left = DEFAULT_LINE_LEFT_INSET;
		m_rcLinePadding.right = DEFAULT_LINE_RIGHT_INSET;
	}

	CMenuElementUI::~CMenuElementUI()
	{}

	LPCTSTR CMenuElementUI::GetClass() const
	{
		return _T("MenuElementUI");
	}

	LPVOID CMenuElementUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, _T("MenuElement")) == 0 ) return static_cast<CMenuElementUI*>(this);    
		return CListContainerElementUI::GetInterface(pstrName);
	}

	bool CMenuElementUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy);
		RECT m_rcLinePadding = CMenuElementUI::m_rcLinePadding;
		GetManager()->GetDPIObj()->Scale(&m_rcLinePadding);

		RECT rcTemp = { 0 };
		if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;

		if(m_bDrawLine)
		{
			RECT rcLine = { m_rcItem.left +  m_rcLinePadding.left, m_rcItem.top + m_cxyFixed.cy/2, m_rcItem.right - m_rcLinePadding.right, m_rcItem.top + m_cxyFixed.cy/2 };
			CRenderEngine::DrawLine(hDC, rcLine, 1, m_dwLineColor);
		}
		else
		{
			//CMenuElementUI::DrawItemBk(hDC, m_rcItem);
			//DrawItemText(hDC, m_rcItem);
			//DrawItemIcon(hDC, m_rcItem);
			//DrawItemExpland(hDC, m_rcItem);
			//for (int i = 0; i < GetCount(); ++i)
			//{
			//	if (GetItemAt(i)->GetInterface(_T("MenuElement")) == NULL) {
			//		GetItemAt(i)->DoPaint(hDC, rcPaint);
			//	}
			//}

			CRenderClip clip;
			CRenderClip::GenerateClip(hDC, rcTemp, clip);
			CMenuElementUI::DrawItemBk(hDC, m_rcItem);
			DrawItemText(hDC, m_rcItem);
			DrawItemBorder(hDC, rcTemp);
			DrawItemIcon(hDC, m_rcItem);
			DrawItemExpland(hDC, m_rcItem);

			if( m_items.GetSize() > 0 ) {
				RECT rc = m_rcItem;
				RECT rcInset = GetInset();
				rc.left += rcInset.left;
				rc.top += rcInset.top;
				rc.right -= rcInset.right;
				rc.bottom -= rcInset.bottom;
				if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
				if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();

				if( !::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
					for( int it = 0; it < m_items.GetSize(); it++ ) {
						CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
						if( pControl == pStopControl ) return false;
						if( !pControl->IsVisible() ) continue;
						if( pControl->GetInterface(_T("MenuElement")) != NULL ) continue;
						if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
						if( pControl->IsFloat() ) {
							if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
							if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
						}
					}
				}
				else {
					CRenderClip childClip;
					CRenderClip::GenerateClip(hDC, rcTemp, childClip);
					for( int it = 0; it < m_items.GetSize(); it++ ) {
						CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
						if( pControl == pStopControl ) return false;
						if( !pControl->IsVisible() ) continue;
						if( pControl->GetInterface(_T("MenuElement")) != NULL ) continue;
						if( !::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()) ) continue;
						if( pControl->IsFloat() ) {
							if( !::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos()) ) continue;
							CRenderClip::UseOldClipBegin(hDC, childClip);
							if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
							CRenderClip::UseOldClipEnd(hDC, childClip);
						}
						else {
							if( !::IntersectRect(&rcTemp, &rc, &pControl->GetPos()) ) continue;
							if( !pControl->Paint(hDC, rcPaint, pStopControl) ) return false;
						}
					}
				}
			}

		}

		if( m_pVerticalScrollBar != NULL ) {
			if( m_pVerticalScrollBar == pStopControl ) return false;
			if (m_pVerticalScrollBar->IsVisible()) {
				if( ::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos()) ) {
					if( !m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
				}
			}
		}

		if( m_pHorizontalScrollBar != NULL ) {
			if( m_pHorizontalScrollBar == pStopControl ) return false;
			if (m_pHorizontalScrollBar->IsVisible()) {
				if( ::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos()) ) {
					if( !m_pHorizontalScrollBar->Paint(hDC, rcPaint, pStopControl) ) return false;
				}
			}
		}
		return true;
	}

	void CMenuElementUI::DrawItemIcon(HDC hDC, const RECT& rcItem)
	{
		CDuiString strIcon = m_strIcon;
		if(GetCheckItem())	//如果是复选框
		{
			if(GetChecked())
			{
				//如果当前没有设置图标, 使用默认的
				if(m_strIcon.IsEmpty() && GetMenuUI()) 
				{
					strIcon = GetMenuUI()->GetCheckIcon();
				}
			}
			else
			{
				strIcon = m_strUnCheckIcon;
			}
		}
		else
		{
			strIcon = m_strIcon;
		}

		if(strIcon.IsEmpty()) return;

		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy);

		SIZE m_szIconSize = CMenuElementUI::m_szIconSize;
		m_szIconSize.cx = GetManager()->GetDPIObj()->Scale(m_szIconSize.cx);
		m_szIconSize.cy = GetManager()->GetDPIObj()->Scale(m_szIconSize.cy);

		int nWidth = 0;
		if(GetMenuUI())
			nWidth = GetManager()->GetDPIObj()->Scale(GetMenuUI()->GetIconWidth());
		int padding = (nWidth - m_szIconSize.cx) / 2;
		RECT rcDest =
		{
			padding,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2,
			padding + m_szIconSize.cx,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2 + m_szIconSize.cy
		};
		GetManager()->GetDPIObj()->ScaleBack(&rcDest);
		/*
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		RECT rcTextPadding = pInfo->rcTextPadding;
		GetManager()->GetDPIObj()->Scale(&rcTextPadding);

		int padding = (rcTextPadding.left - m_szIconSize.cx) / 2;
		RECT rcDest =
		{
			padding,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2,
			padding + m_szIconSize.cx,
			(m_cxyFixed.cy - m_szIconSize.cy) / 2 + m_szIconSize.cy
		};
		GetManager()->GetDPIObj()->ScaleBack(&rcDest);
		*/

		CDuiString pStrImage;
		pStrImage.Format(_T("dest='%d,%d,%d,%d'"), rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
		//if(IsEnabled())
			DrawImage(hDC, strIcon, pStrImage);	
		//else
		//	DrawDisableItemIcon(hDC, strIcon, pStrImage);
	}

	void CMenuElementUI::DrawItemExpland(HDC hDC, const RECT& rcItem)
	{
		if (m_bShowExplandIcon && IsEnabled())
		{
			CDuiString strExplandIcon;
			strExplandIcon = GetManager()->GetDefaultAttributeList(_T("ExplandIcon"));
			if (strExplandIcon.IsEmpty()) 
			{
				if(GetMenuUI())
					strExplandIcon = GetMenuUI()->GetExpandIcon();
				if (strExplandIcon.IsEmpty()) 
					return;
			}
			SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
			m_cxyFixed.cx = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx);
			m_cxyFixed.cy = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy);
			int padding = GetManager()->GetDPIObj()->Scale(ITEM_DEFAULT_EXPLAND_ICON_WIDTH) / 3;
			const TDrawInfo* pDrawInfo = GetManager()->GetDrawInfo((LPCTSTR)strExplandIcon, NULL);
			const TImageInfo *pImageInfo = GetManager()->GetImageEx(pDrawInfo->sImageName, NULL, 0);
			if (!pImageInfo) {
				return;
			}
			RECT rcDest =
			{
				m_cxyFixed.cx - pImageInfo->nX - padding,
				(m_cxyFixed.cy - pImageInfo->nY) / 2,
				m_cxyFixed.cx - pImageInfo->nX - padding + pImageInfo->nX,
				(m_cxyFixed.cy - pImageInfo->nY) / 2 + pImageInfo->nY
			};
			GetManager()->GetDPIObj()->ScaleBack(&rcDest);
			CDuiString pStrImage;
			pStrImage.Format(_T("dest='%d,%d,%d,%d'"), rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
			DrawImage(hDC, strExplandIcon, pStrImage);
		}
	}

	void CMenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
	{
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		if( m_pOwner == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		DWORD iTextColor = pInfo->dwTextColor;
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iTextColor = pInfo->dwHotTextColor;
		}
		if( IsSelected() ) {
			iTextColor = pInfo->dwSelectedTextColor;
		}
		if( !IsEnabled() ) {
			iTextColor = pInfo->dwDisabledTextColor;
		}
		int nLinks = 0;
		RECT rcText = rcItem;
		RECT rcTextPadding = pInfo->rcTextPadding;
		GetManager()->GetDPIObj()->Scale(&rcTextPadding);
		rcText.left += rcTextPadding.left;
		rcText.right -= rcTextPadding.right;
		rcText.top += rcTextPadding.top;
		rcText.bottom -= rcTextPadding.bottom;

		if( pInfo->bShowHtml )
			CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, sText, iTextColor, \
			NULL, NULL, nLinks, pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
		else
			CRenderEngine::DrawText(hDC, m_pManager, rcText, sText, iTextColor, \
			pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
	}

	void CMenuElementUI::DrawItemBorder(HDC hDC, const RECT& rcItem)
	{
		int nBorderSize;
		SIZE cxyBorderRound;
		RECT rcBorderSize;
		if (m_pManager) {
			nBorderSize = GetManager()->GetDPIObj()->Scale(m_nBorderSize);
			cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_cxyBorderRound);
			rcBorderSize = GetManager()->GetDPIObj()->Scale(m_rcBorderSize);
		}
		else {
			nBorderSize = m_nBorderSize;
			cxyBorderRound = m_cxyBorderRound;
			rcBorderSize = m_rcBorderSize;
		}

		if(m_dwBorderColor != 0 || m_dwFocusBorderColor != 0 || m_dwHotBorderColor != 0) {
			//画圆角边框
			if(nBorderSize > 0 && ( cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 )) {
				if (IsFocused() && m_dwFocusBorderColor != 0)
					CRenderEngine::DrawRoundRect(hDC, rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);	
				//////////////////////////////////////////////////////////////////////////
				//add by liqs99
				else if (IsHot() && m_dwHotBorderColor != 0)
					CRenderEngine::DrawRoundRect(hDC, rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
				//////////////////////////////////////////////////////////////////////////
				else
					CRenderEngine::DrawRoundRect(hDC, rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
			}
			else {
				if (IsFocused() && m_dwFocusBorderColor != 0 && nBorderSize > 0) { 
					CRenderEngine::DrawRect(hDC, rcItem, nBorderSize, GetAdjustColor(m_dwFocusBorderColor), m_nBorderStyle);
				}
				//////////////////////////////////////////////////////////////////////////
				//add by liqs99
				else if (IsHot() && m_dwHotBorderColor != 0 && nBorderSize > 0) { 
					CRenderEngine::DrawRect(hDC, rcItem, nBorderSize, GetAdjustColor(m_dwHotBorderColor), m_nBorderStyle);
				}
				//////////////////////////////////////////////////////////////////////////
				else if(rcBorderSize.left > 0 || rcBorderSize.top > 0 || rcBorderSize.right > 0 || rcBorderSize.bottom > 0) {
					RECT rcBorder;

					if(rcBorderSize.left > 0){
						rcBorder		= rcItem;
						rcBorder.right	= rcBorder.left;
						CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.left,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.top > 0){
						rcBorder		= rcItem;
						rcBorder.bottom	= rcBorder.top;
						CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.top,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.right > 0){
						rcBorder		= rcItem;
						rcBorder.right -= 1;
						rcBorder.left	= rcBorder.right;
						CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.right,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
					if(rcBorderSize.bottom > 0){
						rcBorder		= rcItem;
						rcBorder.bottom -= 1;
						rcBorder.top	= rcBorder.bottom;
						CRenderEngine::DrawLine(hDC,rcBorder,rcBorderSize.bottom,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
					}
				}
				else if(nBorderSize > 0) {
					CRenderEngine::DrawRect(hDC, rcItem, nBorderSize, GetAdjustColor(m_dwBorderColor), m_nBorderStyle);
				}
			}
		}
	}

	SIZE CMenuElementUI::EstimateSize(SIZE szAvailable)
	{
		SIZE m_cxyFixed = CMenuElementUI::m_cxyFixed;
		m_cxyFixed.cx = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cx);
		m_cxyFixed.cy = GetManager()->GetDPIObj()->Scale(m_cxyFixed.cy);
		SIZE cXY = {0};
		for( int it = 0; it < GetCount(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
			if( !pControl->IsVisible() ) continue;
			SIZE sz = pControl->EstimateSize(szAvailable);
			cXY.cy += sz.cy;
			if( cXY.cx < sz.cx )
				cXY.cx = sz.cx;
		}

		if(m_pOwner == NULL) //不判断下面就崩了，暂时先这样啊
		{
			return m_cxyFixed;
		}

		if(cXY.cy == 0) {
			TListInfoUI* pInfo = m_pOwner->GetListInfo();

			DWORD iTextColor = pInfo->dwTextColor;
			if( (m_uButtonState & UISTATE_HOT) != 0 ) {
				iTextColor = pInfo->dwHotTextColor;
			}
			if( IsSelected() ) {
				iTextColor = pInfo->dwSelectedTextColor;
			}
			if( !IsEnabled() ) {
				iTextColor = pInfo->dwDisabledTextColor;
			}
			CDuiString sText = GetText();

			RECT rcText = { 0, 0, MAX(szAvailable.cx, m_cxyFixed.cx), 9999 };
			RECT rcTextPadding = pInfo->rcTextPadding;
			GetManager()->GetDPIObj()->Scale(&rcTextPadding);
			rcText.left += rcTextPadding.left;
			rcText.right -= rcTextPadding.right;
			if( pInfo->bShowHtml ) {   
				int nLinks = 0;
				CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, iTextColor, NULL, NULL, nLinks, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
			}
			else {
				CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
			}
			cXY.cx = rcText.right - rcText.left + rcTextPadding.left + rcTextPadding.right ;
			cXY.cy = rcText.bottom - rcText.top + rcTextPadding.top + rcTextPadding.bottom;
		}

		if( m_cxyFixed.cy != 0 ) cXY.cy = m_cxyFixed.cy;
		if ( cXY.cx < m_cxyFixed.cx )
			cXY.cx =  m_cxyFixed.cx;

		CMenuElementUI::m_cxyFixed.cy = MulDiv(cXY.cy, 100, GetManager()->GetDPIObj()->GetScale());
		CMenuElementUI::m_cxyFixed.cx = MulDiv(cXY.cx, 100, GetManager()->GetDPIObj()->GetScale());
		return cXY;
	}

	void CMenuElementUI::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			CListContainerElementUI::DoEvent(event);
			if( m_pWindow ) return;
			SetHot(true);
			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i )
			{
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
				{
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu && IsEnabled())
			{
				m_pOwner->SelectItem(GetIndex(), true);
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 2;
				CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				m_pOwner->SelectItem(GetIndex(), true);
			}
			return;
		}


		if (event.Type == UIEVENT_MOUSELEAVE) {
			SetHot(false);
			bool hasSubMenu = false;
			for (int i = 0; i < GetCount(); ++i)
			{
				if (GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
				{
					
					hasSubMenu = true;
				}
			}

			if (!hasSubMenu) {
				m_pOwner->SelectItem(-1, true);
			}
		}

		if( event.Type == UIEVENT_BUTTONUP )
		{
			if( IsEnabled() ){
				CListContainerElementUI::DoEvent(event);

				if( m_pWindow ) return;

				bool hasSubMenu = false;
				for( int i = 0; i < GetCount(); ++i ) {
					if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL ) {
						(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
						(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

						hasSubMenu = true;
					}
				}
				if( hasSubMenu )
				{
					CreateMenuWnd();
				}
				else
				{
					SetChecked(!GetChecked());

					
					bool isClosing = false;
					CMenuUI* menuUI=static_cast<CMenuUI*>(GetManager()->GetRoot());
					isClosing = (menuUI->m_pWindow->isClosing);
					if (IsWindow(GetManager()->GetPaintWindow()) && !isClosing) {
						if (CMenuWnd::GetGlobalContextMenuObserver().GetManager() != NULL)
						{

							MenuCmd* pMenuCmd = new MenuCmd();
							_tcscpy(pMenuCmd->szName, GetName().GetData());
							_tcscpy(pMenuCmd->szUserData, GetUserData().GetData());
							_tcscpy(pMenuCmd->szText, GetText().GetData());
							pMenuCmd->bChecked = GetChecked();
							if (!PostMessage(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), UIMSG_MENUCLICK, (WPARAM)pMenuCmd, (LPARAM)this))
							{
								delete pMenuCmd;
								pMenuCmd = NULL;
							}
						}
					}
					ContextMenuParam param;
					param.hWnd = m_pManager->GetPaintWindow();
					param.wParam = 1;
					CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				}
			}

			return;
		}

		if ( event.Type == UIEVENT_KEYDOWN && event.chKey == VK_RIGHT )
		{
			if( m_pWindow ) return;
			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i )
			{
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
				{
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);
					hasSubMenu = true;
				}
			}
			if( hasSubMenu )
			{
				m_pOwner->SelectItem(GetIndex(), true);
				CreateMenuWnd();
			}
			else
			{
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 2;
				CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
				m_pOwner->SelectItem(GetIndex(), true);
			}

			return;
		}

		CListContainerElementUI::DoEvent(event);
	}

	CMenuWnd* CMenuElementUI::GetMenuWnd()
	{
		return m_pWindow;
	}

	CMenuUI* CMenuElementUI::GetMenuUI()
	{
		//return dynamic_cast<CMenuUI*>(GetManager()->GetRoot());
		return dynamic_cast<CMenuUI*>(GetOwner());
	}

	void CMenuElementUI::CreateMenuWnd()
	{
		if( m_pWindow ) return;

		m_pWindow = new CMenuWnd();
		ASSERT(m_pWindow);

		ContextMenuParam param;
		param.hWnd = m_pManager->GetPaintWindow();
		param.wParam = 2;
		CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);

		m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), CDuiPoint(), NULL);
	}

	void CMenuElementUI::SetLineType()
	{
		m_bDrawLine = true;
		if (m_cxyFixed.cy == 0 || m_cxyFixed.cy == ITEM_DEFAULT_HEIGHT)
			SetFixedHeight(DEFAULT_LINE_HEIGHT);

		SetMouseChildEnabled(false);
		SetMouseEnabled(false);
		SetEnabled(false);
	}

	void CMenuElementUI::SetLineColor(DWORD color)
	{
		m_dwLineColor = color;
	}

	DWORD CMenuElementUI::GetLineColor() const
	{
		return m_dwLineColor;
	}
	void CMenuElementUI::SetLinePadding(RECT rcInset)
	{
		m_rcLinePadding = rcInset;
	}

	RECT CMenuElementUI::GetLinePadding() const
	{
		return m_rcLinePadding;
	}

	void CMenuElementUI::SetIcon(LPCTSTR strIcon)
	{
		if ( strIcon != _T("") )
			m_strIcon = strIcon;
	}
	void CMenuElementUI::SetUnCheckIcon(LPCTSTR strIcon)
	{
		if ( strIcon != _T("") )
			m_strUnCheckIcon = strIcon;
	}

	void CMenuElementUI::SetIconSize(LONG cx, LONG cy)
	{
		m_szIconSize.cx = cx;
		m_szIconSize.cy = cy;
	}

	void CMenuElementUI::SetChecked(bool bCheck/* = true*/)
	{
		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if(mCheckInfos == NULL)
		{
			m_bCheck = bCheck;
			return;
		}
		SetItemInfo(GetName(), bCheck);
	}

	bool CMenuElementUI::GetChecked()
	{
		LPCTSTR pstrName = GetName();
		if(pstrName == NULL || lstrlen(pstrName) <= 0) return false;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo != NULL) {
				return pItemInfo->bChecked;
			}
		}
		else
		{
			return m_bCheck;
		}
		return false;
	}

	void CMenuElementUI::SetCheckItem(bool bCheckItem/* = false*/)
	{
		m_bCheckItem = bCheckItem;
	}

	bool CMenuElementUI::GetCheckItem() const
	{
		return m_bCheckItem;
	}

	void CMenuElementUI::SetShowExplandIcon(bool bShow)
	{
		m_bShowExplandIcon = bShow;
	}

	void CMenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("icon")) == 0){
			SetIcon(pstrValue);
		}
		if( _tcsicmp(pstrName, _T("uncheckicon")) == 0){
			SetUnCheckIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconsize")) == 0 ) {
			LPTSTR pstr = NULL;
			LONG cx = 0, cy = 0;
			cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			SetIconSize(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("checkitem")) == 0 ) {		
			SetCheckItem(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);		
		}
		else if( _tcsicmp(pstrName, _T("ischeck")) == 0 ) {		
			CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
			if (mCheckInfos != NULL)
			{
				bool bFind = false;
				for(int i = 0; i < mCheckInfos->GetSize(); i++) {
					MenuItemInfo* itemInfo = (MenuItemInfo*)mCheckInfos->GetAt(i);
					if(lstrcmpi(itemInfo->szName, GetName()) == 0) {
						bFind = true;
						break;
					}
				}
				if(!bFind) SetChecked(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);
			}
		}	
		else if( _tcsicmp(pstrName, _T("linetype")) == 0){
			if (_tcsicmp(pstrValue, _T("true")) == 0)
				SetLineType();
		}
		else if( _tcsicmp(pstrName, _T("expland")) == 0 ) {
			SetShowExplandIcon(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);
		}
		else if( _tcsicmp(pstrName, _T("linecolor")) == 0){
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			SetLineColor(_tcstoul(pstrValue, &pstr, 16));
		}
		else if( _tcsicmp(pstrName, _T("linepadding")) == 0 ) {
			RECT rcInset = { 0 };
			LPTSTR pstr = NULL;
			rcInset.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcInset.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcInset.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcInset.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetLinePadding(rcInset);
		}
		else if	( _tcsicmp(pstrName, _T("height")) == 0){
			SetFixedHeight(_ttoi(pstrValue));
		}
		else
			CListContainerElementUI::SetAttribute(pstrName, pstrValue);
	}


	MenuItemInfo* CMenuElementUI::GetItemInfo(LPCTSTR pstrName)
	{
		if(pstrName == NULL || lstrlen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo != NULL) {
				return pItemInfo;
			}
		}

		return NULL;
	}

	MenuItemInfo* CMenuElementUI::SetItemInfo(LPCTSTR pstrName, bool bChecked)
	{
		if(pstrName == NULL || lstrlen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
		if (mCheckInfos != NULL)
		{
			MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
			if(pItemInfo == NULL) {
				pItemInfo = new MenuItemInfo;
				_tcscpy(pItemInfo->szName, pstrName);
				pItemInfo->bChecked = bChecked;
				mCheckInfos->Insert(pstrName, pItemInfo);
			}
			else {
				pItemInfo->bChecked = bChecked;
			}

			return pItemInfo;
		}
		return NULL;
	}

	void CMenuElementUI::OnUpdateCommandUI()
	{
		//发消息给父窗口，返回check状态
		if (!::IsWindow(GetManager()->GetPaintWindow())) 
			return;

		if (CMenuWnd::GetGlobalContextMenuObserver().GetManager() == NULL)
			return;

		//if(!GetCheckItem())
		//	return;

		CMenuCmdUI cmdUI(this);
		LRESULT lRet = ::SendMessage(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), UIMSG_MENU_UPDATE_COMMAND_UI, (WPARAM)&cmdUI, (LPARAM)this);		
	}

	void CMenuElementUI::DrawDisableItemIcon(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		if ((m_pManager == NULL) || (hDC == NULL)) return;
		const TDrawInfo* pDrawInfo = m_pManager->GetDrawInfo(pStrImage, pStrModify);

		CPaintManagerUI *pManager = m_pManager;
		RECT rcItem = m_rcItem;
		RECT rcPaint = m_rcItem;
		//return DrawImageInfo(hDC, pManager, rcItem, rcPaint, pDrawInfo, instance);

		RECT rcDest = rcItem;
		if( pDrawInfo->rcDest.left != 0 || pDrawInfo->rcDest.top != 0 ||
			pDrawInfo->rcDest.right != 0 || pDrawInfo->rcDest.bottom != 0 ) {
				rcDest.left = rcItem.left + pDrawInfo->rcDest.left;
				rcDest.top = rcItem.top + pDrawInfo->rcDest.top;
				rcDest.right = rcItem.left + pDrawInfo->rcDest.right;
				if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;
				rcDest.bottom = rcItem.top + pDrawInfo->rcDest.bottom;
				if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;
		}
		bool bRet = _DrawImageMenuDisableIcon(hDC, pManager, rcItem, rcPaint, pDrawInfo->sImageName, pDrawInfo->sResType, rcDest, \
			pDrawInfo->rcSource, pDrawInfo->rcCorner, pDrawInfo->dwMask, pDrawInfo->uFade, pDrawInfo->bHole, pDrawInfo->bTiledX, pDrawInfo->bTiledY, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, m_instance);

	}
	bool CMenuElementUI::_DrawImageMenuDisableIcon(HDC hDC, CPaintManagerUI* pManager, const RECT& rc, const RECT& rcPaint, const CDuiString& sImageName, \
		const CDuiString& sImageResType, RECT rcItem, RECT rcBmpPart, RECT rcCorner, DWORD dwMask, BYTE bFade, \
		bool bHole, bool bTiledX, bool bTiledY, int width, int height, DWORD fillcolor, HINSTANCE instance)
	{
		if (sImageName.IsEmpty()) {
			return false;
		}
		const TImageInfo* data = NULL;
		if( sImageResType.IsEmpty() ) {
			data = pManager->GetImageEx((LPCTSTR)sImageName, NULL, dwMask, width, height, fillcolor, false, instance);
		}
		else {
			data = pManager->GetImageEx((LPCTSTR)sImageName, (LPCTSTR)sImageResType, dwMask, width, height, fillcolor, false, instance);
		}
		if( !data ) return false;    

		if( rcBmpPart.left == 0 && rcBmpPart.right == 0 && rcBmpPart.top == 0 && rcBmpPart.bottom == 0 ) {
			rcBmpPart.right = data->nX;
			rcBmpPart.bottom = data->nY;
		}
		if (rcBmpPart.right > data->nX) rcBmpPart.right = data->nX;
		if (rcBmpPart.bottom > data->nY) rcBmpPart.bottom = data->nY;

		RECT rcTemp;
		if( !::IntersectRect(&rcTemp, &rcItem, &rc) ) return true;
		if( !::IntersectRect(&rcTemp, &rcItem, &rcPaint) ) return true;

		CRenderEngine::DrawImage(hDC, data->hBitmap, rcItem, rcPaint, rcBmpPart, rcCorner, pManager->IsLayered() ? true : data->bAlpha, bFade, bHole, bTiledX, bTiledY);

		//需要把data->hBitmap转为灰度图
//		CRenderEngine::DrawImage(hDC, hbmp, rcItem, rcPaint, rcBmpPart, rcCorner, pManager->IsLayered() ? true : data->bAlpha, bFade, bHole, bTiledX, bTiledY);

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	CMenuCmdUI::CMenuCmdUI(CMenuElementUI *p) : pMenuElement(p)//m_bEnable(true), m_bCheck(false)
	{
	}

	void CMenuCmdUI::Enable(BOOL bEnable)
	{
		//m_bEnable = bOn;
		pMenuElement->SetEnabled(bEnable == TRUE);
	}

	void CMenuCmdUI::SetCheck(BOOL bCheck)
	{
		//m_bCheck = bCheck;
		pMenuElement->SetChecked(bCheck == TRUE);
		
	}

	bool CMenuCmdUI::GetCheck()
	{
		//return m_bCheck;
		return pMenuElement->GetChecked();
	}

	void CMenuCmdUI::SetText(LPCTSTR lpszText)
	{
		//m_sText = lpszText;
		return pMenuElement->SetText(lpszText);
	}

	CDuiString CMenuCmdUI::GetText()
	{
		//return m_sText;
		return pMenuElement->GetText();
	}

	CDuiString CMenuCmdUI::GetName()
	{
		//return m_sName;
		return pMenuElement->GetName();
	}
} // namespace DuiLib
