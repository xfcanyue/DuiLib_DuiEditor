#include "StdAfx.h"
#include "IRender.h"
#include "UIRender_gdi.h"
#include "UIRender_gdiplus.h"

namespace DuiLib {

	//////////////////////////////////////////////////////////////////////////
	//
	//
	tagTDrawInfo::tagTDrawInfo()
	{
		Clear();
	}

	void tagTDrawInfo::Parse(LPCTSTR pStrImage, LPCTSTR pStrModify,CPaintManagerUI *paintManager)
	{
		// 1、aaa.jpg
		// 2、file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' 
		// mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false'
		sDrawString = pStrImage;
		sDrawModify = pStrModify;
		sImageName = pStrImage;

		CDuiString sItem;
		CDuiString sValue;
		LPTSTR pstr = NULL;
		for( int i = 0; i < 2; ++i ) {
			if( i == 1) pStrImage = pStrModify;
			if( !pStrImage ) continue;
			while( *pStrImage != _T('\0') ) {
				sItem.Empty();
				sValue.Empty();
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				while( *pStrImage != _T('\0') && *pStrImage != _T('=') && *pStrImage > _T(' ') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sItem += *pStrImage++;
					}
				}
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('=') ) break;
				while( *pStrImage > _T('\0') && *pStrImage <= _T(' ') ) pStrImage = ::CharNext(pStrImage);
				if( *pStrImage++ != _T('\'') ) break;
				while( *pStrImage != _T('\0') && *pStrImage != _T('\'') ) {
					LPTSTR pstrTemp = ::CharNext(pStrImage);
					while( pStrImage < pstrTemp) {
						sValue += *pStrImage++;
					}
				}
				if( *pStrImage++ != _T('\'') ) break;
				if( !sValue.IsEmpty() ) {
					if( sItem == _T("file") || sItem == _T("res") ) {
						sImageName = sValue;
					}
					else if( sItem == _T("restype") ) {
						sResType = sValue;
					}
					else if( sItem == _T("dest") ) {
						rcDest.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
						rcDest.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcDest.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcDest.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);  
						paintManager->GetDPIObj()->ScaleRect(&rcDest);
					}
					else if( sItem == _T("source") ) {
						rcSource.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
						rcSource.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcSource.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcSource.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
					}
					else if( sItem == _T("corner") ) {
						rcCorner.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
						rcCorner.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcCorner.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
					}
					else if( sItem == _T("mask") ) {
						if( sValue[0] == _T('#')) dwMask = _tcstoul(sValue.GetData() + 1, &pstr, 16);
						else dwMask = _tcstoul(sValue.GetData(), &pstr, 16);
					}
					else if( sItem == _T("fade") ) {
						uFade = (BYTE)_tcstoul(sValue.GetData(), &pstr, 10);
					}
					else if( sItem == _T("hole") ) {
						bHole = (_tcsicmp(sValue.GetData(), _T("true")) == 0);
					}
					else if( sItem == _T("xtiled") ) {
						bTiledX = (_tcsicmp(sValue.GetData(), _T("true")) == 0);
					}
					else if( sItem == _T("ytiled") ) {
						bTiledY = (_tcsicmp(sValue.GetData(), _T("true")) == 0);
					}
					else if( sItem == _T("hsl") ) {
						bHSL = (_tcsicmp(sValue.GetData(), _T("true")) == 0);
					}
					else if( sItem == _T("padding") ) {
						rcPadding.left = _tcstol(sValue.GetData(), &pstr, 10);  ASSERT(pstr);    
						rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
						rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
						rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);  
						if(paintManager != NULL) paintManager->GetDPIObj()->ScaleRect(&rcPadding);
					}
					else if( sItem == _T("align")){
						if(_tcsicmp(sValue.GetData(), _T("left")) == 0){
							uAlign &= ~(DT_CENTER | DT_RIGHT);
							uAlign |= DT_LEFT;
						}
						else if(_tcsicmp(sValue.GetData(), _T("right")) == 0){
							uAlign &= ~(DT_LEFT | DT_CENTER);
							uAlign |= DT_RIGHT;
						}
						else if(_tcsicmp(sValue.GetData(), _T("center")) == 0){
							uAlign &= ~(DT_LEFT | DT_RIGHT);
							uAlign |= DT_CENTER;
						}
					}
					else if( sItem == _T("valign")){
						if(_tcsicmp(sValue.GetData(), _T("top")) == 0){
							uAlign &= ~(DT_BOTTOM | DT_VCENTER);
							uAlign |= DT_TOP;
						}
						else if(_tcsicmp(sValue.GetData(), _T("bottom")) == 0){
							uAlign &= ~(DT_TOP | DT_VCENTER);
							uAlign |= DT_BOTTOM;
						}
						else if(_tcsicmp(sValue.GetData(), _T("vcenter")) == 0){
							uAlign &= ~(DT_TOP | DT_BOTTOM);
							uAlign |= DT_VCENTER;
						}
					}
					else if( sItem == _T("width") ) {
						width = _ttoi(sValue);
						width = paintManager->GetDPIObj()->ScaleInt(width);
					}
					else if( sItem == _T("height") ) {
						height = _ttoi(sValue);
						height = paintManager->GetDPIObj()->ScaleInt(height);
					}
					else if( sItem == _T("fillcolor") ) {
						if( sValue[0] == _T('#')) fillcolor = _tcstoul(sValue.GetData() + 1, &pstr, 16);
						else fillcolor = _tcstoul(sValue.GetData(), &pstr, 16);
					}
				}
				if( *pStrImage++ != _T(' ') ) break;
			}
		}

		// 调整DPI资源
		if (paintManager && paintManager->GetDPIObj()->GetScale() != 100) {
			CDuiString sScale;
			sScale.Format(_T("@%d."), paintManager->GetDPIObj()->GetScale());
			//sImageName.Replace(_T("."), sScale);

			//如果对应dpi的图像不存在，就不要替换了。
			CDuiString sImageNameTemp = sImageName;
			sImageNameTemp.Replace(_T("."), sScale);
			CUIFile file;
			if(file.LoadFile(sImageNameTemp.GetData()))
			{
				paintManager->GetDPIObj()->ScaleRect(&rcSource);
				paintManager->GetDPIObj()->ScaleRect(&rcCorner);

				sImageName = sImageNameTemp;
			}
		}
	}
	
	void tagTDrawInfo::Clear()
	{
		sDrawString.Empty();
		sDrawModify.Empty();
		sImageName.Empty();

		memset(&rcDest, 0, sizeof(RECT));
		memset(&rcSource, 0, sizeof(RECT));
		memset(&rcCorner, 0, sizeof(RECT));
		dwMask = 0;
		uFade = 255;
		bHole = false;
		bTiledX = false;
		bTiledY = false;
		bHSL = false;
		memset(&rcPadding, 0, sizeof(RECT));
		uAlign = DT_LEFT;
		width = 0;
		height = 0;
		fillcolor = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	BOOL UIFont::CreateDefaultFont()
	{
		DeleteObject();

#ifdef WIN32
		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

		sFontName = lf.lfFaceName;
		iSize = -lf.lfHeight;
		bBold = (lf.lfWeight >= FW_BOLD);
		bUnderline = (lf.lfUnderline == TRUE);
		bItalic = (lf.lfItalic == TRUE);
#else
		sFontName = _T("微软雅黑");
		iSize = 14;
		bBold = FALSE;
		bUnderline = FALSE;
		bItalic = FALSE;
		return _buildFont(NULL);
#endif

		return _buildFont(NULL);
	}

	BOOL UIFont::CreateFont(CPaintManagerUI *pManager, int id, LPCTSTR sFontName, int iSize, bool bBold, bool bUnderline, bool bItalic, bool bDefault, bool bShared)
	{
		DeleteObject();

		UIFont::id			= id;
		UIFont::sFontName	= sFontName;
		UIFont::iSize		= iSize;
		UIFont::bBold		= bBold;
		UIFont::bUnderline	= bUnderline;
		UIFont::bItalic		= bItalic;
		UIFont::bDefault	= bDefault;
		UIFont::bShared		= bShared;

		return _buildFont(pManager);
	}

	BOOL UIFont::CreateFont(CPaintManagerUI *pManager, CXmlNodeUI node)
	{
		DeleteObject();

		id			= node.attribute(_T("id")).as_int(0);
		sFontName	= node.attribute(_T("name")).as_string();
		iSize		= node.attribute(_T("size")).as_int(12);
		bBold		= node.attribute(_T("bold")).as_bool();
		bUnderline	= node.attribute(_T("underline")).as_bool();
		bItalic		= node.attribute(_T("italic")).as_bool();
		bDefault	= node.attribute(_T("default")).as_bool();
		bShared		= node.attribute(_T("shared")).as_bool();

		return _buildFont(pManager);
	}

	BOOL UIFont::RebuildFont(CPaintManagerUI *pManager)
	{
		DeleteObject();
		return _buildFont(pManager);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPen::UIPen()
	{
		m_nStyle = 0;
		m_nWidth = 0;
		m_dwColor = 0;
	}

	bool UIPen::operator == (UIPen &obj) const
	{
		return obj.m_nStyle == m_nStyle && 
			obj.m_nWidth == m_nWidth && 
			obj.m_dwColor == m_dwColor;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
#ifdef WIN32
#	ifndef strtoll
#		define strtoll _strtoi64
#	endif
#	if _MSC_VER <= 1600
		double roundf(double r){ return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);}
#	endif
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "../Utils/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Utils/stb_image_write.h"
#define NANOSVG_IMPLEMENTATION
#include "../Utils/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "../Utils/nanosvgrast.h"
	static LPBYTE svg_load_from_memory(const LPBYTE buffer, int &out_width, int &out_height, int hope_width, int hope_height, DWORD fillcolor, CPaintManagerUI* pManager)
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
				out_width = pManager->GetDPIObj()->ScaleInt(out_width);
				out_height = pManager->GetDPIObj()->ScaleInt(out_height);
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

	BOOL UIImage::LoadImage(const TDrawInfo *pDrawInfo, CPaintManagerUI* pManager, HINSTANCE instance)
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

	BOOL UIImage::LoadImage(STRINGorID bitmap, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		CUIFile f;
		if(!f.LoadFile(bitmap, type, instance)) 
			return FALSE;

		return LoadImageFromMemory(f.GetData(), f.GetSize(), mask, width, height, fillcolor, pManager);
	}

	BOOL UIImage::LoadImage(LPCTSTR pStrImage, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
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

	BOOL UIImage::LoadImage(UINT nID, LPCTSTR type, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager, HINSTANCE instance)
	{
		return LoadImage(STRINGorID(nID), type, mask, width, height, fillcolor, pManager, instance);
	}

	void UIImage::AdjustHslImage(bool bHSL, short H, short S, short L)
	{
		if( (bUseHSL == false && !CPaintManagerUI::IsForceHSL()) || bitmap == NULL || pBits == NULL || bitmap->GetHandle() == NULL) 
			return;

		if(pSrcBits == NULL)
		{
			pSrcBits = new BYTE[nWidth * nHeight * 4];
			memcpy(pSrcBits, pBits, nWidth * nHeight * 4);
		}

		if( bHSL == false || (H == 180 && S == 100 && L == 100)) {
			memcpy(pBits, pSrcBits, nWidth * nHeight * 4);
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


	BOOL UIImage::LoadImageFromMemory(const LPBYTE pData, DWORD dwSize, DWORD mask, int width, int height, DWORD fillcolor, CPaintManagerUI* pManager)
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

		bitmap->CreateFromData(pImage, x, y, mask);

		pBits = bitmap->GetBits();
		pSrcBits = NULL;
		nWidth = x;
		nHeight = y;
		bAlpha = bitmap->IsAlpha() == TRUE;
		delay = delay;

		stbi_image_free(pImage);
		return TRUE;
	}

	bool UIImage::LoadGifImageFromMemory(const LPBYTE pData, DWORD dwSize, CStdPtrArray &arrImageInfo)
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
			UIImage* data = UIGlobal::CreateImage();
			data->bitmap->CreateFromData(pFrame, x, y, 0);
			data->pBits = data->bitmap->GetBits();
			data->pSrcBits = NULL;
			data->nWidth = x;
			data->nHeight = y;
			data->bAlpha = data->bitmap->IsAlpha() == TRUE;
			data->delay = delays[f];

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

	//////////////////////////////////////////////////////////////////////////
	//
	//

	void UIRender::DrawBitmap(UIBitmap *pUiBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade, bool hole, bool xtiled, bool ytiled)
	{
// 		CStdRefPtr<UIRender> pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
// 		pRender->CloneFrom(this);
// 		pRender->SelectObject(pUiBitmap);

		CDuiRect rcTemp;
		CDuiRect rcDest;
		if( bAlpha || uFade < 255 ) 
		{
			// middle
			if( !hole ) {
				rcDest.left = rc.left + rcCorners.left;
				rcDest.top = rc.top + rcCorners.top;
				rcDest.right = rc.right - rc.left - rcCorners.left - rcCorners.right;
				rcDest.bottom = rc.bottom - rc.top - rcCorners.top - rcCorners.bottom;
				rcDest.right += rcDest.left;
				rcDest.bottom += rcDest.top;
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					if( !xtiled && !ytiled ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
// 							rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
							rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
// 								AlphaBlend(rcDest.left + lWidth * i, rcDest.top + lHeight * j, 
// 									lDestRight - lDestLeft, lDestBottom - lDestTop, pRender, 
// 									rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, lDrawWidth, lDrawHeight, uFade);
								DrawBitmapAlpha(rcDest.left + lWidth * i, rcDest.top + lHeight * j, 
									lDestRight - lDestLeft, lDestBottom - lDestTop, pUiBitmap, 
									rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, lDrawWidth, lDrawHeight, uFade);
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
// 							AlphaBlend(lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
// 								pRender, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 								lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
							DrawBitmapAlpha(lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
								pUiBitmap, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
// 							AlphaBlend(rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
// 								pRender, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, uFade);         
							DrawBitmapAlpha(rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
								pUiBitmap, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, uFade);  
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
// 						rcCorners.left - rcCorners.right, rcCorners.top, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
						rcCorners.left - rcCorners.right, rcCorners.top, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
// 						rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
						rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
// 						rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
						rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
// 						rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
						rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, uFade);
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
				if( rcTemp.Intersect(rcPaint, rcDest) ) {
					rcDest.right -= rcDest.left;
					rcDest.bottom -= rcDest.top;
// 					AlphaBlend(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 						rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
// 						rcCorners.bottom, uFade);
					DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
						rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
						rcCorners.bottom, uFade);
				}
			}
		}
		else 
		{
			if (rc.right - rc.left == rcBmpPart.right - rcBmpPart.left \
				&& rc.bottom - rc.top == rcBmpPart.bottom - rcBmpPart.top \
				&& rcCorners.left == 0 && rcCorners.right == 0 && rcCorners.top == 0 && rcCorners.bottom == 0)
			{
				if( rcTemp.Intersect(rcPaint, rc) ) {
// 					BitBlt(rcTemp.left, rcTemp.top, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, \
// 						pRender, rcBmpPart.left + rcTemp.left - rc.left, rcBmpPart.top + rcTemp.top - rc.top, SRCCOPY);
					DrawBitmapAlpha(rcTemp.left, rcTemp.top, rcTemp.right - rcTemp.left, rcTemp.bottom - rcTemp.top, \
						pUiBitmap, rcBmpPart.left + rcTemp.left - rc.left, rcBmpPart.top + rcTemp.top - rc.top, \
						rcTemp.GetWidth(), rcTemp.GetHeight(), uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						if( !xtiled && !ytiled ) {
							rcDest.right -= rcDest.left;
							rcDest.bottom -= rcDest.top;
// 							StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 								rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
// 								rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
							DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
								rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
								rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, \
								rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
// 									BitBlt(rcDest.left + lWidth * i, rcDest.top + lHeight * j, \
// 										lDestRight - lDestLeft, lDestBottom - lDestTop, pRender, \
// 										rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, SRCCOPY);
									DrawBitmapAlpha(rcDest.left + lWidth * i, rcDest.top + lHeight * j, \
										lDestRight - lDestLeft, lDestBottom - lDestTop, pUiBitmap, \
										rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
										rcTemp.GetWidth(), rcTemp.GetHeight(), uFade);
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
// 								StretchBlt(lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
// 									pRender, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 									lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
								DrawBitmapAlpha(lDestLeft, rcDest.top, lDestRight - lDestLeft, rcDest.bottom, 
									pUiBitmap, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
									lDrawWidth, rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
// 								StretchBlt(rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
// 									pRender, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
// 									rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, SRCCOPY);  
								DrawBitmapAlpha(rcDest.left, rcDest.top + lHeight * i, rcDest.right, lDestBottom - lDestTop, 
									pUiBitmap, rcBmpPart.left + rcCorners.left, rcBmpPart.top + rcCorners.top, \
									rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, lDrawHeight, uFade);    
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left, rcBmpPart.top, rcCorners.left, rcCorners.top, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
// 							rcCorners.left - rcCorners.right, rcCorners.top, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.top, rcBmpPart.right - rcBmpPart.left - \
							rcCorners.left - rcCorners.right, rcCorners.top, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.right - rcCorners.right, rcBmpPart.top, rcCorners.right, rcCorners.top, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
// 							rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left, rcBmpPart.top + rcCorners.top, rcCorners.left, rcBmpPart.bottom - \
							rcBmpPart.top - rcCorners.top - rcCorners.bottom, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.right - rcCorners.right, rcBmpPart.top + rcCorners.top, rcCorners.right, \
// 							rcBmpPart.bottom - rcBmpPart.top - rcCorners.top - rcCorners.bottom, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left, rcBmpPart.bottom - rcCorners.bottom, rcCorners.left, rcCorners.bottom, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
// 							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.left + rcCorners.left, rcBmpPart.bottom - rcCorners.bottom, \
							rcBmpPart.right - rcBmpPart.left - rcCorners.left - rcCorners.right, rcCorners.bottom, uFade);
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
					if( rcTemp.Intersect(rcPaint, rcDest) ) {
						rcDest.right -= rcDest.left;
						rcDest.bottom -= rcDest.top;
// 						StretchBlt(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pRender, \
// 							rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
// 							rcCorners.bottom, SRCCOPY);
						DrawBitmapAlpha(rcDest.left, rcDest.top, rcDest.right, rcDest.bottom, pUiBitmap, \
							rcBmpPart.right - rcCorners.right, rcBmpPart.bottom - rcCorners.bottom, rcCorners.right, \
							rcCorners.bottom, uFade);
					}
				}
			}
		}
	}

	void UIRender::DrawBackColor(const RECT& rcItem, const SIZE &round, DWORD dwBackColor, DWORD dwBackColor2, DWORD dwBackColor3, bool bVertical)
	{
		if( dwBackColor != 0 )
		{
			if( dwBackColor2 != 0 ) 
			{
				if( dwBackColor3 != 0 ) 
				{
					RECT rc = rcItem;
					rc.bottom = (rc.bottom + rc.top) / 2;
					DrawGradient(rc, dwBackColor, dwBackColor2, bVertical, 8);

					rc.top = rc.bottom;
					rc.bottom = rcItem.bottom;
					DrawGradient(rc, dwBackColor2, dwBackColor3, bVertical, 8);
				}
				else
				{
					DrawGradient(rcItem, dwBackColor, dwBackColor2, bVertical, 16);
				}
			}
			else if( dwBackColor >= 0xFF000000 )
			{
				DrawColor(rcItem, round, dwBackColor);
			}
			else
			{
				DrawColor(rcItem, round, dwBackColor);
			}
		}
	}

	void UIRender::DrawLine(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle)
	{
		DrawLine(rc.left, rc.top, rc.right, rc.bottom, nSize, dwPenColor, nStyle);
	}

	void UIRender::DrawBorder(const RECT &rcItem, int nBorderSize, SIZE cxyBorderRound, RECT rcBorderSize, DWORD dwColor, int nBorderStyle)
	{
		if(nBorderSize > 0)
		{
			if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 )
			{
				DrawRoundRect(rcItem, nBorderSize, cxyBorderRound, dwColor, nBorderStyle);
			}
			else
			{
				DrawRect(rcItem, nBorderSize, dwColor, nBorderStyle);
			}
		}
		else
		{
			RECT rcBorder;

			if(rcBorderSize.left > 0){
				rcBorder		= rcItem;
				rcBorder.right	= rcBorder.left;
				DrawLine(rcBorder,rcBorderSize.left,dwColor, nBorderStyle);
			}
			if(rcBorderSize.top > 0){
				rcBorder		= rcItem;
				rcBorder.bottom	= rcBorder.top;
				DrawLine(rcBorder,rcBorderSize.top, dwColor, nBorderStyle);
			}
			if(rcBorderSize.right > 0){
				rcBorder		= rcItem;
				rcBorder.right -= 1;
				rcBorder.left	= rcBorder.right;
				DrawLine(rcBorder,rcBorderSize.right, dwColor, nBorderStyle);
			}
			if(rcBorderSize.bottom > 0){
				rcBorder		= rcItem;
				rcBorder.bottom -= 1;
				rcBorder.top	= rcBorder.bottom;
				DrawLine(rcBorder,rcBorderSize.bottom, dwColor, nBorderStyle);
			}
		}
	}

	void UIRender::DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle)
	{
		if((uStyle & DT_CALCRECT) == DT_CALCRECT)
		{
			DrawText(rc, pstrText, dwTextColor, iFont, uStyle);

			rc.right += rcTextPadding.left + rcTextPadding.right;
			rc.bottom += rcTextPadding.top + rcTextPadding.bottom;
		}
		else
		{
			rc.left += rcTextPadding.left;
			rc.right -= rcTextPadding.right;
			rc.top += rcTextPadding.top;
			rc.bottom -= rcTextPadding.bottom;

			DrawText(rc, pstrText, dwTextColor, iFont, uStyle);
		}
	}

	void UIRender::DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle, DWORD dwTextBKColor)
	{
		DrawColor(rc, CDuiSize(0,0), dwTextBKColor);
		DrawText(rc, rcTextPadding, pstrText, dwTextColor, iFont, uStyle);
	}
	
	bool UIRender::DrawImageInfo(const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance)
	{
		CPaintManagerUI *pManager = GetManager();
		if( pManager == NULL || pDrawInfo == NULL ) return false;
		if (pDrawInfo->sImageName.IsEmpty()) return false;

		//////////////////////////////////////////////////////////////////////////
		//调整dest

		//如果有定义pDrawInfo->rcDest，调整显示为控件位置的偏移
		CDuiRect rcDest = rcItem;
		if( pDrawInfo->rcDest.left != 0 || pDrawInfo->rcDest.top != 0 || pDrawInfo->rcDest.right != 0 || pDrawInfo->rcDest.bottom != 0 ) 
		{
			rcDest.left = rcItem.left + pDrawInfo->rcDest.left;
			rcDest.top = rcItem.top + pDrawInfo->rcDest.top;
			rcDest.right = rcItem.left + pDrawInfo->rcDest.right;
			rcDest.bottom = rcItem.top + pDrawInfo->rcDest.bottom;
			if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;	
			if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;
		}

		if((pDrawInfo->uAlign & DT_RIGHT) == DT_RIGHT){
			rcDest.left = rcItem.right - (pDrawInfo->rcDest.right - pDrawInfo->rcDest.left);
			rcDest.right = rcItem.right;
		}
		else if((pDrawInfo->uAlign & DT_CENTER) == DT_CENTER){
			rcDest.left = rcItem.left + ((rcItem.right - rcItem.left) - (pDrawInfo->rcDest.right - pDrawInfo->rcDest.left))/2;
			rcDest.right = rcDest.left + (pDrawInfo->rcDest.right - pDrawInfo->rcDest.left);
		}

		if((pDrawInfo->uAlign & DT_BOTTOM) == DT_BOTTOM){
			rcDest.top = rcItem.bottom - (pDrawInfo->rcDest.bottom - pDrawInfo->rcDest.top);
			rcDest.bottom = rcItem.bottom;
		}
		else if((pDrawInfo->uAlign & DT_VCENTER) == DT_VCENTER){
			rcDest.top = rcItem.top + ((rcItem.bottom - rcItem.top) - (pDrawInfo->rcDest.bottom - pDrawInfo->rcDest.top))/2;
			rcDest.bottom = rcDest.top + (pDrawInfo->rcDest.bottom - pDrawInfo->rcDest.top);
		}

// 		::OffsetRect(&rcDest, pDrawInfo->rcPadding.left, pDrawInfo->rcPadding.top);		
// 		::OffsetRect(&rcDest, -pDrawInfo->rcPadding.right, -pDrawInfo->rcPadding.bottom);
		rcDest.Offset(pDrawInfo->rcPadding.left, pDrawInfo->rcPadding.top);		
		rcDest.Offset(-pDrawInfo->rcPadding.right, -pDrawInfo->rcPadding.bottom);
		if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;
		if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;

		CDuiRect rcTemp;
		//if( !::IntersectRect(&rcTemp, &rcDest, &rcItem) ) return true;
		if( !rcTemp.Intersect(rcDest, rcItem) ) return true;
		//if( !::IntersectRect(&rcTemp, &rcDest, &rcPaint) ) return true;
		if( !rcTemp.Intersect(rcDest, rcPaint) ) return true;

		//////////////////////////////////////////////////////////////////////////
		//调整source

		const UIImage* data = pManager->GetImageExX(pDrawInfo, instance);
		if( !data ) return false;    

		RECT rcSource = pDrawInfo->rcSource;
		if( rcSource.left == 0 && rcSource.right == 0 && rcSource.top == 0 && rcSource.bottom == 0 ) 
		{
			rcSource.right = data->nWidth;
			rcSource.bottom = data->nHeight;
		}
		if (rcSource.right > data->nWidth) rcSource.right = data->nWidth;
		if (rcSource.bottom > data->nHeight) rcSource.bottom = data->nHeight;

		DrawBitmap(data->bitmap, rcDest, rcPaint, 
			rcSource, pDrawInfo->rcCorner, pManager->IsLayered() ? true : data->bAlpha, 
			pDrawInfo->uFade, pDrawInfo->bHole, pDrawInfo->bTiledX, pDrawInfo->bTiledY);

		return true;
	}

	//根据xml中设置的图像属性绘制
	bool UIRender::DrawImageString(const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify, HINSTANCE instance)
	{
		if ( !GetManager() ) return false;

		//根据这两参数pStrImage, pStrModify，找到之前已经解析好的pDrawInfo, 找不到就新建一个。
		const TDrawInfo* pDrawInfo = m_pManager->GetDrawInfo(pStrImage, pStrModify);
		return DrawImageInfo(rcItem, rcPaint, pDrawInfo, instance);
	}
	
} // namespace DuiLib

