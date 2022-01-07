#pragma once

namespace DuiLib
{

class CComboEditWnd : public CWindowWnd
{
public:
	CComboEditWnd();

	void Init(CComboExUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

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