
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
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
	使用官方版或者根据官方版改造的同学，请打开这个宏（UILIB_VERSION_ORIGINAL）进行编译
	保留这个，只是为了使用官方版的同学能够编译成功，具体细化，需要您自己努力了。
	一定要维护您自己的控件定义文件： DuiLib.xml
	请不要直接使用我项目中提供的duilib原版库，应该使用您自己的库进行编译。
*/
//#define DUILIB_VERSION_ORIGINAL

#ifdef DUILIB_VERSION_ORIGINAL
	#include "DuiLibOriginal/DuiLib/UIlib.h"
	#ifdef _DEBUG
		#pragma comment (lib, "DuiLibOriginal/Lib/DuiLib_ud.lib")
	#else
		#pragma comment (lib, "DuiLibOriginal/Lib/DuiLib_u.lib")
	#endif
	using namespace DuiLib;
#else
	#include "../DuiLib/DuiLib.h"
	using namespace DuiLib;
	#include "../DuiPlugins/Include/DuiPlugins.h"
#endif
//////////////////////////////////////////////////////////////////////////
#define INCLUDE_DEPRECATED_FEATURES
#include "scintilla/include/SciLexer.h"
#include "scintilla/include/Scintilla.h"

#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"
using namespace pugi;
#define XML_PARSER_OPTIONS pugi::parse_full//parse_default|parse_declaration|parse_comments

//////////////////////////////////////////////////////////////////////////
#include "MemDCEx.h"
#include "LsStringConverter.h"

#define RULEBAR_SIZE_X	20
#define RULEBAR_SIZE_Y	20

#define WM_CREATE_UIWINDOW		WM_USER + 1

class CLockWindowUpdate
{
public:
	CLockWindowUpdate(CWnd *pWnd) 
	{ 
		m_pWnd = pWnd;
		m_pWnd->LockWindowUpdate(); 
	}
	~CLockWindowUpdate() 
	{ 
		m_pWnd->UnlockWindowUpdate(); 
	}
private:
	CWnd *m_pWnd;
};

#include "EditorConfig.h"
extern tagEditorConfig g_cfg;