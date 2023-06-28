#include "StdAfx.h"
#include "UIGlobal.h"

#include "../Render/UIRenderFactory_gdi.h"
#include "../Render/UIRenderFactory_gdiplus.h"
#include "../Render/UIRenderFactory_Cairo.h"
///////////////////////////////////////////////////////////////////////////////////////
namespace DuiLib {

	UIGlobal::UIGlobal()
	{
		
	}

	UIGlobal::~UIGlobal()
	{
		
	}

	UIGlobal* UIGlobal::GetInstance()
	{
		static UIGlobal instance;
		return &instance;
	}

	void UIGlobal::Release()
	{
		if(GetInstance()->GetRenderFactory())
		{
			GetInstance()->m_renderEngineFactory = NULL;
		}
	}

	UIRenderFactory *UIGlobal::GetRenderFactory()
	{
		if(!m_renderEngineFactory)
		{
			switch (CPaintManagerUI::GetRenderEngineType())
			{
#ifdef DUILIB_WIN32
			case DuiLib_Render_Default:
				m_renderEngineFactory = MakeRefPtr<UIRenderFactory>(new UIRenderFactory_gdi);
				break;
			case DuiLib_Render_GdiPlus:
				m_renderEngineFactory = MakeRefPtr<UIRenderFactory>(new UIRenderFactory_gdiplus);
				break;
#endif
#ifdef DUILIB_USE_RENDER_CAIRO
			case DuiLib_Render_Cairo:
				m_renderEngineFactory = MakeRefPtr<UIRenderFactory>(new UIRenderFactory_Cairo);
				break;
#endif
			default:
				ASSERT(FALSE);
				break;
			}
		}
		return m_renderEngineFactory;
	}

	UIRender *UIGlobal::CreateRenderTarget()
	{
		return GetInstance()->GetRenderFactory()->CreateRenderTarget();
	}

	UIFont *UIGlobal::CreateFont()
	{
		return GetInstance()->GetRenderFactory()->CreateFont();
	}

	UIPen *UIGlobal::CreatePen()
	{
		return GetInstance()->GetRenderFactory()->CreatePen();
	}

	UIBitmap *UIGlobal::CreateBitmap()
	{
		return GetInstance()->GetRenderFactory()->CreateBitmap();
	}

	UIBrush *UIGlobal::CreateBrush()
	{
		return GetInstance()->GetRenderFactory()->CreateBrush();
	}

	UIImage *UIGlobal::CreateImage()
	{
		return GetInstance()->GetRenderFactory()->CreateImage();
	}

	//////////////////////////////////////////////////////////////////////////
	static const float OneThird = 1.0f / 3;
	void UIGlobal::RGBtoHSL(DWORD ARGB, float* H, float* S, float* L) 
	{
		const float
			R = (float)GetRValue(ARGB),
			G = (float)GetGValue(ARGB),
			B = (float)GetBValue(ARGB),
			nR = (R<0?0:(R>255?255:R))/255,
			nG = (G<0?0:(G>255?255:G))/255,
			nB = (B<0?0:(B>255?255:B))/255,
			m = min(min(nR,nG),nB),
			M = max(max(nR,nG),nB);
		*L = (m + M)/2;
		if (M==m) *H = *S = 0;
		else {
			const float
				f = (nR==m)?(nG-nB):((nG==m)?(nB-nR):(nR-nG)),
				i = (nR==m)?3.0f:((nG==m)?5.0f:1.0f);
			*H = (i-f/(M-m));
			if (*H>=6) *H-=6;
			*H*=60;
			*S = (2*(*L)<=1)?((M-m)/(M+m)):((M-m)/(2-M-m));
		}
	}

	void UIGlobal::HSLtoRGB(DWORD* ARGB, float H, float S, float L) 
	{
		const float
			q = 2*L<1?L*(1+S):(L+S-L*S),
			p = 2*L-q,
			h = H/360,
			tr = h + OneThird,
			tg = h,
			tb = h - OneThird,
			ntr = tr<0?tr+1:(tr>1?tr-1:tr),
			ntg = tg<0?tg+1:(tg>1?tg-1:tg),
			ntb = tb<0?tb+1:(tb>1?tb-1:tb),
			B = 255*(6*ntr<1?p+(q-p)*6*ntr:(2*ntr<1?q:(3*ntr<2?p+(q-p)*6*(2.0f*OneThird-ntr):p))),
			G = 255*(6*ntg<1?p+(q-p)*6*ntg:(2*ntg<1?q:(3*ntg<2?p+(q-p)*6*(2.0f*OneThird-ntg):p))),
			R = 255*(6*ntb<1?p+(q-p)*6*ntb:(2*ntb<1?q:(3*ntb<2?p+(q-p)*6*(2.0f*OneThird-ntb):p)));
		*ARGB &= 0xFF000000;
		*ARGB |= RGB( (BYTE)(R<0?0:(R>255?255:R)), (BYTE)(G<0?0:(G>255?255:G)), (BYTE)(B<0?0:(B>255?255:B)) );
	}

	DWORD UIGlobal::AdjustHslColor(DWORD dwColor, short H, short S, short L)
	{
		if( H == 180 && S == 100 && L == 100 ) return dwColor;
		float fH, fS, fL;
		float S1 = S / 100.0f;
		float L1 = L / 100.0f;
		RGBtoHSL(dwColor, &fH, &fS, &fL);
		fH += (H - 180);
		fH = fH > 0 ? fH : fH + 360; 
		fS *= S1;
		fL *= L1;
		HSLtoRGB(&dwColor, fH, fS, fL);
		return dwColor;
	}

	void UIGlobal::AdjustHslImage(bool bUseHSL, UIImage *imageInfo, short H, short S, short L)
	{
		if(imageInfo == NULL) return;
		imageInfo->AdjustHslImage(bUseHSL, H, S, L);
	}

	void UIGlobal::CheckAlphaColor(DWORD& dwColor)
	{
		//RestoreAlphaColor认为0x00000000是真正的透明，其它都是GDI绘制导致的
		//所以在GDI绘制中不能用0xFF000000这个颜色值，现在处理是让它变成RGB(0,0,1)
		//RGB(0,0,1)与RGB(0,0,0)很难分出来
		if((0x00FFFFFF & dwColor) == 0)
		{
			dwColor += 1;
		}
	}

} // namespace DuiLib
