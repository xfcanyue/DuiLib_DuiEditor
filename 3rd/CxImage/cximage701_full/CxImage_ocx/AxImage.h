#if !defined(AFX_AXIMAGE_H__4FED1ACB_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_)
#define AFX_AXIMAGE_H__4FED1ACB_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AxImage.h : main header file for AXIMAGE.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAxImageApp : See AxImage.cpp for implementation.

class CAxImageApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIMAGE_H__4FED1ACB_6CF3_11D6_BB83_9FDD9223647D__INCLUDED)
