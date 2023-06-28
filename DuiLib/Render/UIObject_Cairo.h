#ifndef __UIOBJECT_CAIRO_H__
#define __UIOBJECT_CAIRO_H__
#pragma once

#include "IRender.h"

#ifdef DUILIB_USE_RENDER_CAIRO
namespace DuiLib { 
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIFont_pango : public UIFont
	{
	public:
		UIFont_pango();

		virtual BOOL CreateDefaultFont() override;

		virtual void DeleteObject() override;

		virtual HANDLE  GetHandle() override;
#ifdef WIN32
		virtual HFONT GetHFONT(CPaintManagerUI *pManager=NULL) override;
#endif

		virtual UIFont* Clone(CPaintManagerUI *pManager) override;

		virtual int GetHeight(CPaintManagerUI *pManager) override;

	protected:
		virtual ~UIFont_pango();

		virtual BOOL _buildFont(CPaintManagerUI *pManager=NULL) override;
	protected:
		//cairo_font_face_t *m_hFont;
		PangoFontDescription * m_pPangoFontDesc;
#ifdef WIN32
		HFONT m_hFont;
#endif
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIPen_Cairo : public UIPen
	{
	public:
		UIPen_Cairo();

		virtual void DeleteObject() override;

#ifdef WIN32
		virtual HPEN GetHPEN() const override;
#endif

		virtual BOOL CreatePen(int nStyle, int nWidth, DWORD dwColor) override;

	protected:
		virtual ~UIPen_Cairo();
	private:
#ifdef WIN32
		HPEN  m_hPen;
#endif
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBitmap_Cairo : public UIBitmap
	{
	public:
		UIBitmap_Cairo();

		const emUIOBJTYPE ObjectType() const override {  return OT_BITMAP_CAIRO;  }

		virtual void DeleteObject() override;

#ifdef WIN32
		virtual BOOL CreateFromHBitmap(HBITMAP hBitmap) override; //hBitmap由内部释放
		virtual HBITMAP GetHBITMAP() override;
		virtual BOOL CreateARGB32Bitmap(HDC hDC, int width, int height, BOOL bFlip) override;
		virtual BOOL CreateCompatibleBitmap(HDC hDC, int width, int height) override;
#endif

		virtual BOOL CreateFromData(LPBYTE pImage, int width, int height, DWORD mask) override;

		virtual HANDLE  GetHandle()	override;
		virtual BYTE* GetBits() override;
		virtual int	GetWidth() override;
		virtual int GetHeight() override;
		virtual BOOL IsAlpha() override;

		virtual UIBitmap *Clone() override;

		virtual void Clear() override;
		virtual void ClearAlpha(const RECT &rc, int alpha = 0) override;

		virtual BOOL SaveFile(LPCTSTR pstrFileName) override;
	protected:
		virtual ~UIBitmap_Cairo();
	protected:
		cairo_surface_t *m_surface;
		BYTE	*m_pDataBits;
		BOOL	m_bAlphaChannel;
	};

	class UILIB_API UIBitmap_Pixbuf : public UIBitmap
	{
	public:
		UIBitmap_Pixbuf();

		const emUIOBJTYPE ObjectType() const override {  return OT_BITMAP_PIXBUF;  }

		virtual void DeleteObject() override;

#ifdef WIN32
		virtual BOOL CreateFromHBitmap(HBITMAP hBitmap) override; //hBitmap由内部释放
		virtual HBITMAP GetHBITMAP() override;
		virtual BOOL CreateARGB32Bitmap(HDC hDC, int width, int height, BOOL bFlip) override;
		virtual BOOL CreateCompatibleBitmap(HDC hDC, int width, int height) override;
#endif

		virtual BOOL CreateFromData(LPBYTE pImage, int width, int height, DWORD mask) override;

		virtual HANDLE  GetHandle()	override;
		virtual BYTE* GetBits() override;
		virtual int	GetWidth() override;
		virtual int GetHeight() override;
		virtual BOOL IsAlpha() override;

		virtual UIBitmap *Clone() override;

		virtual void Clear() override;
		virtual void ClearAlpha(const RECT &rc, int alpha = 0) override;

		virtual BOOL SaveFile(LPCTSTR pstrFileName) override;
	protected:
		virtual ~UIBitmap_Pixbuf();
	protected:
		GdkPixbuf *m_pixbuf;
		BYTE	*m_pDataBits;
		BOOL	m_bAlphaChannel;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIImage_Cairo : public UIImage
	{
	public:
		UIImage_Cairo();

		virtual void DeleteObject() override;

#ifdef WIN32
		virtual BOOL CreateImage(HBITMAP hBitmap, bool bAlpha) override;
#endif

	protected:
		virtual ~UIImage_Cairo();
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBrush_Cairo : public UIBrush
	{
	public:
		UIBrush_Cairo();

		virtual void DeleteObject() override;

#ifdef WIN32
		virtual HBRUSH GetHBrush() const override;
		virtual BOOL CreateFromHBrush(HBRUSH hBrush) override;
#endif

		virtual BOOL CreateSolidBrush(DWORD clr) override;
		virtual BOOL CreateBitmapBrush(UIBitmap *bitmap) override;

	protected:
		virtual ~UIBrush_Cairo();

	private:
#ifdef WIN32
		HBRUSH m_hBrush;
#endif
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIPath_Cairo  : public UIPath
	{
	public:
		UIPath_Cairo();
		UIPath_Cairo(cairo_t *cr);

		virtual void DeleteObject() override;

		virtual BOOL Beginpath() override;
		virtual BOOL EndPath() override;
		virtual BOOL AbortPath() override;

		virtual BOOL AddLine(int x1, int y1, int x2, int y2) override;
		virtual BOOL AddLines(CDuiPoint *points, int count) override;

// 		virtual BOOL AddArc(int x, int y, int width, int height, double angel1, double angel2) override;
// 
// 		virtual BOOL AddRect(const RECT& rc, const SIZE &round) override;
	protected:
		virtual ~UIPath_Cairo();

	protected:
		cairo_t *m_cr;
		cairo_path_t *m_path;
		CDuiPoint m_curPoint;
	};

} // namespace DuiLib
#endif //#ifdef DUILIB_USE_RENDER_CAIRO

#endif // __UIOBJECT_CAIRO_H__
