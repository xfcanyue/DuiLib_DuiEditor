#include "StdAfx.h"

#include "../Render/UIObject_Cairo.h"

#ifdef DUILIB_USE_RENDER_CAIRO

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIFont_pango::UIFont_pango()
	{
		m_pPangoFontDesc = NULL; 
#ifdef WIN32
		m_hFont = NULL;
#endif
	}

	UIFont_pango::~UIFont_pango()
	{
		if (m_pPangoFontDesc)
		{
			//cairo_font_face_destroy(m_hFont);
			pango_font_description_free(m_pPangoFontDesc);
			m_pPangoFontDesc = NULL;
		}

#ifdef WIN32
		if(m_hFont) { ::DeleteObject(m_hFont); m_hFont = NULL; }
#endif
	}

	void UIFont_pango::DeleteObject()
	{
		if (m_pPangoFontDesc)
		{
			//cairo_font_face_destroy(m_hFont);
			pango_font_description_free(m_pPangoFontDesc);
			m_pPangoFontDesc = NULL;
		}

#ifdef WIN32
		if(m_hFont) { ::DeleteObject(m_hFont); m_hFont = NULL; }
#endif
	}

	BOOL UIFont_pango::CreateDefaultFont()
	{
		sFontName = _T("Sans");
		iSize = 14;
		bBold = FALSE;
		bUnderline = FALSE;
		bItalic = FALSE;
		return _buildFont(NULL);
	}

	HANDLE UIFont_pango::GetHandle()
	{
		return m_pPangoFontDesc;
	}

#ifdef WIN32
	HFONT UIFont_pango::GetHFONT(CPaintManagerUI *pManager)
	{
		//为啥不行
// 		PangoFontMap *fontmap;
// 		PangoFontDescription *desc;
// 		PangoFont *font;
// 		fontmap = pango_cairo_font_map_get_default ();
// 		font = pango_font_map_load_font (fontmap, m_pPangoFontDesc);
// 		LOGFONTA pango_win32_font_logfont(font);

		if(m_hFont)
			return m_hFont;

		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		if(sFontName.GetLength() > 0) 
		{
			_tcsncpy(lf.lfFaceName, sFontName, LF_FACESIZE);
		}
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfHeight = pManager ? -pManager->GetDPIObj()->ScaleInt(iSize) : iSize;
		if( bBold ) lf.lfWeight = FW_BOLD;
		if( bUnderline ) lf.lfUnderline = TRUE;
		if( bItalic ) lf.lfItalic = TRUE;

		m_hFont = ::CreateFontIndirect(&lf);
		return m_hFont;
	}
