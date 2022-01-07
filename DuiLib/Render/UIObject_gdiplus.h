#ifndef __UIFONT_GDIPLUS_H__
#define __UIFONT_GDIPLUS_H__

#pragma once
#include "IRender.h"
#include "UIObject_gdi.h"

namespace DuiLib { 
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIFont_gdiplus : public UIFont_gdi
	{
	public:
		UIFont_gdiplus() {}
	protected:
		virtual ~UIFont_gdiplus() {}
	};

// 	class UILIB_API UIFont_gdiplus : public UIFont
// 	{
// 	public:
// 		UIFont_gdiplus();
// 	protected:
// 		virtual ~UIFont_gdiplus();
// 
// 	public:
// 		virtual void DeleteObject() override;
// 
// 		virtual HFONT GetHFont(CPaintManagerUI *pManager) override;
// 
// 		virtual UIFont* Clone(CPaintManagerUI *pManager) override;
// 
// 		virtual int GetHeight(CPaintManagerUI *pManager) override;
// 
// 		Gdiplus::Font *GetGdiPlusFont() const;
// 
// 	protected:
// 		virtual BOOL _buildFont(CPaintManagerUI *pManager=NULL) override;
// 
// 	protected:
// 		Gdiplus::Font *m_pFont;
// 		HFONT m_hFont; //gdi字体句柄，由于内部窗口如Edit还是用的gdi字体
// 	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIPen_gdiplus : public UIPen_gdi
	{
	public:
		UIPen_gdiplus();

	protected:
		virtual ~UIPen_gdiplus();
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBitmap_gdiplus : public UIBitmap_gdi
	{
	public:
		UIBitmap_gdiplus();

	protected:
		virtual ~UIBitmap_gdiplus();

	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIImage_gdiplus : public UIImage_gdi
	{
	public:
		UIImage_gdiplus();

	protected:
		virtual ~UIImage_gdiplus();
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBrush_gdiplus : public UIBrush_gdi
	{
	public:
		UIBrush_gdiplus();

	protected:
		virtual ~UIBrush_gdiplus();

	private:
		HBRUSH m_hBrush;
	};


} // namespace DuiLib

#endif // __UIRENDER_H__
