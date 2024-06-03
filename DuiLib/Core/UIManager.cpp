#include "StdAfx.h"

#ifdef WIN32
#include <zmouse.h>
#endif

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//


	/////////////////////////////////////////////////////////////////////////////////////
	HINSTANCE CPaintManagerUI::m_hResourceInstance = NULL;
	CDuiString CPaintManagerUI::m_pStrResourcePath;
	CDuiString CPaintManagerUI::m_pStrResourceZip;
	CDuiString CPaintManagerUI::m_pStrResourceZipPwd;  //Garfield 20160325 带密码zip包解密
	HANDLE CPaintManagerUI::m_hResourceZip = NULL;
	bool CPaintManagerUI::m_bCachedResourceZip = true;
	int CPaintManagerUI::m_nResType = UILIB_FILE;
	TResInfo CPaintManagerUI::m_SharedResInfo;
	HINSTANCE CPaintManagerUI::m_hInstance = NULL;
	bool CPaintManagerUI::m_bUseHSL = false;
	bool CPaintManagerUI::m_bForceHSL = false;
	short CPaintManagerUI::m_H = 180;
	short CPaintManagerUI::m_S = 100;
	short CPaintManagerUI::m_L = 100;
	CStdPtrArray CPaintManagerUI::m_aPreMessages;
	CStdPtrArray CPaintManagerUI::m_aPlugins;

	BOOL CPaintManagerUI::UIDESIGNMODE = FALSE;
	BOOL CPaintManagerUI::UIDESIGNPREVIEW = FALSE;

	emRenderEngine CPaintManagerUI::m_emRenderEngine = DuiLib_Render_Default;

	CPaintManagerUI::CPaintManagerUI() :
		m_hWndPaint(NULL),
		m_pWindow(NULL),
		m_hwndTooltip(NULL),
		m_uTimerID(0x1000),
		m_pRoot(NULL),
		m_pFocus(NULL), m_pLastFocus(NULL),
		m_pEventHover(NULL),
		m_pEventClick(NULL),
		m_pEventKey(NULL),
		m_bFirstLayout(true),
		m_bFocusNeeded(false),
		m_bUpdateNeeded(false),
		m_bMouseTracking(false),
		m_bMouseCapture(false),
		m_bUsedVirtualWnd(false),
		m_bAsyncNotifyPosted(false), //add by liqs99, 原版的这个成员没有初始化
		m_bForceUseSharedRes(false),
		m_nOpacity(0xFF),
		m_bLayered(false),
		m_bLayeredChanged(false),
		m_bShowUpdateRect(false),
		m_bUseGdiplusText(false),
		m_trh(0),
		m_bDragMode(false),
		m_pDPI(NULL),
		m_iTooltipHoverTime(400UL),
		m_bLockUpdate(false)
	{
		GDIPLUS_STARTUP_INSTANCE;

		if (!m_SharedResInfo.m_DefaultFontInfo)
		{
			m_SharedResInfo.m_dwDefaultDisabledColor = 0xFFA7A6AA;
			m_SharedResInfo.m_dwDefaultFontColor = 0xFF000000;
			m_SharedResInfo.m_dwDefaultLinkFontColor = 0xFF0000FF;
			m_SharedResInfo.m_dwDefaultLinkHoverFontColor = 0xFFD3215F;
			m_SharedResInfo.m_dwDefaultSelectedBkColor = 0xFFBAE4FF;

			m_SharedResInfo.m_DefaultFontInfo = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
			m_SharedResInfo.m_DefaultFontInfo->CreateDefaultFont();		
		}

		m_ResInfo.m_dwDefaultDisabledColor = m_SharedResInfo.m_dwDefaultDisabledColor;
		m_ResInfo.m_dwDefaultFontColor = m_SharedResInfo.m_dwDefaultFontColor;
		m_ResInfo.m_dwDefaultLinkFontColor = m_SharedResInfo.m_dwDefaultLinkFontColor;
		m_ResInfo.m_dwDefaultLinkHoverFontColor = m_SharedResInfo.m_dwDefaultLinkHoverFontColor;
		m_ResInfo.m_dwDefaultSelectedBkColor = m_SharedResInfo.m_dwDefaultSelectedBkColor;
		m_ResInfo.m_DefaultFontInfo = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
		//m_ResInfo.m_DefaultFontInfo->CreateDefaultFont();	//这里如果也创建,会出现奇怪的问题(字体变了),为什么?



		m_szMinWindow.cx = 0;
		m_szMinWindow.cy = 0;
		m_szMaxWindow.cx = 0;
		m_szMaxWindow.cy = 0;
		m_szInitWindowSize.cx = 0;
		m_szInitWindowSize.cy = 0;
		m_szRoundCorner.cx = m_szRoundCorner.cy = 0;
		::ZeroMemory(&m_rcSizeBox, sizeof(m_rcSizeBox));
		::ZeroMemory(&m_rcCaption, sizeof(m_rcCaption));
		::ZeroMemory(&m_rcLayeredInset, sizeof(m_rcLayeredInset));
		//::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
		m_ptLastMousePos.x = m_ptLastMousePos.y = -1;


		CShadowUI::Initialize(m_hInstance);

		m_pLangManager = new CLangManagerUI;
		m_bInitWindowParameter = false;
	}

	CPaintManagerUI::~CPaintManagerUI()
	{
// 		if(m_pLangManager){ delete m_pLangManager; m_pLangManager = NULL; }
// 
// 		// Delete the control-tree structures
// 		for( int i = 0; i < m_aDelayedCleanup.GetSize(); i++ ) delete static_cast<CControlUI*>(m_aDelayedCleanup[i]);
// 		m_aDelayedCleanup.Resize(0);
// 		for( int i = 0; i < m_aAsyncNotify.GetSize(); i++ ) delete static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
// 		m_aAsyncNotify.Resize(0);
// 
// 		m_mNameHash.Resize(0);
// 		if( m_pRoot != NULL ) delete m_pRoot;
// 
// 		//delete m_ResInfo.m_DefaultFontInfo;
// 		RemoveAllFonts();
// 		RemoveAllImages();
// 		RemoveAllStyle();
// 		RemoveAllDefaultAttributeList();
// 		RemoveAllWindowCustomAttribute();
// 		RemoveAllOptionGroups();
// 		//RemoveAllTimers(); //这里调用是无效的，析构函数不允许调用虚成员函数
// 		RemoveAllDrawInfos();
// 
// #ifdef WIN32
// 		if (!m_aFonts.IsEmpty()) {
// 			for (int i = 0; i < m_aFonts.GetSize();++i)
// 			{
// 				HANDLE handle = static_cast<HANDLE>(m_aFonts.GetAt(i));
// 				::RemoveFontMemResourceEx(handle);
// 			}
// 		}
// #endif
// 		m_aPreMessages.Remove(m_aPreMessages.Find(this));
// 
// 
// 		// DPI管理对象
// 		if (m_pDPI != NULL) {
// 			delete m_pDPI;
// 			m_pDPI = NULL;
// 		}
// 		
// 		if(m_pRenderEngine)
// 		{
// 			//m_pRenderEngine->Release();
// 			m_pRenderEngine = NULL;
// 		}
	}

	void CPaintManagerUI::Init(UIWND hWnd, LPCTSTR pstrName, CWindowWnd *pWindow)
	{
		m_mNameHash.Resize();
		RemoveAllFonts();
		RemoveAllImages();
		RemoveAllStyle();
		RemoveAllDefaultAttributeList();
		RemoveAllWindowCustomAttribute();
		RemoveAllOptionGroups();
		RemoveAllTimers();

		m_sName = pstrName;
	}

	void CPaintManagerUI::SetRenderEngineType(emRenderEngine render)
	{
		if(m_emRenderEngine == render)
			return;
	
		m_emRenderEngine = render;	
		UIGlobal::Release();
		
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
		{
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			pManager->m_pRenderEngine = NULL;
		}

		//注意，切换渲染引擎，需要重新构造Font，Images等等与渲染相关的资源。
		//但是动态切换引擎，是非常麻烦的。
		//比如UIPicture加载的图像，Edit等等内部控件的字体，应用端通过UIFont创建的字体，通过UIImage加载的图像。这些通通需要重新载入。
		//所以，就没法支持动态切换！！！（gdi和Gdiplus特殊）
	}

	emRenderEngine CPaintManagerUI::GetRenderEngineType()
	{
		return m_emRenderEngine;
	}

	HINSTANCE CPaintManagerUI::GetInstance()
	{
		return m_hInstance;
	}

	CDuiString CPaintManagerUI::GetInstancePath()
	{
#ifdef WIN32
		if (m_hInstance == NULL) return _T("");
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetModuleFileName(m_hInstance, tszModule, MAX_PATH);
		CDuiString sInstancePath = tszModule;
		int pos = sInstancePath.ReverseFind(_T('\\'));
		if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);
		return sInstancePath;
#elif defined __linux__
		char  path[4096];
		//获取当前程序绝对路径
		int len = readlink("/proc/self/exe", path, 4096);
		path[len] = '\0';
		CDuiString sInstancePath = path;
		int pos = sInstancePath.ReverseFind(_T('/'));
		if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);
		return sInstancePath;
#elif defined __APPLE__
		char path[1025];
        memset(path, 0, 1025*sizeof(char));
		unsigned int size = 1024;
		_NSGetExecutablePath(path, &size);
		CDuiString sInstancePath = path;
		int pos = sInstancePath.ReverseFind(_T('/'));
		if (pos >= 0) sInstancePath = sInstancePath.Left(pos + 1);
		return sInstancePath;
#endif
	}

	CDuiString CPaintManagerUI::GetCurrentPath()
	{
#ifdef WIN32
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetCurrentDirectory(MAX_PATH, tszModule);
		return tszModule;
#endif
		return _T("");
	}

	HINSTANCE CPaintManagerUI::GetResourceDll()
	{
		if( m_hResourceInstance == NULL ) return m_hInstance;
		return m_hResourceInstance;
	}

	const CDuiString& CPaintManagerUI::GetResourcePath()
	{
		return m_pStrResourcePath;
	}

	const CDuiString& CPaintManagerUI::GetResourceZip()
	{
		return m_pStrResourceZip;
	}

	const CDuiString& CPaintManagerUI::GetResourceZipPwd()
	{
		return m_pStrResourceZipPwd;
	}

	bool CPaintManagerUI::IsCachedResourceZip()
	{
		return m_bCachedResourceZip;
	}

	HANDLE CPaintManagerUI::GetResourceZipHandle()
	{
		return m_hResourceZip;
	}

	void CPaintManagerUI::SetInstance(HINSTANCE hInst)
	{
		m_hInstance = hInst;
	}

	void CPaintManagerUI::SetCurrentPath(LPCTSTR pStrPath)
	{
#ifdef WIN32
		::SetCurrentDirectory(pStrPath);
#endif
	}

	void CPaintManagerUI::SetResourceDll(HINSTANCE hInst)
	{
		m_hResourceInstance = hInst;
	}

	void CPaintManagerUI::SetResourcePath(LPCTSTR pStrPath)
	{
		m_pStrResourcePath = pStrPath;
		if( m_pStrResourcePath.IsEmpty() ) return;
		TCHAR cEnd = m_pStrResourcePath.GetAt(m_pStrResourcePath.GetLength() - 1);
		if( cEnd != _T('\\') && cEnd != _T('/') ) m_pStrResourcePath += _T('/');
	}

	void CPaintManagerUI::SetResourceZip(LPVOID pVoid, unsigned int len, LPCTSTR password)
	{
#ifdef WIN32
		if( m_pStrResourceZip == _T("membuffer") ) return;
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}
		m_pStrResourceZip = _T("membuffer");
		m_bCachedResourceZip = true;
		m_pStrResourceZipPwd = password;  //Garfield 20160325 带密码zip包解密
		if( m_bCachedResourceZip ) 
		{
			UISTRING_CONVERSION;
			m_hResourceZip = (HANDLE)OpenZip(pVoid, len, UIT2A(password));
		}
