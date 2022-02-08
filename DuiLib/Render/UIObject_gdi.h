#ifndef __UIFONT_GDI_H__
#define __UIFONT_GDI_H__

#pragma once
#include "IRender.h"

namespace DuiLib { 
	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIFont_gdi : public UIFont
	{
	public:
		UIFont_gdi();

		virtual void DeleteObject() override;

		virtual HFONT GetHFont(CPaintManagerUI *pManager=NULL) override;

		virtual UIFont* Clone(CPaintManagerUI *pManager) override;

		virtual int GetHeight(CPaintManagerUI *pManager) override;

	protected:
		virtual ~UIFont_gdi();

		virtual BOOL _buildFont(CPaintManagerUI *pManager=NULL) override;
	protected:
		HFONT m_hFont;
		TEXTMETRIC tm;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIPen_gdi : public UIPen
	{
	public:
		UIPen_gdi();

		virtual void DeleteObject() override;

		virtual HPEN GetHPen() const override;

		virtual BOOL CreatePen(int nStyle, int nWidth, DWORD dwColor) override;

	protected:
		virtual ~UIPen_gdi();
	private:
		HPEN  m_hPen;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBitmap_gdi : public UIBitmap
	{
	public:
		UIBitmap_gdi();

		virtual void DeleteObject() override;

		virtual BOOL CreateFromHBitmap(HBITMAP hBitmap) override;

		virtual BOOL CreateARGB32Bitmap(HDC hDC, int width, int height) override;

		virtual BOOL CreateCompatibleBitmap(HDC hDC, int width, int height) override;

		virtual HBITMAP GetBitmap() override;
		virtual BYTE* GetBits() override;
		virtual int	GetWidth() override;
		virtual int GetHeight() override;

		virtual void Clear() override;
		virtual void ClearAlpha(const RECT &rc, int alpha = 0) override;

		virtual BOOL SaveFile(LPCTSTR pstrFileName) override;
	protected:
		virtual ~UIBitmap_gdi();
	protected:
		HBITMAP	m_hBitmap;
		BYTE	*m_pBits;
		int		m_nWidth;
		int		m_nHeight;
	};

	/////////////////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIImage_gdi : public UIImage
	{
	public:
		UIImage_gdi();

		virtual void DeleteObject() override;

		virtual BOOL CreateImage(HBITMAP hBitmap, bool bAlpha) override;

		virtual BOOL LoadImage(const TDrawInfo *pDrawInfo, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL) override;
		virtual BOOL LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL)  override;
		virtual BOOL LoadImage(LPCTSTR pStrImage, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL)  override;
		virtual BOOL LoadImage(UINT nID, LPCTSTR type = NULL, DWORD mask = 0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL, HINSTANCE instance = NULL)  override;

		virtual BOOL LoadImageFromMemory(const LPBYTE pData, DWORD dwSize, DWORD mask=0, int width=0, int height=0, DWORD fillcolor=0, CPaintManagerUI* pManager=NULL) override;

		virtual void AdjustHslImage(bool bHSL, short H, short S, short L) override;

		virtual bool LoadGifImageFromFile(LPCTSTR fileName, CStdPtrArray &arrImageInfo) override;

		virtual bool LoadGifImageFromMemory(const LPBYTE pData, DWORD dwSize, CStdPtrArray &arrImageInfo) override;

	protected:
		virtual ~UIImage_gdi();
	};

	//////////////////////////////////////////////////////////////////////////
	//
	class UILIB_API UIBrush_gdi : public UIBrush
	{
	public:
		UIBrush_gdi();

		virtual void DeleteObject() override;

		virtual HBRUSH GetHBrush() const override;

		virtual BOOL CreateFromHBrush(HBRUSH hBrush) override;
		virtual BOOL CreateSolidBrush(DWORD clr) override;
		virtual BOOL CreateBitmapBrush(UIBitmap *bitmap) override;

	protected:
		virtual ~UIBrush_gdi();

	private:
		HBRUSH m_hBrush;
	};

	//////////////////////////////////////////////////////////////////////////
	//
	//
	class UILIB_API UIPath_gdi  : public UIPath
	{
	public:
		UIPath_gdi();
		UIPath_gdi(HDC hDC);

		virtual void DeleteObject() override;

		virtual BOOL Beginpath() override;
		virtual BOOL EndPath() override;
		virtual BOOL AbortPath() override;

		virtual BOOL AddLine(int x1, int y1, int x2, int y2) override;
		virtual BOOL AddLines(CDuiPoint *points, int count) override;

	protected:
		virtual ~UIPath_gdi();

	protected:
		HDC m_hDC;
		POINT m_curPoint;
	};

} // namespace DuiLib

#endif // __UIRENDER_H__
