// demo.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "demo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "demoDoc.h"
#include "demoView.h"
#include "xTargetButton.h"
#include "DlgURL.h"
#include "DlgOptions.h"

#include "ximage.h"
#include "ximagif.h"
#include "ximatif.h"
#include "xfile.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

DocType doctypes[CMAX_IMAGE_FORMATS] =
{
	{ -1, TRUE, TRUE, _T("Supported files"), _T("*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf;*.psd") },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng;*.png") },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, _T("WBMP files"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
	{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA files"), _T("*.ska") },
#endif
#if CXIMAGE_SUPPORT_RAW
	{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW files"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#endif
#if CXIMAGE_SUPPORT_PSD
	{ CXIMAGE_FORMAT_PSD, TRUE, FALSE, _T("PSD files"), _T("*.psd") }
#endif
};

DlgDataExtInfo dlgDataExtInfos = {0.0f,100.0f,0.0f,100.0f,200,0,true,true,true,false,false};

//////////////////////////////////////////////////////////////////////////////
// CDemoApp
BEGIN_MESSAGE_MAP(CDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CDemoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_IMAGE_FROM_BITMAP, OnImageFromBitmap)
	ON_COMMAND(ID_CXIMAGE_LOADJPEGRESOURCE, OnCximageLoadjpegresource)
	ON_COMMAND(ID_CXIMAGE_LOADICONRESOURCE, OnCximageLoadiconresource)
	ON_COMMAND(ID_CXIMAGE_LOADGIFRESOURCE, OnCximageLoadgifresource)
	ON_COMMAND(ID_CXIMAGE_LOADPNGRESOURCE, OnCximageLoadpngresource)
	ON_COMMAND(ID_CXIMAGE_LOADTIFRESOURCE, OnCximageLoadtifresource)
	ON_COMMAND(ID_WINDOW_CLOSEALL, OnWindowCloseall)
	ON_COMMAND(ID_CXIMAGE_DEMOSAVEMULTIPAGETIFF, OnCximageDemosavemultipagetiff)
	ON_COMMAND(ID_CXIMAGE_DEMOSAVEANIMATEDGIF, OnCximageDemosaveanimatedgif)
	ON_COMMAND(ID_CXIMAGE_DOWNLOADFROMURL, OnCximageDownloadfromurl)
	ON_COMMAND(ID_CXIMAGE_DEMOCREATEFROMARRAY, OnCximageDemocreatefromarray)
	ON_COMMAND(ID_CXIMAGE_DEMOSAVEMULTIPAGEICON, OnCximageDemosavemultipageicon)
	ON_COMMAND(ID_CXIMAGE_OPTIONS, OnCximageOptions)
	ON_COMMAND(ID_HELP_CXIMAGEONLINE_WWWXDPIT, OnHelpCximageonlineWwwxdpit)
	ON_COMMAND(ID_HELP_CXIMAGEONLINE_WWWTHECODEPROJECTCOM, OnHelpCximageonlineWwwthecodeprojectcom)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CDemoApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// CDemoApp construction
CDemoApp::CDemoApp()
{
	m_nDocCount=1; //counter for documents
	nDocType = 0;

}
//////////////////////////////////////////////////////////////////////////////
// The one and only CDemoApp object
CDemoApp theApp;

//////////////////////////////////////////////////////////////////////////////
// CDemoApp initialization
BOOL CDemoApp::InitInstance()
{
	// <dave> dump memory leaks
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

#if _MSC_VER <= 1200
	// Standard initialization
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif

	LoadStdProfileSettings(9);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DEMOTYPE,
		RUNTIME_CLASS(CDemoDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDemoView));
	demoTemplate = pDocTemplate;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME)) return FALSE;
	m_pMainWnd = pMainFrame;

	// This code replaces the MFC created menus with 
	// the Ownerdrawn versions 
	pDocTemplate->m_hMenuShared=pMainFrame->NewMenu();
	pMainFrame->m_hMenuDefault=pMainFrame->NewDefaultMenu();
	// This simulates a window being opened if you don't have
	// a default window displayed at startup
	pMainFrame->OnUpdateFrameMenu(pMainFrame->m_hMenuDefault);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
    // Alter behaviour to not open window immediately
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo)) return FALSE;

