// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#define VC_EXTRALEAN
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlbase.h>
#include <atlstr.h>

// TODO: 在此处引用程序需要的其他头文件
//////////////////////////////////////////////////////////////////////////
#include "../DuiLib/Duilib.h"
#include "../DuiPlugins/Include/DuiPlugins.h"
#include "../DuiScript/Include/DuiScriptInterface.h"
//#include "E:/MyLibrary/DuiLib/3rd/CxImage/Include/image.h"
using namespace DuiLib;

#define PUGIXML_HEADER_ONLY
#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "pugixml/pugixml.hpp"
using namespace pugi;


#include "LsStringConverter.h"

extern CString g_strSkinPath;
extern CString g_strSkinFile;
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