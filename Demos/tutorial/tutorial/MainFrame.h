#pragma once
class CMainFrame : public CUIFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile() 			{ return _T("MainFrame.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void Notify(TNotifyUI& msg);

	virtual void InitWindow();
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnMenuCommand(const MenuCmd *cmd);
	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI);
	virtual void OnNotifyClick(TNotifyUI& msg);
	virtual void OnNotifyTimer(TNotifyUI& msg);

	void OnShowHideTopPane();
	void OnShowHideBottomPane();


	CImageBoxUI *m_pImageBox;
	CProgressUI *m_pProgress;

	CStdStringPtrMap m_MenuInfos; //²Ëµ¥Ñ¡ÖÐ×´Ì¬
	BOOL m_bCheckmenu_cmd_2;
	BOOL m_bCheckmenu_cmd_22;
	BOOL m_bCheckmenu_cmd_41;
	BOOL m_bCheckmenu_cmd_42;

	int m_nMenuSelected;
};

