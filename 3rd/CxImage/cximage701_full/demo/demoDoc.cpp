// demoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "demo.h"

#include "MainFrm.h"
#include "demoDoc.h"
#include "demoView.h"
#include "DlgRotate.h"
#include "DlgResample.h"
#include "DlgDecBpp.h"
#include "DlgIncBpp.h"
#include "DlgOptions.h"
#include "DlgDither.h"
#include "DlgThreshold.h"
#include "DlgColorize.h"
#include "Quantize.h"
#include "DlgOpacity.h"
#include "DlgGamma.h"
#include "DlgPalette.h"
#include "DlgCombine.h"
#include "DlgFFT.h"
#include "DlgRepair.h"
#include "DlgText.h"
#include "DlgMix.h"
#include "DlgSkew.h"
#include "DlgJpeg.h"
#include "DlgDataExt.h"
#include "DlgCustomFilter.h"
#include "DlgExpand.h"
#include "DlgFloodFill.h"
#include "DlgShadow.h"

#include "ximage.h"
#include <math.h>
#include <process.h>

extern DlgDataExtInfo dlgDataExtInfos;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_STRETCH_MODE, OnStretchMode)
	ON_UPDATE_COMMAND_UI(ID_STRETCH_MODE, OnUpdateStretchMode)
	ON_COMMAND(ID_TRANSFORM_ELLIPSE, OnTransformEllipse)
	ON_COMMAND(ID_WINDOW_DUPLICATE, OnWindowDuplicate)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_CXIMAGE_FLIP, OnCximageFlip)
	ON_COMMAND(ID_CXIMAGE_MIRROR, OnCximageMirror)
	ON_COMMAND(ID_CXIMAGE_NEGATIVE, OnCximageNegative)
	ON_COMMAND(ID_CXIMAGE_GRAYSCALE, OnCximageGrayscale)
	ON_COMMAND(ID_CXIMAGE_ROTATE, OnCximageRotate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_COMMAND(ID_VIEW_NORMALVIEWING11, OnViewNormalviewing11)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NORMALVIEWING11, OnUpdateViewNormalviewing11)
	ON_COMMAND(ID_CXIMAGE_SETTRANSPARENCY, OnCximageSettransparency)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SETTRANSPARENCY, OnUpdateCximageSettransparency)
	ON_COMMAND(ID_CXIMAGE_REMOVETRANSPARENCY, OnCximageRemovetransparency)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REMOVETRANSPARENCY, OnUpdateCximageRemovetransparency)
	ON_COMMAND(ID_CXIMAGE_RESAMPLE, OnCximageResample)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_FLIP, OnUpdateCximageFlip)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GRAYSCALE, OnUpdateCximageGrayscale)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_MIRROR, OnUpdateCximageMirror)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_NEGATIVE, OnUpdateCximageNegative)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_RESAMPLE, OnUpdateCximageResample)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATE, OnUpdateCximageRotate)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORM_ELLIPSE, OnUpdateTransformEllipse)
	ON_COMMAND(ID_CXIMAGE_DECREASEBPP, OnCximageDecreasebpp)
	ON_COMMAND(ID_CXIMAGE_INCREASEBPP, OnCximageIncreasebpp)
	ON_COMMAND(ID_CXIMAGE_OPTIONS, OnCximageOptions)
	ON_COMMAND(ID_CXIMAGE_DITHER, OnCximageDither)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DITHER, OnUpdateCximageDither)
	ON_COMMAND(ID_CXIMAGE_THRESHOLD, OnCximageThreshold)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_THRESHOLD, OnUpdateCximageThreshold)
	ON_COMMAND(ID_CXIMAGE_SPLITRGB, OnCximageSplitrgb)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITRGB, OnUpdateCximageSplitrgb)
	ON_COMMAND(ID_CXIMAGE_SPLITYUV, OnCximageSplityuv)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITYUV, OnUpdateCximageSplityuv)
	ON_COMMAND(ID_CXIMAGE_SPLITHSL, OnCximageSplithsl)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITHSL, OnUpdateCximageSplithsl)
	ON_COMMAND(ID_CXIMAGE_PSEUDOCOLORS, OnCximagePseudocolors)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_PSEUDOCOLORS, OnUpdateCximagePseudocolors)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_COLORIZE, OnUpdateCximageFiltersColorize)
	ON_COMMAND(ID_CXIMAGE_COLORIZE, OnCximageFiltersColorize)
	ON_COMMAND(ID_CXIMAGE_DARKEN, OnCximageDarken)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DARKEN, OnUpdateCximageDarken)
	ON_COMMAND(ID_CXIMAGE_LIGHTEN, OnCximageLighten)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_LIGHTEN, OnUpdateCximageLighten)
	ON_COMMAND(ID_CXIMAGE_CONTRAST, OnCximageContrast)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CONTRAST, OnUpdateCximageContrast)
	ON_COMMAND(ID_CXIMAGE_EMBOSS, OnCximageEmboss)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_EMBOSS, OnUpdateCximageEmboss)
	ON_COMMAND(ID_CXIMAGE_BLUR, OnCximageBlur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_BLUR, OnUpdateCximageBlur)
	ON_COMMAND(ID_CXIMAGE_DILATE, OnCximageDilate)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_DILATE, OnUpdateCximageDilate)
	ON_COMMAND(ID_CXIMAGE_EDGE, OnCximageEdge)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_EDGE, OnUpdateCximageEdge)
	ON_COMMAND(ID_CXIMAGE_ERODE, OnCximageErode)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ERODE, OnUpdateCximageErode)
	ON_COMMAND(ID_CXIMAGE_SHARPEN, OnCximageSharpen)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SHARPEN, OnUpdateCximageSharpen)
	ON_COMMAND(ID_CXIMAGE_SOFTEN, OnCximageSoften)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SOFTEN, OnUpdateCximageSoften)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CROP, OnUpdateCximageCrop)
	ON_COMMAND(ID_CXIMAGE_CROP, OnCximageCrop)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REMOVEALPHACHANNEL, OnUpdateCximageRemovealphachannel)
	ON_COMMAND(ID_CXIMAGE_REMOVEALPHACHANNEL, OnCximageRemovealphachannel)
	ON_COMMAND(ID_CXIMAGE_OPACITY, OnCximageOpacity)
	ON_COMMAND(ID_CXIMAGE_INVETALPHA, OnCximageInvetalpha)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_INVETALPHA, OnUpdateCximageInvetalpha)
	ON_COMMAND(ID_CXIMAGE_ALPHAPALETTETOGGLE, OnCximageAlphapalettetoggle)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ALPHAPALETTETOGGLE, OnUpdateCximageAlphapalettetoggle)
	ON_COMMAND(ID_CXIMAGE_ALPHASTRIP, OnCximageAlphastrip)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ALPHASTRIP, OnUpdateCximageAlphastrip)
	ON_COMMAND(ID_CXIMAGE_ROTATEL, OnCximageRotatel)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATEL, OnUpdateCximageRotatel)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ROTATER, OnUpdateCximageRotater)
	ON_COMMAND(ID_CXIMAGE_ROTATER, OnCximageRotater)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GAMMA, OnUpdateCximageGamma)
	ON_COMMAND(ID_CXIMAGE_GAMMA, OnCximageGamma)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_MEDIAN, OnUpdateCximageMedian)
	ON_COMMAND(ID_CXIMAGE_MEDIAN, OnCximageMedian)
	ON_COMMAND(ID_CXIMAGE_ADDNOISE, OnCximageAddnoise)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ADDNOISE, OnUpdateCximageAddnoise)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_MOVE, OnUpdateViewToolsMove)
	ON_COMMAND(ID_VIEW_TOOLS_MOVE, OnViewToolsMove)
	ON_COMMAND(ID_VIEW_TOOLS_SELECT, OnViewToolsSelect)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_SELECT, OnUpdateViewToolsSelect)
	ON_COMMAND(ID_VIEW_TOOLS_ZOOM, OnViewToolsZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_ZOOM, OnUpdateViewToolsZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PALETTE, OnUpdateViewPalette)
	ON_COMMAND(ID_VIEW_PALETTE, OnViewPalette)
	ON_COMMAND(ID_CXIMAGE_COMBINE, OnCximageCombine)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_COMBINE, OnUpdateCximageCombine)
	ON_COMMAND(ID_CXIMAGE_FFT, OnCximageFft)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_FFT, OnUpdateCximageFft)
	ON_COMMAND(ID_CXIMAGE_SPLITYIQ, OnCximageSplityiq)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITYIQ, OnUpdateCximageSplityiq)
	ON_COMMAND(ID_CXIMAGE_SPLITXYZ, OnCximageSplitxyz)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITXYZ, OnUpdateCximageSplitxyz)
	ON_COMMAND(ID_CXIMAGE_REPAIR, OnCximageRepair)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REPAIR, OnUpdateCximageRepair)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_ALPHACHANNEL_SPLIT, OnUpdateCximageAlphachannelSplit)
	ON_COMMAND(ID_CXIMAGE_ALPHACHANNEL_SPLIT, OnCximageAlphachannelSplit)
	ON_COMMAND(ID_VIEW_TOOLS_TEXT, OnViewToolsText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_TEXT, OnUpdateViewToolsText)
	ON_COMMAND(ID_CXIMAGE_SPLITCMYK, OnCximageSplitcmyk)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SPLITCMYK, OnUpdateCximageSplitcmyk)
	ON_COMMAND(ID_CXIMAGE_ALPHACREATE, OnCximageAlphaCreate)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_LOG, OnCximageFiltersLog)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_LOG, OnUpdateCximageFiltersLog)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_ROOT, OnCximageFiltersRoot)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_ROOT, OnUpdateCximageFiltersRoot)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_EQUALIZE, OnCximageHistogramEqualize)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_EQUALIZE, OnUpdateCximageHistogramEqualize)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_NORMALIZE, OnCximageHistogramNormalize)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_NORMALIZE, OnUpdateCximageHistogramNormalize)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCH, OnCximageHistogramStretch)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCH, OnUpdateCximageHistogramStretch)
	ON_COMMAND(ID_CXIMAGE_GAUSSIAN3X3, OnCximageGaussian3x3)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GAUSSIAN3X3, OnUpdateCximageGaussian3x3)
	ON_COMMAND(ID_CXIMAGE_GAUSSIAN5X5, OnCximageGaussian5x5)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GAUSSIAN5X5, OnUpdateCximageGaussian5x5)
	ON_COMMAND(ID_CXIMAGE_CONTOUR, OnCximageContour)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CONTOUR, OnUpdateCximageContour)
	ON_COMMAND(ID_CXIMAGE_LESSCONTRAST, OnCximageLesscontrast)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_LESSCONTRAST, OnUpdateCximageLesscontrast)
	ON_COMMAND(ID_CXIMAGE_JITTER, OnCximageJitter)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_JITTER, OnUpdateCximageJitter)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_DUPLICATE, OnUpdateWindowDuplicate)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_MIX, OnUpdateFiltersMix)
	ON_COMMAND(ID_FILTERS_MIX, OnFiltersMix)
	ON_COMMAND(ID_CXIMAGE_CIRCLETRANSFORM_CYLINDER, OnCximageCircletransformCylinder)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CIRCLETRANSFORM_CYLINDER, OnUpdateCximageCircletransformCylinder)
	ON_COMMAND(ID_CXIMAGE_CIRCLETRANSFORM_PINCH, OnCximageCircletransformPinch)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CIRCLETRANSFORM_PINCH, OnUpdateCximageCircletransformPinch)
	ON_COMMAND(ID_CXIMAGE_CIRCLETRANSFORM_PUNCH, OnCximageCircletransformPunch)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CIRCLETRANSFORM_PUNCH, OnUpdateCximageCircletransformPunch)
	ON_COMMAND(ID_CXIMAGE_CIRCLETRANSFORM_SWIRL, OnCximageCircletransformSwirl)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CIRCLETRANSFORM_SWIRL, OnUpdateCximageCircletransformSwirl)
	ON_COMMAND(ID_CXIMAGE_CIRCLETRANSFORM_BATHROOM, OnCximageCircletransformBathroom)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CIRCLETRANSFORM_BATHROOM, OnUpdateCximageCircletransformBathroom)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCH1, OnCximageHistogramStretch1)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCH2, OnCximageHistogramStretch2)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCH1, OnUpdateCximageHistogramStretch1)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCH2, OnUpdateCximageHistogramStretch2)
	ON_COMMAND(ID_FILTERS_NONLINEAR_EDGE, OnFiltersNonlinearEdge)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_NONLINEAR_EDGE, OnUpdateFiltersNonlinearEdge)
	ON_COMMAND(ID_CXIMAGE_SKEW, OnCximageSkew)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SKEW, OnUpdateCximageSkew)
	ON_COMMAND(ID_VIEW_TOOLS_TRACKER, OnViewToolsTracker)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_TRACKER, OnUpdateViewToolsTracker)
	ON_COMMAND(ID_FILTERS_JPEGCOMPRESSION, OnJpegcompression)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_JPEGCOMPRESSION, OnUpdateJpegcompression)
	ON_COMMAND(ID_VIEW_SMOOTH, OnViewSmooth)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SMOOTH, OnUpdateViewSmooth)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_DATAEXT, OnUpdateFiltersDataext)
	ON_COMMAND(ID_FILTERS_DATAEXT, OnFiltersDataext)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_UNSHARPMASK, OnUpdateCximageUnsharpmask)
	ON_COMMAND(ID_CXIMAGE_UNSHARPMASK, OnCximageUnsharpmask)
	ON_COMMAND(ID_CXIMAGE_TEXTBLUR, OnCximageTextblur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_TEXTBLUR, OnUpdateCximageTextblur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REDEYEREMOVE, OnUpdateCximageRedeyeremove)
	ON_COMMAND(ID_CXIMAGE_REDEYEREMOVE, OnCximageRedeyeremove)
	ON_COMMAND(ID_CXIMAGE_BLURSELBORDER, OnCximageBlurselborder)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_BLURSELBORDER, OnUpdateCximageBlurselborder)
	ON_COMMAND(ID_CXIMAGE_SELECTIVEBLUR, OnCximageSelectiveblur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_SELECTIVEBLUR, OnUpdateCximageSelectiveblur)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_GETTRANSPARENCYMASK, OnUpdateCximageGettransparencymask)
	ON_COMMAND(ID_CXIMAGE_GETTRANSPARENCYMASK, OnCximageGettransparencymask)
	ON_COMMAND(ID_COLORS_COUNTCOLORS, OnColorsCountcolors)
	ON_UPDATE_COMMAND_UI(ID_COLORS_COUNTCOLORS, OnUpdateColorsCountcolors)
	ON_COMMAND(ID_FILTERS_LINEAR_CUSTOM, OnFiltersLinearCustom)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_LINEAR_CUSTOM, OnUpdateFiltersLinearCustom)
	ON_COMMAND(ID_CXIMAGE_CANVASSIZE, OnCximageCanvassize)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_CANVASSIZE, OnUpdateCximageCanvassize)
	ON_COMMAND(ID_VIEW_TOOLS_FLOODFILL, OnViewToolsFloodfill)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLS_FLOODFILL, OnUpdateViewToolsFloodfill)
	ON_COMMAND(ID_CXIMAGE_REMOVESELECTION, OnCximageRemoveselection)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_REMOVESELECTION, OnUpdateCximageRemoveselection)
	ON_COMMAND(ID_COLORS_MORESATURATIONHSL, OnColorsMoresaturationhsl)
	ON_UPDATE_COMMAND_UI(ID_COLORS_MORESATURATIONHSL, OnUpdateColorsMoresaturationhsl)
	ON_COMMAND(ID_COLORS_MORESATURATIONYUV, OnColorsMoresaturationyuv)
	ON_UPDATE_COMMAND_UI(ID_COLORS_MORESATURATIONYUV, OnUpdateColorsMoresaturationyuv)
	ON_COMMAND(ID_COLORS_LESSSATURATION, OnColorsLesssaturation)
	ON_UPDATE_COMMAND_UI(ID_COLORS_LESSSATURATION, OnUpdateColorsLesssaturation)
	ON_COMMAND(ID_COLORS_HISTOGRAM_FULLSATURATION, OnColorsHistogramFullsaturation)
	ON_UPDATE_COMMAND_UI(ID_COLORS_HISTOGRAM_FULLSATURATION, OnUpdateColorsHistogramFullsaturation)
	ON_COMMAND(ID_COLORS_HISTOGRAM_HALFSATURATION, OnColorsHistogramHalfsaturation)
	ON_UPDATE_COMMAND_UI(ID_COLORS_HISTOGRAM_HALFSATURATION, OnUpdateColorsHistogramHalfsaturation)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCHT0, OnCximageHistogramStretcht0)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCHT0, OnUpdateCximageHistogramStretcht0)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCHT1, OnCximageHistogramStretcht1)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCHT1, OnUpdateCximageHistogramStretcht1)
	ON_COMMAND(ID_CXIMAGE_HISTOGRAM_STRETCHT2, OnCximageHistogramStretcht2)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_HISTOGRAM_STRETCHT2, OnUpdateCximageHistogramStretcht2)
	ON_COMMAND(ID_COLORS_ADAPTIVETHRESHOLD, OnColorsAdaptivethreshold)
	ON_UPDATE_COMMAND_UI(ID_COLORS_ADAPTIVETHRESHOLD, OnUpdateColorsAdaptivethreshold)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIOUSFRAME, OnUpdateViewPreviousframe)
	ON_COMMAND(ID_VIEW_PREVIOUSFRAME, OnViewPreviousframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NEXTFRAME, OnUpdateViewNextframe)
	ON_COMMAND(ID_VIEW_NEXTFRAME, OnViewNextframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAYANIMATION, OnUpdateViewPlayanimation)
	ON_COMMAND(ID_VIEW_PLAYANIMATION, OnViewPlayanimation)
	ON_UPDATE_COMMAND_UI(ID_FILTERS_ADDSHADOW, OnUpdateFiltersAddshadow)
	ON_COMMAND(ID_FILTERS_ADDSHADOW, OnFiltersAddshadow)
	ON_COMMAND(ID_CXIMAGE_TRACE, OnCximageTrace)
	ON_UPDATE_COMMAND_UI(ID_CXIMAGE_TRACE, OnUpdateCximageTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction
CDemoDoc::CDemoDoc()
{
	image = NULL;
	m_WaitingClick = stretchMode = m_bSmoothDisplay = FALSE;
	for (int i=0;i<MAX_UNDO_LEVELS;i++) imageUndo[i]=NULL;
	m_UndoLevel=0;
	m_ZoomFactor=1;
	QueryPerformanceFrequency(&m_swFreq);
	m_etime = 0.0;
	hThread=hProgress=0;
	m_NumSel=0;
	m_tool=0;
	m_playanimation = 0;
	m_hmax=0;
#ifndef VATI_EXTENSIONS
	memset(&m_font,0,sizeof(m_font));
	m_color=0;
	m_text=_T("text");
#endif
}
//////////////////////////////////////////////////////////////////////////////
CDemoDoc::~CDemoDoc()
{
	// stop the elaborations
	if (image) image->SetEscape(1);
	// stop the progress bar
	if (hProgress){
		ResumeThread(hProgress); //wake up!
		WaitForSingleObject(hProgress,INFINITE);
		CloseHandle(hProgress);
	}
	if (hThread){
		WaitForSingleObject(hThread,INFINITE);
		CloseHandle(hThread);
	}
	// free objects
	delete image;
	for (int i=0;i<MAX_UNDO_LEVELS;i++) delete imageUndo[i];
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()) return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	image = NULL;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization
void CDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())	{ } 
	else { }
}
//////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics
#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
//////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands
CString CDemoDoc::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
CString CDemoDoc::RemoveExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(0,i);
		}
	}
	return name;
}
//////////////////////////////////////////////////////////////////////////////
int CDemoDoc::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
#if CXIMAGE_SUPPORT_DECODE
	CString filename(lpszPathName);
	CString ext(FindExtension(filename));
	ext.MakeLower();
	if (ext == _T("")) return FALSE;

	int type = FindType(ext);