#endif
	}

	void CPaintManagerUI::SetResourceZip(LPCTSTR pStrPath, bool bCachedResourceZip, LPCTSTR password)
	{
#ifdef WIN32
		if( m_pStrResourceZip == pStrPath && m_bCachedResourceZip == bCachedResourceZip ) return;
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}
		m_pStrResourceZip = pStrPath;
		m_bCachedResourceZip = bCachedResourceZip;
		m_pStrResourceZipPwd = password;
		if( m_bCachedResourceZip ) {
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			sFile += CPaintManagerUI::GetResourceZip();
			UISTRING_CONVERSION;
			m_hResourceZip = (HANDLE)OpenZip(sFile.GetData(), UIT2A(password));
		}
#endif
	}

	void CPaintManagerUI::SetResourceType(int nType)
	{
		m_nResType = nType;
	}

	int CPaintManagerUI::GetResourceType()
	{
		return m_nResType;
	}

	bool CPaintManagerUI::GetHSL(short* H, short* S, short* L)
	{
		*H = m_H;
		*S = m_S;
		*L = m_L;
		return m_bUseHSL;
	}

	void CPaintManagerUI::SetHSL(short H, short S, short L)
	{
		bool bUseHSL = H != 180 || S != 100 || L != 100;
		if( m_bUseHSL || m_bUseHSL != bUseHSL ) 
		{
			m_bUseHSL = bUseHSL;
			if( H == m_H && S == m_S && L == m_L ) return;
			m_H = CLAMP(H, 0, 360);
			m_S = CLAMP(S, 0, 200);
			m_L = CLAMP(L, 0, 200);
			AdjustSharedImagesHSL();
			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
				CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
				if( pManager != NULL ) pManager->AdjustImagesHSL();
			}
		}
	}

	bool CPaintManagerUI::IsUseHSL()
	{
		return m_bUseHSL;
	}

	void CPaintManagerUI::SetForceHSL(bool bForceHSL)
	{
		m_bForceHSL = bForceHSL;
	}

	bool CPaintManagerUI::IsForceHSL()
	{
		return m_bForceHSL;
	}

	void CPaintManagerUI::ReloadSkin()
	{
		ReloadSharedImages();
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			pManager->ReloadImages();
		}
	}

	CPaintManagerUI* CPaintManagerUI::GetPaintManager(LPCTSTR pstrName)
	{
		if( pstrName == NULL ) return NULL;
		CDuiString sName = pstrName;
		if( sName.IsEmpty() ) return NULL;
		for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			if( pManager != NULL && sName == pManager->GetName() ) return pManager;
		}
		return NULL;
	}

	CStdPtrArray* CPaintManagerUI::GetPaintManagers()
	{
		return &m_aPreMessages;
	}

	BOOL CPaintManagerUI::UIAction(UIWND hWnd, LPCTSTR sControlName, UINT action, WPARAM wparam, LPARAM lparam)
	{
		if(!CPlatform::IsWindow(hWnd)) return FALSE;

		TUIAction act;
		act.sControlName = sControlName;
		act.action = action;
		act.wParam = wparam;
		act.lParam = lparam;
		return CPlatform::SendMessage(hWnd, UIMSG_CONTROL_ACTION, (WPARAM)&act, 0);
	}

	BOOL CPaintManagerUI::UIActionAsync(UIWND hWnd, LPCTSTR sControlName, UINT action, WPARAM wparam, LPARAM lparam)
	{
		if(!CPlatform::IsWindow(hWnd)) return FALSE;

		TUIAction *act = new TUIAction;
		act->sControlName = sControlName;
		act->action = action;
		act->wParam = wparam;
		act->lParam = lparam;
		return CPlatform::PostMessage(hWnd, UIMSG_CONTROL_ACTION_ASYNC, (WPARAM)act, 0);
	}

	bool CPaintManagerUI::LoadPlugin(LPCTSTR pstrModuleName)
	{
#ifdef WIN32
		ASSERT( !::IsBadStringPtr(pstrModuleName,-1) || pstrModuleName == NULL );
		if( pstrModuleName == NULL ) return false;
		HMODULE hModule = ::LoadLibrary(pstrModuleName);
		if( hModule != NULL ) {
			LPCREATECONTROL lpCreateControl = (LPCREATECONTROL)::GetProcAddress(hModule, "CreateControl");
			if( lpCreateControl != NULL ) {
				if( m_aPlugins.Find(lpCreateControl) >= 0 ) return true;
				m_aPlugins.Add(lpCreateControl);
				return true;
			}
		}
#endif
		return false;
	}

	CStdPtrArray* CPaintManagerUI::GetPlugins()
	{
		return &m_aPlugins;
	}

	UIWND CPaintManagerUI::GetPaintWindow() const
	{
		return m_hWndPaint;
	}

	CWindowWnd *CPaintManagerUI::GetWindow() const
	{
		return m_pWindow;
	}

	UIWND CPaintManagerUI::GetTooltipWindow() const
	{
		return m_hwndTooltip;
	}

	int CPaintManagerUI::GetTooltipHoverTime() const
	{
		return m_iTooltipHoverTime;
	}

	void CPaintManagerUI::SetTooltipHoverTime(int iTime)
	{
		m_iTooltipHoverTime = iTime;
	}

	CDuiString CPaintManagerUI::GetName() const
	{
		return m_sName;
	}

	POINT CPaintManagerUI::GetLastMousePos() const
	{
		return m_ptLastMousePos;
	}

	SIZE CPaintManagerUI::GetInitSize()
	{
		return m_szInitWindowSize;
	}

	void CPaintManagerUI::SetInitSize(int cx, int cy)
	{
		m_szInitWindowSize.cx = cx;
		m_szInitWindowSize.cy = cy;
		if( m_pRoot == NULL && m_hWndPaint != NULL ) {
			CPlatform::SetWindowPos(m_hWndPaint, NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		}
	}

	RECT& CPaintManagerUI::GetSizeBox()
	{
		return m_rcSizeBox;
	}

	void CPaintManagerUI::SetSizeBox(RECT& rcSizeBox)
	{
		m_rcSizeBox = rcSizeBox;
	}

	RECT& CPaintManagerUI::GetCaptionRect()
	{
		return m_rcCaption;
	}

	void CPaintManagerUI::SetCaptionRect(RECT& rcCaption)
	{
		m_rcCaption = rcCaption;
	}

	SIZE CPaintManagerUI::GetRoundCorner() const
	{
		return m_szRoundCorner;
	}

	void CPaintManagerUI::SetRoundCorner(int cx, int cy)
	{
		m_szRoundCorner.cx = cx;
		m_szRoundCorner.cy = cy;
	}

	SIZE CPaintManagerUI::GetMinInfo()  
	{ 
		SIZE sz;
		sz.cx = GetDPIObj()->ScaleInt(	m_szMinWindow.cx );
		sz.cy = GetDPIObj()->ScaleInt(	m_szMinWindow.cy);
		return sz;
	}

	void CPaintManagerUI::SetMinInfo(int cx, int cy)
	{
		ASSERT(cx>=0 && cy>=0);
		
		m_szMinWindow.cx = cx;
		m_szMinWindow.cy = cy;
	}

	SIZE CPaintManagerUI::GetMaxInfo()  
	{ 
		SIZE sz;
		sz.cx = GetDPIObj()->ScaleInt(m_szMaxWindow.cx);
  sz.cy = GetDPIObj()->ScaleInt(m_szMaxWindow.cy);
		return sz;
	}

	void CPaintManagerUI::SetMaxInfo(int cx, int cy)
	{
		ASSERT(cx>=0 && cy>=0);
		m_szMaxWindow.cx =   cx;
		m_szMaxWindow.cy =  cy;
	}

	bool CPaintManagerUI::IsShowUpdateRect() const
	{
		return m_bShowUpdateRect;
	}

	void CPaintManagerUI::SetShowUpdateRect(bool show)
	{
		m_bShowUpdateRect = show;
	}

	bool CPaintManagerUI::IsNoActivate()
	{
		return m_bNoActivate;
	}

	void CPaintManagerUI::SetNoActivate(bool bNoActivate)
	{
		m_bNoActivate = bNoActivate;
	}

	BYTE CPaintManagerUI::GetOpacity() const
	{
		return m_nOpacity;
	}

	void CPaintManagerUI::SetOpacity(BYTE nOpacity)
	{
		m_nOpacity = nOpacity;
#ifdef DUILIB_WIN32
		if( m_hWndPaint != NULL ) {
			typedef BOOL (__stdcall *PFUNCSETLAYEREDWINDOWATTR)(HWND, COLORREF, BYTE, DWORD);
			PFUNCSETLAYEREDWINDOWATTR fSetLayeredWindowAttributes = NULL;

			HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
			if (hUser32)
			{
				fSetLayeredWindowAttributes = 
					(PFUNCSETLAYEREDWINDOWATTR)::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
				if( fSetLayeredWindowAttributes == NULL ) return;
			}

			DWORD dwStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
			DWORD dwNewStyle = dwStyle;
			if( nOpacity >= 0 && nOpacity < 256 ) dwNewStyle |= WS_EX_LAYERED;
			else dwNewStyle &= ~WS_EX_LAYERED;
			if(dwStyle != dwNewStyle) ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewStyle);
			fSetLayeredWindowAttributes(m_hWndPaint, 0, nOpacity, LWA_ALPHA);
		}
#endif
	}

	bool CPaintManagerUI::IsLayered()
	{
		return m_bLayered;
	}

	RECT& CPaintManagerUI::GetLayeredInset()
	{
		return m_rcLayeredInset;
	}

	void CPaintManagerUI::SetLayeredInset(RECT& rcLayeredInset)
	{
		m_rcLayeredInset = rcLayeredInset;
		m_bLayeredChanged = true;
		Invalidate();
	}

	BYTE CPaintManagerUI::GetLayeredOpacity()
	{
		return m_nOpacity;
	}

	void CPaintManagerUI::SetLayeredOpacity(BYTE nOpacity)
	{
		m_nOpacity = nOpacity;
		m_bLayeredChanged = true;
		Invalidate();
	}

	LPCTSTR CPaintManagerUI::GetLayeredImage()
	{
		return m_diLayered.sDrawString;
	}

	void CPaintManagerUI::SetLayeredImage(LPCTSTR pstrImage)
	{
		m_diLayered.sDrawString = pstrImage;
		m_diLayered.Parse(pstrImage, NULL, this);
		RECT rcNull = {0};
		Render()->DrawImageInfo(rcNull, rcNull, &m_diLayered);
		m_bLayeredChanged = true;
		Invalidate();
	}

	TDrawInfo *CPaintManagerUI::GetLayeredDrawInfo() { return &m_diLayered; }

	void CPaintManagerUI::SetLayeredChanged(bool bChange) { m_bLayeredChanged = bChange; }

	bool CPaintManagerUI::IsLayeredChanged() const { return m_bLayeredChanged; }

	bool CPaintManagerUI::IsFocusNeeded() const { return m_bFocusNeeded; }

	CShadowUI* CPaintManagerUI::GetShadow()
	{
		return &m_shadow;
	}

	void CPaintManagerUI::SetUseGdiplusText(bool bUse)
	{
		m_bUseGdiplusText = bUse;
	}

	bool CPaintManagerUI::IsUseGdiplusText() const
	{
		return m_bUseGdiplusText;
	}

	void CPaintManagerUI::SetGdiplusTextRenderingHint(int trh)
	{
		m_trh = trh;
	}

	int CPaintManagerUI::GetGdiplusTextRenderingHint() const
	{
		return m_trh;
	}

	
	bool CPaintManagerUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		// Custom handling of events
		switch( uMsg ) {
		case WM_APP + 1:
			return OnApp1(wParam, lParam, lRes);
			break;
		case WM_CLOSE:
			return OnClose(wParam, lParam, lRes);
			break;
		case WM_ERASEBKGND:
			{
				// We'll do the painting here...
				lRes = 1;
				return true;
				return OnEraseBkgnd(wParam, lParam, lRes); //预留，不调用
			}
			break;
		case WM_PAINT:
			return OnPaint(wParam, lParam, lRes);
			break;
		case WM_PRINTCLIENT:
			return false;
			return OnPrintClient(wParam, lParam, lRes);
			break;
		case WM_GETMINMAXINFO:
			return OnGetMaxMinInfo(wParam, lParam, lRes);
			break;
		case WM_SIZE:
			return OnSize(wParam, lParam, lRes);
			break;
		case WM_TIMER:
			return OnTimer(wParam, lParam, lRes);
			break;
		case WM_MOUSEHOVER:
			return OnMouseOver(wParam, lParam, lRes);
			break;
		case WM_MOUSELEAVE:
			return OnMouseLeave(wParam, lParam, lRes);
			break;
		case WM_MOUSEMOVE:
			return OnMouseMove(wParam, lParam, lRes);
			break;
		case WM_LBUTTONDOWN:
			return OnLButtonDown(wParam, lParam, lRes);
			break;
		case WM_LBUTTONDBLCLK:
			return OnLButtonDbClick(wParam, lParam, lRes);
			break;
		case WM_LBUTTONUP:
			return OnLButtonUp(wParam, lParam, lRes);
			break;
		case WM_RBUTTONDOWN:
			return OnRButtonDown(wParam, lParam, lRes);
			break;
		case WM_RBUTTONUP:
			return OnRButtonUp(wParam, lParam, lRes);
			break;
		case WM_MBUTTONDOWN:
			return OnMButtonDown(wParam, lParam, lRes);
			break;
		case WM_MBUTTONUP:
			return OnMButtonUp(wParam, lParam, lRes);
			break;
		case WM_CONTEXTMENU:
			return OnContextMenu(wParam, lParam, lRes);
			break;
		case WM_MOUSEWHEEL:
			return OnMouseWheel(wParam, lParam, lRes);
			break;
		case WM_CHAR:
			return OnChar(wParam, lParam, lRes);
			break;
		case WM_KEYDOWN:
			return OnKeyDown(wParam, lParam, lRes);
			break;
		case WM_KEYUP:
			return OnKeyUp(wParam, lParam, lRes);
			break;
		case WM_SETCURSOR:
			return OnSetCursor(wParam, lParam, lRes);
			break;
		case WM_SETFOCUS:
			return OnSetFocus(wParam, lParam, lRes);
			break;
		case WM_KILLFOCUS:
			return OnKillFocus(wParam, lParam, lRes);
			break;
		case WM_NOTIFY:
			return OnNotify(wParam, lParam, lRes);
			break;
		case WM_COMMAND:
			return OnCommand(wParam, lParam, lRes);
			break;
		case WM_CTLCOLOREDIT:
			return OnCtlColorEdit(wParam, lParam, lRes);
			break;
		case WM_CTLCOLORSTATIC:
			return OnCtlColorStatic(wParam, lParam, lRes);
			break;
		default:
			break;
		}
		return false;
	}

	bool CPaintManagerUI::IsUpdateNeeded() const
	{
		return m_bUpdateNeeded;
	}

	void CPaintManagerUI::NeedUpdate()
	{
		m_bUpdateNeeded = true;
	}

	void CPaintManagerUI::LockUpdate(bool bLock)
	{
		m_bLockUpdate = bLock;
	}

	bool CPaintManagerUI::IsLockUpdate()
	{
		return m_bLockUpdate;
	}
	void CPaintManagerUI::Invalidate()
	{
		RECT rcClient = { 0 };
		CPlatform::GetClientRect(m_hWndPaint, &rcClient);
		//::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcClient);
		m_rcLayeredUpdate.Union(m_rcLayeredUpdate, rcClient);
		InvalidateRect(m_hWndPaint, NULL, FALSE);
	}

	void CPaintManagerUI::Invalidate(RECT& rcItem)
	{
		if( rcItem.left < 0 ) rcItem.left = 0;
		if( rcItem .top < 0 ) rcItem.top = 0;
		if( rcItem.right < rcItem.left ) rcItem.right = rcItem.left;
		if( rcItem.bottom < rcItem.top ) rcItem.bottom = rcItem.top;
		//::UnionRect(&m_rcLayeredUpdate, &m_rcLayeredUpdate, &rcItem);
		m_rcLayeredUpdate.Union(m_rcLayeredUpdate, rcItem);
		InvalidateRect(m_hWndPaint, &rcItem, FALSE);
	}

	bool CPaintManagerUI::AttachDialog(CControlUI* pControl)
	{
		ASSERT(CPlatform::IsWindow(m_hWndPaint));
		// 创建阴影窗口
		m_shadow.Create(this);

		// Reset any previous attachment
		SetFocus(NULL);
		m_pEventKey = NULL;
		m_pEventHover = NULL;
		m_pEventClick = NULL;
		// Remove the existing control-tree. We might have gotten inside this function as
		// a result of an event fired or similar, so we cannot just delete the objects and
		// pull the internal memory of the calling code. We'll delay the cleanup.
		if( m_pRoot != NULL ) {
			m_aPostPaintControls.Empty();
			AddDelayedCleanup(m_pRoot);
		}
		// Set the dialog root element
		m_pRoot = pControl;
		// Go ahead...
		m_bUpdateNeeded = true;
		m_bFirstLayout = true;
		m_bFocusNeeded = true;
		// Initiate all control
		return InitControls(pControl);
	}

	bool CPaintManagerUI::InitControls(CControlUI* pControl, CControlUI* pParent /*= NULL*/)
	{
		ASSERT(pControl);
		if( pControl == NULL ) return false;
		pControl->SetManager(this, pParent != NULL ? pParent : pControl->GetParent(), true);
		pControl->FindControl(__FindControlFromNameHash, this, UIFIND_ALL);
		return true;
	}

	void CPaintManagerUI::ReapObjects(CControlUI* pControl)
	{
		if( pControl == m_pEventKey ) m_pEventKey = NULL;
		if( pControl == m_pEventHover ) m_pEventHover = NULL;
		if( pControl == m_pEventClick ) m_pEventClick = NULL;
		if( pControl == m_pFocus ) m_pFocus = NULL;
		KillTimer(pControl, -1);
		const CDuiString& sName = pControl->GetName();
		if( !sName.IsEmpty() ) {
			if( pControl == FindControl(sName) ) m_mNameHash.Remove(sName);
		}
		for( int i = 0; i < m_aAsyncNotify.GetSize(); i++ ) {
			TNotifyUI* pMsg = static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
			if( pMsg->pSender == pControl ) pMsg->pSender = NULL;
		}    
	}

	bool CPaintManagerUI::AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) {
			CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
			for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
				if( static_cast<CControlUI*>(aOptionGroup->GetAt(i)) == pControl ) {
					return false;
				}
			}
			aOptionGroup->Add(pControl);
		}
		else {
			CStdPtrArray* aOptionGroup = new CStdPtrArray(6);
			aOptionGroup->Add(pControl);
			m_mOptionGroup.Insert(pStrGroupName, aOptionGroup);
		}
		return true;
	}

	CStdPtrArray* CPaintManagerUI::GetOptionGroup(LPCTSTR pStrGroupName)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) return static_cast<CStdPtrArray*>(lp);
		return NULL;
	}

	void CPaintManagerUI::RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl)
	{
		LPVOID lp = m_mOptionGroup.Find(pStrGroupName);
		if( lp ) {
			CStdPtrArray* aOptionGroup = static_cast<CStdPtrArray*>(lp);
			if( aOptionGroup == NULL ) return;
			for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
				if( static_cast<CControlUI*>(aOptionGroup->GetAt(i)) == pControl ) {
					aOptionGroup->Remove(i);
					break;
				}
			}
			if( aOptionGroup->IsEmpty() ) {
				delete aOptionGroup;
				m_mOptionGroup.Remove(pStrGroupName);
			}
		}
	}

	void CPaintManagerUI::RemoveAllOptionGroups()
	{
		CStdPtrArray* aOptionGroup;
		for( int i = 0; i< m_mOptionGroup.GetSize(); i++ ) {
			if(LPCTSTR key = m_mOptionGroup.GetAt(i)) {
				aOptionGroup = static_cast<CStdPtrArray*>(m_mOptionGroup.Find(key));
				delete aOptionGroup;
			}
		}
		m_mOptionGroup.RemoveAll();
	}

	void CPaintManagerUI::MessageLoop()
	{
#ifdef DUILIB_WIN32
		MSG msg = { 0 };
		while( ::GetMessage(&msg, NULL, 0, 0) ) {
			if( !CPaintManagerUI::TranslateMessage(&msg) ) {
				::TranslateMessage(&msg);
				try{
					::DispatchMessage(&msg);
				} catch(...) {
					DUITRACE(_T("EXCEPTION: %s(%d)\n"), __FILET__, __LINE__);
#ifdef _DEBUG
					throw "CPaintManagerUI::MessageLoop";
#endif
				}
			}
		}
#endif //#ifdef DUILIB_WIN32

#ifdef DUILIB_GTK
		gtk_main();
#endif
	}

	void CPaintManagerUI::Term()
	{
		//卸载脚本引擎
		if(m_funDeleteScriptEngine){
			(*m_funDeleteScriptEngine)(m_pSharedScriptEngine);
		}

		// 销毁资源管理器
		CResourceManager::GetInstance()->Release();
		CControlFactory::GetInstance()->Release();
		CMenuWnd::DestroyMenu();
		CLangManagerUI::ReleaseStringTable();

		// 清理共享资源
		// 图片
		UIImage* data;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) {
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key, false));
				if (data) {
					data->Release();
				}
			}
		}
		m_SharedResInfo.m_ImageHash.RemoveAll();
		// 字体
		UIFont* pFontInfo;
		for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key, false));
				if (pFontInfo) {
					pFontInfo->Release();
				}
			}
		}
		m_SharedResInfo.m_CustomFonts.RemoveAll();
		// 默认字体
 		if(m_SharedResInfo.m_DefaultFontInfo) {
 			m_SharedResInfo.m_DefaultFontInfo = NULL;
 		}
		// 样式
		CDuiString* pStyle;
		for( int i = 0; i< m_SharedResInfo.m_StyleHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_StyleHash.GetAt(i)) {
				pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(key, false));
				if (pStyle) {
					delete pStyle;
					pStyle = NULL;
				}
			}
		}
		m_SharedResInfo.m_StyleHash.RemoveAll();

		// 样式
		CDuiString* pAttr;
		for( int i = 0; i< m_SharedResInfo.m_AttrHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_AttrHash.GetAt(i)) {
				pAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(key, false));
				if (pAttr) {
					delete pAttr;
					pAttr = NULL;
				}
			}
		}
		m_SharedResInfo.m_AttrHash.RemoveAll();

		// 关闭ZIP
