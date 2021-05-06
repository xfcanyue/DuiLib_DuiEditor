#include "StdAfx.h"
#include "TxtWinHost.h"


// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0

namespace DuiLib {

	EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
		0x8d33f740,
		0xcf58,
		0x11ce,
		{0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
	};

	EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
		0xc5bdd8d0,
		0xd26e,
		0x11ce,
		{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
	};

	const LONG cInitTextMax = (32 * 1024) - 1;

	// Convert Pixels on the X axis to Himetric
	LONG DXtoHimetricX(LONG dx, LONG xPerInch)
	{
		return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
	}

	// Convert Pixels on the Y axis to Himetric
	LONG DYtoHimetricY(LONG dy, LONG yPerInch)
	{
		return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
	}

	HRESULT InitDefaultCharFormat(CRichEditUI* re, CHARFORMAT2W* pcf, HFONT hfont) 
	{
		memset(pcf, 0, sizeof(CHARFORMAT2W));
		if(hfont == NULL) {
			hfont = re->GetManager()->GetFont(re->GetFont());
		}
		LOGFONT lf;
		::GetObject(hfont, sizeof(LOGFONT), &lf);

		DWORD dwColor = re->GetTextColor();
		if(re->GetManager()->IsLayered()) {
			CRenderEngine::CheckAlphaColor(dwColor);
		}
		pcf->cbSize = sizeof(CHARFORMAT2W);
		pcf->crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		LONG yPixPerInch = GetDeviceCaps(re->GetManager()->GetPaintDC(), LOGPIXELSY);
		pcf->yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
		pcf->yOffset = 0;
		pcf->dwEffects = 0;
		pcf->dwMask = CFM_SIZE | CFM_OFFSET | CFM_FACE | CFM_CHARSET | CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE;
		if(lf.lfWeight >= FW_BOLD)
			pcf->dwEffects |= CFE_BOLD;
		if(lf.lfItalic)
			pcf->dwEffects |= CFE_ITALIC;
		if(lf.lfUnderline)
			pcf->dwEffects |= CFE_UNDERLINE;
		pcf->bCharSet = lf.lfCharSet;
		pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
		_tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
		//need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
		MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

		return S_OK;
	}

	HRESULT InitDefaultParaFormat(CRichEditUI* re, PARAFORMAT2* ppf) 
	{	
		memset(ppf, 0, sizeof(PARAFORMAT2));
		ppf->cbSize = sizeof(PARAFORMAT2);
		ppf->dwMask = PFM_ALL;
		ppf->wAlignment = PFA_LEFT;
		ppf->cTabCount = 1;
		ppf->rgxTabs[0] = lDefaultTab;

		return S_OK;
	}

	CTxtWinHost::CTxtWinHost() : m_re(NULL)
	{
		::ZeroMemory(&cRefs, sizeof(CTxtWinHost) - offsetof(CTxtWinHost, cRefs));
		cchTextMost = cInitTextMax;
		laccelpos = -1;
	}

	CTxtWinHost::~CTxtWinHost()
	{
		pserv->OnTxInPlaceDeactivate();
		pserv->Release();
	}

	////////////////////// Create/Init/Destruct Commands ///////////////////////

