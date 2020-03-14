#pragma once

#ifndef UILIB_PLUGIN_API
#define UILIB_PLUGIN_API __declspec(dllimport)
#endif

#include "../CxImage/Include/image.h"
#include "../GridControl/Include/GridControl.h"

#include "../ComboExUI.h"
#include "../GridCtrlUI.h"
#include "../MsgWndUI.h"
#include "../KeyBoardUI.h"
#include "../ImageBoxUI.h"
#include "../UIDataExchange.h"
#include "../PictureControlUI.h"

extern "C" __declspec(dllimport) void DuiPluginsRegister();

extern "C" __declspec(dllimport) void InsertMsgUI(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));

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