#endif

	UIFont* UIFont_pango::Clone(CPaintManagerUI *pManager)
	{
		UIFont_pango *pNewFont	= new UIFont_pango;
		pNewFont->id			= id;
		pNewFont->sFontName		= sFontName;
		pNewFont->iSize			= iSize;
		pNewFont->bBold			= bBold;
		pNewFont->bUnderline	= bUnderline;
		pNewFont->bItalic		= bItalic;
		pNewFont->bDefault		= bDefault;
		pNewFont->bShared		= bShared;
		//pNewFont->m_hFont		= cairo_font_face_reference(m_hFont);
		pNewFont->m_pPangoFontDesc		= pango_font_description_copy(m_pPangoFontDesc);
		ASSERT(pNewFont);
		return pNewFont;
	}

	int UIFont_pango::GetHeight(CPaintManagerUI *pManager)
	{
#ifdef DUILIB_WIN32
		TEXTMETRIC tm;
		HFONT hOldFont = (HFONT)::SelectObject(pManager->GetPaintDC(), GetHFONT(pManager));
		::GetTextMetrics(pManager->GetPaintDC(), &tm);
		::SelectObject(pManager->GetPaintDC(), hOldFont);
		return tm.tmHeight;
#endif
		if (m_pPangoFontDesc)
		{
			return pango_font_description_get_size(m_pPangoFontDesc) / PANGO_SCALE;
		}
		return 0;
	}

	BOOL UIFont_pango::_buildFont(CPaintManagerUI *pManager)
	{
		if(sFontName.IsEmpty())
			return FALSE;

 		UISTRING_CONVERSION;
// 		m_hFont = cairo_toy_font_face_create ((LPCSTR)UIA2UTF8(sFontName), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

		PangoFontDescription *desc = pango_font_description_new();

		if (!sFontName.IsEmpty())
		{
			pango_font_description_set_family(desc, (LPCSTR)UIT2UTF8(sFontName));
		}

		//pango_font_description_set_size(desc, iSize*PANGO_SCALE);
		pango_font_description_set_absolute_size(desc, iSize*PANGO_SCALE);

		if (bItalic)
		{
			pango_font_description_set_style(desc, PANGO_STYLE_ITALIC);
		}
		else
		{
			pango_font_description_set_style(desc, PANGO_STYLE_NORMAL);
		}

		if (bBold)
		{
			pango_font_description_set_weight(desc, PANGO_WEIGHT_BOLD);
		}
		else
		{
			pango_font_description_set_weight(desc, PANGO_WEIGHT_NORMAL);
		}

		m_pPangoFontDesc = desc;
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPen_Cairo::UIPen_Cairo()
	{
#ifdef WIN32
		m_hPen = NULL;
#endif
	}

	UIPen_Cairo::~UIPen_Cairo() 
	{
		
	}

	void UIPen_Cairo::DeleteObject()
	{
		
	}

#ifdef WIN32
	HPEN UIPen_Cairo::GetHPEN() const
	{
		return m_hPen;
	}
#endif

	BOOL UIPen_Cairo::CreatePen(int nStyle, int nWidth, DWORD dwColor)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBrush_Cairo::UIBrush_Cairo() 
	{
#ifdef WIN32
		m_hBrush = NULL;
#endif
	}

	UIBrush_Cairo::~UIBrush_Cairo() 
	{
		
	}

	void UIBrush_Cairo::DeleteObject()
	{
		
	}

#ifdef WIN32
	HBRUSH UIBrush_Cairo::GetHBrush() const
	{
		return m_hBrush;
	}

	BOOL UIBrush_Cairo::CreateFromHBrush(HBRUSH hBrush)
	{	
		return TRUE;
	}
#endif

	BOOL UIBrush_Cairo::CreateSolidBrush(DWORD clr)
	{
#ifdef WIN32
		return m_hBrush != NULL;
#endif
        return FALSE;
	}

	BOOL UIBrush_Cairo::CreateBitmapBrush(UIBitmap *bitmap)
	{
#ifdef WIN32
		return m_hBrush != NULL;
#endif
        return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPath_Cairo::UIPath_Cairo()
	{
		m_cr = NULL;
		m_path = NULL;
	}

	UIPath_Cairo::UIPath_Cairo(cairo_t *cr)
	{
		m_cr = cr;
		m_path = NULL;
	}

	UIPath_Cairo::~UIPath_Cairo()
	{
		if(m_path)
		{
			cairo_path_destroy(m_path); m_path = NULL;
		}
	}

	void UIPath_Cairo::DeleteObject()
	{
		if(m_path)
		{
			cairo_path_destroy(m_path); m_path = NULL;
		}
	}

	BOOL UIPath_Cairo::Beginpath()
	{
		cairo_new_path(m_cr);
		return TRUE;
	}

	BOOL UIPath_Cairo::EndPath()
	{
		cairo_close_path(m_cr);
		return TRUE;
	}

	BOOL UIPath_Cairo::AbortPath()
	{
		return TRUE;
	}

	BOOL UIPath_Cairo::AddLine(int x1, int y1, int x2, int y2)
	{
		if(m_curPoint.x != x1 && m_curPoint.y != y1)
		{
			cairo_move_to(m_cr, x1, y1);
		}
		cairo_line_to(m_cr, x2, y2);
		m_curPoint.x = x2;
		m_curPoint.y = y2;
		return TRUE;
	}

	BOOL UIPath_Cairo::AddLines(CDuiPoint *points, int count)
	{
		cairo_move_to(m_cr, points[0].x, points[0].y);
		for (int i=1; i<count; i++)
		{
			cairo_line_to(m_cr, points[i].x, points[i].y);
		}
		cairo_line_to(m_cr, points[0].x, points[0].y);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBitmap_Cairo::UIBitmap_Cairo()
	{
		m_surface = NULL;
		m_pDataBits = NULL;
		m_bAlphaChannel = FALSE;
	}

	UIBitmap_Cairo::~UIBitmap_Cairo()
	{
		if(m_surface) { cairo_surface_destroy(m_surface); m_surface = NULL; }
		if(m_pDataBits) { free(m_pDataBits); m_pDataBits = NULL; }
	}

	void UIBitmap_Cairo::DeleteObject()
	{
		if(m_surface) { cairo_surface_destroy(m_surface); m_surface = NULL; }
		if(m_pDataBits) { free(m_pDataBits); m_pDataBits = NULL; }
	}

#ifdef WIN32
	BOOL UIBitmap_Cairo::CreateFromHBitmap(HBITMAP hBitmap) //hBitmap由内部释放
	{
		DeleteObject();
		m_surface = cairo_surface_reference((cairo_surface_t *)hBitmap);
		return FALSE;
	}

	HBITMAP UIBitmap_Cairo::GetHBITMAP()
	{
		return (HBITMAP)m_surface;
	}

	BOOL UIBitmap_Cairo::CreateARGB32Bitmap(HDC hDC, int width, int height, BOOL bFlip)
	{
		DeleteObject();
		m_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		//m_surface = cairo_image_surface_create_from_png ("c:/tomcat.png");
		return m_surface != NULL;
	}

	BOOL UIBitmap_Cairo::CreateCompatibleBitmap(HDC hDC, int width, int height)
	{
		DeleteObject();
		m_surface = cairo_win32_surface_create(hDC);
		return m_surface != NULL;
	}
#endif

	BOOL UIBitmap_Cairo::CreateFromData(LPBYTE pImage, int width, int height, DWORD mask)
	{
		DeleteObject();

		//每行图像有几个字节
		int tride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32,width);

		//Creates an image surface for the provided pixel data. The output buffer must be kept around until the #cairo_surface_t is destroyed or cairo_surface_finish() is called on the surface. The initial contents of data will be used as the initial image contents; you must explicitly clear the buffer, using, for example, cairo_rectangle() and cairo_fill() if you want it cleared.
		//cairo不会备份pImage，所以自己存起来。
		m_pDataBits = (BYTE *)malloc(tride*height);
		memcpy(m_pDataBits, pImage, tride*height);
		m_surface = cairo_image_surface_create_for_data(m_pDataBits, CAIRO_FORMAT_ARGB32, width, height, tride);
		if(m_surface == NULL) return FALSE;
		if(cairo_surface_status(m_surface) != CAIRO_STATUS_SUCCESS)
			return FALSE;

		//stbi的图像
		//pImage[0], R
		//pImage[1], G
		//pImage[2], B
		//pImage[3], alpha

		//cairo的图像 B  G  R
		//pDest[0], B
		//pDest[1], G
		//pDest[2], R
		//pDest[3], alpha

		//RGB 转 BGR
		BYTE *pDest = GetBits();
		m_bAlphaChannel = FALSE;
		for( int i = 0; i < width * height; i++ ) 
		{
			if( pDest[i*4 + 3] < 255 )
			{
				BYTE tp = pDest[i*4];
				pDest[i*4]		= pDest[i*4 + 2] * pDest[i*4 + 3] / 255;
				pDest[i*4 + 1] = (BYTE)(DWORD(pDest[i*4 + 1])*pDest[i*4 + 3]/255);
				pDest[i*4 + 2]	= tp * pDest[i*4 + 3] / 255;
				m_bAlphaChannel = TRUE;
			}
			else
			{ 
				BYTE tp = pDest[i*4];
				pDest[i*4]		= pDest[i*4 + 2];
				pDest[i*4 + 2]	= tp;
			}

			if( *(DWORD*)(&pDest[i*4]) == mask ) {
				pDest[i*4] = (BYTE)0;
				pDest[i*4 + 1] = (BYTE)0;
				pDest[i*4 + 2] = (BYTE)0; 
				pDest[i*4 + 3] = (BYTE)0;
				m_bAlphaChannel = TRUE;
			}
		}
		return TRUE;
	}

	HANDLE UIBitmap_Cairo::GetHandle()
	{
		return m_surface;
	}

	BYTE* UIBitmap_Cairo::GetBits()
	{
		return cairo_image_surface_get_data(m_surface);
	}

	int	UIBitmap_Cairo::GetWidth()
	{
		if(!m_surface) return 0;
		return cairo_image_surface_get_width(m_surface);
	}

	int UIBitmap_Cairo::GetHeight()
	{
		if(!m_surface) return 0;
		return cairo_image_surface_get_height(m_surface);
	}

	BOOL UIBitmap_Cairo::IsAlpha()
	{
		return m_bAlphaChannel;
	}

	UIBitmap *UIBitmap_Cairo::Clone()
	{
		UIBitmap_Cairo *bmp = (UIBitmap_Cairo *)UIGlobal::CreateBitmap();
		bmp->m_surface = cairo_surface_reference(m_surface);
		return bmp;
	}

	void UIBitmap_Cairo::Clear()
	{
		
	}

	void UIBitmap_Cairo::ClearAlpha(const RECT &rc, int alpha)
	{
		
	}

	BOOL UIBitmap_Cairo::SaveFile(LPCTSTR pstrFileName)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	UIBitmap_Pixbuf::UIBitmap_Pixbuf()
	{
		m_pixbuf = NULL;
		m_pDataBits = NULL;
		m_bAlphaChannel = FALSE;
	}

	UIBitmap_Pixbuf::~UIBitmap_Pixbuf()
	{
		if(m_pixbuf) { g_object_unref(m_pixbuf); m_pixbuf = NULL; }
		if(m_pDataBits) { free(m_pDataBits); m_pDataBits = NULL; }
	}

	void UIBitmap_Pixbuf::DeleteObject()
	{
		if(m_pixbuf) { g_object_unref(m_pixbuf); m_pixbuf = NULL; }
		if(m_pDataBits) { free(m_pDataBits); m_pDataBits = NULL; }
	}

#ifdef WIN32
	BOOL UIBitmap_Pixbuf::CreateFromHBitmap(HBITMAP hBitmap) //hBitmap由内部释放
	{
		return FALSE;
	}

	HBITMAP UIBitmap_Pixbuf::GetHBITMAP()
	{
		return (HBITMAP)m_pixbuf;
	}

	BOOL UIBitmap_Pixbuf::CreateARGB32Bitmap(HDC hDC, int width, int height, BOOL bFlip)
	{
		DeleteObject();
		m_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 32, width, height);
		return m_pixbuf != NULL;
	}

	BOOL UIBitmap_Pixbuf::CreateCompatibleBitmap(HDC hDC, int width, int height)
	{
		DeleteObject();
		return FALSE;
	}
#endif

	BOOL UIBitmap_Pixbuf::CreateFromData(LPBYTE pImage, int width, int height, DWORD mask)
	{
		DeleteObject();

		//每行图像有几个字节
		int tride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32,width);

		m_pDataBits = (BYTE *)malloc(tride*height);
		memcpy(m_pDataBits, pImage, tride*height);

		m_pixbuf = gdk_pixbuf_new_from_data(m_pDataBits, GDK_COLORSPACE_RGB, true, 8, width, height, tride, NULL, NULL);

		return m_pixbuf != NULL;
	}

	HANDLE UIBitmap_Pixbuf::GetHandle()
	{
		return m_pixbuf;
	}

	BYTE* UIBitmap_Pixbuf::GetBits()
	{
		return m_pixbuf ? gdk_pixbuf_get_pixels(m_pixbuf) : NULL;
	}

	int	UIBitmap_Pixbuf::GetWidth()
	{
		return m_pixbuf ? gdk_pixbuf_get_width(m_pixbuf) : 0;
	}

	int UIBitmap_Pixbuf::GetHeight()
	{
		return m_pixbuf ? gdk_pixbuf_get_height(m_pixbuf) : 0;
	}

	BOOL UIBitmap_Pixbuf::IsAlpha()
	{
		return m_bAlphaChannel;
	}

	UIBitmap *UIBitmap_Pixbuf::Clone()
	{
		//????
		return NULL;
	}

	void UIBitmap_Pixbuf::Clear()
	{

	}

	void UIBitmap_Pixbuf::ClearAlpha(const RECT &rc, int alpha)
	{

	}

	BOOL UIBitmap_Pixbuf::SaveFile(LPCTSTR pstrFileName)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIImage_Cairo::UIImage_Cairo()
	{
		bitmap = UIGlobal::CreateBitmap();
		pBits		= nullptr;
		pSrcBits	= nullptr;;
		nWidth			= 0;
		nHeight			= 0;
		bAlpha		= false;
		bUseHSL		= false;
		dwMask		= 0;
		delay		= 0;
	}

	UIImage_Cairo::~UIImage_Cairo()
	{
		//pBits是从::CreateDIBSection搞过来的，不能delete
		//pSrcBits 是HSL色彩转换时分配的，需要delete
		if (pSrcBits) { delete[] pSrcBits; pSrcBits = NULL; }	
		if(bitmap) { bitmap->Release(); };
	}

	void UIImage_Cairo::DeleteObject()
	{
		bitmap->DeleteObject();
		if (pSrcBits) { delete[] pSrcBits; pSrcBits = NULL; }
	}

#ifdef WIN32
	BOOL UIImage_Cairo::CreateImage(HBITMAP hBitmap, bool bAlpha)
	{
		return FALSE;
	}
#endif

} // namespace DuiLib
#endif //#ifdef DUILIB_USE_RENDER_CAIRO

