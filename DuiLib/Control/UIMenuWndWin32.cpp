#include "StdAfx.h"

#include "UIMenuWndWin32.h"

#ifdef DUILIB_WIN32
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	CMenuWndWin32::CMenuWndWin32():
	m_pOwner(NULL),
		m_pLayout(),
		m_xml(_T("")),
		isClosing(false),
		m_bAutoDestroy(false)
	{
		m_dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;
	}

	CMenuWndWin32::~CMenuWndWin32()
	{
		
	}

	void CMenuWndWin32::Close(UINT nRet)
	{
		ASSERT(::IsWindow(m_hWnd));
		if (!::IsWindow(m_hWnd)) return;
		PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
		isClosing = true;
	}


	BOOL CMenuWndWin32::Receive(ContextMenuParam param)
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

	CMenuWndWin32* CMenuWndWin32::CreateMenu(CMenuElementUI* pOwner, STRINGorID xml, POINT point, CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo /*= NULL*/, DWORD dwAlignment /*= eMenuAlignment_Left | eMenuAlignment_Top*/)
	{
		CMenuWndWin32* pMenu = new CMenuWndWin32;
		pMenu->Init(pOwner, xml, point, pMainPaintManager, pMenuCheckInfo, dwAlignment);
		return pMenu;
	}

	void CMenuWndWin32::DestroyMenu()
	{
		CStdStringPtrMap* mCheckInfos = CMenuWndWin32::GetGlobalContextMenuObserver().GetMenuCheckInfo();
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
			CMenuWndWin32::GetGlobalContextMenuObserver().SetMenuCheckInfo(NULL);
		}
	}
	
	MenuItemInfo* CMenuWndWin32::SetMenuItemInfo(LPCTSTR pstrName, bool bChecked)
	{
		if(pstrName == NULL || lstrlen(pstrName) <= 0) return NULL;

		CStdStringPtrMap* mCheckInfos = CMenuWndWin32::GetGlobalContextMenuObserver().GetMenuCheckInfo();
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

	void CMenuWndWin32::Init(CMenuElementUI* pOwner, STRINGorID xml, POINT point,
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
			CMenuWndWin32::GetGlobalContextMenuObserver().SetManger(pMainPaintManager);
			if (pMenuCheckInfo != NULL)
				CMenuWndWin32::GetGlobalContextMenuObserver().SetMenuCheckInfo(pMenuCheckInfo);
		}

		CMenuWndWin32::GetGlobalContextMenuObserver().AddReceiver(this);

		Create((m_pOwner == NULL) ? pMainPaintManager->GetPaintWindow() : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
		
		// HACK: Don't deselect the parent's caption
		HWND hWndParent = m_hWnd;
		while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

		::ShowWindow(m_hWnd, SW_SHOW);
		::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
	}

	LPCTSTR CMenuWndWin32::GetWindowClassName() const
	{
		return _T("DuiMenuWnd");
	}


	void CMenuWndWin32::Notify(TNotifyUI& msg)
	{
		if( CMenuWndWin32::GetGlobalContextMenuObserver().GetManager() != NULL) 
		{
			if( msg.sType == _T("click") || msg.sType == _T("valuechanged") ) 
			{
				CMenuWndWin32::GetGlobalContextMenuObserver().GetManager()->SendNotify(msg, false);
			}
		}

	}

	CControlUI* CMenuWndWin32::CreateControl( LPCTSTR pstrClassName )
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


	void CMenuWndWin32::OnFinalMessage(UIWND hWnd)
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
			m_pOwner->SetPushedState(false);
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

	LRESULT CMenuWndWin32::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

			m_pm.Init(m_hWnd, NULL, this);
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
			m_pm.Init(m_hWnd, NULL, this);
			m_pm.GetDPIObj()->SetScale(CMenuWndWin32::GetGlobalContextMenuObserver().GetManager()->GetDPIObj()->GetDPI());
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

	CMenuUI* CMenuWndWin32::GetMenuUI()
	{
		return dynamic_cast<CMenuUI*>(m_pm.GetRoot());
	}

	/*
	void CMenuWndWin32::ResizeMenu()
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
	*/
	void CMenuWndWin32::ResizeMenu()
	{
		//优化版修复扩展屏幕存在时，菜单弹出位置不正确的问题 2023年3月26 段先生 QQ547453134  
		//计算边界问题
		CMenuUI* pMenuRoot = static_cast<CMenuUI*>(m_pm.GetRoot());
		if (!pMenuRoot)
		{
			//获取菜单根节点失败
			return;
		}

		const int nWidth = m_pm.GetInitSize().cx;
		const int nHeight = m_pm.GetInitSize().cy;
		const int nMenuInset = pMenuRoot->GetInset().bottom + pMenuRoot->GetInset().top;

		RECT rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

		POINT ptBase = m_BasedPoint;
		ptBase.y += GetSystemMetrics(SM_CYMENU); // 加上任务栏的高度

		HMONITOR hMonitor = MonitorFromPoint(ptBase, MONITOR_DEFAULTTONEAREST);
		MONITORINFO mi = { sizeof(mi) };
		if (!GetMonitorInfo(hMonitor, &mi))
		{
			// 无法获取显示器信息
			return;
		}

		RECT rcMenu = { 0 };
		rcMenu.right = rcMenu.left + nWidth;
		rcMenu.bottom = rcMenu.top + nHeight + nMenuInset;

		if (ptBase.x < mi.rcWork.left)
		{
			ptBase.x = mi.rcWork.left;
		}
		else if (ptBase.x + nWidth > mi.rcWork.right)
		{
			ptBase.x = mi.rcWork.right - nWidth;
		}

		if (ptBase.y + nHeight > mi.rcWork.bottom)
		{
			ptBase.y = mi.rcWork.bottom - nHeight;
		}

		if (ptBase.y < mi.rcWork.top)
		{
			ptBase.y = mi.rcWork.top;
		}

		::SetWindowPos(m_hWnd, HWND_TOPMOST, ptBase.x, ptBase.y, nWidth, nHeight + nMenuInset, SWP_SHOWWINDOW);
		MoveWindow(m_hWnd, ptBase.x, ptBase.y, nWidth, nHeight, FALSE);
	}

	void CMenuWndWin32::ResizeSubMenu()
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
		MenuObserverImpl::Iterator iterator(CMenuWndWin32::GetGlobalContextMenuObserver());
		MenuMenuReceiverImplBase* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWndWin32* pContextMenu = dynamic_cast<CMenuWndWin32*>(pReceiver);
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

	void CMenuWndWin32::setDPI(int DPI) {
		m_pm.SetDPI(DPI);
	}


	LRESULT CMenuWndWin32::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HWND hFocusWnd = (HWND)wParam;

		BOOL bInMenuWindowList = FALSE;
		ContextMenuParam param;
		param.hWnd = GetHWND();

		MenuObserverImpl::Iterator iterator(CMenuWndWin32::GetGlobalContextMenuObserver());
		MenuMenuReceiverImplBase* pReceiver = iterator.next();
		while( pReceiver != NULL ) {
			CMenuWndWin32* pContextMenu = dynamic_cast<CMenuWndWin32*>(pReceiver);
			if( pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd ) {
				bInMenuWindowList = TRUE;
				break;
			}
			pReceiver = iterator.next();
		}

		if( !bInMenuWindowList ) {
			param.wParam = 1;
			CMenuWndWin32::GetGlobalContextMenuObserver().RBroadcast(param);
			return 0;
		}
		return 0;
	}
	LRESULT CMenuWndWin32::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	LRESULT CMenuWndWin32::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

} // namespace DuiLib
#endif // #ifdef DUILIB_WIN32


