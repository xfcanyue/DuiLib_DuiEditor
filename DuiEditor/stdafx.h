
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#pragma warning(disable:4996)

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "../DuiLib/UIlib.h"
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "../Lib/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "../Lib/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "../Lib/DuiLib_u.lib")
#   else
#       pragma comment(lib, "../Lib/DuiLib.lib")
#   endif
#endif
using namespace DuiLib;

//#include "E:/Mylibrary/GridControl/Include/GridControl.h"
#include "../DuiPlugins/Include/DuiPlugins.h"

#include "scintilla/include/Scintilla.h"
#include "scintilla/include/SciLexer.h"

#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"
using namespace pugi;
#define XML_PARSER_OPTIONS pugi::parse_full//parse_default|parse_declaration|parse_comments

/*
#include "../DuiLib/3rd/DuiAngelScript/include/angelscript.h"
#ifdef _DEBUG
#pragma comment(lib, "../DuiLib/3rd/DuiAngelScript/Lib/DuiAngelScriptd.lib")
#else
#pragma comment(lib, "../DuiLib/3rd/DuiAngelScript/Lib/DuiAngelScript.lib")
#endif
*/

#include "MemDCEx.h"



#define RULEBAR_SIZE_X	20
#define RULEBAR_SIZE_Y	20


#define WM_CREATE_UIWINDOW		WM_USER + 1



extern pugi::xml_document g_proj;