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
						paintManager->GetDPIObj()->Scale(&rcDest);
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
						if(paintManager != NULL) paintManager->GetDPIObj()->Scale(&rcPadding);
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
						width = paintManager->GetDPIObj()->Scale(width);
					}
					else if( sItem == _T("height") ) {
						height = _ttoi(sValue);
						height = paintManager->GetDPIObj()->Scale(height);
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
				paintManager->GetDPIObj()->Scale(&rcSource);
				paintManager->GetDPIObj()->Scale(&rcCorner);

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

		LOGFONT lf = { 0 };
		::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

		sFontName = lf.lfFaceName;
		iSize = -lf.lfHeight;
		bBold = (lf.lfWeight >= FW_BOLD);
		bUnderline = (lf.lfUnderline == TRUE);
		bItalic = (lf.lfItalic == TRUE);

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
	UIClip::UIClip()
	{
		m_hDC = NULL;
		m_hRgn = NULL;
		m_hOldRgn = NULL;
	}

	UIClip::~UIClip()
	{
		ASSERT(::GetObjectType(m_hDC)==OBJ_DC || ::GetObjectType(m_hDC)==OBJ_MEMDC);
		ASSERT(::GetObjectType(m_hRgn)==OBJ_REGION);
		ASSERT(::GetObjectType(m_hOldRgn)==OBJ_REGION);
		::SelectClipRgn(m_hDC, m_hOldRgn);
		::DeleteObject(m_hOldRgn);
		::DeleteObject(m_hRgn);
	}

	void UIClip::GenerateClip(UIRender *pRender, RECT rc)
	{
		RECT rcClip = { 0 };
		::GetClipBox(pRender->GetDC(), &rcClip);
		m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
		m_hRgn = ::CreateRectRgnIndirect(&rc);
		::ExtSelectClipRgn(pRender->GetDC(), m_hRgn, RGN_AND);
		m_hDC = pRender->GetDC();
		m_rcItem = rc;
		m_szRound.cx = 0;
		m_szRound.cy = 0;
	}

	void UIClip::GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY)
	{
		RECT rcClip = { 0 };
		::GetClipBox(pRender->GetDC(), &rcClip);
		m_hOldRgn = ::CreateRectRgnIndirect(&rcClip);
		m_hRgn = ::CreateRectRgnIndirect(&rc);
		HRGN hRgnItem = NULL;

		if(CPaintManagerUI::GetRenderEngineType() == DuiLib_Render_GdiPlus)
		{
			UIRender_gdiplus *pRenderPlus = dynamic_cast<UIRender_gdiplus *>(pRender);

			Gdiplus::GraphicsPath pPath;
			int x = rcItem.left-1;
			int y = rcItem.top-1;
			int width = rcItem.right- rcItem.left + 1; //修正区域
			int height = rcItem.bottom - rcItem.top + 1; //修正区域

			// 圆角矩形的宽和高，圆角等于宽或高就是圆形了。
			if(roundX > width/2) roundX = width/2;
			if(roundY > height/2) roundY = height/2;

			//左上圆角
			pPath.AddArc(x, y, 2*roundX, 2*roundY, 180, 90); //圆角不乘以2就有锯齿？
			//顶部横线
			pPath.AddLine(x+roundX, y, x+width-roundX, y);
			//右上圆角
			pPath.AddArc(x+width-2*roundX, y, 2*roundX, 2*roundY, 270, 90);
			//右侧竖线
			pPath.AddLine(x+width, y+roundY, x+width, y+height-roundY);
			//右下圆角
			pPath.AddArc(x+width-2*roundX, y+height-2*roundY, 2*roundX, 2*roundY, 0, 90);
			//底部横线
			pPath.AddLine(x+width-roundX, y+height, x+roundX, y+height);
			//左下圆角
			pPath.AddArc(x, y+height-2*roundY, 2*roundX, 2*roundY, 90, 90);
			//左侧横线
			pPath.AddLine(x, y+height-roundY, x, y+roundY);
			pPath.CloseFigure();

			Gdiplus::Region rg(&pPath);
			Gdiplus::Graphics g(pRenderPlus->GetDC());
			hRgnItem = rg.GetHRGN(&g);
		}
		else
		{
			hRgnItem = ::CreateRoundRectRgn(rcItem.left, rcItem.top, rcItem.right + 1, rcItem.bottom + 1, roundX, roundY);
		}

		if(hRgnItem) ::CombineRgn(m_hRgn, m_hRgn, hRgnItem, RGN_AND);
		::ExtSelectClipRgn(pRender->GetDC(), m_hRgn, RGN_AND);
		
		m_hDC = pRender->GetDC();
		m_rcItem = rc;
		m_szRound.cx = roundX;
		m_szRound.cy = roundY;
		if(hRgnItem) ::DeleteObject(hRgnItem);
	}

	void UIClip::UseOldClipBegin(UIRender *pRender)
	{
		::SelectClipRgn(pRender->GetDC(), m_hOldRgn);
	}

	void UIClip::UseOldClipEnd(UIRender *pRender)
	{
		::SelectClipRgn(pRender->GetDC(), m_hRgn);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
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
		RECT rcDest = rcItem;
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

		::OffsetRect(&rcDest, pDrawInfo->rcPadding.left, pDrawInfo->rcPadding.top);		
		::OffsetRect(&rcDest, -pDrawInfo->rcPadding.right, -pDrawInfo->rcPadding.bottom);
		if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;
		if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;

		RECT rcTemp;
		if( !::IntersectRect(&rcTemp, &rcDest, &rcItem) ) return true;
		if( !::IntersectRect(&rcTemp, &rcDest, &rcPaint) ) return true;

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

		DrawBitmap(data->bitmap->GetBitmap(), rcDest, rcPaint, 
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
