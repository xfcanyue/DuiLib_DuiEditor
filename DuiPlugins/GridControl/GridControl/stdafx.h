// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

/*
#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT
*/

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

// #include "../../CxImage/ximage.h"
// #ifdef _DEBUG
// #pragma comment(lib, "../../CxImage/imaged.lib")
// #else
// #pragma comment(lib, "../../CxImage/image.lib")
// #endif
// 
// #include "../../MyAdo/MyAdo/Ado.h"
// #include "../../MyAdo/MyAdo/ExcelFile.h"
// #ifdef _DEBUG
// #pragma comment(lib, "../../../Exec/MyAdod.lib")
// #else
// #pragma comment(lib, "../../../Exec/MyAdo.lib")
// #endif


#include "../../ExcelFile/Include/ExcelFile.h"
/*
#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFileux64d.lib")
#		else
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFileux64.lib")
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFilex64d.lib")
#		else
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFilex64.lib")
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFileud.lib")
#		else
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFileu.lib")
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFiled.lib")
#		else
#			pragma comment(lib, "../../ExcelFile/Lib/ExcelFile.lib")
#		endif
#	endif
#endif
*/
//#include "../../pugixml/pugixml.hpp"
#include "TinyXml2/tinyxml2.h"

#define XF_GRIDCONTROL_CTRL __declspec(dllexport)