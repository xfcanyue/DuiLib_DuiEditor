// StdAfx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_)
#define AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_

#pragma once

#ifdef __GNUC__
// 怎么都没找到min，max的头文件-_-
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

#ifndef __FILET__
#define __DUILIB_STR2WSTR(str)	L##str
#define _DUILIB_STR2WSTR(str)	__DUILIB_STR2WSTR(str)
#ifdef _UNICODE
#define __FILET__	_DUILIB_STR2WSTR(__FILE__)
#define __FUNCTIONT__	_DUILIB_STR2WSTR(__FUNCTION__)
#else
#define __FILET__	__FILE__
#define __FUNCTIONT__	__FUNCTION__
#endif
#endif

#define _CRT_SECURE_NO_DEPRECATE

// Remove pointless warning messages
#ifdef _MSC_VER
#pragma warning (disable : 4511) // copy operator could not be generated
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
#pragma warning (disable : 4786) // identifier was truncated
#pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
#endif
#endif // _MSC_VER
#ifdef __BORLANDC__
#pragma option -w-8027		   // function not expanded inline
#endif



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef WIN32
// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WINXP//0x0501
#endif

#ifndef WINVER                 // Specify that the minimum required platform is Windows 7.
#define WINVER 0x0601          //WIN7
#endif
#define MOUSEEVENTF_FROMTOUCH 0xFF515700

#include <olectl.h>
#endif

#include "UIlib.h"

#define lengthof(x) (sizeof(x)/sizeof(*x))
#ifndef MAX
#define MAX max
#endif
#ifndef MIN
#define MIN min
#endif
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

//////////////////////////////////////////////////////////////////////////

#define PUGIXML_HEADER_ONLY
#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "Utils/pugixml/pugixml.hpp"




//////////////////////////////////////////////////////////////////////////
// 
#ifdef DUILIB_WIN32
namespace DuiLib
{
//GdiPlus初始化
#define GDIPLUS_STARTUP_INSTANCE GdiplusStartup::Instance()
class GdiplusStartup
{
public:
	//创建任何Gdiplus对象之前都要先调用一下
	static void Instance()
	{
		static GdiplusStartup obj;
	}
protected:
	GdiplusStartup()
	{
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiplusStartup()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
};
} //namespace DuiLib
#else
#define GDIPLUS_STARTUP_INSTANCE
#endif //DUILIB_WIN32

#ifdef DUILIB_GTK
#include <gtk/gtk.h>
#endif

#ifdef DUILIB_USE_RENDER_CAIRO
#	ifdef WIN32
#		include "cairo-win32.h"
#		include "pango/pangocairo.h"
#		include "pango/pangoWin32.h"
#		include "gdk-pixbuf/gdk-pixbuf.h"
#		include "gdk/gdk.h"
#		ifdef _WIN64
#			ifdef _DEBUG
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/cairo.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/pango-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/pangocairo-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/pangowin32-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/glib-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/gdk_pixbuf-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/gobject-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/gtk-3.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/debug/lib/gdk-3.0.lib")
#			else
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/cairo.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/pangocairo-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/pango-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/pangowin32-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/glib-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/gdk_pixbuf-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/gobject-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/gtk-3.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/x64/release/lib/gdk-3.0.lib")
#			endif
#		else
#			ifdef _DEBUG
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/cairo.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/pangocairo-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/pango-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/pangowin32-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/glib-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/gdk_pixbuf-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/gobject-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/gtk-3.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/debug/lib/gdk-3.0.lib")
#			else
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/cairo.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/pangocairo-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/pango-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/pangowin32-1.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/glib-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/gdk_pixbuf-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/gobject-2.0.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/gtk-3.lib")
#				pragma comment(lib, "E:/wingtk/gtk-build/gtk/Win32/release/lib/gdk-3.0.lib")
#			endif
#		endif
#	endif
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E30B2003_188B_4EB4_AB99_3F3734D6CE6C__INCLUDED_)
