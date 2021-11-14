#pragma once
#include <list>

namespace DuiLib {

typedef MenuCmd MENUCOMMAND;

class CUIForm;
class UILIB_API CUIFrameWnd : public WindowImplBase, public CUIFrmBase
{
public:
	CUIFrameWnd(void);
	virtual ~CUIFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const = 0;
	virtual CDuiString GetSkinFile() = 0;
	virtual void OnFinalMessage( HWND hWnd );

	virtual CPaintManagerUI *GetManager() { return &m_pm; }
	virtual CControlUI *GetRoot() { return m_pm.GetRoot(); }
	void AttachVirtualForm(CUIForm *pForm);
	void DetachVirtualForm(CUIForm *pForm);

	void ShowFullScreen(); //»´∆¡œ‘ æ
public:
	virtual void InitWindow(){}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMenuCommandMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	virtual void __InitWindow();
public:
	CUIApplication *m_pApplication;

protected:
	std::list<CUIForm *>	m_listForm;
};

inline CMenuWnd *DuiCreateMenu(LPCTSTR xmlFile, CPaintManagerUI *pManager)
{
	CDuiPoint pt;
	::GetCursorPos(&pt);
	CMenuWnd *pMenuWnd = CMenuWnd::CreateMenu(NULL, xmlFile, pt, pManager);
	pMenuWnd->m_bAutoDestroy = true;
	if(pMenuWnd)
	{
		pMenuWnd->ResizeMenu();
	}
	return pMenuWnd;
}

} //namespace DuiLib {