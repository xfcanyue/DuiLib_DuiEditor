#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib
{

class CComboEditWndWin32 : public CWindowWnd
{
public:
	CComboEditWndWin32();

	void Init(CComboExUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void edit_SetPasswordMode(BOOL bPasswordMode);
	void edit_SetSel(int ichStart, int ichEnd);
	void edit_SetText(LPCTSTR pstrText);
	void edit_LimitText(UINT uMax);
	void edit_SetReadOnly(BOOL bReadOnly);
	void edit_SetPasswordChar(TCHAR cPasswordChar);
	void edit_ReplaceSel(LPCTSTR lpszReplace);
protected:
	enum { 
		DEFAULT_TIMERID = 21,
	};

	CComboExUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
	bool m_bDrawCaret;

	CStdRefPtr<UIFont> m_font;
	CStdRefPtr<UIBitmap> m_bmpBrush;
	CStdRefPtr<UIBrush> m_brush;
};


}
#endif // #ifdef DUILIB_WIN32