/*
	CxImage iinfo;
	CxIOFile file;
	file.Open(filename,"rb");
	bool bOk = iinfo.CheckFormat(&file);
	long t = iinfo.GetType();
	long w = iinfo.GetWidth();
	long h = iinfo.GetHeight();
*/

	Stopwatch(0);
	image = new CxImage(filename, type);
	Stopwatch(1);

	if (!image->IsValid()){
		CString s = image->GetLastError();
		AfxMessageBox(s);
		delete image;
		image = NULL;
		return FALSE;
	}

	UpdateStatusBar();
	UpdateAllViews(NULL,WM_USER_NEWIMAGE);

	//multiple images (TIFF/ICO)
	if (image->GetNumFrames()>1){
		CString s;
		s.Format(_T("File with %d images. Read all?"),image->GetNumFrames());
		if (AfxMessageBox(s,MB_OKCANCEL)==IDOK){
			
			int j; // points to the document name
			for(j=_tcslen(filename)-1;j>=0;j--){
				if (filename[j]=='\\'){	j++; break;	}
			}
			// create the documents for the other images
			for(int i=1;i<image->GetNumFrames();i++){
				CDemoDoc *NewDoc=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL);
				if (NewDoc)	{
					CxImage *newImage = new CxImage();
					newImage->SetFrame(i);
					newImage->Load(filename,type);
					NewDoc->image = newImage;
					CString s;
					s.Format(_T("%s (%d)"),filename.Mid(j),i+1);
					NewDoc->SetTitle(s);
					NewDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
				}
			}
		} else {
#if CXIMAGE_SUPPORT_GIF
			if (type == CXIMAGE_FORMAT_GIF){
				image->SetRetreiveAllFrames(true);
				//image->SetFrame(image->GetNumFrames()-1);
				image->Load(filename, type);
				s = _T("Play animation?");
				if (AfxMessageBox(s,MB_YESNO)==IDYES){
					OnViewPlayanimation();
				}
			}
#endif
		}
	}

 #if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
	// EXIF jpegs
	if (image->GetType() == CXIMAGE_FORMAT_JPG){
		FILE* hfile = _tfopen(filename,_T("rb"));
		m_exif.DecodeExif(hfile);
		fclose(hfile);
	}
 #endif //CXIMAGE_SUPPORT_EXIF
#endif
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CString filename(lpszPathName);
	CString ext(FindExtension(filename));
	ext.MakeLower();
	if (ext == _T("")) return FALSE;

	int type = FindType(ext);
	if (type == CXIMAGE_FORMAT_UNKNOWN) return FALSE;

#if CXIMAGE_SUPPORT_GIF
	if (type == CXIMAGE_FORMAT_GIF && image->GetBpp()>8){
		AfxMessageBox(_T("The image will be saved as a true color GIF!\nThis is ok for CxImage, but not for many other programs.\nFor better compatibility, please use DecreaseBpp to 8 bits or less."),MB_ICONINFORMATION);
	}
#endif

	bool retval=0;
#if CXIMAGE_SUPPORT_ENCODE
	Stopwatch(0);
	retval = image->Save(filename, type);
	Stopwatch(1);
	UpdateStatusBar();
#endif
	if (retval) return TRUE;
	CString s = image->GetLastError();
	AfxMessageBox(s);

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
{
	if (!image)	return FALSE;

	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty()) bSaveAs = TRUE;
	else if (!theApp.GetWritableType(image->GetType()))	bSaveAs = TRUE;

	if (bSaveAs){
		newName = m_strPathName;

		newName = RemoveExtension(newName);

		if (bReplace && newName.IsEmpty()){
			newName = m_strTitle;
			newName = RemoveExtension(newName);
			int iBad = newName.FindOneOf(_T("#%;/\\"));    // dubious filename
			if (iBad != -1)	//newName.ReleaseBuffer(iBad);
				newName = _T("UntitledImage");

			// append the default suffix if there is one
			//if (image->GetType()) newName += theApp.GetExtFromType(image->GetType()).Mid(1,4);
		}

		if (image->GetType())
			theApp.nDocType = image->GetType();

		if (!theApp.PromptForFileName(newName, 
			bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
			OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, &theApp.nDocType))
		{
			return FALSE;       // don't even try to save
		}
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName)){
		if (pszPathName == NULL){
			// be sure to delete the file
			TRY 
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}
			END_CATCH_ALL
		}
		EndWaitCursor();
		return FALSE;
	}

	EndWaitCursor();
	if (bReplace) {
		// Reset the title and change the document name
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName); // must be set
	}
	else // SaveCopyAs
	{
		SetModifiedFlag(bModified);
	}
	return TRUE;        // success
}
//////////////////////////////////////////////////////////////////////////////
#define EPSILON (0.0000001)

