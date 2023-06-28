#include "StdAfx.h"

#ifdef DUILIB_WIN32
#include "UIRender_gdi.h"
#include "UIRender_gdiplus.h"
namespace DuiLib {
	//////////////////////////////////////////////////////////////////////////
	//
	//
	UIClipWin32::UIClipWin32()
	{
		m_hDC = NULL;
		m_hRgn = NULL;
		m_hOldRgn = NULL;
	}

	UIClipWin32::~UIClipWin32()
	{
		ASSERT(::GetObjectType(m_hDC)==OBJ_DC || ::GetObjectType(m_hDC)==OBJ_MEMDC);
		ASSERT(::GetObjectType(m_hRgn)==OBJ_REGION);
		ASSERT(::GetObjectType(m_hOldRgn)==OBJ_REGION);
		::SelectClipRgn(m_hDC, m_hOldRgn);
		::DeleteObject(m_hOldRgn);
		::DeleteObject(m_hRgn);
	}

	void UIClipWin32::GenerateClip(UIRender *pRender, RECT rc)
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

	void UIClipWin32::GenerateRoundClip(UIRender *pRender, RECT rc, RECT rcItem, int roundX, int roundY)
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

	void UIClipWin32::UseOldClipBegin(UIRender *pRender)
	{
		::SelectClipRgn(pRender->GetDC(), m_hOldRgn);
	}

	void UIClipWin32::UseOldClipEnd(UIRender *pRender)
	{
		::SelectClipRgn(pRender->GetDC(), m_hRgn);
	}
} // namespace DuiLib
#endif //#ifdef DUILIB_WIN32
