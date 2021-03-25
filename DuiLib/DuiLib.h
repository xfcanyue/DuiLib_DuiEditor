#pragma once

#include "UIlib.h"
#include "Utils/UIDataExchange.hpp"

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64s.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_usd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_us.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_sd.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_s.lib"))
#		endif
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
#else

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib_d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../Lib/DuiLib.lib"))
#		endif
#	endif
#endif



//////////////////////////////////////////////////////////////////////////
#endif