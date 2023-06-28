#include "StdAfx.h"
#include "UIObject_gdiplus.h"
#include "UIRender_gdiplus.h"

#ifdef DUILIB_WIN32
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
// 	UIFont_gdiplus::UIFont_gdiplus()
// 	{
// 		m_pFont = NULL;
// 		m_hFont = NULL;
// 	}
// 
// 	UIFont_gdiplus::~UIFont_gdiplus()
// 	{
// 		if(m_pFont != NULL) { delete m_pFont; m_pFont = NULL; }
// 		if(m_hFont != NULL) { ::DeleteObject(m_hFont); m_hFont = NULL; }
// 	}
// 
// 	void UIFont_gdiplus::DeleteObject()
// 	{
// 		if(m_pFont != NULL) { delete m_pFont; m_pFont = NULL; }
// 		if(m_hFont != NULL) { ::DeleteObject(m_hFont); m_hFont = NULL; }
// 	}
// 
// 	HFONT UIFont_gdiplus::GetHFont(CPaintManagerUI *pManager)
// 	{
// 		if(m_hFont != NULL) return m_hFont;
// 		if(pManager == NULL) return NULL;
// 		UIRender_gdiplus *pRenderPlus = dynamic_cast<UIRender_gdiplus *>(pManager->Render());
// 		if(!pRenderPlus) return NULL;
// 
// 		//创建gdi字体
// 		Gdiplus::Graphics graphics(pRenderPlus->GetDC());
// 		LOGFONT lf;
// #ifdef _UNICODE
// 		m_pFont->GetLogFontW(&graphics, &lf);
// #else
// 		m_pFont->GetLogFontA(&graphics, &lf);
// #endif
// 		m_hFont = ::CreateFontIndirect(&lf);
// 		return m_hFont;
// 	}
// 
// 	UIFont* UIFont_gdiplus::Clone(CPaintManagerUI *pManager)
// 	{
// 		UIFont_gdiplus *pNewFont = new UIFont_gdiplus;
// 		pNewFont->id			= id;
// 		pNewFont->sFontName		= sFontName;
// 		pNewFont->iSize			= iSize;
// 		pNewFont->bBold			= bBold;
// 		pNewFont->bUnderline	= bUnderline;
// 		pNewFont->bItalic		= bItalic;
// 		pNewFont->bDefault		= bDefault;
// 		pNewFont->bShared		= bShared;
// 
// 		pNewFont->m_pFont = m_pFont->Clone();
// 		return pNewFont;
// 	}
// 
// 	int UIFont_gdiplus::GetHeight(CPaintManagerUI *pManager)
// 	{
// 		if(pManager == NULL || m_pFont == NULL) return 0;
// 		UIRender_gdiplus *pRenderPlus = dynamic_cast<UIRender_gdiplus *>(pManager->Render());
// 		if(!pRenderPlus) return 0;
// 
// 		Gdiplus::StringFormat stringFormat = Gdiplus::StringFormat::GenericTypographic();
// 		Gdiplus::RectF bounds;
// 		Gdiplus::RectF rectF(0,0,9999,9999);
// 		Gdiplus::Graphics graphics(pRenderPlus->GetDC());
// 		graphics.MeasureString(_T("A"), -1, m_pFont, rectF, &stringFormat, &bounds);
// 
// 		// MeasureString存在计算误差，这里加一像素
// 		return (long)bounds.Height + 1;
// 	}
// 
// 	Gdiplus::Font *UIFont_gdiplus::GetGdiPlusFont() const
// 	{
// 		return m_pFont;
// 	}
// 
// 	BOOL UIFont_gdiplus::_buildFont(CPaintManagerUI *pManager)
// 	{
// 		if(sFontName.IsEmpty())
// 			return FALSE;
// 
// 		INT style = Gdiplus::FontStyleRegular;
// 		if(bBold && bItalic) style = Gdiplus::FontStyleBoldItalic;
// 		else if(bBold) style = Gdiplus::FontStyleBold;
// 		else if(bItalic) style = Gdiplus::FontStyleItalic;
// 		if(bUnderline) style |= Gdiplus::FontStyleUnderline;
// 
// 		int xSize = pManager ? pManager->GetDPIObj()->Scale(iSize) : iSize;
// 
// 		UISTRING_CONVERSION;
// 		m_pFont = new Gdiplus::Font(UIT2W(sFontName), (Gdiplus::REAL)xSize, style, Gdiplus::UnitPoint, NULL);
// 
// 		return m_pFont->GetLastStatus() == Gdiplus::Ok;
// 	}
	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPen_gdiplus::UIPen_gdiplus()
	{
		
	}

	UIPen_gdiplus::~UIPen_gdiplus() 
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBrush_gdiplus::UIBrush_gdiplus() 
	{
		
	}

	UIBrush_gdiplus::~UIBrush_gdiplus() 
	{
		
	}
	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIPath_gdiplus::UIPath_gdiplus()
	{
		m_pPath = new Gdiplus::GraphicsPath;
	}

	UIPath_gdiplus::~UIPath_gdiplus()
	{
		delete m_pPath;
	}

	void UIPath_gdiplus::DeleteObject()
	{
		delete m_pPath;
	}

	BOOL UIPath_gdiplus::Beginpath()
	{
		m_pPath->StartFigure();
		return TRUE;
	}

	BOOL UIPath_gdiplus::EndPath()
	{
		m_pPath->CloseFigure();
		return TRUE;
	}

	BOOL UIPath_gdiplus::AbortPath()
	{
		m_pPath->CloseFigure();
		return TRUE;
	}

	BOOL UIPath_gdiplus::AddLine(int x1, int y1, int x2, int y2)
	{
		m_pPath->AddLine(x1, y1, x2, y2);
		return TRUE;
	}

	BOOL UIPath_gdiplus::AddLines(CDuiPoint *points, int count)
	{
		if(count < 2) return FALSE;

		std::vector<Gdiplus::Point> p;
		for (int i = 0; i < count; i++)
		{
			p.emplace_back(Gdiplus::Point(points[i].x, points[i].y));
		}
		return m_pPath->AddLines(&p[0], p.size()) == Gdiplus::Ok;
	}

	Gdiplus::GraphicsPath* UIPath_gdiplus::Getpath()
	{
		return m_pPath;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIBitmap_gdiplus::UIBitmap_gdiplus()
	{
	
	}

	UIBitmap_gdiplus::~UIBitmap_gdiplus()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIImage_gdiplus::UIImage_gdiplus()
	{
		
	}

	UIImage_gdiplus::~UIImage_gdiplus()
	{
		
	}
} // namespace DuiLib
#endif // #ifdef DUILIB_WIN32