#ifdef VATI_EXTENSIONS
    // init m_text by a temporary CxImage 
	CxImage *ima = new CxImage;
	ima->InitTextInfo( &m_text );
	delete ima;
    
	//recall last used font data for Text tool
    _stprintf( m_text.lfont.lfFaceName, GetProfileString ( _T("TextTool"), _T("lfFaceName"), _T("Times New Roman")) );
	m_text.lfont.lfCharSet   = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfCharSet"), EASTEUROPE_CHARSET ) ;
    m_text.lfont.lfWeight    = GetProfileInt ( _T("TextTool"), _T("lfWeight"), 0 );
    m_text.lfont.lfItalic    = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfItalic"), 0 ); 
    m_text.lfont.lfUnderline = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfUnderline"), 0 ); 
    m_text.fcolor = GetProfileInt ( _T("TextTool"), _T("fcolor"), RGB( 255,255,160 ));
    m_text.bcolor = GetProfileInt ( _T("TextTool"), _T("bcolor"), RGB(   0, 80,160 ));
    m_text.opaque = (BYTE)GetProfileInt ( _T("TextTool"), _T("opaque"), 1);
    m_text.b_opacity = (float)(GetProfileInt( _T("TextTool"), _T("opacity"), 0 ))/(float)100.;  
    m_text.b_round   = (BYTE)GetProfileInt ( _T("TextTool"), _T("roundradius"), 25 );
    m_text.smooth    = (BYTE)GetProfileInt ( _T("TextTool"), _T("antialias"), 1 );

	m_optJpegQuality = GetProfileInt(_T("Options"),_T("JpegQualityI"),90) + 0.001f * GetProfileInt(_T("Options"),_T("JpegQualityF"),0);
	m_optJpegOptions = GetProfileInt(_T("Options"),_T("JpegOptions"),0);

	m_optRawOptions = GetProfileInt(_T("Options"),_T("RawOptions"),0);

	nDocType = GetProfileInt(_T("General"),_T("DocType"),0);

	BCMenu::SetMenuDrawMode(GetProfileInt(_T("General"),_T("MenuStyle"),1));

	m_FloodColor.rgbBlue = 255;
	m_FloodColor.rgbGreen = 255;
	m_FloodColor.rgbRed = 255;
	m_FloodColor.rgbReserved = 0;
	m_FloodTolerance = 0;
	m_FloodOpacity = 255;
	m_FloodSelect = 0;

	int i;
	for(i=0;i<25;i++) m_Filters.Kernel5x5[i]=1;
	for(i=0;i<9;i++) m_Filters.Kernel3x3[i]=1;
	m_Filters.kSize = 3;
	m_Filters.kBias = 0;
	m_Filters.kDivisor = 9;

	m_Filters.RotateAngle = 12.345f;
	m_Filters.RotateMethod = 1;
	m_Filters.RotateOverflow = 0;
	m_Filters.RotateKeepsize = 1;

	m_Filters.ResampleSizemode = 1;
	m_Filters.ResampleFactor = 2;
	m_Filters.ResampleW = 100;
	m_Filters.ResampleH = 100;
	m_Filters.ResampleKeepRatio = 1;
	m_Filters.ResampleMethod = 2;

	m_Filters.SkewPivotX = 0;
	m_Filters.SkewPivotY = 0;
	m_Filters.SkewX =  5.5f;
	m_Filters.SkewY = -5.0f;
	m_Filters.SkewInterp = 1;

	m_Filters.DitherMethod = 0;

	m_Filters.ThreshLevel = 128;
	m_Filters.ThreshPreserveColors = 0;

	m_Filters.ColorMode = 0;
	m_Filters.ColorHSL.rgbBlue = 50;
	m_Filters.ColorHSL.rgbGreen = 12;
	m_Filters.ColorHSL.rgbRed = 50;
	m_Filters.ColorHSL.rgbReserved = 0;
	m_Filters.ColorBlue = -50;
	m_Filters.ColorGreen = 0;
	m_Filters.ColorRed = 50;
	m_Filters.ColorSolarLevel = 128;
	m_Filters.ColorSolarLink = 1;

	m_Filters.GammaLevel = 1.23f;
	m_Filters.GammaR = 0.9f;
	m_Filters.GammaG = 1.1f;
	m_Filters.GammaB = 1.2f;
	m_Filters.GammaLink = 0;

	m_Filters.IncBppBPP = 24;
	m_Filters.DecBppBPP = 4;
	m_Filters.DecBppPalMethod = 1;
	m_Filters.DecBppErrDiff = 0;
	m_Filters.DecBppLimitColors = 0;
	m_Filters.DecBppMaxColors = 256;

	m_Filters.CanvasMode = 0;
	m_Filters.CanvasW = 800;
	m_Filters.CanvasH = 600;
	m_Filters.CanvasLeft = 20;
	m_Filters.CanvasRight = 20;
	m_Filters.CanvasTop = 10;
	m_Filters.CanvasBottom = 10;
	m_Filters.CanvasCenterH = 1;
	m_Filters.CanvasCenterV = 1;
	m_Filters.CanvasKeepRatio = 0;
	m_Filters.CanvasUseImageBkg = 0;
	m_Filters.CanvasBkg = RGB(255,255,255);

	m_Filters.ShadowX = -5;
	m_Filters.ShadowY = 5;
	m_Filters.ShadowR = 7;
	m_Filters.ShadowColor = RGB(0,0,0);
	m_Filters.ShadowBkg = RGB(255,255,255);
	m_Filters.ShadowIntensity = 128;
	m_Filters.ShadowRelative = 0;

	// recall if main window was maximized on last exit
    if ( GetProfileInt ( _T("Screen"), _T("maximized"), 0 ))
        m_nCmdShow|=SW_MAXIMIZE;
