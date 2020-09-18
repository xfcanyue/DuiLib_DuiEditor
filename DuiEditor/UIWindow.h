#pragma once

// CUIWindow
class CUIManager;
class CUIWindow
{
public:
	CUIWindow();
	virtual ~CUIWindow();
	static CString GetMessageText(UINT message);

	virtual CPaintManagerUI *GetManager()  = 0;
	virtual HWND GetSafeHwnd() = 0;

	virtual HWND CreateUiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle, DWORD dwExStyle) = 0;
	virtual BOOL DestroyUiWindow();
	virtual BOOL GetWindowRect(LPRECT lpRect);
	virtual void Invalidate();
	virtual BOOL MoveWindow(__in int X, __in int Y, __in int nWidth, __in int nHeight, __in BOOL bRepaint);

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;
};


