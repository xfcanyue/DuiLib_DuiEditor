// dllmain.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#include "Include/DuiPlugins.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE DuiPluginsDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DuiPlugins.DLL 正在初始化!\n");
		
		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(DuiPluginsDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL (如 ActiveX 控件)隐式链接到，
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(DuiPluginsDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DuiPlugins.DLL 正在终止!\n");

		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(DuiPluginsDLL);
	}
	return 1;   // 确定
}

UILIB_PLUGIN_API void DuiPluginsRegister()
{
	REGIST_DUICONTROL(CGridCtrlUI);
	REGIST_DUICONTROL(CMsgWndUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CKeyBoardNumberUI);
	REGIST_DUICONTROL(CKeyBoardSimpleUI);
	REGIST_DUICONTROL(CImageBoxUI);
	REGIST_DUICONTROL(CPictureControlUI);
	REGIST_DUICONTROL(CComboExUI);
	REGIST_DUICONTROL(CIconButtonUI);
	REGIST_DUICONTROL(CDateTimeExUI);
	REGIST_DUICONTROL(CAccordionPaneUI);
	REGIST_DUICONTROL(CAnimationPaneUI);
	REGIST_DUICONTROL(CImageBoxExUI);
	REGIST_DUICONTROL(CRollTextExUI);
}

extern "C" UILIB_PLUGIN_API CControlUI *CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("GridCtrl")) == 0 ) 
	{
		return new CGridCtrlUI;
	}
	else if( _tcscmp(pstrClass, _T("MsgWnd")) == 0 ) 
	{
		return new CMsgWndUI;
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
	else if( _tcscmp(pstrClass, _T("ImageBox")) == 0 ) 
	{
		return new CImageBoxUI;
	}
	else if( _tcscmp(pstrClass, _T("PictureControl")) == 0 ) 
	{
		return new CPictureControlUI;
	}
	else if( _tcscmp(pstrClass, _T("ComboEx")) == 0 ) 
	{
		return new CComboExUI;
	}
	else if( _tcscmp(pstrClass, _T("IconButton")) == 0 ) 
	{
		return new CIconButtonUI;
	}
	else if( _tcscmp(pstrClass, _T("DateTimeEx")) == 0 ) 
	{
		return new CDateTimeExUI;
	}
	else if( _tcscmp(pstrClass, _T("AccordionPane")) == 0 ) 
	{
		return new CAccordionPaneUI;
	}
	else if( _tcscmp(pstrClass, _T("AnimationPane")) == 0 ) 
	{
		return new CAnimationPaneUI;
	}
	else if( _tcscmp(pstrClass, _T("ImageBoxEx")) == 0 ) 
	{
		return new CImageBoxExUI;
	}
	else if( _tcscmp(pstrClass, _T("RollTextEx")) == 0 ) 
	{
		return new CRollTextExUI;
	}
	return NULL;
}

UILIB_PLUGIN_API void InsertMsgUI(LPCTSTR pstring, COLORREF cr)
{
	CMsgWndUI::InsertMsg(pstring, cr);
}

UILIB_PLUGIN_API void InsertMsgUiV(LPCTSTR lpszFormat, ...)
{
	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	CMsgWndUI::InsertMsg(strText, RGB(0,0,0));
}