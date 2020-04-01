// DlgDataExt.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgDataExt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDataExt dialog

DlgDataExt::DlgDataExt(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDataExt::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDataExt)
	m_Ymax = 100.0f;
	m_Ymin = 0.0f;
	m_Xmin = 0.0f;
	m_Xmax = 100.0f;
	m_thresh = 200;
	//}}AFX_DATA_INIT

	m_bitmap = 0;
	m_image = 0;
	m_color = 0;
	m_bDetect = true;
	m_bMinmax = true;
	m_bAvg = true;
	m_bLogXaxis = false;
	m_bLogYaxis = false;
}


void DlgDataExt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDataExt)
	DDX_Control(pDX, IDC_PREVIEW, m_chk_preview);
	DDX_Control(pDX, IDC_CHECK6, m_chk_logyaxis);
	DDX_Control(pDX, IDC_CHECK2, m_chk_logxaxis);
	DDX_Control(pDX, IDC_RADIO3, m_btn_matchcolor);
	DDX_Control(pDX, IDC_GREYLEVEL, m_btn_graylevel);
	DDX_Control(pDX, IDC_COLORSEL, m_btn_colorsel);
	DDX_Control(pDX, IDC_MINMAX, m_chk_minmax);
	DDX_Control(pDX, IDC_AVG, m_chk_avg);
	DDX_Control(pDX, IDCANCEL2, m_btn_process);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_EDIT4, m_Ymax);
	DDX_Text(pDX, IDC_EDIT3, m_Ymin);
	DDX_Text(pDX, IDC_EDIT2, m_Xmin);
	DDX_Text(pDX, IDC_EDIT1, m_Xmax);
	DDX_Text(pDX, IDC_THRESH, m_thresh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgDataExt, CDialog)
	//{{AFX_MSG_MAP(DlgDataExt)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDCANCEL2, OnProcess)
	ON_BN_CLICKED(IDC_COLORSEL, OnColorsel)
	ON_EN_CHANGE(IDC_THRESH, OnChangeThresh)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_BN_CLICKED(IDC_GREYLEVEL, OnGreylevel)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDataExt message handlers

