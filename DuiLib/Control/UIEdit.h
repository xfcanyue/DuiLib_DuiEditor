#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

namespace DuiLib
{
	class CEditWnd;

	class UILIB_API CEditUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CEditUI)
		friend class CEditWnd;
	public:
		CEditUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(bool bReadOnly);
		void SetPasswordMode(bool bPasswordMode);
		bool IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		void SetNumberOnly(bool bNumberOnly);
		bool IsNumberOnly() const;
		void SetLowerCase(bool bLowerCase);
		bool IsLowerCase() const;
		void SetUpperCase(bool bUpperCase);
		bool IsUpperCase() const;
		void SetMultiLine(bool bMultiLine);
		bool IsMultiLine() const;
		void SetWantReturn(bool bWantReturn = true);
		bool IsWantReturn();
		int GetWindowStyls() const;

		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;
		void SetNativeEditTextColor( LPCTSTR pStrColor );
		DWORD GetNativeEditTextColor() const;

		bool IsAutoSelAll();
		void SetAutoSelAll(bool bAutoSelAll);
		void SetSel(long nStartChar, long nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		void SetTipValue(LPCTSTR pStrTipValue);
		CDuiString GetTipValue();
		void SetTipValueColor(LPCTSTR pStrColor);
		DWORD GetTipValueColor();

		void SetPos(RECT rc, bool bNeedInvalidate = true);
		void Move(SIZE szOffset, bool bNeedInvalidate = true);
		void SetVisible(bool bVisible = true);
		void SetInternVisible(bool bVisible = true);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintText(UIRender *pRender);

		virtual bool OnEnableResponseDefaultKeyEvent(WPARAM wParam) override;
	protected:
		CEditWnd* m_pWindow;

		UINT m_uMaxChar;
		bool m_bPasswordMode;
		bool m_bAutoSelAll;
		TCHAR m_cPasswordChar;

		bool m_bMultiLine;
		bool m_bWantReturn;

		CDuiString m_sTipValue;
		DWORD m_dwTipValueColor;
		DWORD m_dwEditbkColor;
		DWORD m_dwEditTextColor;
		int m_iWindowStyls;
	};
}
#endif // __UIEDIT_H__