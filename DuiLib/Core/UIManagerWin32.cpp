#include "StdAfx.h"

#ifdef DUILIB_WIN32
#include <zmouse.h>

namespace DuiLib {

	static void GetChildWndRect(HWND hWnd, HWND hChildWnd, RECT& rcChildWnd)
	{
		::GetWindowRect(hChildWnd, &rcChildWnd);

		POINT pt;
		pt.x = rcChildWnd.left;
		pt.y = rcChildWnd.top;
		::ScreenToClient(hWnd, &pt);
		rcChildWnd.left = pt.x;
		rcChildWnd.top = pt.y;

		pt.x = rcChildWnd.right;
		pt.y = rcChildWnd.bottom;
		::ScreenToClient(hWnd, &pt);
		rcChildWnd.right = pt.x;
		rcChildWnd.bottom = pt.y;
	}

// 	static UINT MapKeyState()
// 	{
// 		UINT uState = 0;
// 		if( ::GetKeyState(VK_CONTROL) < 0 ) uState |= MK_CONTROL;
// 		if( ::GetKeyState(VK_LBUTTON) < 0 ) uState |= MK_LBUTTON;
// 		if( ::GetKeyState(VK_RBUTTON) < 0 ) uState |= MK_RBUTTON;
// 		if( ::GetKeyState(VK_SHIFT) < 0 ) uState |= MK_SHIFT;
// 		if( ::GetKeyState(VK_MENU) < 0 ) uState |= MK_ALT;
// 		return uState;
// 	}

	typedef BOOL (__stdcall *PFUNCUPDATELAYEREDWINDOW)(HWND, HDC, POINT*, SIZE*, HDC, POINT*, COLORREF, BLENDFUNCTION*, DWORD);
	PFUNCUPDATELAYEREDWINDOW g_fUpdateLayeredWindow = NULL;

	HPEN m_hUpdateRectPen = NULL;

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	CPaintManagerWin32UI::CPaintManagerWin32UI()
		: m_hDcPaint(NULL)
	{
		if( m_hUpdateRectPen == NULL ) 
		{
			m_hUpdateRectPen = ::CreatePen(PS_SOLID, 1, RGB(220, 0, 0));
			// Boot Windows Common Controls (for the ToolTip control)
			::InitCommonControls();
			::LoadLibrary(_T("msimg32.dll"));
		}
	}

	CPaintManagerWin32UI::~CPaintManagerWin32UI()
	{
		if(m_pLangManager){ delete m_pLangManager; m_pLangManager = NULL; }

		// Delete the control-tree structures
		for( int i = 0; i < m_aDelayedCleanup.GetSize(); i++ ) delete static_cast<CControlUI*>(m_aDelayedCleanup[i]);
		m_aDelayedCleanup.Resize(0);
		for( int i = 0; i < m_aAsyncNotify.GetSize(); i++ ) delete static_cast<TNotifyUI*>(m_aAsyncNotify[i]);
		m_aAsyncNotify.Resize(0);

		m_mNameHash.Resize(0);
		if( m_pRoot != NULL ) delete m_pRoot;

		//delete m_ResInfo.m_DefaultFontInfo;
		RemoveAllFonts();
		RemoveAllImages();
		RemoveAllStyle();
		RemoveAllDefaultAttributeList();
		RemoveAllWindowCustomAttribute();
		RemoveAllOptionGroups();
		RemoveAllTimers();
		RemoveAllDrawInfos();

		if( m_hwndTooltip != NULL ) {
			::DestroyWindow(m_hwndTooltip);
			m_hwndTooltip = NULL;
		}
		if (!m_aFonts.IsEmpty()) {
			for (int i = 0; i < m_aFonts.GetSize();++i)
			{
				HANDLE handle = static_cast<HANDLE>(m_aFonts.GetAt(i));
				::RemoveFontMemResourceEx(handle);
			}
		}
		if( m_hDcPaint != NULL ) ::ReleaseDC(m_hWndPaint, m_hDcPaint);
		m_aPreMessages.Remove(m_aPreMessages.Find(this));


		// DPI管理对象
		if (m_pDPI != NULL) {
			delete m_pDPI;
			m_pDPI = NULL;
		}

		if(m_pRenderEngine)
		{
			//m_pRenderEngine->Release();
			m_pRenderEngine = NULL;
		}
	}

	void CPaintManagerWin32UI::Init(HWND hWnd, LPCTSTR pstrName, CWindowWnd *pWindow)
	{
		ASSERT(::IsWindow(hWnd));
		CPaintManagerUI::Init(hWnd, pstrName, pWindow);
		if( m_hWndPaint != hWnd ) 
		{
			m_hWndPaint = hWnd;
			m_pWindow = pWindow;
			m_hDcPaint = ::GetDC(hWnd);
			m_aPreMessages.Add(this);
			Render()->Init(this, m_hDcPaint);
		}
		SetTargetWnd(hWnd);
		InitDragDrop();
	}

	UIRender *CPaintManagerWin32UI::Render()
	{
		if(!m_pRenderEngine)
		{
			m_pRenderEngine = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
			m_pRenderEngine->Init(this, m_hDcPaint);
		}
		ASSERT(m_pRenderEngine);
		return m_pRenderEngine;
	}


	BOOL CPaintManagerWin32UI::InvalidateRect(UIWND hWnd, const RECT *lpRect, BOOL bErase)
	{
		return ::InvalidateRect(hWnd, lpRect, bErase);
	}

	HDC CPaintManagerWin32UI::GetPaintDC() const
	{
		return m_hDcPaint;
	}

	void CPaintManagerWin32UI::SetLayered(bool bLayered)
	{
		if( m_hWndPaint != NULL && bLayered != m_bLayered ) {
			UINT uStyle = GetWindowStyle(m_hWndPaint);
			if( (uStyle & WS_CHILD) != 0 ) return;
			if( g_fUpdateLayeredWindow == NULL ) {
				HMODULE hUser32 = ::GetModuleHandle(_T("User32.dll"));
				if (hUser32) {
					g_fUpdateLayeredWindow = 
						(PFUNCUPDATELAYEREDWINDOW)::GetProcAddress(hUser32, "UpdateLayeredWindow");
					if( g_fUpdateLayeredWindow == NULL ) return;
				}
			}
			m_bLayered = bLayered;
			if( m_pRoot != NULL ) m_pRoot->NeedUpdate();
			Invalidate();
		}
	}

