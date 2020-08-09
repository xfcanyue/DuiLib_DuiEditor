#pragma once
#include "UIWindow.h"
#include "UIManager.h"
#include "UITrackerMuliti.h"

#ifndef DUILIB_VERSION_ORIGINAL

class CUIWindowMenu : public CUIWindow, public CMenuWnd
{
public:
	CUIWindowMenu(void);
	virtual ~CUIWindowMenu(void);

	virtual CPaintManagerUI *GetManager() { return &m_pm; }
	virtual CUITrackerMuliti *GetUiTracker() { return &m_tracker; }
	virtual HWND GetSafeHwnd() { return GetHWND(); }
	virtual HWND CreateUiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle, DWORD dwExStyle);

protected:
	void AddNewControlFromToolBox(xml_node nodeToolBox, CPoint pt);
	std::list<CControlUI *>	m_listTrackerSeleted;
	void OnSelectingControl(CControlUI *pControl, const CRect &rcTracker);
protected:
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	CUITrackerMuliti m_tracker;

};
#endif

