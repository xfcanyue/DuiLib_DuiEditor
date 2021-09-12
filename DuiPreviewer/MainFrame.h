#pragma once

class CMainFrame : public CUIFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const  { return _T("DuiPreview"); }
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg) override;

	void LoadConfigXml();
};