#endif;

	// The main window has been initialized, so show and update it.
    pMainFrame -> ShowWindow ( m_nCmdShow ) ;
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Register our Clipboard format name
	m_CF = ::RegisterClipboardFormat(_T("CF_CXIMAGE"));

	// Enable open from command line
	if (*m_lpCmdLine != 0)
		OpenDocumentFile(m_lpCmdLine);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CxTargetButton	m_ok;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}
//////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCapture)
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}
//////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ok.SetIcon(IDI_G,BS_LEFT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//////////////////////////////////////////////////////////////////////////////
// App command to run the dialog
void CDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnFileOpen() 
{
	// prompt the user (with all document templates)
	CString newName;
	int nDocType = -1;
	if (!PromptForFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, &nDocType))
		return; // open cancelled
	
	OpenDocumentFile(newName);

}
//////////////////////////////////////////////////////////////////////////////
// prompt for file name - used for open and save as
// static function called from app
BOOL CDemoApp::PromptForFileName(CString& fileName, UINT /*nIDSTitle*/, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open image file"; else title="Save image file");

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : CXIMAGE_FORMAT_UNKNOWN;
	if (nDocType==0) nDocType=1;

	int nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
	if (nIndex == -1) nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = nIndex +1;
	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(nDocType).Mid(2,3);
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet){
		if (pType != NULL){
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
		}
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////
int CDemoApp::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (doctypes[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
int CDemoApp::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite){
			if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
                return doctypes[i].nID;
			nCnt++;
		}
	}
	ASSERT(FALSE);
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
CString CDemoApp::GetExtFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].ext;
	}
	return CString(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
CString CDemoApp::GetDescFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].description;
	}
	return CString(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoApp::GetWritableType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (doctypes[i].nID == nDocType)
			return doctypes[i].bWrite;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
CString CDemoApp::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && doctypes[i].bRead){
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		} else if (!bOpenFileDialog && doctypes[i].bWrite) {
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		}
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////////
// CDemoApp commands
void CDemoApp::OnImageFromBitmap() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_BMP
/*	HBITMAP bitmap;
	if (!(bitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1)))){
		AfxMessageBox(_T("Could not load bitmap from resource"));
		return;
	}*/

	CxImage* newImage = new CxImage();
	HRSRC handle = FindResource(NULL,MAKEINTRESOURCE(IDB_BITMAP1),RT_BITMAP);
	if (!newImage->LoadResource(handle,CXIMAGE_FORMAT_BMP)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}

	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;
		CString s;
		s.Format(_T("BITMAP Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageLoadjpegresource() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_JPG
	CxImage* newImage = new CxImage();
	if (!newImage->LoadResource(FindResource(NULL,_T("IDR_JPG2"),_T("JPG")),CXIMAGE_FORMAT_JPG)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}
	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;
		CString s;
		s.Format(_T("JPEG Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageLoadiconresource() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ICO
	CxImage* newImage = new CxImage();
#if CXIMAGE_SUPPORT_ICO
	if (!newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_ICO1),_T("ICO")),CXIMAGE_FORMAT_ICO)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}
#endif
//	HICON my_Icon = (HICON)::LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_G),IMAGE_ICON,0, 0, 0);
//	CxImage my_image;
//	my_image.CreateFromHICON(my_Icon);
//	DestroyIcon(my_Icon);
//	*newImage = my_image;

	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;
		CString s;
		s.Format(_T("Icon Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageLoadgifresource() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_GIF
	CxImage* newImage = new CxImage();
	if (!newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_GIF1),_T("GIF")),CXIMAGE_FORMAT_GIF)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}
	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;

		CString s;
		s.Format(_T("GIF Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageLoadpngresource() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_PNG
	CxImage* newImage = new CxImage();
	if (!newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG1),_T("PNG")),CXIMAGE_FORMAT_PNG)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}

	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;

		CString s;
		s.Format(_T("PNG Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageLoadtifresource() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_TIF
	CxImage* newImage = new CxImage();
#if CXIMAGE_SUPPORT_TIF
	if (!newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_TIF1),_T("TIF")),CXIMAGE_FORMAT_TIF)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}
