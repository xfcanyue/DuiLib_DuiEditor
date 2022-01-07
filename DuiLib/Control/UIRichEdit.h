#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once

namespace DuiLib {

	class CTxtWinHost;

	class UILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI
	{
		DECLARE_DUICONTROL(CRichEditUI)
	public:
		CRichEditUI();
		~CRichEditUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine);
		bool IsWantTab();
		void SetWantTab(bool bWantTab = true);
		bool IsWantReturn();
		void SetWantReturn(bool bWantReturn = true);
		bool IsWantCtrlReturn();
		void SetWantCtrlReturn(bool bWantCtrlReturn = true);
		bool IsTransparent();
		void SetTransparent(bool bTransparent = true);
		bool IsParagraphIndent();
		void SetParagraphIndent(bool bIndent = true);
		bool IsRich();
		void SetRich(bool bRich = true);

		virtual void SetReadOnly(bool bReadOnly = true) override;
		bool IsWordWrap();
		void SetWordWrap(bool bWordWrap = true);

		virtual void SetFont(int index) override;
		void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);

		LONG GetWinStyle();
		void SetWinStyle(LONG lStyle);

		virtual void SetTextColor(DWORD dwTextColor) override;

		int GetLimitText();
		void SetLimitText(int iChars);
		long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
		CDuiString GetText() const;
		void SetText(LPCTSTR pstrText);
		bool IsModify() const;
		void SetModify(bool bModified = true) const;
		void GetSel(CHARRANGE &cr) const;
		void GetSel(long& nStartChar, long& nEndChar) const;
		int SetSel(CHARRANGE &cr);
		int SetSel(long nStartChar, long nEndChar);
		void ReplaceSel(LPCTSTR lpszNewText, bool bCanUndo);
		void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
		CDuiString GetSelText() const;
		int SetSelAll();
		int SetSelNone();
		WORD GetSelectionType() const;
		bool GetZoom(int& nNum, int& nDen) const;
		bool SetZoom(int nNum, int nDen);
		bool SetZoomOff();
		bool GetAutoURLDetect() const;
		bool SetAutoURLDetect(bool bAutoDetect = true);
		DWORD GetEventMask() const;
		DWORD SetEventMask(DWORD dwEventMask);
		CDuiString GetTextRange(long nStartChar, long nEndChar) const;
		void HideSelection(bool bHide = true, bool bChangeStyle = false);
		void ScrollCaret();
		int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
		int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
		DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
		bool SetDefaultCharFormat(CHARFORMAT2 &cf);
		DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
		bool SetSelectionCharFormat(CHARFORMAT2 &cf);
		bool SetWordCharFormat(CHARFORMAT2 &cf);
		DWORD GetParaFormat(PARAFORMAT2 &pf) const;
		bool SetParaFormat(PARAFORMAT2 &pf);
		bool CanUndo();
		bool CanRedo();
		bool CanPaste();
		bool Redo();
		bool Undo();
		void Clear();
		void Copy();
		void Cut();
		void Paste();
		int GetLineCount() const;
		CDuiString GetLine(int nIndex, int nMaxLength) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		bool LineScroll(int nLines, int nChars = 0);
		CDuiPoint GetCharPos(long lChar) const;
		long LineFromChar(long nIndex) const;
		CDuiPoint PosFromChar(UINT nChar) const;
		int CharFromPos(CDuiPoint pt) const;
		void EmptyUndoBuffer();
		UINT SetUndoLimit(UINT nLimit);
		long StreamIn(int nFormat, EDITSTREAM &es);
		long StreamOut(int nFormat, EDITSTREAM &es);
		void SetAccumulateDBCMode(bool bDBCMode);
		bool IsAccumulateDBCMode();

		void SetTipValue(LPCTSTR pStrTipValue);
		CDuiString GetTipValue();
		void SetTipValueColor(LPCTSTR pStrColor);
		DWORD GetTipValueColor();
		void SetTipValueAlign(UINT uAlign);
		UINT GetTipValueAlign();

		void DoInit();
		bool SetDropAcceptFile(bool bAccept);
		// 注意：TxSendMessage和SendMessage是有区别的，TxSendMessage没有multibyte和unicode自动转换的功能，
		// 而richedit2.0内部是以unicode实现的，在multibyte程序中，必须自己处理unicode到multibyte的转换
		virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
		IDropTarget* GetTxDropTarget();
		virtual bool OnTxViewChanged();
		virtual void OnTxNotify(DWORD iNotify, void *pv);

		virtual void SetScrollPos(SIZE szPos, bool bMsg = true) override;
		virtual void LineUp() override;
		virtual void LineDown() override;
		virtual void PageUp() override;
		virtual void PageDown() override;
		virtual void HomeUp() override;
		virtual void EndDown() override;
		virtual void LineLeft() override;
		virtual void LineRight() override;
		virtual void PageLeft() override;
		virtual void PageRight() override;
		virtual void HomeLeft() override;
		virtual void EndRight() override;

		virtual int GetFixedWidth() const override;
		virtual int GetFixedHeight() const override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) override;

		virtual bool OnEnableResponseDefaultKeyEvent(WPARAM wParam) override;
	protected:
		enum { 
			DEFAULT_TIMERID = 20,
		};

		CTxtWinHost* m_pTwh;
		bool m_bVScrollBarFixing;
		bool m_bWantTab;
		bool m_bWantReturn;
		bool m_bWantCtrlReturn;
		bool m_bTransparent;
		bool m_bRich;
		bool m_bWordWrap;
		bool m_bParagraphIndent;

		int m_iLimitText;
		LONG m_lTwhStyle;
		bool m_bDrawCaret;
		bool m_bInited;

		bool  m_fAccumulateDBC ; // TRUE - need to cumulate ytes from 2 WM_CHAR msgs
		// we are in this mode when we receive VK_PROCESSKEY
		UINT m_chLeadByte; // use when we are in _fAccumulateDBC mode

		CDuiString m_sTipValue;
		DWORD m_dwTipValueColor;
		UINT m_uTipValueAlign;
		bool	m_bReturnFixedWidth;
		bool	m_bReturnFixedHeight;
	};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
