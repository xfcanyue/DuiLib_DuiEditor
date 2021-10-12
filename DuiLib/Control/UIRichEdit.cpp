#include "StdAfx.h"
#include "UIRichEdit.h"
#include "TxtWinHost.h"

namespace DuiLib {

#define ID_RICH_UNDO			101
#define ID_RICH_CUT				102
#define ID_RICH_COPY			103
#define ID_RICH_PASTE			104
#define ID_RICH_CLEAR			105
#define ID_RICH_SELECTALL		106
#define ID_RICH_REDO			107

	const LONG cInitTextMax = (32 * 1024) - 1;

	HRESULT CreateHost(CRichEditUI *re, const CREATESTRUCT *pcs, CTxtWinHost **pptec)
	{
		HRESULT hr = E_FAIL;
		CTxtWinHost *phost = new CTxtWinHost();
		if(phost) {
			if (phost->Init(re, pcs)) {
				*pptec = phost;
				hr = S_OK;
			}
		}

		if (FAILED(hr)) {
			delete phost;
		}

		return TRUE;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CRichEditUI)
		// 		CRichEditUI::CRichEditUI() : m_pTwh(NULL), m_bVScrollBarFixing(false), m_bWantTab(true), m_bWantReturn(true), 
		// 		m_bWantCtrlReturn(true), m_bTransparent(true), m_bRich(true), m_bReadOnly(false), m_bWordWrap(false), m_dwTextColor(0), m_iFont(-1), 
		// 		m_iLimitText(cInitTextMax), m_lTwhStyle(ES_MULTILINE), m_bDrawCaret(true), m_bInited(false), m_chLeadByte(0),m_uButtonState(0),
		// 		m_dwTipValueColor(0xFFBAC0C5), m_uTipValueAlign(DT_SINGLELINE | DT_LEFT)

		//modify by liqs99 
		CRichEditUI::CRichEditUI() : m_pTwh(NULL), m_bVScrollBarFixing(false), m_bWantTab(true), m_bWantReturn(false), 
		m_bWantCtrlReturn(false), m_bTransparent(true), m_bRich(false), m_bReadOnly(false), m_bWordWrap(false), m_bParagraphIndent(false), m_dwTextColor(0), m_iFont(-1), 
		m_iLimitText(cInitTextMax), m_lTwhStyle(ES_LEFT|ES_VERTICAL|ECO_VERTICAL), m_bDrawCaret(true), m_bInited(false), m_chLeadByte(0),
		m_dwTipValueColor(0xFFBAC0C5), m_uTipValueAlign(DT_SINGLELINE | DT_LEFT), m_uTextStyle(DT_TOP), 
		m_bReturnFixedWidth(true), m_bReturnFixedHeight(true)
	{
#ifndef _UNICODE
		m_fAccumulateDBC =true;
#else
		m_fAccumulateDBC= false;
#endif
		::ZeroMemory(&m_rcTextPadding, sizeof(m_rcTextPadding));
	}

	CRichEditUI::~CRichEditUI()
	{
		if( m_pTwh ) {
			m_pTwh->Release();
			m_pManager->RemoveMessageFilter(this);
		}
	}

	LPCTSTR CRichEditUI::GetClass() const
	{
		return DUI_CTR_RICHEDIT;
	}

	LPVOID CRichEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_RICHEDIT) == 0 ) return static_cast<CRichEditUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	UINT CRichEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	bool CRichEditUI::IsMultiLine()
	{
		return (m_lTwhStyle & ES_MULTILINE) == ES_MULTILINE;
	}

	void CRichEditUI::SetMultiLine(bool bMultiLine)
	{
		if(!bMultiLine) m_lTwhStyle &= ~ES_MULTILINE;
		else  m_lTwhStyle |= ES_MULTILINE;
	}

	bool CRichEditUI::IsWantTab()
	{
		return m_bWantTab;
	}

	void CRichEditUI::SetWantTab(bool bWantTab)
	{
		m_bWantTab = bWantTab;
	}

	bool CRichEditUI::IsWantReturn()
	{
		return m_bWantReturn;
	}

	void CRichEditUI::SetWantReturn(bool bWantReturn)
	{
		m_bWantReturn = bWantReturn;
	}

	bool CRichEditUI::IsWantCtrlReturn()
	{
		return m_bWantCtrlReturn;
	}

	void CRichEditUI::SetWantCtrlReturn(bool bWantCtrlReturn)
	{
		m_bWantCtrlReturn = bWantCtrlReturn;
	}

	bool CRichEditUI::IsTransparent()
	{
		return m_bTransparent;
	}

	void CRichEditUI::SetTransparent(bool bTransparent)
	{
		m_bTransparent = bTransparent;
		if( m_pTwh ) m_pTwh->SetTransparent(bTransparent);
	}

	bool CRichEditUI::IsRich()
	{
		return m_bRich;
	}

	void CRichEditUI::SetRich(bool bRich)
	{
		m_bRich = bRich;
		if( m_pTwh ) m_pTwh->SetRichTextFlag(bRich);
	}

	bool CRichEditUI::IsReadOnly()
	{
		return m_bReadOnly;
	}

	void CRichEditUI::SetReadOnly(bool bReadOnly)
	{
		m_lTwhStyle |= ES_READONLY; 
		m_bReadOnly = bReadOnly;
		if( m_pTwh ) m_pTwh->SetReadOnly(bReadOnly);
	}

	bool CRichEditUI::IsWordWrap()
	{
		return m_bWordWrap;
	}

	void CRichEditUI::SetWordWrap(bool bWordWrap)
	{
		m_bWordWrap = bWordWrap;
		if( m_pTwh ) m_pTwh->SetWordWrap(bWordWrap);
	}

	bool CRichEditUI::IsParagraphIndent()
	{
		return m_bParagraphIndent;
	}

	void CRichEditUI::SetParagraphIndent(bool bIndent)
	{
		m_bParagraphIndent = bIndent;
		if(m_pTwh)
		{
			const CHARFORMATW *cf;
			m_pTwh->TxGetCharFormat(&cf);
			m_pTwh->SetParagraphIndent(cf->yHeight * 2);
			m_pTwh->GetTextServices()->OnTxPropertyBitsChange(TXTBIT_PARAFORMATCHANGE, 0);
		}
	}

	int CRichEditUI::GetFont()
	{
		return m_iFont;
	}

	void CRichEditUI::SetFont(int index)
	{
		m_iFont = index;
		if( m_pTwh ) {
			m_pTwh->SetFont(GetManager()->GetFont(m_iFont));
		}
	}

	void CRichEditUI::SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
	{
		if( m_pTwh ) {
			LOGFONT lf = { 0 };
			::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			_tcsncpy(lf.lfFaceName, pStrFontName, LF_FACESIZE);
			lf.lfCharSet = DEFAULT_CHARSET;
			lf.lfHeight = -nSize;
			if( bBold ) lf.lfWeight += FW_BOLD;
			if( bUnderline ) lf.lfUnderline = TRUE;
			if( bItalic ) lf.lfItalic = TRUE;
			HFONT hFont = ::CreateFontIndirect(&lf);
			if( hFont == NULL ) return;
			m_pTwh->SetFont(hFont);
			::DeleteObject(hFont);
		}
	}

	LONG CRichEditUI::GetWinStyle()
	{
		return m_lTwhStyle;
	}

	void CRichEditUI::SetWinStyle(LONG lStyle)
	{
		m_lTwhStyle = lStyle;
	}

	DWORD CRichEditUI::GetTextColor()
	{
		return m_dwTextColor;
	}

	void CRichEditUI::SetTextColor(DWORD dwTextColor)
	{
		m_dwTextColor = dwTextColor;
		if( m_pTwh ) {
			m_pTwh->SetColor(dwTextColor);
		}
	}

	int CRichEditUI::GetLimitText()
	{
		return m_iLimitText;
	}

	void CRichEditUI::SetLimitText(int iChars)
	{
		m_iLimitText = iChars;
		if( m_pTwh ) {
			m_pTwh->LimitText(m_iLimitText);
		}
	}

	long CRichEditUI::GetTextLength(DWORD dwFlags) const
	{
		GETTEXTLENGTHEX textLenEx;
		textLenEx.flags = dwFlags;
#ifdef _UNICODE
		textLenEx.codepage = 1200;
#else
		textLenEx.codepage = CP_ACP;
#endif
		LRESULT lResult;
		HRESULT lSendMessage = TxSendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&textLenEx, 0, &lResult);
		if(!SUCCEEDED(lSendMessage)) return 0;
		return (long)lResult;
	}

	CDuiString CRichEditUI::GetText() const
	{
		long lLen = GetTextLength(GTL_DEFAULT);
		LPTSTR lpText = NULL;
		GETTEXTEX gt;
		gt.flags = GT_DEFAULT;
#ifdef _UNICODE
		gt.cb = sizeof(TCHAR) * (lLen + 1) ;
		gt.codepage = 1200;
		lpText = new TCHAR[lLen + 1];
		::ZeroMemory(lpText, (lLen + 1) * sizeof(TCHAR));
#else
		gt.cb = sizeof(TCHAR) * lLen * 2 + 1;
		gt.codepage = CP_ACP;
		lpText = new TCHAR[lLen * 2 + 1];
		::ZeroMemory(lpText, (lLen * 2 + 1) * sizeof(TCHAR));
#endif
		gt.lpDefaultChar = NULL;
		gt.lpUsedDefChar = NULL;
		TxSendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)lpText, 0);
		CDuiString sText(lpText);
		delete[] lpText;
		return sText;
	}

	void CRichEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( !m_pTwh ) return;
		SetSel(0, -1);
		ReplaceSel(pstrText, FALSE);
	}

	bool CRichEditUI::IsModify() const
	{ 
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_GETMODIFY, 0, 0, &lResult);
		return (BOOL)lResult == TRUE;
	}

	void CRichEditUI::SetModify(bool bModified) const
	{ 
		TxSendMessage(EM_SETMODIFY, bModified, 0, 0);
	}

	void CRichEditUI::GetSel(CHARRANGE &cr) const
	{ 
		TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
	}

	void CRichEditUI::GetSel(long& nStartChar, long& nEndChar) const
	{
		CHARRANGE cr;
		TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0); 
		nStartChar = cr.cpMin;
		nEndChar = cr.cpMax;
	}

	int CRichEditUI::SetSel(CHARRANGE &cr)
	{ 
		LRESULT lResult;
		TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
		return (int)lResult;
	}

	int CRichEditUI::SetSel(long nStartChar, long nEndChar)
	{
		CHARRANGE cr;
		cr.cpMin = nStartChar;
		cr.cpMax = nEndChar;
		LRESULT lResult;
		TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lResult); 
		return (int)lResult;
	}

	void CRichEditUI::ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo)
	{
#ifdef _UNICODE		
		TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
#else
		int iLen = _tcslen(lpszNewText);
		LPWSTR lpText = new WCHAR[iLen + 1];
		::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
		::MultiByteToWideChar(CP_ACP, 0, lpszNewText, -1, (LPWSTR)lpText, iLen) ;
		TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpText, 0); 
		delete[] lpText;