#endif
	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;

		CString s;
		s.Format(_T("TIF Resource Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0,WM_USER_NEWIMAGE);
		doc->UpdateStatusBar();
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnWindowCloseall() 
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition(); 
    while(pos) { 
        CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos); 
        POSITION pos2 = pTemplate->GetFirstDocPosition(); 
        while(pos2) { 
            CDocument* pDocument; 
			if ((pDocument=pTemplate->GetNextDoc(pos2)) != NULL) 
				pDocument->OnCloseDocument();  
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageDemosavemultipagetiff() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ENCODE && CXIMAGE_SUPPORT_TIF && CXIMAGE_SUPPORT_GIF && CXIMAGE_SUPPORT_JPG

	CxImage* newImage1 = new CxImage();
	if (!newImage1->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_TIF1),_T("TIF")),CXIMAGE_FORMAT_TIF)){
		CString s = newImage1->GetLastError();
		AfxMessageBox(s);
		delete newImage1;
		newImage1 =NULL;
		return;
	}
	CxImage* newImage2 = new CxImage();
	if (!newImage2->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_GIF1),_T("GIF")),CXIMAGE_FORMAT_GIF)){
		CString s = newImage2->GetLastError();
		AfxMessageBox(s);
		delete newImage2;
		newImage2 =NULL;
		return;
	}
	CxImage* newImage3 = new CxImage();
	if (!newImage3->LoadResource(FindResource(NULL,_T("IDR_JPG2"),_T("JPG")),CXIMAGE_FORMAT_JPG)){
		CString s = newImage3->GetLastError();
		AfxMessageBox(s);
		delete newImage3;
		newImage3 =NULL;
		return;
	}
	newImage3->DecreaseBpp(8,true);

	CxImage *pimage[3];
	pimage[0]=newImage1;
	pimage[1]=newImage2;
	pimage[2]=newImage3;

	FILE* hFile;
	hFile = _tfopen(_T("multipage.tif"),_T("w+b"));

	CxImage multiimage;
	multiimage.Encode(hFile,pimage,3,CXIMAGE_FORMAT_TIF);

	fclose(hFile);

	delete newImage1;
	delete newImage2;
	delete newImage3;

	/*{
	FILE* hFile;
	hFile = _tfopen("c:\\multi.tif",_T("w+b"));

	CxImageTIF image;
	image.Load(_T("c:\\1.tif"),CXIMAGE_FORMAT_TIF);
	image.Encode(hFile,true);
	image.Load(_T("c:\\2.bmp"),CXIMAGE_FORMAT_BMP);
	image.Encode(hFile,true);
	image.Load(_T("c:\\3.png"),CXIMAGE_FORMAT_PNG);
	image.Encode(hFile);

	fclose(hFile);
	}*/

	AfxMessageBox(_T("file 'mulipage.tif' saved to disk"),MB_OK);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageDemosaveanimatedgif() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ENCODE && CXIMAGE_SUPPORT_GIF
	CxImage* newImage = new CxImage();
	if (!newImage->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_GIF1),_T("GIF")),CXIMAGE_FORMAT_GIF)){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
	}
	
	newImage->IncreaseBpp(24);

	CxImage ima1,ima2,ima3;
	ima1.Copy(*newImage);
	ima1.Light(22);
	ima2.Copy(*newImage);
	ima2.Light(44);
	ima2.Flip();
	ima3.Copy(*newImage);
	ima3.Light(66);

	newImage->DecreaseBpp(8,0,0);
	ima1.DecreaseBpp(8,0,0);
	ima2.DecreaseBpp(8,0,0);
	ima3.DecreaseBpp(8,0,0);

	newImage->SetTransIndex(newImage->GetPixelIndex(0,0));
	ima1.SetTransIndex(ima1.GetPixelIndex(0,0));
	ima2.SetTransIndex(ima2.GetPixelIndex(0,0));
	ima3.SetTransIndex(ima3.GetPixelIndex(0,0));

	newImage->SetFrameDelay(22);
	ima1.SetFrameDelay(22);
	ima2.SetFrameDelay(22);
	ima3.SetFrameDelay(44);

	ima1.SetDisposalMethod(0);
	ima2.SetDisposalMethod(0);
	ima3.SetDisposalMethod(0);

	CxImage *pimage[7];
	pimage[0]=newImage;
	pimage[1]=&ima1;
	pimage[2]=&ima2;
	pimage[3]=&ima3;
	pimage[4]=&ima2;
	pimage[5]=&ima1;
	pimage[6]=newImage;

	CxIOFile hFile;
	hFile.Open(_T("ani.gif"),_T("wb"));

	CxImageGIF multiimage;
	multiimage.SetComment("CxImage demo");
	multiimage.SetLoops(3);
	multiimage.SetDisposalMethod(2);
	multiimage.Encode(&hFile,pimage,7,false,false);

	hFile.Close();

	delete newImage;

	AfxMessageBox(_T("file 'ani.gif' saved to disk"),MB_OK);
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageDemosavemultipageicon() 
{
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_ENCODE && CXIMAGE_SUPPORT_BMP && CXIMAGE_SUPPORT_ICO

	CxImage* icon = new CxImage();
	if (!icon->LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDB_BITMAP1),RT_BITMAP),CXIMAGE_FORMAT_BMP)){
		CString s = icon->GetLastError();
		AfxMessageBox(s);
		delete icon;
		icon =NULL;
		return;
	}

	RGBQUAD c = {255,255,255,0};
	icon->SetTransIndex(icon->GetNearestIndex(c));

	CxImage gray,ico2,ico3;

	ico2.Copy(*icon);
	ico2.IncreaseBpp(24);
	ico2.Resample(128,128);
