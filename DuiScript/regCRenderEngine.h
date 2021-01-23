#pragma once

namespace DuiLib
{

class regCRenderEngine
{
public:
	static bool Register(asIScriptEngine *engine)
	{
		int r = 0;

		REG_GLOBAL_FUNPR(DWORD,  CRenderEngine::AdjustColor, (DWORD dwColor, short H, short S, short L));
// 		static HBITMAP CreateARGB32Bitmap(HDC hDC, int cx, int cy, BYTE** pBits);
// 		static void AdjustImage(bool bUseHSL, TImageInfo* imageInfo, short H, short S, short L);
// 		static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL);
// 
// 		static DWORD CRenderEngine::LoadImage2Memory(const STRINGorID &bitmap, LPCTSTR type,LPBYTE &pData);
// 		static void  CRenderEngine::FreeMemory(LPBYTE &pData);
// 
 		REG_GLOBAL_FUNPR(void, CRenderEngine::FreeImage, (TImageInfo* bitmap, bool bDelete));
 		REG_GLOBAL_FUNPR(TImageInfo*, CRenderEngine::LoadImage, (LPCTSTR pStrImage, LPCTSTR type, DWORD mask, int width, int height, CPaintManagerUI* pManager, HINSTANCE instance));
 		REG_GLOBAL_FUNPR(TImageInfo*, CRenderEngine::LoadImage, (UINT nID, LPCTSTR type, DWORD mask, int width, int height, CPaintManagerUI* pManager, HINSTANCE instance));

// 		static Gdiplus::Image*	GdiplusLoadImage(LPCTSTR pstrPath);
// 		static Gdiplus::Image* GdiplusLoadImage(LPVOID pBuf, size_t dwSize);

 		REG_GLOBAL_FUNPR(bool, CRenderEngine::DrawIconImageString, (HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify));
 		REG_GLOBAL_FUNPR(bool, CRenderEngine::MakeFitIconDest, (const RECT& rcControl,const CDuiSize& szIcon, const CDuiString& sAlign, RECT& rcDest));
 
 		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawText, (HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle, DWORD dwTextBKColor));
 
 		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawImage, (HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade, bool hole, bool xtiled, bool ytiled));

 		REG_GLOBAL_FUNPR(bool, CRenderEngine::DrawImageInfo, (HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance));
 		REG_GLOBAL_FUNPR(bool, CRenderEngine::DrawImageString, (HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify, HINSTANCE instance));
 
 		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawColor, (HDC hDC, const RECT& rc, DWORD color));
 		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawGradient, (HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps));
 
// 		// 以下函数中的颜色参数alpha值无效
		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawLine, (HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor,int nStyle));
		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawRect, (HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor,int nStyle));
		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawRoundRect, (HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor,int nStyle));
		REG_GLOBAL_FUNPR(void, CRenderEngine::DrawText, (HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle));
		//REG_GLOBAL_FUNPR(void,  CRenderEngine::DrawHtmlText, (HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, RECT* pLinks, CDuiString* sLinks, int& nLinkRects, int iFont, UINT uStyle));
		REG_GLOBAL_FUNPR(HBITMAP, CRenderEngine::GenerateBitmap, (CPaintManagerUI* pManager, RECT rc, CControlUI* pStopControl, DWORD dwFilterColor));
		REG_GLOBAL_FUNPR(HBITMAP, CRenderEngine::GenerateBitmap, (CPaintManagerUI* pManager, CControlUI* pControl, RECT rc, DWORD dwFilterColor));
		REG_GLOBAL_FUNPR(SIZE, CRenderEngine::GetTextSize, (HDC hDC, CPaintManagerUI* pManager , LPCTSTR pstrText, int iFont, UINT uStyle));

		//alpha utilities
 		REG_GLOBAL_FUNPR(void, CRenderEngine::CheckAlphaColor, (DWORD& dwColor));

		return r >= 0;
	}
};


} //namespace DuiLib