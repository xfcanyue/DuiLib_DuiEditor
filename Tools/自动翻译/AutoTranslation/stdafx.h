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
#include "E:/MyLibrary/DuiLib/DuiLib/Duilib.h"
//#include "E:/MyLibrary/DuiLib/DuiPlugins/Include/DuiPlugins.h"
//#include "E:/MyLibrary/DuiLib/DuiScript/Include/DuiScriptInterface.h"
//#include "E:/MyLibrary/DuiLib/3rd/CxImage/Include/image.h"
using namespace DuiLib;

#include "E:/MyLibrary/jsoncpp/jsoncpp/jsoncpp/json/json.h"
#ifdef _DEBUG
#pragma comment(lib, "E:/MyLibrary/jsoncpp/Lib/jsoncppd.lib")
#else
#pragma comment(lib, "E:/MyLibrary/jsoncpp/Lib/jsoncpp.lib")
#endif

#include "E:/MyLibrary/libcurl7.78.0/Include/curl/curl.h"
#ifdef _DEBUG
#pragma comment(lib, "E:/MyLibrary/libcurl7.78.0/build/Win32/VC10/DLL Debug - DLL OpenSSL/libcurld.lib")
#else
#pragma comment(lib, "E:/MyLibrary/libcurl7.78.0/build/Win32/VC10/DLL Release - DLL OpenSSL/libcurl.lib")
#endif

#include <openssl/md5.h>
#ifdef _DEBUG
#pragma comment(lib, "E:/MyLibrary/openssl-1.1.1j/win32Debug/lib/libcrypto.lib")
#pragma comment(lib, "E:/MyLibrary/openssl-1.1.1j/win32Debug/lib/libssl.lib")
#else
#pragma comment(lib, "E:/MyLibrary/openssl-1.1.1j/win32Release/lib/libcrypto.lib")
#pragma comment(lib, "E:/MyLibrary/openssl-1.1.1j/win32Release/lib/libssl.lib")
#endif

#include "App.h"

