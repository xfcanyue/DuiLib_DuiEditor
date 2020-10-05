// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
/*
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
*/
#pragma warning(disable:4996)

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
//////////////////////////////////////////////////////////////////////////
/*
	使用官方版或者根据官方版改造的同学，请打开这个宏（UILIB_VERSION_ORIGINAL）进行编译
	保留这个，只是为了使用官方版的同学能够编译成功，具体细化，需要您自己努力了。
	一定要维护您自己的控件定义文件： DuiLib.xml
	请不要直接使用我项目中提供的duilib原版库，应该使用您自己的库进行编译。
*/
//#define DUILIB_VERSION_ORIGINAL

#ifdef DUILIB_VERSION_ORIGINAL
	#include "../DuiLibOriginal/DuiLib/UIlib.h"
	#ifdef _DEBUG
		#pragma comment (lib, "../DuiLibOriginal/Lib/DuiLib_ud.lib")
	#else
		#pragma comment (lib, "../DuiLibOriginal/Lib/DuiLib_u.lib")
	#endif
	using namespace DuiLib;
#else
	#include "../../DuiLib/DuiLib.h"
	using namespace DuiLib;
#endif


#define PUGIXML_WCHAR_MODE
#include "../pugixml/pugixml.hpp"
	using namespace pugi;