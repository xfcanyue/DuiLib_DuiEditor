#pragma once
#include "TestListDemo.h"
#include "MainFrame.h"

class CApp : public CUIApplication
{
public:
	CApp(void);
	virtual ~CApp(void);

	virtual bool InitInstance(HINSTANCE hInstance);
	virtual int ExitInstance();
};


extern CApp uiApp;
inline CMainFrame *GetMainWnd(){ return (CMainFrame *)uiApp.GetMainWnd(); }