	void CPaintManagerWin32UI::SetCursor(int nCursor)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(nCursor)));
	}

	bool CPaintManagerWin32UI::InitDragDrop()
	{
		AddRef();

		if(FAILED(RegisterDragDrop(m_hWndPaint, this))) //calls addref
		{
			DWORD dwError = GetLastError();
			return false;
		}
		else Release(); //i decided to AddRef explicitly after new

		FORMATETC ftetc={0};
		ftetc.cfFormat = CF_BITMAP;
		ftetc.dwAspect = DVASPECT_CONTENT;
		ftetc.lindex = -1;
		ftetc.tymed = TYMED_GDI;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_DIB;
		ftetc.tymed = TYMED_HGLOBAL;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_HDROP;
		ftetc.tymed = TYMED_HGLOBAL;
		AddSuportedFormat(ftetc);
		ftetc.cfFormat = CF_ENHMETAFILE;
		ftetc.tymed = TYMED_ENHMF;
		AddSuportedFormat(ftetc);
		return true;
	}
	
	static WORD DIBNumColors(void* pv) 
	{     
		int bits;     
		LPBITMAPINFOHEADER  lpbi;     
		LPBITMAPCOREHEADER  lpbc;      
		lpbi = ((LPBITMAPINFOHEADER)pv);     
		lpbc = ((LPBITMAPCOREHEADER)pv);      
		/*  With the BITMAPINFO format headers, the size of the palette 
		*  is in biClrUsed, whereas in the BITMAPCORE - style headers, it      
		*  is dependent on the bits per pixel ( = 2 raised to the power of      
		*  bits/pixel).
		*/     
		if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
		{         
			if (lpbi->biClrUsed != 0)
				return (WORD)lpbi->biClrUsed;         
			bits = lpbi->biBitCount;     
		}     
		else         
			bits = lpbc->bcBitCount;
		switch (bits)
		{         
		case 1:                 
			return 2;         
		case 4:                 
			return 16;         
		case 8:       
			return 256;
		default:
			/* A 24 bitcount DIB has no color table */                 
			return 0;
		} 
	} 

	//code taken from SEEDIB MSDN sample
	static WORD ColorTableSize(LPVOID lpv)
	{
		LPBITMAPINFOHEADER lpbih = (LPBITMAPINFOHEADER)lpv;

		if (lpbih->biSize != sizeof(BITMAPCOREHEADER))
		{
			if (((LPBITMAPINFOHEADER)(lpbih))->biCompression == BI_BITFIELDS)
				/* Remember that 16/32bpp dibs can still have a color table */
				return (sizeof(DWORD) * 3) + (DIBNumColors (lpbih) * sizeof (RGBQUAD));
			else
				return (WORD)(DIBNumColors (lpbih) * sizeof (RGBQUAD));
		}
		else
			return (WORD)(DIBNumColors (lpbih) * sizeof (RGBTRIPLE));
	}

	bool CPaintManagerWin32UI::OnDropOver(DWORD grfKeyState, LPDWORD pdwEffect)
	{
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(GetPaintWindow(), &point);
		CControlUI* pControl = FindControl(point);
		if( pControl == NULL )
		{
			*pdwEffect = DROPEFFECT_NONE;
			return true;
		}
		if(pControl->IsAcceptDropFile())
		{
			*pdwEffect = DROPEFFECT_COPY;
			return true;
		}
		*pdwEffect = DROPEFFECT_NONE;
		return true;
	}

	bool CPaintManagerWin32UI::OnDrop(FORMATETC* pFmtEtc, STGMEDIUM& medium,DWORD *pdwEffect)
	{
		POINT ptMouse = {0};
		GetCursorPos(&ptMouse);
		::SendMessage(m_hTargetWnd, WM_LBUTTONUP, NULL, MAKELPARAM(ptMouse.x, ptMouse.y));

		::ScreenToClient(GetPaintWindow(), &ptMouse);
		CControlUI* pControl = FindControl(ptMouse);
		if(pControl && pControl->IsAcceptDropFile())
		{
			HDROP hDrop = (HDROP)GlobalLock(medium.hGlobal);
			if(hDrop != NULL)
			{
				UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
				if(cFiles > 0)
				{
					TCHAR szFileName[MAX_PATH];
					DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName)); 
					SendNotify(pControl, DUI_MSGTYPE_ACCEPTFILE, (WPARAM)szFileName, 0);
				}
			}
			GlobalUnlock(medium.hGlobal);
		}

		if(pFmtEtc->cfFormat == CF_DIB && medium.tymed == TYMED_HGLOBAL)
		{
			if(medium.hGlobal != NULL)
			{
				LPBITMAPINFOHEADER  lpbi = (BITMAPINFOHEADER*)GlobalLock(medium.hGlobal);
				if(lpbi != NULL)
				{
					HBITMAP hbm;
					HDC hdc = GetDC(NULL);
					if(hdc != NULL)
					{
						int i = ((BITMAPFILEHEADER *)lpbi)->bfOffBits;
						hbm = CreateDIBitmap(hdc,(LPBITMAPINFOHEADER)lpbi,
							(LONG)CBM_INIT,
							(LPSTR)lpbi + lpbi->biSize + ColorTableSize(lpbi),
							(LPBITMAPINFO)lpbi,DIB_RGB_COLORS);

						::ReleaseDC(NULL,hdc);
					}
					GlobalUnlock(medium.hGlobal);
					if(hbm != NULL)
						hbm = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbm);
					if(hbm != NULL)
						DeleteObject(hbm);
					return true; //release the medium
				}
			}
		}
		if(pFmtEtc->cfFormat == CF_BITMAP && medium.tymed == TYMED_GDI)
		{
			if(medium.hBitmap != NULL)
			{
				HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)medium.hBitmap);
				if(hBmp != NULL)
					DeleteObject(hBmp);
				return false; //don't free the bitmap
			}
		}
		if(pFmtEtc->cfFormat == CF_ENHMETAFILE && medium.tymed == TYMED_ENHMF)
		{
			ENHMETAHEADER emh;
			GetEnhMetaFileHeader(medium.hEnhMetaFile, sizeof(ENHMETAHEADER),&emh);
			RECT rc;//={0,0,EnhMetaHdr.rclBounds.right-EnhMetaHdr.rclBounds.left, EnhMetaHdr.rclBounds.bottom-EnhMetaHdr.rclBounds.top};
			HDC hDC= GetDC(m_hTargetWnd);
			//start code: taken from ENHMETA.EXE MSDN Sample
			//*ALSO NEED to GET the pallete (select and RealizePalette it, but i was too lazy*
			// Get the characteristics of the output device
			float PixelsX = (float)GetDeviceCaps( hDC, HORZRES );
			float PixelsY = (float)GetDeviceCaps( hDC, VERTRES );
			float MMX = (float)GetDeviceCaps( hDC, HORZSIZE );
			float MMY = (float)GetDeviceCaps( hDC, VERTSIZE );
			// Calculate the rect in which to draw the metafile based on the
			// intended size and the current output device resolution
			// Remember that the intended size is given in 0.01mm units, so
			// convert those to device units on the target device
			rc.top = (int)((float)(emh.rclFrame.top) * PixelsY / (MMY*100.0f));
			rc.left = (int)((float)(emh.rclFrame.left) * PixelsX / (MMX*100.0f));
			rc.right = (int)((float)(emh.rclFrame.right) * PixelsX / (MMX*100.0f));
			rc.bottom = (int)((float)(emh.rclFrame.bottom) * PixelsY / (MMY*100.0f));
			//end code: taken from ENHMETA.EXE MSDN Sample

			HDC hdcMem = CreateCompatibleDC(hDC);
			HGDIOBJ hBmpMem = CreateCompatibleBitmap(hDC, emh.rclBounds.right, emh.rclBounds.bottom);
			HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBmpMem);
			PlayEnhMetaFile(hdcMem,medium.hEnhMetaFile,&rc);
			HBITMAP hBmp = (HBITMAP)::SelectObject(hdcMem, hOldBmp);
			DeleteDC(hdcMem);
			ReleaseDC(m_hTargetWnd,hDC);
			hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);
			if(hBmp != NULL)
				DeleteObject(hBmp);
			return true;
		}
		if(pFmtEtc->cfFormat == CF_HDROP && medium.tymed == TYMED_HGLOBAL)
		{
			HDROP hDrop = (HDROP)GlobalLock(medium.hGlobal);
			if(hDrop != NULL)
			{
				TCHAR szFileName[MAX_PATH];
				UINT cFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); 
				if(cFiles > 0)
				{
					DragQueryFile(hDrop, 0, szFileName, sizeof(szFileName)); 
					HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);
					if(hBitmap)
					{
						HBITMAP hBmp = (HBITMAP)SendMessage(m_hTargetWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
						if(hBmp != NULL)
							DeleteObject(hBmp);
					}
				}
				//DragFinish(hDrop); // base class calls ReleaseStgMedium
			}
			GlobalUnlock(medium.hGlobal);
		}
		return true; //let base free the medium
	}

	void CPaintManagerWin32UI::SetCapture()
	{
		::SetCapture(m_hWndPaint);
		m_bMouseCapture = true;
	}

	void CPaintManagerWin32UI::ReleaseCapture()
	{
		::ReleaseCapture();
		m_bMouseCapture = false;
		m_bDragMode = false;
	}

	bool CPaintManagerWin32UI::IsCaptured()
	{
		return m_bMouseCapture;
	}

	bool CPaintManagerWin32UI::PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& /*lRes*/)
	{
		for( int i = 0; i < m_aPreMessageFilters.GetSize(); i++ ) 
		{
			bool bHandled = false;
			LRESULT lResult = static_cast<IMessageFilterUI*>(m_aPreMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
			if( bHandled ) {
				return true;
			}
		}
		switch( uMsg ) {
		case WM_KEYDOWN:
			{
				// Tabbing between controls
				if( wParam == VK_TAB ) {
					if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("RichEditUI")) != NULL ) {
						if( static_cast<CRichEditUI*>(m_pFocus)->IsWantTab() ) return false;
					}
					if( m_pFocus && m_pFocus->IsVisible() && m_pFocus->IsEnabled() && _tcsstr(m_pFocus->GetClass(), _T("WkeWebkitUI")) != NULL ) {
						return false;
					}
					SetNextTabControl(::GetKeyState(VK_SHIFT) >= 0);
					return true;
				}
			}
			break;
		case WM_SYSCHAR:
			{
				// Handle ALT-shortcut key-combinations
				FINDSHORTCUT fs = { 0 };
				fs.ch = toupper((int)wParam);
				CControlUI* pControl = m_pRoot->FindControl(__FindControlFromShortcut, &fs, UIFIND_ENABLED | UIFIND_ME_FIRST | UIFIND_TOP_FIRST);
				if( pControl != NULL ) {
					pControl->SetFocus();
					pControl->Activate();
					return true;
				}
			}
			break;
		case WM_SYSKEYDOWN:
			{
				if( m_pFocus != NULL ) {
					TEventUI event = { 0 };
					event.Type = UIEVENT_SYSKEY;
					event.chKey = (TCHAR)wParam;
					event.ptMouse = m_ptLastMousePos;
					event.wKeyState = CPlatform::MapKeyState();
					event.dwTimestamp = ::GetTickCount();
					m_pFocus->Event(event);
				}
			}
			break;
		}
		return false;
	}

	bool CPaintManagerWin32UI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_hWndPaint == NULL ) return false;
		// Cycle through listeners
		for( int i = 0; i < m_aMessageFilters.GetSize(); i++ ) 
		{
			bool bHandled = false;
			LRESULT lResult = static_cast<IMessageFilterUI*>(m_aMessageFilters[i])->MessageHandler(uMsg, wParam, lParam, bHandled);
			if( bHandled ) {
				lRes = lResult;
				switch( uMsg ) {
				case WM_MOUSEMOVE:
				case WM_LBUTTONDOWN:
				case WM_LBUTTONDBLCLK:
				case WM_LBUTTONUP:
					{
						POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
						m_ptLastMousePos = pt;
					}
					break;
				case WM_CONTEXTMENU:
				case WM_MOUSEWHEEL:
					{
						POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
						::ScreenToClient(m_hWndPaint, &pt);
						m_ptLastMousePos = pt;
					}
					break;
				}
				return true;
			}
		}

		if( m_bLayered ) {
			switch( uMsg ) {
			case WM_NCACTIVATE:
				if( !::IsIconic(m_hWndPaint) ) {
					lRes = (wParam == 0) ? TRUE : FALSE;
					return true;
				}
				break;
			case WM_NCCALCSIZE:
			case WM_NCPAINT:
				lRes = 0;
				return true;
			}
		}

		return CPaintManagerUI::MessageHandler(uMsg, wParam, lParam, lRes);
	}

	bool CPaintManagerWin32UI::OnApp1(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		for( int i = 0; i < m_aDelayedCleanup.GetSize(); i++ ) 
			delete static_cast<CControlUI*>(m_aDelayedCleanup[i]);
		m_aDelayedCleanup.Empty();

		//CDuiInnerLock lock(&m_lockAsyncNotify); //add by liq99
		m_bAsyncNotifyPosted = false;

		TNotifyUI* pMsg = NULL;
		while( pMsg = static_cast<TNotifyUI*>(m_aAsyncNotify.GetAt(0)) ) {
			m_aAsyncNotify.Remove(0);
			if( pMsg->pSender != NULL ) {
				if( pMsg->pSender->OnNotify ) pMsg->pSender->OnNotify(pMsg);
				if(!pMsg->pSender->m_asOnNotify.IsEmpty()) 
					ExecuteScript(pMsg->pSender->m_asOnNotify, pMsg->pSender, pMsg);
			}
			for( int j = 0; j < m_aNotifiers.GetSize(); j++ ) {
				static_cast<INotifyUI*>(m_aNotifiers[j])->Notify(*pMsg);
			}
			delete pMsg;
		}
		return false;
	}

	bool CPaintManagerWin32UI::OnClose(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		TEventUI event = { 0 };
		event.ptMouse = m_ptLastMousePos;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		if( m_pEventHover != NULL ) {
			event.Type = UIEVENT_MOUSELEAVE;
			event.pSender = m_pEventHover;
			m_pEventHover->Event(event);
		}
		if( m_pEventClick != NULL ) {
			event.Type = UIEVENT_BUTTONUP;
			event.pSender = m_pEventClick;
			m_pEventClick->Event(event);
		}

		SetFocus(NULL);

		if( ::GetActiveWindow() == m_hWndPaint ) {
			HWND hwndParent = ::GetWindow(m_hWndPaint, GW_OWNER);//GetWindowOwner(m_hWndPaint);
			if( hwndParent != NULL ) ::SetFocus(hwndParent);
		}

		if (m_hwndTooltip != NULL) {
			::DestroyWindow(m_hwndTooltip);
			m_hwndTooltip = NULL;
		}
		return false;
	}

	bool CPaintManagerWin32UI::OnPaint(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) {
			PAINTSTRUCT ps = { 0 };
			::BeginPaint(m_hWndPaint, &ps);
			//Render()->DrawColor( ps.rcPaint, 0xFF000000);
			::EndPaint(m_hWndPaint, &ps);
			return true;
		}

		CDuiRect rcClient;
		::GetClientRect(m_hWndPaint, &rcClient);

		//RECT rcPaint = { 0 };
		CDuiRect rcPaint;
		if( !::GetUpdateRect(m_hWndPaint, &rcPaint, FALSE) ) 
			return true;

		// Set focus to first control?
		if( m_bFocusNeeded ) {
			SetNextTabControl();
		}

		bool bNeedSizeMsg = false;
		DWORD dwWidth = rcClient.right - rcClient.left;
		DWORD dwHeight = rcClient.bottom - rcClient.top;

		SetPainting(true);
		if( m_bUpdateNeeded && !m_bLockUpdate) 
		{
			m_bUpdateNeeded = false;
			if( !rcClient.IsEmpty() && !::IsIconic(m_hWndPaint) ) 
			{
				if( m_pRoot->IsUpdateNeeded() ) 
				{
					RECT rcRoot = rcClient;
					if( m_bLayered ) 
					{
						rcRoot.left += m_rcLayeredInset.left;
						rcRoot.top += m_rcLayeredInset.top;
						rcRoot.right -= m_rcLayeredInset.right;
						rcRoot.bottom -= m_rcLayeredInset.bottom;
					}
					m_pRoot->SetPos(rcRoot, true);
					bNeedSizeMsg = true;
				}
				else 
				{
					//单独NeedUpdate某个控件时
					CControlUI* pControl = NULL;
					m_aFoundControls.Empty();
					m_pRoot->FindControl(__FindControlsFromUpdate, NULL, UIFIND_VISIBLE | UIFIND_ME_FIRST | UIFIND_UPDATETEST);
					for( int it = 0; it < m_aFoundControls.GetSize(); it++ ) {
						pControl = static_cast<CControlUI*>(m_aFoundControls[it]);
						//float控件不需要重新计算RelativePos，因为第一次SetPos时，已经把偏移计算好了，并且保存到m_rcItem。
						//if( !pControl->IsFloat() ) pControl->SetPos(pControl->GetPos(), true);
						//else pControl->SetPos(pControl->GetRelativePos(), true);
						pControl->SetPos(pControl->GetPos(), true);
					}
					bNeedSizeMsg = true;
				}

				//第一次绘制窗口之前，发送DUI_MSGTYPE_WINDOWINIT消息
				if( m_bFirstLayout ) 
				{
					m_bFirstLayout = false;
					SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWINIT,  0, 0, false);
					if( m_bLayered && m_bLayeredChanged ) 
					{
						Invalidate();
						SetPainting(false);
						return true;
					}
					// 更新阴影窗口显示
					m_shadow.Update(m_hWndPaint);
				}
			}
		}
		else if( m_bLayered && m_bLayeredChanged ) {
			RECT rcRoot = rcClient;
			rcRoot.left += m_rcLayeredInset.left;
			rcRoot.top += m_rcLayeredInset.top;
			rcRoot.right -= m_rcLayeredInset.right;
			rcRoot.bottom -= m_rcLayeredInset.bottom;
			m_pRoot->SetPos(rcRoot, true);
		}

		if( m_bLayered ) 
		{
			DWORD dwExStyle = ::GetWindowLong(m_hWndPaint, GWL_EXSTYLE);
			DWORD dwNewExStyle = dwExStyle | WS_EX_LAYERED;
			if(dwExStyle != dwNewExStyle) ::SetWindowLong(m_hWndPaint, GWL_EXSTYLE, dwNewExStyle);

			//UnionRect(&rcPaint, &rcPaint, &m_rcLayeredUpdate);
			rcPaint.Union(rcPaint, m_rcLayeredUpdate);
			if( rcPaint.right > rcClient.right ) rcPaint.right = rcClient.right;
			if( rcPaint.bottom > rcClient.bottom ) rcPaint.bottom = rcClient.bottom;
			//::ZeroMemory(&m_rcLayeredUpdate, sizeof(m_rcLayeredUpdate));
			m_rcLayeredUpdate.Empty();
		}

		PAINTSTRUCT ps = { 0 };
		::BeginPaint(m_hWndPaint, &ps);

		Render()->Resize(dwWidth, dwHeight);

		Render()->ClearAlpha(rcPaint);

		Render()->SaveDC();

		if( m_bLayered ) 
		{
			if(!m_diLayered.sDrawString.IsEmpty() && IsLayeredChanged()) 
			{
				DWORD dwWidth = rcClient.right - rcClient.left;
				DWORD dwHeight = rcClient.bottom - rcClient.top;
				RECT rcLayeredClient = rcClient;
				rcLayeredClient.left += m_rcLayeredInset.left;
				rcLayeredClient.top += m_rcLayeredInset.top;
				rcLayeredClient.right -= m_rcLayeredInset.right;
				rcLayeredClient.bottom -= m_rcLayeredInset.bottom;

				UIClip clip;
				clip.GenerateClip(Render(), rcLayeredClient);
				Render()->DrawImageInfo(rcLayeredClient, rcLayeredClient, &m_diLayered);
			}
		}

		GetRoot()->Paint(Render(), rcPaint, NULL);

		for( int i = 0; i < m_aPostPaintControls.GetSize(); i++ ) {
			CControlUI* pPostPaintControl = static_cast<CControlUI*>(m_aPostPaintControls[i]);
			pPostPaintControl->DoPostPaint(Render(), rcPaint);
		}

		//画出需要刷新的区域，可以用于某些测试场景。
		if( IsShowUpdateRect() && !IsLayered() ) 
		{
			Render()->DrawRect(rcPaint, 1, UIRGB(255,0,0));
		}

		//Render()->GetBitmap()->SaveFile(_T("c:\\uiframe.bmp"));
		Render()->RestoreDC();

		if( IsLayered() ) 
		{
			RECT rcWnd = { 0 };
			::GetWindowRect(m_hWndPaint, &rcWnd);
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, GetOpacity(), AC_SRC_ALPHA };
			POINT ptPos   = { rcWnd.left, rcWnd.top };
			SIZE sizeWnd  = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
			POINT ptSrc   = { 0, 0 };
			::UpdateLayeredWindow(m_hWndPaint, NULL, &ptPos, &sizeWnd, Render()->GetDC(), &ptSrc, 0, &bf, ULW_ALPHA);
		}
		else 
		{
			::BitBlt(m_hDcPaint, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, Render()->GetDC(), rcPaint.left, rcPaint.top, SRCCOPY);
		}

		Render()->RestoreObject();
		::EndPaint(m_hWndPaint, &ps);

		// 绘制结束
		SetPainting(false);
		m_bLayeredChanged = false;
		if( m_bUpdateNeeded ) Invalidate();

		// 发送窗口大小改变消息
		if(bNeedSizeMsg) {
			this->SendNotify(m_pRoot, DUI_MSGTYPE_WINDOWSIZE, 0, 0, true);
		}
		return true;
	}

	bool CPaintManagerWin32UI::OnEraseBkgnd(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		lRes = 1;
		return false;
	}

	bool CPaintManagerWin32UI::OnPrintClient(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
// 		if( m_pRoot == NULL ) return false;
// 		RECT rcClient;
// 		::GetClientRect(m_hWndPaint, &rcClient);
// 		HDC hDC = (HDC) wParam;
// 		int save = ::SaveDC(hDC);
// 		m_pRoot->Paint(hDC, rcClient, NULL);
// 		if( (lParam & PRF_CHILDREN) != 0 ) {
// 			HWND hWndChild = ::GetWindow(m_hWndPaint, GW_CHILD);
// 			while( hWndChild != NULL ) {
// 				RECT rcPos = { 0 };
// 				::GetWindowRect(hWndChild, &rcPos);
// 				::MapWindowPoints(HWND_DESKTOP, m_hWndPaint, reinterpret_cast<LPPOINT>(&rcPos), 2);
// 				::SetWindowOrgEx(hDC, -rcPos.left, -rcPos.top, NULL);
// 				::SendMessage(hWndChild, WM_PRINT, wParam, lParam | PRF_NONCLIENT);
// 				hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
// 			}
// 		}
// 		::RestoreDC(hDC, save);
		return false;
	}

	bool CPaintManagerWin32UI::OnGetMaxMinInfo(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		MONITORINFO Monitor = {};
		Monitor.cbSize = sizeof(Monitor);
		::GetMonitorInfo(::MonitorFromWindow(m_hWndPaint, MONITOR_DEFAULTTOPRIMARY), &Monitor);
		//RECT rcWork = Monitor.rcWork;
		CDuiRect rcWork = Monitor.rcWork;
		if( Monitor.dwFlags != MONITORINFOF_PRIMARY ) {
			//::OffsetRect(&rcWork, -rcWork.left, -rcWork.top);
			rcWork.Offset(-rcWork.left, -rcWork.top);
		}

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		if( m_szMinWindow.cx > 0 ) lpMMI->ptMinTrackSize.x = m_szMinWindow.cx;
		if( m_szMinWindow.cy > 0 ) lpMMI->ptMinTrackSize.y = m_szMinWindow.cy;
		if( m_szMaxWindow.cx > 0 ) lpMMI->ptMaxTrackSize.x = m_szMaxWindow.cx;
		if( m_szMaxWindow.cy > 0 ) lpMMI->ptMaxTrackSize.y = m_szMaxWindow.cy;
		if( m_szMaxWindow.cx > 0 ) lpMMI->ptMaxSize.x = m_szMaxWindow.cx;
		if( m_szMaxWindow.cy > 0 ) lpMMI->ptMaxSize.y = m_szMaxWindow.cy;

		return false;
	}

	bool CPaintManagerWin32UI::OnSize(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pFocus != NULL ) {
			TEventUI event = { 0 };
			event.Type = UIEVENT_WINDOWSIZE;
			event.pSender = m_pFocus;
			event.dwTimestamp = ::GetTickCount();
			m_pFocus->Event(event);
		}
		if( m_pRoot != NULL ) m_pRoot->NeedUpdate();

		return true;
	}

	bool CPaintManagerWin32UI::OnTimer(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		//DUITRACE(_T("CPaintManagerWin32UI::OnTimer"));
		for( int i = 0; i < m_aTimers.GetSize(); i++ ) {
			const TIMERINFO* pTimer = static_cast<TIMERINFO*>(m_aTimers[i]);
			if(pTimer->hWnd == m_hWndPaint && 
				pTimer->uWinTimer == LOWORD(wParam) && 
				pTimer->bKilled == false)
			{
				TEventUI event = { 0 };
				event.Type = UIEVENT_TIMER;
				event.pSender = pTimer->pSender;
				event.dwTimestamp = ::GetTickCount();
				event.ptMouse = m_ptLastMousePos;
				event.wKeyState = CPlatform::MapKeyState();
				event.wParam = pTimer->nLocalID;
				event.lParam = lParam;
				pTimer->pSender->Event(event);
				break;
			}
		}

		return false;
	}

	bool CPaintManagerWin32UI::OnLButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		// We alway set focus back to our app (this helps
		// when Win32 child windows are placed on the dialog
		// and we need to remove them on focus change).
		if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
		if( m_pRoot == NULL ) return false;
		// 查找控件
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( pControl->GetManager() != this ) return false;

		// 准备拖拽
		if(pControl->IsDragEnabled()) {
			m_bDragMode = true;
			m_dragBitmap = MakeRefPtr<UIBitmap>(CreateControlBitmap(pControl));
		}

		// 开启捕获
		SetCapture();
		// 事件处理
		m_pEventClick = pControl;
		pControl->SetFocus();

		TEventUI event = { 0 };
		event.Type = UIEVENT_BUTTONDOWN;
		event.pSender = pControl;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);

		return false;
	}

	bool CPaintManagerWin32UI::OnLButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		if( m_pEventClick == NULL ) return false;
		ReleaseCapture();
		TEventUI event = { 0 };
		event.Type = UIEVENT_BUTTONUP;
		event.pSender = m_pEventClick;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();

		CControlUI* pClick = m_pEventClick;
		m_pEventClick = NULL;
		pClick->Event(event);

		return false;
	}

	bool CPaintManagerWin32UI::OnLButtonDbClick(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if (!m_bNoActivate) ::SetFocus(m_hWndPaint);

		//由于duilib把单击和双击混淆一起，
		//在双击事件之后弹出窗口，如果在窗口中改变了父窗口控件，可能导致m_pEvenClick失效
		//双击时事件的处理顺序是 1左键按下 -- 2左键弹起  -- 3双击 
		//左键弹起时，写了m_pEventClick = NULL; 双击之后没机会进行m_pEventClick = NULL了

		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( pControl->GetManager() != this ) return false;
		SetCapture();
		TEventUI event = { 0 };
		event.Type = UIEVENT_DBLCLICK;
		event.pSender = pControl;
		event.ptMouse = pt;
		event.wParam = wParam;
		event.lParam = lParam;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);
		m_pEventClick = pControl;

		return false;
	}

	bool CPaintManagerWin32UI::OnRButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( pControl->GetManager() != this ) return false;
		pControl->SetFocus();
		SetCapture();
		TEventUI event = { 0 };
		event.Type = UIEVENT_RBUTTONDOWN;
		event.pSender = pControl;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);
		m_pEventClick = pControl;

		return false;
	}

	bool CPaintManagerWin32UI::OnRButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if(m_bMouseCapture) ReleaseCapture();
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		m_pEventClick = FindControl(pt);
		if(m_pEventClick == NULL) return false;

		TEventUI event = { 0 };
		event.Type = UIEVENT_RBUTTONUP;
		event.pSender = m_pEventClick;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		m_pEventClick->Event(event);

		return false;
	}

	bool CPaintManagerWin32UI::OnRButtonDbClick(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		return false;
	}

	bool CPaintManagerWin32UI::OnMButtonDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if (!m_bNoActivate) ::SetFocus(m_hWndPaint);
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( pControl->GetManager() != this ) return false;
		pControl->SetFocus();
		SetCapture();
		TEventUI event = { 0 };
		event.Type = UIEVENT_MBUTTONDOWN;
		event.pSender = pControl;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);
		m_pEventClick = pControl;

		return false;
	}

	bool CPaintManagerWin32UI::OnMButtonUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if(m_bMouseCapture) ReleaseCapture();
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_ptLastMousePos = pt;
		m_pEventClick = FindControl(pt);
		if(m_pEventClick == NULL) return false;

		TEventUI event = { 0 };
		event.Type = UIEVENT_MBUTTONUP;
		event.pSender = m_pEventClick;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.dwTimestamp = ::GetTickCount();
		m_pEventClick->Event(event);

		return false;
	}

	bool CPaintManagerWin32UI::OnMouseOver(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		m_bMouseTracking = false;
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		CControlUI* pHover = FindControl(pt);
		if( pHover == NULL ) return false;
		// Generate mouse hover event
		if( m_pEventHover != NULL ) {
			TEventUI event = { 0 };
			event.Type = UIEVENT_MOUSEHOVER;
			event.pSender = m_pEventHover;
			event.wParam = wParam;
			event.lParam = lParam;
			event.dwTimestamp = ::GetTickCount();
			event.ptMouse = pt;
			event.wKeyState = CPlatform::MapKeyState();
			m_pEventHover->Event(event);
		}
		// Create tooltip information
		CDuiString sToolTip = pHover->GetToolTip();
		if( sToolTip.IsEmpty() ) return true;
		::ZeroMemory(&m_ToolTip, sizeof(TOOLINFO));
		m_ToolTip.cbSize = sizeof(TOOLINFO);
		m_ToolTip.uFlags = TTF_IDISHWND;
		m_ToolTip.hwnd = m_hWndPaint;
		m_ToolTip.uId = (UINT_PTR) m_hWndPaint;
		m_ToolTip.hinst = m_hInstance;
		m_ToolTip.lpszText = const_cast<LPTSTR>( (LPCTSTR) sToolTip );
		m_ToolTip.rect = pHover->GetPos();
		if( m_hwndTooltip == NULL ) {
			m_hwndTooltip = ::CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hWndPaint, NULL, m_hInstance, NULL);
			::SendMessage(m_hwndTooltip, TTM_ADDTOOL, 0, (LPARAM) &m_ToolTip);
			::SendMessage(m_hwndTooltip,TTM_SETMAXTIPWIDTH,0, pHover->GetToolTipWidth());
		}
		if(!::IsWindowVisible(m_hwndTooltip))
		{
			::SendMessage(m_hwndTooltip, TTM_SETTOOLINFO, 0, (LPARAM)&m_ToolTip);
			::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ToolTip);
		}

		return true;
	}

	bool CPaintManagerWin32UI::OnMouseLeave(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_hwndTooltip != NULL ) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
		if( m_bMouseTracking ) {
			POINT pt = { 0 };
			CDuiRect rcWnd;
			::GetCursorPos(&pt);
			::GetWindowRect(m_hWndPaint, &rcWnd);
			//if( !::IsIconic(m_hWndPaint) && ::GetActiveWindow() == m_hWndPaint && ::PtInRect(&rcWnd, pt) ) {
			if( !::IsIconic(m_hWndPaint) && ::GetActiveWindow() == m_hWndPaint && rcWnd.PtInRect(pt) ) {
				if( ::SendMessage(m_hWndPaint, WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y)) == HTCLIENT ) {
					::ScreenToClient(m_hWndPaint, &pt);
					::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
				}
				else 
					::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM)-1);
			}
			else 
				::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM)-1);
		}
		m_bMouseTracking = false;

		return false;
	}

	bool CPaintManagerWin32UI::OnMouseMove(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		// Start tracking this entire window again...
		if( !m_bMouseTracking ) {
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = m_hWndPaint;
			tme.dwHoverTime = m_hwndTooltip == NULL ? m_iTooltipHoverTime : (DWORD) ::SendMessage(m_hwndTooltip, TTM_GETDELAYTIME, TTDT_INITIAL, 0L);
			_TrackMouseEvent(&tme);
			m_bMouseTracking = true;
		}

		// Generate the appropriate mouse messages
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		// 是否移动
		bool bNeedDrag = true;
		if(m_ptLastMousePos.x == pt.x && m_ptLastMousePos.y == pt.y) {
			bNeedDrag = false;
		}
		// 记录鼠标位置
		m_ptLastMousePos = pt;
		CControlUI* pNewHover = FindControl(pt);
		if( pNewHover != NULL && pNewHover->GetManager() != this ) return false;

		// 拖拽事件
		if(bNeedDrag && m_bDragMode && wParam == MK_LBUTTON)
		{
			::ReleaseCapture();
			CIDropSource* pdsrc = new CIDropSource;
			if(pdsrc == NULL) return 0;
			pdsrc->AddRef();

			CIDataObject* pdobj = new CIDataObject(pdsrc);
			if(pdobj == NULL) return 0;
			pdobj->AddRef();

			FORMATETC fmtetc = {0};
			STGMEDIUM medium = {0};
			fmtetc.dwAspect = DVASPECT_CONTENT;
			fmtetc.lindex = -1;
			//////////////////////////////////////
			fmtetc.cfFormat = CF_BITMAP;
			fmtetc.tymed = TYMED_GDI;			
			medium.tymed = TYMED_GDI;
			//HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_hDragBitmap, fmtetc.cfFormat, NULL);
			HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_dragBitmap->GetHBITMAP(), fmtetc.cfFormat, NULL);
			medium.hBitmap = hBitmap;
			pdobj->SetData(&fmtetc,&medium,FALSE);
			//////////////////////////////////////
			BITMAP bmap;
			GetObject(hBitmap, sizeof(BITMAP), &bmap);
			RECT rc={0, 0, bmap.bmWidth, bmap.bmHeight};
			fmtetc.cfFormat = CF_ENHMETAFILE;
			fmtetc.tymed = TYMED_ENHMF;
			HDC hMetaDC = CreateEnhMetaFile(m_hDcPaint, NULL, NULL, NULL);
			HDC hdcMem = CreateCompatibleDC(m_hDcPaint);
			HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBitmap);
			::BitBlt(hMetaDC, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
			::SelectObject(hdcMem, hOldBmp);
			medium.hEnhMetaFile = CloseEnhMetaFile(hMetaDC);
			DeleteDC(hdcMem);
			medium.tymed = TYMED_ENHMF;
			pdobj->SetData(&fmtetc, &medium, TRUE);
			//////////////////////////////////////
			CDragSourceHelper dragSrcHelper;
			POINT ptDrag = {0};
			ptDrag.x = bmap.bmWidth / 2;
			ptDrag.y = bmap.bmHeight / 2;
			dragSrcHelper.InitializeFromBitmap(hBitmap, ptDrag, rc, pdobj); //will own the bmp
			DWORD dwEffect;
			HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
			if(dwEffect )
				pdsrc->Release();
			delete pdsrc;
			pdobj->Release();
			m_bDragMode = false;
			return false;
		}
		TEventUI event = { 0 };
		event.ptMouse = pt;
		event.wParam = wParam;
		event.lParam = lParam;
		event.dwTimestamp = ::GetTickCount();
		event.wKeyState = CPlatform::MapKeyState();
		if( !IsCaptured() ) {
			pNewHover = FindControl(pt);
			if( pNewHover != NULL && pNewHover->GetManager() != this ) return false;
			if( pNewHover != m_pEventHover && m_pEventHover != NULL ) {
				event.Type = UIEVENT_MOUSELEAVE;
				event.pSender = m_pEventHover;

				CStdPtrArray aNeedMouseLeaveNeeded(m_aNeedMouseLeaveNeeded.GetSize());
				aNeedMouseLeaveNeeded.Resize(m_aNeedMouseLeaveNeeded.GetSize());
				::CopyMemory(aNeedMouseLeaveNeeded.GetData(), m_aNeedMouseLeaveNeeded.GetData(), m_aNeedMouseLeaveNeeded.GetSize() * sizeof(LPVOID));
				for( int i = 0; i < aNeedMouseLeaveNeeded.GetSize(); i++ ) {
					static_cast<CControlUI*>(aNeedMouseLeaveNeeded[i])->Event(event);
				}

				m_pEventHover->Event(event);
				m_pEventHover = NULL;
				if( m_hwndTooltip != NULL ) ::SendMessage(m_hwndTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM) &m_ToolTip);
			}
			if( pNewHover != m_pEventHover && pNewHover != NULL ) {
				event.Type = UIEVENT_MOUSEENTER;
				event.pSender = pNewHover;
				pNewHover->Event(event);
				m_pEventHover = pNewHover;
			}
		}
		if( m_pEventClick != NULL ) {
			event.Type = UIEVENT_MOUSEMOVE;
			event.pSender = m_pEventClick;
			m_pEventClick->Event(event);
		}
		else if( pNewHover != NULL ) {
			event.Type = UIEVENT_MOUSEMOVE;
			event.pSender = pNewHover;
			pNewHover->Event(event);
		}

		return false;
	}

	bool CPaintManagerWin32UI::OnMouseWheel(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		::ScreenToClient(m_hWndPaint, &pt);
		m_ptLastMousePos = pt;
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( pControl->GetManager() != this ) return false;
		int zDelta = (int) (short) HIWORD(wParam);
		TEventUI event = { 0 };
		event.Type = UIEVENT_SCROLLWHEEL;
		event.pSender = pControl;
		event.wParam = MAKELPARAM(zDelta < 0 ? SB_LINEDOWN : SB_LINEUP, zDelta);
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);

		// Let's make sure that the scroll item below the cursor is the same as before...
		::SendMessage(m_hWndPaint, WM_MOUSEMOVE, 0, (LPARAM) MAKELPARAM(m_ptLastMousePos.x, m_ptLastMousePos.y));

		return false;
	}

	bool CPaintManagerWin32UI::OnContextMenu(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		::ScreenToClient(m_hWndPaint, &pt);
		m_ptLastMousePos = pt;
		if( m_pEventClick == NULL ) return false;
		ReleaseCapture();
		TEventUI event = { 0 };
		event.Type = UIEVENT_CONTEXTMENU;
		event.pSender = m_pEventClick;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = (WORD)wParam;
		event.lParam = (LPARAM)m_pEventClick;
		event.dwTimestamp = ::GetTickCount();
		m_pEventClick->Event(event);
		m_pEventClick = NULL;

		return false;
	}

	bool CPaintManagerWin32UI::OnChar(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		if( m_pFocus == NULL ) return false;
		TEventUI event = { 0 };
		event.Type = UIEVENT_CHAR;
		event.pSender = m_pFocus;
		event.wParam = wParam;
		event.lParam = lParam;
		event.chKey = (TCHAR)wParam;
		event.ptMouse = m_ptLastMousePos;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		m_pFocus->Event(event);

		return false;
	}

	bool CPaintManagerWin32UI::OnKeyDown(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		if( m_pFocus == NULL ) return false;
		TEventUI event = { 0 };
		event.Type = UIEVENT_KEYDOWN;
		event.pSender = m_pFocus;
		event.wParam = wParam;
		event.lParam = lParam;
		event.chKey = (TCHAR)wParam;
		event.ptMouse = m_ptLastMousePos;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		m_pFocus->Event(event);
		m_pEventKey = m_pFocus;

		return false;
	}

	bool CPaintManagerWin32UI::OnKeyUp(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		if( m_pEventKey == NULL ) return false;
		TEventUI event = { 0 };
		event.Type = UIEVENT_KEYUP;
		event.pSender = m_pEventKey;
		event.wParam = wParam;
		event.lParam = lParam;
		event.chKey = (TCHAR)wParam;
		event.ptMouse = m_ptLastMousePos;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		m_pEventKey->Event(event);
		m_pEventKey = NULL;

		return false;
	}

	bool CPaintManagerWin32UI::OnSetCursor(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pRoot == NULL ) return false;
		if( LOWORD(lParam) != HTCLIENT ) return false;
		if( m_bMouseCapture ) return true;

		POINT pt = { 0 };
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWndPaint, &pt);
		CControlUI* pControl = FindControl(pt);
		if( pControl == NULL ) return false;
		if( (pControl->GetControlFlags() & UIFLAG_SETCURSOR) == 0 ) return false;
		TEventUI event = { 0 };
		event.Type = UIEVENT_SETCURSOR;
		event.pSender = pControl;
		event.wParam = wParam;
		event.lParam = lParam;
		event.ptMouse = pt;
		event.wKeyState = CPlatform::MapKeyState();
		event.dwTimestamp = ::GetTickCount();
		pControl->Event(event);

		return true;
	}

	bool CPaintManagerWin32UI::OnSetFocus(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( m_pFocus != NULL ) {
			TEventUI event = { 0 };
			event.Type = UIEVENT_SETFOCUS;
			event.wParam = wParam;
			event.lParam = lParam;
			event.pSender = m_pFocus;
			event.dwTimestamp = ::GetTickCount();
			m_pFocus->Event(event);
		}

		return false;
	}

	bool CPaintManagerWin32UI::OnKillFocus(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if(IsCaptured()) ReleaseCapture();
		return false;
	}

	bool CPaintManagerWin32UI::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( lParam == 0 ) return false;
		LPNMHDR lpNMHDR = (LPNMHDR) lParam;
		if( lpNMHDR != NULL ) lRes = ::SendMessage(lpNMHDR->hwndFrom, OCM__BASE + WM_NOTIFY, wParam, lParam);
		return true;
	}

	bool CPaintManagerWin32UI::OnCommand(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		if( lParam == 0 ) return false;
		HWND hWndChild = (HWND) lParam;
		lRes = ::SendMessage(hWndChild, OCM__BASE + WM_COMMAND, wParam, lParam);
		if(lRes != 0) return true;
		return false;
	}

	bool CPaintManagerWin32UI::OnCtlColorEdit(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		// Refer To: http://msdn.microsoft.com/en-us/library/bb761691(v=vs.85).aspx
		// Read-only or disabled edit controls do not send the WM_CTLCOLOREDIT message; instead, they send the WM_CTLCOLORSTATIC message.
		if( lParam == 0 ) return false;
		HWND hWndChild = (HWND) lParam;
		lRes = ::SendMessage(hWndChild, OCM__BASE + WM_CTLCOLOREDIT, wParam, lParam);
		if(lRes != 0) return true;
		return false;
	}

	bool CPaintManagerWin32UI::OnCtlColorStatic(WPARAM wParam, LPARAM lParam, LRESULT& lRes)
	{
		// Refer To: http://msdn.microsoft.com/en-us/library/bb761691(v=vs.85).aspx
		// Read-only or disabled edit controls do not send the WM_CTLCOLOREDIT message; instead, they send the WM_CTLCOLORSTATIC message.
		if( lParam == 0 ) return false;
		HWND hWndChild = (HWND) lParam;
		lRes = ::SendMessage(hWndChild, OCM__BASE + WM_CTLCOLORSTATIC, wParam, lParam);
		if(lRes != 0) return true;
		return false;
	}


	UIBitmap* CPaintManagerWin32UI::CreateControlBitmap(CControlUI* pControl, DWORD dwFilterColor, CControlUI* pStopControl)
	{
		CPaintManagerUI *pManager = pControl->GetManager();
		if(pManager == NULL) return FALSE;
		if(pManager->GetRoot() == NULL) return FALSE;

		RECT rcControl = pControl->GetPos();
		int cx = rcControl.right - rcControl.left;
		int cy = rcControl.bottom - rcControl.top;

		//建立新的绘图
		CStdRefPtr<UIRender> pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRender->Init(pManager);

		//注意这里必须是root的rc，或者整个client的pos。 总之这个size必须包含了rcControl
		pRender->Resize(pManager->GetRoot()->GetPos()); 

		//在整个区域只绘制了pControl
		pControl->Paint(pRender, rcControl, NULL);

		//再建立一个新的绘图， 把绘制好的pControl的区域BitBlt过来
		CStdRefPtr<UIRender> pRenderClone = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRenderClone->Init(pManager);

		UIBitmap *pBitmapClone = UIGlobal::CreateBitmap();
		pBitmapClone->CreateARGB32Bitmap(pManager->GetPaintDC(), cx, cy, TRUE);
		pRenderClone->SelectObject(pBitmapClone);

		pRenderClone->BitBlt(0, 0, cx, cy, pRender, rcControl.left, rcControl.top, SRCCOPY);
		if (dwFilterColor > 0x00FFFFFF) 
			pRenderClone->DrawColor(CDuiRect(0,0,cx,cy), CDuiSize(0,0), dwFilterColor);

		return pBitmapClone;
	}

	//原理是，整个窗口画一遍，除了pControl自己不画，然后rcWnd的位置截图下来，就是内部窗口的背景图了。
	//这样就可以实现Edit的透明背景了
	UIBitmap* CPaintManagerWin32UI::CreateControlBackBitmap(CControlUI* pControl, const RECT &rcWnd, DWORD dwFilterColor)
	{
		CPaintManagerUI *pManager = pControl->GetManager();
		if(pManager == NULL) return FALSE;
		if(pManager->GetRoot() == NULL) return FALSE;

		CControlUI *pRoot = pManager->GetRoot();
		RECT rcRoot = pRoot->GetPos();

		int cx = rcWnd.right - rcWnd.left;
		int cy = rcWnd.bottom - rcWnd.top;

		//建立新的绘图
		CStdRefPtr<UIRender> pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRender->Init(pManager);
		pRender->Resize(rcRoot); 

		//从root开始绘制，遇到pControl时，停止绘制
		pRoot->Paint(pRender, rcWnd, pControl);

		//再建立一个新的绘图， 把绘制好的rcWnd的区域BitBlt过来
		CStdRefPtr<UIRender> pRenderClone = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRenderClone->Init(pManager);

		//创建返回的位图，调用方需要释放。
		UIBitmap *pBitmapClone = UIGlobal::CreateBitmap();
		pBitmapClone->CreateARGB32Bitmap(pManager->GetPaintDC(), cx, cy, TRUE);
		pRenderClone->SelectObject(pBitmapClone);
		pRenderClone->BitBlt(0, 0, cx, cy, pRender, rcWnd.left, rcWnd.top, SRCCOPY);

		if (dwFilterColor > 0x00FFFFFF) 
			pRenderClone->DrawColor(CDuiRect(0,0,cx,cy), CDuiSize(0,0), dwFilterColor);

		return pBitmapClone;
	}

	//////////////////////////////////////////////////////////////////////////
	CLockWindowUpdateUI::CLockWindowUpdateUI(CPaintManagerUI* pManager) : m_pManager(pManager)
	{
		if (::IsWindow(m_pManager->GetPaintWindow()))
		{
			//::LockWindowUpdate(m_pManager->GetPaintWindow());
			SetWindowRedraw(pManager->GetPaintWindow(), FALSE);
		}
	}
	CLockWindowUpdateUI::~CLockWindowUpdateUI()
	{
		if (::IsWindow(m_pManager->GetPaintWindow()))
		{
			//::LockWindowUpdate(NULL);
			SetWindowRedraw(m_pManager->GetPaintWindow(), TRUE);
			m_pManager->Invalidate();
		}
	}

} // namespace DuiLib
#endif //#ifdef DUILIB_WIN32
