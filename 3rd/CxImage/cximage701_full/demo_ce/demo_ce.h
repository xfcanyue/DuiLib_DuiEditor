// demo_ce.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// Cdemo_ceApp:
// See demo_ce.cpp for the implementation of this class
//

class Cdemo_ceApp : public CWinApp
{
public:
	Cdemo_ceApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cdemo_ceApp theApp;
