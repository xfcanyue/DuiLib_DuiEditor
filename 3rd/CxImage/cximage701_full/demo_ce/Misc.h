#pragma once

//
//	Class:		CMisc
//
//	Compiler:	eMbedded Visual C++, Visual6, Visual_2003, Visual_2005
//	Tested on:	Windows CE 3.0, PPC2003, PPC2005
//
//	Version:
//
//	Created:	10/03/2006
//	Updated:
//
//	Author:		Vincent RICHOMME'
//
//  Constraints : YOU MUST USE SAFE C RUN-TIME TO PREVENT BUFFER-OVERFLOW
//
//  TODO : PROVIDE A Fixed Path version for better performance
//
//	Note:		Misceleanous functions - Waiting for better place

#define KEY_NUMERIC 1
#define KEY_ALPHANUMERIC 2

class CMisc
{
public:

//static CString	GetAppPath();
static int		Message(LPCTSTR szFmt, ...);
//static BOOL		GetInputPanelState();
//static BOOL		ChangeInputPanelState(int nState, int nKeypad=KEY_ALPHANUMERIC);

};

