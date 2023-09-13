#include "StdAfx.h"
#include "UIRender_gdi.h"

#ifdef DUILIB_WIN32
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	static COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
	{
		return RGB (GetRValue (clrSrc) * src_darken + GetRValue (clrDest) * dest_darken, 
			GetGValue (clrSrc) * src_darken + GetGValue (clrDest) * dest_darken, 
			GetBValue (clrSrc) * src_darken + GetBValue (clrDest) * dest_darken);
	}

	static BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, \
		int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn)
	{
		HDC hTempDC = ::CreateCompatibleDC(hDC);
		if (NULL == hTempDC)
			return FALSE;

		//Creates Source DIB
		LPBITMAPINFO lpbiSrc = NULL;
		// Fill in the BITMAPINFOHEADER
		lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiSrc == NULL)
		{
			::DeleteDC(hTempDC);
			return FALSE;
		}
		lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiSrc->bmiHeader.biWidth = dwWidth;
		lpbiSrc->bmiHeader.biHeight = dwHeight;
		lpbiSrc->bmiHeader.biPlanes = 1;
		lpbiSrc->bmiHeader.biBitCount = 32;
		lpbiSrc->bmiHeader.biCompression = BI_RGB;
		lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
		lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
		lpbiSrc->bmiHeader.biClrUsed = 0;
		lpbiSrc->bmiHeader.biClrImportant = 0;

		COLORREF* pSrcBits = NULL;
		HBITMAP hSrcDib = CreateDIBSection (
			hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
			NULL, NULL);

		if ((NULL == hSrcDib) || (NULL == pSrcBits)) 
		{
			delete [] lpbiSrc;
			::DeleteDC(hTempDC);
			return FALSE;
		}

		HBITMAP hOldTempBmp = (HBITMAP)::SelectObject (hTempDC, hSrcDib);
		::StretchBlt(hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY);
		::SelectObject (hTempDC, hOldTempBmp);

		//Creates Destination DIB
		LPBITMAPINFO lpbiDest = NULL;
		// Fill in the BITMAPINFOHEADER
		lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
		if (lpbiDest == NULL)
		{
			delete [] lpbiSrc;
			::DeleteObject(hSrcDib);
			::DeleteDC(hTempDC);
			return FALSE;
		}

		lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiDest->bmiHeader.biWidth = dwWidth;
		lpbiDest->bmiHeader.biHeight = dwHeight;
		lpbiDest->bmiHeader.biPlanes = 1;
		lpbiDest->bmiHeader.biBitCount = 32;
		lpbiDest->bmiHeader.biCompression = BI_RGB;
		lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
		lpbiDest->bmiHeader.biXPelsPerMeter = 0;
		lpbiDest->bmiHeader.biYPelsPerMeter = 0;
		lpbiDest->bmiHeader.biClrUsed = 0;
		lpbiDest->bmiHeader.biClrImportant = 0;

		COLORREF* pDestBits = NULL;
		HBITMAP hDestDib = CreateDIBSection (
			hDC, lpbiDest, DIB_RGB_COLORS, (void **)&pDestBits,
			NULL, NULL);

		if ((NULL == hDestDib) || (NULL == pDestBits))
		{
			delete [] lpbiSrc;
			::DeleteObject(hSrcDib);
			::DeleteDC(hTempDC);
			return FALSE;
		}

		::SelectObject (hTempDC, hDestDib);
		::BitBlt (hTempDC, 0, 0, dwWidth, dwHeight, hDC, nDestX, nDestY, SRCCOPY);
		::SelectObject (hTempDC, hOldTempBmp);

		double src_darken;
		BYTE nAlpha;

		for (int pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++)
		{
			nAlpha = LOBYTE(*pSrcBits >> 24);
			src_darken = (double) (nAlpha * ftn.SourceConstantAlpha) / 255.0 / 255.0;
			if( src_darken < 0.0 ) src_darken = 0.0;
			*pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, 1.0 - src_darken);
		} //for

		::SelectObject (hTempDC, hDestDib);
		::BitBlt (hDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);
		::SelectObject (hTempDC, hOldTempBmp);

		delete [] lpbiDest;
		::DeleteObject(hDestDib);

		delete [] lpbiSrc;
		::DeleteObject(hSrcDib);

		::DeleteDC(hTempDC);
		return TRUE;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender_gdi::UIRender_gdi()
	{
		m_pManager	= NULL;
		m_iSaveDC = 0;
		m_hDC = NULL;
		m_bAttachHDC = FALSE;
	}

	UIRender_gdi::~UIRender_gdi()
	{
		RestoreDefaultObject();
		if(!m_bAttachHDC && m_hDC != NULL) { ::DeleteDC(m_hDC); m_hDC = NULL; }
	}

	void UIRender_gdi::Init(CPaintManagerUI *pManager, HDC hDC)
	{
		if(!m_bAttachHDC && m_hDC != NULL) { ::DeleteDC(m_hDC); m_hDC = NULL; }
		m_pManager = pManager;
		m_bAttachHDC = FALSE;

		if(hDC == NULL)
		{
			hDC = ::GetDC(NULL);
			m_hDC = ::CreateCompatibleDC(hDC);
			::ReleaseDC(NULL, hDC);
		}
		else
		{
			m_hDC = ::CreateCompatibleDC(hDC);
		}

		::SetBkMode(hDC, TRANSPARENT);

		m_defBmp = MakeRefPtr<UIBitmap>(UIGlobal::CreateBitmap());
		m_defBmp->CreateARGB32Bitmap(m_hDC, 1, 1, TRUE);

		m_defBrush = MakeRefPtr<UIBrush>(UIGlobal::CreateBrush());
		m_defBrush->CreateFromHBrush((HBRUSH)::GetStockObject(NULL_BRUSH));

		m_defFont = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
		m_defFont->CreateDefaultFont();

		m_defPen = MakeRefPtr<UIPen>(UIGlobal::CreatePen());
		m_defPen->CreatePen(PS_SOLID, 1, RGB(0,0,0));

		SelectObject(m_defBmp);
		SelectObject(m_defBrush);
		SelectObject(m_defFont);
		SelectObject(m_defPen);	
	}

	void UIRender_gdi::AttachDC(CPaintManagerUI *pManager, HDC hDC)
	{
		m_bAttachHDC = TRUE;
		m_pManager = pManager;
		m_hDC = hDC;
	}

	HDC UIRender_gdi::GetDC()
	{
		return m_hDC;
	}


	bool UIRender_gdi::CloneFrom(UIRender *pSrcRender)
	{
		m_pManager = pSrcRender->GetManager();
		m_hDC = ::CreateCompatibleDC(pSrcRender->GetDC());

		m_defBmp = MakeRefPtr<UIBitmap>(UIGlobal::CreateBitmap());
		m_defBmp->CreateARGB32Bitmap(m_hDC, 1, 1, TRUE);

		m_defBrush = MakeRefPtr<UIBrush>(UIGlobal::CreateBrush());
		m_defBrush->CreateFromHBrush((HBRUSH)::GetStockObject(NULL_BRUSH));

		m_defFont = MakeRefPtr<UIFont>(UIGlobal::CreateFont());
		m_defFont->CreateDefaultFont();

		m_defPen = MakeRefPtr<UIPen>(UIGlobal::CreatePen());
		m_defPen->CreatePen(PS_SOLID, 1, RGB(0,0,0));

		SelectObject(m_defBmp);
		SelectObject(m_defBrush);
		SelectObject(m_defFont);
		SelectObject(m_defPen);	
		return m_hDC != NULL;
	}

	UIBitmap *UIRender_gdi::GetBitmap()
	{
		return *(UIBitmap **)&m_curBmp;
	}

	bool UIRender_gdi::Resize(int width, int height)
	{
		if(!m_curBmp) 
			return false;

		if(m_curBmp->GetWidth() == width && m_curBmp->GetHeight() == height)
			return false;

		m_curBmp->DeleteObject();
		if(!m_curBmp->CreateARGB32Bitmap(m_hDC, width, height, TRUE))
			return false;

		SelectObject(m_curBmp);
		return true;
	}

	bool UIRender_gdi::Resize(const RECT &rc)
	{
		return Resize(rc.right - rc.left, rc.bottom - rc.top);
	}

	int UIRender_gdi::GetWidth() const
	{
		if(!m_curBmp) return 0;
		return m_curBmp->GetWidth();
	}

	int UIRender_gdi::GetHeight() const
	{
		if(!m_curBmp) return 0;
		return m_curBmp->GetHeight();
	}

	void UIRender_gdi::Clear()
	{
		if(!m_curBmp) 
			return;
		m_curBmp->Clear();
	}

	void UIRender_gdi::ClearAlpha(const RECT &rc, int alpha)
	{
		if(!m_curBmp) 
			return;
		m_curBmp->ClearAlpha(rc, alpha);
	}

	void UIRender_gdi::SaveDC()
	{
		m_iSaveDC = ::SaveDC(m_hDC);
	}

	void UIRender_gdi::RestoreDC()
	{
		::RestoreDC(m_hDC, m_iSaveDC);
	}

	CStdRefPtr<UIObject> UIRender_gdi::SelectObject(UIObject *pObject)
	{
		emUIOBJTYPE eType = pObject->ObjectType();
		if(eType == OT_FONT)
		{
			::SelectObject(m_hDC, ((UIFont *)pObject)->GetHFONT(GetManager()));
			m_lastFont = m_curFont ? m_curFont : m_defFont;
			m_curFont = (UIFont_gdi *)pObject;
			return *(UIFont**)&m_lastFont;
		}
		else if(eType == OT_PEN)
		{
			::SelectObject(m_hDC, ((UIPen *)pObject)->GetHPEN());
			m_lastPen = m_curPen ? m_curPen : m_defPen;
			m_curPen = (UIPen_gdi *)pObject;
			return *(UIPen**)&m_lastPen;
		}
		else if(eType == OT_BRUSH)
		{
			::SelectObject(m_hDC, ((UIBrush *)pObject)->GetHBrush());
			m_lastBrush = m_curBrush ? m_curBrush : m_defBrush;
			m_curBrush = (UIBrush_gdi *)pObject;
			return *(UIBrush**)&m_lastBrush;
		}
		else if(eType == OT_BITMAP)
		{
			::SelectObject(m_hDC, ((UIBitmap *)pObject)->GetHBITMAP());
			m_lastBmp = m_curBmp ? m_curBmp : m_defBmp;
			m_curBmp = (UIBitmap_gdi *)pObject;
			return *(UIBitmap**)&m_lastBmp;
		}

		return NULL;
	}

	void UIRender_gdi::RestoreObject(UIObject *pObject)
	{
		if(pObject == NULL)
		{
			m_lastFont	= m_lastFont	? m_lastFont	: m_defFont;
			m_lastPen	= m_lastPen		? m_lastPen		: m_defPen;
			m_lastBrush = m_lastBrush	? m_lastBrush	: m_defBrush;
			m_lastBmp	= m_lastBmp		? m_lastBmp		: m_defBmp;

			if(m_curFont != m_lastFont)
			{
				m_curFont = m_lastFont;
				::SelectObject(m_hDC, m_curFont->GetHFONT(GetManager()));
			}

			if(m_curPen != m_lastPen)
			{
				m_curPen = m_lastPen;
				::SelectObject(m_hDC, m_curPen->GetHPEN());
			}

			if(m_curBrush != m_lastBrush)
			{
				m_curBrush = m_lastBrush;
				::SelectObject(m_hDC, m_curBrush->GetHBrush());
			}

			if(m_curBmp != m_lastBmp)
			{
				m_curBmp = m_lastBmp;
				::SelectObject(m_hDC, m_curBmp->GetHBITMAP());
			}
		}
		else
		{
			emUIOBJTYPE eType = pObject->ObjectType();
			if(eType == OT_FONT || eType == OT_PEN || eType == OT_BRUSH || eType == OT_BITMAP)
			{
				SelectObject(pObject);
			}
		}
	}

	void UIRender_gdi::RestoreDefaultObject()
	{
		m_lastFont = m_defFont;
		m_lastPen = m_defPen;
		m_lastBrush = m_defBrush;
		m_lastBmp = m_defBmp;

		if(m_curFont != m_defFont)
		{
			m_curFont = m_defFont;
			::SelectObject(m_hDC, m_curFont->GetHFONT(GetManager()));
		}

		if(m_curPen != m_defPen)
		{
			m_curPen = m_defPen;
			::SelectObject(m_hDC, m_curPen->GetHPEN());
		}

		if(m_curBrush != m_defBrush)
		{
			m_curBrush = m_defBrush;
			::SelectObject(m_hDC, m_curBrush->GetHBrush());
		}

		if(m_curBmp != m_defBmp)
		{
			m_curBmp = m_defBmp;
			::SelectObject(m_hDC, m_curBmp->GetHBITMAP());
		}
	}

	DWORD UIRender_gdi::SetPixel(int x, int y, DWORD dwColor)
	{
		return ::SetPixel(GetDC(), x, y, dwColor);
	}

	BOOL UIRender_gdi::BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop)
	{
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, pSrcRender->GetDC(), xSrc, ySrc, dwRop);
	}

	BOOL UIRender_gdi::StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
	{
		::SetStretchBltMode(GetDC(), HALFTONE);
		return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, pSrcRender->GetDC(), xSrc, ySrc, nWidthSrc, nHeightSrc, dwRop);
	}

	BOOL UIRender_gdi::AlphaBlend(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int alpha )
	{
		typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
		static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
		if( lpAlphaBlend == NULL ) lpAlphaBlend = AlphaBitBlt;

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, AC_SRC_ALPHA };
		return lpAlphaBlend(m_hDC, x, y, nWidth, nHeight, pSrcRender->GetDC(), xSrc, ySrc, nWidthSrc, nHeightSrc, bf);
	}

	void UIRender_gdi::DrawBitmapAlpha(int x, int y, int nWidth, int nHeight, UIBitmap *pUiBitmap, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, int alpha)
	{
		UIRender_gdi *pRender = (UIRender_gdi *)UIGlobal::CreateRenderTarget();
		pRender->Init(GetManager(), GetDC());
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(pRender->GetDC(), pUiBitmap->GetHBITMAP());
		if(pUiBitmap->IsAlpha() || alpha < 255)
		{
			AlphaBlend(x, y, nWidth, nHeight, pRender, xSrc, ySrc, nWidthSrc, nHeightSrc, alpha);
		}
		else if(nWidth == nWidthSrc && nHeight == nHeightSrc)
		{
			BitBlt(x, y, nWidth, nHeight, pRender, xSrc, ySrc, SRCCOPY);
		}
		else
		{
			StretchBlt(x, y, nWidth, nHeight, pRender, xSrc, ySrc, nWidthSrc, nHeightSrc, SRCCOPY);
		}
		::SelectObject(pRender->GetDC(), hOldBitmap);
		pRender->Release();
	}

	void UIRender_gdi::DrawColor(const RECT& rc, const SIZE &round, DWORD color)
	{
		HDC hDC = GetDC();
		if( color <= 0x00FFFFFF ) return;

		Gdiplus::Graphics graphics( hDC );
		Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
		graphics.FillRectangle(&brush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	void UIRender_gdi::DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
		static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
		if( lpAlphaBlend == NULL ) lpAlphaBlend = AlphaBitBlt;
		typedef BOOL (WINAPI *PGradientFill)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
		static PGradientFill lpGradientFill = (PGradientFill) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "GradientFill");

		BYTE bAlpha = (BYTE)(((dwFirst >> 24) + (dwSecond >> 24)) >> 1);
		if( bAlpha == 0 ) return;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		RECT rcPaint = rc;
		HDC hPaintDC = hDC;
		HBITMAP hPaintBitmap = NULL;
		HBITMAP hOldPaintBitmap = NULL;
		if( bAlpha < 255 ) {
			rcPaint.left = rcPaint.top = 0;
			rcPaint.right = cx;
			rcPaint.bottom = cy;
			hPaintDC = ::CreateCompatibleDC(hDC);
			hPaintBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
			ASSERT(hPaintDC);
			ASSERT(hPaintBitmap);
			hOldPaintBitmap = (HBITMAP) ::SelectObject(hPaintDC, hPaintBitmap);
		}
		if( lpGradientFill != NULL ) 
		{
			TRIVERTEX triv[2] = 
			{
				{ rcPaint.left, rcPaint.top, 
				static_cast<COLOR16>(GetBValue(dwFirst) << 8),
				static_cast<COLOR16>(GetGValue(dwFirst) << 8),
				static_cast<COLOR16>(GetRValue(dwFirst) << 8), 0xFF00 },
				{ rcPaint.right, rcPaint.bottom, 
				static_cast<COLOR16>(GetBValue(dwSecond) << 8),
				static_cast<COLOR16>(GetGValue(dwSecond) << 8),
				static_cast<COLOR16>(GetRValue(dwSecond) << 8), 0xFF00 }
			};
			GRADIENT_RECT grc = { 0, 1 };
			lpGradientFill(hPaintDC, triv, 2, &grc, 1, bVertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
		}
		else 
		{
			// Determine how many shades
			int nShift = 1;
			if( nSteps >= 64 ) nShift = 6;
			else if( nSteps >= 32 ) nShift = 5;
			else if( nSteps >= 16 ) nShift = 4;
			else if( nSteps >= 8 ) nShift = 3;
			else if( nSteps >= 4 ) nShift = 2;
			int nLines = 1 << nShift;
			for( int i = 0; i < nLines; i++ ) {
				// Do a little alpha blending
				BYTE bR = (BYTE) ((GetBValue(dwSecond) * (nLines - i) + GetBValue(dwFirst) * i) >> nShift);
				BYTE bG = (BYTE) ((GetGValue(dwSecond) * (nLines - i) + GetGValue(dwFirst) * i) >> nShift);
				BYTE bB = (BYTE) ((GetRValue(dwSecond) * (nLines - i) + GetRValue(dwFirst) * i) >> nShift);
				// ... then paint with the resulting color
				HBRUSH hBrush = ::CreateSolidBrush(RGB(bR,bG,bB));
				RECT r2 = rcPaint;
				if( bVertical ) {
					r2.bottom = rc.bottom - ((i * (rc.bottom - rc.top)) >> nShift);
					r2.top = rc.bottom - (((i + 1) * (rc.bottom - rc.top)) >> nShift);
					if( (r2.bottom - r2.top) > 0 ) ::FillRect(hDC, &r2, hBrush);
				}
				else {
					r2.left = rc.right - (((i + 1) * (rc.right - rc.left)) >> nShift);
					r2.right = rc.right - ((i * (rc.right - rc.left)) >> nShift);
					if( (r2.right - r2.left) > 0 ) ::FillRect(hPaintDC, &r2, hBrush);
				}
				::DeleteObject(hBrush);
			}
		}
		if( bAlpha < 255 ) {
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, bAlpha, AC_SRC_ALPHA };
			lpAlphaBlend(hDC, rc.left, rc.top, cx, cy, hPaintDC, 0, 0, cx, cy, bf);
			::SelectObject(hPaintDC, hOldPaintBitmap);
			::DeleteObject(hPaintBitmap);
			::DeleteDC(hPaintDC);
		}
	}

	void UIRender_gdi::DrawLine(int x1, int y1, int x2, int y2, int nSize, DWORD dwPenColor,int nStyle)
	{
		HDC hDC = GetDC();
		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);

