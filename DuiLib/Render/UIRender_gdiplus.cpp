#include "StdAfx.h"
#include "UIRender_gdiplus.h"

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	UIRender_gdiplus::UIRender_gdiplus()
	{
		
	}

	UIRender_gdiplus::~UIRender_gdiplus()
	{
		
	}

// 	UIObject* UIRender_gdiplus::SelectObject(UIObject *pObject)
// 	{
// 		emUIOBJTYPE eType = pObject->ObjectType();
// 		if(eType == OT_FONT)
// 		{
// 			m_lastFont = m_curFont ? m_curFont : m_defFont;
// 			m_curFont = (UIFont *)pObject;
// 			return *(UIFont**)&m_lastFont;
// 		}
// 		else if(eType == OT_PEN)
// 		{
// 			m_lastPen = m_curPen ? m_curPen : m_defPen;
// 			m_curPen = (UIPen *)pObject;
// 			return *(UIPen**)&m_lastPen;
// 		}
// 		else if(eType == OT_BRUSH)
// 		{
// 			m_lastBrush = m_curBrush ? m_curBrush : m_defBrush;
// 			m_curBrush = (UIBrush *)pObject;
// 			return *(UIBrush**)&m_lastBrush;
// 		}
// 		else if(eType == OT_BITMAP)
// 		{
// 			m_lastBmp = m_curBmp ? m_curBmp : m_defBmp;
// 			m_curBmp = (UIBitmap *)pObject;
// 			return *(UIBitmap**)&m_lastBmp;
// 		}
// 
// 		return NULL;
// 	}
// 
// 	void UIRender_gdiplus::RestoreObject(UIObject *pObject)
// 	{
// 		if(pObject == NULL)
// 		{
// 			m_lastFont	= m_lastFont	? m_lastFont	: m_defFont;
// 			m_lastPen	= m_lastPen		? m_lastPen		: m_defPen;
// 			m_lastBrush = m_lastBrush	? m_lastBrush	: m_defBrush;
// 			m_lastBmp	= m_lastBmp		? m_lastBmp		: m_defBmp;
// 
// 			if(m_curFont != m_lastFont)
// 			{
// 				m_curFont = m_lastFont;
// 			}
// 
// 			if(m_curPen != m_lastPen)
// 			{
// 				m_curPen = m_lastPen;
// 			}
// 
// 			if(m_curBrush != m_lastBrush)
// 			{
// 				m_curBrush = m_lastBrush;
// 			}
// 
// 			if(m_curBmp != m_lastBmp)
// 			{
// 				m_curBmp = m_lastBmp;
// 			}
// 		}
// 		else
// 		{
// 			SelectObject(pObject);
// 		}
// 	}
// 
// 	void UIRender_gdiplus::RestoreDefaultObject()
// 	{
// 		m_lastFont = m_defFont;
// 		m_lastPen = m_defPen;
// 		m_lastBrush = m_defBrush;
// 		m_lastBmp = m_defBmp;
// 
// 		if(m_curFont != m_defFont)
// 		{
// 			m_curFont = m_defFont;
// 		}
// 
// 		if(m_curPen != m_defPen)
// 		{
// 			m_curPen = m_defPen;
// 		}
// 
// 		if(m_curBrush != m_defBrush)
// 		{
// 			m_curBrush = m_defBrush;
// 		}
// 
// 		if(m_curBmp != m_defBmp)
// 		{
// 			m_curBmp = m_defBmp;
// 		}
// 	}

	void UIRender_gdiplus::DrawColor(const RECT& rc, const SIZE &round, DWORD color)
	{
		if( color <= 0x00FFFFFF ) return;

		if(round.cx == 0 && round.cy == 0)
		{
			Gdiplus::Graphics graphics(m_hDC);
			Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
			graphics.FillRectangle(&brush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
		}
		else
		{
			Gdiplus::Graphics graphics(m_hDC);
			graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
			//graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
			Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color)>>24)), GetBValue(color), GetGValue(color), GetRValue(color)));
			Gdiplus::GraphicsPath pPath;

			int x = rc.left;
			int y = rc.top;
			int width = rc.right- rc.left; //不需要修正区域
			int height = rc.bottom-rc.top; //不需要修正区域

			int roundX = round.cx;
			int roundY = round.cy;
			// 圆角矩形的半宽和半高
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

			graphics.FillPath(&brush, &pPath);
		}
	}