#ifdef WIN32
		if( m_bCachedResourceZip && m_hResourceZip != NULL ) {
			CloseZip((HZIP)m_hResourceZip);
			m_hResourceZip = NULL;
		}
#endif
	}

	CDPI * DuiLib::CPaintManagerUI::GetDPIObj()
	{
		if (m_pDPI == NULL) {
			m_pDPI = new CDPI;
		}
		return m_pDPI;
	}

	void DuiLib::CPaintManagerUI::SetDPI(int iDPI)
	{
#ifdef DUILIB_WIN32
		int scale1 = GetDPIObj()->GetScale();
		GetDPIObj()->SetScale(iDPI);
		int scale2 = GetDPIObj()->GetScale();
		ResetDPIAssets();
		RECT rcWnd = {0};
		::GetWindowRect(GetPaintWindow(), &rcWnd);
		RECT*  prcNewWindow = &rcWnd;
		if (!::IsZoomed(GetPaintWindow())) {
			RECT rc = rcWnd;
			rc.right = rcWnd.left + (rcWnd.right - rcWnd.left) * scale2 / scale1;
			rc.bottom = rcWnd.top + (rcWnd.bottom - rcWnd.top) * scale2 / scale1;
			prcNewWindow = &rc;
		}
		CPlatform::SetWindowPos(GetPaintWindow(), NULL, prcNewWindow->left, prcNewWindow->top, prcNewWindow->right - prcNewWindow->left, prcNewWindow->bottom - prcNewWindow->top, SWP_NOZORDER | SWP_NOACTIVATE);
		if (GetRoot() != NULL) GetRoot()->NeedUpdate();
		CPlatform::PostMessage(GetPaintWindow(), UIMSG_SET_DPI, 0, 0);
#endif
	}

	void DuiLib::CPaintManagerUI::SetAllDPI(int iDPI)
	{
		for (int i = 0; i < m_aPreMessages.GetSize(); i++) {
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
			pManager->SetDPI(iDPI);
		}
	}

	void DuiLib::CPaintManagerUI::ResetDPIAssets()
	{
		RemoveAllDrawInfos();
		RemoveAllImages();;

		for (int it = 0; it < m_ResInfo.m_CustomFonts.GetSize(); it++) {
			UIFont* pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(m_ResInfo.m_CustomFonts[it]));
			pFontInfo->RebuildFont(this);
		}
		m_ResInfo.m_DefaultFontInfo->RebuildFont(this);

		for (int it = 0; it < m_SharedResInfo.m_CustomFonts.GetSize(); it++) {
			UIFont* pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(m_SharedResInfo.m_CustomFonts[it]));
			pFontInfo->RebuildFont(this);
		}
		m_SharedResInfo.m_DefaultFontInfo->RebuildFont(this);

