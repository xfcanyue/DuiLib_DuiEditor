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

#include "../../3rd/CxImage/Include/image.h"
#include "../../3rd/GridControl/Include/GridControl.h"

#include "../ComboExUI.h"
#include "../GridCtrlUI.h"
#include "../MsgWndUI.h"
#include "../KeyBoardUI.h"
#include "../KeyBoardNumberUI.h"
#include "../KeyBoardSimpleUI.h"
#include "../ImageBoxUI.h"
#include "../UIDataExchange.h"
#include "../PictureControlUI.h"

//extern "C" 
UILIB_PLUGIN_API void DuiPluginsRegister();

//extern "C" 
UILIB_PLUGIN_API void InsertMsgUI(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));

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