// 		if(m_curPen->GetStyle() != nStyle || m_curPen->GetWidth() != nSize || m_curPen->GetColor() != dwPenColor)
// 		{
// 			CStdRefPtr<UIPen> pPen = MakeRefPtr<UIPen>(UIGlobal::CreatePen());
// 			pPen->CreatePen(nStyle, nSize, dwPenColor);
// 			SelectObject(pPen);
// 		}
// 		POINT point;
// 		::MoveToEx(hDC, rc.left, rc.top, &point);
// 		::LineTo(hDC, rc.right, rc.bottom);

		LOGPEN lg;
		lg.lopnColor = RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor));
		lg.lopnStyle = nStyle;
		lg.lopnWidth.x = nSize;
		HPEN hPen = CreatePenIndirect(&lg);
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		POINT ptTemp = { 0 };
		::MoveToEx(hDC, x1, y1, &ptTemp);
		::LineTo(hDC, x2, y2);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
	}

	void UIRender_gdi::DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		HDC hDC = GetDC();
#if USE_GDI_RENDER
		ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
		HPEN hPen = ::CreatePen(nStyle | PS_INSIDEFRAME, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
		::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
#else
		ASSERT(::GetObjectType(hDC) == OBJ_DC || ::GetObjectType(hDC) == OBJ_MEMDC);
		Gdiplus::Graphics graphics(hDC);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		pen.SetAlignment(Gdiplus::PenAlignmentInset);

		graphics.DrawRectangle(&pen, rc.left, rc.top, rc.right - rc.left - 1, rc.bottom - rc.top - 1);
#endif
	}

	void UIRender_gdi::DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		HDC hDC = GetDC();
		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		HPEN hPen = ::CreatePen(nStyle, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
		HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
		::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, round.cx, round.cy);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
	}

	void UIRender_gdi::DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle)
	{
		HPEN hPen = ::CreatePen(nStyle, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
		::Ellipse(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
		::SelectObject(m_hDC, hOldPen);
		::DeleteObject(hPen);
	}

	void UIRender_gdi::FillEllipse(const RECT& rc, DWORD dwColor)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor)));
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor)));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDC, hBrush);

		::Ellipse(m_hDC, rc.left, rc.top, rc.right, rc.bottom);

		::SelectObject(m_hDC, hOldBrush);
		::SelectObject(m_hDC, hOldPen);
		::DeleteObject(hPen);
		::DeleteObject(hBrush);
	}

	void UIRender_gdi::DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		if( pstrText == NULL || pManager == NULL ) return;

		if ( pManager->IsLayered() || pManager->IsUseGdiplusText())
		{
			Gdiplus::Graphics graphics( hDC );
			Gdiplus::Font font(hDC, (HFONT)pManager->GetFont(iFont)->GetHFONT(GetManager()));
			Gdiplus::TextRenderingHint trh = Gdiplus::TextRenderingHintSystemDefault;
			switch(pManager->GetGdiplusTextRenderingHint()) 
			{
			case 0: {trh = Gdiplus::TextRenderingHintSystemDefault; break;}
			case 1: {trh = Gdiplus::TextRenderingHintSingleBitPerPixelGridFit; break;}
			case 2: {trh = Gdiplus::TextRenderingHintSingleBitPerPixel; break;}
			case 3: {trh = Gdiplus::TextRenderingHintAntiAliasGridFit; break;}
			case 4: {trh = Gdiplus::TextRenderingHintAntiAlias; break;}
			case 5: {trh = Gdiplus::TextRenderingHintClearTypeGridFit; break;}
			}
			graphics.SetTextRenderingHint(trh);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality); 
			graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

			Gdiplus::RectF rectF((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top, (Gdiplus::REAL)(rc.right - rc.left), (Gdiplus::REAL)(rc.bottom - rc.top));
			Gdiplus::SolidBrush brush(Gdiplus::Color(254, GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

			Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();

			if ((uStyle & DT_END_ELLIPSIS) != 0) {
				stringFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			}

			int formatFlags = 0;
			if ((uStyle & DT_NOCLIP) != 0) {
				formatFlags |= Gdiplus::StringFormatFlagsNoClip;
			}
			if ((uStyle & DT_SINGLELINE) != 0) {
				formatFlags |= Gdiplus::StringFormatFlagsNoWrap;
			}

			stringFormat.SetFormatFlags(formatFlags);

			if ((uStyle & DT_LEFT) != 0) {
				stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
			}
			else if ((uStyle & DT_CENTER) != 0) {
				stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
			}
			else if ((uStyle & DT_RIGHT) != 0) {
				stringFormat.SetAlignment(Gdiplus::StringAlignmentFar);
			}
			else {
				stringFormat.SetAlignment(Gdiplus::StringAlignmentNear);
			}
			stringFormat.GenericTypographic();
			if ((uStyle & DT_TOP) != 0) {
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
			}
			else if ((uStyle & DT_VCENTER) != 0) {
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			}
			else if ((uStyle & DT_BOTTOM) != 0) {
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentFar);
			}
			else {
				stringFormat.SetLineAlignment(Gdiplus::StringAlignmentNear);
			}
#ifdef UNICODE
			if ((uStyle & DT_CALCRECT) == DT_CALCRECT)
			{
				Gdiplus::RectF bounds;

				graphics.MeasureString(pstrText, -1, &font, rectF, &stringFormat, &bounds);

				// MeasureString存在计算误差，这里加一像素
				rc.bottom = rc.top + (long)bounds.Height + 1;
				rc.right = rc.left + (long)bounds.Width + 1;
			}
			else
			{
				graphics.DrawString(pstrText, -1, &font, rectF, &stringFormat, &brush);
			}
#else
			DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, pstrText, -1, NULL, 0);
			WCHAR * pcwszDest = new WCHAR[dwSize + 1];
			memset(pcwszDest, 0, (dwSize + 1) * sizeof(WCHAR));
			MultiByteToWideChar(CP_ACP, NULL, pstrText, -1, pcwszDest, dwSize);
			if(pcwszDest != NULL)
			{
				if ((uStyle & DT_CALCRECT) == DT_CALCRECT)
				{
					Gdiplus::RectF bounds;
					graphics.MeasureString(pcwszDest, -1, &font, rectF, &stringFormat, &bounds);
					rc.bottom = rc.top + (long)(bounds.Height * 1.06);
					rc.right = rc.left + (long)(bounds.Width * 1.06);
				}
				else
				{
					graphics.DrawString(pcwszDest, -1, &font, rectF, &stringFormat, &brush);
				}
				delete []pcwszDest;
			}
#endif
		}
		else
		{
			::SetBkMode(hDC, TRANSPARENT);
			::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
			CStdRefPtr<UIObject> pOldFont = SelectObject(pManager->GetFont(iFont));
			::DrawText(hDC, pstrText, -1, &rc, uStyle);
			SelectObject(pOldFont);
		}
	}

	UIPath* UIRender_gdi::CreatePath()
	{
		return new UIPath_gdi(m_hDC);
	}

	BOOL UIRender_gdi::DrawPath(const UIPath* path, int nSize, DWORD dwColor)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor)));
		HPEN hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
		BOOL bRet = ::StrokePath(m_hDC);
		::SelectObject(m_hDC, hOldPen);
		::DeleteObject(hPen);
		return bRet;
	}

	BOOL UIRender_gdi::FillPath(const UIPath* path, const DWORD dwColor)
	{
		HBRUSH hBrush = ::CreateSolidBrush(RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor)));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDC, hBrush);
		BOOL bRet = ::FillPath(m_hDC);
		::SelectObject(m_hDC, hOldBrush);
		::DeleteObject(hBrush);
		return bRet;
	}

	SIZE UIRender_gdi::GetTextSize(LPCTSTR pstrText, int iFont, UINT uStyle )
	{
		HDC hDC = GetDC();
		SIZE size = {0,0};
		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		if( pstrText == NULL || m_pManager == NULL ) return size;
		::SetBkMode(hDC, TRANSPARENT);
		SelectObject(m_pManager->GetFont(iFont));
		GetTextExtentPoint32(hDC, pstrText, _tcslen(pstrText) , &size);
		RestoreObject();
		return size;
	}

} // namespace DuiLib
#endif //#ifdef DUILIB_WIN32