#endif
	}

	void CRichEditUI::ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo)
	{
		TxSendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText, 0); 
	}

	CDuiString CRichEditUI::GetSelText() const
	{
		if( !m_pTwh ) return CDuiString();
		CHARRANGE cr;
		cr.cpMin = cr.cpMax = 0;
		TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
		LPWSTR lpText = NULL;
		lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
		::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));
		TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
		CDuiString sText;
		sText = (LPCWSTR)lpText;
		delete[] lpText;
		return sText;
	}

	int CRichEditUI::SetSelAll()
	{
		return SetSel(0, -1);
	}

	int CRichEditUI::SetSelNone()
	{
		return SetSel(-1, 0);
	}

	bool CRichEditUI::GetZoom(int& nNum, int& nDen) const
	{
		LRESULT lResult;
		TxSendMessage(EM_GETZOOM, (WPARAM)&nNum, (LPARAM)&nDen, &lResult);
		return (BOOL)lResult == TRUE;
	}

	bool CRichEditUI::SetZoom(int nNum, int nDen)
	{
		if (nNum < 0 || nNum > 64) return false;
		if (nDen < 0 || nDen > 64) return false;
		LRESULT lResult;
		TxSendMessage(EM_SETZOOM, nNum, nDen, &lResult);
		return (BOOL)lResult == TRUE;
	}

	bool CRichEditUI::SetZoomOff()
	{
		LRESULT lResult;
		TxSendMessage(EM_SETZOOM, 0, 0, &lResult);
		return (BOOL)lResult == TRUE;
	}

	WORD CRichEditUI::GetSelectionType() const
	{
		LRESULT lResult;
		TxSendMessage(EM_SELECTIONTYPE, 0, 0, &lResult);
		return (WORD)lResult;
	}

	bool CRichEditUI::GetAutoURLDetect() const
	{
		LRESULT lResult;
		TxSendMessage(EM_GETAUTOURLDETECT, 0, 0, &lResult);
		return (BOOL)lResult == TRUE;
	}

	bool CRichEditUI::SetAutoURLDetect(bool bAutoDetect)
	{
		LRESULT lResult;
		TxSendMessage(EM_AUTOURLDETECT, bAutoDetect, 0, &lResult);
		return (BOOL)lResult == FALSE;
	}

	DWORD CRichEditUI::GetEventMask() const
	{
		LRESULT lResult;
		TxSendMessage(EM_GETEVENTMASK, 0, 0, &lResult);
		return (DWORD)lResult;
	}

	DWORD CRichEditUI::SetEventMask(DWORD dwEventMask)
	{
		LRESULT lResult;
		TxSendMessage(EM_SETEVENTMASK, 0, dwEventMask, &lResult);
		return (DWORD)lResult;
	}

	CDuiString CRichEditUI::GetTextRange(long nStartChar, long nEndChar) const
	{
		TEXTRANGEW tr = { 0 };
		tr.chrg.cpMin = nStartChar;
		tr.chrg.cpMax = nEndChar;
		LPWSTR lpText = NULL;
		lpText = new WCHAR[nEndChar - nStartChar + 1];
		::ZeroMemory(lpText, (nEndChar - nStartChar + 1) * sizeof(WCHAR));
		tr.lpstrText = lpText;
		TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, 0);
		CDuiString sText;
		sText = (LPCWSTR)lpText;
		delete[] lpText;
		return sText;
	}

	void CRichEditUI::HideSelection(bool bHide, bool bChangeStyle)
	{
		TxSendMessage(EM_HIDESELECTION, bHide, bChangeStyle, 0);
	}

	void CRichEditUI::ScrollCaret()
	{
		TxSendMessage(EM_SCROLLCARET, 0, 0, 0);
	}

	int CRichEditUI::InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo)
	{
		int nRet = SetSel(nInsertAfterChar, nInsertAfterChar);
		ReplaceSel(lpstrText, bCanUndo);
		return nRet;
	}

	int CRichEditUI::AppendText(LPCTSTR lpstrText, bool bCanUndo)
	{
		int nRet = SetSel(-1, -1);
		ReplaceSel(lpstrText, bCanUndo);
		return nRet;
	}

	DWORD CRichEditUI::GetDefaultCharFormat(CHARFORMAT2 &cf) const
	{
		cf.cbSize = sizeof(CHARFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
		return (DWORD)lResult;
	}

	bool CRichEditUI::SetDefaultCharFormat(CHARFORMAT2 &cf)
	{
		if( !m_pTwh ) return false;
		cf.cbSize = sizeof(CHARFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lResult);
		if( (BOOL)lResult == TRUE ) {
			CHARFORMAT2W cfw;
			cfw.cbSize = sizeof(CHARFORMAT2W);
			TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cfw, 0);
			m_pTwh->SetCharFormat(cfw);
			return true;
		}
		return false;
	}

	DWORD CRichEditUI::GetSelectionCharFormat(CHARFORMAT2 &cf) const
	{
		cf.cbSize = sizeof(CHARFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lResult);
		return (DWORD)lResult;
	}

	bool CRichEditUI::SetSelectionCharFormat(CHARFORMAT2 &cf)
	{
		if( !m_pTwh ) return false;
		cf.cbSize = sizeof(CHARFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lResult);
		return (BOOL)lResult == TRUE;
	}

	bool CRichEditUI::SetWordCharFormat(CHARFORMAT2 &cf)
	{
		if( !m_pTwh ) return false;
		cf.cbSize = sizeof(CHARFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION|SCF_WORD, (LPARAM)&cf, &lResult); 
		return (BOOL)lResult == TRUE;
	}

	DWORD CRichEditUI::GetParaFormat(PARAFORMAT2 &pf) const
	{
		pf.cbSize = sizeof(PARAFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
		return (DWORD)lResult;
	}

	bool CRichEditUI::SetParaFormat(PARAFORMAT2 &pf)
	{
		if( !m_pTwh ) return false;
		pf.cbSize = sizeof(PARAFORMAT2);
		LRESULT lResult;
		TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf, &lResult);
		if( (BOOL)lResult == TRUE ) {
			m_pTwh->SetParaFormat(pf);
			return true;
		}
		return false;
	}

	bool CRichEditUI::CanUndo()
	{
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_CANUNDO, 0, 0, &lResult);
		return (BOOL)lResult == TRUE; 
	}

	bool CRichEditUI::CanRedo()
	{
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_CANREDO, 0, 0, &lResult);
		return (BOOL)lResult == TRUE; 
	}

	bool CRichEditUI::CanPaste()
	{
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_CANPASTE, 0, 0, &lResult);
		return (BOOL)lResult == TRUE; 
	}
	bool CRichEditUI::Redo()
	{ 
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_REDO, 0, 0, &lResult);
		return (BOOL)lResult == TRUE; 
	}

	bool CRichEditUI::Undo()
	{ 
		if( !m_pTwh ) return false;
		LRESULT lResult;
		TxSendMessage(EM_UNDO, 0, 0, &lResult);
		return (BOOL)lResult == TRUE; 
	}

	void CRichEditUI::Clear()
	{ 
		TxSendMessage(WM_CLEAR, 0, 0, 0); 
	}

	void CRichEditUI::Copy()
	{ 
		TxSendMessage(WM_COPY, 0, 0, 0); 
	}

	void CRichEditUI::Cut()
	{ 
		TxSendMessage(WM_CUT, 0, 0, 0); 
	}

	void CRichEditUI::Paste()
	{ 
		TxSendMessage(WM_PASTE, 0, 0, 0); 
	}

	int CRichEditUI::GetLineCount() const
	{ 
		if( !m_pTwh ) return 0;
		LRESULT lResult;
		TxSendMessage(EM_GETLINECOUNT, 0, 0, &lResult);
		return (int)lResult; 
	}

	CDuiString CRichEditUI::GetLine(int nIndex, int nMaxLength) const
	{
		LPWSTR lpText = NULL;
		lpText = new WCHAR[nMaxLength + 1];
		::ZeroMemory(lpText, (nMaxLength + 1) * sizeof(WCHAR));
		*(LPWORD)lpText = (WORD)nMaxLength;
		TxSendMessage(EM_GETLINE, nIndex, (LPARAM)lpText, 0);
		CDuiString sText;
		sText = (LPCWSTR)lpText;
		delete[] lpText;
		return sText;
	}

	int CRichEditUI::LineIndex(int nLine) const
	{
		LRESULT lResult;
		TxSendMessage(EM_LINEINDEX, nLine, 0, &lResult);
		return (int)lResult;
	}

	int CRichEditUI::LineLength(int nLine) const
	{
		LRESULT lResult;
		TxSendMessage(EM_LINELENGTH, nLine, 0, &lResult);
		return (int)lResult;
	}

	bool CRichEditUI::LineScroll(int nLines, int nChars)
	{
		LRESULT lResult;
		TxSendMessage(EM_LINESCROLL, nChars, nLines, &lResult);
		return (BOOL)lResult == TRUE;
	}

	CDuiPoint CRichEditUI::GetCharPos(long lChar) const
	{ 
		CDuiPoint pt; 
		TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar, 0); 
		return pt;
	}

	long CRichEditUI::LineFromChar(long nIndex) const
	{ 
		if( !m_pTwh ) return 0L;
		LRESULT lResult;
		TxSendMessage(EM_EXLINEFROMCHAR, 0, nIndex, &lResult);
		return (long)lResult;
	}

	CDuiPoint CRichEditUI::PosFromChar(UINT nChar) const
	{ 
		POINTL pt; 
		TxSendMessage(EM_POSFROMCHAR, (WPARAM)&pt, nChar, 0); 
		return CDuiPoint(pt.x, pt.y); 
	}

	int CRichEditUI::CharFromPos(CDuiPoint pt) const
	{ 
		POINTL ptl = {pt.x, pt.y}; 
		if( !m_pTwh ) return 0;
		LRESULT lResult;
		TxSendMessage(EM_CHARFROMPOS, 0, (LPARAM)&ptl, &lResult);
		return (int)lResult; 
	}

	void CRichEditUI::EmptyUndoBuffer()
	{ 
		TxSendMessage(EM_EMPTYUNDOBUFFER, 0, 0, 0); 
	}

	UINT CRichEditUI::SetUndoLimit(UINT nLimit)
	{ 
		if( !m_pTwh ) return 0;
		LRESULT lResult;
		TxSendMessage(EM_SETUNDOLIMIT, (WPARAM) nLimit, 0, &lResult);
		return (UINT)lResult; 
	}

	long CRichEditUI::StreamIn(int nFormat, EDITSTREAM &es)
	{ 
		if( !m_pTwh ) return 0L;
		LRESULT lResult;
		TxSendMessage(EM_STREAMIN, nFormat, (LPARAM)&es, &lResult);
		return (long)lResult;
	}

	long CRichEditUI::StreamOut(int nFormat, EDITSTREAM &es)
	{ 
		if( !m_pTwh ) return 0L;
		LRESULT lResult;
		TxSendMessage(EM_STREAMOUT, nFormat, (LPARAM)&es, &lResult);
		return (long)lResult; 
	}

	void CRichEditUI::SetAccumulateDBCMode( bool bDBCMode )
	{
		m_fAccumulateDBC = bDBCMode;
	}

	bool CRichEditUI::IsAccumulateDBCMode()
	{
		return m_fAccumulateDBC;
	}

	void CRichEditUI::DoInit()
	{
		if(m_bInited)
			return ;

		CREATESTRUCT cs;
		cs.style = m_lTwhStyle;
		cs.x = 0;
		cs.y = 0;
		cs.cy = 0;
		cs.cx = 0;
		cs.lpszName = m_sText.GetData();
		CreateHost(this, &cs, &m_pTwh);
		if( m_pTwh ) {
			if( m_bTransparent ) m_pTwh->SetTransparent(TRUE);
			LRESULT lResult;
			m_pTwh->GetTextServices()->TxSendMessage(EM_SETLANGOPTIONS, 0, 0, &lResult);
			m_pTwh->GetTextServices()->TxSendMessage(EM_SETEVENTMASK, 0, ENM_DROPFILES|ENM_LINK|ENM_CHANGE, &lResult);
			m_pTwh->OnTxInPlaceActivate(NULL);
			m_pManager->AddMessageFilter(this);
			if( m_pManager->IsLayered() ) m_pManager->SetTimer(this, DEFAULT_TIMERID, ::GetCaretBlinkTime());
			if (!m_bEnabled) {
				m_pTwh->SetColor(m_pManager->GetDefaultDisabledColor());
			}
		}

		m_bInited= true;
	}

	HRESULT CRichEditUI::TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const
	{
		if( m_pTwh ) {
			if( msg == WM_KEYDOWN && TCHAR(wparam) == VK_RETURN ) {
				if( !m_bWantReturn || (::GetKeyState(VK_CONTROL) < 0 && !m_bWantCtrlReturn) ) {
					if( m_pManager != NULL ) m_pManager->SendNotify((CControlUI*)this, DUI_MSGTYPE_RETURN);
					return S_OK;
				}
			}
			return m_pTwh->GetTextServices()->TxSendMessage(msg, wparam, lparam, plresult);
		}
		return S_FALSE;
	}

	IDropTarget* CRichEditUI::GetTxDropTarget()
	{
		IDropTarget *pdt = NULL;
		if( m_pTwh->GetTextServices()->TxGetDropTarget(&pdt) == NOERROR ) return pdt;
		return NULL;
	}

	bool CRichEditUI::OnTxViewChanged()
	{
		return true;
	}

	bool CRichEditUI::SetDropAcceptFile(bool bAccept) 
	{
		LRESULT lResult;
		TxSendMessage(EM_SETEVENTMASK, 0,ENM_DROPFILES|ENM_LINK, // ENM_CHANGE| ENM_CORRECTTEXT | ENM_DRAGDROPDONE | ENM_DROPFILES | ENM_IMECHANGE | ENM_LINK | ENM_OBJECTPOSITIONS | ENM_PROTECTED | ENM_REQUESTRESIZE | ENM_SCROLL | ENM_SELCHANGE | ENM_UPDATE,
			&lResult);
		return (BOOL)lResult == FALSE;
	}

	void CRichEditUI::OnTxNotify(DWORD iNotify, void *pv)
	{
		switch(iNotify)
		{ 
		case EN_CHANGE:
			{
				GetManager()->SendNotify(this, DUI_MSGTYPE_TEXTCHANGED);
				if(IsAutoCalcWidth() || IsAutoCalcHeight())
					NeedParentUpdate();
				break;
			}
		case EN_DROPFILES:   
		case EN_MSGFILTER:   
		case EN_OLEOPFAILED:   
		case EN_PROTECTED:   
		case EN_SAVECLIPBOARD:   
		case EN_SELCHANGE:   
		case EN_STOPNOUNDO:   
		case EN_LINK:   
		case EN_OBJECTPOSITIONS:   
		case EN_DRAGDROPDONE:   
			{
				if(pv)                        // Fill out NMHDR portion of pv   
				{   
					LONG nId =  GetWindowLong(this->GetManager()->GetPaintWindow(), GWL_ID);   
					NMHDR  *phdr = (NMHDR *)pv;   
					phdr->hwndFrom = this->GetManager()->GetPaintWindow();   
					phdr->idFrom = nId;   
					phdr->code = iNotify;  

					if(SendMessage(this->GetManager()->GetPaintWindow(), WM_NOTIFY, (WPARAM) nId, (LPARAM) pv))   
					{   
						//hr = S_FALSE;   
					}   
				}    
			}
			break;
		}
	}

	// 多行非rich格式的richedit有一个滚动条bug，在最后一行是空行时，LineDown和SetScrollPos无法滚动到最后
	// 引入iPos就是为了修正这个bug
	void CRichEditUI::SetScrollPos(SIZE szPos, bool bMsg)
	{
		int cx = 0;
		int cy = 0;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
			int iLastScrollPos = m_pVerticalScrollBar->GetScrollPos();
			m_pVerticalScrollBar->SetScrollPos(szPos.cy);
			cy = m_pVerticalScrollBar->GetScrollPos() - iLastScrollPos;
		}
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
			int iLastScrollPos = m_pHorizontalScrollBar->GetScrollPos();
			m_pHorizontalScrollBar->SetScrollPos(szPos.cx);
			cx = m_pHorizontalScrollBar->GetScrollPos() - iLastScrollPos;
		}
		if( cy != 0 ) {
			int iPos = 0;
			if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
				iPos = m_pVerticalScrollBar->GetScrollPos();
			WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pVerticalScrollBar->GetScrollPos());
			TxSendMessage(WM_VSCROLL, wParam, 0L, 0);
			if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
				if( cy > 0 && m_pVerticalScrollBar->GetScrollPos() <= iPos )
					m_pVerticalScrollBar->SetScrollPos(iPos);
			}
		}
		if( cx != 0 ) {
			WPARAM wParam = MAKEWPARAM(SB_THUMBPOSITION, m_pHorizontalScrollBar->GetScrollPos());
			TxSendMessage(WM_HSCROLL, wParam, 0L, 0);
		}
	}

	void CRichEditUI::LineUp()
	{
		TxSendMessage(WM_VSCROLL, SB_LINEUP, 0L, 0);
	}

	void CRichEditUI::LineDown()
	{
		int iPos = 0;
		if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) 
			iPos = m_pVerticalScrollBar->GetScrollPos();
		TxSendMessage(WM_VSCROLL, SB_LINEDOWN, 0L, 0);
		if( m_pTwh && !m_bRich && m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
			if( m_pVerticalScrollBar->GetScrollPos() <= iPos )
				m_pVerticalScrollBar->SetScrollPos(m_pVerticalScrollBar->GetScrollRange());
		}
	}

	void CRichEditUI::PageUp()
	{
		TxSendMessage(WM_VSCROLL, SB_PAGEUP, 0L, 0);
	}

	void CRichEditUI::PageDown()
	{
		TxSendMessage(WM_VSCROLL, SB_PAGEDOWN, 0L, 0);
	}

	void CRichEditUI::HomeUp()
	{
		TxSendMessage(WM_VSCROLL, SB_TOP, 0L, 0);
	}

	void CRichEditUI::EndDown()
	{
		TxSendMessage(WM_VSCROLL, SB_BOTTOM, 0L, 0);
	}

	void CRichEditUI::LineLeft()
	{
		TxSendMessage(WM_HSCROLL, SB_LINELEFT, 0L, 0);
	}

	void CRichEditUI::LineRight()
	{
		TxSendMessage(WM_HSCROLL, SB_LINERIGHT, 0L, 0);
	}

	void CRichEditUI::PageLeft()
	{
		TxSendMessage(WM_HSCROLL, SB_PAGELEFT, 0L, 0);
	}

	void CRichEditUI::PageRight()
	{
		TxSendMessage(WM_HSCROLL, SB_PAGERIGHT, 0L, 0);
	}

	void CRichEditUI::HomeLeft()
	{
		TxSendMessage(WM_HSCROLL, SB_LEFT, 0L, 0);
	}

	void CRichEditUI::EndRight()
	{
		TxSendMessage(WM_HSCROLL, SB_RIGHT, 0L, 0);
	}

	void CRichEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CControlUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			if( m_pTwh && m_pTwh->DoSetCursor(NULL, &event.ptMouse) ) {
				return;
			}
		}
		else if( event.Type == UIEVENT_WINDOWSIZE ) {
			if( m_pTwh ) m_pTwh->NeedFreshCaret();
		}
		else if( event.Type == UIEVENT_SETFOCUS ) {
			if( m_pTwh ) {
				m_pTwh->OnTxInPlaceActivate(NULL);
				m_pTwh->GetTextServices()->TxSendMessage(WM_SETFOCUS, 0, 0, 0);
			}
			m_bFocused = true;
			Invalidate();
			return;
		}
		if( event.Type == UIEVENT_KILLFOCUS )  {
			if( m_pTwh ) {
				m_pTwh->OnTxInPlaceActivate(NULL);
				m_pTwh->GetTextServices()->TxSendMessage(WM_KILLFOCUS, 0, 0, 0);
			}
			m_bFocused = false;
			Invalidate();
			return;
		}
		else if( event.Type == UIEVENT_TIMER ) {
			if( event.wParam == DEFAULT_TIMERID ) {
				if( m_pTwh && m_pManager->IsLayered() && IsFocused() ) {
					if (::GetFocus() != m_pManager->GetPaintWindow()) return;
					m_bDrawCaret = !m_bDrawCaret;
					POINT ptCaret;
					::GetCaretPos(&ptCaret);
					RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x + m_pTwh->GetCaretWidth(), 
						ptCaret.y + m_pTwh->GetCaretHeight() };
					RECT rcTemp = rcCaret;
					if( !::IntersectRect(&rcCaret, &rcTemp, &m_rcItem) ) return;
					CControlUI* pParent = this;
					RECT rcParent;
					while( pParent = pParent->GetParent() ) {
						rcTemp = rcCaret;
						rcParent = pParent->GetPos();
						if( !::IntersectRect(&rcCaret, &rcTemp, &rcParent) ) {
							return;
						}
					}                    
					m_pManager->Invalidate(rcCaret);
				}
				return;
			}
			if( m_pTwh ) {
				m_pTwh->GetTextServices()->TxSendMessage(WM_TIMER, event.wParam, event.lParam, 0);
			} 
			return;
		}
		if( event.Type == UIEVENT_SCROLLWHEEL ) {
			if( (event.wKeyState & MK_CONTROL) != 0  ) {
				return;
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK ) 
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type > UIEVENT__KEYBEGIN && event.Type < UIEVENT__KEYEND )
		{
			return;
		}
		CContainerUI::DoEvent(event);
	}

	int CRichEditUI::GetFixedWidth() const
	{
		if(!m_bReturnFixedWidth) return 0; //告诉父容器，预设宽度需要重新计算。
		return CControlUI::GetFixedWidth();
	}

	int CRichEditUI::GetFixedHeight() const
	{
		if(!m_bReturnFixedHeight) return 0; //告诉父容器，预设高度需要重新计算。
		return CControlUI::GetFixedHeight();
	}

	SIZE CRichEditUI::EstimateSize(SIZE szAvailable)
	{
		if (m_pTwh && (IsAutoCalcWidth() || IsAutoCalcHeight()))
		{
			SIZE szNeed = {0,0};
			RECT rc = {0, 0, szAvailable.cx, szAvailable.cy};
			RECT rcInset = GetInset();
			rc.left += rcInset.left;
			rc.top += rcInset.top;
			rc.right -= rcInset.right;
			rc.bottom -= rcInset.bottom;

			RECT rcText = rc;
			rcText.left += m_rcTextPadding.left;
			rcText.right -= m_rcTextPadding.right;
			rcText.top += m_rcTextPadding.top;
			rcText.bottom -= m_rcTextPadding.bottom;

			RECT rcScrollView = rcText;

			bool bVScrollBarVisiable = false;
			if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
				bVScrollBarVisiable = true;
				rc.top -= m_pVerticalScrollBar->GetScrollPos();
				rc.bottom -= m_pVerticalScrollBar->GetScrollPos();
				rc.bottom += m_pVerticalScrollBar->GetScrollRange();
				rc.right -= m_pVerticalScrollBar->GetFixedWidth();
				rcScrollView.right -= m_pVerticalScrollBar->GetFixedWidth();
			}
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
				rc.left -= m_pHorizontalScrollBar->GetScrollPos();
				rc.right -= m_pHorizontalScrollBar->GetScrollPos();
				rc.right += m_pHorizontalScrollBar->GetScrollRange();
				rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
				rcScrollView.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
			}

			RECT rcScrollTextView = rcScrollView;
			rcScrollTextView.left += m_rcTextPadding.left;
			rcScrollTextView.right -= m_rcTextPadding.right;
			rcScrollTextView.top += m_rcTextPadding.top;
			rcScrollTextView.bottom -= m_rcTextPadding.bottom;

			m_pTwh->SetClientRect(&rcScrollTextView);

			LONG lWidth = rcText.right - rcText.left;
			
			if(m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) 
				lWidth += m_pVerticalScrollBar->GetFixedWidth();
			LONG lHeight = 0;
			SIZEL szExtent = { -1, -1 };
			int hret = m_pTwh->GetTextServices()->TxGetNaturalSize(
				DVASPECT_CONTENT, 
				GetManager()->GetPaintDC(), 
				NULL,
				NULL,
				TXTNS_FITTOCONTENT,
				&szExtent,
				&lWidth,
				&lHeight);

			if(IsAutoCalcWidth())
			{
				szNeed.cx = lWidth + m_rcTextPadding.left + m_rcTextPadding.right + rcInset.left + rcInset.right;
				if(szNeed.cx < GetMinWidth()) szNeed.cx = GetMinWidth();
				if(szNeed.cx > GetMaxWidth()) szNeed.cx = GetMaxWidth();
			}

			if(IsAutoCalcHeight())
			{
				szNeed.cy = lHeight + m_rcTextPadding.top + m_rcTextPadding.bottom + rcInset.top + rcInset.bottom;
				if(szNeed.cy < GetMinHeight()) szNeed.cy = GetMinHeight();
				if(szNeed.cy > GetMaxHeight()) szNeed.cy = GetMaxHeight();
			}
			
			if(IsAutoCalcWidth() && szNeed.cx < szAvailable.cx) //如果宽度还有多，下次要重新计算宽度
			{
				m_bReturnFixedWidth = false;
				m_bReturnFixedHeight = true;
			}
			else if(IsAutoCalcHeight() && szNeed.cy < szAvailable.cy) //宽度不够的前提下，高度有剩下的，下次重新计算高度
			{
				m_bReturnFixedWidth = true;
				m_bReturnFixedHeight = false;
			}

			szNeed.cy += 5;
			return CDuiSize(GetManager()->GetDPIObj()->Scale(szNeed.cx), GetManager()->GetDPIObj()->Scale(szNeed.cy));
		}
		//return CDuiSize(m_rcItem); // 这种方式在第一次设置大小之后就大小不变了
		return CContainerUI::EstimateSize(szAvailable);
	}

	void CRichEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		rc = m_rcItem;

		//////////////////////////////////////////////////////////////////////////
		//add by liqs99, 实现单行时RichEdit垂直居中
		if(!IsMultiLine())
		{
			if(m_uTextStyle & DT_TOP)
			{

			}
			else if(m_uTextStyle & DT_CENTER)
			{
				SIZE sz = CRenderEngine::GetTextSize(GetManager()->GetPaintDC(), GetManager(), _T("A"), GetFont(), 0);
				rc.top += (rc.bottom-rc.top-sz.cy) / 2;
				rc.bottom -= (rc.bottom-rc.top-sz.cy) / 2;
			}
			else if(m_uTextStyle & DT_BOTTOM)
			{
				SIZE sz = CRenderEngine::GetTextSize(GetManager()->GetPaintDC(), GetManager(), _T("A"), GetFont(), 0);
				rc.top += rc.bottom-rc.top- sz.cy;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		RECT rcInset = GetInset();
		rc.left += rcInset.left;
		rc.top += rcInset.top;
		rc.right -= rcInset.right;
		rc.bottom -= rcInset.bottom;

		RECT rcScrollView = rc;

		bool bVScrollBarVisiable = false;
		if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) {
			bVScrollBarVisiable = true;
			rc.top -= m_pVerticalScrollBar->GetScrollPos();
			rc.bottom -= m_pVerticalScrollBar->GetScrollPos();
			rc.bottom += m_pVerticalScrollBar->GetScrollRange();
			rc.right -= m_pVerticalScrollBar->GetFixedWidth();
			rcScrollView.right -= m_pVerticalScrollBar->GetFixedWidth();
		}
		if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) {
			rc.left -= m_pHorizontalScrollBar->GetScrollPos();
			rc.right -= m_pHorizontalScrollBar->GetScrollPos();
			rc.right += m_pHorizontalScrollBar->GetScrollRange();
			rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
			rcScrollView.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
		}

		if( m_pTwh != NULL ) {
			RECT rcScrollTextView = rcScrollView;
			rcScrollTextView.left += m_rcTextPadding.left;
			rcScrollTextView.right -= m_rcTextPadding.right;
			rcScrollTextView.top += m_rcTextPadding.top;
			rcScrollTextView.bottom -= m_rcTextPadding.bottom;
			RECT rcText = rc;
			rcText.left += m_rcTextPadding.left;
			rcText.right -= m_rcTextPadding.right;
			rcText.top += m_rcTextPadding.top;
			rcText.bottom -= m_rcTextPadding.bottom;
			m_pTwh->SetClientRect(&rcScrollTextView);
			if( bVScrollBarVisiable && (!m_pVerticalScrollBar->IsVisible() || m_bVScrollBarFixing) ) {
				LONG lWidth = rcText.right - rcText.left + m_pVerticalScrollBar->GetFixedWidth();
				LONG lHeight = 0;
				SIZEL szExtent = { -1, -1 };
				m_pTwh->GetTextServices()->TxGetNaturalSize(
					DVASPECT_CONTENT, 
					GetManager()->GetPaintDC(), 
					NULL,
					NULL,
					TXTNS_FITTOCONTENT,
					&szExtent,
					&lWidth,
					&lHeight);
				if( lHeight > rcText.bottom - rcText.top ) {
					m_pVerticalScrollBar->SetVisible(true);
					m_pVerticalScrollBar->SetScrollPos(0);
					m_bVScrollBarFixing = true;
				}
				else {
					if( m_bVScrollBarFixing ) {
						m_pVerticalScrollBar->SetVisible(false);
						m_bVScrollBarFixing = false;
					}
				}
			}
		}

		if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() ) {
			RECT rcScrollBarPos = { rcScrollView.right, rcScrollView.top, 
				rcScrollView.right + m_pVerticalScrollBar->GetFixedWidth(), rcScrollView.bottom};
			m_pVerticalScrollBar->SetPos(rcScrollBarPos, false);
		}
		if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() ) {
			RECT rcScrollBarPos = { rcScrollView.left, rcScrollView.bottom, rcScrollView.right, 
				rcScrollView.bottom + m_pHorizontalScrollBar->GetFixedHeight()};
			m_pHorizontalScrollBar->SetPos(rcScrollBarPos, false);
		}

		for( int it = 0; it < m_items.GetSize(); it++ ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
			if( !pControl->IsVisible() ) continue;
			if( pControl->IsFloat() ) {
				SetFloatPos(it);
			}
			else {
				SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
				if( sz.cx < pControl->GetMinWidth() ) sz.cx = pControl->GetMinWidth();
				if( sz.cx > pControl->GetMaxWidth() ) sz.cx = pControl->GetMaxWidth();
				if( sz.cy < pControl->GetMinHeight() ) sz.cy = pControl->GetMinHeight();
				if( sz.cy > pControl->GetMaxHeight() ) sz.cy = pControl->GetMaxHeight();
				RECT rcCtrl = { rc.left, rc.top, rc.left + sz.cx, rc.top + sz.cy };
				pControl->SetPos(rcCtrl, false);
			}
		}
	}

	void CRichEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CContainerUI::Move(szOffset, bNeedInvalidate);
		if( m_pTwh != NULL ) {
			RECT rc = m_rcItem;
			RECT rcInset = GetInset();
			rc.left += rcInset.left;
			rc.top += rcInset.top;
			rc.right -= rcInset.right;
			rc.bottom -= rcInset.bottom;

			if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) rc.right -= m_pVerticalScrollBar->GetFixedWidth();
			if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
			m_pTwh->SetClientRect(&rc);
		}
	}

	bool CRichEditUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		RECT rcTemp = { 0 };
		if( !::IntersectRect(&rcTemp, &rcPaint, &m_rcItem) ) return true;

		CRenderClip clip;
		CRenderClip::GenerateClip(hDC, rcTemp, clip);
		CControlUI::DoPaint(hDC, rcPaint, pStopControl);

		if( m_pTwh ) {
			RECT rc;
			m_pTwh->GetControlRect(&rc);
			// Remember wparam is actually the hdc and lparam is the update
			// rect because this message has been preprocessed by the window.
			m_pTwh->GetTextServices()->TxDraw(
				DVASPECT_CONTENT,  		// Draw Aspect
				/*-1*/0,				// Lindex
				NULL,					// Info for drawing optimazation
				NULL,					// target device information
				hDC,			        // Draw device HDC
				NULL, 				   	// Target device HDC
				(RECTL*)&rc,			// Bounding client rectangle
				NULL, 		            // Clipping rectangle for metafiles
				(RECT*)&rcPaint,		// Update rectangle
				NULL, 	   				// Call back function
				NULL,					// Call back parameter
				0);				        // What view of the object
			if( m_bVScrollBarFixing ) {
				LONG lWidth = rc.right - rc.left + m_pVerticalScrollBar->GetFixedWidth();
				LONG lHeight = 0;
				SIZEL szExtent = { -1, -1 };
				m_pTwh->GetTextServices()->TxGetNaturalSize(
					DVASPECT_CONTENT, 
					GetManager()->GetPaintDC(), 
					NULL,
					NULL,
					TXTNS_FITTOCONTENT,
					&szExtent,
					&lWidth,
					&lHeight);
				if( lHeight <= rc.bottom - rc.top ) {
					NeedUpdate();
				}
			}
		}

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

		if( m_pTwh && m_pTwh->IsShowCaret() && m_pManager->IsLayered() && IsFocused() && m_bDrawCaret ) {
			POINT ptCaret;
			::GetCaretPos(&ptCaret);
			if( ::PtInRect(&m_rcItem, ptCaret) ) {
				RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y + m_pTwh->GetCaretHeight() };
				CRenderEngine::DrawLine(hDC, rcCaret, m_pTwh->GetCaretWidth(), 0xFF000000);
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
		// 绘制提示文字
		CDuiString sDrawText = GetText();
		if(sDrawText.IsEmpty() && !m_bFocused) {
			DWORD dwTextColor = GetTipValueColor();
			CDuiString sTipValue = GetTipValue();
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding.left;
			rc.right -= m_rcTextPadding.right;
			rc.top += m_rcTextPadding.top;
			rc.bottom -= m_rcTextPadding.bottom;
			UINT uTextAlign = GetTipValueAlign();
			if(IsMultiLine()) uTextAlign |= DT_TOP;
			else uTextAlign |= DT_VCENTER;
			CRenderEngine::DrawText(hDC, m_pManager, rc, sTipValue, dwTextColor, m_iFont, uTextAlign);
		}
		return true;
	}

	LPCTSTR CRichEditUI::GetNormalImage()
	{
		return m_sNormalImage;
	}

	void CRichEditUI::SetNormalImage(LPCTSTR pStrImage)
	{
		m_sNormalImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CRichEditUI::GetHotImage()
	{
		return m_sHotImage;
	}

	void CRichEditUI::SetHotImage(LPCTSTR pStrImage)
	{
		m_sHotImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CRichEditUI::GetFocusedImage()
	{
		return m_sFocusedImage;
	}

	void CRichEditUI::SetFocusedImage(LPCTSTR pStrImage)
	{
		m_sFocusedImage = pStrImage;
		Invalidate();
	}

	LPCTSTR CRichEditUI::GetDisabledImage()
	{
		return m_sDisabledImage;
	}

	void CRichEditUI::SetDisabledImage(LPCTSTR pStrImage)
	{
		m_sDisabledImage = pStrImage;
		Invalidate();
	}

	RECT CRichEditUI::GetTextPadding() const
	{
		return m_rcTextPadding;
	}

	void CRichEditUI::SetTextPadding(RECT rc)
	{
		m_rcTextPadding = rc;
		Invalidate();
	}

	void CRichEditUI::SetTipValue( LPCTSTR pStrTipValue )
	{
		m_sTipValue	= pStrTipValue;
	}

	LPCTSTR CRichEditUI::GetTipValue()
	{
		if (IsResourceText()) 
			return CResourceManager::GetInstance()->GetText(m_sTipValue);

		CLangPackageUI *pkg = GetLangPackage();
		if(pkg && GetResourceID() > 0)
		{
			LPCTSTR s = pkg->GetTipValue(GetResourceID());
			if(s && *s!='\0') return s; 
		}

		return m_sTipValue.GetData();
	}

	void CRichEditUI::SetTipValueColor( LPCTSTR pStrColor )
	{
		if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwTipValueColor = clrColor;
	}

	DWORD CRichEditUI::GetTipValueColor()
	{
		return m_dwTipValueColor;
	}

	void CRichEditUI::SetTipValueAlign(UINT uAlign)
	{
		m_uTipValueAlign = uAlign;
		if(GetText().IsEmpty()) Invalidate();
	}

	UINT CRichEditUI::GetTipValueAlign()
	{
		return m_uTipValueAlign;
	}

	void CRichEditUI::PaintBkColor(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if(m_dwFocusBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwFocusBkColor));
				return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT ) != 0 ) {
			if(m_dwHotBkColor != 0) {
				CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
				return;
			}
		}
		return __super::PaintBkColor(hDC);
	}

	void CRichEditUI::PaintStatusImage(HDC hDC)
	{
		if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
		else m_uButtonState &= ~ UISTATE_FOCUSED;
		if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
		else m_uButtonState &= ~ UISTATE_DISABLED;

		if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
			if( !m_sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sDisabledImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_FOCUSED) != 0 ) {
			if( !m_sFocusedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sFocusedImage) ) {}
				else return;
			}
		}
		else if( (m_uButtonState & UISTATE_HOT ) != 0 ) {
			if( !m_sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sHotImage) ) {}
				else return;
			}
		}

		if( !m_sNormalImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sNormalImage) ) {}
			else return;
		}
	}

	void CRichEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("vscrollbar")) == 0 ) {
			if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_VSCROLL;
		}
		if( _tcscmp(pstrName, _T("autovscroll")) == 0 ) {
			if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOVSCROLL;
		}
		else if( _tcscmp(pstrName, _T("hscrollbar")) == 0 ) {
			if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_DISABLENOSCROLL | WS_HSCROLL;
		}
		if( _tcscmp(pstrName, _T("autohscroll")) == 0 ) {
			if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_AUTOHSCROLL;
		}
		else if( _tcsicmp(pstrName, _T("multiline")) == 0 ) {
			SetMultiLine(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("wanttab")) == 0 ) {
			SetWantTab(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("wantreturn")) == 0 ) {
			SetWantReturn(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("wantctrlreturn")) == 0 ) {
			SetWantCtrlReturn(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("transparent")) == 0 ) {
			SetTransparent(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("rich")) == 0 ) {
			SetRich(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("paragraphindent")) == 0 ) {
			SetParagraphIndent(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("multiline")) == 0 ) {
			if( _tcscmp(pstrValue, _T("false")) == 0 ) m_lTwhStyle &= ~ES_MULTILINE;
		}
		else if( _tcscmp(pstrName, _T("readonly")) == 0 ) 
		{
			//if( _tcscmp(pstrValue, _T("true")) == 0 ) { m_lTwhStyle |= ES_READONLY; m_bReadOnly = true; }
			SetReadOnly(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("password")) == 0 ) {
			if( _tcscmp(pstrValue, _T("true")) == 0 ) m_lTwhStyle |= ES_PASSWORD;
		}
		else if( _tcscmp(pstrName, _T("align")) == 0 ) {
			if( _tcsstr(pstrValue, _T("left")) != NULL ) {
				m_lTwhStyle &= ~(ES_CENTER | ES_RIGHT);
				m_lTwhStyle |= ES_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_lTwhStyle &= ~(ES_LEFT | ES_RIGHT);
				m_lTwhStyle |= ES_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) {
				m_lTwhStyle &= ~(ES_LEFT | ES_CENTER);
				m_lTwhStyle |= ES_RIGHT;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//add by liqs99
		else if( _tcscmp(pstrName, _T("valign")) == 0 ) {
			if( _tcsstr(pstrValue, _T("top")) != NULL ) {
				m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
				m_uTextStyle |= DT_TOP;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_BOTTOM);
				m_uTextStyle |= DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("bottom")) != NULL ) {
				m_uTextStyle &= ~(DT_TOP | DT_CENTER);
				m_uTextStyle |= DT_BOTTOM;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		else if( _tcscmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
		else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) {
			while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetTextColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("maxchar")) == 0 ) SetLimitText(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("normalimage")) == 0 ) SetNormalImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("hotimage")) == 0 ) SetHotImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("focusedimage")) == 0 ) SetFocusedImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("disabledimage")) == 0 ) SetDisabledImage(pstrValue);
		else if( _tcsicmp(pstrName, _T("textpadding")) == 0 ) {
			RECT rcTextPadding = { 0 };
			LPTSTR pstr = NULL;
			rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			SetTextPadding(rcTextPadding);
		}
		else if( _tcsicmp(pstrName, _T("tipvalue")) == 0 ) SetTipValue(pstrValue);
		else if( _tcsicmp(pstrName, _T("tipvaluecolor")) == 0 ) SetTipValueColor(pstrValue);
		else if( _tcsicmp(pstrName, _T("tipvaluealign")) == 0 ) {
			if( _tcsstr(pstrValue, _T("left")) != NULL ) {
				m_uTipValueAlign = DT_SINGLELINE | DT_LEFT;
			}
			if( _tcsstr(pstrValue, _T("center")) != NULL ) {
				m_uTipValueAlign = DT_SINGLELINE | DT_CENTER;
			}
			if( _tcsstr(pstrValue, _T("right")) != NULL ) {
				m_uTipValueAlign = DT_SINGLELINE | DT_RIGHT;
			}
		}
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	LRESULT CRichEditUI::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
	{
		if( !IsVisible() || !IsEnabled() ) return 0;
		if( !IsMouseEnabled() && uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) return 0;
		if( uMsg == WM_MOUSEWHEEL && (LOWORD(wParam) & MK_CONTROL) == 0 ) return 0;

		if (uMsg == WM_IME_COMPOSITION) {
			// 解决微软输入法位置异常的问题
			HIMC hIMC = ImmGetContext(GetManager()->GetPaintWindow());
			if (hIMC)  {
				POINT point;
				GetCaretPos(&point);

				COMPOSITIONFORM Composition;
				Composition.dwStyle = CFS_POINT;
				Composition.ptCurrentPos.x = point.x;
				Composition.ptCurrentPos.y = point.y;
				ImmSetCompositionWindow(hIMC, &Composition);

				ImmReleaseContext(GetManager()->GetPaintWindow(),hIMC);
			}

			return 0;
		}
		bool bWasHandled = true;
		if( (uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST) || uMsg == WM_SETCURSOR ) {
			if( !m_pTwh->IsCaptured() ) {
				switch (uMsg) {
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_LBUTTONDBLCLK:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
					{
						POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
						CControlUI* pHover = GetManager()->FindControl(pt);
						if(pHover != this) {
							bWasHandled = false;
							return 0;
						}
					}
					break;
				}
			}
			// Mouse message only go when captured or inside rect
			DWORD dwHitResult = m_pTwh->IsCaptured() ? HITRESULT_HIT : HITRESULT_OUTSIDE;
			if( dwHitResult == HITRESULT_OUTSIDE ) {
				RECT rc;
				m_pTwh->GetControlRect(&rc);
				POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
				if( uMsg == WM_SETCURSOR ) {
					::GetCursorPos(&pt);
					::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
				}
				else if( uMsg == WM_MOUSEWHEEL ) ::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
				if( ::PtInRect(&rc, pt) && !GetManager()->IsCaptured() ) dwHitResult = HITRESULT_HIT;
			}
			if( dwHitResult != HITRESULT_HIT ) return 0;
			if( uMsg == WM_SETCURSOR ) bWasHandled = false;
			else if( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDOWN ) {
				if (!GetManager()->IsNoActivate()) ::SetFocus(GetManager()->GetPaintWindow());
				SetFocus();
			}

			if(uMsg == WM_LBUTTONUP)	//add by liqs99
			{
				if( m_pManager != NULL )
				{
					m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
				}
			}
		}
#ifdef _UNICODE
		else if( uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST ) {
#else
		else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR ) {
#endif
			if( !IsFocused() ) return 0;
		}
#ifdef _USEIMM
		else if( uMsg == WM_IME_STARTCOMPOSITION ) {
			if( IsFocused() ) {
				POINT ptCaret;
				::GetCaretPos(&ptCaret);
				HIMC hMic = ::ImmGetContext(GetManager()->GetPaintWindow());
				COMPOSITIONFORM cpf;
				cpf.dwStyle = CFS_FORCE_POSITION;
				cpf.ptCurrentPos.x = ptCaret.x + m_pTwh->GetCaretWidth();
				cpf.ptCurrentPos.y = ptCaret.y;
				::ImmSetCompositionWindow(hMic, &cpf);

				HFONT hFont = GetManager()->GetFont(m_iFont);
				LOGFONT lf;
				::GetObject(hFont, sizeof(LOGFONT), &lf);
				::ImmSetCompositionFont(hMic, &lf);

				::ImmReleaseContext(GetManager()->GetPaintWindow(), hMic);
			}
			bWasHandled = false;
			return 0;
		}
#endif
		else if( uMsg == WM_CONTEXTMENU ) {
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CControlUI* pHover = GetManager()->FindControl(pt);
			if(pHover != this) {
				bWasHandled = false;
				return 0;
			}
			//创建一个弹出式菜单
			HMENU hPopMenu = CreatePopupMenu();
			AppendMenu(hPopMenu, 0, ID_RICH_UNDO, _T("撤销(&U)"));
			AppendMenu(hPopMenu, 0, ID_RICH_REDO, _T("重做(&R)"));
			AppendMenu(hPopMenu, MF_SEPARATOR, 0, _T(""));
			AppendMenu(hPopMenu, 0, ID_RICH_CUT, _T("剪切(&X)"));
			AppendMenu(hPopMenu, 0, ID_RICH_COPY, _T("复制(&C)"));
			AppendMenu(hPopMenu, 0, ID_RICH_PASTE, _T("粘帖(&V)"));
			AppendMenu(hPopMenu, 0, ID_RICH_CLEAR, _T("清空(&L)"));
			AppendMenu(hPopMenu, MF_SEPARATOR, 0, _T(""));
			AppendMenu(hPopMenu, 0, ID_RICH_SELECTALL, _T("全选(&A)"));

			//初始化菜单项
			UINT uUndo = (CanUndo() ? 0 : MF_GRAYED);
			EnableMenuItem(hPopMenu, ID_RICH_UNDO, MF_BYCOMMAND | uUndo);
			UINT uRedo = (CanRedo() ? 0 : MF_GRAYED);
			EnableMenuItem(hPopMenu, ID_RICH_REDO, MF_BYCOMMAND | uRedo);
			UINT uSel = ((GetSelectionType() != SEL_EMPTY) ? 0 : MF_GRAYED);
			UINT uReadonly = IsReadOnly() ? MF_GRAYED : 0;
			EnableMenuItem(hPopMenu, ID_RICH_CUT, MF_BYCOMMAND | uSel | uReadonly);
			EnableMenuItem(hPopMenu, ID_RICH_COPY, MF_BYCOMMAND | uSel);
			EnableMenuItem(hPopMenu, ID_RICH_CLEAR, MF_BYCOMMAND | uReadonly);
			EnableMenuItem(hPopMenu, ID_RICH_PASTE, MF_BYCOMMAND | uReadonly);
			::ClientToScreen(GetManager()->GetPaintWindow(), &pt);
			TrackPopupMenu(hPopMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, GetManager()->GetPaintWindow(), NULL);
			DestroyMenu(hPopMenu);
		}
		else if( uMsg == WM_COMMAND ) {
			bHandled = FALSE;
			if( !IsFocused() ) return 0;
			UINT uCmd = (UINT)wParam;
			switch(uCmd) {
			case ID_RICH_UNDO:
				{
					Undo();
					break;
				}
			case ID_RICH_REDO:
				{
					Redo();
					break;
				}
			case ID_RICH_CUT:
				{
					Cut();
					break;
				}
			case ID_RICH_COPY:
				{
					Copy();
					break;
				}
			case ID_RICH_PASTE:
				{
					Paste();
					break;
				}
			case ID_RICH_CLEAR:
				{
					SetSel(0,-1);
					Clear();
					break;
				}
			case ID_RICH_SELECTALL:
				{
					SetSelAll();
					break;
				}
			default:break;
			}
		}
		else
		{
			switch( uMsg ) {
			case WM_HELP:
				bWasHandled = false;
				break;
			default:
				return 0;
			}
		}
		if(WM_CHAR == uMsg)
		{
#ifndef _UNICODE
			// check if we are waiting for 2 consecutive WM_CHAR messages
			if ( IsAccumulateDBCMode() )
			{
				if ( (GetKeyState(VK_KANA) & 0x1) )
				{
					// turn off accumulate mode
					SetAccumulateDBCMode ( false );
					m_chLeadByte = 0;
				}
				else
				{
					if ( !m_chLeadByte )
					{
						// This is the first WM_CHAR message, 
						// accumulate it if this is a LeadByte.  Otherwise, fall thru to
						// regular WM_CHAR processing.
						if ( IsDBCSLeadByte ( (WORD)wParam ) )
						{
							// save the Lead Byte and don't process this message
							m_chLeadByte = (WORD)wParam << 8 ;

							//TCHAR a = (WORD)wParam << 8 ;
							return 0;
						}
					}
					else
					{
						// This is the second WM_CHAR message,
						// combine the current byte with previous byte.
						// This DBC will be handled as WM_IME_CHAR.
						wParam |= m_chLeadByte;
						uMsg = WM_IME_CHAR;

						// setup to accumulate more WM_CHAR
						m_chLeadByte = 0; 
					}
				}
			}
#endif
		}
		LRESULT lResult = 0;
		HRESULT Hr = TxSendMessage(uMsg, wParam, lParam, &lResult);
		if( Hr == S_OK ) bHandled = bWasHandled;
		else if( (uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST) || uMsg == WM_CHAR || uMsg == WM_IME_CHAR )
			bHandled = bWasHandled;
		else if( uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST ) {
			if( m_pTwh->IsCaptured() ) bHandled = bWasHandled;
		}
		return lResult;
	}

	bool CRichEditUI::OnEnableResponseDefaultKeyEvent(WPARAM wParam)
	{
		if(wParam == VK_RETURN && IsFocused() && (IsWantReturn() || IsWantCtrlReturn()) )
			return false;
		return true;
	}

} // namespace DuiLib

	