BOOL DlgDataExt::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_btn_process.SetIcon(IDI_B,BS_LEFT);

	if (m_bAvg) m_chk_avg.SetCheck(1);
	if (m_bMinmax) m_chk_minmax.SetCheck(1);

	if(m_bDetect)
		m_btn_graylevel.SetCheck(1);
	else
		m_btn_matchcolor.SetCheck(1);

	if (m_bLogXaxis) m_chk_logxaxis.SetCheck(1);
	if (m_bLogYaxis) m_chk_logyaxis.SetCheck(1);

	m_chk_preview.SetCheck(1);

	m_btn_colorsel.SetBkgColor(m_color);
	m_btn_colorsel.SetToolTipText("click to select a new color");

	UpdatePreview();

	UpdateData(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_bitmap) DeleteObject(m_bitmap);	
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnProcess() 
{
	if (!m_image)
	{
		AfxMessageBox(_T("null image pointer!!!"));
		return;
	}

	UpdateData(1);

	long xlen = m_image->GetWidth();
	long ylen = m_image->GetHeight();

	if (0 == xlen || 0 == ylen)
	{
		AfxMessageBox(_T("empty image!!!"));
		return;
	}

	if (m_chk_logxaxis.GetCheck() && (m_Xmin<=0 || m_Xmax<=0)){
		AfxMessageBox(_T("Logarithmic X axis must have both Xmin and Xmax greater than zero\n(values are real, not logarithimic. Example: use '0.01', not '-2')"));
		return;
	}

	if (m_chk_logyaxis.GetCheck() && (m_Ymin<=0 || m_Ymax<=0)){
		AfxMessageBox(_T("Logarithmic Y axis must have both Ymin and Ymax greater than zero\n(values are real, not logarithimic. Example: use '0.01', not '-2')"));
		return;
	}
	
	long x,y,ymin,ymax,npoints;

	CString s,s2;

	s = _T("x");
	if (m_chk_minmax.GetCheck())	s += _T("\tymin\tymax");
	if (m_chk_avg.GetCheck())		s += _T("\tyavg");
	s += _T("\r\n");

	npoints = 0;

	for (x = 0; x<xlen; x++){

		if (m_btn_graylevel.GetCheck()){
			for(y = 0; (y<ylen) && (m_image->GetPixelGray(x,y)>=m_thresh); y++);
			ymin = y;
			for (y = ylen-1; (y>ymin) && (m_image->GetPixelGray(x,y)>=m_thresh); y--);
			ymax = y;
		} else {
			for(y = 0; (y<ylen)&&(m_image->RGBQUADtoRGB(m_image->GetPixelColor(x,y))!=m_color); y++);
			ymin = y;
			for (y = ylen-1; (y>ymin)&&(m_image->RGBQUADtoRGB(m_image->GetPixelColor(x,y))!=m_color); y--);
			ymax = y;
		}

		if (ymin<ylen){
			npoints++;

			float ftmp;
			
			if (m_chk_logxaxis.GetCheck()){
				ftmp = (float)pow(10.0f,x*(log10(m_Xmax)-log10(m_Xmin))/xlen + log10(m_Xmin));
			} else {
				ftmp = x*(m_Xmax-m_Xmin)/xlen + m_Xmin;
			}
			s2.Format(_T("%f"),ftmp);
			s += s2;
			
			if (m_chk_minmax.GetCheck()){
				if (m_chk_logyaxis.GetCheck()){
					ftmp = (float)pow(10.0f,ymin*(log10(m_Ymax)-log10(m_Ymin))/ylen + log10(m_Ymin));
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
					ftmp = (float)pow(10.0f,ymax*(log10(m_Ymax)-log10(m_Ymin))/ylen + log10(m_Ymin));
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
				} else {
					ftmp = ymin*(m_Ymax-m_Ymin)/ylen + m_Ymin;
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
					ftmp = ymax*(m_Ymax-m_Ymin)/ylen + m_Ymin;
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
				}
			}
			
			if (m_chk_avg.GetCheck()){
				if (m_chk_logyaxis.GetCheck()){
					ftmp = (float)pow(10.0f,0.5f*(ymin+ymax)*(log10(m_Ymax)-log10(m_Ymin))/ylen + log10(m_Ymin));
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
				} else {
					ftmp = 0.5f*(ymin+ymax)*(m_Ymax-m_Ymin)/ylen + m_Ymin;
					s2.Format(_T("\t%f"),ftmp);
					s += s2;
				}
			}
			
			s += "\r\n";
		}
	}

	LPTSTR lpszBuffer;
	HANDLE hClipboard;
	hClipboard = GlobalAlloc (GPTR, (s.GetLength () + 1)*sizeof(TCHAR));
	lpszBuffer = (LPTSTR) GlobalLock (hClipboard);
	_tcscpy (lpszBuffer, s);
	GlobalUnlock (hClipboard);

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd())) {
		if(::EmptyClipboard()) {
#if defined(UNICODE)
			if (::SetClipboardData(CF_UNICODETEXT, hClipboard) == NULL ) {
				AfxMessageBox( _T("Unable to set Clipboard data") );
#else
			if (::SetClipboardData(CF_TEXT, hClipboard) == NULL ) {
				AfxMessageBox( _T("Unable to set Clipboard data") );
#endif
			}
		}
	}
	CloseClipboard();

	s.Format(_T("%d points saved to clipboard"),npoints);
	AfxMessageBox(s);
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnColorsel() 
{

	CColorDialog dlg(m_color, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_color = dlg.GetColor();
		m_btn_colorsel.SetBkgColor(m_color);
		Invalidate();
	}

	m_btn_matchcolor.SetCheck(1);
	m_btn_graylevel.SetCheck(0);

	if (m_chk_preview.GetCheck()) UpdatePreview();

}
/////////////////////////////////////////////////////////////////////////////
//DEL void DlgDataExt::OnPaint() 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	
//DEL 	CRect rect(120,200,180,220);
//DEL 	CBrush brush;
//DEL 	
//DEL 	//brush.CreateSolidBrush(RGB(m_pal[index].rgbRed , m_pal[index].rgbGreen , m_pal[index].rgbBlue ));
//DEL 	brush.CreateSolidBrush(m_color);
//DEL 
//DEL 	CPen   pen;
//DEL 	pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
//DEL 
//DEL 	CBrush* pOldBrush = (CBrush*) dc.SelectObject(&brush);
//DEL 	CPen*   pOldPen   = (CPen*)   dc.SelectObject(&pen);
//DEL 
//DEL 	// Draw the cell colour
//DEL 	dc.Rectangle(rect);
//DEL 
//DEL 	// restore DC and cleanup
//DEL 	dc.SelectObject(pOldBrush);
//DEL 	dc.SelectObject(pOldPen);
//DEL 	brush.DeleteObject();
//DEL 	pen.DeleteObject();
//DEL }
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnOK() 
{
	m_bAvg = m_chk_avg.GetCheck()!=0;
	m_bMinmax = m_chk_minmax.GetCheck()!=0;
	m_bDetect = m_btn_graylevel.GetCheck()!=0;
	m_bLogXaxis = m_chk_logxaxis.GetCheck()!=0;
	m_bLogYaxis = m_chk_logyaxis.GetCheck()!=0;
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnChangeThresh() 
{
	m_btn_graylevel.SetCheck(1);
	m_btn_matchcolor.SetCheck(0);

	if (m_chk_preview.GetCheck()) UpdatePreview();
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::UpdatePreview()
{

	if (m_image){
		CxImage tmp;
		tmp.Copy(*m_image,true,false,false);

		if (m_chk_preview.GetCheck()){
			UpdateData(1);
			if (m_btn_graylevel.GetCheck()){
				tmp.Threshold(m_thresh);
			} else {
#if CXIMAGE_SUPPORT_SELECTION
				tmp.SelectionAddColor(tmp.RGBtoRGBQUAD(m_color));
				tmp.SelectionSplit(&tmp);
				tmp.Negative();
#endif //CXIMAGE_SUPPORT_SELECTION
			}
		}

		tmp.IncreaseBpp(24);
		tmp.Resample(150,90,0);

		if (m_bitmap) DeleteObject(m_bitmap);
		m_bitmap = tmp.MakeBitmap(m_picture.GetDC()->m_hDC);
		m_picture.SetBitmap(m_bitmap);
	}

}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnPreview() 
{
	UpdatePreview();	
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnGreylevel() 
{
	if (m_chk_preview.GetCheck()) UpdatePreview();
}
/////////////////////////////////////////////////////////////////////////////
void DlgDataExt::OnRadio3() 
{
	if (m_chk_preview.GetCheck()) UpdatePreview();
}
/////////////////////////////////////////////////////////////////////////////
