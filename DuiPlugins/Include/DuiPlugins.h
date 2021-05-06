#pragma once

#ifndef __DUIPLUGINS_H__
#define __DUIPLUGINS_H__

#ifdef UILIB_STATIC
#define UILIB_EXP_API 
#else
#if defined(UILIB_EXPORTS)
#	if defined(_MSC_VER)
#		define UILIB_EXP_API extern "C" __declspec(dllexport)
#	else
#		define UILIB_EXP_API 
#	endif
#else
#	if defined(_MSC_VER)
#		define UILIB_EXP_API extern "C" __declspec(dllimport)
#	else
#		define UILIB_EXP_API 
#	endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////

#define DUI_CTR_GRIDCTRL					(_T("GridCtrl"))
#include "../MFCGridCtrl/GridCtrl.h"
#include "../DuiLib/UIGridCtrl.h"

#include "../DuiLib/UIKeyBoard.h"
#include "../DuiLib/UIKeyBoardNumber.h"
#include "../DuiLib/UIKeyBoardSimple.h"

#define DUI_CTR_SCIEDIT						(_T("SciEdit"))
#include "../DuiLib/UISciEdit.h"

#define DUI_CTR_GRIDLIST					(_T("GridList"))
#define DUI_CTR_GRIDLISTHEADER				(_T("GridListHeader"))
#define DUI_CTR_GRIDLISTBODY				(_T("GridListBody"))
#define DUI_CTR_GRIDLISTROW					(_T("GridListRow"))
#define DUI_CTR_GRIDLISTCELL				(_T("GridListCell"))
#include "../DuiLib/UIGridList.h"

UILIB_EXP_API void __stdcall DuiPluginsRegister();

UILIB_EXP_API void __stdcall InsertMsgUI(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));
UILIB_EXP_API void __stdcall InsertMsgUiV(LPCTSTR lpszFormat, ...);

#ifndef UILIB_STATIC
UILIB_EXP_API CControlUI *CreateControl(LPCTSTR pstrClass);
#endif


#ifndef UILIB_EXPORTS

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
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiPlugins_s.lib"))
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

#endif //#ifndef UILIB_EXPORTS


#endif //#ifndef __DUIPLUGINS_H__