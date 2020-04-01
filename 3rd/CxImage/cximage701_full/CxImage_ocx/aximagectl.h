#if !defined(AFX_AXIMAGECTL_H__4FED1AD2_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_)
#define AFX_AXIMAGECTL_H__4FED1AD2_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// aximagectl.h : Declaration of the AxImage ActiveX Control class.
#include "../cximage/ximage.h"

/////////////////////////////////////////////////////////////////////////////
// AxImage : See aximagectl.cpp for implementation.

class AxImage : public COleControl
{
	DECLARE_DYNCREATE(AxImage)

// Constructor
public:
	AxImage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AxImage)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~AxImage();
	CxImage*	image;

	DECLARE_OLECREATE_EX(AxImage)    // Class factory and guid
	DECLARE_OLETYPELIB(AxImage)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(AxImage)     // Property page IDs
	DECLARE_OLECTLTYPE(AxImage)		// Type name and misc status

	// Subclassed control support
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// Message maps
	//{{AFX_MSG(AxImage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(AxImage)
	BOOL m_border;
	afx_msg void OnBorderChanged();
	BOOL m_stretch;
	afx_msg void OnStretchChanged();
	afx_msg long Load(LPCTSTR FileName, long ImageType);
	afx_msg long Save(LPCTSTR FileName, long ImageType);
	afx_msg long CreateFromHandle(long handle);
	afx_msg long CreateFromBitmap(long bitmap);
	afx_msg OLE_HANDLE MakeBitmap(long hdc);
	afx_msg long Create(long width, long height, long bpp);
	afx_msg void Clear(long val);
	afx_msg void SetStdPalette();
	afx_msg BOOL IsValid();
	afx_msg BOOL Flip();
	afx_msg BOOL Mirror();
	afx_msg BOOL Rotate(float angle);
	afx_msg BOOL RotateLeft();
	afx_msg BOOL RotateRight();
	afx_msg BOOL Rotate180();
	afx_msg BOOL Negative();
	afx_msg BOOL GrayScale();
	afx_msg BOOL Resample(long newx, long newy, long fast);
	afx_msg BOOL DecreaseBpp(long bpp, long errordiffusion);
	afx_msg BOOL IncreaseBpp(long bpp);
	afx_msg BOOL Dither(long method);
	afx_msg BOOL Crop(long left, long top, long right, long bottom);
	afx_msg BOOL Threshold(long level);
	afx_msg BOOL Colorize(long hue, long sat);
	afx_msg BOOL Light(long light, long contrast);
	afx_msg float Mean();
	afx_msg BOOL Dilate(long Ksize);
	afx_msg BOOL Erode(long Ksize);
	afx_msg BOOL Filter(int FAR* kernel, long Ksize, long Kfactor, long Koffset);
	afx_msg long GetNumFrames();
	afx_msg long GetFrame();
	afx_msg void SetFrame(long nFrame);
	afx_msg long GetTransIndex();
	afx_msg OLE_COLOR GetTransColor();
	afx_msg void SetTransIndex(long index);
	afx_msg void SetTransColor(long color);
	afx_msg long GetPaletteSize();
	afx_msg OLE_COLOR* GetPalette();
	afx_msg OLE_COLOR GetPaletteColor(long index);
	afx_msg void SetPalette(long FAR* pal, long ncolors);
	afx_msg void SetPaletteIndex(long index, OLE_COLOR color);
	afx_msg void SetGrayPalette();
	afx_msg BOOL IsGrayScale();
	afx_msg BOOL IsIndexed();
	afx_msg void SwapIndex(long index1, long index2);
	afx_msg void HuePalette(float correction);
	afx_msg BOOL IsInside(long x, long y);
	afx_msg long GetPixelIndex(long x, long y);
	afx_msg OLE_COLOR GetPixelColor(long x, long y);
	afx_msg void SetPixelIndex(long x, long y, long index);
	afx_msg void SetPixelColor(long x, long y, OLE_COLOR color);
	afx_msg long GetWidth();
	afx_msg long GetHeight();
	afx_msg long GetNumColors();
	afx_msg long GetColorType();
	afx_msg long GetBpp();
	afx_msg long GetType();
	afx_msg BSTR GetLastError();
	afx_msg long GetJpegQuality();
	afx_msg void SetJpegQuality(long q);
	afx_msg long GetXDPI();
	afx_msg long GetYDPI();
	afx_msg void SetXDPI(long dpi);
	afx_msg void SetYDPI(long dpi);
	afx_msg OLE_HANDLE CopyToHandle();
	afx_msg long StringToFormat(LPCTSTR extension);
	afx_msg BOOL FloodFill(long xStart, long yStart, OLE_COLOR cFillColor, short nTolerance, short nOpacity, short bSelectFillArea, short nSelectionLevel);
	afx_msg void Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(AxImage)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(AxImage)
	dispidBorder = 1L,
	dispidStretch = 2L,
	dispidLoad = 3L,
	dispidSave = 4L,
	dispidCreateFromHandle = 5L,
	dispidCreateFromBitmap = 6L,
	dispidMakeBitmap = 7L,
	dispidCreate = 8L,
	dispidClear = 9L,
	dispidSetStdPalette = 10L,
	dispidIsValid = 11L,
	dispidFlip = 12L,
	dispidMirror = 13L,
	dispidRotate = 14L,
	dispidRotateLeft = 15L,
	dispidRotateRight = 16L,
	dispidRotate180 = 17L,
	dispidNegative = 18L,
	dispidGrayScale = 19L,
	dispidResample = 20L,
	dispidDecreaseBpp = 21L,
	dispidIncreaseBpp = 22L,
	dispidDither = 23L,
	dispidCrop = 24L,
	dispidThreshold = 25L,
	dispidColorize = 26L,
	dispidLight = 27L,
	dispidMean = 28L,
	dispidDilate = 29L,
	dispidErode = 30L,
	dispidFilter = 31L,
	dispidGetNumFrames = 32L,
	dispidGetFrame = 33L,
	dispidSetFrame = 34L,
	dispidGetTransIndex = 35L,
	dispidGetTransColor = 36L,
	dispidSetTransIndex = 37L,
	dispidSetTransColor = 38L,
	dispidGetPaletteSize = 39L,
	dispidGetPalette = 40L,
	dispidGetPaletteColor = 41L,
	dispidSetPalette = 42L,
	dispidSetPaletteIndex = 43L,
	dispidSetGrayPalette = 44L,
	dispidIsGrayScale = 45L,
	dispidIsIndexed = 46L,
	dispidSwapIndex = 47L,
	dispidHuePalette = 48L,
	dispidIsInside = 49L,
	dispidGetPixelIndex = 50L,
	dispidGetPixelColor = 51L,
	dispidSetPixelIndex = 52L,
	dispidSetPixelColor = 53L,
	dispidGetWidth = 54L,
	dispidGetHeight = 55L,
	dispidGetNumColors = 56L,
	dispidGetColorType = 57L,
	dispidGetBpp = 58L,
	dispidGetType = 59L,
	dispidGetLastError = 60L,
	dispidGetJpegQuality = 61L,
	dispidSetJpegQuality = 62L,
	dispidGetXDPI = 63L,
	dispidGetYDPI = 64L,
	dispidSetXDPI = 65L,
	dispidSetYDPI = 66L,
	dispidCopyToHandle = 67L,
	dispidStringToFormat = 68L,
	dispidFloodFill = 69L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIMAGECTL_H__4FED1AD2_6CF3_11D6_BB83_9FDD9223647D__INCLUDED)
