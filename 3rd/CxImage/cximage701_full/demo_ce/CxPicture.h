

#if !defined(AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_)
#define AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
//#include <process.h>
#include "../cximage/ximage.h"




class CxPicture : public CStatic
{
public:

	static enum EImageSize{ OriginalSize, FitControl};

struct TFrame    // structure that keeps a single frame info
{
	CxImage* m_pPicture;  // pointer to the interface used for drawing
	SIZE     m_frameSize;
	SIZE     m_frameOffset;
	UINT     m_nDelay;     // delay (in 1/100s of a second)
	UINT     m_nDisposal;  // disposal method
};


public:
	BOOL GetPaintRect(RECT *lpRect);
	BOOL SetPaintRect(const RECT *lpRect);
	CxPicture();
	virtual ~CxPicture();
	void Stop();   // stops animation
	void UnLoad(); // stops animation plus releases all resources



	BOOL	IsGIF() const;
	BOOL	IsPlaying() const;
	BOOL	IsAnimatedGIF() const;
	SIZE	GetSize() const;
	int		GetFrameCount() const;
	COLORREF GetBkColor() const;


	BOOL	SetBitmap(HBITMAP hBitmap, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(UINT nIDResource, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(LPCTSTR lpszResName, EImageSize Emode = OriginalSize);
	BOOL	SetBitmap(const CByteArray& ImgBuffer, CString csFName, EImageSize Emode = OriginalSize);

	void	SetOriginalSize();



protected:
	BOOL			m_bNotifyParent;
	BOOL			m_bTransparentBk;
	int				m_nSizeMode;
	UINT			m_nFrameCount;
	RECT			m_PaintRect;
	SIZE			m_PictureSize;
	COLORREF		m_clrBackground;
	UINT			m_nCurrFrame;
	UINT			m_nDataSize;
	UINT			m_nCurrOffset;
	UINT			m_nGlobalCTSize;
	BOOL			m_bIsGIF;
	BOOL			m_bIsPlaying;
	volatile BOOL	m_bExitThread;
	BOOL			m_bIsInitialized;
	HDC				m_hMemDC;

	HDC				m_hDispMemDC;
	HBITMAP			m_hDispMemBM;
	HBITMAP			m_hDispOldBM;

	HBITMAP			m_hBitmap;
	HBITMAP			m_hOldBitmap;
	CWinThread*		m_pThread;
	HANDLE			m_hExitEvent;
	CxImage*		m_pPicture;
	CxMemFile		m_MemFile;

	// FILL ATTRIBUTES
	CBrush*			m_pBrush;
	COLORREF		m_rgbBkgnd;
	COLORREF		m_crHiColor;
	COLORREF		m_crLoColor;

	// TEXT ATTRIBUTES
	COLORREF		m_rgbText;
	BOOL			m_bAutoWrapping;
	CString			m_strText;

	// FONT ATTRIBUTES
	BOOL			m_bAutoAdjustFont;
	BOOL			m_bFont3d;
	LOGFONT			m_DefLogFont;
	LOGFONT			m_lf;
	CFont			m_font;
	int				m_nFontSize;
	int				m_nFontSizeVar;
	CString			m_csFontFamilly;

	

	std::vector<TFrame> m_arrFrames;

	int		FindType(const CString& ext);
	BOOL	DrawBitmap();
	BOOL	Load(int nImgType);
	void	ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);


	BOOL	PrepareDC(int nWidth, int nHeight);
	BOOL	RedrawWindow();


	// Generated message map functions
	//{{AFX_MSG(CxPicture)
	afx_msg void	OnDestroy();
	afx_msg void	OnPaint();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_)
