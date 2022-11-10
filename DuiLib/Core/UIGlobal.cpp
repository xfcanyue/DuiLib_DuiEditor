#include "StdAfx.h"
#include "UIGlobal.h"

#include "../Render/UIRenderFactory_gdi.h"
#include "../Render/UIRenderFactory_gdiplus.h"
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
			//GetInstance()->GetRenderFactory()->Release();
			GetInstance()->m_renderEngineFactory = NULL;
		}
	}

	UIRenderFactory *UIGlobal::GetRenderFactory()
	{
		if(!m_renderEngineFactory)
		{
			switch (CPaintManagerUI::GetRenderEngineType())
			{
			case DuiLib_Render_Default:
				m_renderEngineFactory = MakeRefPtr<UIRenderFactory>(new UIRenderFactory_gdi);
				break;
			case DuiLib_Render_GdiPlus:
				m_renderEngineFactory = MakeRefPtr<UIRenderFactory>(new UIRenderFactory_gdiplus);
				break;
// 			case DuiLib_Render_Skia:
// 				break;
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

	UIBitmap* UIGlobal::CreateControlBitmap(CControlUI* pControl, DWORD dwFilterColor, CControlUI* pStopControl)
	{
		CPaintManagerUI *pManager = pControl->GetManager();
		if(pManager == NULL) return FALSE;
		if(pManager->GetRoot() == NULL) return FALSE;

		RECT rcControl = pControl->GetPos();
		int cx = rcControl.right - rcControl.left;
		int cy = rcControl.bottom - rcControl.top;

		//建立新的绘图
		CStdRefPtr<UIRender> pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRender->Init(pManager);

		//注意这里必须是root的rc，或者整个client的pos。 总之这个size必须包含了rcControl
		pRender->Resize(pManager->GetRoot()->GetPos()); 

		//在整个区域只绘制了pControl
		pControl->Paint(pRender, rcControl, NULL);

		//再建立一个新的绘图， 把绘制好的pControl的区域BitBlt过来
		CStdRefPtr<UIRender> pRenderClone = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRenderClone->Init(pManager);

		UIBitmap *pBitmapClone = UIGlobal::CreateBitmap();
		pBitmapClone->CreateARGB32Bitmap(pManager->GetPaintDC(), cx, cy, TRUE);
		pRenderClone->SelectObject(pBitmapClone);

		pRenderClone->BitBlt(0, 0, cx, cy, pRender, rcControl.left, rcControl.top, SRCCOPY);
		if (dwFilterColor > 0x00FFFFFF) 
			pRenderClone->DrawColor(CDuiRect(0,0,cx,cy), CDuiSize(0,0), dwFilterColor);

		return pBitmapClone;
	}

	//原理是，整个窗口画一遍，除了pControl自己不画，然后rcWnd的位置截图下来，就是内部窗口的背景图了。
	//这样就可以实现Edit的透明背景了
	UIBitmap* UIGlobal::CreateControlBackBitmap(CControlUI* pControl, const RECT &rcWnd, DWORD dwFilterColor)
	{
		CPaintManagerUI *pManager = pControl->GetManager();
		if(pManager == NULL) return FALSE;
		if(pManager->GetRoot() == NULL) return FALSE;

		CControlUI *pRoot = pManager->GetRoot();
		RECT rcRoot = pRoot->GetPos();

		int cx = rcWnd.right - rcWnd.left;
		int cy = rcWnd.bottom - rcWnd.top;

		//建立新的绘图
		CStdRefPtr<UIRender> pRender = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRender->Init(pManager);
		pRender->Resize(rcRoot); 

		//从root开始绘制，遇到pControl时，停止绘制
		pRoot->Paint(pRender, rcWnd, pControl);

		//再建立一个新的绘图， 把绘制好的rcWnd的区域BitBlt过来
		CStdRefPtr<UIRender> pRenderClone = MakeRefPtr<UIRender>(UIGlobal::CreateRenderTarget());
		pRenderClone->Init(pManager);

		//创建返回的位图，调用方需要释放。
		UIBitmap *pBitmapClone = UIGlobal::CreateBitmap();
		pBitmapClone->CreateARGB32Bitmap(pManager->GetPaintDC(), cx, cy, TRUE);
		pRenderClone->SelectObject(pBitmapClone);
		pRenderClone->BitBlt(0, 0, cx, cy, pRender, rcWnd.left, rcWnd.top, SRCCOPY);

		if (dwFilterColor > 0x00FFFFFF) 
			pRenderClone->DrawColor(CDuiRect(0,0,cx,cy), CDuiSize(0,0), dwFilterColor);

		return pBitmapClone;
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
