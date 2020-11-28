#pragma once

#ifdef UILIB_STATIC
#define UILIB_PLUGIN_API 
#else
#if defined(UILIB_PLUGIN_API_EXPORTS)
#	if defined(_MSC_VER)
#		define UILIB_PLUGIN_API __declspec(dllexport)
#	else
#		define UILIB_PLUGIN_API 
#	endif
#else
#	if defined(_MSC_VER)
#		define UILIB_PLUGIN_API __declspec(dllimport)
#	else
#		define UILIB_PLUGIN_API 
#	endif
#endif
#endif


#include "../UIDataExchange.h"

#define DUI_CTR_COMBOEX						(_T("ComboEx"))
#include "../UIComboEx.h"

#define DUI_CTR_GRIDCTRL					(_T("GridCtrl"))
#include "../MFCGridCtrl/GridCtrl.h"
#include "../UIGridCtrl.h"

#define DUI_CTR_ICONBUTTON					(_T("IconButton"))
#include "../UIIconButton.h"

#define DUI_CTR_MSGWND						(_T("RichEdit"))
#include "../UIMsgWnd.h"

#include "../UIKeyBoard.h"
#include "../UIKeyBoardNumber.h"
#include "../UIKeyBoardSimple.h"

#define DUI_CTR_PICTURECONTROL				(_T("PictureControl"))
#include "../UIPictureControl.h"

#define DUI_CTR_DATETIMEEX					(_T("DateTimeEx"))
#include "../UIDateTimeEx.h"

#define DUI_CTR_ACCORDIONPANE				(_T("AccordionPane"))	
#include "../UIAccordionPane.h"

#define DUI_CTR_IMAGEBOX					(_T("ImageBox"))
#include "../UIImageBox.h"

#define DUI_CTR_IMAGEBOXEX					(_T("ImageBoxEx"))
#include "../UIImageBoxEx.h"

#define DUI_CTR_ROLLTEXTEX					(_T("RollTextEx"))
#include "../UIRollTextEx.h"

#define DUI_CTR_SCIEDIT						(_T("SciEdit"))
#include "../UISciEdit.h"

//extern "C" 
UILIB_PLUGIN_API void DuiPluginsRegister();

//extern "C" 
UILIB_PLUGIN_API void InsertMsgUI(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));
UILIB_PLUGIN_API void InsertMsgUiV(LPCTSTR lpszFormat, ...);

extern "C" UILIB_PLUGIN_API CControlUI *CreateControl(LPCTSTR pstrClass);


#ifndef UILIB_PLUGIN_API_EXPORTS

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins.lib"))
#		endif
#	endif
#endif

#endif