#ifdef DUILIB_WIN32
		CStdPtrArray *richEditList = FindSubControlsByInterface(GetRoot(), DUI_CTR_RICHEDIT);
		for (int i = 0; i < richEditList->GetSize(); i++)
		{
			CRichEditUI* pT = static_cast<CRichEditUI*>((*richEditList)[i]);
			pT->SetFont(pT->GetFont());
		}
#endif
	}

	CControlUI* CPaintManagerUI::GetLastFocus() const //add by liqs99
	{
		if(m_pLastFocus == NULL)	return NULL;
		if(!m_pLastFocus->IsEnabled() || !m_pLastFocus->IsVisible())	return NULL;
		return m_pLastFocus;
	}

	CControlUI* CPaintManagerUI::GetFocus() const
	{
		return m_pFocus;
	}

	void CPaintManagerUI::SetFocus(CControlUI* pControl)
	{
		//设计器模式，设置控件的焦点，不要设置窗口焦点
		if(!UIDESIGNMODE)
		{
			// Paint manager window has focus?
			UIWND hFocusWnd = CPlatform::GetFocus();
			if( hFocusWnd != m_hWndPaint && pControl != m_pFocus ) 
				CPlatform::SetFocus(m_hWndPaint);
		}

		// Already has focus?
		if( pControl == m_pFocus ) return;
		// Remove focus from old control
		if( m_pFocus != NULL ) 
		{
			TEventUI event = { 0 };
			event.Type = UIEVENT_KILLFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = CPlatform::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
			m_pLastFocus = m_pFocus;
			m_pFocus = NULL;
		}
		if( pControl == NULL ) return;
		// Set focus to new control
		if( pControl != NULL 
			&& pControl->GetManager() == this 
			&& pControl->IsVisible() 
			&& pControl->IsEnabled() ) 
		{
			m_pFocus = pControl;
			TEventUI event = { 0 };
			event.Type = UIEVENT_SETFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = CPlatform::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_SETFOCUS);
		}
	}

	void CPaintManagerUI::SetFocusNeeded(CControlUI* pControl)
	{
		CPlatform::SetFocus(m_hWndPaint);
		if( pControl == NULL ) return;
		if( m_pFocus != NULL ) {
			TEventUI event = { 0 };
			event.Type = UIEVENT_KILLFOCUS;
			event.pSender = pControl;
			event.dwTimestamp = CPlatform::GetTickCount();
			m_pFocus->Event(event);
			SendNotify(m_pFocus, DUI_MSGTYPE_KILLFOCUS);
			m_pFocus = NULL;
		}
		FINDTABINFO info = { 0 };
		info.pFocus = pControl;
		info.bForward = false;
		m_pFocus = m_pRoot->FindControl(__FindControlFromTab, &info, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
		m_bFocusNeeded = true;
		if( m_pRoot != NULL ) m_pRoot->NeedUpdate();
	}

	bool CPaintManagerUI::SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse)
	{
		ASSERT(pControl!=NULL);
		ASSERT(uElapse>0);
		for( int i = 0; i< m_aTimers.GetSize(); i++ ) {
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if( pTimer->pSender == pControl && pTimer->hWnd == m_hWndPaint && pTimer->nLocalID == nTimerID ) 
			{
				if( pTimer->bKilled == true ) 
				{
					if(m_pWindow)
					{
						if(m_pWindow->SetTimer(uElapse, pTimer))
						{
							pTimer->bKilled = false;
							return true;
						}
					}
#ifdef DUILIB_WIN32
					else //需要兼容以前的某些场景
					{
						if( ::SetTimer(m_hWndPaint, pTimer->uWinTimer, uElapse, NULL) ) 
						{
							pTimer->bKilled = false;
							return true;
						}
					}
#endif
					return false;
				}
				return false;
			}
		}

		m_uTimerID = (++m_uTimerID) % 0xF0; //0xf1-0xfe特殊用途

		TIMERINFO* pTimer = new TIMERINFO;
		if( pTimer == NULL ) return false;
		pTimer->hWnd = m_hWndPaint;
		pTimer->pWindow = m_pWindow;
		pTimer->pSender = pControl;
		pTimer->nLocalID = nTimerID;
		pTimer->uWinTimer = m_uTimerID;
		pTimer->bKilled = false;

		if(m_pWindow)
		{
			if(!m_pWindow->SetTimer(uElapse, pTimer)) { delete pTimer; return false; };
		}
#ifdef DUILIB_WIN32
		else//需要兼容以前的某些场景
		{
			if( !::SetTimer(m_hWndPaint, m_uTimerID, uElapse, NULL) ) { delete pTimer; return false; };
		}
#endif
		return m_aTimers.Add(pTimer);
	}

	bool CPaintManagerUI::KillTimer(CControlUI* pControl, UINT nTimerID)
	{
		ASSERT(pControl!=NULL);
		if(nTimerID > 0)
		{
			for( int i = 0; i< m_aTimers.GetSize(); i++ ) 
			{
				TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
				if( pTimer->pSender == pControl
					&& pTimer->hWnd == m_hWndPaint
					&& pTimer->nLocalID == nTimerID )
				{
					if( pTimer->bKilled == false ) 
					{
						if( CPlatform::IsWindow(m_hWndPaint) ) 
						{
							if(m_pWindow) m_pWindow->KillTimer(pTimer);
#ifdef DUILIB_WIN32
							else ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
#endif
						}
						pTimer->bKilled = true;
						return true;
					}
				}
			}
		}
		else
		{
			int count = m_aTimers.GetSize();
			for( int i = 0, j = 0; i < count; i++ ) 
			{
				TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i - j]);
				if( pTimer->pSender == pControl && pTimer->hWnd == m_hWndPaint ) 
				{
					if( pTimer->bKilled == false ) 
					{
						if(m_pWindow) m_pWindow->KillTimer(pTimer);
#ifdef DUILIB_WIN32
						else ::KillTimer(pTimer->hWnd, pTimer->uWinTimer);
#endif
					}
					delete pTimer;
					m_aTimers.Remove(i - j);
					j++;
				}
			}
		}
		return false;
	}

	void CPaintManagerUI::RemoveAllTimers()
	{
		for( int i = 0; i < m_aTimers.GetSize(); i++ ) 
		{
			TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if( pTimer->hWnd == m_hWndPaint ) 
			{
				if( pTimer->bKilled == false ) 
				{
					if( CPlatform::IsWindow(m_hWndPaint) ) 
					{
						if(m_pWindow) m_pWindow->KillTimer(pTimer);
#ifdef DUILIB_WIN32
						else ::KillTimer(m_hWndPaint, pTimer->uWinTimer);
#endif
					}
				}
				delete pTimer;
			}
		}

		m_aTimers.Empty();
	}

	bool CPaintManagerUI::IsPainting()
	{
		return m_bIsPainting;
	}

	void CPaintManagerUI::SetPainting(bool bIsPainting)
	{
		m_bIsPainting = bIsPainting;
	}

	bool CPaintManagerUI::SetNextTabControl(bool bForward)
	{
		// If we're in the process of restructuring the layout we can delay the
		// focus calulation until the next repaint.
		if( m_bUpdateNeeded && bForward ) {
			m_bFocusNeeded = true;
			InvalidateRect(m_hWndPaint, NULL, FALSE);
			return true;
		}
		// Find next/previous tabbable control
		FINDTABINFO info1 = { 0 };
		info1.pFocus = m_pFocus;
		info1.bForward = bForward;
		CControlUI* pControl = m_pRoot->FindControl(__FindControlFromTab, &info1, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
		if( pControl == NULL ) {  
			if( bForward ) {
				// Wrap around
				FINDTABINFO info2 = { 0 };
				info2.pFocus = bForward ? NULL : info1.pLast;
				info2.bForward = bForward;
				pControl = m_pRoot->FindControl(__FindControlFromTab, &info2, UIFIND_VISIBLE | UIFIND_ENABLED | UIFIND_ME_FIRST);
			}
			else {
				pControl = info1.pLast;
			}
		}
		if( pControl != NULL ) SetFocus(pControl);
		m_bFocusNeeded = false;
		return true;
	}

	bool CPaintManagerUI::AddNotifier(INotifyUI* pNotifier)
	{
		ASSERT(m_aNotifiers.Find(pNotifier)<0);
		return m_aNotifiers.Add(pNotifier);
	}

	bool CPaintManagerUI::RemoveNotifier(INotifyUI* pNotifier)
	{
		for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
			if( static_cast<INotifyUI*>(m_aNotifiers[i]) == pNotifier ) {
				return m_aNotifiers.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddPreMessageFilter(IMessageFilterUI* pFilter)
	{
		ASSERT(m_aPreMessageFilters.Find(pFilter)<0);
		return m_aPreMessageFilters.Add(pFilter);
	}

	bool CPaintManagerUI::RemovePreMessageFilter(IMessageFilterUI* pFilter)
	{
		for( int i = 0; i < m_aPreMessageFilters.GetSize(); i++ ) {
			if( static_cast<IMessageFilterUI*>(m_aPreMessageFilters[i]) == pFilter ) {
				return m_aPreMessageFilters.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddMessageFilter(IMessageFilterUI* pFilter)
	{
		ASSERT(m_aMessageFilters.Find(pFilter)<0);
		return m_aMessageFilters.Add(pFilter);
	}

	bool CPaintManagerUI::RemoveMessageFilter(IMessageFilterUI* pFilter)
	{
		for( int i = 0; i < m_aMessageFilters.GetSize(); i++ ) {
			if( static_cast<IMessageFilterUI*>(m_aMessageFilters[i]) == pFilter ) {
				return m_aMessageFilters.Remove(i);
			}
		}
		return false;
	}

	int CPaintManagerUI::GetPostPaintCount() const
	{
		return m_aPostPaintControls.GetSize();
	}

	bool CPaintManagerUI::IsPostPaint(CControlUI* pControl)
	{
		return m_aPostPaintControls.Find(pControl) >= 0;
	}

	bool CPaintManagerUI::AddPostPaint(CControlUI* pControl)
	{
		ASSERT(m_aPostPaintControls.Find(pControl) < 0);
		return m_aPostPaintControls.Add(pControl);
	}

	bool CPaintManagerUI::RemovePostPaint(CControlUI* pControl)
	{
		for( int i = 0; i < m_aPostPaintControls.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aPostPaintControls[i]) == pControl ) {
				return m_aPostPaintControls.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::SetPostPaintIndex(CControlUI* pControl, int iIndex)
	{
		RemovePostPaint(pControl);
		return m_aPostPaintControls.InsertAt(iIndex, pControl);
	}

#ifdef DUILIB_WIN32
	int CPaintManagerUI::GetNativeWindowCount() const
	{
		return m_aNativeWindow.GetSize();
	}

	bool CPaintManagerUI::AddNativeWindow(CControlUI* pControl, HWND hChildWnd)
	{
		if (pControl == NULL || hChildWnd == NULL) return false;

		RECT rcChildWnd = GetNativeWindowRect(hChildWnd);
		Invalidate(rcChildWnd);

		if (m_aNativeWindow.Find(hChildWnd) >= 0) return false;
		if (m_aNativeWindow.Add(hChildWnd)) {
			m_aNativeWindowControl.Add(pControl);
			return true;
		}
		return false;
	}

	bool CPaintManagerUI::RemoveNativeWindow(HWND hChildWnd)
	{
		for( int i = 0; i < m_aNativeWindow.GetSize(); i++ ) {
			if( static_cast<HWND>(m_aNativeWindow[i]) == hChildWnd ) {
				if( m_aNativeWindow.Remove(i) ) {
					m_aNativeWindowControl.Remove(i);
					return true;
				}
				return false;
			}
		}
		return false;
	}

	RECT CPaintManagerUI::GetNativeWindowRect(HWND hChildWnd)
	{
		RECT rcChildWnd;
		::GetWindowRect(hChildWnd, &rcChildWnd);
		::ScreenToClient(m_hWndPaint, (LPPOINT)(&rcChildWnd));
		::ScreenToClient(m_hWndPaint, (LPPOINT)(&rcChildWnd)+1);
		return rcChildWnd;
	}
#endif //#ifdef DUILIB_WIN32

	void CPaintManagerUI::AddDelayedCleanup(CControlUI* pControl)
	{
		if (pControl == NULL) return;
		pControl->SetManager(this, NULL, false);
		m_aDelayedCleanup.Add(pControl);
		PostAsyncNotify();
	}

	void CPaintManagerUI::AddMouseLeaveNeeded(CControlUI* pControl)
	{
		if (pControl == NULL) return;
		for( int i = 0; i < m_aNeedMouseLeaveNeeded.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aNeedMouseLeaveNeeded[i]) == pControl ) {
				return;
			}
		}
		m_aNeedMouseLeaveNeeded.Add(pControl);
	}

	bool CPaintManagerUI::RemoveMouseLeaveNeeded(CControlUI* pControl)
	{
		if (pControl == NULL) return false;
		for( int i = 0; i < m_aNeedMouseLeaveNeeded.GetSize(); i++ ) {
			if( static_cast<CControlUI*>(m_aNeedMouseLeaveNeeded[i]) == pControl ) {
				return m_aNeedMouseLeaveNeeded.Remove(i);
			}
		}
		return false;
	}

	void CPaintManagerUI::SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/, bool bAsync /*= false*/)
	{
		TNotifyUI Msg;
		Msg.pSender = pControl;
		Msg.sType = pstrMessage;
		Msg.wParam = wParam;
		Msg.lParam = lParam;
		SendNotify(Msg, bAsync);
	}

	void CPaintManagerUI::SendNotify(TNotifyUI& Msg, bool bAsync /*= false*/)
	{
		Msg.ptMouse = m_ptLastMousePos;
		Msg.dwTimestamp = CPlatform::GetTickCount();
		if( m_bUsedVirtualWnd )
		{
			Msg.sVirtualWnd = Msg.pSender->GetVirtualWnd();
		}

		if( !bAsync ) {
			// Send to all listeners
			if( Msg.pSender != NULL ) {
				if( Msg.pSender->OnNotify ) Msg.pSender->OnNotify(&Msg);
				if(!Msg.pSender->m_asOnNotify.IsEmpty()) 
					ExecuteScript(Msg.pSender->m_asOnNotify, Msg.pSender, &Msg);
			}
			for( int i = 0; i < m_aNotifiers.GetSize(); i++ ) {
				static_cast<INotifyUI*>(m_aNotifiers[i])->Notify(Msg);
			}
		}
		else 
		{
			//CDuiInnerLock lock(&m_lockAsyncNotify);//add by liq99

			TNotifyUI *pMsg = new TNotifyUI;
			pMsg->pSender = Msg.pSender;
			pMsg->sType = Msg.sType;
			pMsg->wParam = Msg.wParam;
			pMsg->lParam = Msg.lParam;
			pMsg->ptMouse = Msg.ptMouse;
			pMsg->dwTimestamp = Msg.dwTimestamp;
			m_aAsyncNotify.Add(pMsg);

			PostAsyncNotify();
		}
	}

	bool CPaintManagerUI::IsForceUseSharedRes() const
	{
		return m_bForceUseSharedRes;
	}

	void CPaintManagerUI::SetForceUseSharedRes(bool bForce)
	{
		m_bForceUseSharedRes = bForce;
	}

	DWORD CPaintManagerUI::GetDefaultDisabledColor() const
	{
		return m_ResInfo.m_dwDefaultDisabledColor;
	}

	void CPaintManagerUI::SetDefaultDisabledColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultDisabledColor == m_SharedResInfo.m_dwDefaultDisabledColor)
				m_ResInfo.m_dwDefaultDisabledColor = dwColor;
			m_SharedResInfo.m_dwDefaultDisabledColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultDisabledColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultFontColor() const
	{
		return m_ResInfo.m_dwDefaultFontColor;
	}

	void CPaintManagerUI::SetDefaultFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultFontColor == m_SharedResInfo.m_dwDefaultFontColor)
				m_ResInfo.m_dwDefaultFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultLinkFontColor() const
	{
		return m_ResInfo.m_dwDefaultLinkFontColor;
	}

	void CPaintManagerUI::SetDefaultLinkFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultLinkFontColor == m_SharedResInfo.m_dwDefaultLinkFontColor)
				m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultLinkFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultLinkFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultLinkHoverFontColor() const
	{
		return m_ResInfo.m_dwDefaultLinkHoverFontColor;
	}

	void CPaintManagerUI::SetDefaultLinkHoverFontColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultLinkHoverFontColor == m_SharedResInfo.m_dwDefaultLinkHoverFontColor)
				m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
			m_SharedResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultLinkHoverFontColor = dwColor;
		}
	}

	DWORD CPaintManagerUI::GetDefaultSelectedBkColor() const
	{
		return m_ResInfo.m_dwDefaultSelectedBkColor;
	}

	void CPaintManagerUI::SetDefaultSelectedBkColor(DWORD dwColor, bool bShared)
	{
		if (bShared)
		{
			if (m_ResInfo.m_dwDefaultSelectedBkColor == m_SharedResInfo.m_dwDefaultSelectedBkColor)
				m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
			m_SharedResInfo.m_dwDefaultSelectedBkColor = dwColor;
		}
		else
		{
			m_ResInfo.m_dwDefaultSelectedBkColor = dwColor;
		}
	}

	UIFont* CPaintManagerUI::GetDefaultFontInfo()
	{
		if (m_ResInfo.m_DefaultFontInfo->GetFontName().IsEmpty())
		{
			return m_SharedResInfo.m_DefaultFontInfo;
		}
		else
		{
			return m_ResInfo.m_DefaultFontInfo;
		}
	}

	void CPaintManagerUI::SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared)
	{
		if (bShared)
		{
			m_SharedResInfo.m_DefaultFontInfo->CreateFont(this, -1, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);
		}
		else
		{
			m_ResInfo.m_DefaultFontInfo->CreateFont(this, -1, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);
		}
	}

	DWORD CPaintManagerUI::GetCustomFontCount(bool bShared) const
	{
		if (bShared)
			return m_SharedResInfo.m_CustomFonts.GetSize();
		else
			return m_ResInfo.m_CustomFonts.GetSize();
	}

	UIFont* CPaintManagerUI::AddFont(int id, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bShared)
	{
		UIFont *pFontInfo = UIGlobal::CreateFont();
		pFontInfo->CreateFont(this, id, pStrFontName, nSize, bBold, bUnderline, bItalic, true, bShared);

		CDuiString idBuffer;
		idBuffer.Format(_T("%d"), id);
		if (bShared || m_bForceUseSharedRes)
		{
			UIFont* pOldFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
			if (pOldFontInfo)
			{
				pOldFontInfo->Release();
				m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
			}

			if( !m_SharedResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo) ) 
			{
				pFontInfo->Release();
				return NULL;
			}
		}
		else
		{
			UIFont* pOldFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
			if (pOldFontInfo)
			{
				pOldFontInfo->Release();
				m_ResInfo.m_CustomFonts.Remove(idBuffer);
			}

			if( !m_ResInfo.m_CustomFonts.Insert(idBuffer, pFontInfo) ) 
			{
				pFontInfo->Release();
				return NULL;
			}
		}

		return pFontInfo;
	}

	void CPaintManagerUI::AddFontArray(LPCTSTR pstrPath) 
	{
#ifdef WIN32
		CUIFile f;
		if(!f.LoadFile(pstrPath)) return;

		DWORD nFonts;
		HANDLE hFont = ::AddFontMemResourceEx((LPVOID)f.GetData(), f.GetSize(), NULL, &nFonts);
		m_aFonts.Add(hFont);
#endif
	}

	UIFont* CPaintManagerUI::GetFont(int id)
	{
		if (id < 0) return GetDefaultFontInfo();

		CDuiString idBuffer;
		idBuffer.Format(_T("%d"), id);
		UIFont* pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
		if( !pFontInfo ) pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
		if (!pFontInfo) return GetDefaultFontInfo();
		return pFontInfo;
	}

	UIFont* CPaintManagerUI::GetFont(int nIndex, bool bShared)
	{
		UIFont *pFontInfo = NULL;
		if (bShared)
		{
			LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(nIndex);
			pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
		}
		else
		{
			LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(nIndex);
			pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
		}
		return pFontInfo;
	}

	UIFont* CPaintManagerUI::GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
	{
		UIFont* pFontInfo = NULL;
		for( int i = 0; i< m_ResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
				if (pFontInfo && pFontInfo->GetFontName() == pStrFontName && pFontInfo->GetSize() == nSize && 
					pFontInfo->IsBold() == bBold && pFontInfo->IsUnderline() == bUnderline && pFontInfo->IsItalic() == bItalic) 
					return pFontInfo;
			}
		}
		for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
			if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
				pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
				if (pFontInfo && pFontInfo->GetFontName() == pStrFontName && pFontInfo->GetSize() == nSize && 
					pFontInfo->IsBold() == bBold && pFontInfo->IsUnderline() == bUnderline && pFontInfo->IsItalic() == bItalic) 
					return pFontInfo;
			}
		}

		return NULL;
	}

	int CPaintManagerUI::GetFontHeight(int id)
	{
		return GetFont(id)->GetHeight(this);
	}

	void CPaintManagerUI::RemoveFont(UIFont *uiFont, bool bShared)
	{
		UIFont* pFontInfo = NULL;
		if (bShared)
		{
			for( int i = 0; i < m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) 
			{
				if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) 
				{
					pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key));
					if (pFontInfo && pFontInfo == uiFont) 
					{
						pFontInfo->Release();
						m_SharedResInfo.m_CustomFonts.Remove(key);
						return;
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < m_ResInfo.m_CustomFonts.GetSize(); i++ ) 
			{
				if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) 
				{
					pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key));
					if (pFontInfo && pFontInfo == uiFont) 
					{
						pFontInfo->Release();
						m_ResInfo.m_CustomFonts.Remove(key);
						return;
					}
				}
			}
		}
	}

	void CPaintManagerUI::RemoveFont(int id, bool bShared)
	{
		CDuiString idBuffer;
		idBuffer.Format(_T("%d"), id);

		UIFont* pFontInfo = NULL;
		if (bShared)
		{
			pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(idBuffer));
			if (pFontInfo)
			{
				pFontInfo->Release();
				m_SharedResInfo.m_CustomFonts.Remove(idBuffer);
			}
		}
		else
		{
			pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(idBuffer));
			if (pFontInfo)
			{
				pFontInfo->Release();
				m_ResInfo.m_CustomFonts.Remove(idBuffer);
			}
		}
	}

	void CPaintManagerUI::RemoveAllFonts(bool bShared)
	{
		UIFont* pFontInfo;
		if (bShared)
		{
			for( int i = 0; i< m_SharedResInfo.m_CustomFonts.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_CustomFonts.GetAt(i)) {
					pFontInfo = static_cast<UIFont*>(m_SharedResInfo.m_CustomFonts.Find(key, false));
					if (pFontInfo) {
						pFontInfo->Release();
					}
				}
			}
			m_SharedResInfo.m_CustomFonts.RemoveAll();
		}
		else
		{
			for( int i = 0; i< m_ResInfo.m_CustomFonts.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_CustomFonts.GetAt(i)) {
					pFontInfo = static_cast<UIFont*>(m_ResInfo.m_CustomFonts.Find(key, false));
					if (pFontInfo) {
						pFontInfo->Release();
					}
				}
			}
			m_ResInfo.m_CustomFonts.RemoveAll();
		}
	}

	UIFont* CPaintManagerUI::CloneFont(int id)
	{
		return GetFont(id)->Clone(this);
	}

	const UIImage* CPaintManagerUI::GetImage(LPCTSTR bitmap)
	{
		UIImage* data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
		if( !data ) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
		return data;
	}

	const UIImage* CPaintManagerUI::GetImageEx(LPCTSTR bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, bool bUseHSL, HINSTANCE instance)
	{
		CDuiString sImageName = bitmap;
		if(width !=0 || height != 0 || fillcolor != 0)
		{
			//如果有定义svg的宽度、高度、填充色，把他们一起放进来作为关键字。
			sImageName.Format(_T("%s-%d-%d-%08X"), bitmap, width, height, fillcolor);
		}

		const UIImage* data = GetImage(sImageName);
		if( !data ) {
			if( AddImage(bitmap, type, mask, width, height, fillcolor, bUseHSL, false, instance) ) {
				if (m_bForceUseSharedRes) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				else data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName)); 
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::GetImageExX(const TDrawInfo *pDrawInfo, HINSTANCE instance)
	{
		CDuiString sImageName = pDrawInfo->sImageName;
		if(pDrawInfo->width !=0 || pDrawInfo->height != 0 || pDrawInfo->fillcolor != 0)
		{
			//如果有定义svg的宽度、高度、填充色，把他们一起放进来作为关键字。
			sImageName.Format(_T("%s-%d-%d-%08X"), pDrawInfo->sImageName.GetData(), pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor);		
		}

		const UIImage* data = GetImage(sImageName);
		if( !data ) {
			if( AddImageX(pDrawInfo, false, instance) ) {
				if (m_bForceUseSharedRes) data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				else data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName)); 
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::AddImageX(const TDrawInfo *pDrawInfo, bool bShared, HINSTANCE instance)
	{
		if(pDrawInfo->sImageName.IsEmpty()) return NULL;

		CDuiString sImageName = pDrawInfo->sImageName;
		if(pDrawInfo->width !=0 || pDrawInfo->height != 0 || pDrawInfo->fillcolor != 0)
		{
			//如果有定义svg的宽度、高度、填充色，把他们一起放进来作为关键字。
			sImageName.Format(_T("%s-%d-%d-%08X"), pDrawInfo->sImageName.GetData(), pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor);
		}

		UIImage* data = UIGlobal::CreateImage();
		if(!data->LoadImage(pDrawInfo, this, instance))
		{
			data->Release();
			return NULL;
		}	

		data->bUseHSL = pDrawInfo->bHSL;
		data->sResType = pDrawInfo->sResType;
		data->dwMask = pDrawInfo->dwMask;

		if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
		if (data)
		{
			if (bShared || m_bForceUseSharedRes)
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_SharedResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_SharedResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
			else
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_ResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_ResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
		}

		return data;
	}

	const UIImage* CPaintManagerUI::AddImage(LPCTSTR bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, bool bUseHSL, bool bShared, HINSTANCE instance)
	{
		if( bitmap == NULL || bitmap[0] == _T('\0') ) return NULL;

		CDuiString sImageName = bitmap;
		if(width !=0 || height != 0 || fillcolor != 0)
		{
			//如果有定义svg的宽度、高度、填充色，把他们一起放进来作为关键字。
			sImageName.Format(_T("%s-%d-%d-%08X"), bitmap, width, height, fillcolor);
		}

		UIImage* data = UIGlobal::CreateImage();
		if( type != NULL && _tcslen(type) > 0 && isdigit(*bitmap)) 
		{
			LPTSTR pstr = NULL;
			int iIndex = _tcstol(bitmap, &pstr, 10);
			if(!data->LoadImage(iIndex, type, mask, width, height, fillcolor, this, instance))
			{
				data->Release();
				return NULL;
			}	
		}
		else 
		{
			if(!data->LoadImage(bitmap, NULL, mask, width, height, fillcolor, this, instance))
			{
				data->Release();
				return NULL;
			}
		}

		data->bUseHSL = bUseHSL;
		if( type != NULL ) data->sResType = type;
		data->dwMask = mask;
		if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
		if (data)
		{
			if (bShared || m_bForceUseSharedRes)
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_SharedResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_SharedResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release();
				}
			}
			else
			{
				UIImage* pOldImageInfo = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(sImageName));
				if (pOldImageInfo)
				{
					pOldImageInfo->Release(); 
					m_ResInfo.m_ImageHash.Remove(sImageName);
				}

				if( !m_ResInfo.m_ImageHash.Insert(sImageName, data) ) 
				{
					data->Release(); 
				}
			}
		}

		return data;
	}

