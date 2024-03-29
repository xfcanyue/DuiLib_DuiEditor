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
#include "E:/MyLibrary/DuiLib/DuiPlugins/Include/DuiPlugins.h"
#include "E:/MyLibrary/DuiLib/DuiScript/Include/DuiScriptInterface.h"
#include "E:/MyLibrary/DuiLib/3rd/CxImage/Include/image.h"
using namespace DuiLib;

#include "setupapi.h"
#pragma comment(lib, "setupapi.lib")

#include <initguid.h>
#include "cfgmgr32.h"

inline CDuiString FormatGuid(GUID *guid)
{
	CDuiString sGuid;
	sGuid.Format(_T("{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"), 
		guid->Data1, 
		guid->Data2, 
		guid->Data3, 
		guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3], 
		guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);

	return sGuid;
};

#include "TreeDeviceUI.h"
#include "App.h"

typedef CMAPI CONFIGRET (WINAPI *CM_GET_DEVNODE_PROPERTY)(DEVINST dnDevInst, const DEVPROPKEY *PropertyKey, DEVPROPTYPE *PropertyType, PBYTE PropertyBuffer, PULONG PropertyBufferSize, ULONG ulFlags);
typedef CMAPI CONFIGRET (WINAPI *CM_GET_CLASS_PROPERTY)(LPCGUID ClassGUID, const DEVPROPKEY *PropertyKey, DEVPROPTYPE *PropertyType, PBYTE PropertyBuffer, PULONG PropertyBufferSize, ULONG ulFlags );
