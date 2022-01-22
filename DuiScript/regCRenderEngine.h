#pragma once

namespace DuiLib
{


class regUIFile
{
	DECL_FACTORY(CUIFile);
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "CUIFile";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(CUIFile);

		REG_METHOD_FUNPR(CUIFile, LPBYTE,	GetData,	() const	);
		REG_METHOD_FUNPR(CUIFile, DWORD,	GetSize,	() const	);
		REG_METHOD_FUNPR(CUIFile, void,		Empty,		()			);

		//REG_METHOD_FUNPR(CUIFile, BOOL, LoadFile, (const STRINGorID &bitmap, LPCTSTR type, HINSTANCE instance)	);
		REG_METHOD_FUNPR(CUIFile, BOOL, LoadFile, (LPCTSTR pStrImage, LPCTSTR type, HINSTANCE instance)			);
		REG_METHOD_FUNPR(CUIFile, BOOL, LoadFile, (UINT nID, LPCTSTR type, HINSTANCE instance)					);
	}	
};

class regUIGlobal
{
public:
	static void Register(asIScriptEngine *engine)
	{
		int r = 0;

// 		REG_GLOBAL_FUNPR(UIRender *,	UIGlobal::CreateRenderTarget,	()	);
// 		REG_GLOBAL_FUNPR(UIFont *,		UIGlobal::CreateFont,			()	);
// 		REG_GLOBAL_FUNPR(UIPen *,		UIGlobal::CreatePen,			()	);
// 		REG_GLOBAL_FUNPR(UIBitmap *,	UIGlobal::CreateBitmap,			()	);
// 		REG_GLOBAL_FUNPR(UIBrush *,		UIGlobal::CreateBrush,			()	);
// 		REG_GLOBAL_FUNPR(UIImage *,		UIGlobal::CreateImage,			()	);
	}
};

class regUIRender
{
	static UIRender *UIRender_Ref_Factory() { return UIGlobal::CreateRenderTarget(); }
public:
	static void Register(asIScriptEngine *engine)
	{
		CStringA classname = "UIRender";

		int r = 0;
		r = engine->RegisterObjectType(classname, 0, asOBJ_REF|asOBJ_NOCOUNT); 
		REG_FACTORY(UIRender);

		REG_METHOD_FUNPR(UIRender, void, Init, (CPaintManagerUI *pManager, HDC hDC) );
		REG_METHOD_FUNPR(UIRender, void, AttachDC, (CPaintManagerUI *pManager, HDC hDC) );

		REG_METHOD_FUNPR(UIRender, CPaintManagerUI *, GetManager, () );
		REG_METHOD_FUNPR(UIRender, HDC,  GetDC, () );
		REG_METHOD_FUNPR(UIRender, UIBitmap *, GetBitmap, () );

		REG_METHOD_FUNPR(UIRender, bool, Resize, (int width, int height) );
		REG_METHOD_FUNPR(UIRender, bool, Resize, (const RECT &rc) );

		//擦除当前画布。
		REG_METHOD_FUNPR(UIRender, void, Clear, () );

		REG_METHOD_FUNPR(UIRender, void, SaveDC, () );
		REG_METHOD_FUNPR(UIRender, void, RestoreDC, () );

		//REG_METHOD_FUNPR(UIRender, UIObject*, SelectObject, (UIObject *pObject) );
		//REG_METHOD_FUNPR(UIRender, void, RestoreObject, (UIObject *pObject) );
		//REG_METHOD_FUNPR(UIRender, void, RestoreDefaultObject, () );

		//BitBlt (bit block transfer)
		REG_METHOD_FUNPR(UIRender, BOOL, BitBlt, (int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop) );

		//stretch BitBlt
		REG_METHOD_FUNPR(UIRender, BOOL, StretchBlt, (int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop) );

		//绘制位图
		REG_METHOD_FUNPR(UIRender, void, DrawBitmap, (HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade, bool hole, bool xtiled, bool ytiled) );

		//画颜色(填充颜色)
		REG_METHOD_FUNPR(UIRender, void, DrawColor, (const RECT& rc, const SIZE &round, DWORD color) );

		//画渐变色
		REG_METHOD_FUNPR(UIRender, void, DrawGradient, (const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) );

		//画线  MoveTo(rc.left, rc.top)，LineTo(rc.right, rc.bottom)
		REG_METHOD_FUNPR(UIRender, void, DrawLine, (const RECT& rc, int nSize, DWORD dwPenColor,int nStyle) );

		//画边框
		REG_METHOD_FUNPR(UIRender, void, DrawRect, (const RECT& rc, int nSize, DWORD dwPenColor,int nStyle) );

		//画圆角边框
		REG_METHOD_FUNPR(UIRender, void, DrawRoundRect, (const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle) );

		//画椭圆
		REG_METHOD_FUNPR(UIRender, void, DrawEllipse, (const RECT& rc, int nSize, DWORD dwPenColor, int nStyle) );

		//绘制文本
		REG_METHOD_FUNPR(UIRender, void, DrawText, (RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) );

		//绘制文字的占用的空间大小
		REG_METHOD_FUNPR(UIRender, SIZE, GetTextSize, (LPCTSTR pstrText, int iFont, UINT uStyle) );

		//////////////////////////////////////////////////////////////////////////

		//画背景色
		REG_METHOD_FUNPR(UIRender, void, DrawBackColor, (const RECT& rc, const SIZE &round, DWORD dwBackColor, DWORD dwBackColor2, DWORD dwBackColor3, bool bVertical) );

		//画边框
		REG_METHOD_FUNPR(UIRender, void, DrawBorder, (const RECT &rcItem, int nBorderSize, SIZE szBorderRound, RECT rcBorderSize, DWORD dwColor, int nBorderStyle) );

		//绘制带背景颜色的文本
		REG_METHOD_FUNPR(UIRender, void, DrawText, (RECT& rc, const RECT &rcTextPadding, LPCTSTR pstrText,DWORD dwTextColor, int iFont, UINT uStyle, DWORD dwTextBKColor) );

		//根据TDrawInfo绘制图像
		REG_METHOD_FUNPR(UIRender, bool, DrawImageInfo, (const RECT& rcItem, const RECT& rcPaint, const TDrawInfo* pDrawInfo, HINSTANCE instance) );

		//根据字符串属性绘制图像
		REG_METHOD_FUNPR(UIRender, bool, DrawImageString, (const RECT& rcItem, const RECT& rcPaint, LPCTSTR pStrImage, LPCTSTR pStrModify, HINSTANCE instance) );
		
	}
};


} //namespace DuiLib