int CDemoDoc::ComputePixel(float x, float y, float &x1, float &y1)
{
  double r, nn;

  if (x==0 && y==0) {
	 x1 = x;
	 y1 = y;
	 return 1;
  }

  nn = sqrt(x*x + y*y);
  r =  (fabs(x) > fabs(y)) ? fabs(nn/x): fabs(nn/y);

  x1 = (float)(r*x);
  y1 = (float)(r*y);

  return 1;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!(image==0 || hThread));
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(0);
//	pCmdUI->Enable((image != NULL)); //&& theApp.GetWritableType(image->GetType()));
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnStretchMode() 
{
	stretchMode = !stretchMode;
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateStretchMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(stretchMode);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateCximageFlip(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread)
		pCmdUI->Enable(0);
}
void CDemoDoc::OnUpdateCximageGrayscale(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageMirror(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageNegative(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageResample(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRotate(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRotater(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRotatel(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageDither(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageThreshold(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplityuv(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplitrgb(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplithsl(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplityiq(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplitxyz(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSplitcmyk(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximagePseudocolors(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageFiltersColorize(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageLighten(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageDarken(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageContrast(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageEmboss(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageBlur(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageDilate(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageEdge(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageErode(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSharpen(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSoften(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCrop(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || !image->SelectionIsValid()) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRemovealphachannel(CCmdUI* pCmdUI) 
{	if(image==0 || hThread
#if CXIMAGE_SUPPORT_ALPHA
	   || !image->AlphaIsValid()
#endif
	   ) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageInvetalpha(CCmdUI* pCmdUI) 
{	if(image==0 || hThread
#if CXIMAGE_SUPPORT_ALPHA
	   || !image->AlphaIsValid()
#endif
	   ) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageAlphapalettetoggle(CCmdUI* pCmdUI) 
{	if(image==0 || hThread
#if CXIMAGE_SUPPORT_ALPHA
	   || !image->AlphaPaletteIsValid()
#endif
	   ) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageAlphastrip(CCmdUI* pCmdUI) 
{	if(image==0 || hThread
#if CXIMAGE_SUPPORT_ALPHA
		|| (!image->AlphaIsValid() && !image->AlphaPaletteIsValid())
#endif
	) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRemovetransparency(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || image->GetTransIndex()<0) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageGamma(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageMedian(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageAddnoise(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCombine(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageFft(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRepair(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageAlphachannelSplit(CCmdUI* pCmdUI) 
{	if(image==0 || hThread
#if CXIMAGE_SUPPORT_ALPHA
	   || !image->AlphaIsValid()
#endif
	   ) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageFiltersLog(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageFiltersRoot(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramEqualize(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramNormalize(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretch(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageGaussian3x3(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageGaussian5x5(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageContour(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageTrace(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageLesscontrast(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageJitter(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateWindowDuplicate(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateFiltersMix(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCircletransformCylinder(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCircletransformPinch(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCircletransformPunch(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCircletransformSwirl(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCircletransformBathroom(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretch1(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretch2(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateFiltersNonlinearEdge(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSkew(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateJpegcompression(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateFiltersDataext(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageUnsharpmask(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageTextblur(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRedeyeremove(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || !image->SelectionIsValid()) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageBlurselborder(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || !image->SelectionIsValid()) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageSelectiveblur(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageGettransparencymask(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || (image->GetTransIndex()<0
#if CXIMAGE_SUPPORT_ALPHA
	   && !image->AlphaIsValid()
#endif
	   )) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsCountcolors(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateFiltersLinearCustom(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageCanvassize(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageRemoveselection(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || !image->SelectionIsValid()) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsMoresaturationhsl(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsMoresaturationyuv(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsLesssaturation(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsHistogramHalfsaturation(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsHistogramFullsaturation(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretcht0(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretcht1(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateCximageHistogramStretcht2(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateColorsAdaptivethreshold(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateViewPreviousframe(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || image->GetFrame(0)==0) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateViewNextframe(CCmdUI* pCmdUI) 
{	if(image==0 || hThread || image->GetFrame(0)==0) pCmdUI->Enable(0);}
void CDemoDoc::OnUpdateFiltersAddshadow(CCmdUI* pCmdUI) 
{	if(image==0 || hThread) pCmdUI->Enable(0);}

//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateTransformEllipse(CCmdUI* pCmdUI) 
{
	if (image==NULL) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnTransformEllipse() 
{
	SubmitUndo();
	DWORD x, y;
	float x1, y1; //***bd*** use float source with GetPixelColorInterpolated
	float fx, fy, xmid, ymid, ar;
	CxImage *image2 = new CxImage(*image,false,false,true);

	xmid = (float) (image->GetWidth()/2.0);
	ymid = (float) (image->GetHeight()/2.0);
	ar = (float)(image->GetHeight())/(float)(image->GetWidth());
	for (y=0; y<image->GetHeight(); y++) {
		for (x=0; x<image->GetWidth(); x++) {
			ComputePixel(ar*(x-xmid), y-ymid, fx, fy);
			x1 = xmid+fx/ar;
			y1 = ymid+fy;
			//correct method to use would be GetAreaColorAveraged (but I guess there's not that much aliasing here)
			image2->SetPixelColor(x, y, image->GetPixelColorInterpolated(x1, y1, CxImage::IM_BILINEAR, CxImage::OM_BACKGROUND));
			//image2->SetPixelColor(x, y, image->GetPixelColor(x1, y1));
		}
	}
	delete image;
	image = image2;
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnWindowDuplicate() 
{
	CDemoDoc *NewDoc=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 

	if (NewDoc)	{
		CxImage *newImage = new CxImage(*image);

		NewDoc->image = newImage;

		CString s;
		s.Format(_T("Copy %d of %s"),((CDemoApp*)AfxGetApp())->m_nDocCount++,GetTitle());
		NewDoc->SetTitle(s);
		NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if (image==NULL) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnEditCopy() 
{
	CxImage* iSrc = image;

	//copy only the selected region box
#if CXIMAGE_SUPPORT_SELECTION
	CxImage iSel;
	if (image->SelectionIsValid()){
		RECT r;
		image->SelectionGetBox(r);
		r.bottom = image->GetHeight() - 1 -r.bottom; 
		r.top = image->GetHeight() - 1 -r.top; 
		image->Crop(r, &iSel);
		iSrc = &iSel;
	}
#endif //CXIMAGE_SUPPORT_SELECTION

	// standard DIB image
	HANDLE hDIB=iSrc->CopyToHandle();

#define USE_CF_CXIMAGE 1

#if USE_CF_CXIMAGE
	//custom CXIMAGE object
	HANDLE hMem=NULL;
	if (iSrc->IsValid() && (iSrc->SelectionIsValid() || iSrc->IsTransparent()
#if CXIMAGE_SUPPORT_ALPHA
		|| iSrc->AlphaIsValid()
#endif
		)){
		hMem= GlobalAlloc(GHND, iSrc->DumpSize());
		if (hMem){
			BYTE* pDst=(BYTE*)GlobalLock(hMem);
			iSrc->Dump(pDst);
			GlobalUnlock(hMem);
		}
	}
#endif //USE_CF_CXIMAGE

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
		if(::EmptyClipboard()) {
			if (::SetClipboardData(CF_DIB,hDIB) == NULL ) {
				AfxMessageBox( _T("Unable to set DIB clipboard data") );
			}
#if USE_CF_CXIMAGE
			if (hMem){
				UINT cf = ((CDemoApp*)AfxGetApp())->GetCF();
				if (::SetClipboardData(cf,hMem) == NULL ) {
					AfxMessageBox( _T("Unable to set CXIMAGE clipboard data") );
				}
			}
#endif //USE_CF_CXIMAGE
		}
	}
	CloseClipboard();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread) pCmdUI->Enable(0);
	else pCmdUI->Enable(m_UndoLevel>0);

	CString s;
	s.Format(_T("Undo (%d)\tCtrl+Z"),m_UndoLevel);
	pCmdUI->SetText(s);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread) pCmdUI->Enable(0);
	else pCmdUI->Enable((m_UndoLevel<(MAX_UNDO_LEVELS))&&
				   (imageUndo[m_UndoLevel]!=0));
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnEditUndo() 
{
	m_UndoLevel--;
	CxImage* tmp = image;
	image=imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;

	UpdateAllViews(0,WM_USER_NEWIMAGE);
	UpdateStatusBar();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnEditRedo() 
{
	CxImage* tmp = image;
	image=imageUndo[m_UndoLevel];
	imageUndo[m_UndoLevel]=tmp;
	m_UndoLevel++;

	UpdateAllViews(0,WM_USER_NEWIMAGE);
	UpdateStatusBar();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::SubmitUndo()
{
	if (m_UndoLevel>=MAX_UNDO_LEVELS){	// Max Undo reached
		delete imageUndo[0];			// discard the early undo
		for(int i=1;i<MAX_UNDO_LEVELS;i++){
			imageUndo[i-1]=imageUndo[i]; //shift the history
		}
		imageUndo[MAX_UNDO_LEVELS-1]=0;	// clear the undo slot
		m_UndoLevel=MAX_UNDO_LEVELS-1;  // reposition at last level
	}
	// we must clear the "redo" history when a new action is performed
	for (int i=m_UndoLevel;i<MAX_UNDO_LEVELS;i++){
		if (imageUndo[i]){
			delete imageUndo[i];
			imageUndo[i]=0;
		}
	}
	// save the actual image in the undo history
	if (image->IsValid()){
		imageUndo[m_UndoLevel] = new CxImage();
		imageUndo[m_UndoLevel]->Copy(*image);
		m_UndoLevel++;
	}
}
//////////////////////////////////////////////////////////////////////////////
void /*unsigned long _stdcall*/ RunProgressThread(void *lpParam)
{
	CDemoDoc *pDoc = (CDemoDoc *)lpParam;
	POSITION pos;
	CView *pView;
	while(pDoc->hThread){
		Sleep(333);
		if(!pDoc->image) break;
		if(pDoc->image->GetEscape()) break;
		long n=pDoc->image->GetProgress();
		pos = pDoc->GetFirstViewPosition();
		pView = pDoc->GetNextView(pos);
		if (pView) SendMessage(pView->m_hWnd, WM_USER_PROGRESS,n,0);
	}
	pos = pDoc->GetFirstViewPosition();
	pView = pDoc->GetNextView(pos);
	if (pView) SendMessage(pView->m_hWnd, WM_USER_PROGRESS,100,0);
	Sleep(111);
	pos = pDoc->GetFirstViewPosition();
	pView = pDoc->GetNextView(pos);
	if (pView) SendMessage(pView->m_hWnd, WM_USER_NEWIMAGE,0,0);
	pDoc->hProgress = 0;
	_endthread();
	return;
}
//////////////////////////////////////////////////////////////////////////////
void /*unsigned long _stdcall*/ RunCxImageThread(void *lpParam)
{
	CDemoDoc *pDoc = (CDemoDoc *)lpParam;
	if (pDoc==NULL) return;
	if (pDoc->image==NULL){
		pDoc->hThread=0;
		return;
	}

	 //prepare for elaboration
	pDoc->image->SetProgress(0);
	pDoc->image->SetEscape(0);

	pDoc->SubmitUndo();
	
	// auxilary thread for progress bar
	pDoc->hProgress = (HANDLE)_beginthread(RunProgressThread,0,pDoc);

	pDoc->Stopwatch(0);

	bool status = true;

	switch (pDoc->m_MenuCommand)
	{
//	case ID_FILE_OPEN:
//		status = pDoc->image->ReadFile(theApp.m_filename,theApp.m_filetype);
//		break;
	case ID_CXIMAGE_FLIP:
		status = pDoc->image->Flip();
		break;
	case ID_CXIMAGE_MIRROR:
		status = pDoc->image->Mirror();
		break;
	case ID_CXIMAGE_NEGATIVE:
		status = pDoc->image->Negative();
		break;
	case ID_CXIMAGE_GRAYSCALE:
		status = pDoc->image->GrayScale();
		break;
	case ID_CXIMAGE_DITHER:
		status = pDoc->image->Dither(theApp.m_Filters.DitherMethod);
		break;
	case ID_CXIMAGE_THRESHOLD:
		if (theApp.m_Filters.ThreshPreserveColors){
			RGBQUAD c = {255,255,255,0};
			status = pDoc->image->Threshold2(theApp.m_Filters.ThreshLevel,true,c,true);
		} else {
			status = pDoc->image->Threshold(theApp.m_Filters.ThreshLevel);
		}
		break;
	case ID_COLORS_ADAPTIVETHRESHOLD:
		{
			/*
			CxImage iContrastMask;
			iContrastMask.Copy(*pDoc->image,true,false,false);
			if (!iContrastMask.IsValid()) break;
			iContrastMask.GrayScale();
			long edge[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
			iContrastMask.Filter(edge,3,1,0);
			long blur[]={1,1,1,1,1,1,1,1,1};
			iContrastMask.Filter(blur,3,9,0);
			status = pDoc->image->AdaptiveThreshold(0,64,&iContrastMask);
			*/
			status = pDoc->image->AdaptiveThreshold();
		}
		break;
	case ID_CXIMAGE_COLORIZE:
		switch (theApp.m_Filters.ColorMode)
		{
		case 1:
			status = pDoc->image->Colorize(theApp.m_Filters.ColorHSL.rgbRed,theApp.m_Filters.ColorHSL.rgbGreen,theApp.m_Filters.ColorHSL.rgbBlue/100.0f);
			break;
		case 2:
			status = pDoc->image->Solarize(theApp.m_Filters.ColorSolarLevel,theApp.m_Filters.ColorSolarLink!=0);
			break;
		default:
			status = pDoc->image->ShiftRGB(theApp.m_Filters.ColorRed,theApp.m_Filters.ColorGreen,theApp.m_Filters.ColorBlue);
		}
		break;
	case ID_CXIMAGE_LIGHTEN:
		status = pDoc->image->Light(10);
		break;
	case ID_CXIMAGE_DARKEN:
		status = pDoc->image->Light(-10);
		break;
	case ID_CXIMAGE_CONTRAST:
		status = pDoc->image->Light(0,10);
		break;
	case ID_CXIMAGE_LESSCONTRAST:
		status = pDoc->image->Light(0,-10);
		break;
	case ID_COLORS_MORESATURATIONHSL:
		status = pDoc->image->Saturate(25,1);
		break;
	case ID_COLORS_MORESATURATIONYUV:
		status = pDoc->image->Saturate(25,2);
		break;
	case ID_COLORS_LESSSATURATION:
		status = pDoc->image->Saturate(-20,2);
		break;
	case ID_CXIMAGE_DILATE:
		status = pDoc->image->Dilate(3);
		break;
	case ID_CXIMAGE_ERODE:
		status = pDoc->image->Erode(3);
		break;
	case ID_CXIMAGE_CONTOUR:
		status = pDoc->image->Contour();
		break;
	case ID_CXIMAGE_TRACE:
		{
			RGBQUAD c = {0,0,0,0};
			status = pDoc->image->Trace(c,c);
		}
		break;
	case ID_CXIMAGE_ADDNOISE:
		status = pDoc->image->Noise(25);
		break;
	case ID_CXIMAGE_JITTER:
		status = pDoc->image->Jitter();
		break;
	case ID_CXIMAGE_TEXTBLUR:
		status = pDoc->image->TextBlur(100,2,3,true,true);
		break;
	case ID_CXIMAGE_BLURSELBORDER:
		{
#if CXIMAGE_SUPPORT_SELECTION
			CxImage iSel1,iSel2;
			pDoc->image->SelectionSplit(&iSel1);
			pDoc->image->SelectionSplit(&iSel2);
			iSel2.Edge();
			//iSel2.Erode();
			iSel2.Negative();
			pDoc->image->SelectionSet(iSel2);
			pDoc->image->GaussianBlur();
			pDoc->image->SelectionSet(iSel1);
#endif //CXIMAGE_SUPPORT_SELECTION
			break;
		}
	case ID_CXIMAGE_SELECTIVEBLUR:
		status = pDoc->image->SelectiveBlur(1,25);
		break;
	case ID_CXIMAGE_REDEYEREMOVE:
		status = pDoc->image->RedEyeRemove();
		break;
	case ID_FILTERS_NONLINEAR_EDGE:
		status = pDoc->image->Edge();
		break;
	case ID_CXIMAGE_CIRCLETRANSFORM_CYLINDER:
		status = pDoc->image->CircleTransform(3,0,100);
		break;
	case ID_CXIMAGE_CIRCLETRANSFORM_PINCH:
		status = pDoc->image->CircleTransform(1,0,100);
		break;
	case ID_CXIMAGE_CIRCLETRANSFORM_PUNCH:
		status = pDoc->image->CircleTransform(0,0,100);
		break;
	case ID_CXIMAGE_CIRCLETRANSFORM_SWIRL:
		status = pDoc->image->CircleTransform(2,0,100);
		break;
	case ID_CXIMAGE_CIRCLETRANSFORM_BATHROOM:
		status = pDoc->image->CircleTransform(4);
		break;
	case ID_CXIMAGE_EMBOSS:
		{
		int32_t kernel[]={0,0,-1,0,0,0,1,0,0};
		status = pDoc->image->Filter(kernel,3,-1,127);
		break;
		}
	case ID_CXIMAGE_BLUR:
		{
		int32_t kernel[]={1,1,1,1,1,1,1,1,1};
		status = pDoc->image->Filter(kernel,3,9,0);
		break;
		}
	case ID_CXIMAGE_GAUSSIAN3X3:
		{
		//long kernel[]={1,2,1,2,4,2,1,2,1};
		//status = pDoc->image->Filter(kernel,3,16,0);
		status = pDoc->image->GaussianBlur(1.0f);
		break;
		}
	case ID_CXIMAGE_GAUSSIAN5X5:
		{
		//long kernel[]={0,1,2,1,0,1,3,4,3,1,2,4,8,4,2,1,3,4,3,1,0,1,2,1,0};
		//status = pDoc->image->Filter(kernel,5,52,0);
		status = pDoc->image->GaussianBlur(2.0f);
		break;
		}
	case ID_CXIMAGE_SOFTEN:
		{
		int32_t kernel[]={1,1,1,1,8,1,1,1,1};
		status = pDoc->image->Filter(kernel,3,16,0);
		break;
		}
	case ID_CXIMAGE_SHARPEN:
		{
		int32_t kernel[]={-1,-1,-1,-1,15,-1,-1,-1,-1};
		status = pDoc->image->Filter(kernel,3,7,0);
		break;
		}
	case ID_CXIMAGE_EDGE:
		{
		int32_t kernel[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
		status = pDoc->image->Filter(kernel,3,-1,255);
		break;
		}
	case ID_FILTERS_LINEAR_CUSTOM: // [Priyank Bolia]
		{
			if(theApp.m_Filters.kSize==3)
				pDoc->image->Filter(theApp.m_Filters.Kernel3x3,3,theApp.m_Filters.kDivisor,theApp.m_Filters.kBias);
			else
				pDoc->image->Filter(theApp.m_Filters.Kernel5x5,5,theApp.m_Filters.kDivisor,theApp.m_Filters.kBias);
		break;
		}
	case ID_CXIMAGE_MEDIAN:
		status = pDoc->image->Median(3);
		break;
	case ID_CXIMAGE_UNSHARPMASK:
		status = pDoc->image->UnsharpMask();
		break;
	case ID_CXIMAGE_GAMMA:
		if (theApp.m_Filters.GammaLink){
			status = pDoc->image->GammaRGB(theApp.m_Filters.GammaR,theApp.m_Filters.GammaG,theApp.m_Filters.GammaB);
		} else {
			status = pDoc->image->Gamma(theApp.m_Filters.GammaLevel);
		}
		break;
	case ID_CXIMAGE_HISTOGRAM_LOG:
		status = pDoc->image->HistogramLog();
		break;
	case ID_CXIMAGE_HISTOGRAM_ROOT:
		status = pDoc->image->HistogramRoot();
		break; 
	case ID_CXIMAGE_HISTOGRAM_EQUALIZE:
		status = pDoc->image->HistogramEqualize();
		break;
	case ID_CXIMAGE_HISTOGRAM_NORMALIZE:
		status = pDoc->image->HistogramNormalize();
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCH:
		status = pDoc->image->HistogramStretch();
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCH1:
		status = pDoc->image->HistogramStretch(1);
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCH2:
		status = pDoc->image->HistogramStretch(2);
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCHT0:
		status = pDoc->image->HistogramStretch(0,0.005f);
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCHT1:
		status = pDoc->image->HistogramStretch(1,0.005f);
		break;
	case ID_CXIMAGE_HISTOGRAM_STRETCHT2:
		status = pDoc->image->HistogramStretch(2,0.005f);
		break;
	case ID_COLORS_HISTOGRAM_FULLSATURATION:
		{
			CxImage tmp;
			tmp.Copy(*(pDoc->image),true,false,false);
			tmp.ConvertColorSpace(2,0);
			int32_t u[256];
			int32_t v[256];
			tmp.Histogram(0,u,v,0,0);
			int umin = 255;
			int umax = 0;
			int vmin = 255;
			int vmax = 0;
			for (int i = 0; i<255; i++){
				if (u[i]) umin = i;
				if (u[255-i]) umax = i;
				if (v[i]) vmin = i;
				if (v[255-i]) vmax = i;
			}
			float cmin = (float)min(umin,vmin);
			float cmax = (float)max(umax,vmax);
			if (cmin<128) cmin = 128.0f/(128-cmin);
			else cmin = 128.0f;
			if (cmax>128) cmax = 128.0f/(cmax-128);
			else cmax = 128.0f;
			int sat = (int)(100.0f*(min(cmin,cmax)-1.0f));
			pDoc->image->Saturate(sat,2);
		}
		break;
	case ID_COLORS_HISTOGRAM_HALFSATURATION:
		{
			CxImage tmp;
			tmp.Copy(*(pDoc->image),true,false,false);
			tmp.ConvertColorSpace(2,0);
			int32_t u[256];
			int32_t v[256];
			tmp.Histogram(0,u,v,0,0);
			int umin = 255;
			int umax = 0;
			int vmin = 255;
			int vmax = 0;
			for (int i = 0; i<255; i++){
				if (u[i]) umin = i;
				if (u[255-i]) umax = i;
				if (v[i]) vmin = i;
				if (v[255-i]) vmax = i;
			}
			float cmin = (float)min(umin,vmin);
			float cmax = (float)max(umax,vmax);
			if (cmin<128) cmin = 128.0f/(128-cmin);
			else cmin = 128.0f;
			if (cmax>128) cmax = 128.0f/(cmax-128);
			else cmax = 128.0f;
			int sat = (int)(50.0f*(min(cmin,cmax)-1.0f));
			pDoc->image->Saturate(sat,2);
		}
		break;
	case ID_CXIMAGE_SKEW:
		status = pDoc->image->Skew(theApp.m_Filters.SkewSlopeX,theApp.m_Filters.SkewSlopeY,
							theApp.m_Filters.SkewPivotX,theApp.m_Filters.SkewPivotY,
							theApp.m_Filters.SkewInterp!=0);
		break;
	case ID_CXIMAGE_ROTATE:
		//***bd*** more rotation options
		CxImage::InterpolationMethod intm;
		CxImage::OverflowMethod overm;
		switch (theApp.m_Filters.RotateMethod) {
		case 0: intm=CxImage::IM_NEAREST_NEIGHBOUR; break;
		case 1: intm=CxImage::IM_BILINEAR; break;
		case 2: intm=CxImage::IM_BICUBIC; break;
		case 3: intm=CxImage::IM_BICUBIC2; break;
		case 4: intm=CxImage::IM_BSPLINE; break;
		case 5: intm=CxImage::IM_LANCZOS; break;
		case 6: intm=CxImage::IM_HERMITE; break;
		default: throw(0);
		}//switch
		switch (theApp.m_Filters.RotateOverflow) {
		case 0: overm=CxImage::OM_BACKGROUND; break;
		case 1: overm=CxImage::OM_BACKGROUND; break;
		case 2: overm=CxImage::OM_BACKGROUND; break;
		case 3: overm=CxImage::OM_WRAP; break;
		case 4: overm=CxImage::OM_REPEAT; break;
		case 5: overm=CxImage::OM_MIRROR; break;
		case 6: overm=CxImage::OM_TRANSPARENT; break;
		}//switch
		switch (theApp.m_Filters.RotateOverflow) {
		case 0: {
			RGBQUAD bkg = pDoc->image->GetPixelColor(0,0);
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, &bkg,true,theApp.m_Filters.RotateKeepsize!=0);
			break; }
		case 1: {
			RGBQUAD bkg = {0,0,0,0};
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, &bkg,true,theApp.m_Filters.RotateKeepsize!=0);
			break; }
		default:
			status = pDoc->image->Rotate2(theApp.m_Filters.RotateAngle, 0, intm, overm, 0,true,theApp.m_Filters.RotateKeepsize!=0);
		}
		break;
	case ID_CXIMAGE_ROTATEL:
		status = pDoc->image->RotateLeft();
		if (status) pDoc->RegionRotateLeft();
		break;
	case ID_CXIMAGE_ROTATER:
		status = pDoc->image->RotateRight();
		if (status) pDoc->RegionRotateRight();
		break;
	case ID_CXIMAGE_RESAMPLE:
		//***bd*** more resample options
		CxImage::InterpolationMethod rintm;
		switch (theApp.m_Filters.ResampleMethod) {
		case 0: rintm=CxImage::IM_NEAREST_NEIGHBOUR; break;
		case 1: rintm=CxImage::IM_BILINEAR; break;
		case 2: rintm=CxImage::IM_BILINEAR; break;
		case 3: rintm=CxImage::IM_BICUBIC; break;
		case 4: rintm=CxImage::IM_BICUBIC2; break;
		case 5: rintm=CxImage::IM_BSPLINE; break;
		case 6: rintm=CxImage::IM_LANCZOS; break;
		case 7: rintm=CxImage::IM_HERMITE; break;
		default: throw(0);
		}//switch
		switch (theApp.m_Filters.ResampleMethod) {
		case 0:
			status = pDoc->image->Resample(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,1);
			break;
		case 1:
			status = pDoc->image->Resample(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,0);
			break;
		case 2:
			if ((long)pDoc->image->GetWidth()>theApp.m_Filters.ResampleW && (long)pDoc->image->GetHeight()>theApp.m_Filters.ResampleH)
				status = pDoc->image->QIShrink(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH);
			else
				status = pDoc->image->Resample2(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,rintm,CxImage::OM_REPEAT);
			break;
		default:
			status = pDoc->image->Resample2(theApp.m_Filters.ResampleW,theApp.m_Filters.ResampleH,rintm,CxImage::OM_REPEAT);
		}
		break;
	case ID_CXIMAGE_CANVASSIZE:
		{
			RGBQUAD color = CxImage::RGBtoRGBQUAD(theApp.m_Filters.CanvasBkg);
			if (theApp.m_Filters.CanvasUseImageBkg)
				color = pDoc->image->GetTransColor();

			RECT r;
			if (theApp.m_Filters.CanvasMode == 0){
				r.top = 0;
				r.left = 0;
				r.right = theApp.m_Filters.CanvasW - pDoc->image->GetWidth();
				r.bottom = theApp.m_Filters.CanvasH - pDoc->image->GetHeight();
				if (theApp.m_Filters.CanvasCenterH){
					r.left = (theApp.m_Filters.CanvasW - pDoc->image->GetWidth()) / 2;
					r.right = theApp.m_Filters.CanvasW - pDoc->image->GetWidth() - r.left;
				}
				if (theApp.m_Filters.CanvasCenterV){
					r.top = (theApp.m_Filters.CanvasH - pDoc->image->GetHeight()) / 2;
					r.bottom = theApp.m_Filters.CanvasH - pDoc->image->GetHeight() - r.top;
				}
			} else {
				r.top = theApp.m_Filters.CanvasTop;
				r.left = theApp.m_Filters.CanvasLeft;
				r.right = theApp.m_Filters.CanvasRight;
				r.bottom = theApp.m_Filters.CanvasBottom;
			}

			status = pDoc->image->Expand(r.left, r.top,	r.right, r.bottom, color);											
		}
		break;
	case ID_CXIMAGE_INCREASEBPP:
		status = pDoc->image->IncreaseBpp(theApp.m_Filters.IncBppBPP);
		break;
	case ID_CXIMAGE_DECREASEBPP:
		{
			long bit = theApp.m_Filters.DecBppBPP;
			long method = theApp.m_Filters.DecBppPalMethod;
			bool errordiffusion = theApp.m_Filters.DecBppErrDiff!=0;
			long colors = theApp.m_Filters.DecBppMaxColors;

			//pDoc->image->IncreaseBpp(24);
			RGBQUAD c = pDoc->image->GetTransColor();

			RGBQUAD* ppal = NULL;
			if (method==1){
				switch (bit){
				/*case 1:
					{
						CQuantizer q(2,8);
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(2*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
						break;
					}*/
				case 4:
					{
						CQuantizer q(colors,8);
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(16*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
						break;
					}
				case 8:
					{
						CQuantizer q(colors,(colors>16?7:8));
						q.ProcessImage(pDoc->image->GetDIB());
						ppal=(RGBQUAD*)calloc(256*sizeof(RGBQUAD),1);
						q.SetColorTable(ppal);
					}
				}
				status = pDoc->image->DecreaseBpp(bit,errordiffusion,ppal,colors);
			} else status = pDoc->image->DecreaseBpp(bit,errordiffusion,0);

#if CXIMAGE_SUPPORT_ALPHA
			if (!pDoc->image->AlphaPaletteIsValid()) pDoc->image->AlphaPaletteEnable(0);
#endif
			if (pDoc->image->IsTransparent()){
				pDoc->image->SetTransIndex(pDoc->image->GetNearestIndex(c));
			}

			if (ppal) free(ppal);
			break;
		}
	}

	pDoc->Stopwatch(1);

	pDoc->image->SetProgress(100);

	if (!status){
		CString s = pDoc->image->GetLastError();
		AfxMessageBox(s);
	}

	pDoc->hThread=0;

	_endthread();
	return ;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageFlip() 
{
	m_MenuCommand=ID_CXIMAGE_FLIP;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageMirror() 
{
	m_MenuCommand=ID_CXIMAGE_MIRROR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageNegative() 
{
	m_MenuCommand=ID_CXIMAGE_NEGATIVE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageGrayscale() 
{
	m_MenuCommand=ID_CXIMAGE_GRAYSCALE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageDecreasebpp() 
{
	if (image==NULL) return;
	DlgDecBpp dlg;
	dlg.m_bit = theApp.m_Filters.DecBppBPP;
	dlg.m_method = theApp.m_Filters.DecBppPalMethod;
	dlg.m_errordiffusion = theApp.m_Filters.DecBppErrDiff;
	dlg.m_bLimitColors = theApp.m_Filters.DecBppLimitColors;
	dlg.m_maxcolors = theApp.m_Filters.DecBppMaxColors;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_DECREASEBPP;
		theApp.m_Filters.DecBppBPP = dlg.m_bit;
		theApp.m_Filters.DecBppPalMethod = dlg.m_method;
		theApp.m_Filters.DecBppErrDiff = dlg.m_errordiffusion;
		theApp.m_Filters.DecBppLimitColors = dlg.m_bLimitColors;
		theApp.m_Filters.DecBppMaxColors = dlg.m_maxcolors;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageIncreasebpp() 
{
	if (image==NULL) return;
	DlgIncBpp dlg;
	dlg.m_bit = theApp.m_Filters.IncBppBPP;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_INCREASEBPP;
		theApp.m_Filters.IncBppBPP = dlg.m_bit;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRotatel() 
{
	m_MenuCommand=ID_CXIMAGE_ROTATEL;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRotater() 
{
	m_MenuCommand=ID_CXIMAGE_ROTATER;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRotate() 
{
	if (image==NULL) return;
	DlgRotate dlg;
	dlg.m_angle = theApp.m_Filters.RotateAngle;
	dlg.m_method = theApp.m_Filters.RotateMethod;
	dlg.m_overflow = theApp.m_Filters.RotateOverflow;
	dlg.m_keepsize = theApp.m_Filters.RotateKeepsize;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_ROTATE;
		theApp.m_Filters.RotateAngle = dlg.m_angle;
		theApp.m_Filters.RotateMethod = dlg.m_method;
		theApp.m_Filters.RotateOverflow = dlg.m_overflow;
		theApp.m_Filters.RotateKeepsize = dlg.m_keepsize;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageResample() 
{
	if (image==NULL) return;
	
	DlgResample dlg;
	dlg.m_w = image->GetWidth();
	dlg.m_h = image->GetHeight();
	dlg.m_ratio = ((float)image->GetWidth())/((float)image->GetHeight());

	dlg.m_sizemode = theApp.m_Filters.ResampleSizemode;
	dlg.m_factor = theApp.m_Filters.ResampleFactor;
	dlg.m_newwidth = theApp.m_Filters.ResampleW;
	dlg.m_newheight = theApp.m_Filters.ResampleH;
	dlg.m_bKeepRatio = theApp.m_Filters.ResampleKeepRatio;
	dlg.m_mode = theApp.m_Filters.ResampleMethod;
	if (dlg.m_bKeepRatio) dlg.m_newheight = (DWORD)(dlg.m_newwidth/dlg.m_ratio);

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_RESAMPLE;

		if (dlg.m_sizemode==1){
			dlg.m_newwidth = (DWORD)(dlg.m_w * fabs(dlg.m_factor));
			dlg.m_newheight = (DWORD)(dlg.m_h * fabs(dlg.m_factor));
		}

		theApp.m_Filters.ResampleSizemode = dlg.m_sizemode;
		theApp.m_Filters.ResampleFactor = dlg.m_factor;
		theApp.m_Filters.ResampleW = dlg.m_newwidth;
		theApp.m_Filters.ResampleH = dlg.m_newheight;
		theApp.m_Filters.ResampleKeepRatio = dlg.m_bKeepRatio;
		theApp.m_Filters.ResampleMethod = dlg.m_mode;

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::UpdateStatusBar()
{
	if (image){
		CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
		CString s,t;
		t = theApp.GetDescFromType(image->GetType());
		s.Format(_T("(%dx%dx%d)"),image->GetWidth(),image->GetHeight(),image->GetBpp());
		statusBar.SetPaneText(4, s);
		statusBar.SetPaneText(3,t.Mid(0,3));
		s.Format(_T("Time (s): %.3f"),m_etime);
		statusBar.SetPaneText(2, s);

//		((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetProgressBar().SetPos(0);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewZoomin() 
{
	if (m_ZoomFactor>=16) return;

	if		(m_ZoomFactor == 0.50f) m_ZoomFactor = 0.75f;
	else if (m_ZoomFactor == 0.75f) m_ZoomFactor = 1.00f;
	else if (m_ZoomFactor == 1.00f)	m_ZoomFactor = 1.50f;
	else if (m_ZoomFactor == 1.50f)	m_ZoomFactor = 2.00f;
	else							m_ZoomFactor*=2;

	CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
	CString s;
	s.Format(_T("%4.0f %%"),m_ZoomFactor*100);
	statusBar.SetPaneText(2, s);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewZoomout() 
{
	if (m_ZoomFactor<=0.0625) return;

	if		(m_ZoomFactor == 2.00f) m_ZoomFactor = 1.50f;
	else if (m_ZoomFactor == 1.50f) m_ZoomFactor = 1.00f;
	else if (m_ZoomFactor == 1.00f)	m_ZoomFactor = 0.75f;
	else if (m_ZoomFactor == 0.75f)	m_ZoomFactor = 0.50f;
	else							m_ZoomFactor/=2;

	CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
	CString s;
	s.Format(_T("%4.1f %%"),m_ZoomFactor*100);
	statusBar.SetPaneText(2, s);

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewZoomin(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_ZoomFactor>=16) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewZoomout(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_ZoomFactor<=0.125) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewNormalviewing11() 
{
	m_ZoomFactor=1;
	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewNormalviewing11(CCmdUI* pCmdUI) 
{
	if (stretchMode) pCmdUI->Enable(0);
	if (m_ZoomFactor==1) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateCximageSettransparency(CCmdUI* pCmdUI) 
{
	if (image && hThread==0) pCmdUI->Enable(1);
	else pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRemovetransparency() 
{
	SubmitUndo();
	if (image) image->SetTransIndex(-1);
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSettransparency() 
{
	m_WaitingClick=TRUE;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::Stopwatch(int start0stop1)
{
	if (start0stop1==0)	QueryPerformanceCounter(&m_swStart);
	else {
		QueryPerformanceCounter(&m_swStop);
		if (m_swFreq.LowPart==0 && m_swFreq.HighPart==0) m_etime = -1;
		else {
			m_etime = (float)(m_swStop.LowPart - m_swStart.LowPart);
			if (m_etime < 0) m_etime += 2^32;
			m_etime /= (m_swFreq.LowPart+m_swFreq.HighPart * 2^32);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageOptions() 
{
	if (image==NULL) return;
	
	DlgOptions dlg;
	dlg.m_jpeg_quality = image->GetJpegQualityF();
	dlg.m_xres = image->GetXDPI();
	dlg.m_yres = image->GetYDPI();

#if CXIMAGE_SUPPORT_TIF
	dlg.m_Opt_tif = image->GetCodecOption(CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
	dlg.m_Opt_gif = image->GetCodecOption(CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_JPG
	dlg.m_Opt_jpg = image->GetCodecOption(CXIMAGE_FORMAT_JPG);
#endif
#if CXIMAGE_SUPPORT_PNG
	dlg.m_Opt_png = image->GetCodecOption(CXIMAGE_FORMAT_PNG);
#endif
#if CXIMAGE_SUPPORT_RAW
	dlg.m_Opt_raw = image->GetCodecOption(CXIMAGE_FORMAT_RAW);
#endif
	
#if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
	dlg.m_exif = &m_exif;
#endif
	if (dlg.DoModal()==IDOK){
		image->SetJpegQualityF(dlg.m_jpeg_quality);
		image->SetXDPI(dlg.m_xres);
		image->SetYDPI(dlg.m_yres);

#if CXIMAGE_SUPPORT_TIF
		image->SetCodecOption(dlg.m_Opt_tif, CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
		image->SetCodecOption(dlg.m_Opt_gif, CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_JPG
		image->SetCodecOption(dlg.m_Opt_jpg, CXIMAGE_FORMAT_JPG);
#endif
#if CXIMAGE_SUPPORT_PNG
		image->SetCodecOption(dlg.m_Opt_png, CXIMAGE_FORMAT_PNG);
#endif
#if CXIMAGE_SUPPORT_RAW
		image->SetCodecOption(dlg.m_Opt_raw, CXIMAGE_FORMAT_RAW);
#endif

#ifdef VATI_EXTENSIONS
		theApp.m_optJpegQuality = dlg.m_jpeg_quality;
		theApp.m_optJpegOptions = dlg.m_Opt_jpg;
		theApp.m_optRawOptions  = dlg.m_Opt_raw;
#endif
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageDither() 
{
	if (image==NULL) return;
	
	DlgDither dlg;
	dlg.m_method = theApp.m_Filters.DitherMethod;
	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_DITHER;
		theApp.m_Filters.DitherMethod = dlg.m_method;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageThreshold() 
{
	if (image==NULL) return;
	DlgThreshold dlg;

	CxImage iContrastMask;
	iContrastMask.Copy(*image,true,false,false);
	if (!iContrastMask.IsValid()){
		AfxMessageBox(_T("cannot create ContrastMask")); 
		return;
	}
	iContrastMask.GrayScale();
	int32_t edge[]={-1,-1,-1,-1,8,-1,-1,-1,-1};
	iContrastMask.Filter(edge,3,1,0);
	int32_t blur[]={1,1,1,1,1,1,1,1,1};
	iContrastMask.Filter(blur,3,9,0);

	if (image->IsGrayScale()){
		dlg.m_thresh1 = (long)image->OptimalThreshold(0,0);
		dlg.m_thresh2 = (long)image->OptimalThreshold(0,0,&iContrastMask);
	} else {
		CxImage iGray;
		iGray.Copy(*image,true,false,false);
		iGray.GrayScale();
		dlg.m_thresh1 = (long)iGray.OptimalThreshold(0,0);
		dlg.m_thresh2 = (long)iGray.OptimalThreshold(0,0,&iContrastMask);
	}

	dlg.m_mean = (BYTE)image->Mean();
	dlg.m_bPreserve = theApp.m_Filters.ThreshPreserveColors;
	dlg.m_level = theApp.m_Filters.ThreshLevel;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_THRESHOLD;
		theApp.m_Filters.ThreshLevel = dlg.m_level;
		theApp.m_Filters.ThreshPreserveColors = dlg.m_bPreserve;
		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplitrgb() 
{
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	Stopwatch(0);
	
	image->SplitRGB(newr,newg,newb);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format(_T("Red Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format(_T("Green Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format(_T("Blue Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplityuv() 
{
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	Stopwatch(0);

	image->SplitYUV(newr,newg,newb);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format(_T("Y Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format(_T("U Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format(_T("V Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplithsl() 
{
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	Stopwatch(0);

	image->SplitHSL(newr,newg,newb);

	Stopwatch(1);
	UpdateStatusBar();
	
	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format(_T("Hue Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format(_T("Saturation Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format(_T("Lightness Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplitcmyk() 
{
	if (image==NULL) return;

	CxImage *newc = new CxImage();
	CxImage *newm = new CxImage();
	CxImage *newy = new CxImage();
	CxImage *newk = new CxImage();

	Stopwatch(0);
	
	image->SplitCMYK(newc,newm,newy,newk);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newc;
		CString s;
		s.Format(_T("C Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newm;
		CString s;
		s.Format(_T("M Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newy;
		CString s;
		s.Format(_T("Y Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDock=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDock)	{
		NewDock->image = newk;
		CString s;
		s.Format(_T("K Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDock->SetTitle(s);
		NewDock->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximagePseudocolors() 
{
	if (image==NULL) return;
	
	SubmitUndo();

	if (!image->IsGrayScale()) image->GrayScale();
	image->HuePalette();

/*	POSITION pos = GetFirstViewPosition();
	CDemoView* pView = (CDemoView*)GetNextView(pos);
	HDC srcDC = ::GetDC(pView->GetSafeHwnd());
	HDC memDC = ::CreateCompatibleDC(srcDC);
	// copy the screen to the bitmap
	CSize sz(image->GetWidth(), image->GetHeight());
	int xshift = 0, yshift = 0;
	HBITMAP bm =::CreateCompatibleBitmap(srcDC, sz.cx, sz.cy);
	HBITMAP oldbm = (HBITMAP)::SelectObject(memDC,bm);
	::BitBlt(memDC, 0, 0, sz.cx, sz.cy, srcDC, xshift, yshift, SRCCOPY);
//	image->SetTransIndex(-1);
//	image->Draw(memDC);
	::TextOut(memDC,10,10,_T("test"),4);
	
	CxImage newima;
	newima.CreateFromHBITMAP(bm);
	image->Transfer(newima);
	// free objects
	SelectObject(memDC,oldbm);    
	DeleteObject(memDC);*/

	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageFiltersColorize() 
{
	if (image==NULL) return;
	DlgColorize dlg;

	dlg.m_bHSL = theApp.m_Filters.ColorMode;
	dlg.m_blend = theApp.m_Filters.ColorHSL.rgbBlue;
	dlg.m_sat = theApp.m_Filters.ColorHSL.rgbGreen;
	dlg.m_hue = theApp.m_Filters.ColorHSL.rgbRed;
	dlg.m_b = theApp.m_Filters.ColorBlue;
	dlg.m_g = theApp.m_Filters.ColorGreen;
	dlg.m_r = theApp.m_Filters.ColorRed;
	dlg.m_sol = theApp.m_Filters.ColorSolarLevel;
	dlg.m_bLinked = theApp.m_Filters.ColorSolarLink;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_COLORIZE;

		theApp.m_Filters.ColorMode = dlg.m_bHSL;
		theApp.m_Filters.ColorHSL.rgbBlue = dlg.m_blend;
		theApp.m_Filters.ColorHSL.rgbGreen = dlg.m_sat;
		theApp.m_Filters.ColorHSL.rgbRed = dlg.m_hue;
		theApp.m_Filters.ColorBlue = dlg.m_b;
		theApp.m_Filters.ColorGreen = dlg.m_g;
		theApp.m_Filters.ColorRed = dlg.m_r;
		theApp.m_Filters.ColorSolarLevel = dlg.m_sol;
		theApp.m_Filters.ColorSolarLink = dlg.m_bLinked;

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageDarken() 
{
	m_MenuCommand=ID_CXIMAGE_DARKEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageLighten() 
{
	m_MenuCommand=ID_CXIMAGE_LIGHTEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageContrast() 
{
	m_MenuCommand=ID_CXIMAGE_CONTRAST;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageEmboss() 
{
	m_MenuCommand=ID_CXIMAGE_EMBOSS;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageBlur() 
{
	m_MenuCommand=ID_CXIMAGE_BLUR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageDilate() 
{
	m_MenuCommand=ID_CXIMAGE_DILATE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageEdge() 
{
	m_MenuCommand=ID_CXIMAGE_EDGE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageErode() 
{
	m_MenuCommand=ID_CXIMAGE_ERODE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSharpen() 
{
	m_MenuCommand=ID_CXIMAGE_SHARPEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSoften() 
{
	m_MenuCommand=ID_CXIMAGE_SOFTEN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCrop() 
{
	SubmitUndo();
	
#ifdef VATI_EXTENSIONS
	// if there is a valid rectangle selection, then call the CropRotatedRectangle instead original Crop
	if (m_isRectSel && m_NumSel==5 )
	{
		CPoint top;
		long height, width;
		float angle;
		int topcorner = -1;

		// get upperleft corner
		top.x = 999999;
		top.y = 999999;
		for(int i=0; i<4; i++)
		{
			if ( top.y >= m_Sel[i].y )
			{
				if ( top.y == m_Sel[i].y && top.x < m_Sel[i].x )
					continue;
				top.x = m_Sel[i].x;
				top.y = m_Sel[i].y;
				topcorner = i;
			}
		}
		// get side lengths (-1st and +1st indexes points to 2 sides)
		if ( topcorner > 0 && topcorner < 4 )
			height = (long)LEN2D( top.x - m_Sel[topcorner-1].x, top.y - m_Sel[topcorner-1].y );
		else if ( topcorner == 0 )
			height = (long)LEN2D( top.x - m_Sel[3].x, top.y - m_Sel[3].y );
		else
			return;	// fatal prog error

		width  = (long)LEN2D( top.x - m_Sel[topcorner+1].x, top.y - m_Sel[topcorner+1].y );
		angle = (float)atan2( (float)(m_Sel[topcorner+1].y - top.y), (float)(m_Sel[topcorner+1].x - top.x) );

		image->CropRotatedRectangle( top.x, top.y, width, height, angle );
	}
		
	else // freehand selection
	{
#if CXIMAGE_SUPPORT_SELECTION
		RECT r;
		image->SelectionGetBox(r);
		r.bottom = image->GetHeight() - 1 -r.bottom; 
		r.top = image->GetHeight() - 1 -r.top; 
		image->Crop(r);
#endif //CXIMAGE_SUPPORT_SELECTION
	}
#else
	RECT r;
	image->SelectionGetBox(r);
	r.bottom = image->GetHeight() - 1 -r.bottom; 
	r.top = image->GetHeight() - 1 -r.top; 
	image->Crop(r);
#endif	
	UpdateStatusBar(); // VAti - to refresh image size in the status bar
	UpdateAllViews(NULL,WM_USER_NEWIMAGE);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRemovealphachannel() 
{
#if CXIMAGE_SUPPORT_ALPHA
	SubmitUndo();
	image->AlphaDelete();
	image->AlphaSetMax(255);
	UpdateAllViews(NULL);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageOpacity() 
{
#if CXIMAGE_SUPPORT_ALPHA
	if (image==NULL) return;
	DlgOpacity dlg;
	dlg.m_level=image->AlphaGetMax();
	if (dlg.DoModal()==IDOK){
		SubmitUndo();
		if (!image->AlphaIsValid()){
			image->AlphaCreate();
		}
		image->AlphaSetMax(dlg.m_level);
	}
	UpdateAllViews(NULL);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageInvetalpha() 
{
#if CXIMAGE_SUPPORT_ALPHA
	SubmitUndo();
	image->AlphaInvert();
	UpdateAllViews(NULL);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageAlphapalettetoggle() 
{
#if CXIMAGE_SUPPORT_ALPHA
	SubmitUndo();
	image->AlphaPaletteEnable(!image->AlphaPaletteIsEnabled());
	UpdateAllViews(NULL);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageAlphastrip() 
{
#if CXIMAGE_SUPPORT_ALPHA
	SubmitUndo();
	SetCursor(LoadCursor(0,IDC_WAIT));
	Stopwatch(0);

	RGBQUAD c={255,255,255,0};
	image->SetTransColor(c);
	image->AlphaStrip();

	Stopwatch(1);
	SetCursor(LoadCursor(0,IDC_ARROW));
	UpdateStatusBar();
	UpdateAllViews(NULL,WM_USER_NEWIMAGE);	
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageGamma() 
{
	if (image==NULL) return;
	DlgGamma dlg;
	dlg.m_gamma = theApp.m_Filters.GammaLevel;
	dlg.m_gammaR = theApp.m_Filters.GammaR;
	dlg.m_gammaG = theApp.m_Filters.GammaG;
	dlg.m_gammaB = theApp.m_Filters.GammaB;
	dlg.m_bGammaMode = theApp.m_Filters.GammaLink;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_GAMMA;
		theApp.m_Filters.GammaLevel = dlg.m_gamma;
		theApp.m_Filters.GammaR = dlg.m_gammaR;
		theApp.m_Filters.GammaG = dlg.m_gammaG;
		theApp.m_Filters.GammaB = dlg.m_gammaB;
		theApp.m_Filters.GammaLink = dlg.m_bGammaMode;

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageMedian() 
{
	m_MenuCommand=ID_CXIMAGE_MEDIAN;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageAddnoise() 
{
	m_MenuCommand=ID_CXIMAGE_ADDNOISE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsMove() 
{
	m_tool=0;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsMove(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsSelect() 
{
	m_tool=1;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==1);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsZoom() 
{
	m_tool=2;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsZoom(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==2);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsText() 
{
	if (image==NULL) return;
	DlgText dlg;

#ifndef VATI_EXTENSIONS
	memcpy(&(dlg.m_font),&m_font,sizeof(m_font));
	dlg.m_text=m_text;
	dlg.m_color=m_color;
	if (dlg.DoModal()==IDOK){
		m_text=dlg.m_text;
		m_color=dlg.m_color;
		memcpy(&m_font,&(dlg.m_font),sizeof(m_font));
		m_tool=3;
	}
#else
    //pass all data about text
    memcpy( &(dlg.m_textdata), &theApp.m_text, sizeof(CxImage::CXTEXTINFO) );
    if (dlg.DoModal()==IDOK)
    {
        //retrieve all data about text
		memcpy( &theApp.m_text, &(dlg.m_textdata), sizeof(CxImage::CXTEXTINFO) );
		m_tool=3;
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsText(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==3);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewPalette(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread || image->GetNumColors()==0) pCmdUI->Enable(0);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewPalette() 
{
	if (image==NULL) return;
	DlgPalette dlg;
	dlg.m_numcolors=image->GetNumColors();
	memcpy(dlg.m_pal,image->GetPalette(),dlg.m_numcolors*sizeof(RGBQUAD));
	if (dlg.DoModal()==IDOK){
		if (dlg.m_changed){
			SubmitUndo();
			switch (dlg.m_replace){
			case 1:
				{
					image->SetPalette(dlg.m_pal,dlg.m_numcolors);
					break;
				}
			case 2:
				{
					int bpp=image->GetBpp();
					image->IncreaseBpp(24);
					image->DecreaseBpp(bpp,false,dlg.m_pal);
					break;
				}
			case 3:
				{
					int bpp=image->GetBpp();
					image->IncreaseBpp(24);
					image->DecreaseBpp(bpp,true,dlg.m_pal);
					break;
				}
			}

			UpdateAllViews(NULL,WM_USER_NEWIMAGE);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCombine() 
{
	if (image==NULL) return;
	DlgCombine dlg;
	if (dlg.DoModal()==IDOK){
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);
		CxImage *newr,*newg,*newb,*newa;
		newr=newg=newb=newa=NULL;

		newr = new CxImage();
		switch(dlg.pChR){
		case 0:
			newr->Copy(*(dlg.pDocR->GetImage()),1,0,0);
			newr->GrayScale();
			break;
		case 1:
			dlg.pDocR->GetImage()->SplitRGB(newr,0,0);
			break;
		case 2:
			dlg.pDocR->GetImage()->SplitRGB(0,newr,0);
			break;
		case 3:
			dlg.pDocR->GetImage()->SplitRGB(0,0,newr);
			break;
		}

		newg = new CxImage();
		switch(dlg.pChG){
		case 0:
			newg->Copy(*(dlg.pDocG->GetImage()),1,0,0);
			newg->GrayScale();
			break;
		case 1:
			dlg.pDocG->GetImage()->SplitRGB(newg,0,0);
			break;
		case 2:
			dlg.pDocG->GetImage()->SplitRGB(0,newg,0);
			break;
		case 3:
			dlg.pDocG->GetImage()->SplitRGB(0,0,newg);
			break;
		}

		newb = new CxImage();
		switch(dlg.pChB){
		case 0:
			newb->Copy(*(dlg.pDocB->GetImage()),1,0,0);
			newb->GrayScale();
			break;
		case 1:
			dlg.pDocB->GetImage()->SplitRGB(newb,0,0);
			break;
		case 2:
			dlg.pDocB->GetImage()->SplitRGB(0,newb,0);
			break;
		case 3:
			dlg.pDocB->GetImage()->SplitRGB(0,0,newb);
			break;
		}

		if (dlg.pDocA){
			newa = new CxImage();
			switch(dlg.pChA){
			case 0:
				newa->Copy(*(dlg.pDocA->GetImage()),1,0,0);
				newa->GrayScale();
				break;
			case 1:
				dlg.pDocA->GetImage()->SplitRGB(newa,0,0);
				break;
			case 2:
				dlg.pDocA->GetImage()->SplitRGB(0,newa,0);
				break;
			case 3:
				dlg.pDocA->GetImage()->SplitRGB(0,0,newa);
				break;
			}
		}

		CxImage *mix = new CxImage();
		mix->Combine(newr,newg,newb,newa,dlg.pChS);
		
		CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
		if (NewDocr)	{
			NewDocr->image = mix;
			CString s;
			s.Format(_T("NewImage%d"),((CDemoApp*)AfxGetApp())->m_nDocCount++);
			NewDocr->SetTitle(s);
			NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}

		delete newr;
		delete newg;
		delete newb;
		delete newa;
		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageFft() 
{
	if (image==NULL) return;
	DlgFFT dlg;
	if (dlg.DoModal()==IDOK){
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);

		CxImage *srcr,*srci,*dstr,*dsti,tmp;

		srcr = (dlg.pDocReal) ? dlg.pDocReal->GetImage() : 0;
		srci = (dlg.pDocImag) ? dlg.pDocImag->GetImage() : 0;

		if (srcr==0 && srci==0) return;

		if (srcr) dstr = new CxImage(*srcr,true,false,false); else dstr=0;
		if (srci) dsti = new CxImage(*srci,true,false,false); else dsti=0;

		if (dstr==0){
			dstr = new CxImage(dsti->GetWidth(),dsti->GetHeight(),8);
			dstr->Clear(0);
			dstr->SetGrayPalette();
		}
		if (dsti==0){
			dsti = new CxImage(dstr->GetWidth(),dstr->GetHeight(),8);
			dsti->Clear(0);
			dsti->SetGrayPalette();
		}

		tmp.FFT2(dstr,dsti,0,0,dlg.bInverse,dlg.bForceFFT!=0,dlg.bMagnitude!=0);

		((CDemoApp*)AfxGetApp())->m_nDocCount++;
		CDemoDoc *NewDoci=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
		if (NewDoci)	{
			NewDoci->image = dsti;
			CString s;
			if (dlg.bMagnitude){
				s.Format(_T("FFT Phase %d"),((CDemoApp*)AfxGetApp())->m_nDocCount);
			} else {
				s.Format(_T("FFT Imag %d"),((CDemoApp*)AfxGetApp())->m_nDocCount);
			}
			NewDoci->SetTitle(s);
			NewDoci->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}
		CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
		if (NewDocr)	{
			NewDocr->image = dstr;
			CString s;
			if (dlg.bMagnitude){
				s.Format(_T("FFT Magnitude %d"),((CDemoApp*)AfxGetApp())->m_nDocCount);
			} else {
				s.Format(_T("FFT Real %d"),((CDemoApp*)AfxGetApp())->m_nDocCount);
			}
			NewDocr->SetTitle(s);
			NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}

		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplityiq() 
{
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	Stopwatch(0);

	image->SplitYIQ(newr,newg,newb);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format(_T("Y Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format(_T("I Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format(_T("Q Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSplitxyz() 
{
	if (image==NULL) return;

	CxImage *newr = new CxImage();
	CxImage *newg = new CxImage();
	CxImage *newb = new CxImage();

	Stopwatch(0);
	
	image->SplitXYZ(newr,newg,newb);
	
	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newr;
		CString s;
		s.Format(_T("X Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocg=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocg)	{
		NewDocg->image = newg;
		CString s;
		s.Format(_T("Y Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocg->SetTitle(s);
		NewDocg->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
	CDemoDoc *NewDocb=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocb)	{
		NewDocb->image = newb;
		CString s;
		s.Format(_T("Z Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocb->SetTitle(s);
		NewDocb->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRepair() 
{
	if (image==NULL) return;
	
	DlgRepair dlg;
	dlg.m_iterations = 2;
	dlg.m_radius = (float)0.25;
	if (dlg.DoModal()==IDOK){
		SubmitUndo();
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);

		image->Repair(dlg.m_radius,dlg.m_iterations,dlg.m_ncs);

		Stopwatch(1);
		UpdateAllViews(NULL);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageAlphachannelSplit() 
{
#if CXIMAGE_SUPPORT_ALPHA
	if (image==NULL) return;

	CxImage *newa = new CxImage();

	Stopwatch(0);
	
	image->AlphaSplit(newa);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newa;
		CString s;
		s.Format(_T("Alpha Channel %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageAlphaCreate() 
{
#if CXIMAGE_SUPPORT_ALPHA
	if (image==NULL) return;
	CxImage gray(*image,true,false,false);
	gray.IncreaseBpp(8);
	gray.Negative();
	gray.GrayScale();
	image->AlphaSet(gray);
	UpdateAllViews(NULL);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageFiltersLog() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_LOG;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageFiltersRoot() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_ROOT;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramEqualize() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_EQUALIZE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramNormalize() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_NORMALIZE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretch() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCH;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageGaussian3x3() 
{
	m_MenuCommand=ID_CXIMAGE_GAUSSIAN3X3;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageGaussian5x5() 
{
	m_MenuCommand=ID_CXIMAGE_GAUSSIAN5X5;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageContour() 
{
	m_MenuCommand=ID_CXIMAGE_CONTOUR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageTrace() 
{
	m_MenuCommand=ID_CXIMAGE_TRACE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageLesscontrast() 
{
	m_MenuCommand=ID_CXIMAGE_LESSCONTRAST;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageJitter() 
{
	m_MenuCommand=ID_CXIMAGE_JITTER;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnFiltersMix() 
{
	if (image==NULL) return;
	DlgMix dlg;
	if (dlg.DoModal()==IDOK){
		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);

		CxImage *src, *dst, *tmp;

		src = (dlg.pDocSrc) ? dlg.pDocSrc->GetImage() : 0;
		dst = (dlg.pDocDst) ? dlg.pDocDst->GetImage() : 0;

		if (src==0 && dst==0) return;

		tmp = new CxImage(*dst);
		tmp->Mix(*src,(CxImage::ImageOpType)dlg.OpType,dlg.m_xoffset,dlg.m_yoffset,dlg.m_mixalpha!=0);

		((CDemoApp*)AfxGetApp())->m_nDocCount++;
		CDemoDoc *NewDoci=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
		if (NewDoci)	{
			NewDoci->image = tmp;
			CString s;
			s.Format(_T("Mix %d"),((CDemoApp*)AfxGetApp())->m_nDocCount);
			NewDoci->SetTitle(s);
			NewDoci->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}

		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCircletransformCylinder() 
{
	m_MenuCommand=ID_CXIMAGE_CIRCLETRANSFORM_CYLINDER;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCircletransformPinch() 
{
	m_MenuCommand=ID_CXIMAGE_CIRCLETRANSFORM_PINCH;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCircletransformPunch() 
{
	m_MenuCommand=ID_CXIMAGE_CIRCLETRANSFORM_PUNCH;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCircletransformSwirl() 
{
	m_MenuCommand=ID_CXIMAGE_CIRCLETRANSFORM_SWIRL;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCircletransformBathroom() 
{
	m_MenuCommand=ID_CXIMAGE_CIRCLETRANSFORM_BATHROOM;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretch1() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCH1;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretch2() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCH2;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnFiltersNonlinearEdge() 
{
	m_MenuCommand=ID_FILTERS_NONLINEAR_EDGE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageSkew() 
{
	if (image==NULL) return;
	
	DlgSkew dlg;
	dlg.m_w = image->GetWidth();
	dlg.m_h = image->GetHeight();

	dlg.m_skewx = theApp.m_Filters.SkewX;
	dlg.m_skewy = theApp.m_Filters.SkewY;
	dlg.m_pivotx = theApp.m_Filters.SkewPivotX;
	dlg.m_pivoty = theApp.m_Filters.SkewPivotY;
	dlg.m_bEnableInterpolation = theApp.m_Filters.SkewInterp;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_CXIMAGE_SKEW;

		theApp.m_Filters.SkewX = dlg.m_skewx;
		theApp.m_Filters.SkewY = dlg.m_skewy;
		theApp.m_Filters.SkewSlopeX = dlg.m_slopex;
		theApp.m_Filters.SkewSlopeY = dlg.m_slopey;
		theApp.m_Filters.SkewPivotX = dlg.m_pivotx;
		theApp.m_Filters.SkewPivotY = dlg.m_pivoty;
		theApp.m_Filters.SkewInterp = dlg.m_bEnableInterpolation;

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsTracker(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==4);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsTracker() 
{
	m_tool=4;
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnJpegcompression() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ENCODE
	if (image==NULL) return;
	DlgJpeg dlg;
	dlg.m_quality=50.0f;
	if (dlg.DoModal()==IDOK){

		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);

		CxImage *tmp;
		tmp = new CxImage(*image);
		if (!tmp->IsGrayScale()) tmp->IncreaseBpp(24);
		tmp->SetTransIndex(-1);
		tmp->SetJpegQualityF(dlg.m_quality);

		DWORD imagetype = 0;
#if CXIMAGE_SUPPORT_JPG
		if (dlg.m_format==0) imagetype = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_JPC
		if (dlg.m_format==1) imagetype = CXIMAGE_FORMAT_JPC;
#endif

		CxMemFile tmpFile;
		tmpFile.Open();

		if (tmp->Encode(&tmpFile,imagetype)){

			tmpFile.Seek(0,SEEK_SET);
			if (tmp->Decode(&tmpFile,imagetype)){

				((CDemoApp*)AfxGetApp())->m_nDocCount++;
				CDemoDoc *NewDoc=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
				if (NewDoc)	{
					NewDoc->image = tmp;
					CString s;
					s.Format(_T("Jpeg compr. %d, q = %.3f, size = %d"),
							((CDemoApp*)AfxGetApp())->m_nDocCount, (double)dlg.m_quality, tmpFile.Size());
					NewDoc->SetTitle(s);
					NewDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
				}
			} else {
				CString s = tmp->GetLastError();
				AfxMessageBox(s);
				delete tmp;
			}
		} else {
			CString s = tmp->GetLastError();
			AfxMessageBox(s);
			delete tmp;
		}

		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewSmooth() 
{
	m_bSmoothDisplay = !m_bSmoothDisplay;
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bSmoothDisplay);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnFiltersDataext() 
{
	if (image==NULL) return;
	DlgDataExt dlg;

	dlg.m_image = image;

	dlg.m_Ymin = dlgDataExtInfos.fYmin;
	dlg.m_Ymax = dlgDataExtInfos.fYmax;
	dlg.m_Xmin = dlgDataExtInfos.fXmin;
	dlg.m_Xmax = dlgDataExtInfos.fXmax;
	dlg.m_thresh = dlgDataExtInfos.nThresh;
	dlg.m_color = dlgDataExtInfos.cColor;
	dlg.m_bMinmax = dlgDataExtInfos.bMinmax;
	dlg.m_bAvg = dlgDataExtInfos.bAvg;
	dlg.m_bDetect = dlgDataExtInfos.bDetect;
	dlg.m_bLogXaxis = dlgDataExtInfos.bLogXaxis;
	dlg.m_bLogYaxis = dlgDataExtInfos.bLogYaxis;


	if (dlg.DoModal()==IDOK){
		dlgDataExtInfos.fYmin = dlg.m_Ymin;
		dlgDataExtInfos.fYmax = dlg.m_Ymax;
		dlgDataExtInfos.fXmin = dlg.m_Xmin;
		dlgDataExtInfos.fXmax = dlg.m_Xmax ;
		dlgDataExtInfos.nThresh = dlg.m_thresh;
		dlgDataExtInfos.cColor = dlg.m_color;
		dlgDataExtInfos.bMinmax = dlg.m_bMinmax;
		dlgDataExtInfos.bAvg = dlg.m_bAvg;
		dlgDataExtInfos.bDetect = dlg.m_bDetect;
		dlgDataExtInfos.bLogXaxis = dlg.m_bLogXaxis;
		dlgDataExtInfos.bLogYaxis = dlg.m_bLogYaxis;
	}

}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageUnsharpmask() 
{
	m_MenuCommand=ID_CXIMAGE_UNSHARPMASK;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::RegionRotateLeft()
{
	long i,n;
	long h=image->GetHeight();
	for(i=0;i<m_NumSel;i++){
		n = m_Sel[i].x;
		m_Sel[i].x = m_Sel[i].y;
		m_Sel[i].y = h-1-n;
	}

}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::RegionRotateRight()
{
	long i,n;
	long w=image->GetWidth();
	for(i=0;i<m_NumSel;i++){
		n = m_Sel[i].y;
		m_Sel[i].y = m_Sel[i].x;
		m_Sel[i].x = w-1-n;
	}

}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageTextblur() 
{
	m_MenuCommand=ID_CXIMAGE_TEXTBLUR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRedeyeremove() 
{
	m_MenuCommand=ID_CXIMAGE_REDEYEREMOVE;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageBlurselborder() 
{
	m_MenuCommand=ID_CXIMAGE_BLURSELBORDER;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////

void CDemoDoc::OnCximageSelectiveblur() 
{
	m_MenuCommand=ID_CXIMAGE_SELECTIVEBLUR;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
/*void CDemoDoc::OnFiltersSka280x350() 
{
	if (image==NULL) return;

	SubmitUndo();

	image->IncreaseBpp(24);

	if (image->GetWidth()<280 && image->GetHeight()<350){
		image->Resample2((image->GetWidth()*350)/image->GetHeight(),350);
	}

	RGBQUAD canvas = {0,0,0,0};
	image->Thumbnail(280,350,canvas);

	long colors=232;
	RGBQUAD* ppal = NULL;
	CQuantizer q(colors,(colors>16?7:8));
	q.ProcessImage(image->GetDIB());
	ppal=(RGBQUAD*)calloc(256*sizeof(RGBQUAD),1);
	q.SetColorTable(ppal);

	int idx=0;
	for(; colors>0;colors--){
		ppal[colors+23]=ppal[colors];
		idx++;
	}

	const BYTE data[96] = {
			0x00,0x00,0x00,0x00,
			0xA8,0x00,0x00,0x00,
			0x00,0xA8,0x00,0x00,
			0xA8,0xA8,0x00,0x00,
			0x00,0x00,0xA8,0x00,
			0xA8,0x00,0xA8,0x00,
			0x00,0x54,0xA8,0x00,
			0xA8,0xA8,0xA8,0x00,
			0x54,0x54,0x54,0x00,
			0xFF,0x54,0x54,0x00,
			0x54,0xFF,0x54,0x00,
			0xFF,0xFF,0x54,0x00,
			0x54,0x54,0xFF,0x00,
			0xFF,0x54,0xFF,0x00,
			0x54,0xFF,0xFF,0x00,
			0xFF,0xFF,0xFF,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00
	};
	memcpy(ppal,data,96);

	image->DecreaseBpp(8,1,ppal,256);

	free(ppal);
	
	image->SetType(CXIMAGE_FORMAT_SKA);

	UpdateAllViews(0,WM_USER_NEWIMAGE);
	UpdateStatusBar();
}*/
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageGettransparencymask() 
{
	if (image==NULL) return;

	CxImage *newa = new CxImage();

	Stopwatch(0);

	image->GetTransparentMask(newa);

	Stopwatch(1);
	UpdateStatusBar();

	((CDemoApp*)AfxGetApp())->m_nDocCount++;
	CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
	if (NewDocr)	{
		NewDocr->image = newa;
		CString s;
		s.Format(_T("Transparency mask %d from %s"),((CDemoApp*)AfxGetApp())->m_nDocCount,GetTitle());
		NewDocr->SetTitle(s);
		NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsCountcolors() 
{
	if (image==NULL) return;

	SetCursor(LoadCursor(0,IDC_WAIT));
	Stopwatch(0);

	CQuantizer q(16777216,8);
	q.ProcessImage(image->GetDIB());

	Stopwatch(1);
	UpdateStatusBar();
	SetCursor(LoadCursor(0,IDC_ARROW));

	CString s;
	s.Format(_T("The number of colors in the active image is: %d"), q.GetColorCount());
	AfxMessageBox(s);

}
//////////////////////////////////////////////////////////////////////////////

void CDemoDoc::OnFiltersLinearCustom() 
{
	// <Priyank Bolia> priyank_bolia@yahoo.com
	// TODO: Add your command handler code here
	if (image==NULL) return;
	DlgCustomFilter dlg;

	memcpy(dlg.dlgkernel,theApp.m_Filters.Kernel5x5,25*sizeof(long));
	dlg.m_kSize = theApp.m_Filters.kSize;
	dlg.m_EditBias = theApp.m_Filters.kBias;
	dlg.m_EditDivisor = theApp.m_Filters.kDivisor;

	if (dlg.DoModal()==IDOK){
		m_MenuCommand=ID_FILTERS_LINEAR_CUSTOM;

		theApp.m_Filters.kDivisor = dlg.m_EditDivisor;
		theApp.m_Filters.kBias = dlg.m_EditBias;
		theApp.m_Filters.kSize = dlg.m_kSize;

		memcpy(theApp.m_Filters.Kernel5x5,dlg.dlgkernel,25*sizeof(long));

		theApp.m_Filters.Kernel3x3[0]=(long)dlg.dlgkernel[6];
		theApp.m_Filters.Kernel3x3[1]=(long)dlg.dlgkernel[7];
		theApp.m_Filters.Kernel3x3[2]=(long)dlg.dlgkernel[8];

		theApp.m_Filters.Kernel3x3[3]=(long)dlg.dlgkernel[11];
		theApp.m_Filters.Kernel3x3[4]=(long)dlg.dlgkernel[12];
		theApp.m_Filters.Kernel3x3[5]=(long)dlg.dlgkernel[13];

		theApp.m_Filters.Kernel3x3[6]=(long)dlg.dlgkernel[16];
		theApp.m_Filters.Kernel3x3[7]=(long)dlg.dlgkernel[17];
		theApp.m_Filters.Kernel3x3[8]=(long)dlg.dlgkernel[18];

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageCanvassize() 
{
	if (image==NULL) return;
	DlgExpand dlg;

	dlg.m_Mode = theApp.m_Filters.CanvasMode;
	dlg.m_CenterH = theApp.m_Filters.CanvasCenterH;
	dlg.m_CenterV = theApp.m_Filters.CanvasCenterV;
	dlg.m_KeepRatio = theApp.m_Filters.CanvasKeepRatio;
	dlg.m_UseImageBkg = theApp.m_Filters.CanvasUseImageBkg;
	dlg.m_color = theApp.m_Filters.CanvasBkg;
	dlg.m_newwidth = theApp.m_Filters.CanvasW;
	dlg.m_newheight = theApp.m_Filters.CanvasH;
	dlg.m_left = theApp.m_Filters.CanvasLeft;
	dlg.m_right = theApp.m_Filters.CanvasRight;
	dlg.m_top = theApp.m_Filters.CanvasTop;
	dlg.m_bottom = theApp.m_Filters.CanvasBottom;

	if (dlg.m_newwidth < (long)image->GetWidth()) dlg.m_newwidth = (long)image->GetWidth();
	if (dlg.m_newheight < (long)image->GetHeight()) dlg.m_newheight = (long)image->GetHeight();

	dlg.m_ratio = ((float)image->GetWidth())/((float)image->GetHeight());

	if (dlg.DoModal()==IDOK){

		m_MenuCommand=ID_CXIMAGE_CANVASSIZE;

		theApp.m_Filters.CanvasMode = dlg.m_Mode;
		theApp.m_Filters.CanvasCenterH = dlg.m_CenterH;
		theApp.m_Filters.CanvasCenterV = dlg.m_CenterV;
		theApp.m_Filters.CanvasKeepRatio = dlg.m_KeepRatio;
		theApp.m_Filters.CanvasUseImageBkg = dlg.m_UseImageBkg;
		theApp.m_Filters.CanvasBkg = dlg.m_color;
		theApp.m_Filters.CanvasW = dlg.m_newwidth;
		theApp.m_Filters.CanvasH = dlg.m_newheight;
		theApp.m_Filters.CanvasLeft = dlg.m_left;
		theApp.m_Filters.CanvasRight = dlg.m_right;
		theApp.m_Filters.CanvasTop = dlg.m_top;
		theApp.m_Filters.CanvasBottom = dlg.m_bottom;

		if (dlg.m_Mode == 0 && ((dlg.m_newwidth < (long)image->GetWidth()) || (dlg.m_newheight < (long)image->GetHeight()))){
			AfxMessageBox(_T("New canvas size must be greater than the original"));
			return;
		}
		if (dlg.m_Mode == 1 && (dlg.m_left<0 || dlg.m_right<0 || dlg.m_top<0 || dlg.m_bottom<0)){
			AfxMessageBox(_T("New canvas size must be greater than the original"));
			return;
		}

		hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
	}

}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewToolsFloodfill(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_tool==5);
	DlgFloodFill* pDlg = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_pDlgFlood;
	if (pDlg && pDlg->GetSafeHwnd() && m_tool!=5){
		pDlg->ShowWindow(SW_HIDE);
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewToolsFloodfill() 
{
	m_tool=5;
	DlgFloodFill* pDlg = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_pDlgFlood;
	if (pDlg){
		if (pDlg->GetSafeHwnd()==0){
			pDlg->m_color = RGB(theApp.m_FloodColor.rgbRed, theApp.m_FloodColor.rgbGreen, theApp.m_FloodColor.rgbBlue);
			pDlg->m_tol = theApp.m_FloodTolerance;
			pDlg->m_opacity = theApp.m_FloodOpacity;
			pDlg->m_select = theApp.m_FloodSelect;
			pDlg->Create();
		} else {
			pDlg->ShowWindow(SW_SHOW);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageRemoveselection() 
{
	if (image==NULL) return;
#if CXIMAGE_SUPPORT_SELECTION
	image->SelectionDelete();

	POSITION pos = GetFirstViewPosition();
	CDemoView *pView = (CDemoView *)GetNextView(pos);
	if (pView) pView->KillTimer(1);
	pView->m_SelShift=0;
	m_NumSel=0;
#endif //CXIMAGE_SUPPORT_SELECTION

	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsMoresaturationhsl() 
{
	m_MenuCommand=ID_COLORS_MORESATURATIONHSL;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsMoresaturationyuv() 
{
	m_MenuCommand=ID_COLORS_MORESATURATIONYUV;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsLesssaturation() 
{
	m_MenuCommand=ID_COLORS_LESSSATURATION;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsHistogramFullsaturation() 
{
	m_MenuCommand=ID_COLORS_HISTOGRAM_FULLSATURATION;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsHistogramHalfsaturation() 
{
	m_MenuCommand=ID_COLORS_HISTOGRAM_HALFSATURATION;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretcht0() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCHT0;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretcht1() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCHT1;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnCximageHistogramStretcht2() 
{
	m_MenuCommand=ID_CXIMAGE_HISTOGRAM_STRETCHT2;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnColorsAdaptivethreshold() 
{
	m_MenuCommand=ID_COLORS_ADAPTIVETHRESHOLD;
	hThread=(HANDLE)_beginthread(RunCxImageThread,0,this);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewPreviousframe() 
{
	long m = image->GetNumFrames();
	long n = image->GetFrame()-1;
	if (n<0) n=m-1;
	if (image->GetFrame(n))
		image->Copy(*image->GetFrame(n));
	image->SetFrame(n);
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewNextframe() 
{
	long m = image->GetNumFrames();
	long n = image->GetFrame()+1;
	if (n>=m) n=0;
	if (image->GetFrame(n))
		image->Copy(*image->GetFrame(n));
	image->SetFrame(n);
	UpdateAllViews(NULL);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnUpdateViewPlayanimation(CCmdUI* pCmdUI) 
{
	if(image==0 || hThread || image->GetFrame(0)==0) pCmdUI->Enable(0);
	pCmdUI->SetCheck(m_playanimation);
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnViewPlayanimation() 
{
	m_playanimation = 1-m_playanimation;

	POSITION pos = GetFirstViewPosition();
	CDemoView *pView = (CDemoView *)GetNextView(pos);
	if (pView){
		if (m_playanimation){
			pView->SetTimer(2,200,NULL);
		} else {
			pView->KillTimer(2);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::PlayNextFrame()
{
	POSITION pos = GetFirstViewPosition();
	CDemoView *pView = (CDemoView *)GetNextView(pos);
	if (pView && image){
		pView->KillTimer(2);
		pView->SetTimer(2,10*max(1,image->GetFrameDelay()),NULL);
	}
	OnViewNextframe();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoDoc::OnFiltersAddshadow() 
{
	if (image==NULL) return;
#if CXIMAGE_SUPPORT_SELECTION
	DlgShadow dlg;

	dlg.m_x = theApp.m_Filters.ShadowX;
	dlg.m_y = theApp.m_Filters.ShadowY;
	dlg.m_radius = theApp.m_Filters.ShadowR;
	dlg.m_shadow = theApp.m_Filters.ShadowColor;
	dlg.m_bkg = theApp.m_Filters.ShadowBkg;
	dlg.m_intensity = theApp.m_Filters.ShadowIntensity;
	dlg.m_relative = theApp.m_Filters.ShadowRelative;

	if (dlg.DoModal()==IDOK){

		theApp.m_Filters.ShadowX = dlg.m_x;
		theApp.m_Filters.ShadowY = dlg.m_y;
		theApp.m_Filters.ShadowR = dlg.m_radius;
		theApp.m_Filters.ShadowColor = dlg.m_shadow;
		theApp.m_Filters.ShadowBkg = dlg.m_bkg;
		theApp.m_Filters.ShadowIntensity = dlg.m_intensity;
		theApp.m_Filters.ShadowRelative = dlg.m_relative;

		SetCursor(LoadCursor(0,IDC_WAIT));
		Stopwatch(0);

		RGBQUAD c0 = CxImage::RGBtoRGBQUAD(theApp.m_Filters.ShadowColor);
		RGBQUAD c1 = CxImage::RGBtoRGBQUAD(theApp.m_Filters.ShadowBkg);

		CxImage *mix = new CxImage(*image);
		mix->IncreaseBpp(24);
		mix->SelectionClear();
		mix->SelectionAddColor(c1);
		CxImage iShadow;
		mix->SelectionSplit(&iShadow);
		mix->SelectionDelete();

		if (theApp.m_Filters.ShadowRelative){
			CxImage gray(*image);
			gray.GrayScale();
			iShadow.Mix(gray,CxImage::OpOr);
		}

		iShadow.GaussianBlur(theApp.m_Filters.ShadowR);

		for (int n = 0; n<256; n++){
			BYTE r = (BYTE)(c1.rgbRed   + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbRed   - (long)c1.rgbRed))>>16));
			BYTE g = (BYTE)(c1.rgbGreen + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbGreen - (long)c1.rgbGreen))>>16));
			BYTE b = (BYTE)(c1.rgbBlue  + ((theApp.m_Filters.ShadowIntensity*n*((long)c0.rgbBlue  - (long)c1.rgbBlue))>>16));
			iShadow.SetPaletteColor((BYTE)(255-n),r,g,b);
		}

		mix->SetTransColor(c1);
		mix->SetTransIndex(0);
		mix->Mix(iShadow,CxImage::OpSrcCopy,theApp.m_Filters.ShadowX,theApp.m_Filters.ShadowY);
		//mix->Transfer(iShadow);
		mix->SetTransIndex(-1);

		CDemoDoc *NewDocr=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL); 
		if (NewDocr)	{
			NewDocr->image = mix;
			CString s;
			s.Format(_T("NewImage%d"),((CDemoApp*)AfxGetApp())->m_nDocCount++);
			NewDocr->SetTitle(s);
			NewDocr->UpdateAllViews(0,WM_USER_NEWIMAGE);
		}

		Stopwatch(1);
		UpdateStatusBar();
		SetCursor(LoadCursor(0,IDC_ARROW));
	}
#endif //CXIMAGE_SUPPORT_SELECTION
}
//////////////////////////////////////////////////////////////////////////////
