#include "StdAfx.h"

#include "../Render/UIRenderFactory_Cairo.h"
#include "../Render/UIRender_Cairo.h"
#include "../Render/UIRender_CairoWin32.h"
#include "../Render/UIObject_Cairo.h"

#ifdef DUILIB_USE_RENDER_CAIRO
///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender *UIRenderFactory_Cairo::CreateRenderTarget()
	{
#ifdef DUILIB_WIN32
		return new UIRender_CairoWin32();
#else
		return new UIRender_Cairo();
#endif
	}

	UIFont *UIRenderFactory_Cairo::CreateFont()
	{
		return new UIFont_pango;
	}

	UIPen *UIRenderFactory_Cairo::CreatePen()
	{
		return new UIPen_Cairo;
	}

	UIBitmap *UIRenderFactory_Cairo::CreateBitmap()
	{
		//return new UIBitmap_Cairo;
		return new UIBitmap_Pixbuf;
	}

	UIBrush *UIRenderFactory_Cairo::CreateBrush()
	{
		return new UIBrush_Cairo;
	}

	UIImage *UIRenderFactory_Cairo::CreateImage()
	{
		return new UIImage_Cairo;
	}

// 	UIPath *UIRenderFactory_Cairo::CreatePath()
// 	{
// 		return new UIPath_Cairo;
// 	}
	

	//////////////////////////////////////////////////////////////////////////
	//
	//



} // namespace DuiLib
#endif //#ifdef DUILIB_USE_RENDER_CAIRO
