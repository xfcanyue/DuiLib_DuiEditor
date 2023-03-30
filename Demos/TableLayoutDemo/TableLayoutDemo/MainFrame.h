#pragma once

class CMainFrame : public CUIFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile() 			{ return _T("Test.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T("Test"); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void InitWindow();
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnMenuCommand(const MenuCmd *cmd);
	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI);
	virtual void OnNotifyClick(TNotifyUI& msg);
};

