#pragma once

#ifdef DUILIB_GTK
namespace DuiLib
{

class CComboEditWndGtk : public CWindowWnd
{
public:
	CComboEditWndGtk();

	void Init(CComboExUI* pOwner);
	RECT CalPos();

	void OnFinalMessage(UIWND hWnd);

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
	bool m_bInit;
	bool m_bDrawCaret;

	CStdRefPtr<UIFont> m_font;
	CStdRefPtr<UIBitmap> m_bmpBrush;
	CStdRefPtr<UIBrush> m_brush;
};


}
#endif //#ifdef DUILIB_GTK