#if CXIMAGE_SUPPORT_ALPHA
	gray.Copy(ico2);
	gray.IncreaseBpp(8);
	gray.Negative();
	gray.GrayScale();
	ico2.AlphaSet(gray);
#endif

	ico3.Copy(*icon);
	ico3.IncreaseBpp(8);
	ico3.Resample(256,256);
#if CXIMAGE_SUPPORT_ALPHA
	gray.Copy(ico3);
	gray.IncreaseBpp(8);
	gray.Negative();
	gray.GrayScale();
	ico3.AlphaSet(gray);
#endif

	icon->Resample(32,32);

	CxImage *pimage[3];
	pimage[0]=icon;
	pimage[1]=&ico2;
	pimage[2]=&ico3;

	FILE* hFile;
	hFile = _tfopen(_T("multipage.ico"),_T("w+b"));

	CxImage multiicon;
	multiicon.Encode(hFile,pimage,3,CXIMAGE_FORMAT_ICO);

	fclose(hFile);

	delete icon;

	AfxMessageBox(_T("file 'mulipage.ico' saved to disk"),MB_OK);

#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageDownloadfromurl() 
{
#if CXIMAGE_SUPPORT_DECODE
	DlgURL dlg;
	if (dlg.DoModal()==IDOK){
		long size = dlg.m_size;
		if (size){
			CxMemFile memfile((BYTE*)dlg.m_data.GetBuffer(0),size);

			CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
			if (doc){
				CString ext(doc->FindExtension(dlg.m_url));
				ext.MakeLower();
				if (ext == _T("")) return;

				int type = doc->FindType(ext);

				CxImage* ima = new CxImage();
				ima->Decode(&memfile,type);

				doc->image = ima;
				doc->SetTitle(dlg.m_url);
				doc->UpdateAllViews(0);
				doc->UpdateStatusBar();




				if (ima->GetNumFrames()>1){
					CString s;
					s.Format(_T("File with %d images. Read all?"),ima->GetNumFrames());
					if (AfxMessageBox(s,MB_OKCANCEL)==IDOK){
						
						int j; // points to the document name
						for(j=_tcslen(dlg.m_url)-1;j>=0;j--){
							if (dlg.m_url[j]=='\\'){	j++; break;	}
						}
						// create the documents for the other images
						for(int i=1;i<ima->GetNumFrames();i++){
							CDemoDoc *NewDoc=(CDemoDoc*)((CDemoApp*)AfxGetApp())->demoTemplate->OpenDocumentFile(NULL);
							if (NewDoc)	{
								CxImage *newImage = new CxImage();
								newImage->SetFrame(i);
								memfile.Seek(0,SEEK_SET);
								newImage->Decode(&memfile,type);
								NewDoc->image = newImage;
								CString s;
								s.Format(_T("%s (%d)"),dlg.m_url.Mid(j),i+1);
								NewDoc->SetTitle(s);
								NewDoc->UpdateAllViews(NULL,WM_USER_NEWIMAGE);
							}
						}
					}
				}
			}
		}
	}
#endif
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageDemocreatefromarray() 
{
	CxImage* newImage = new CxImage();

	long w = 256;
	long h = 256;
	BYTE c = 4;

	BYTE* pArray = (BYTE*)malloc(w*h*c);

	int n,d,e;
	int a=w/2;
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			n=c*(j+w*i);
			d=(BYTE)((a*i)/h+(a*(w-j))/w);
			e=(BYTE)((a*(h-i))/h+(a*(w-j))/w);
			if (c==4) pArray[n+3]=(BYTE)(d);
			pArray[n+2]=(BYTE)(e);
			pArray[n+1]=(BYTE)(255-d);
			pArray[n]=(BYTE)(255-e);
		}
	}

	newImage->CreateFromArray(pArray,w,h,8*c,c*w,0);
	free(pArray);

	if (!newImage->IsValid()){
		CString s = newImage->GetLastError();
		AfxMessageBox(s);
		delete newImage;
		newImage =NULL;
		return;
	}

	CDemoDoc *doc = (CDemoDoc *)demoTemplate->OpenDocumentFile(NULL);
	if (doc){
		doc->image = newImage;

		CString s;
		s.Format(_T("CreateFromArray Image %d"),m_nDocCount++);
		doc->SetTitle(s);
		doc->UpdateAllViews(0);
		doc->UpdateStatusBar();
	}
}
//////////////////////////////////////////////////////////////////////////////
void CDemoApp::OnCximageOptions() 
{
	CxImage* image = new CxImage();

	if (image==NULL) return;

	DlgOptions dlg;
	dlg.m_jpeg_quality = theApp.m_optJpegQuality;
	dlg.m_xres = image->GetXDPI();
	dlg.m_yres = image->GetYDPI();

#if CXIMAGE_SUPPORT_TIF
	dlg.m_Opt_tif = image->GetCodecOption(CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
	dlg.m_Opt_gif = image->GetCodecOption(CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_PNG
	dlg.m_Opt_png = image->GetCodecOption(CXIMAGE_FORMAT_PNG);
#endif

	dlg.m_Opt_jpg = theApp.m_optJpegOptions;
	dlg.m_Opt_raw = theApp.m_optRawOptions;
#if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
	dlg.m_exif = 0;
#endif
	if (dlg.DoModal()==IDOK){
#ifdef VATI_EXTENSIONS
		theApp.m_optJpegQuality = dlg.m_jpeg_quality;
		theApp.m_optJpegOptions = dlg.m_Opt_jpg;
		theApp.m_optRawOptions  = dlg.m_Opt_raw;
#endif
	}

	delete image;
}

void CDemoApp::OnHelpCximageonlineWwwxdpit() 
{
	// TODO: Add your command handler code here
	SHELLEXECUTEINFO s;
	s.cbSize=sizeof(SHELLEXECUTEINFO);
	s.fMask=SEE_MASK_FLAG_NO_UI;
	s.hwnd=AfxGetMainWnd()->m_hWnd;
	s.lpVerb=_T("Open");
	s.lpFile=_T("iexplore.exe");
	s.lpParameters=_T("http://www.xdp.it/cximage/");
	s.lpDirectory=NULL;
	s.nShow=SW_NORMAL;
	ShellExecuteEx(&s);
}

void CDemoApp::OnHelpCximageonlineWwwthecodeprojectcom() 
{
	// TODO: Add your command handler code here
	SHELLEXECUTEINFO s;
	s.cbSize=sizeof(SHELLEXECUTEINFO);
	s.fMask=SEE_MASK_FLAG_NO_UI;
	s.hwnd=AfxGetMainWnd()->m_hWnd;
	s.lpVerb=_T("Open");
	s.lpFile=_T("iexplore.exe");
	s.lpParameters=_T("http://www.codeproject.com/KB/graphics/cximage.aspx");
	s.lpDirectory=NULL;
	s.nShow=SW_NORMAL;
	ShellExecuteEx(&s);
}
