#include "StdAfx.h"
#include "UIRender_gdi.h"

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
		m_defBmp->CreateARGB32Bitmap(m_hDC, 1, 1);

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

	CPaintManagerUI *UIRender_gdi::GetManager()
	{
		return m_pManager;
	}

	HDC UIRender_gdi::GetDC()
	{
		return m_hDC;
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
		if(!m_curBmp->CreateARGB32Bitmap(m_hDC, width, height))
			return false;

		SelectObject(m_curBmp);
		return true;
	}

	bool UIRender_gdi::Resize(const RECT &rc)
	{
		return Resize(rc.right - rc.left, rc.bottom - rc.top);
	}

	void UIRender_gdi::Clear()
	{
		if(!m_curBmp) 
			return;
		m_curBmp->Clear();
	}

	void UIRender_gdi::SaveDC()
	{
		m_iSaveDC = ::SaveDC(m_hDC);
	}

	void UIRender_gdi::RestoreDC()
	{
		::RestoreDC(m_hDC, m_iSaveDC);
	}

	UIObject* UIRender_gdi::SelectObject(UIObject *pObject)
	{
		emUIOBJTYPE eType = pObject->ObjectType();
		if(eType == OT_FONT)
		{
			::SelectObject(m_hDC, ((UIFont *)pObject)->GetHFont(GetManager()));
			m_lastFont = m_curFont ? m_curFont : m_defFont;
			m_curFont = (UIFont_gdi *)pObject;
			return *(UIFont**)&m_lastFont;
		}
		else if(eType == OT_PEN)
		{
			::SelectObject(m_hDC, ((UIPen *)pObject)->GetHPen());
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
			::SelectObject(m_hDC, ((UIBitmap *)pObject)->GetBitmap());
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
				::SelectObject(m_hDC, m_curFont->GetHFont(GetManager()));
			}

			if(m_curPen != m_lastPen)
			{
				m_curPen = m_lastPen;
				::SelectObject(m_hDC, m_curPen->GetHPen());
			}

			if(m_curBrush != m_lastBrush)
			{
				m_curBrush = m_lastBrush;
				::SelectObject(m_hDC, m_curBrush->GetHBrush());
			}

			if(m_curBmp != m_lastBmp)
			{
				m_curBmp = m_lastBmp;
				::SelectObject(m_hDC, m_curBmp->GetBitmap());
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
			::SelectObject(m_hDC, m_curFont->GetHFont(GetManager()));
		}

		if(m_curPen != m_defPen)
		{
			m_curPen = m_defPen;
			::SelectObject(m_hDC, m_curPen->GetHPen());
		}

		if(m_curBrush != m_defBrush)
		{
			m_curBrush = m_defBrush;
			::SelectObject(m_hDC, m_curBrush->GetHBrush());
		}

		if(m_curBmp != m_defBmp)
		{
			m_curBmp = m_defBmp;
			::SelectObject(m_hDC, m_curBmp->GetBitmap());
		}
	}

	BOOL UIRender_gdi::BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop)
	{
		return ::BitBlt(m_hDC, x, y, nWidth, nHeight, pSrcRender->GetDC(), xSrc, ySrc, dwRop);
	}

	BOOL UIRender_gdi::StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop)
	{
		return ::StretchBlt(m_hDC, x, y, nWidth, nHeight, pSrcRender->GetDC(), xSrc, ySrc, nWidthSrc, nHeightSrc, dwRop);
	}

	void UIRender_gdi::DrawBitmap(HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint,
		const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, 
		BYTE uFade, bool hole, bool xtiled, bool ytiled)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);

		typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
		static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");

		if( lpAlphaBlend == NULL ) lpAlphaBlend = AlphaBitBlt;
		if( hBitmap == NULL ) return;

		HDC hCloneDC = ::CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
		::SetStretchBltMode(hDC, HALFTONE);

		RECT rcTemp = {0};
		RECT rcDest = {0};
		if( lpAlphaBlend && (bAlpha || uFade < 255) ) {
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, uFade, AC_SRC_ALPHA };
			// middle
			if( !hole ) {
				rcDest.left = rc.left + rcCorners.left;
				rcDest.top = rc.top + rcCorners.top;
				rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
				rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					if( !xtiled && !ytiled ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
							rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf);
					}
					else if( xtiled && ytiled ) {
						LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
						LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
						int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
						int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
						for( int j = 0; j < iTimesY; ++j ) {
							LONG lDestTop = rcDest.top + lHeight * j;
							LONG lDestBottom = rcDest.top + lHeight * (j + 1);
							LONG lDrawHeight = lHeight;
							if( lDestBottom > rcDest.bottom ) {
								lDrawHeight -= lDestBottom - rcDest.bottom;
								lDestBottom = rcDest.bottom;
							}
							for( int i = 0; i < iTimesX; ++i ) {
								LONG lDestLeft = rcDest.left + lWidth * i;
								LONG lDestRight = rcDest.left + lWidth * (i + 1);
								LONG lDrawWidth = lWidth;
								if( lDestRight > rcDest.right ) {
									lDrawWidth -= lDestRight - rcDest.right;
									lDestRight = rcDest.right;
								}
								lpAlphaBlend(hDC, rcDest.left + lWidth * i, rcDest.top + lHeight * j, 
									lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC, 
									rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, lDrawWidth, lDrawHeight, bf);
							}
						}
					}
					else if( xtiled ) {
						LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
						int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
						for( int i = 0; i < iTimes; ++i ) {
							LONG lDestLeft = rcDest.left + lWidth * i;
							LONG lDestRight = rcDest.left + lWidth * (i + 1);
							LONG lDrawWidth = lWidth;
							if( lDestRight > rcDest.right ) {
								lDrawWidth -= lDestRight - rcDest.right;
								lDestRight = rcDest.right;
							}
							lpAlphaBlend(hDC, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
								hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf);
						}
					}
					else { // ytiled
						LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
						int iTimes = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
						for( int i = 0; i < iTimes; ++i ) {
							LONG lDestTop = rcDest.top + lHeight * i;
							LONG lDestBottom = rcDest.top + lHeight * (i + 1);
							LONG lDrawHeight = lHeight;
							if( lDestBottom > rcDest.bottom ) {
								lDrawHeight -= lDestBottom - rcDest.bottom;
								lDestBottom = rcDest.bottom;
							}
							lpAlphaBlend(hDC, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
								hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, bf);                    
						}
					}
				}
			}

			// left-top
			if( rcCorners.left > 0 && rcCorners.top > 0 ) {
				rcDest.left = rc.left;
				rcDest.top = rc.top;
				rcDest.right = rcCorners.left;
				rcDest.bottom = rcCorners.top;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, bf);
				}
			}
			// top
			if( rcCorners.top > 0 ) {
				rcDest.left = rc.left + rcCorners.left;
				rcDest.top = rc.top;
				rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
				rcDest.bottom = rcCorners.top;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
						rcCorners.left - rcCorners.right, rcCorners.top, bf);
				}
			}
			// right-top
			if( rcCorners.right > 0 && rcCorners.top > 0 ) {
				rcDest.left = rc.right - rcCorners.right;
				rcDest.top = rc.top;
				rcDest.right = rcCorners.right;
				rcDest.bottom = rcCorners.top;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, bf);
				}
			}
			// left
			if( rcCorners.left > 0 ) {
				rcDest.left = rc.left;
				rcDest.top = rc.top + rcCorners.top;
				rcDest.right = rcCorners.left;
				rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
						rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf);
				}
			}
			// right
			if( rcCorners.right > 0 ) {
				rcDest.left = rc.right - rcCorners.right;
				rcDest.top = rc.top + rcCorners.top;
				rcDest.right = rcCorners.right;
				rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
						rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, bf);
				}
			}
			// left-bottom
			if( rcCorners.left > 0 && rcCorners.bottom > 0 ) {
				rcDest.left = rc.left;
				rcDest.top = rc.bottom - rcCorners.bottom;
				rcDest.right = rcCorners.left;
				rcDest.bottom = rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, bf);
				}
			}
			// bottom
			if( rcCorners.bottom > 0 ) {
				rcDest.left = rc.left + rcCorners.left;
				rcDest.top = rc.bottom - rcCorners.bottom;
				rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
				rcDest.bottom = rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
						rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, bf);
				}
			}
			// right-bottom
			if( rcCorners.right > 0 && rcCorners.bottom > 0 ) {
				rcDest.left = rc.right - rcCorners.right;
				rcDest.top = rc.bottom - rcCorners.bottom;
				rcDest.right = rcCorners.right;
				rcDest.bottom = rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
					lpAlphaBlend(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
						rcCorners.bottom, bf);
				}
			}
		}
		else 
		{
			if (rc.right - rc.left == rcBmpPart.right - rcBmpPart.left \
				&& rc.bottom - rc.top == rcBmpPart.bottom - rcBmpPart.top \
				&& rcCorners.left == 0 && rcCorners.right == 0 && rcCorners.top == 0 && rcCorners.bottom == 0)
			{
				if( ::IntersectRect(&rcTemp, &rcPaint, &rc) ) {
					::BitBlt(hDC, rcTemp.left, rcTemp.top, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, \
						hCloneDC, rcBmpPart.left + rcTemp.left - rc.left, rcBmpPart.top + rcTemp.top - rc.top, SRCCOPY);
				}
			}
			else
			{
				// middle
				if( !hole ) {
					rcDest.left = rc.left + rcCorners.left;
					rcDest.top = rc.top + rcCorners.top;
					rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
					rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						if( !xtiled && !ytiled ) {
							rcDest.right -= rcDest.left;
							rcDest.bottom -= rcDest.top;
							::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
								rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
								rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
						}
						else if( xtiled && ytiled ) {
							LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
							LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
							int iTimesX = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
							int iTimesY = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
							for( int j = 0; j < iTimesY; ++j ) {
								LONG lDestTop = rcDest.top + lHeight * j;
								LONG lDestBottom = rcDest.top + lHeight * (j + 1);
								LONG lDrawHeight = lHeight;
								if( lDestBottom > rcDest.bottom ) {
									lDrawHeight -= lDestBottom - rcDest.bottom;
									lDestBottom = rcDest.bottom;
								}
								for( int i = 0; i < iTimesX; ++i ) {
									LONG lDestLeft = rcDest.left + lWidth * i;
									LONG lDestRight = rcDest.left + lWidth * (i + 1);
									LONG lDrawWidth = lWidth;
									if( lDestRight > rcDest.right ) {
										lDrawWidth -= lDestRight - rcDest.right;
										lDestRight = rcDest.right;
									}
									::BitBlt(hDC, rcDest.left + lWidth * i, rcDest.top + lHeight * j, \
										lDestRight - lDestLeft, lDestBottom - lDestTop, hCloneDC, \
										rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, SRCCOPY);
								}
							}
						}
						else if( xtiled ) {
							LONG lWidth = rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right;
							int iTimes = (rcDest.right - rcDest.left + lWidth - 1) / lWidth;
							for( int i = 0; i < iTimes; ++i ) {
								LONG lDestLeft = rcDest.left + lWidth * i;
								LONG lDestRight = rcDest.left + lWidth * (i + 1);
								LONG lDrawWidth = lWidth;
								if( lDestRight > rcDest.right ) {
									lDrawWidth -= lDestRight - rcDest.right;
									lDestRight = rcDest.right;
								}
								::StretchBlt(hDC, lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
									hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
									lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
							}
						}
						else { // ytiled
							LONG lHeight = rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom;
							int iTimes = (rcDest.bottom - rcDest.top + lHeight - 1) / lHeight;
							for( int i = 0; i < iTimes; ++i ) {
								LONG lDestTop = rcDest.top + lHeight * i;
								LONG lDestBottom = rcDest.top + lHeight * (i + 1);
								LONG lDrawHeight = lHeight;
								if( lDestBottom > rcDest.bottom ) {
									lDrawHeight -= lDestBottom - rcDest.bottom;
									lDestBottom = rcDest.bottom;
								}
								::StretchBlt(hDC, rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
									hCloneDC, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
									rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, SRCCOPY);                    
							}
						}
					}
				}

				// left-top
				if( rcCorners.left > 0 && rcCorners.top > 0 ) {
					rcDest.left = rc.left;
					rcDest.top = rc.top;
					rcDest.right = rcCorners.left;
					rcDest.bottom = rcCorners.top;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, SRCCOPY);
					}
				}
				// top
				if( rcCorners.top > 0 ) {
					rcDest.left = rc.left + rcCorners.left;
					rcDest.top = rc.top;
					rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
					rcDest.bottom = rcCorners.top;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
							rcCorners.left - rcCorners.right, rcCorners.top, SRCCOPY);
					}
				}
				// right-top
				if( rcCorners.right > 0 && rcCorners.top > 0 ) {
					rcDest.left = rc.right - rcCorners.right;
					rcDest.top = rc.top;
					rcDest.right = rcCorners.right;
					rcDest.bottom = rcCorners.top;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, SRCCOPY);
					}
				}
				// left
				if( rcCorners.left > 0 ) {
					rcDest.left = rc.left;
					rcDest.top = rc.top + rcCorners.top;
					rcDest.right = rcCorners.left;
					rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
							rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
					}
				}
				// right
				if( rcCorners.right > 0 ) {
					rcDest.left = rc.right - rcCorners.right;
					rcDest.top = rc.top + rcCorners.top;
					rcDest.right = rcCorners.right;
					rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
							rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
					}
				}
				// left-bottom
				if( rcCorners.left > 0 && rcCorners.bottom > 0 ) {
					rcDest.left = rc.left;
					rcDest.top = rc.bottom - rcCorners.bottom;
					rcDest.right = rcCorners.left;
					rcDest.bottom = rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, SRCCOPY);
					}
				}
				// bottom
				if( rcCorners.bottom > 0 ) {
					rcDest.left = rc.left + rcCorners.left;
					rcDest.top = rc.bottom - rcCorners.bottom;
					rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
					rcDest.bottom = rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, SRCCOPY);
					}
				}
				// right-bottom
				if( rcCorners.right > 0 && rcCorners.bottom > 0 ) {
					rcDest.left = rc.right - rcCorners.right;
					rcDest.top = rc.bottom - rcCorners.bottom;
					rcDest.right = rcCorners.right;
					rcDest.bottom = rcCorners.bottom;
					rcDest.right += rcDest.left;
					rcDest.bottom += rcDest.top;
					if( ::IntersectRect(&rcTemp, &rcPaint, &rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
						::StretchBlt(hDC, rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, hCloneDC, \
							rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
							rcCorners.bottom, SRCCOPY);
					}
				}
			}
		}

		::SelectObject(hCloneDC, hOldBitmap);
		::DeleteDC(hCloneDC);
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

	void UIRender_gdi::DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/ )
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
		::MoveToEx(hDC, rc.left, rc.top, &ptTemp);
		::LineTo(hDC, rc.right, rc.bottom);
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
		::Ellipse(m_hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	void UIRender_gdi::DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		rc.left += rcTextPadding.left;
		rc.right -= rcTextPadding.right;
		rc.top += rcTextPadding.top;
		rc.bottom -= rcTextPadding.bottom;

		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		if( pstrText == NULL || pManager == NULL ) return;

		if ( pManager->IsLayered() || pManager->IsUseGdiplusText())
		{
			Gdiplus::Graphics graphics( hDC );
			Gdiplus::Font font(hDC, (HFONT)pManager->GetFont(iFont)->GetHFont(GetManager()));
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
			if ((uStyle & DT_CALCRECT) != 0)
			{
				Gdiplus::RectF rectFCalc(0,0,9999,9999);
				Gdiplus::RectF bounds;

				graphics.MeasureString(pstrText, -1, &font, rectFCalc, &stringFormat, &bounds);

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
				if ((uStyle & DT_CALCRECT) != 0)
				{
					Gdiplus::RectF rectFCalc(0,0,9999,9999);
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
			UIObject *pOldFont = SelectObject(pManager->GetFont(iFont));
			::DrawText(hDC, pstrText, -1, &rc, uStyle);
			SelectObject(pOldFont);
		}
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
