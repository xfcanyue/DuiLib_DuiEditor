#ifndef __UIHOTKEY_H__
#define __UIHOTKEY_H__
#pragma once

namespace DuiLib{
	class CHotKeyUI;

	class UILIB_API CHotKeyWnd : public CWindowWnd
	{
	public:
		CHotKeyWnd(void);

	public:
		void Init(CHotKeyUI * pOwner);
		RECT CalPos();
		LPCTSTR GetWindowClassName() const;
		void OnFinalMessage(HWND hWnd);
		LPCTSTR GetSuperClassName() const;
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	public:
		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);
		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const;
		DWORD GetHotKey(void) const;
		CDuiString GetHotKeyName();
		void SetRules(WORD wInvalidComb, WORD wModifiers);
		CDuiString GetKeyName(UINT vk, BOOL fExtended);
	protected:
		CHotKeyUI * m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		CStdRefPtr<UIFont> m_font;
	};

	class UILIB_API CHotKeyUI : public CLabelUI
	{
		DECLARE_DUICONTROL(CHotKeyUI)
		friend CHotKeyWnd;
	public:
		CHotKeyUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;
		virtual void SetText(LPCTSTR pstrText) override;
		
		virtual void SetNativeBkColor(DWORD dwBkColor);
		virtual DWORD GetNativeBkColor() const;

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
		virtual void SetVisible(bool bVisible = true) override;
		virtual void SetInternVisible(bool bVisible = true) override;
		virtual SIZE EstimateSize(SIZE szAvailable) override;
		virtual void DoEvent(TEventUI& event) override;
		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	public:
		void GetHotKey(WORD &wVirtualKeyCode, WORD &wModifiers) const;
		DWORD GetHotKey(void) const;
		void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);

	protected:
		CHotKeyWnd * m_pWindow;
		DWORD m_dwHotKeybkColor;

	protected:
		WORD m_wVirtualKeyCode;
		WORD m_wModifiers;
	};
}


#endif