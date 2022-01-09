#ifndef __UIRENDER_GDI_H__
#define __UIRENDER_GDI_H__

#pragma once
#include "UIObject_gdi.h"

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIRender_gdi : public TObjRefImpl<UIRender>
	{
	public:
		UIRender_gdi();
		virtual ~UIRender_gdi();

		virtual void Init(CPaintManagerUI *pManager, HDC hDC = NULL) override;
		virtual void AttachDC(CPaintManagerUI *pManager, HDC hDC) override;

		virtual CPaintManagerUI *GetManager() override;
		virtual HDC GetDC() override;
		virtual UIBitmap *GetBitmap() override;

		virtual bool Resize(int width, int height) override;
		virtual bool Resize(const RECT &rc) override;
		virtual void Clear() override;

		virtual void SaveDC() override;
		virtual void RestoreDC() override;

		virtual UIObject* SelectObject(UIObject *pObject) override;
		virtual void RestoreObject(UIObject *pObject = NULL) override;
		virtual void RestoreDefaultObject() override;

		virtual BOOL BitBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, DWORD dwRop = SRCCOPY ) override;
		virtual BOOL StretchBlt(int x, int y, int nWidth, int nHeight, UIRender *pSrcRender, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc, DWORD dwRop = SRCCOPY) override;

		virtual void DrawBitmap(HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, const RECT& rcBmpPart, const RECT& rcCorners, bool bAlpha, BYTE uFade = 255, bool hole = false, bool xtiled = false, bool ytiled = false);

		virtual void DrawColor(const RECT& rc, const SIZE &round, DWORD color) override;
		virtual void DrawGradient(const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps) override;

		virtual void DrawLine(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawRect(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawRoundRect(const RECT& rc, int nSize, const SIZE &round, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawEllipse(const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID) override;
		virtual void DrawText(RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, int iFont, UINT uStyle) override;
		virtual SIZE GetTextSize(LPCTSTR pstrText, int iFont, UINT uStyle) override;

	protected:
		HDC m_hDC;
		BOOL m_bAttachHDC;
		int m_iSaveDC;

		CStdRefPtr<UIBitmap>	m_curBmp;
		CStdRefPtr<UIPen>		m_curPen;
		CStdRefPtr<UIBrush>		m_curBrush;
		CStdRefPtr<UIFont>		m_curFont;

		CStdRefPtr<UIBitmap>	m_lastBmp;
		CStdRefPtr<UIPen>		m_lastPen;
		CStdRefPtr<UIBrush>		m_lastBrush;
		CStdRefPtr<UIFont>		m_lastFont;

		CStdRefPtr<UIBitmap>	m_defBmp;
		CStdRefPtr<UIPen>		m_defPen;
		CStdRefPtr<UIBrush>		m_defBrush;
		CStdRefPtr<UIFont>		m_defFont;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__
