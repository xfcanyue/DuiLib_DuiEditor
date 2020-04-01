// logomdi.cpp : implementation file
//

#include "stdafx.h"
#include "logomdi.h"
#include "demo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// CLogoMdi
CLogoMdi::CLogoMdi()  : m_sizeClient(0,0)
{
  	m_LogoFont.CreateFont(36, 0, 0, 0, FW_BOLD, 1, FALSE, FALSE,
	  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	  FIXED_PITCH | FF_ROMAN, _T("Times New Roman"));

#ifdef _DEBUG
	sLogoString = _T("CxImage Debug ");
#else
	sLogoString = _T("CxImage ");
#endif

/*	//BITMAP background
	HBITMAP bitmap;
	if (bitmap = ::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1))){
		bgImage = new CxImage((HBITMAP)bitmap);
	} else {
		bgImage = NULL;
	}*/

	//JPEG background
#if CXIMAGE_SUPPORT_DECODE && CXIMAGE_SUPPORT_JPG
	bgImage = new CxImage();
	if (!bgImage->LoadResource(FindResource(NULL,_T("IDR_JPG1"),_T("JPG")),CXIMAGE_FORMAT_JPG)){
		delete bgImage;
		bgImage =NULL;
	}
#else
	bgImage = 0;
#endif
	if (bgImage) {
		COLORREF rgb=GetSysColor(COLOR_APPWORKSPACE);
		RGBQUAD hsl=bgImage->RGBtoHSL(bgImage->RGBtoRGBQUAD(rgb));
		bgImage->Colorize(hsl.rgbRed,min(hsl.rgbGreen,100));
	}
}
//////////////////////////////////////////////////////////////////////////////
CLogoMdi::~CLogoMdi()
{
	m_LogoFont.DeleteObject();
	delete bgImage;
}
//////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLogoMdi, CWnd)
    //{{AFX_MSG_MAP(CLogoMdi)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// CLogoMdi message handlers
BOOL CLogoMdi::OnEraseBkgnd(CDC* pDC)
{
	// This is where the real work is done printing the logo in the corner..

	RECT rect,m_rDataBox;
	GetClientRect(&rect);
	CopyRect(&m_rDataBox,&rect);

	if (bgImage==NULL){
		CBrush NewBrush(GetSysColor(COLOR_DESKTOP)) ; 
		pDC->SetBrushOrg(0,0) ;
		CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
		pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top ,PATCOPY);
		pDC->SelectObject(pOldBrush);
	} else {
		bgImage->Tile(pDC->GetSafeHdc(),&rect);
	}

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	CFont* oldFont = pDC->SelectObject(&m_LogoFont);
	CSize sz = pDC->GetTextExtent(sLogoString, sLogoString.GetLength());
	// GetTextExtent calculates the size of the displayed logo
	// which depends on the device context....
	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...
	m_rDataBox.left = m_rDataBox.right  - sz.cx - tm.tmAveCharWidth/2;
	m_rDataBox.top  = m_rDataBox.bottom - sz.cy - tm.tmHeight/5;
	pDC->SetBkMode(TRANSPARENT);
	// shift logo box right, and print black...
	COLORREF oldColor = pDC->SetTextColor(RGB(0,0,0));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// shift logo box left and print white
	m_rDataBox.left -= 3*tm.tmAveCharWidth/5;
	pDC->SetTextColor(RGB(255,255,255));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// Restore original location and print in the button face color
	m_rDataBox.left += tm.tmAveCharWidth/5;
	pDC->SetTextColor(GetSysColor(COLOR_BTNFACE));
	pDC->DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// restore the original properties and release resources...
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);   

    return TRUE;
}      
//////////////////////////////////////////////////////////////////////////////
void CLogoMdi::OnSize(UINT /*nType*/, int cx, int cy)
{
    Default() ;    // Same as DefWindowProc(), or CWnd::OnSize();
    // if the app is just starting up, save the window
    // dimensions and get out
    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy==0)) {
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
        return ;
    }
    // if the size hasn't changed, break and pass to default
    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy)) 
        return ;
    // window size has changed so save new dimensions and force
    // entire background to redraw, including icon backgrounds
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
    RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);    
    return ;                
}
//////////////////////////////////////////////////////////////////////////////
