// demoDoc.h : interface of the CDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////
#define MAX_UNDO_LEVELS 8
#define MAX_SEL_POINTS 8000

#define	WM_USER_NEWIMAGE WM_USER+1
#define WM_USER_PROGRESS WM_USER+2

// <VATI>
#define LEN2D(X,Y) sqrt((float)((X)*(X)+(Y)*(Y)))

#include "ximajpg.h"

class CxImage;
class CDemoDoc : public CDocument
{
protected: // create from serialization only
	CDemoDoc();
	DECLARE_DYNCREATE(CDemoDoc)

// Attributes
public:
//	enum
//	{
//		WM_USER_NEWIMAGE = WM_USER+1
//	};

	CxImage *image;				//main image
	HANDLE	hThread,hProgress;	//elaboration thread

	long	m_MenuCommand;

	POINT m_Sel[MAX_SEL_POINTS]; //Selection
	long m_NumSel;
	int	 m_tool;
	int  m_playanimation;

#ifndef VATI_EXTENSIONS
	LOGFONT m_font;
	CString m_text;
	COLORREF m_color;
#else
	int  m_isRectSel;
#endif

	int32_t m_hr[256]; //histogram
	int32_t m_hg[256];
	int32_t m_hb[256];
	int32_t m_hgray[256];
	int32_t m_hmax;

#if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
	CxImageJPG m_exif;
#endif

// Operations
public:
	void SubmitUndo();
	inline CxImage *GetImage() { return image; }
	inline BOOL GetStretchMode() { return stretchMode; }
	inline BOOL GetSmoothMode() { return m_bSmoothDisplay; }
	inline BOOL GetWaitingClick() { return m_WaitingClick; }
	inline void SetWaitingClick(BOOL b) { m_WaitingClick=b; }
	inline float GetZoomFactor() { return m_ZoomFactor; }
	int ComputePixel(float x, float y, float &x1, float &y1);
	CString FindExtension(const CString& name);
	CString RemoveExtension(const CString& name);
	int FindType(const CString& ext);
	void RegionRotateLeft();
	void RegionRotateRight();
	void PlayNextFrame();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace =TRUE);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Stopwatch(int start0stop1);
	void UpdateStatusBar();
	virtual ~CDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	BOOL stretchMode;
	BOOL m_bSmoothDisplay;
	CxImage *imageUndo[MAX_UNDO_LEVELS];
	int m_UndoLevel;
	float m_ZoomFactor;
	BOOL m_WaitingClick;
	LARGE_INTEGER m_swFreq, m_swStart, m_swStop; //stopwatch
	float m_etime; //elapsed time

	//{{AFX_MSG(CDemoDoc)
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnStretchMode();
	afx_msg void OnUpdateStretchMode(CCmdUI* pCmdUI);
	afx_msg void OnTransformEllipse();
	afx_msg void OnWindowDuplicate();
	afx_msg void OnEditCopy();
	afx_msg void OnCximageFlip();
	afx_msg void OnCximageMirror();
	afx_msg void OnCximageNegative();
	afx_msg void OnCximageGrayscale();
	afx_msg void OnCximageRotate();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI* pCmdUI);
	afx_msg void OnViewNormalviewing11();
	afx_msg void OnUpdateViewNormalviewing11(CCmdUI* pCmdUI);
	afx_msg void OnCximageSettransparency();
	afx_msg void OnUpdateCximageSettransparency(CCmdUI* pCmdUI);
	afx_msg void OnCximageRemovetransparency();
	afx_msg void OnUpdateCximageRemovetransparency(CCmdUI* pCmdUI);
	afx_msg void OnCximageResample();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageFlip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageGrayscale(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageMirror(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageNegative(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageResample(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransformEllipse(CCmdUI* pCmdUI);
	afx_msg void OnCximageDecreasebpp();
	afx_msg void OnCximageIncreasebpp();
	afx_msg void OnCximageOptions();
	afx_msg void OnCximageDither();
	afx_msg void OnUpdateCximageDither(CCmdUI* pCmdUI);
	afx_msg void OnCximageThreshold();
	afx_msg void OnUpdateCximageThreshold(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplitrgb();
	afx_msg void OnUpdateCximageSplitrgb(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplityuv();
	afx_msg void OnUpdateCximageSplityuv(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplithsl();
	afx_msg void OnUpdateCximageSplithsl(CCmdUI* pCmdUI);
	afx_msg void OnCximagePseudocolors();
	afx_msg void OnUpdateCximagePseudocolors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageFiltersColorize(CCmdUI* pCmdUI);
	afx_msg void OnCximageFiltersColorize();
	afx_msg void OnCximageDarken();
	afx_msg void OnUpdateCximageDarken(CCmdUI* pCmdUI);
	afx_msg void OnCximageLighten();
	afx_msg void OnUpdateCximageLighten(CCmdUI* pCmdUI);
	afx_msg void OnCximageContrast();
	afx_msg void OnUpdateCximageContrast(CCmdUI* pCmdUI);
	afx_msg void OnCximageEmboss();
	afx_msg void OnUpdateCximageEmboss(CCmdUI* pCmdUI);
	afx_msg void OnCximageBlur();
	afx_msg void OnUpdateCximageBlur(CCmdUI* pCmdUI);
	afx_msg void OnCximageDilate();
	afx_msg void OnUpdateCximageDilate(CCmdUI* pCmdUI);
	afx_msg void OnCximageEdge();
	afx_msg void OnUpdateCximageEdge(CCmdUI* pCmdUI);
	afx_msg void OnCximageErode();
	afx_msg void OnUpdateCximageErode(CCmdUI* pCmdUI);
	afx_msg void OnCximageSharpen();
	afx_msg void OnUpdateCximageSharpen(CCmdUI* pCmdUI);
	afx_msg void OnCximageSoften();
	afx_msg void OnUpdateCximageSoften(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageCrop(CCmdUI* pCmdUI);
	afx_msg void OnCximageCrop();
	afx_msg void OnUpdateCximageRemovealphachannel(CCmdUI* pCmdUI);
	afx_msg void OnCximageRemovealphachannel();
	afx_msg void OnCximageOpacity();
	afx_msg void OnCximageInvetalpha();
	afx_msg void OnUpdateCximageInvetalpha(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphapalettetoggle();
	afx_msg void OnUpdateCximageAlphapalettetoggle(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphastrip();
	afx_msg void OnUpdateCximageAlphastrip(CCmdUI* pCmdUI);
	afx_msg void OnCximageRotatel();
	afx_msg void OnUpdateCximageRotatel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageRotater(CCmdUI* pCmdUI);
	afx_msg void OnCximageRotater();
	afx_msg void OnUpdateCximageGamma(CCmdUI* pCmdUI);
	afx_msg void OnCximageGamma();
	afx_msg void OnUpdateCximageMedian(CCmdUI* pCmdUI);
	afx_msg void OnCximageMedian();
	afx_msg void OnCximageAddnoise();
	afx_msg void OnUpdateCximageAddnoise(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewToolsMove(CCmdUI* pCmdUI);
	afx_msg void OnViewToolsMove();
	afx_msg void OnViewToolsSelect();
	afx_msg void OnUpdateViewToolsSelect(CCmdUI* pCmdUI);
	afx_msg void OnViewToolsZoom();
	afx_msg void OnUpdateViewToolsZoom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPalette(CCmdUI* pCmdUI);
	afx_msg void OnViewPalette();
	afx_msg void OnCximageCombine();
	afx_msg void OnUpdateCximageCombine(CCmdUI* pCmdUI);
	afx_msg void OnCximageFft();
	afx_msg void OnUpdateCximageFft(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplityiq();
	afx_msg void OnUpdateCximageSplityiq(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplitxyz();
	afx_msg void OnUpdateCximageSplitxyz(CCmdUI* pCmdUI);
	afx_msg void OnCximageRepair();
	afx_msg void OnUpdateCximageRepair(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageAlphachannelSplit(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphachannelSplit();
	afx_msg void OnViewToolsText();
	afx_msg void OnUpdateViewToolsText(CCmdUI* pCmdUI);
	afx_msg void OnCximageSplitcmyk();
	afx_msg void OnUpdateCximageSplitcmyk(CCmdUI* pCmdUI);
	afx_msg void OnCximageAlphaCreate();
	afx_msg void OnCximageFiltersLog();
	afx_msg void OnUpdateCximageFiltersLog(CCmdUI* pCmdUI);
	afx_msg void OnCximageFiltersRoot();
	afx_msg void OnUpdateCximageFiltersRoot(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramEqualize();
	afx_msg void OnUpdateCximageHistogramEqualize(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramNormalize();
	afx_msg void OnUpdateCximageHistogramNormalize(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramStretch();
	afx_msg void OnUpdateCximageHistogramStretch(CCmdUI* pCmdUI);
	afx_msg void OnCximageGaussian3x3();
	afx_msg void OnUpdateCximageGaussian3x3(CCmdUI* pCmdUI);
	afx_msg void OnCximageGaussian5x5();
	afx_msg void OnUpdateCximageGaussian5x5(CCmdUI* pCmdUI);
	afx_msg void OnCximageContour();
	afx_msg void OnUpdateCximageContour(CCmdUI* pCmdUI);
	afx_msg void OnCximageLesscontrast();
	afx_msg void OnUpdateCximageLesscontrast(CCmdUI* pCmdUI);
	afx_msg void OnCximageJitter();
	afx_msg void OnUpdateCximageJitter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowDuplicate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiltersMix(CCmdUI* pCmdUI);
	afx_msg void OnFiltersMix();
	afx_msg void OnCximageCircletransformCylinder();
	afx_msg void OnUpdateCximageCircletransformCylinder(CCmdUI* pCmdUI);
	afx_msg void OnCximageCircletransformPinch();
	afx_msg void OnUpdateCximageCircletransformPinch(CCmdUI* pCmdUI);
	afx_msg void OnCximageCircletransformPunch();
	afx_msg void OnUpdateCximageCircletransformPunch(CCmdUI* pCmdUI);
	afx_msg void OnCximageCircletransformSwirl();
	afx_msg void OnUpdateCximageCircletransformSwirl(CCmdUI* pCmdUI);
	afx_msg void OnCximageCircletransformBathroom();
	afx_msg void OnUpdateCximageCircletransformBathroom(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramStretch1();
	afx_msg void OnCximageHistogramStretch2();
	afx_msg void OnUpdateCximageHistogramStretch1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageHistogramStretch2(CCmdUI* pCmdUI);
	afx_msg void OnFiltersNonlinearEdge();
	afx_msg void OnUpdateFiltersNonlinearEdge(CCmdUI* pCmdUI);
	afx_msg void OnCximageSkew();
	afx_msg void OnUpdateCximageSkew(CCmdUI* pCmdUI);
	afx_msg void OnViewToolsTracker();
	afx_msg void OnUpdateViewToolsTracker(CCmdUI* pCmdUI);
	afx_msg void OnJpegcompression();
	afx_msg void OnUpdateJpegcompression(CCmdUI* pCmdUI);
	afx_msg void OnViewSmooth();
	afx_msg void OnUpdateViewSmooth(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFiltersDataext(CCmdUI* pCmdUI);
	afx_msg void OnFiltersDataext();
	afx_msg void OnUpdateCximageUnsharpmask(CCmdUI* pCmdUI);
	afx_msg void OnCximageUnsharpmask();
	afx_msg void OnCximageTextblur();
	afx_msg void OnUpdateCximageTextblur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageRedeyeremove(CCmdUI* pCmdUI);
	afx_msg void OnCximageRedeyeremove();
	afx_msg void OnCximageBlurselborder();
	afx_msg void OnUpdateCximageBlurselborder(CCmdUI* pCmdUI);
	afx_msg void OnCximageSelectiveblur();
	afx_msg void OnUpdateCximageSelectiveblur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCximageGettransparencymask(CCmdUI* pCmdUI);
	afx_msg void OnCximageGettransparencymask();
	afx_msg void OnColorsCountcolors();
	afx_msg void OnUpdateColorsCountcolors(CCmdUI* pCmdUI);
	afx_msg void OnFiltersLinearCustom();
	afx_msg void OnUpdateFiltersLinearCustom(CCmdUI* pCmdUI);
	afx_msg void OnCximageCanvassize();
	afx_msg void OnUpdateCximageCanvassize(CCmdUI* pCmdUI);
	afx_msg void OnViewToolsFloodfill();
	afx_msg void OnUpdateViewToolsFloodfill(CCmdUI* pCmdUI);
	afx_msg void OnCximageRemoveselection();
	afx_msg void OnUpdateCximageRemoveselection(CCmdUI* pCmdUI);
	afx_msg void OnColorsMoresaturationhsl();
	afx_msg void OnUpdateColorsMoresaturationhsl(CCmdUI* pCmdUI);
	afx_msg void OnColorsMoresaturationyuv();
	afx_msg void OnUpdateColorsMoresaturationyuv(CCmdUI* pCmdUI);
	afx_msg void OnColorsLesssaturation();
	afx_msg void OnUpdateColorsLesssaturation(CCmdUI* pCmdUI);
	afx_msg void OnColorsHistogramFullsaturation();
	afx_msg void OnUpdateColorsHistogramFullsaturation(CCmdUI* pCmdUI);
	afx_msg void OnColorsHistogramHalfsaturation();
	afx_msg void OnUpdateColorsHistogramHalfsaturation(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramStretcht0();
	afx_msg void OnUpdateCximageHistogramStretcht0(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramStretcht1();
	afx_msg void OnUpdateCximageHistogramStretcht1(CCmdUI* pCmdUI);
	afx_msg void OnCximageHistogramStretcht2();
	afx_msg void OnUpdateCximageHistogramStretcht2(CCmdUI* pCmdUI);
	afx_msg void OnColorsAdaptivethreshold();
	afx_msg void OnUpdateColorsAdaptivethreshold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPreviousframe(CCmdUI* pCmdUI);
	afx_msg void OnViewPreviousframe();
	afx_msg void OnUpdateViewNextframe(CCmdUI* pCmdUI);
	afx_msg void OnViewNextframe();
	afx_msg void OnUpdateViewPlayanimation(CCmdUI* pCmdUI);
	afx_msg void OnViewPlayanimation();
	afx_msg void OnUpdateFiltersAddshadow(CCmdUI* pCmdUI);
	afx_msg void OnFiltersAddshadow();
	afx_msg void OnCximageTrace();
	afx_msg void OnUpdateCximageTrace(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
