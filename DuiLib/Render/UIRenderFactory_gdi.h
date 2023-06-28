#ifndef __UIRENDER_FACTORY_GDI_H__
#define __UIRENDER_FACTORY_GDI_H__

#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIRenderFactory_gdi : public TObjRefImpl<UIRenderFactory>
	{
	public:
		virtual UIRender *CreateRenderTarget() override;

		virtual UIFont *CreateFont() override;
		virtual UIPen *CreatePen() override;
		virtual UIBitmap *CreateBitmap() override;
		virtual UIBrush *CreateBrush() override;
		virtual UIImage *CreateImage() override;
	};

} // namespace DuiLib
#endif // #ifdef DUILIB_WIN32
#endif // __UIRENDER_H__
