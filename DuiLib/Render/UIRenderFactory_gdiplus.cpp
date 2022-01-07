#include "StdAfx.h"
#include "UIRenderFactory_gdiplus.h"

#include "UIRender_gdiplus.h"
#include "UIObject_gdiplus.h"



///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender *UIRenderFactory_gdiplus::CreateRenderTarget()
	{
		return new UIRender_gdiplus;
	}

	UIFont *UIRenderFactory_gdiplus::CreateFont()
	{
		return new UIFont_gdiplus;
	}

	UIPen *UIRenderFactory_gdiplus::CreatePen()
	{
		return new UIPen_gdiplus;
	}

	UIBitmap *UIRenderFactory_gdiplus::CreateBitmap()
	{
		return new UIBitmap_gdiplus;
	}

	UIBrush *UIRenderFactory_gdiplus::CreateBrush()
	{
		return new UIBrush_gdiplus;
	}

	UIImage *UIRenderFactory_gdiplus::CreateImage()
	{
		return new UIImage_gdiplus;
	}

	

	//////////////////////////////////////////////////////////////////////////
	//
	//



} // namespace DuiLib