// 	void UIRender_gdiplus::DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps)
// 	{
// 		Gdiplus::Graphics graphics(m_hDC);
// 		Gdiplus::LinearGradientBrush linGrBrush(Gdiplus::PointF(rc.left,rc.top), Gdiplus::PointF(rc.top,rc.bottom), Gdiplus::Color(255,255,0,0), Gdiplus::Color(255,0,0,255));
// 		graphics.FillRectangle(&linGrBrush, rc.left, rc.top, rc.right, rc.bottom);
// 	}

	void UIRender_gdiplus::DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle/* = PS_SOLID*/)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		graphics.DrawLine(&pen, rc.left, rc.top, rc.right, rc.bottom);
	}

	void UIRender_gdiplus::DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle/* = PS_SOLID*/)
	{
		Gdiplus::Graphics graphics(m_hDC);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		pen.SetAlignment(Gdiplus::PenAlignmentInset);
		graphics.DrawRectangle(&pen, rc.left, rc.top, rc.right - rc.left - 1, rc.bottom - rc.top - 1);
	}

	void UIRender_gdiplus::DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle /*= PS_SOLID*/)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		//graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
		Gdiplus::Pen pen(Gdiplus::Color(dwPenColor), (Gdiplus::REAL)nSize);
		Gdiplus::GraphicsPath pPath;
		
		int x = rc.left;
		int y = rc.top;
		int width = rc.right- rc.left - nSize; //修正区域
		int height = rc.bottom-rc.top - nSize; //修正区域

		int roundX = round.cx;
		int roundY = round.cy;
		// 圆角矩形的半宽和半高
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

		graphics.DrawPath(&pen, &pPath);
	}

	void UIRender_gdiplus::DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor, int nStyle)
	{
		Gdiplus::Graphics graphics(m_hDC);
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pen(dwPenColor, (Gdiplus::REAL)nSize);
		graphics.DrawEllipse(&pen, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	}

	void UIRender_gdiplus::DrawText(RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle)
	{
		CPaintManagerUI *pManager = GetManager();
		HDC hDC = GetDC();

		rc.left += rcTextPadding.left;
		rc.right -= rcTextPadding.right;
		rc.top += rcTextPadding.top;
		rc.bottom -= rcTextPadding.bottom;

		ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
		if( pstrText == NULL || pManager == NULL ) return;

		UIObject *pOldFont = SelectObject(pManager->GetFont(iFont));

		Gdiplus::Graphics graphics( hDC );
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

			//UIFont_gdiplus *pFont = *(UIFont_gdiplus **)&m_curFont;
			//graphics.MeasureString(pstrText, -1, pFont->GetGdiPlusFont(), rectFCalc, &stringFormat, &bounds);
			Gdiplus::Font font(hDC, (HFONT)pManager->GetFont(iFont)->GetHFont(GetManager()));
			graphics.MeasureString(pstrText, -1, &font, rectFCalc, &stringFormat, &bounds);

			// MeasureString存在计算误差，这里加一像素
			rc.bottom = rc.top + (long)bounds.Height + 1;
			rc.right = rc.left + (long)bounds.Width + 1;
		}
		else
		{
			//UIFont_gdiplus *pFont = *(UIFont_gdiplus **)&m_curFont;
			//graphics.DrawString(pstrText, -1, pFont->GetGdiPlusFont(), rectF, &stringFormat, &brush);
			Gdiplus::Font font(hDC, (HFONT)pManager->GetFont(iFont)->GetHFont(GetManager()));
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
				graphics.MeasureString(pcwszDest, -1, &font, rectFCalc, &stringFormat, &bounds);
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
		SelectObject(pOldFont);
	}

} // namespace DuiLib
