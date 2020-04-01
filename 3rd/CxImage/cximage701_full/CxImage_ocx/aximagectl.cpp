// aximagectl.cpp : Implementation of the AxImage ActiveX Control class.

#include "stdafx.h"
#include "AxImage.h"
#include "aximagectl.h"
#include "AxImagePpg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(AxImage, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(AxImage, COleControl)
	//{{AFX_MSG_MAP(AxImage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(OCM_COMMAND, OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(AxImage, COleControl)
	//{{AFX_DISPATCH_MAP(AxImage)
	DISP_PROPERTY_NOTIFY(AxImage, "Border", m_border, OnBorderChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(AxImage, "Stretch", m_stretch, OnStretchChanged, VT_BOOL)
	DISP_FUNCTION(AxImage, "Load", Load, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(AxImage, "Save", Save, VT_I4, VTS_BSTR VTS_I4)
	DISP_FUNCTION(AxImage, "CreateFromHandle", CreateFromHandle, VT_I4, VTS_I4)
	DISP_FUNCTION(AxImage, "CreateFromBitmap", CreateFromBitmap, VT_I4, VTS_I4)
	DISP_FUNCTION(AxImage, "MakeBitmap", MakeBitmap, VT_HANDLE, VTS_I4)
	DISP_FUNCTION(AxImage, "Create", Create, VT_I4, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "Clear", Clear, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "SetStdPalette", SetStdPalette, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(AxImage, "IsValid", IsValid, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Flip", Flip, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Mirror", Mirror, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Rotate", Rotate, VT_BOOL, VTS_R4)
	DISP_FUNCTION(AxImage, "RotateLeft", RotateLeft, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "RotateRight", RotateRight, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Rotate180", Rotate180, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Negative", Negative, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "GrayScale", GrayScale, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "Resample", Resample, VT_BOOL, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "DecreaseBpp", DecreaseBpp, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "IncreaseBpp", IncreaseBpp, VT_BOOL, VTS_I4)
	DISP_FUNCTION(AxImage, "Dither", Dither, VT_BOOL, VTS_I4)
	DISP_FUNCTION(AxImage, "Crop", Crop, VT_BOOL, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "Threshold", Threshold, VT_BOOL, VTS_I4)
	DISP_FUNCTION(AxImage, "Colorize", Colorize, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "Light", Light, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "Mean", Mean, VT_R4, VTS_NONE)
	DISP_FUNCTION(AxImage, "Dilate", Dilate, VT_BOOL, VTS_I4)
	DISP_FUNCTION(AxImage, "Erode", Erode, VT_BOOL, VTS_I4)
	DISP_FUNCTION(AxImage, "Filter", Filter, VT_BOOL, VTS_PI4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "GetNumFrames", GetNumFrames, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetFrame", GetFrame, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "SetFrame", SetFrame, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "GetTransIndex", GetTransIndex, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetTransColor", GetTransColor, VT_COLOR, VTS_NONE)
	DISP_FUNCTION(AxImage, "SetTransIndex", SetTransIndex, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "SetTransColor", SetTransColor, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "GetPaletteSize", GetPaletteSize, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetPalette", GetPalette, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetPaletteColor", GetPaletteColor, VT_COLOR, VTS_I4)
	DISP_FUNCTION(AxImage, "SetPalette", SetPalette, VT_EMPTY, VTS_PI4 VTS_I4)
	DISP_FUNCTION(AxImage, "SetPaletteIndex", SetPaletteIndex, VT_EMPTY, VTS_I4 VTS_COLOR)
	DISP_FUNCTION(AxImage, "SetGrayPalette", SetGrayPalette, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(AxImage, "IsGrayScale", IsGrayScale, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "IsIndexed", IsIndexed, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(AxImage, "SwapIndex", SwapIndex, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "HuePalette", HuePalette, VT_EMPTY, VTS_R4)
	DISP_FUNCTION(AxImage, "IsInside", IsInside, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "GetPixelIndex", GetPixelIndex, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "GetPixelColor", GetPixelColor, VT_COLOR, VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "SetPixelIndex", SetPixelIndex, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(AxImage, "SetPixelColor", SetPixelColor, VT_EMPTY, VTS_I4 VTS_I4 VTS_COLOR)
	DISP_FUNCTION(AxImage, "GetWidth", GetWidth, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetHeight", GetHeight, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetNumColors", GetNumColors, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetColorType", GetColorType, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetBpp", GetBpp, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetType", GetType, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetLastError", GetLastError, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetJpegQuality", GetJpegQuality, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "SetJpegQuality", SetJpegQuality, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "GetXDPI", GetXDPI, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "GetYDPI", GetYDPI, VT_I4, VTS_NONE)
	DISP_FUNCTION(AxImage, "SetXDPI", SetXDPI, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "SetYDPI", SetYDPI, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(AxImage, "CopyToHandle", CopyToHandle, VT_HANDLE, VTS_NONE)
	DISP_FUNCTION(AxImage, "StringToFormat", StringToFormat, VT_I4, VTS_BSTR)
	DISP_FUNCTION(AxImage, "FloodFill", FloodFill, VT_BOOL, VTS_I4 VTS_I4 VTS_COLOR VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_STOCKPROP_BACKCOLOR()
	DISP_FUNCTION_ID(AxImage, "Refresh", DISPID_REFRESH, Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(AxImage, COleControl)
	//{{AFX_EVENT_MAP(AxImage)
	EVENT_STOCK_CLICK()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(AxImage, 1)
	PROPPAGEID(AxImagePropPage::guid)
END_PROPPAGEIDS(AxImage)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(AxImage, "AXIMAGE.AxImageCtrl.1",
	0x4fed1ac6, 0x6cf3, 0x11d6, 0xbb, 0x83, 0x9f, 0xdd, 0x92, 0x23, 0x64, 0x7d)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(AxImage, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DAxImage =
		{ 0x4fed1ac4, 0x6cf3, 0x11d6, { 0xbb, 0x83, 0x9f, 0xdd, 0x92, 0x23, 0x64, 0x7d } };
const IID BASED_CODE IID_DAxImageEvents =
		{ 0x4fed1ac5, 0x6cf3, 0x11d6, { 0xbb, 0x83, 0x9f, 0xdd, 0x92, 0x23, 0x64, 0x7d } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwAxImageOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(AxImage, IDS_AXIMAGE, _dwAxImageOleMisc)


/////////////////////////////////////////////////////////////////////////////
// AxImage::AxImageFactory::UpdateRegistry -
// Adds or removes system registry entries for AxImage

BOOL AxImage::AxImageFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_AXIMAGE,
			IDB_AXIMAGE,
			afxRegApartmentThreading,
			_dwAxImageOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::AxImage - Constructor

AxImage::AxImage()
{
	InitializeIIDs(&IID_DAxImage, &IID_DAxImageEvents);
	image = new CxImage();
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::~AxImage - Destructor

AxImage::~AxImage()
{
	delete image;
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::OnDraw - Drawing function

void AxImage::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
   // Paint the background using the BackColor property
	CBrush bkBrush(TranslateColor(GetBackColor()));
	pdc->FillRect(rcBounds, &bkBrush);

	if (image && image->IsValid()){
		if (m_stretch){
			image->Draw(pdc->GetSafeHdc(),0,0,rcBounds.Width(),rcBounds.Height());
		} else {
			image->Draw(pdc->GetSafeHdc());
		}
	}

	if (m_border){
		RECT r={rcBounds.left,rcBounds.top,rcBounds.right,rcBounds.bottom};
		pdc->DrawEdge(&r,EDGE_SUNKEN,BF_RECT);
	}

	//	DoSuperclassPaint(pdc, rcBounds);
}
/////////////////////////////////////////////////////////////////////////////
// AxImage::DoPropExchange - Persistence support

void AxImage::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::OnResetState - Reset control to default state

void AxImage::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::PreCreateWindow - Modify parameters for CreateWindowEx

BOOL AxImage::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::IsSubclassedControl - This is a subclassed control

BOOL AxImage::IsSubclassedControl()
{
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// AxImage::OnOcmCommand - Handle command messages

LRESULT AxImage::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN32
	WORD wNotifyCode = HIWORD(wParam);
#else
	WORD wNotifyCode = HIWORD(lParam);
#endif

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// AxImage message handlers


void AxImage::OnBorderChanged() 
{
	Invalidate();
	SetModifiedFlag();
}

void AxImage::Refresh() 
{
	Invalidate();
}

long AxImage::Load(LPCTSTR FileName, long ImageType) 
{
	if (image) return image->Load(FileName,ImageType);
	return 0;
}

long AxImage::Save(LPCTSTR FileName, long ImageType) 
{
	if (image) return image->Save(FileName,ImageType);
	return 0;
}

void AxImage::OnStretchChanged() 
{
	Invalidate();
	SetModifiedFlag();
}

long AxImage::CreateFromHandle(long handle) 
{
	if (image){
		image->CreateFromHANDLE((HANDLE)handle);
		return image->IsValid();
	}
	return 0;
}

long AxImage::CreateFromBitmap(long bitmap) 
{
	if (image) {
		image->CreateFromHBITMAP((HBITMAP)bitmap);
		return image->IsValid();
	}
	return 0;
}

OLE_HANDLE AxImage::MakeBitmap(long hdc) 
{
	if (image) return (OLE_HANDLE)image->MakeBitmap((HDC)hdc);
	return 0;
}

long AxImage::Create(long width, long height, long bpp) 
{
	if (image) return (long)image->Create(width,height,bpp);
	return 0;
}

void AxImage::Clear(long val) 
{
	if (image) image->Clear((BYTE)val);
}

void AxImage::SetStdPalette() 
{
	if (image) image->SetStdPalette();
}


BOOL AxImage::IsValid() 
{
	if (image) return image->IsValid();
	return FALSE;
}

BOOL AxImage::Flip() 
{
	if (image) return image->Flip();
	return FALSE;
}

BOOL AxImage::Mirror() 
{
	if (image) return image->Mirror();
	return FALSE;
}

BOOL AxImage::Rotate(float angle) 
{
	if (image) return image->Rotate(angle);
	return FALSE;
}

BOOL AxImage::RotateLeft() 
{
	if (image) return image->RotateLeft();
	return FALSE;
}

BOOL AxImage::RotateRight() 
{
	if (image) return image->RotateRight();
	return FALSE;
}

BOOL AxImage::Rotate180() 
{
	if (image) return image->Rotate180();
	return FALSE;
}

BOOL AxImage::Negative() 
{
	if (image) return image->Negative();
	return FALSE;
}

BOOL AxImage::GrayScale() 
{
	if (image) return image->GrayScale();
	return FALSE;
}

BOOL AxImage::Resample(long newx, long newy, long fast) 
{
	if (image) return image->Resample(newx,newy,fast);
	return FALSE;
}

BOOL AxImage::DecreaseBpp(long bpp, long errordiffusion) 
{
	if (image) return image->DecreaseBpp(bpp,errordiffusion!=0);
	return FALSE;
}

BOOL AxImage::IncreaseBpp(long bpp) 
{
	if (image) return image->IncreaseBpp(bpp);
	return FALSE;
}

BOOL AxImage::Dither(long method) 
{
	if (image) return image->Dither(method);
	return FALSE;
}

BOOL AxImage::Crop(long left, long top, long right, long bottom) 
{
	if (image) return image->Crop(left,top,right,bottom);
	return FALSE;
}

BOOL AxImage::Threshold(long level) 
{
	if (image) return image->Threshold((BYTE)level);
	return FALSE;
}

BOOL AxImage::Colorize(long hue, long sat) 
{
	if (image) return image->Colorize((BYTE)hue,(BYTE)sat);
	return FALSE;
}

BOOL AxImage::Light(long light, long contrast) 
{
	if (image) return image->Light(light,contrast);
	return FALSE;
}

float AxImage::Mean() 
{
	if (image) return image->Mean();
	return 0.0f;
}

BOOL AxImage::Dilate(long Ksize) 
{
	if (image) return image->Dilate(Ksize);
	return FALSE;
}

BOOL AxImage::Erode(long Ksize) 
{
	if (image) return image->Erode(Ksize);
	return FALSE;
}

BOOL AxImage::Filter(int FAR* kernel, long Ksize, long Kfactor, long Koffset) 
{
	if (image) return image->Filter(kernel,Ksize,Kfactor,Koffset);
	return FALSE;
}

long AxImage::GetNumFrames() 
{
	if (image) return image->GetNumFrames();
	return 0;
}

long AxImage::GetFrame() 
{
	if (image) return image->GetFrame();
	return 0;
}

void AxImage::SetFrame(long nFrame) 
{
	if (image) image->SetFrame(nFrame);
}

long AxImage::GetTransIndex() 
{
	if (image) return image->GetTransIndex();
	return 0;
}

OLE_COLOR AxImage::GetTransColor() 
{
	if (image){
		RGBQUAD rgb = image->GetTransColor();
		return RGB(rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	}
	return RGB(0,0,0);
}

void AxImage::SetTransIndex(long index) 
{
	if (image) image->SetTransIndex(index);
}

void AxImage::SetTransColor(long color) 
{
	if (image){
		RGBQUAD rgb = {GetRValue(color),GetGValue(color),GetBValue(color)};
		image->SetTransColor(rgb);
	}
}

long AxImage::GetPaletteSize() 
{
	if (image) return image->GetPaletteSize();
	return 0;
}

OLE_COLOR* AxImage::GetPalette() 
{
	if (image) return (OLE_COLOR*)image->GetPalette();
	return 0;
}

OLE_COLOR AxImage::GetPaletteColor(long index) 
{
	if (image){
		RGBQUAD rgb = image->GetPaletteColor((BYTE)index);
		return RGB(rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	}
	return RGB(0,0,0);
}

void AxImage::SetPalette(long FAR* pal, long ncolors) 
{
	if (image) image->SetPalette((RGBQUAD*)pal,ncolors);
}

void AxImage::SetPaletteIndex(long index, OLE_COLOR color) 
{
	if (image){
		RGBQUAD rgb = {GetRValue(color),GetGValue(color),GetBValue(color)};
		image->SetPaletteColor((BYTE)index,rgb);
	}
}

void AxImage::SetGrayPalette() 
{
	if (image) image->SetGrayPalette();
}

BOOL AxImage::IsGrayScale() 
{
	if (image) return image->IsGrayScale();
	return FALSE;
}

BOOL AxImage::IsIndexed() 
{
	if (image) return image->IsIndexed();
	return FALSE;
}

void AxImage::SwapIndex(long index1, long index2) 
{
	if (image) image->SwapIndex((BYTE)index1,(BYTE)index2);
}

void AxImage::HuePalette(float correction) 
{
	if (image) image->HuePalette(correction);
}

BOOL AxImage::IsInside(long x, long y) 
{
	if (image) return image->IsInside(x,y);
	return FALSE;
}

long AxImage::GetPixelIndex(long x, long y) 
{
	if (image) return image->GetPixelIndex(x,y);
	return 0;
}

OLE_COLOR AxImage::GetPixelColor(long x, long y) 
{
	if (image){
		RGBQUAD rgb = image->GetPixelColor(x,y);
		return RGB(rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	}
	return RGB(0,0,0);
}

void AxImage::SetPixelIndex(long x, long y, long index) 
{
	if (image) image->SetPixelIndex(x,y,(BYTE)index);
}

void AxImage::SetPixelColor(long x, long y, OLE_COLOR color) 
{
	if (image){
		RGBQUAD rgb = {GetRValue(color),GetGValue(color),GetBValue(color)};
		image->SetPixelColor(x,y,rgb);
	}
}

long AxImage::GetWidth() 
{
	if (image) return image->GetWidth();
	return 0;
}


long AxImage::GetHeight() 
{
	if (image) return image->GetHeight();
	return 0;
}

long AxImage::GetNumColors() 
{
	if (image) return image->GetNumColors();
	return 0;
}

long AxImage::GetColorType() 
{
	if (image) return image->GetColorType();
	return 0;
}

long AxImage::GetBpp() 
{
	if (image) return image->GetBpp();
	return 0;
}

long AxImage::GetType() 
{
	if (image) return image->GetType();
	return 0;
}

BSTR AxImage::GetLastError() 
{
	CString strResult;
	if (image)  strResult=image->GetLastError();
	return strResult.AllocSysString();
}

long AxImage::GetJpegQuality() 
{
	if (image) return image->GetJpegQuality();
	return 0;
}

void AxImage::SetJpegQuality(long q) 
{
	if (image) image->SetJpegQuality((BYTE)q);
}

long AxImage::GetXDPI() 
{
	if (image) return image->GetXDPI();
	return 0;
}

long AxImage::GetYDPI() 
{
	if (image) return image->GetYDPI();
	return 0;
}

void AxImage::SetXDPI(long dpi) 
{
	if (image) image->SetXDPI(dpi);
}

void AxImage::SetYDPI(long dpi) 
{
	if (image) image->SetYDPI(dpi);
}

OLE_HANDLE AxImage::CopyToHandle() 
{
	if (image) return (OLE_HANDLE)image->CopyToHandle();
	return NULL;
}

long AxImage::StringToFormat(LPCTSTR extension) 
{
	return CxImage::GetTypeIdFromName(extension);
}

BOOL AxImage::FloodFill(long xStart, long yStart, OLE_COLOR cFillColor, short nTolerance, short nOpacity, short bSelectFillArea, short nSelectionLevel) 
{
	if (image){
		RGBQUAD rgb = {GetRValue(cFillColor),GetGValue(cFillColor),GetBValue(cFillColor)};
		return image->FloodFill(xStart,yStart,rgb,(BYTE)nTolerance,(BYTE)nOpacity,bSelectFillArea!=0,(BYTE)nSelectionLevel);
	}
	return FALSE;
}
