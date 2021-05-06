#ifndef __TXTWINHOST_H__
#define __TXTWINHOST_H__

#pragma once
#ifdef _USEIMM
#include <imm.h>
#pragma comment(lib, "imm32.lib")
#endif

#ifndef LY_PER_INCH
#define LY_PER_INCH 1440
#endif

#ifndef HIMETRIC_PER_INCH
#define HIMETRIC_PER_INCH 2540
#endif

#include <textserv.h>

namespace DuiLib {

	class CTxtWinHost : public ITextHost
	{
	public:
		CTxtWinHost();
		BOOL Init(CRichEditUI *re , const CREATESTRUCT *pcs);
		virtual ~CTxtWinHost();

		ITextServices* GetTextServices(void) { return pserv; }
		void SetClientRect(RECT *prc);
		RECT* GetClientRect() { return &rcClient; }
		BOOL IsWordWrap(void) { return fWordWrap; }
		void SetWordWrap(BOOL fWordWrap);
		BOOL IsReadOnly();
		void SetReadOnly(BOOL fReadOnly);

		void SetFont(HFONT hFont);
		void SetColor(DWORD dwColor);

		SIZEL* GetExtent();
		void SetExtent(SIZEL *psizelExtent);

		void LimitText(LONG nChars);

		BOOL IsCaptured();

		BOOL IsShowCaret();

		void NeedFreshCaret();

		INT GetCaretWidth();
		INT GetCaretHeight();

		BOOL GetAllowBeep();
		void SetAllowBeep(BOOL fAllowBeep);

		WORD GetDefaultAlign();
		void SetDefaultAlign(WORD wNewAlign);

		BOOL GetRichTextFlag();
		void SetRichTextFlag(BOOL fNew);

		LONG GetDefaultLeftIndent();
		void SetDefaultLeftIndent(LONG lNewIndent);

		BOOL SetSaveSelection(BOOL fSaveSelection);

		HRESULT OnTxInPlaceDeactivate();
		HRESULT OnTxInPlaceActivate(LPCRECT prcClient);

		BOOL GetActiveState(void) { return fInplaceActive; }

		BOOL DoSetCursor(RECT *prc, POINT *pt);

		void SetTransparent(BOOL fTransparent);

		void GetControlRect(LPRECT prc);

		LONG SetAccelPos(LONG laccelpos);

		WCHAR SetPasswordChar(WCHAR chPasswordChar);

		void SetDisabled(BOOL fOn);

		LONG SetSelBarWidth(LONG lSelBarWidth);

		BOOL GetTimerState();

		void SetParagraphIndent(LONG lIdentSize);

		void SetCharFormat(CHARFORMAT2W &c);

		void SetParaFormat(PARAFORMAT2 &p);

		// -----------------------------
		//	IUnknown interface
		// -----------------------------
		virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
		virtual ULONG _stdcall AddRef(void);
		virtual ULONG _stdcall Release(void);

		// -----------------------------
		//	ITextHost interface
		// -----------------------------
		virtual HDC TxGetDC();
		virtual INT TxReleaseDC(HDC hdc);
		virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);
		virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
		virtual BOOL TxSetScrollRange(INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw);
		virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
		virtual void TxInvalidateRect(LPCRECT prc, BOOL fMode);
		virtual void TxViewChange(BOOL fUpdate);
		virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
		virtual BOOL TxShowCaret(BOOL fShow);
		virtual BOOL TxSetCaretPos(INT x, INT y);
		virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
		virtual void TxKillTimer(UINT idTimer);
		virtual void TxScrollWindowEx (INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll);
		virtual void TxSetCapture(BOOL fCapture);
		virtual void TxSetFocus();
		virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
		virtual BOOL TxScreenToClient (LPPOINT lppt);
		virtual BOOL TxClientToScreen (LPPOINT lppt);
		virtual HRESULT TxActivate( LONG * plOldState );
		virtual HRESULT TxDeactivate( LONG lNewState );
		virtual HRESULT TxGetClientRect(LPRECT prc);
		virtual HRESULT TxGetViewInset(LPRECT prc);
		virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
		virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
		virtual COLORREF TxGetSysColor(int nIndex);
		virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
		virtual HRESULT TxGetMaxLength(DWORD *plength);
		virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
		virtual HRESULT TxGetPasswordChar(TCHAR *pch);
		virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
		virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
		virtual HRESULT OnTxCharFormatChange (const CHARFORMATW * pcf);
		virtual HRESULT OnTxParaFormatChange (const PARAFORMAT * ppf);
		virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
		virtual HRESULT TxNotify(DWORD iNotify, void *pv);
		virtual HIMC TxImmGetContext(void);
		virtual void TxImmReleaseContext(HIMC himc);
		virtual HRESULT TxGetSelectionBarWidth (LONG *lSelBarWidth);

	private:
		CRichEditUI *m_re;
		ULONG	cRefs;					// Reference Count
		ITextServices	*pserv;		    // pointer to Text Services object
		// Properties

		DWORD		dwStyle;				// style bits

		unsigned	fEnableAutoWordSel	:1;	// enable Word style auto word selection?
		unsigned	fWordWrap			:1;	// Whether control should word wrap
		unsigned	fAllowBeep			:1;	// Whether beep is allowed
		unsigned	fRich				:1;	// Whether control is rich text
		unsigned	fSaveSelection		:1;	// Whether to save the selection when inactive
		unsigned	fInplaceActive		:1; // Whether control is inplace active
		unsigned	fTransparent		:1; // Whether control is transparent
		unsigned	fTimer				:1;	// A timer is set
		unsigned    fCaptured           :1;
		unsigned    fShowCaret          :1;
		unsigned    fNeedFreshCaret     :1; // 修正改变大小后点击其他位置原来光标不能消除的问题

		INT         iCaretWidth;
		INT         iCaretHeight;
		INT         iCaretLastWidth;
		INT         iCaretLastHeight;
		LONG		lSelBarWidth;			// Width of the selection bar
		LONG  		cchTextMost;			// maximum text size
		DWORD		dwEventMask;			// DoEvent mask to pass on to parent window
		LONG		icf;
		LONG		ipf;
		RECT		rcClient;				// Client Rect for this control
		SIZEL		sizelExtent;			// Extent array
		CHARFORMAT2W cf;					// Default character format
		PARAFORMAT2	pf;					    // Default paragraph format
		LONG		laccelpos;				// Accelerator position
		WCHAR		chPasswordChar;		    // Password character
	};

} // namespace DuiLib

#endif // __TXTWINHOST_H__
