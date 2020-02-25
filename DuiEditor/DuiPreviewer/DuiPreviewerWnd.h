#pragma once


extern HANDLE g_hOutput;
inline void DuiWriteConsole(LPCTSTR szText)
{
	if(g_hOutput)
	{
		DWORD w;
		WriteFile(g_hOutput, szText, _tcslen(szText) * sizeof(TCHAR), &w, NULL);
	}
	else
	{
		_tprintf(szText);
		_tprintf(_T("\n"));
	}
}

class CDuiPreviewerWnd : public WindowImplBase, public CWebBrowserEventHandler
{
public:
	CDuiPreviewerWnd(void);
	~CDuiPreviewerWnd(void);

	virtual LPCTSTR GetWindowClassName() const  { return _T("duilib"); }
	virtual CDuiString GetSkinFile() 			{ return m_strSkinFile; }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }

	virtual void OnFinalMessage( HWND hWnd );

	//windows原生消息
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	//CPaintManagerUI消息
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	CDuiString m_strSkinFile;
};

