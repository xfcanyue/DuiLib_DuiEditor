#pragma once
#include "AutoTranslation.h"
#include "MainFrame.h"

#define WM_TRANSLATE_COMPLETION			UIMSG_USER + 1
#define WM_AUTO_SELECTED_DEST_LANGUAGE	UIMSG_USER + 2

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