#include "StdAfx.h"

#include "../Render/UIRender_Cairo.h"

#ifdef DUILIB_GTK
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender_Cairo::UIRender_Cairo()
	{
		m_pManager	= NULL;
		m_cr = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	UIRender_Cairo::~UIRender_Cairo()
	{
		RestoreDefaultObject();

	}

	void UIRender_Cairo::Init(CPaintManagerUI* pManager, PVOID pParam)
	{
		m_pManager = pManager;
		m_cr = (cairo_t *)pParam;
	}

	void UIRender_Cairo::BeginPaint()
	{

	}

	void UIRender_Cairo::EndPaint()
	{

	}

	cairo_t *UIRender_Cairo::GetCairo()
	{
		return m_cr;
	}

	bool UIRender_Cairo::CloneFrom(UIRender *pSrcRender)
	{
		return false;
	}

	bool UIRender_Cairo::Resize(int width, int height)
	{
		if(GetWidth() == width && GetHeight() == height)
			return true;

		m_nWidth = width;
		m_nHeight = height;

// 		GdkWindow *gdkWindow = gtk_widget_get_window(GTK_WIDGET(GetManager()->GetPaintWindow()));
// 		if(!m_cr && gdkWindow)
// 		{
// 			m_cr = gdk_cairo_create(gdkWindow);
// 		}
		return true;
	}

	bool UIRender_Cairo::Resize(const RECT &rc)
	{
		return Resize(rc.right - rc.left, rc.bottom - rc.top);
	}

	UIBitmap *UIRender_Cairo::GetBitmap()
	{
		return NULL;
	}

	int UIRender_Cairo::GetWidth() const
	{ 
		return m_nWidth;
// 		if(!m_surface) return 0;
// 		return cairo_image_surface_get_width(m_surface);
	}

	int UIRender_Cairo::GetHeight() const
	{
		return m_nHeight;
// 		if(!m_surface) return 0;
// 		return cairo_image_surface_get_height(m_surface);
	}

	void UIRender_Cairo::Clear()
	{
// 		int width = cairo_image_surface_get_width(m_surface);
// 		int height = cairo_image_surface_get_height(m_surface);
// 		cairo_set_source_rgba(m_cairo, 0, 0, 0, 0);
// 		cairo_set_operator(m_cairo, CAIRO_OPERATOR_CLEAR);
// 		cairo_rectangle(m_cairo, 0, 0, width, height);
// 		cairo_paint_with_alpha(m_cairo, 1.0);
	}

//	void UIRender_Cairo::ClearAlpha(const RECT &rc)
//	{
// 		int width = GetWidth();
// 		int height = GetHeight();
// 		cairo_set_source_rgba(m_cairo, 1, 1, 1, 1);
// 		//cairo_set_operator(m_cairo, CAIRO_OPERATOR_CLEAR);
// 		cairo_rectangle(m_cairo, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
// 		//cairo_paint_with_alpha(m_cairo, 1.0);
// 		cairo_fill(m_cairo);
//	}

	void UIRender_Cairo::SaveDC()
	{
		cairo_save(m_cr);
	}

	void UIRender_Cairo::RestoreDC()
	{
		cairo_restore(m_cr);
	}

	CStdRefPtr<UIObject> UIRender_Cairo::SelectObject(UIObject *pObject)
	{	
		emUIOBJTYPE eType = pObject->ObjectType();
		if(eType == OT_FONT)
		{
			
		}
		else if(eType == OT_PEN)
		{
			
		}
		else if(eType == OT_BRUSH)
		{
			
		}
		else if(eType == OT_BITMAP)
		{
			
		}

		return CStdRefPtr<UIObject>();
	}

	void UIRender_Cairo::RestoreObject(UIObject *pObject)
	{
		
	}

	void UIRender_Cairo::RestoreDefaultObject()
	{
		
	}

	DWORD UIRender_Cairo::SetPixel(int x, int y, DWORD dwColor)
	{
		return 0;
	}

	BOOL UIRender_Cairo::BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop)
	{
		return AlphaBlend(x, y, nWidth, nHeight, pSrcRender, xSrc, ySrc, nWidth, nHeight, 255);
	}

	BOOL UIRender_Cairo::StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
	{
		return AlphaBlend(x, y, nWidth, nHeight, pSrcRender, xSrc, ySrc, nWidth, nHeight, 255);
	}

	BOOL UIRender_Cairo::AlphaBlend(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int alpha )
	{
		return TRUE;
// 		cairo_surface_t *base_surface = (cairo_surface_t *)GetBitmap()->GetHandle();
// 
// 		CStdRefPtr<UIBitmap> pSrcBmp = pSrcRender->GetBitmap();
// 		cairo_surface_t *src_surface = (cairo_surface_t *)pSrcRender->GetBitmap()->GetHandle();
// 
// 		cairo_save(m_cr);
// 		cairo_translate (m_cr, -xSrc, 0);
// 		cairo_set_source_surface (m_cr, src_surface, x, y);
// 		cairo_rectangle(m_cr, x+xSrc, y+ySrc, nWidthSrc, nHeightSrc);
// 		cairo_clip(m_cr);
// 		//cairo_paint(m_cr);
// 		cairo_paint_with_alpha(m_cr, ((double)alpha)/255);
// 		cairo_surface_flush(base_surface);
// 		cairo_restore(m_cr);
// 		cairo_surface_flush(base_surface);
		return TRUE;
	}

	void UIRender_Cairo::DrawBitmapAlpha(int x, int y, int nWidth, int nHeight, UIBitmap *pUiBitmap, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int alpha)
	{
		if(!pUiBitmap) return;

		//DUITRACE(_T("UIRender_Cairo::DrawBitmapAlpha alpha=%d"), alpha);
		
		if(pUiBitmap->ObjectType() == OT_BITMAP_PIXBUF)
		{
			cairo_save(m_cr);

			GdkPixbuf *src_pixbuf = (GdkPixbuf *)pUiBitmap->GetHandle();
			GdkPixbuf *sub_pixbuf = gdk_pixbuf_new_subpixbuf(src_pixbuf, xSrc, ySrc, nWidthSrc, nHeightSrc);
			if(!sub_pixbuf) return;
			GdkPixbuf *new_pixbuf = gdk_pixbuf_new(gdk_pixbuf_get_colorspace(src_pixbuf), 
				gdk_pixbuf_get_has_alpha(src_pixbuf), 8, nWidth, nHeight);
			if(!new_pixbuf)
			{
				g_object_unref(sub_pixbuf);
				return;
			}
			gdk_pixbuf_scale(sub_pixbuf, new_pixbuf, 0, 0, nWidth, nHeight, 0, 0, 
				(double)nWidth/nWidthSrc, (double)nHeight/nHeightSrc, GDK_INTERP_BILINEAR);

			gdk_cairo_set_source_pixbuf(m_cr, new_pixbuf, x, y);
			cairo_paint_with_alpha(m_cr, (double)alpha/255);
			cairo_restore(m_cr);

			g_object_unref(new_pixbuf);
			g_object_unref(sub_pixbuf);
		}
		else if(pUiBitmap->ObjectType() == OT_BITMAP_CAIRO)
		{
			cairo_surface_t *src_surface = (cairo_surface_t *)pUiBitmap->GetHandle();
			cairo_save(m_cr);
			cairo_translate (m_cr, -xSrc, 0);
			cairo_set_source_surface (m_cr, src_surface, x, y);
			cairo_rectangle(m_cr, x+xSrc, y+ySrc, nWidthSrc, nHeightSrc);
			cairo_clip(m_cr);
			cairo_paint_with_alpha(m_cr, ((double)alpha)/255);
			cairo_restore(m_cr);
		}
	}

	void UIRender_Cairo::DrawColor(const RECT& rc, const SIZE &round, DWORD dwColor)
	{
		cairo_set_source_rgba(m_cr, UIARGB_GetRValue(dwColor)/255.0, UIARGB_GetGValue(dwColor)/255.0,
			UIARGB_GetBValue(dwColor)/255.0, UIARGB_GetAValue(dwColor)/255.0);

		cairo_rectangle(m_cr, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);

 		cairo_fill(m_cr);
	}

	void UIRender_Cairo::DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps)
	{
		
	}

	void UIRender_Cairo::DrawLine(int x1, int y1, int x2, int y2, int nSize, DWORD dwPenColor,int nStyle)
	{
		cairo_set_source_rgb (m_cr, UIARGB_GetRValue(dwPenColor), UIARGB_GetGValue(dwPenColor), UIARGB_GetBValue(dwPenColor));
		cairo_move_to(m_cr, x1, y1);
		cairo_line_to(m_cr, x2, y2);
		cairo_set_line_width(m_cr, nSize + 1);
		cairo_stroke (m_cr);
	}

	void UIRender_Cairo::DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		cairo_set_source_rgb (m_cr, UIARGB_GetRValue(dwPenColor)/255.0, UIARGB_GetGValue(dwPenColor)/255.0, UIARGB_GetBValue(dwPenColor)/255.0);	
		cairo_set_line_width(m_cr, nSize+1);
		cairo_rectangle(m_cr, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
		cairo_stroke(m_cr);
	}

	void UIRender_Cairo::DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		
	}

	void UIRender_Cairo::DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle)
	{
		
	}

	void UIRender_Cairo::FillEllipse(const RECT& rc, DWORD dwColor)
	{
		
	}

	void UIRender_Cairo::DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwColor, int iFont, UINT uStyle)
	{
		if(pstrText == NULL || _tcslen(pstrText) == 0)
			return;

		UIFont *pFont = GetManager()->GetFont(iFont);
		PangoFontDescription *desc = (PangoFontDescription *)pFont->GetHandle();
		PangoLayout *layout = pango_cairo_create_layout (m_cr);

		cairo_save(m_cr);
		cairo_rectangle(m_cr, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
		cairo_clip(m_cr);

		cairo_set_source_rgba(m_cr, UIARGB_GetRValue(dwColor)/255.0, UIARGB_GetGValue(dwColor)/255.0,
			UIARGB_GetBValue(dwColor)/255.0, UIARGB_GetAValue(dwColor)/255.0);

		pango_layout_set_font_description(layout, desc);

		pango_layout_set_width(layout, (rc.right-rc.left) * PANGO_SCALE/*-1*/);
		pango_layout_set_height(layout, (rc.bottom-rc.top) * PANGO_SCALE/*-1*/);

		UISTRING_CONVERSION;
		pango_layout_set_text(layout, (LPCSTR)UIT2UTF8(pstrText), -1);

		int textWidth, textHeight;
		pango_layout_get_pixel_size(layout, &textWidth, &textHeight);

		if ((uStyle & DT_CENTER) == DT_CENTER)
		{
			pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		}
		else if ((uStyle & DT_RIGHT) == DT_RIGHT)
		{
			pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
		}
		else //if ((uStyle & DT_LEFT) == DT_LEFT)
		{
			pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
		}

		if ((uStyle & DT_END_ELLIPSIS) == DT_END_ELLIPSIS)
		{
			pango_layout_set_ellipsize(layout, PANGO_ELLIPSIZE_END);
		}

		if ((uStyle & DT_SINGLELINE) == DT_SINGLELINE)
		{
			pango_layout_set_single_paragraph_mode(layout, TRUE);
		}
		else
		{
			pango_layout_set_single_paragraph_mode(layout, FALSE);
		}

		int nFixY = 0;
		if ((uStyle & DT_BOTTOM) == DT_BOTTOM)
		{
			nFixY = rc.top + MAX(rc.bottom-rc.top - textHeight, 0);
		}
		else if ((uStyle & DT_VCENTER) == DT_VCENTER)
		{
			nFixY = rc.top + MAX((rc.bottom-rc.top - textHeight) / 2, 0);
		}
		else //if ((uStyle & DT_TOP) == DT_TOP)
		{
			nFixY = rc.top;
		}

		if((uStyle & DT_CALCRECT) == DT_CALCRECT)
		{	
			rc.left = rc.left;
			rc.top = nFixY;
			rc.right = rc.left + textWidth;
			rc.bottom = nFixY + textHeight;
		}
		else
		{
			cairo_move_to(m_cr, rc.left, nFixY);
			pango_cairo_update_layout(m_cr, layout);
			pango_cairo_show_layout(m_cr, layout);
		}

		g_object_unref(layout);
		cairo_restore(m_cr);

		/*
		cairo_set_source_rgba(m_cairo, UIARGB_GetRValue(dwColor)/255.0, UIARGB_GetGValue(dwColor)/255.0,
			UIARGB_GetBValue(dwColor)/255.0, UIARGB_GetAValue(dwColor)/255.0);

		UIFont *pFont = GetManager()->GetFont(iFont);
		cairo_set_font_face(m_cairo, (cairo_font_face_t *)pFont->GetHFONT(GetManager()));
		cairo_set_font_size (m_cairo, pFont->GetSize());

		UISTRING_CONVERSION;
		const char *pText = UIT2UTF8(pstrText);
		cairo_text_extents_t te;
		cairo_text_extents(m_cairo, pText, &te);

		cairo_move_to(m_cairo, rc.left, rc.bottom + (te.y_bearing + te.height / 2));
		cairo_show_text(m_cairo, pText);
		cairo_surface_flush(m_surface);
		*/
	}

	UIPath* UIRender_Cairo::CreatePath()
	{
		return new UIPath_Cairo(NULL);
	}

	BOOL UIRender_Cairo::DrawPath(const UIPath* path, int nSize, DWORD dwColor)
	{
		return FALSE;
	}

	BOOL UIRender_Cairo::FillPath(const UIPath* path, const DWORD dwColor)
	{
		return FALSE;
	}

	SIZE UIRender_Cairo::GetTextSize(LPCTSTR pstrText, int iFont, UINT uStyle )
	{	
		SIZE sz = { 0 };
		return sz;
	}

} // namespace DuiLib
#endif //#ifdef DUILIB_USE_RENDER_CAIRO