	BOOL CTxtWinHost::Init(CRichEditUI *re, const CREATESTRUCT *pcs)
	{
		IUnknown *pUnk = NULL;
		HRESULT hr;

		m_re = re;
		// Initialize Reference count
		cRefs = 1;

		// Create and cache CHARFORMAT for this control
		if(FAILED(InitDefaultCharFormat(re, &cf, NULL)))
			goto err;

		// Create and cache PARAFORMAT for this control
		if(FAILED(InitDefaultParaFormat(re, &pf)))
			goto err;

		// edit controls created without a window are multiline by default
		// so that paragraph formats can be
		dwStyle = ES_MULTILINE;

		// edit controls are rich by default
		fRich = re->IsRich();

		cchTextMost = re->GetLimitText();

		if (pcs )
		{
			dwStyle = pcs->style;

			if ( !(dwStyle & (ES_AUTOHSCROLL | WS_HSCROLL)) )
			{
				fWordWrap = TRUE;
			}
		}

		if( !(dwStyle & ES_LEFT) )
		{
			if(dwStyle & ES_CENTER)
				pf.wAlignment = PFA_CENTER;
			else if(dwStyle & ES_RIGHT)
				pf.wAlignment = PFA_RIGHT;
		}

		fInplaceActive = TRUE;

		PCreateTextServices TextServicesProc = NULL;
		
// #ifdef _UNICODE		
// 		HMODULE hmod = LoadLibrary(_T("Msftedit.dll"));
// #else
// 		HMODULE hmod = LoadLibrary(_T("Riched20.dll"));
// #endif		
////////////////////////////////////////////////////////////////////////////modify by liqs99
		HMODULE hmod = NULL;//LoadLibrary(_T("Riched20.dll"));
		if(IsWindows7OrGreater())
			hmod = LoadLibrary(_T("Riched20.dll"));
		else
			hmod = LoadLibrary(_T("Msftedit.dll"));
//////////////////////////////////////////////////////////////////////////
		if (hmod) {
			TextServicesProc = (PCreateTextServices)GetProcAddress(hmod,"CreateTextServices");
		}
		if (TextServicesProc != NULL) {
			HRESULT hr = TextServicesProc(NULL, this, &pUnk);
		}

		hr = pUnk->QueryInterface(IID_ITextServices,(void **)&pserv);

		// Whether the previous call succeeded or failed we are done
		// with the private interface.
		pUnk->Release();

		if(FAILED(hr))
		{
			goto err;
		}

		// Set window text
		if(pcs && pcs->lpszName)
		{
#ifdef _UNICODE		
			if(FAILED(pserv->TxSetText((TCHAR *)pcs->lpszName)))
				goto err;
#else
			size_t iLen = _tcslen(pcs->lpszName);
			LPWSTR lpText = new WCHAR[iLen + 1];
			::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
			::MultiByteToWideChar(CP_ACP, 0, pcs->lpszName, -1, (LPWSTR)lpText, iLen) ;
			if(FAILED(pserv->TxSetText((LPWSTR)lpText))) {
				delete[] lpText;
				goto err;
			}
			delete[] lpText;
#endif
		}

		return TRUE;

err:
		return FALSE;
	}

	/////////////////////////////////  IUnknown ////////////////////////////////


	HRESULT CTxtWinHost::QueryInterface(REFIID riid, void **ppvObject)
	{
		HRESULT hr = E_NOINTERFACE;
		*ppvObject = NULL;

		if (IsEqualIID(riid, IID_IUnknown) 
			|| IsEqualIID(riid, IID_ITextHost)) 
		{
			AddRef();
			*ppvObject = (ITextHost *) this;
			hr = S_OK;
		}

		return hr;
	}

	ULONG CTxtWinHost::AddRef(void)
	{
		return ++cRefs;
	}

	ULONG CTxtWinHost::Release(void)
	{
		ULONG c_Refs = --cRefs;

		if (c_Refs == 0)
		{
			delete this;
		}

		return c_Refs;
	}

	/////////////////////////////////  Far East Support  //////////////////////////////////////

	HIMC CTxtWinHost::TxImmGetContext(void)
	{
		return NULL;
	}

	void CTxtWinHost::TxImmReleaseContext(HIMC himc)
	{
		//::ImmReleaseContext( hwnd, himc );
	}

	//////////////////////////// ITextHost Interface  ////////////////////////////

	HDC CTxtWinHost::TxGetDC()
	{
		return m_re->GetManager()->GetPaintDC();
	}

	int CTxtWinHost::TxReleaseDC(HDC hdc)
	{
		return 1;
	}

