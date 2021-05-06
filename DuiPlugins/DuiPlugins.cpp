// DuiPlugins.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

UILIB_EXP_API void __stdcall DuiPluginsRegister()
{
 	REGIST_DUICONTROL(CGridCtrlUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CKeyBoardNumberUI);
	REGIST_DUICONTROL(CKeyBoardSimpleUI);
	REGIST_DUICONTROL(CSciEditUI);
	REGIST_DUICONTROL(CGridListUI);
	REGIST_DUICONTROL(CGridListHeaderUI);
	REGIST_DUICONTROL(CGridListBodyUI);
	REGIST_DUICONTROL(CGridListRowUI);
	REGIST_DUICONTROL(CGridListCellUI);
}


#ifndef UILIB_STATIC
UILIB_EXP_API CControlUI *CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("GridCtrl")) == 0 ) 
	{
		return new CGridCtrlUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoard")) == 0 ) 
	{
		return new CKeyBoardUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardNumber")) == 0 ) 
	{
		return new CKeyBoardNumberUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardSimple")) == 0 ) 
	{
		return new CKeyBoardSimpleUI;
	}
	else if( _tcscmp(pstrClass, _T("ComboEx")) == 0 ) 
	{
		return new CComboExUI;
	}
	else if( _tcscmp(pstrClass, _T("SciEdit")) == 0 ) 
	{
		return new CSciEditUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLIST) == 0 ) 
	{
		return new CGridListUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTHEADER) == 0 ) 
	{
		return new CGridListHeaderUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTBODY) == 0 ) 
	{
		return new CGridListBodyUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTROW) == 0 ) 
	{
		return new CGridListRowUI;
	}
	else if( _tcscmp(pstrClass, DUI_CTR_GRIDLISTCELL) == 0 ) 
	{
		return new CGridListCellUI;
	}
	return NULL;
}
#endif

UILIB_EXP_API void __stdcall InsertMsgUI(LPCTSTR pstring, COLORREF cr)
{
	CMsgWndUI::InsertMsg(pstring, cr);
}

UILIB_EXP_API void __stdcall InsertMsgUiV(LPCTSTR lpszFormat, ...)
{
//	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	CMsgWndUI::InsertMsg(strText, RGB(0,0,0));
}