#ifdef DUILIB_WIN32
	const UIImage* CPaintManagerUI::AddImage(LPCTSTR bitmap, HBITMAP hBitmap, bool bAlpha, bool bShared)
	{
		// 因无法确定外部HBITMAP格式，不能使用hsl调整
		if( bitmap == NULL || bitmap[0] == _T('\0') ) return NULL;
		if( hBitmap == NULL ) return NULL;

		UIImage* data = UIGlobal::CreateImage();
		data->CreateImage(hBitmap, bAlpha);

		if (bShared || m_bForceUseSharedRes)
		{
			if( !m_SharedResInfo.m_ImageHash.Insert(bitmap, data) ) {
				data->Release(); 
			}
		}
		else
		{
			if( !m_ResInfo.m_ImageHash.Insert(bitmap, data) ) {
				data->Release(); 
			}
		}

		return data;
	}
#endif

	void CPaintManagerUI::RemoveImage(LPCTSTR bitmap, bool bShared)
	{
		UIImage* data = NULL;
		if (bShared) 
		{
			data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
			if (data)
			{
				data->Release(); 
				m_SharedResInfo.m_ImageHash.Remove(bitmap);
			}
		}
		else
		{
			data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
			if (data)
			{
				data->Release(); 
				m_ResInfo.m_ImageHash.Remove(bitmap);
			}
		}
	}

	void CPaintManagerUI::RemoveAllImages(bool bShared)
	{
		if (bShared)
		{
			UIImage* data;
			for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) {
					data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key, false));
					if (data) {
						data->Release(); 
					}
				}
			}
			m_SharedResInfo.m_ImageHash.RemoveAll();
		}
		else
		{
			UIImage* data;
			for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i)) {
					data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(key, false));
					if (data) 
					{
						data->Release(); 
					}
				}
			}
			m_ResInfo.m_ImageHash.RemoveAll();
		}
	}

	void CPaintManagerUI::AdjustSharedImagesHSL()
	{
		UIImage* data;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR key = m_SharedResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(key));
				if( data && (data->bUseHSL || IsForceHSL()) )
				{
					data->AdjustHslImage(m_bUseHSL, m_H, m_S, m_L);
				}
			}
		}
	}

	void CPaintManagerUI::AdjustImagesHSL()
	{
		UIImage* data;
		for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_ResInfo.m_ImageHash.GetAt(i)) {
				data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(key));
				if( data && (data->bUseHSL || IsForceHSL()) )
				{
					data->AdjustHslImage(m_bUseHSL, m_H, m_S, m_L);
				}
			}
		}
		Invalidate();
	}

	void CPaintManagerUI::PostAsyncNotify()
	{
		if (!m_bAsyncNotifyPosted) {
			CPlatform::PostMessage(m_hWndPaint, WM_APP + 1, 0, 0L);
			m_bAsyncNotifyPosted = true;
		}
	}
	void CPaintManagerUI::ReloadSharedImages()
	{
		UIImage* data = NULL;
		for( int i = 0; i< m_SharedResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR bitmap = m_SharedResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_SharedResInfo.m_ImageHash.Find(bitmap));
				if( data != NULL ) 
				{
					if( !data->sResType.IsEmpty() ) 
					{
						if( isdigit(*bitmap) ) 
						{
							LPTSTR pstr = NULL;
							int iIndex = _tcstol(bitmap, &pstr, 10);
							data->LoadImage(iIndex, data->sResType.GetData(), data->dwMask);
						}
					}
					else 
					{
						data->LoadImage(bitmap, NULL, data->dwMask);
					}

					if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
				}
			}
		}
	}

	void CPaintManagerUI::ReloadImages()
	{
		RemoveAllDrawInfos();

		UIImage* data = NULL;
		for( int i = 0; i< m_ResInfo.m_ImageHash.GetSize(); i++ ) 
		{
			if(LPCTSTR bitmap = m_ResInfo.m_ImageHash.GetAt(i)) 
			{
				data = static_cast<UIImage*>(m_ResInfo.m_ImageHash.Find(bitmap));
				if( data != NULL ) {
					if( !data->sResType.IsEmpty() ) 
					{
						if( isdigit(*bitmap) ) 
						{
							LPTSTR pstr = NULL;
							int iIndex = _tcstol(bitmap, &pstr, 10);
							data->LoadImage(iIndex, data->sResType.GetData(), data->dwMask);
						}
					}
					else 
					{
						data->LoadImage(bitmap, NULL, data->dwMask);
					}

					if( m_bUseHSL ) data->AdjustHslImage(true, m_H, m_S, m_L);
				}
			}
		}

		if( m_pRoot ) m_pRoot->Invalidate();
	}

	const TDrawInfo* CPaintManagerUI::GetDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sStrImage = pStrImage;
		CDuiString sStrModify = pStrModify;
		CDuiString sKey = sStrImage + sStrModify;
		TDrawInfo* pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
		if(pDrawInfo == NULL && !sKey.IsEmpty()) {
			pDrawInfo = new TDrawInfo();
			pDrawInfo->Parse(pStrImage, pStrModify,this);
			m_ResInfo.m_DrawInfoHash.Insert(sKey, pDrawInfo);
		}
		return pDrawInfo;
	}

	void CPaintManagerUI::RemoveDrawInfo(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sStrImage = pStrImage;
		CDuiString sStrModify = pStrModify;
		CDuiString sKey = sStrImage + sStrModify;
		TDrawInfo* pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(sKey));
		if(pDrawInfo != NULL) {
			m_ResInfo.m_DrawInfoHash.Remove(sKey);
			delete pDrawInfo;
			pDrawInfo = NULL;
		}
	}

	void CPaintManagerUI::RemoveAllDrawInfos()
	{
		TDrawInfo* pDrawInfo = NULL;
		for( int i = 0; i< m_ResInfo.m_DrawInfoHash.GetSize(); i++ ) {
			LPCTSTR key = m_ResInfo.m_DrawInfoHash.GetAt(i);
			if(key != NULL) {
				pDrawInfo = static_cast<TDrawInfo*>(m_ResInfo.m_DrawInfoHash.Find(key, false));
				if (pDrawInfo) {
					delete pDrawInfo;
					pDrawInfo = NULL;
				}
			}
		}
		m_ResInfo.m_DrawInfoHash.RemoveAll();
	}

	void CPaintManagerUI::AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList, bool bShared)
	{
		if (bShared || m_bForceUseSharedRes)
		{
			CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
			if (pDefaultAttr != NULL)
			{
				CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
				if (pOldDefaultAttr) delete pOldDefaultAttr;
			}
		}
		else
		{
			CDuiString* pDefaultAttr = new CDuiString(pStrControlAttrList);
			if (pDefaultAttr != NULL)
			{
				CDuiString* pOldDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Set(pStrControlName, (LPVOID)pDefaultAttr));
				if (pOldDefaultAttr) delete pOldDefaultAttr;
			}
		}
	}

	LPCTSTR CPaintManagerUI::GetDefaultAttributeList(LPCTSTR pStrControlName) const
	{
		CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(pStrControlName));
		if( !pDefaultAttr ) pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(pStrControlName));
		if (pDefaultAttr) return pDefaultAttr->GetData();
		return NULL;
	}

	bool CPaintManagerUI::RemoveDefaultAttributeList(LPCTSTR pStrControlName, bool bShared)
	{
		if (bShared)
		{
			CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(pStrControlName));
			if( !pDefaultAttr ) return false;

			delete pDefaultAttr;
			return m_SharedResInfo.m_AttrHash.Remove(pStrControlName);
		}
		else
		{
			CDuiString* pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(pStrControlName));
			if( !pDefaultAttr ) return false;

			delete pDefaultAttr;
			return m_ResInfo.m_AttrHash.Remove(pStrControlName);
		}
	}

	void CPaintManagerUI::RemoveAllDefaultAttributeList(bool bShared)
	{
		if (bShared)
		{
			CDuiString* pDefaultAttr;
			for( int i = 0; i< m_SharedResInfo.m_AttrHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_AttrHash.GetAt(i)) {
					pDefaultAttr = static_cast<CDuiString*>(m_SharedResInfo.m_AttrHash.Find(key));
					if (pDefaultAttr) delete pDefaultAttr;
				}
			}
			m_SharedResInfo.m_AttrHash.RemoveAll();
		}
		else
		{
			CDuiString* pDefaultAttr;
			for( int i = 0; i< m_ResInfo.m_AttrHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_AttrHash.GetAt(i)) {
					pDefaultAttr = static_cast<CDuiString*>(m_ResInfo.m_AttrHash.Find(key));
					if (pDefaultAttr) delete pDefaultAttr;
				}
			}
			m_ResInfo.m_AttrHash.RemoveAll();
		}
	}

	void CPaintManagerUI::AddWindowCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') || pstrAttr == NULL || pstrAttr[0] == _T('\0') ) return;
		CDuiString* pCostomAttr = new CDuiString(pstrAttr);
		if (pCostomAttr != NULL) {
			if (m_mWindowCustomAttrHash.Find(pstrName) == NULL)
				m_mWindowCustomAttrHash.Set(pstrName, (LPVOID)pCostomAttr);
			else
				delete pCostomAttr;
		}
	}

	LPCTSTR CPaintManagerUI::GetWindowCustomAttribute(LPCTSTR pstrName) const
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(pstrName));
		if( pCostomAttr ) return pCostomAttr->GetData();
		return NULL;
	}

	bool CPaintManagerUI::RemoveWindowCustomAttribute(LPCTSTR pstrName)
	{
		if( pstrName == NULL || pstrName[0] == _T('\0') ) return NULL;
		CDuiString* pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(pstrName));
		if( !pCostomAttr ) return false;

		delete pCostomAttr;
		return m_mWindowCustomAttrHash.Remove(pstrName);
	}

	void CPaintManagerUI::RemoveAllWindowCustomAttribute()
	{
		CDuiString* pCostomAttr;
		for( int i = 0; i< m_mWindowCustomAttrHash.GetSize(); i++ ) {
			if(LPCTSTR key = m_mWindowCustomAttrHash.GetAt(i)) {
				pCostomAttr = static_cast<CDuiString*>(m_mWindowCustomAttrHash.Find(key));
				delete pCostomAttr;
			}
		}
		m_mWindowCustomAttrHash.Resize();
	}

	CControlUI* CPaintManagerUI::GetRoot() const
	{
		//ASSERT(m_pRoot);
		return m_pRoot;
	}

	CControlUI* CPaintManagerUI::FindControl(POINT pt) const
	{
		//ASSERT(m_pRoot);
		if(!m_pRoot) return NULL;
		return m_pRoot->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
	}

	CControlUI* CPaintManagerUI::FindControl(LPCTSTR pstrName) const
	{
		//ASSERT(m_pRoot);
		if(!m_pRoot) return NULL;
		return static_cast<CControlUI*>(m_mNameHash.Find(pstrName));
	}

