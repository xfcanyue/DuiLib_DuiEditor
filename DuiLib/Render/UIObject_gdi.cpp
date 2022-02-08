#include "StdAfx.h"
#include "UIObject_gdi.h"

#ifndef strtoll
#define strtoll _strtoi64
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "..\Utils\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\Utils\stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "..\Utils\nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "..\Utils\nanosvgrast.h"
 
///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIFont_gdi::UIFont_gdi()
	{
		m_hFont = NULL;
		memset(&tm, 0, sizeof(tm));
	}

	UIFont_gdi::~UIFont_gdi()
	{
		if(m_hFont != NULL) { ::DeleteObject(m_hFont); m_hFont = NULL; }
		memset(&tm, 0, sizeof(tm));
	}

	void UIFont_gdi::DeleteObject()
	{
		if(m_hFont != NULL) { ::DeleteObject(m_hFont); m_hFont = NULL; }
		memset(&tm, 0, sizeof(tm));
	}

	HFONT UIFont_gdi::GetHFont(CPaintManagerUI *pManager)
	{
		return m_hFont;
	}

	UIFont* UIFont_gdi::Clone(CPaintManagerUI *pManager)
	{
		UIFont_gdi *pNewFont	= new UIFont_gdi;
		pNewFont->id			= id;
		pNewFont->sFontName		= sFontName;
		pNewFont->iSize			= iSize;
		pNewFont->bBold			= bBold;
		pNewFont->bUnderline	= bUnderline;
		pNewFont->bItalic		= bItalic;
		pNewFont->bDefault		= bDefault;
		pNewFont->bShared		= bShared;
		if(!pNewFont->_buildFont(pManager))
		{
			delete pNewFont; pNewFont = NULL;
		}
		ASSERT(pNewFont);
		return pNewFont;
	}

	int UIFont_gdi::GetHeight(CPaintManagerUI *pManager)
	{
		if(tm.tmHeight == 0)
		{
			HFONT hOldFont = (HFONT) ::SelectObject(pManager->GetPaintDC(), m_hFont);
			::GetTextMetrics(pManager->GetPaintDC(), &tm);
			::SelectObject(pManager->GetPaintDC(), hOldFont);
		}
		return tm.tmHeight;
	}

	BOOL UIFont_gdi::_buildFont(CPaintManagerUI *pManager)
	{
		if(sFontName.IsEmpty())
			return FALSE;

		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		if(sFontName.GetLength() > 0) 
		{
			_tcsncpy(lf.lfFaceName, sFontName, LF_FACESIZE);
		}
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfHeight = pManager ? -pManager->GetDPIObj()->Scale(iSize) : iSize;
		if( bBold ) lf.lfWeight = FW_BOLD;
		if( bUnderline ) lf.lfUnderline = TRUE;
		if( bItalic ) lf.lfItalic = TRUE;

		m_hFont = ::CreateFontIndirect(&lf);
		if( m_hFont == NULL ) 
			return FALSE;
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPen_gdi::UIPen_gdi()
	{
		m_hPen = NULL;
	}

	UIPen_gdi::~UIPen_gdi() 
	{
		if(m_hPen) { ::DeleteObject(m_hPen); m_hPen = NULL; }
	}

	void UIPen_gdi::DeleteObject()
	{
		if(m_hPen) { ::DeleteObject(m_hPen); m_hPen = NULL; }
	}

	HPEN UIPen_gdi::GetHPen() const
	{
		return m_hPen;
	}

	BOOL UIPen_gdi::CreatePen(int nStyle, int nWidth, DWORD dwColor)
	{
		DeleteObject();

		m_nStyle = nWidth;
		m_nWidth = nWidth;
		m_dwColor = dwColor;

		LOGPEN lg;
		lg.lopnColor = RGB(GetBValue(dwColor), GetGValue(dwColor), GetRValue(dwColor));
		lg.lopnStyle = nStyle;
		lg.lopnWidth.x = nWidth;
		m_hPen = CreatePenIndirect(&lg);
		return m_hPen != NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBrush_gdi::UIBrush_gdi() 
	{
		m_hBrush = NULL;
	}

	UIBrush_gdi::~UIBrush_gdi() 
	{
		if(m_hBrush) { ::DeleteObject(m_hBrush); m_hBrush = NULL; }
	}

	void UIBrush_gdi::DeleteObject()
	{
		if(m_hBrush) { ::DeleteObject(m_hBrush); m_hBrush = NULL; }
	}

	HBRUSH UIBrush_gdi::GetHBrush() const
	{
		return m_hBrush;
	}

	BOOL UIBrush_gdi::CreateFromHBrush(HBRUSH hBrush)
	{
		DeleteObject();
		m_hBrush = (HBRUSH)hBrush;
		return TRUE;
	}

	BOOL UIBrush_gdi::CreateSolidBrush(DWORD clr)
	{
		DeleteObject();
		m_hBrush = ::CreateSolidBrush(RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)));
		return m_hBrush != NULL;
	}

	BOOL UIBrush_gdi::CreateBitmapBrush(UIBitmap *bitmap)
	{
		DeleteObject();
		m_hBrush = ::CreatePatternBrush(bitmap->GetBitmap());
		return m_hBrush != NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPath_gdi::UIPath_gdi()
	{
		m_hDC = NULL;
		memset(&m_curPoint, 0, sizeof(POINT));
	}

	UIPath_gdi::UIPath_gdi(HDC hDC)
	{
		m_hDC = hDC;
		memset(&m_curPoint, 0, sizeof(POINT));
	}

	UIPath_gdi::~UIPath_gdi()
	{
		
	}

	void UIPath_gdi::DeleteObject()
	{
		
	}

	BOOL UIPath_gdi::Beginpath()
	{
		if(m_hDC == NULL) return FALSE;
		return ::BeginPath(m_hDC);
	}

	BOOL UIPath_gdi::EndPath()
	{
		if(m_hDC == NULL) return FALSE;
		return ::EndPath(m_hDC);
	}

	BOOL UIPath_gdi::AbortPath()
	{
		if(m_hDC == NULL) return FALSE;
		return ::AbortPath(m_hDC);
	}

	BOOL UIPath_gdi::AddLine(int x1, int y1, int x2, int y2)
	{
		if(m_hDC == NULL) return FALSE;
		
		if(m_curPoint.x != x1 && m_curPoint.y != y1)
		{
			POINT lastPoint;
			::MoveToEx(m_hDC, x1, y1, &lastPoint);
		}
		::LineTo(m_hDC, x2, y2);
		m_curPoint.x = x2;
		m_curPoint.y = y2;
		return TRUE;
	}

	BOOL UIPath_gdi::AddLines(CDuiPoint *points, int count)
	{
		if(m_hDC == NULL) return FALSE;
		POINT pt;
		::MoveToEx(m_hDC, points[0].x, points[0].y, &pt);
		for (int i=1; i<count; i++)
		{
			::LineTo(m_hDC, points[i].x, points[i].y);
		}
		::LineTo(m_hDC, points[0].x, points[0].y);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBitmap_gdi::UIBitmap_gdi()
	{
		m_hBitmap = NULL;
		m_pBits = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	UIBitmap_gdi::~UIBitmap_gdi()
	{
		if(m_hBitmap != NULL) { ::DeleteObject(m_hBitmap); m_hBitmap = NULL; }
		m_pBits = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	void UIBitmap_gdi::DeleteObject()
	{
		if(m_hBitmap != NULL) { ::DeleteObject(m_hBitmap); m_hBitmap = NULL; }
		m_pBits = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	BOOL UIBitmap_gdi::CreateFromHBitmap(HBITMAP hBitmap)
	{
		DeleteObject();
		m_hBitmap = (HBITMAP)hBitmap;

		BITMAP info;
		::GetObject(m_hBitmap, sizeof(info), &info);
		m_pBits = (LPBYTE)info.bmBits;
		m_nWidth = info.bmWidth;
		m_nHeight = info.bmHeight;
		return TRUE;
	}

	BOOL UIBitmap_gdi::CreateARGB32Bitmap(HDC hDC, int width, int height)
	{
		DeleteObject();
		m_nWidth = width;
		m_nHeight = height;

		BITMAPINFO bmi = { 0 };
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = width * height * sizeof(DWORD);

		m_hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void **)&m_pBits, NULL, 0);
		return m_hBitmap != NULL;
	}

	BOOL UIBitmap_gdi::CreateCompatibleBitmap(HDC hDC, int width, int height)
	{
		DeleteObject();
		m_nWidth = width;
		m_nHeight = height;

		m_hBitmap = ::CreateCompatibleBitmap(hDC, width, height);
		if(m_hBitmap)
		{
			BITMAP info;
			::GetObject(m_hBitmap, sizeof(info), &info);
			m_pBits = LPBYTE(info.bmBits);
		}
		return m_hBitmap != NULL;
	}

	HBITMAP UIBitmap_gdi::GetBitmap()
	{
		return m_hBitmap;
	}

	BYTE* UIBitmap_gdi::GetBits()
	{
		return m_pBits;
	}

	int	UIBitmap_gdi::GetWidth()
	{
		return m_nWidth;
	}

	int UIBitmap_gdi::GetHeight()
	{
		return m_nHeight;
	}

	void UIBitmap_gdi::Clear()
	{
		if(m_pBits == NULL || m_hBitmap == NULL)
			return;
		memset(m_pBits, 0, m_nWidth * m_nHeight * sizeof(DWORD));
	}

	void UIBitmap_gdi::ClearAlpha(const RECT &rc, int alpha)
	{
		if (m_pBits == NULL)
			return;

		unsigned int * pBmpBits = (unsigned int *)m_pBits;
		int nTop = MAX(rc.top, 0);
		int nBottom = MIN(rc.bottom, m_nHeight);
		int nLeft = MAX(rc.left, 0);
		int nRight = MIN(rc.right, m_nWidth);
		if (nRight > nLeft)	{
			for (int i = nTop; i < nBottom; ++i) {
				::memset(pBmpBits + i * m_nWidth + nLeft, alpha, (nRight - nLeft) * 4);
			}
		}
	}

	BOOL UIBitmap_gdi::SaveFile(LPCTSTR pstrFileName)
	{
		if(m_hBitmap == NULL) 
			return FALSE;

		HDC     hDC;
		//当前分辨率下每象素所占字节数
		int     iBits;
		//位图中每象素所占字节数
		WORD     wBitCount;
		//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数
		DWORD     dwPaletteSize=0,   dwBmBitsSize=0,   dwDIBSize=0,   dwWritten=0;
		//位图属性结构
		BITMAP     Bitmap;
		//位图文件头结构
		BITMAPFILEHEADER     bmfHdr;
		//位图信息头结构
		BITMAPINFOHEADER     bi;
		//指向位图信息头结构
		LPBITMAPINFOHEADER     lpbi;
		//定义文件，分配内存句柄，调色板句柄
		HANDLE     fh,   hDib,   hPal,hOldPal=NULL;

		int aa = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 288 * 352 * 24 / 8;

		//计算位图文件每个像素所占字节数
		hDC  = CreateDC(_T("DISPLAY"),   NULL,   NULL,   NULL);
		iBits  = GetDeviceCaps(hDC,   BITSPIXEL)     *     GetDeviceCaps(hDC,   PLANES);
		DeleteDC(hDC);
		if(iBits <=  1)
			wBitCount = 1;
		else  if(iBits <=  4)
			wBitCount  = 4;
		else if(iBits <=  8)
			wBitCount  = 8;
		else
			wBitCount  = 24;

		GetObject(m_hBitmap,   sizeof(Bitmap),   (LPSTR)&Bitmap);
		bi.biSize= sizeof(BITMAPINFOHEADER);
		bi.biWidth = Bitmap.bmWidth;
		bi.biHeight =  Bitmap.bmHeight;
		bi.biPlanes =  1;
		bi.biBitCount = wBitCount;
		bi.biCompression= BI_RGB;
		bi.biSizeImage = Bitmap.bmWidth * Bitmap.bmHeight * 3;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrImportant = 0;
		bi.biClrUsed =  0;

		dwBmBitsSize  = ((Bitmap.bmWidth *wBitCount+31) / 32)*4* Bitmap.bmHeight;

		//为位图内容分配内存
		hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
		lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);
		*lpbi  = bi;

		//     处理调色板
		hPal  = GetStockObject(DEFAULT_PALETTE);
		if (hPal)
		{
			hDC  = ::GetDC(NULL);
			hOldPal = ::SelectPalette(hDC,(HPALETTE)hPal, FALSE);
			RealizePalette(hDC);
		}

		//     获取该调色板下新的像素值
		GetDIBits(hDC,m_hBitmap, 0,(UINT)Bitmap.bmHeight,
			(LPSTR)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize,
			(BITMAPINFO *)lpbi, DIB_RGB_COLORS);

		//恢复调色板
		if (hOldPal)
		{
			::SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);
			RealizePalette(hDC);
			::ReleaseDC(NULL,   hDC);
		}

		//创建位图文件
		fh  = CreateFile(pstrFileName,   GENERIC_WRITE,0,   NULL,   CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,   NULL);

		if (fh     ==  INVALID_HANDLE_VALUE)         return     FALSE;

		//     设置位图文件头
		bmfHdr.bfType  = 0x4D42;     //     "BM"
		dwDIBSize  = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;
		bmfHdr.bfSize  = dwDIBSize;
		bmfHdr.bfReserved1  = 0;
		bmfHdr.bfReserved2  = 0;
		bmfHdr.bfOffBits  = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
		//     写入位图文件头
		WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);
		//     写入位图文件其余内容
		WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize - 14,   &dwWritten,   NULL);
		//清除
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		CloseHandle(fh);

		return     TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	//
	static LPBYTE WINAPI svg_load_from_memory(const LPBYTE buffer, int &out_width, int &out_height, int hope_width, int hope_height, DWORD fillcolor, CPaintManagerUI* pManager)
	{
		LPBYTE pImage = NULL;
		float dpi = 96.0f;
		float scale = 1.0f;
		if(pManager)
		{
			scale = (pManager->GetDPIObj()->GetScale() * 1.0)/100;
		}
		//SVG
		NSVGimage* svg = nsvgParse((LPSTR)(LPVOID)buffer, "px", (float)dpi);
		if (svg == NULL) 
		{
			return NULL;
		}

		if(fillcolor != 0)
		{
			NSVGshape *shapes = svg->shapes;
			while (shapes)
			{
				if(shapes->fill.type == NSVG_PAINT_COLOR)
					if(shapes->fill.color != 0)
						shapes->fill.color = UIRGB(GetRValue(fillcolor), GetGValue(fillcolor), GetBValue(fillcolor));
				shapes = shapes->next;
			}
		}

		out_width = (int)svg->width;
		out_height = (int)svg->height;
		if(out_width==0 || out_height==0) 
		{
			nsvgDelete(svg);
			return NULL;
		}

		//如果指定长宽，则使用当前的长宽比例，否则使用dpi适配
		if(hope_width > 0 && hope_height > 0)
		{
			out_width = hope_width;
			scale = hope_width * 1.0 / svg->width;
			out_height = svg->height * scale;
			if (svg->height * scale > out_height)
			{
				scale = out_height * 1.0 / svg->height;
				out_width = svg->width * scale;
			}
		}
		else
		{
			if(pManager)
			{
				out_width = pManager->GetDPIObj()->Scale(out_width);
				out_height = pManager->GetDPIObj()->Scale(out_height);
			}
		}

		NSVGrasterizer* rast = nsvgCreateRasterizer();
		if (rast == NULL)
		{
			nsvgDelete(svg);
			return NULL;
		}

		pImage = (LPBYTE)malloc((out_width + 1) * (out_height + 1) * 4);//由于使用了小数，防止被四舍五入舍去，因此都加1
		if (!pImage)
		{
			nsvgDeleteRasterizer(rast);
			nsvgDelete(svg);
			return NULL;
		}
		nsvgRasterize(rast, svg, 0, 0, scale, pImage, out_width, out_height, out_width * 4);
		nsvgDeleteRasterizer(rast);
		nsvgDelete(svg);
		return pImage;
	}

	static UIImage *make_imageinfo_from_stbi_memory(LPBYTE pImage, int x, int y, DWORD mask, int delay=0)
	{
		UIImage* data = UIGlobal::CreateImage(); //new UIImage_gdi;
		if(!data->bitmap->CreateARGB32Bitmap(NULL, x, y))
		{
			data->Release();
			return NULL;
		}

		bool bAlphaChannel = false;
		LPBYTE pDest = data->bitmap->GetBits();

		for( int i = 0; i < x * y; i++ ) 
		{
			pDest[i*4 + 3] = pImage[i*4 + 3];
			if( pDest[i*4 + 3] < 255 )
			{
				pDest[i*4] = (BYTE)(DWORD(pImage[i*4 + 2])*pImage[i*4 + 3]/255);
				pDest[i*4 + 1] = (BYTE)(DWORD(pImage[i*4 + 1])*pImage[i*4 + 3]/255);
				pDest[i*4 + 2] = (BYTE)(DWORD(pImage[i*4])*pImage[i*4 + 3]/255); 
				bAlphaChannel = true;
			}
			else
			{
				pDest[i*4] = pImage[i*4 + 2];
				pDest[i*4 + 1] = pImage[i*4 + 1];
				pDest[i*4 + 2] = pImage[i*4]; 
			}

			if( *(DWORD*)(&pDest[i*4]) == mask ) {
				pDest[i*4] = (BYTE)0;
				pDest[i*4 + 1] = (BYTE)0;
				pDest[i*4 + 2] = (BYTE)0; 
				pDest[i*4 + 3] = (BYTE)0;
				bAlphaChannel = true;
			}
		}

		data->pBits = pDest;
		data->pSrcBits = NULL;
		data->nWidth = x;
		data->nHeight = y;
		data->bAlpha = bAlphaChannel;
		data->delay = delay;
		return data;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIImage_gdi::UIImage_gdi()
	{
		bitmap = new UIBitmap_gdi;
		pBits		= nullptr;
		pSrcBits	= nullptr;;
		nWidth			= 0;
		nHeight			= 0;
		bAlpha		= false;
		bUseHSL		= false;
		dwMask		= 0;
		delay		= 0;
	}

	UIImage_gdi::~UIImage_gdi()
	{
		//pBits是从::CreateDIBSection搞过来的，不能delete
		//pSrcBits 是HSL色彩转换时分配的，需要delete
		if (pSrcBits) { delete[] pSrcBits; pSrcBits = NULL; }	
		if(bitmap) { bitmap->Release(); };
	}

	void UIImage_gdi::DeleteObject()
	{
		bitmap->DeleteObject();
		if (pSrcBits) { delete[] pSrcBits; pSrcBits = NULL; }
	}

	BOOL UIImage_gdi::CreateImage(HBITMAP hBitmap, bool bAlpha)
	{
		bitmap->CreateFromHBitmap(hBitmap);
		pBits = bitmap->GetBits();
		nWidth = bitmap->GetWidth();
		nHeight = bitmap->GetHeight();
		UIImage_gdi::bAlpha = bAlpha;
		return TRUE;
	}

	BOOL UIImage_gdi::LoadImage(const TDrawInfo *pDrawInfo, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		//是不是资源ID号
		if( _ttoi(pDrawInfo->sResType) != 0 ) 
		{
			CUIFile f;
			if(!f.LoadFile(pDrawInfo->sImageName.GetData(), pDrawInfo->sResType, instance)) 
				return FALSE;

			return LoadImageFromMemory(f.GetData(), f.GetSize(), pDrawInfo->dwMask, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, pManager);
		}
		else
		{
			//从Res.xml文件中读取，做资源替换。
			CDuiString sStrPath = CResourceManager::GetInstance()->GetImagePath(pDrawInfo->sImageName);
			if(sStrPath.IsEmpty())
				sStrPath = pDrawInfo->sImageName;

			CUIFile f;
			if(!f.LoadFile(sStrPath.GetData(), NULL, instance)) 
				return FALSE;

			return LoadImageFromMemory(f.GetData(), f.GetSize(), pDrawInfo->dwMask, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, pManager);
		}

		return FALSE;
	}

	BOOL UIImage_gdi::LoadImage(STRINGorID bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		CUIFile f;
		if(!f.LoadFile(bitmap, type, instance)) 
			return FALSE;

		return LoadImageFromMemory(f.GetData(), f.GetSize(), mask, width, height, fillcolor, pManager);
	}

	BOOL UIImage_gdi::LoadImage(LPCTSTR pStrImage, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		if(pStrImage == NULL) return FALSE;

		CDuiString sStrPath = pStrImage;
		if( type == NULL )  
		{
			//LoadImage需要重载3个函数，就是因为这里有资源替换。 -_-
			sStrPath = CResourceManager::GetInstance()->GetImagePath(pStrImage);
			if (sStrPath.IsEmpty()) sStrPath = pStrImage;
		}
		return LoadImage(STRINGorID(sStrPath.GetData()), type, mask, width, height, fillcolor, pManager, instance);
	}

	BOOL UIImage_gdi::LoadImage(UINT nID, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		return LoadImage(STRINGorID(nID), type, mask, width, height, fillcolor, pManager, instance);
	}

	BOOL UIImage_gdi::LoadImageFromMemory(const LPBYTE pData, DWORD dwSize, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager)
	{
		DeleteObject();

		LPBYTE pImage = NULL;
		int x,y,comp;
		pImage = stbi_load_from_memory(pData, dwSize, &x, &y, &comp, 4);	
		if( !pImage ) 
		{
			pImage = svg_load_from_memory(pData, x, y, width, height, fillcolor, pManager);		
		}
		if(!pImage) return FALSE;


		bool bAlphaChannel = false;
		if(!bitmap->CreateARGB32Bitmap(NULL, x, -y))
		{
			stbi_image_free(pImage);
			return FALSE;
		}

		LPBYTE pDest = bitmap->GetBits();
		for( int i = 0; i < x * y; i++ ) 
		{
			pDest[i*4 + 3] = pImage[i*4 + 3];
			if( pDest[i*4 + 3] < 255 )
			{
				pDest[i*4] = (BYTE)(DWORD(pImage[i*4 + 2])*pImage[i*4 + 3]/255);
				pDest[i*4 + 1] = (BYTE)(DWORD(pImage[i*4 + 1])*pImage[i*4 + 3]/255);
				pDest[i*4 + 2] = (BYTE)(DWORD(pImage[i*4])*pImage[i*4 + 3]/255); 
				bAlphaChannel = true;
			}
			else
			{
				pDest[i*4] = pImage[i*4 + 2];
				pDest[i*4 + 1] = pImage[i*4 + 1];
				pDest[i*4 + 2] = pImage[i*4]; 
			}

			if( *(DWORD*)(&pDest[i*4]) == mask ) {
				pDest[i*4] = (BYTE)0;
				pDest[i*4 + 1] = (BYTE)0;
				pDest[i*4 + 2] = (BYTE)0; 
				pDest[i*4 + 3] = (BYTE)0;
				bAlphaChannel = true;
			}
		}

		pBits = pDest;
		pSrcBits = NULL;
		nWidth = x;
		nHeight = y;
		bAlpha = bAlphaChannel;
		delay = delay;

		stbi_image_free(pImage);
		return TRUE;
	}

	void UIImage_gdi::AdjustHslImage(bool bHSL, short H, short S, short L)
	{
// 		if( (bUseHSL == false && !CPaintManagerUI::IsForceHSL()) || bitmap == NULL || pBits == NULL || pSrcBits == NULL ) 
// 			return;

		if( (bUseHSL == false && !CPaintManagerUI::IsForceHSL()) || bitmap == NULL || pBits == NULL || bitmap->GetBitmap() == NULL) 
			return;

		if(pSrcBits == NULL)
		{
			pSrcBits = new BYTE[nWidth * nHeight * 4];
			::CopyMemory(pSrcBits, pBits, nWidth * nHeight * 4);
		}

		if( bHSL == false || (H == 180 && S == 100 && L == 100)) {
			::CopyMemory(pBits, pSrcBits, nWidth * nHeight * 4);
			return;
		}

		float fH, fS, fL;
		float S1 = S / 100.0f;
		float L1 = L / 100.0f;
		for( int i = 0; i < nWidth * nHeight; i++ ) {
			UIGlobal::RGBtoHSL(*(DWORD*)(pSrcBits + i*4), &fH, &fS, &fL);
			fH += (H - 180);
			fH = fH > 0 ? fH : fH + 360; 
			fS *= S1;
			fL *= L1;
			UIGlobal::HSLtoRGB((DWORD*)(pBits + i*4), fH, fS, fL);
		}
	}

	bool UIImage_gdi::LoadGifImageFromFile(LPCTSTR fileName, CStdPtrArray &arrImageInfo)
	{
		CUIFile f;
		if(!f.LoadFile(fileName))
			return false;
		return LoadGifImageFromMemory(f.GetData(), f.GetSize(), arrImageInfo);
	}

	bool UIImage_gdi::LoadGifImageFromMemory(const LPBYTE pData, DWORD dwSize, CStdPtrArray &arrImageInfo)
	{
		LPBYTE pImage = NULL;
		int x,y,comp;
		int layers = 0;
		int *delays = 0;
		pImage = stbi_load_gif_from_memory(pData, dwSize, &delays, &x, &y, &layers, &comp, 4);
		if(!pImage) return false;

		for (int f=0; f<layers; f++)
		{
			LPBYTE pFrame = pImage + x * y * f * 4;
			UIImage *data = make_imageinfo_from_stbi_memory(pFrame, x, y, 0, delays[f]);
			if(!arrImageInfo.Add(data)) 
			{
				data->Release();
				break;
			}
		}
		stbi_image_free(delays);
		stbi_image_free(pImage);
		return true;
	}
} // namespace DuiLib
