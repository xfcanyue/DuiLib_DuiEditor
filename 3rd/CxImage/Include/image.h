#pragma once

#define USE_LS_CXIMAGE
#define _CRT_SECURE_NO_WARNINGS

#ifdef UILIB_STATIC
#define CXIMAGE_STATIC
#endif

#include "../cximage701_full/CxImage/ximage.h"

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef CXIMAGE_STATIC
#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/CxImageud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/jasperud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/jbigud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/Jpegud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/libdcrud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/libpsdud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/mngud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/pngud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/Tiffud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/zlibud.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/CxImageu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/jasperu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/jbigu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/Jpegu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/libdcru.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/libpsdu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/mngu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/pngu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/Tiffu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/x64/static/zlibu.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64_staticd.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64_static.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/CxImageud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jasperud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jbigud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Jpegud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libdcrud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libpsdud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/mngud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/pngud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Tiffud.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/zlibud.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/CxImageu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jasperu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jbigu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Jpegu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libdcru.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libpsdu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/mngu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/pngu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Tiffu.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/zlibu.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/CxImaged.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jasperd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jbigd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Jpegd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libdcrd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libpsdd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/mngd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/pngd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Tiffd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/zlibd.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/CxImage.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jasper.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/jbig.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Jpeg.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libdcr.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/libpsd.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/mng.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/png.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/Tiff.lib"))
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/static/zlib.lib"))
#		endif
#	endif
#endif


#else


#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64ud.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64u.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64d.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage64.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImageud.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImageu.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImaged.lib"))
#		else
#			pragma comment(lib, _LIBPATH_(__FILE__,   "/../../Lib/CxImage.lib"))
#		endif
#	endif
#endif


#endif