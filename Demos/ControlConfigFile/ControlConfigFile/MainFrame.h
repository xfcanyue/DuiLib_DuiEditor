#pragma once

class CMainFrame : public CUIFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile()			{ return _T("ControlConfigFile.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void InitWindow();
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnMenuCommand(const MenuCmd *cmd);
	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI);
	virtual void OnNotifyClick(TNotifyUI& msg);


	//可以从设计器中生成这段代码。
	CEditUI *m_edit_text;
	CRichEditUI *m_redit_text;
	COptionUI *m_opt_check;
	CComboExUI *m_cb_recv_type;
};

