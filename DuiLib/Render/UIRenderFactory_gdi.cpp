#include "StdAfx.h"
#include "UIRenderFactory_gdi.h"

#include "UIRender_gdi.h"
#include "UIObject_gdi.h"



///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender *UIRenderFactory_gdi::CreateRenderTarget()
	{
		return new UIRender_gdi;
	}

	UIFont *UIRenderFactory_gdi::CreateFont()
	{
		return new UIFont_gdi;
	}

	UIPen *UIRenderFactory_gdi::CreatePen()
	{
		return new UIPen_gdi;
	}

	UIBitmap *UIRenderFactory_gdi::CreateBitmap()
	{
		return new UIBitmap_gdi;
	}

	UIBrush *UIRenderFactory_gdi::CreateBrush()
	{
		return new UIBrush_gdi;
	}

	UIImage *UIRenderFactory_gdi::CreateImage()
	{
		return new UIImage_gdi;
	}

	

	//////////////////////////////////////////////////////////////////////////
	//
	//



} // namespace DuiLib
