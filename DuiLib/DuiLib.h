#pragma once

#include "UIlib.h"

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_64ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_64d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib_d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/DuiLib.lib"))
#		endif
#	endif
#endif