	BOOL CTxtWinHost::TxShowScrollBar(INT fnBar, BOOL fShow)
	{
		CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
		CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
		if( fnBar == SB_VERT && pVerticalScrollBar ) {
			pVerticalScrollBar->SetVisible(fShow == TRUE);
		}
		else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
			pHorizontalScrollBar->SetVisible(fShow == TRUE);
		}
		else if( fnBar == SB_BOTH ) {
			if( pVerticalScrollBar ) pVerticalScrollBar->SetVisible(fShow == TRUE);
			if( pHorizontalScrollBar ) pHorizontalScrollBar->SetVisible(fShow == TRUE);
		}
		return TRUE;
	}

	BOOL CTxtWinHost::TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags)
	{
		if( fuSBFlags == SB_VERT ) {
			m_re->EnableScrollBar(true, m_re->GetHorizontalScrollBar() != NULL);
			m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
		}
		else if( fuSBFlags == SB_HORZ ) {
			m_re->EnableScrollBar(m_re->GetVerticalScrollBar() != NULL, true);
			m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
		}
		else if( fuSBFlags == SB_BOTH ) {
			m_re->EnableScrollBar(true, true);
			m_re->GetVerticalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
			m_re->GetHorizontalScrollBar()->SetVisible(fuArrowflags != ESB_DISABLE_BOTH);
		}
		return TRUE;
	}

	BOOL CTxtWinHost::TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw)
	{
		CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
		CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
		if( fnBar == SB_VERT && pVerticalScrollBar ) {
			if( nMaxPos - nMinPos - rcClient.bottom + rcClient.top <= 0 ) {
				pVerticalScrollBar->SetVisible(false);
			}
			else {
				pVerticalScrollBar->SetVisible(true);
				pVerticalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.bottom + rcClient.top);
			}
		}
		else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
			if( nMaxPos - nMinPos - rcClient.right + rcClient.left <= 0 ) {
				pHorizontalScrollBar->SetVisible(false);
			}
			else {
				pHorizontalScrollBar->SetVisible(true);
				pHorizontalScrollBar->SetScrollRange(nMaxPos - nMinPos - rcClient.right + rcClient.left);
			}   
		}
		return TRUE;
	}

	BOOL CTxtWinHost::TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw)
	{
		CScrollBarUI* pVerticalScrollBar = m_re->GetVerticalScrollBar();
		CScrollBarUI* pHorizontalScrollBar = m_re->GetHorizontalScrollBar();
		if( fnBar == SB_VERT && pVerticalScrollBar ) {
			pVerticalScrollBar->SetScrollPos(nPos);
		}
		else if( fnBar == SB_HORZ && pHorizontalScrollBar ) {
			pHorizontalScrollBar->SetScrollPos(nPos);
		}
		return TRUE;
	}

	void CTxtWinHost::TxInvalidateRect(LPCRECT prc, BOOL fMode)
	{
		if( prc == NULL ) {
			m_re->GetManager()->Invalidate(rcClient);
			return;
		}
		RECT rc = *prc;
		m_re->GetManager()->Invalidate(rc);
	}

	void CTxtWinHost::TxViewChange(BOOL fUpdate) 
	{
		if( m_re->OnTxViewChanged() ) m_re->Invalidate();
	}

	BOOL CTxtWinHost::TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight)
	{
		iCaretWidth = xWidth;
		iCaretHeight = yHeight;
		return ::CreateCaret(m_re->GetManager()->GetPaintWindow(), hbmp, xWidth, yHeight);
	}

	BOOL CTxtWinHost::TxShowCaret(BOOL fShow)
	{
		fShowCaret = fShow;

		if(m_re->IsReadOnly()) //只读状态，不显示光标
		{
			return ::HideCaret(m_re->GetManager()->GetPaintWindow());
		}

		if(fShow)
			return ::ShowCaret(m_re->GetManager()->GetPaintWindow());
		else
			return ::HideCaret(m_re->GetManager()->GetPaintWindow());
	}

	BOOL CTxtWinHost::TxSetCaretPos(INT x, INT y)
	{
		POINT ptCaret = { 0 };
		::GetCaretPos(&ptCaret);
		RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x + iCaretLastWidth, ptCaret.y + iCaretLastHeight };
		if( m_re->GetManager()->IsLayered() ) m_re->GetManager()->Invalidate(rcCaret);
		else if( fNeedFreshCaret == TRUE ) {
			m_re->GetManager()->Invalidate(rcCaret);
			fNeedFreshCaret = FALSE;
		}
		rcCaret.left = x;
		rcCaret.top = y;
		rcCaret.right = x + iCaretWidth;
		rcCaret.bottom = y + iCaretHeight;
		if( m_re->GetManager()->IsLayered() ) m_re->GetManager()->Invalidate(rcCaret);
		iCaretLastWidth = iCaretWidth;
		iCaretLastHeight = iCaretHeight;
		return ::SetCaretPos(x, y);
	}

	BOOL CTxtWinHost::TxSetTimer(UINT idTimer, UINT uTimeout)
	{
		fTimer = TRUE;
		return m_re->GetManager()->SetTimer(m_re, idTimer, uTimeout) == TRUE;
	}

	void CTxtWinHost::TxKillTimer(UINT idTimer)
	{
		m_re->GetManager()->KillTimer(m_re, idTimer);
		fTimer = FALSE;
	}

	void CTxtWinHost::TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll,	LPCRECT lprcClip,	HRGN hrgnUpdate, LPRECT lprcUpdate,	UINT fuScroll)	
	{
		return;
	}

	void CTxtWinHost::TxSetCapture(BOOL fCapture)
	{
		if (fCapture) m_re->GetManager()->SetCapture();
		else m_re->GetManager()->ReleaseCapture();
		fCaptured = fCapture;
	}

	void CTxtWinHost::TxSetFocus()
	{
		m_re->SetFocus();
	}

	void CTxtWinHost::TxSetCursor(HCURSOR hcur,	BOOL fText)
	{
		::SetCursor(hcur);
	}

	BOOL CTxtWinHost::TxScreenToClient(LPPOINT lppt)
	{
		return ::ScreenToClient(m_re->GetManager()->GetPaintWindow(), lppt);	
	}

	BOOL CTxtWinHost::TxClientToScreen(LPPOINT lppt)
	{
		return ::ClientToScreen(m_re->GetManager()->GetPaintWindow(), lppt);
	}

	HRESULT CTxtWinHost::TxActivate(LONG *plOldState)
	{
		return S_OK;
	}

	HRESULT CTxtWinHost::TxDeactivate(LONG lNewState)
	{
		return S_OK;
	}

	HRESULT CTxtWinHost::TxGetClientRect(LPRECT prc)
	{
		*prc = rcClient;
		GetControlRect(prc);
		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetViewInset(LPRECT prc) 
	{
		prc->left = prc->right = prc->top = prc->bottom = 0;
		return NOERROR;	
	}

	HRESULT CTxtWinHost::TxGetCharFormat(const CHARFORMATW **ppCF)
	{
		*ppCF = &cf;
		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetParaFormat(const PARAFORMAT **ppPF)
	{
		*ppPF = &pf;
		return NOERROR;
	}

	COLORREF CTxtWinHost::TxGetSysColor(int nIndex) 
	{
		return ::GetSysColor(nIndex);
	}

	HRESULT CTxtWinHost::TxGetBackStyle(TXTBACKSTYLE *pstyle)
	{
		*pstyle = !fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetMaxLength(DWORD *pLength)
	{
		*pLength = cchTextMost;
		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetScrollBars(DWORD *pdwScrollBar)
	{
		*pdwScrollBar =  dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
			ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetPasswordChar(TCHAR *pch)
	{
#ifdef _UNICODE
		*pch = chPasswordChar;
#else
		::WideCharToMultiByte(CP_ACP, 0, &chPasswordChar, 1, pch, 1, NULL, NULL) ;
#endif
		return NOERROR;
	}

	HRESULT CTxtWinHost::TxGetAcceleratorPos(LONG *pcp)
	{
		*pcp = laccelpos;
		return S_OK;
	} 										   

	HRESULT CTxtWinHost::OnTxCharFormatChange(const CHARFORMATW *pcf)
	{
		return S_OK;
	}

	HRESULT CTxtWinHost::OnTxParaFormatChange(const PARAFORMAT *ppf)
	{
		return S_OK;
	}

	HRESULT CTxtWinHost::TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits) 
	{
		DWORD dwProperties = 0;

		if (fRich)
		{
			dwProperties = TXTBIT_RICHTEXT;
		}

		if (dwStyle & ES_MULTILINE)
		{
			dwProperties |= TXTBIT_MULTILINE;
		}

		if (dwStyle & ES_READONLY)
		{
			dwProperties |= TXTBIT_READONLY;
		}

		if (dwStyle & ES_PASSWORD)
		{
			dwProperties |= TXTBIT_USEPASSWORD;
		}

		if (!(dwStyle & ES_NOHIDESEL))
		{
			dwProperties |= TXTBIT_HIDESELECTION;
		}

		if (fEnableAutoWordSel)
		{
			dwProperties |= TXTBIT_AUTOWORDSEL;
		}

		if (fWordWrap)
		{
			dwProperties |= TXTBIT_WORDWRAP;
		}

		if (fAllowBeep)
		{
			dwProperties |= TXTBIT_ALLOWBEEP;
		}

		if (fSaveSelection)
		{
			dwProperties |= TXTBIT_SAVESELECTION;
		}

		*pdwBits = dwProperties & dwMask; 
		return NOERROR;
	}


	HRESULT CTxtWinHost::TxNotify(DWORD iNotify, void *pv)
	{
		if( iNotify == EN_REQUESTRESIZE ) {
			RECT rc;
			REQRESIZE *preqsz = (REQRESIZE *)pv;
			GetControlRect(&rc);
			rc.bottom = rc.top + preqsz->rc.bottom;
			rc.right  = rc.left + preqsz->rc.right;
			SetClientRect(&rc);
			return S_OK;
		}
		m_re->OnTxNotify(iNotify, pv);
		return S_OK;
	}

	HRESULT CTxtWinHost::TxGetExtent(LPSIZEL lpExtent)
	{
		*lpExtent = sizelExtent;
		return S_OK;
	}

	HRESULT	CTxtWinHost::TxGetSelectionBarWidth (LONG *plSelBarWidth)
	{
		*plSelBarWidth = lSelBarWidth;
		return S_OK;
	}

	void CTxtWinHost::SetWordWrap(BOOL fWordWrap)
	{
		fWordWrap = fWordWrap;
		pserv->OnTxPropertyBitsChange(TXTBIT_WORDWRAP, fWordWrap ? TXTBIT_WORDWRAP : 0);
	}

	BOOL CTxtWinHost::IsReadOnly()
	{
		return (dwStyle & ES_READONLY) != 0;
	}

	void CTxtWinHost::SetReadOnly(BOOL fReadOnly)
	{
		if (fReadOnly)
		{
			dwStyle |= ES_READONLY;
		}
		else
		{
			dwStyle &= ~ES_READONLY;
		}

		pserv->OnTxPropertyBitsChange(TXTBIT_READONLY, 
			fReadOnly ? TXTBIT_READONLY : 0);
	}

	void CTxtWinHost::SetFont(HFONT hFont) 
	{
		if( hFont == NULL ) return;
		LOGFONT lf;
		::GetObject(hFont, sizeof(LOGFONT), &lf);
		LONG yPixPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY);
		cf.yHeight = -lf.lfHeight * LY_PER_INCH / yPixPerInch;
		if(lf.lfWeight >= FW_BOLD) cf.dwEffects |= CFE_BOLD;
		else cf.dwEffects &= ~CFE_BOLD;
		if(lf.lfItalic) cf.dwEffects |= CFE_ITALIC;
		else cf.dwEffects &= ~CFE_ITALIC;
		if(lf.lfUnderline) cf.dwEffects |= CFE_UNDERLINE;
		else cf.dwEffects &= ~CFE_UNDERLINE;
		cf.bCharSet = lf.lfCharSet;
		cf.bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef _UNICODE
		_tcscpy(cf.szFaceName, lf.lfFaceName);
#else
		//need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
		MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, cf.szFaceName, LF_FACESIZE) ;
#endif

		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
			TXTBIT_CHARFORMATCHANGE);
	}

	void CTxtWinHost::SetColor(DWORD dwColor)
	{
		cf.crTextColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
			TXTBIT_CHARFORMATCHANGE);
	}

	SIZEL* CTxtWinHost::GetExtent() 
	{
		return &sizelExtent;
	}

	void CTxtWinHost::SetExtent(SIZEL *psizelExtent) 
	{ 
		sizelExtent = *psizelExtent; 
		pserv->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE);
	}

	void CTxtWinHost::LimitText(LONG nChars)
	{
		cchTextMost = nChars;
		if( cchTextMost <= 0 ) cchTextMost = cInitTextMax;
		pserv->OnTxPropertyBitsChange(TXTBIT_MAXLENGTHCHANGE, TXTBIT_MAXLENGTHCHANGE);
	}

	BOOL CTxtWinHost::IsCaptured()
	{
		return fCaptured;
	}

	BOOL CTxtWinHost::IsShowCaret()
	{
		return fShowCaret;
	}

	void CTxtWinHost::NeedFreshCaret()
	{
		fNeedFreshCaret = TRUE;
	}

	INT CTxtWinHost::GetCaretWidth()
	{
		return iCaretWidth;
	}

	INT CTxtWinHost::GetCaretHeight()
	{
		return iCaretHeight;
	}

	BOOL CTxtWinHost::GetAllowBeep()
	{
		return fAllowBeep;
	}

	void CTxtWinHost::SetAllowBeep(BOOL fAllowBeep)
	{
		fAllowBeep = fAllowBeep;

		pserv->OnTxPropertyBitsChange(TXTBIT_ALLOWBEEP, 
			fAllowBeep ? TXTBIT_ALLOWBEEP : 0);
	}

	WORD CTxtWinHost::GetDefaultAlign()
	{
		return pf.wAlignment;
	}

	void CTxtWinHost::SetDefaultAlign(WORD wNewAlign)
	{
		pf.wAlignment = wNewAlign;

		// Notify control of property change
		pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}

	BOOL CTxtWinHost::GetRichTextFlag()
	{
		return fRich;
	}

	void CTxtWinHost::SetRichTextFlag(BOOL fNew)
	{
		fRich = fNew;

		pserv->OnTxPropertyBitsChange(TXTBIT_RICHTEXT, 
			fNew ? TXTBIT_RICHTEXT : 0);
	}

	LONG CTxtWinHost::GetDefaultLeftIndent()
	{
		return pf.dxOffset;
	}

	void CTxtWinHost::SetDefaultLeftIndent(LONG lNewIndent)
	{
		pf.dxOffset = lNewIndent;

		pserv->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
	}

	void CTxtWinHost::SetClientRect(RECT *prc) 
	{
		rcClient = *prc;

		LONG xPerInch = ::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSX); 
		LONG yPerInch =	::GetDeviceCaps(m_re->GetManager()->GetPaintDC(), LOGPIXELSY); 
		sizelExtent.cx = DXtoHimetricX(rcClient.right - rcClient.left, xPerInch);
		sizelExtent.cy = DYtoHimetricY(rcClient.bottom - rcClient.top, yPerInch);

		pserv->OnTxPropertyBitsChange(TXTBIT_VIEWINSETCHANGE, TXTBIT_VIEWINSETCHANGE);
	}

	BOOL CTxtWinHost::SetSaveSelection(BOOL f_SaveSelection)
	{
		BOOL fResult = f_SaveSelection;

		fSaveSelection = f_SaveSelection;

		// notify text services of property change
		pserv->OnTxPropertyBitsChange(TXTBIT_SAVESELECTION, 
			fSaveSelection ? TXTBIT_SAVESELECTION : 0);

		return fResult;		
	}

	HRESULT	CTxtWinHost::OnTxInPlaceDeactivate()
	{
		HRESULT hr = pserv->OnTxInPlaceDeactivate();

		if (SUCCEEDED(hr))
		{
			fInplaceActive = FALSE;
		}

		return hr;
	}

	HRESULT	CTxtWinHost::OnTxInPlaceActivate(LPCRECT prcClient)
	{
		fInplaceActive = TRUE;

		HRESULT hr = pserv->OnTxInPlaceActivate(prcClient);

		if (FAILED(hr))
		{
			fInplaceActive = FALSE;
		}

		return hr;
	}

	BOOL CTxtWinHost::DoSetCursor(RECT *prc, POINT *pt)
	{
		RECT rc = prc ? *prc : rcClient;

		// Is this in our rectangle?
		if (PtInRect(&rc, *pt))
		{
			RECT *prcClient = (!fInplaceActive || prc) ? &rc : NULL;
			pserv->OnTxSetCursor(DVASPECT_CONTENT,	-1, NULL, NULL,  m_re->GetManager()->GetPaintDC(),
				NULL, prcClient, pt->x, pt->y);

			return TRUE;
		}

		return FALSE;
	}

	void CTxtWinHost::GetControlRect(LPRECT prc)
	{
		prc->top = rcClient.top;
		prc->bottom = rcClient.bottom;
		prc->left = rcClient.left;
		prc->right = rcClient.right;
	}

	void CTxtWinHost::SetTransparent(BOOL f_Transparent)
	{
		fTransparent = f_Transparent;

		// notify text services of property change
		pserv->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, 0);
	}

	LONG CTxtWinHost::SetAccelPos(LONG l_accelpos)
	{
		LONG laccelposOld = l_accelpos;

		laccelpos = l_accelpos;

		// notify text services of property change
		pserv->OnTxPropertyBitsChange(TXTBIT_SHOWACCELERATOR, 0);

		return laccelposOld;
	}

	WCHAR CTxtWinHost::SetPasswordChar(WCHAR ch_PasswordChar)
	{
		WCHAR chOldPasswordChar = chPasswordChar;

		chPasswordChar = ch_PasswordChar;

		// notify text services of property change
		pserv->OnTxPropertyBitsChange(TXTBIT_USEPASSWORD, 
			(chPasswordChar != 0) ? TXTBIT_USEPASSWORD : 0);

		return chOldPasswordChar;
	}

	void CTxtWinHost::SetDisabled(BOOL fOn)
	{
		cf.dwMask	 |= CFM_COLOR | CFM_DISABLED;
		cf.dwEffects |= CFE_AUTOCOLOR | CFE_DISABLED;

		if( !fOn )
		{
			cf.dwEffects &= ~CFE_DISABLED;
		}

		pserv->OnTxPropertyBitsChange(TXTBIT_CHARFORMATCHANGE, 
			TXTBIT_CHARFORMATCHANGE);
	}

	LONG CTxtWinHost::SetSelBarWidth(LONG l_SelBarWidth)
	{
		LONG lOldSelBarWidth = lSelBarWidth;

		lSelBarWidth = l_SelBarWidth;

		if (lSelBarWidth)
		{
			dwStyle |= ES_SELECTIONBAR;
		}
		else
		{
			dwStyle &= (~ES_SELECTIONBAR);
		}

		pserv->OnTxPropertyBitsChange(TXTBIT_SELBARCHANGE, TXTBIT_SELBARCHANGE);

		return lOldSelBarWidth;
	}

	BOOL CTxtWinHost::GetTimerState()
	{
		return fTimer;
	}

	void CTxtWinHost::SetParagraphIndent(LONG lIdentSize)
	{
		pf.dxStartIndent = lIdentSize;
		pf.dxOffset = -lIdentSize;
	}

	void CTxtWinHost::SetCharFormat(CHARFORMAT2W &c)
	{
		cf = c;
	}

	void CTxtWinHost::SetParaFormat(PARAFORMAT2 &p)
	{
		pf = p;
	}

} // namespace DuiLib
