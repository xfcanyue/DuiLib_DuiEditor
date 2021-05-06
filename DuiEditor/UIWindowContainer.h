#pragma once
#include "UIWindow.h"
#include "UITrackerMuliti.h"

class CUIWindowContainer : public CUIWindow, public WindowImplBase
{
public:
	CUIWindowContainer(void);
	virtual ~CUIWindowContainer(void);
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return _T(""); }
	virtual LPCTSTR GetWindowClassName(void) const { return _T("UIWindowContainer"); }

	virtual CPaintManagerUI *GetManager() { return &m_pm; }

	virtual HWND GetSafeHwnd() { return GetHWND(); }
	virtual HWND CreateUiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle, DWORD dwExStyle) 
	{ return CreateDuiWindow(hwndParent, pstrWindowName, dwStyle, dwExStyle); }
public:
	CRectTracker m_tracker;
	CControlUI *m_pControl;

	void ResizeWindow();
	virtual void OnChangeRect();
public:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

