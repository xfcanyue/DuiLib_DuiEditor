#ifndef __UIRENDER_FACTORY_GDIPLUS_H__
#define __UIRENDER_FACTORY_GDIPLUS_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIRenderFactory_gdiplus : public TObjRefImpl<UIRenderFactory>
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

#endif // __UIRENDER_H__