// 	CControlUI* CPaintManagerUI::FindControl(const CDuiString &strName) const
// 	{
// 		if(!m_pRoot) return NULL;
// 		return static_cast<CControlUI*>(m_mNameHash.Find(strName));
// 	}

	CControlUI* CPaintManagerUI::FindSubControlByPoint(CControlUI* pParent, POINT pt) const
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		return pParent->FindControl(__FindControlFromPoint, &pt, UIFIND_VISIBLE | UIFIND_HITTEST | UIFIND_TOP_FIRST);
	}

	CControlUI* CPaintManagerUI::FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		return pParent->FindControl(__FindControlFromName, (LPVOID)pstrName, UIFIND_ALL);
	}

	CControlUI* CPaintManagerUI::FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex)
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		m_aFoundControls.Resize(iIndex + 1);
		return pParent->FindControl(__FindControlFromClass, (LPVOID)pstrClass, UIFIND_ALL);
	}

	CStdPtrArray* CPaintManagerUI::FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass)
	{
		if( pParent == NULL ) pParent = GetRoot();
		//ASSERT(pParent);
		if(!pParent) return NULL;
		m_aFoundControls.Empty();
		pParent->FindControl(__FindControlsFromClass, (LPVOID)pstrClass, UIFIND_ALL);
		return &m_aFoundControls;
	}

	CStdPtrArray* CPaintManagerUI::FindSubControlsByInterface(CControlUI* pParent, LPCTSTR pstrClass)
	{
		if (pParent == NULL) pParent = GetRoot();
		//ASSERT(pParent);
		if (!pParent) return NULL;
		m_aFoundControls.Empty();
		pParent->FindControl(__FindControlsFromInterface, (LPVOID)pstrClass, UIFIND_ALL);
		return &m_aFoundControls;
	}

	CStdPtrArray* CPaintManagerUI::GetFoundControls()
	{
		return &m_aFoundControls;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromNameHash(CControlUI* pThis, LPVOID pData)
	{
		CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(pData);
		const CDuiString& sName = pThis->GetName();
		if( sName.IsEmpty() ) return NULL;
		// Add this control to the hash list
		pManager->m_mNameHash.Set(sName, pThis);
		return NULL; // Attempt to add all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromCount(CControlUI* /*pThis*/, LPVOID pData)
	{
		int* pnCount = static_cast<int*>(pData);
		(*pnCount)++;
		return NULL;  // Count all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromPoint(CControlUI* pThis, LPVOID pData)
	{
		LPPOINT pPoint = static_cast<LPPOINT>(pData);
		//return ::PtInRect(&pThis->GetPos(), *pPoint) ? pThis : NULL;
		return pThis->GetPos().PtInRect(*pPoint) ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromTab(CControlUI* pThis, LPVOID pData)
	{
		FINDTABINFO* pInfo = static_cast<FINDTABINFO*>(pData);
		if( pInfo->pFocus == pThis ) {
			if( pInfo->bForward ) pInfo->bNextIsIt = true;
			return pInfo->bForward ? NULL : pInfo->pLast;
		}
		if( (pThis->GetControlFlags() & UIFLAG_TABSTOP) == 0 ) return NULL;
		pInfo->pLast = pThis;
		if( pInfo->bNextIsIt ) return pThis;
		if( pInfo->pFocus == NULL ) return pThis;
		return NULL;  // Examine all controls
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromShortcut(CControlUI* pThis, LPVOID pData)
	{
		if( !pThis->IsVisible() ) return NULL; 
		FINDSHORTCUT* pFS = static_cast<FINDSHORTCUT*>(pData);
		if( pFS->ch == toupper(pThis->GetShortcut()) ) pFS->bPickNext = true;
		if( _tcsstr(pThis->GetClass(), _T("LabelUI")) != NULL ) return NULL;   // Labels never get focus!
		return pFS->bPickNext ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromName(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrName = static_cast<LPCTSTR>(pData);
		const CDuiString& sName = pThis->GetName();
		if( sName.IsEmpty() ) return NULL;
		return (_tcsicmp(sName, pstrName) == 0) ? pThis : NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlFromClass(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
		LPCTSTR pType = pThis->GetClass();
		CStdPtrArray* pFoundControls = pThis->GetManager()->GetFoundControls();
		if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 ) {
			int iIndex = -1;
			while( pFoundControls->GetAt(++iIndex) != NULL ) ;
			if( iIndex < pFoundControls->GetSize() ) pFoundControls->SetAt(iIndex, pThis);
		}
		if( pFoundControls->GetAt(pFoundControls->GetSize() - 1) != NULL ) return pThis; 
		return NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlsFromClass(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
		LPCTSTR pType = pThis->GetClass();
		if( _tcscmp(pstrType, _T("*")) == 0 || _tcscmp(pstrType, pType) == 0 ) 
			pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
		return NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlsFromInterface(CControlUI* pThis, LPVOID pData)
	{
		LPCTSTR pstrType = static_cast<LPCTSTR>(pData);
		if (pThis->GetInterface(pstrType) != NULL)
			pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
		return NULL;
	}

	CControlUI* CALLBACK CPaintManagerUI::__FindControlsFromUpdate(CControlUI* pThis, LPVOID pData)
	{
		if( pThis->IsUpdateNeeded() ) {
			pThis->GetManager()->GetFoundControls()->Add((LPVOID)pThis);
			return pThis;
		}
		return NULL;
	}

#ifdef DUILIB_WIN32
	bool CPaintManagerUI::TranslateMessage(const LPMSG pMsg)
	{
		// Pretranslate Message takes care of system-wide messages, such as
		// tabbing and shortcut key-combos. We'll look for all messages for
		// each window and any child control attached.
		UINT uStyle = GetWindowStyle(pMsg->hwnd);
		UINT uChildRes = uStyle & WS_CHILD;	
		LRESULT lRes = 0;
		if (uChildRes != 0)
		{
			HWND hWndParent = ::GetParent(pMsg->hwnd);

			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
			{
				CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);        
				HWND hTempParent = hWndParent;
				while(hTempParent)
				{
					if(pMsg->hwnd == pT->GetPaintWindow() || hTempParent == pT->GetPaintWindow())
					{
						if (pT->TranslateAccelerator(pMsg))
							return true;

						pT->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes);
					}
					hTempParent = GetParent(hTempParent);
				}
			}
		}
		else
		{
			for( int i = 0; i < m_aPreMessages.GetSize(); i++ ) 
			{
				CPaintManagerUI* pT = static_cast<CPaintManagerUI*>(m_aPreMessages[i]);
				if(pMsg->hwnd == pT->GetPaintWindow())
				{
					if (pT->TranslateAccelerator(pMsg))
						return true;

					if( pT->PreMessageHandler(pMsg->message, pMsg->wParam, pMsg->lParam, lRes) ) 
						return true;

					return false;
				}
			}
		}
		return false;
	}

	bool CPaintManagerUI::AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
	{
		ASSERT(m_aTranslateAccelerator.Find(pTranslateAccelerator) < 0);
		return m_aTranslateAccelerator.Add(pTranslateAccelerator);
	}

	bool CPaintManagerUI::RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator)
	{
		for (int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
		{
			if (static_cast<ITranslateAccelerator *>(m_aTranslateAccelerator[i]) == pTranslateAccelerator)
			{
				return m_aTranslateAccelerator.Remove(i);
			}
		}
		return false;
	}

	bool CPaintManagerUI::TranslateAccelerator(LPMSG pMsg)
	{
		for (int i = 0; i < m_aTranslateAccelerator.GetSize(); i++)
		{
			LRESULT lResult = static_cast<ITranslateAccelerator *>(m_aTranslateAccelerator[i])->TranslateAccelerator(pMsg);
			if( lResult == S_OK ) return true;
		}
		return false;
	}
#endif //#ifdef DUILIB_WIN32

	void CPaintManagerUI::UsedVirtualWnd(bool bUsed)
	{
		m_bUsedVirtualWnd = bUsed;
	}

	// 样式管理
	void CPaintManagerUI::AddStyle(LPCTSTR pName, LPCTSTR pDeclarationList, bool bShared)
	{
		CDuiString* pStyle = new CDuiString(pDeclarationList);
		if(!pStyle) return;

		if(bShared || m_bForceUseSharedRes)
		{
			//用于设计器重新打开文件 和 即时更新Style定义。 modify by liqs99
			CDuiString *pOldStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Set(pName, pStyle));
			if(pOldStyle) delete pOldStyle;
		}
		else
		{
			//用于设计器重新打开文件 和 即时更新Style定义。 modify by liqs99
			CDuiString *pOldStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Set(pName, pStyle));
			if(pOldStyle) delete pOldStyle;
		}
	}

	LPCTSTR CPaintManagerUI::GetStyle(LPCTSTR pName) const
	{
		CDuiString* pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(pName));
		if( !pStyle ) pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(pName));
		if( pStyle ) return pStyle->GetData();
		else return NULL;
	}

	BOOL CPaintManagerUI::RemoveStyle(LPCTSTR pName, bool bShared)
	{
		CDuiString* pStyle = NULL;
		if (bShared) 
		{
			pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(pName));
			if (pStyle)
			{
				delete pStyle;
				m_SharedResInfo.m_StyleHash.Remove(pName);
			}
		}
		else
		{
			pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(pName));
			if (pStyle)
			{
				delete pStyle;
				m_ResInfo.m_StyleHash.Remove(pName);
			}
		}
		return true;
	}

	const CStdStringPtrMap& CPaintManagerUI::GetStyles(bool bShared) const
	{
		if(bShared) return m_SharedResInfo.m_StyleHash;
		else return m_ResInfo.m_StyleHash;
	}

	void CPaintManagerUI::RemoveAllStyle(bool bShared)
	{
		if (bShared)
		{
			CDuiString* pStyle;
			for( int i = 0; i< m_SharedResInfo.m_StyleHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_SharedResInfo.m_StyleHash.GetAt(i)) {
					pStyle = static_cast<CDuiString*>(m_SharedResInfo.m_StyleHash.Find(key));
					delete pStyle;
				}
			}
			m_SharedResInfo.m_StyleHash.RemoveAll();
		}
		else
		{
			CDuiString* pStyle;
			for( int i = 0; i< m_ResInfo.m_StyleHash.GetSize(); i++ ) {
				if(LPCTSTR key = m_ResInfo.m_StyleHash.GetAt(i)) {
					pStyle = static_cast<CDuiString*>(m_ResInfo.m_StyleHash.Find(key));
					delete pStyle;
				}
			}
			m_ResInfo.m_StyleHash.RemoveAll();
		}
	}

	const UIImage* CPaintManagerUI::GetImageString(LPCTSTR pStrImage, LPCTSTR pStrModify)
	{
		CDuiString sImageName = pStrImage;
		CDuiString sImageResType = _T("");
		DWORD dwMask = 0;
		CDuiString sItem;
		CDuiString sValue;
		LPTSTR pstr = NULL;

		for( int i = 0; i < 2; ++i) {
			if( i == 1)
				pStrImage = pStrModify;

			if( !pStrImage ) continue;

			while( *pStrImage != _T('\0') ) {
				sItem.Empty();
				sValue.Empty();
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				while( *pStrImage != _T('\0') && *pStrImage != _T('=') && *pStrImage > _T(' ') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sItem += *pStrImage++;
					}
				}
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('=') ) break;
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('\'') ) break;
				while( *pStrImage != _T('\0') && *pStrImage != _T('\'') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sValue += *pStrImage++;
					}
				}
				if( *pStrImage++ != _T('\'') ) break;
				if( !sValue.IsEmpty() ) {
					if( sItem == _T("file") || sItem == _T("res") ) {
						sImageName = sValue;
					}
					else if( sItem == _T("restype") ) {
						sImageResType = sValue;
					}
					else if( sItem == _T("mask") ) 
					{
						if( sValue[0] == _T('#')) dwMask = _tcstoul(sValue.GetData() + 1, &pstr, 16);
						else dwMask = _tcstoul(sValue.GetData(), &pstr, 16);
					}

				}
				if( *pStrImage++ != _T(' ') ) break;
			}
		}
		return GetImageEx(sImageName, sImageResType, dwMask);
	}

	//////////////////////////////////////////////////////////////////////////
	// 脚本
	//////////////////////////////////////////////////////////////////////////
	CREATE_SCRIPT_ENGINE_INSTANCE CPaintManagerUI::m_funCreateScriptEngine = NULL;	//add by liqs99
	DELETE_SCRIPT_ENGINE_INSTANCE CPaintManagerUI::m_funDeleteScriptEngine = NULL;	//add by liqs99
	IScriptManager* CPaintManagerUI::m_pSharedScriptEngine = NULL;					//add by liqs99

	bool CPaintManagerUI::LoadScriptPlugin(CREATE_SCRIPT_ENGINE_INSTANCE pFunCreate, DELETE_SCRIPT_ENGINE_INSTANCE pFunDelete)
	{
		m_funCreateScriptEngine = (CREATE_SCRIPT_ENGINE_INSTANCE)pFunCreate;
		m_funDeleteScriptEngine = (DELETE_SCRIPT_ENGINE_INSTANCE)pFunDelete;
		return true;
	}

	IScriptManager *CPaintManagerUI::GetScriptEngine()
	{
		if(m_funCreateScriptEngine == NULL)	
			return NULL;

		if(m_pSharedScriptEngine == NULL)
			m_pSharedScriptEngine = (*m_funCreateScriptEngine)();
		return m_pSharedScriptEngine;
	}

	void CPaintManagerUI::AddScriptFile(LPCTSTR pstrFileName, LPCTSTR pLanguageType)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine == NULL) return;
		pScriptEngine->AddScriptFile(pstrFileName);
	}

	bool CPaintManagerUI::CompileScript()
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(pScriptEngine == NULL) return false;
		return pScriptEngine->CompileScript();
	}

	bool CPaintManagerUI::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(!pScriptEngine)
			return false;

		int r = 0;
		CAutoScriptContext ctx(pScriptEngine);
		if(!ctx)
			return false;

		r = ctx->SetFunByName(lpszFunName); if( r < 0 ) return false;
		r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
		if(ctx->Execute() == 0)
		{
			return ctx->GetReturnByte() == 1;
		}

		return false;
	}

	bool CPaintManagerUI::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TEventUI *ev)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(!pScriptEngine)
			return false;

		int r = 0;
		CAutoScriptContext ctx(pScriptEngine);
		if(!ctx)
			return false;

		r = ctx->SetFunByName(lpszFunName); if( r < 0 ) return false;
		r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
		r = ctx->SetArgObject(1, ev); if( r < 0 ) return false;
		if(ctx->Execute() == 0)
		{
			return ctx->GetReturnByte() == 1;
		}

		return false;
	}

	bool CPaintManagerUI::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, TNotifyUI *pMsg)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(!pScriptEngine)
			return false;

		int r = 0;
		CAutoScriptContext ctx(pScriptEngine);
		if(!ctx)
			return false;

		r = ctx->SetFunByName(lpszFunName); if( r < 0 ) return false;
		r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
		r = ctx->SetArgObject(1, pMsg); if( r < 0 ) return false;
		if(ctx->Execute() == 0)
		{
			return ctx->GetReturnByte() == 1;
		}

		return false;
	}

	bool CPaintManagerUI::ExecuteScript(LPCTSTR lpszFunName, CControlUI *pControl, UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl)
	{
		IScriptManager *pScriptEngine = GetScriptEngine();
		if(!pScriptEngine)
			return false;

		int r = 0;
		CAutoScriptContext ctx(pScriptEngine);
		if(!ctx)
			return false;

		r = ctx->SetFunByName(lpszFunName); if( r < 0 ) return false;
		r = ctx->SetArgObject(0, pControl); if( r < 0 ) return false;
		r = ctx->SetArgAddress(1, pRender); if( r < 0 ) return false;
		r = ctx->SetArgObject(2, (void *)&rcPaint); if( r < 0 ) return false;
		r = ctx->SetArgObject(3, pStopControl); if( r < 0 ) return false;
		if(ctx->Execute() == 0)
		{
			return ctx->GetReturnByte() == 1;
		}

		return false;
	}


} // namespace DuiLib

