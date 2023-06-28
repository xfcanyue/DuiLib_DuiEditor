#ifndef __UIFONT_GDIPLUS_H__
#define __UIFONT_GDIPLUS_H__

#pragma once
#include "IRender.h"
#include "UIObject_gdi.h"

#ifdef DUILIB_WIN32
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

	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIPath_gdiplus  : public UIPath
	{
	public:
		UIPath_gdiplus();

		virtual void DeleteObject() override;

		virtual BOOL Beginpath() override;
		virtual BOOL EndPath() override;
		virtual BOOL AbortPath() override;

		virtual BOOL AddLine(int x1, int y1, int x2, int y2) override;
		virtual BOOL AddLines(CDuiPoint *points, int count) override;

		Gdiplus::GraphicsPath* Getpath();

	protected:
		virtual ~UIPath_gdiplus();

	protected:
		Gdiplus::GraphicsPath *m_pPath;
	};


} // namespace DuiLib

#endif // #ifdef DUILIB_WIN32
#endif // __UIRENDER_H__
