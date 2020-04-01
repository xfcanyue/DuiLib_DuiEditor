// demo.h : main header file for the DEMO application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// just for including CXTEXTINFO type declaration:
#include "ximage.h"

/////////////////////////////////////////////////////////////////////////////
// CDemoApp:
// See demo.cpp for the implementation of this class
//

class CDemoApp : public CWinApp
{
public:
	CDemoApp();

	// Attributes
	int m_nDocCount;	//number of documents
	CMultiDocTemplate *demoTemplate;

	int nDocType;
	UINT	m_CF;

	float m_optJpegQuality;
	int m_optJpegOptions;
	int m_optRawOptions;

#ifdef VATI_EXTENSIONS
   // for text placement
	CxImage::CXTEXTINFO  m_text;
#endif

	RGBQUAD m_FloodColor;
	BYTE	m_FloodTolerance;
	BYTE	m_FloodOpacity;
	BYTE	m_FloodSelect;

	typedef struct FiltersParams_t {
		int32_t	Kernel5x5[25];
		int32_t	Kernel3x3[9];
		long	kSize;
		long	kBias;
		long	kDivisor;

		float	RotateAngle;
		long	RotateMethod;
		long	RotateOverflow;
		BYTE	RotateKeepsize;

		BYTE	ResampleSizemode;
		float	ResampleFactor;
		long	ResampleW;
		long	ResampleH;
		BYTE	ResampleKeepRatio;
		BYTE	ResampleMethod;

		long	SkewPivotX;
		long	SkewPivotY;
		float	SkewSlopeX;
		float	SkewSlopeY;
		float	SkewX;
		float	SkewY;
		BYTE	SkewInterp;

		BYTE	DitherMethod;

		BYTE	ThreshLevel;
		BYTE	ThreshPreserveColors;

		BYTE	ColorMode;
		RGBQUAD	ColorHSL;
		long	ColorRed;
		long	ColorGreen;
		long	ColorBlue;
		BYTE	ColorSolarLevel;
		BYTE	ColorSolarLink;

		float	GammaLevel;
		float	GammaR;
		float	GammaG;
		float	GammaB;
		BYTE	GammaLink;

		BYTE	IncBppBPP;
		BYTE	DecBppBPP;
		BYTE	DecBppPalMethod;
		BYTE	DecBppErrDiff;
		BYTE	DecBppLimitColors;
		long	DecBppMaxColors;

		BYTE	CanvasMode;
		long	CanvasW;
		long	CanvasH;
		long	CanvasLeft;
		long	CanvasRight;
		long	CanvasTop;
		long	CanvasBottom;
		BYTE	CanvasCenterH;
		BYTE	CanvasCenterV;
		BYTE	CanvasKeepRatio;
		BYTE	CanvasUseImageBkg;
		COLORREF CanvasBkg;

		long	ShadowX;
		long	ShadowY;
		BYTE	ShadowR;
		COLORREF ShadowColor;
		COLORREF ShadowBkg;
		BYTE	ShadowIntensity;
		BYTE	ShadowRelative;

	} FiltersParams;

	FiltersParams m_Filters;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Operations
	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetDescFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	BOOL GetWritableType(int nDocType);
	inline UINT	  GetCF() { return m_CF; }

// Implementation

	//{{AFX_MSG(CDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnImageFromBitmap();
	afx_msg void OnCximageLoadjpegresource();
	afx_msg void OnCximageLoadiconresource();
	afx_msg void OnCximageLoadgifresource();
	afx_msg void OnCximageLoadpngresource();
	afx_msg void OnCximageLoadtifresource();
	afx_msg void OnWindowCloseall();
	afx_msg void OnCximageDemosavemultipagetiff();
	afx_msg void OnCximageDemosaveanimatedgif();
	afx_msg void OnCximageDownloadfromurl();
	afx_msg void OnCximageDemocreatefromarray();
	afx_msg void OnCximageDemosavemultipageicon();
	afx_msg void OnCximageOptions();
	afx_msg void OnHelpCximageonlineWwwxdpit();
	afx_msg void OnHelpCximageonlineWwwthecodeprojectcom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDemoApp theApp;

struct DocType
{
public:
	int nID;
	BOOL bRead;
	BOOL bWrite;
	const TCHAR* description;
	const TCHAR* ext;
};

struct DlgDataExtInfo
{
	float	fYmin;
	float	fYmax;
	float	fXmin;
	float	fXmax;
	BYTE	nThresh;
	COLORREF cColor;
	bool	bMinmax;
	bool	bAvg;
	bool	bDetect;
	bool	bLogXaxis;
	bool	bLogYaxis;
};

/////////////////////////////////////////////////////////////////////////////
