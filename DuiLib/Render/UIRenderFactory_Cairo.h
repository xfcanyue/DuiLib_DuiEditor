#ifndef __UIRENDER_FACTORY_GTK_H__
#define __UIRENDER_FACTORY_GTK_H__

#pragma once

#ifdef DUILIB_USE_RENDER_CAIRO
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIRenderFactory_Cairo : public TObjRefImpl<UIRenderFactory>
	{
	public:
		virtual UIRender *CreateRenderTarget() override;

		virtual UIFont *CreateFont() override;
		virtual UIPen *CreatePen() override;
		virtual UIBitmap *CreateBitmap() override;
		virtual UIBrush *CreateBrush() override;
		virtual UIImage *CreateImage() override;
//		virtual UIPath *CreatePath() override;
	};

} // namespace DuiLib
#endif //#ifdef DUILIB_USE_RENDER_CAIRO

#endif // __UIRENDER_H__
