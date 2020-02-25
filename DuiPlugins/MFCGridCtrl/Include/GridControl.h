#pragma once

#ifndef XF_GRIDCONTROL_CTRL
#define XF_GRIDCONTROL_CTRL __declspec(dllimport)
#endif

#include "../GridControl/GridCtrl.h"

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControl64ud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControl64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControl64d.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControl64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControlud.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControlu.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControld.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../Lib/GridControl.lib"))
#		endif
#	endif
#endif