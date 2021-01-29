#pragma once

#ifdef UILIB_STATIC
#	define UILIB_PLUGIN_API 
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


#include "../DuiLib/UIDataExchange.h"

#define DUI_CTR_COMBOEX						(_T("ComboEx"))
#include "../DuiLib/UIComboEx.h"

#define DUI_CTR_GRIDCTRL					(_T("GridCtrl"))
#include "../MFCGridCtrl/GridCtrl.h"
#include "../DuiLib/UIGridCtrl.h"

#define DUI_CTR_ICONBUTTON					(_T("IconButton"))
#include "../DuiLib/UIIconButton.h"

#define DUI_CTR_MSGWND						(_T("RichEdit"))
#include "../DuiLib/UIMsgWnd.h"

#include "../DuiLib/UIKeyBoard.h"
#include "../DuiLib/UIKeyBoardNumber.h"
#include "../DuiLib/UIKeyBoardSimple.h"

#define DUI_CTR_PICTURECONTROL				(_T("PictureControl"))
#include "../DuiLib/UIPictureControl.h"

#define DUI_CTR_DATETIMEEX					(_T("DateTimeEx"))
#include "../DuiLib/UIDateTimeEx.h"

#define DUI_CTR_ACCORDIONPANE				(_T("AccordionPane"))	
#include "../DuiLib/UIAccordionPane.h"

#define DUI_CTR_IMAGEBOX					(_T("ImageBox"))
#include "../DuiLib/UIImageBox.h"

#define DUI_CTR_IMAGEBOXEX					(_T("ImageBoxEx"))
#include "../DuiLib/UIImageBoxEx.h"

#define DUI_CTR_ROLLTEXTEX					(_T("RollTextEx"))
#include "../DuiLib/UIRollTextEx.h"

#define DUI_CTR_SCIEDIT						(_T("SciEdit"))
#include "../DuiLib/UISciEdit.h"

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

//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC


#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_64s.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPluginss.lib"))
#		endif
#	endif
#endif

#else //#ifdef UILIB_STATIC

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

#endif //#ifdef UILIB_STATIC

#endif