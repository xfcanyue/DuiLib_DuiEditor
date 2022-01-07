#ifndef __UIGLOBAL_H__
#define __UIGLOBAL_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API UIGlobal
	{
	public:
		static UIGlobal* GetInstance();
		static void Release();

		UIRenderFactory *GetRenderFactory();
		static UIRender *CreateRenderTarget();
		static UIFont *CreateFont();
		static UIPen *CreatePen();
		static UIBitmap *CreateBitmap();
		static UIBrush *CreateBrush();
		static UIImage *CreateImage();

		//创建控件的图标，比如拖拽时的图片
		static UIBitmap* CreateControlBitmap(CControlUI* pControl, DWORD dwFilterColor = 0, CControlUI* pStopControl = NULL);

		//构造内部窗口的背景画刷
		static UIBitmap* CreateControlBackBitmap(CControlUI* pControl, const RECT &rcWnd, DWORD dwFilterColor);

		//////////////////////////////////////////////////////////////////////////
		//HSL颜色转换
		static void RGBtoHSL(DWORD ARGB, float* H, float* S, float* L);

		//HSL颜色转换
		static void HSLtoRGB(DWORD* ARGB, float H, float S, float L);

		//HSL颜色转换
		static DWORD AdjustHslColor(DWORD dwColor, short H, short S, short L);

		//HSL颜色转换
		static void  AdjustHslImage(bool bUseHSL, UIImage *imageInfo, short H, short S, short L);

		//alpha utilities
		static void CheckAlphaColor(DWORD& dwColor);
	private:
		UIGlobal();
		UIGlobal(const UIGlobal &src);
		const UIGlobal& operator=(const UIGlobal& src);
		~UIGlobal();

	private:
		CStdRefPtr<UIRenderFactory> m_renderEngineFactory;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__
