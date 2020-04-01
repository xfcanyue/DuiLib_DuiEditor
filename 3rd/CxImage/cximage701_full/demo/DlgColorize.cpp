// DlgColorize.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgColorize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgColorize dialog


DlgColorize::DlgColorize(CWnd* pParent /*=NULL*/)
	: CDialog(DlgColorize::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgColorize)
	m_hue = 0;
	m_sat = 0;
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_blend = 50;
	m_sol = 0;
	//}}AFX_DATA_INIT
}


void DlgColorize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgColorize)
	DDX_Control(pDX, IDC_CHECK5, m_rbLink);
	DDX_Control(pDX, IDC_RADIO3, m_rbHSL);
	DDX_Control(pDX, IDC_RADIO6, m_rbSol);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDCOLORS, m_colors);
	DDX_Text(pDX, IDC_EDIT1, m_hue);
	DDX_Text(pDX, IDC_EDIT2, m_sat);
	DDX_Text(pDX, IDC_EDIT3, m_r);
	DDX_Text(pDX, IDC_EDIT4, m_g);
	DDX_Text(pDX, IDC_EDIT5, m_b);
	DDX_Text(pDX, IDC_EDIT8, m_blend);
	DDX_Text(pDX, IDC_EDIT10, m_sol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgColorize, CDialog)
	//{{AFX_MSG_MAP(DlgColorize)
	ON_BN_CLICKED(IDCOLORS, OnColors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgColorize message handlers

BOOL DlgColorize::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_colors.SetIcon(IDI_B,BS_LEFT);
	
	switch(m_bHSL)
	{
	case 1:
		m_rbHSL.SetCheck(1);
		break;
	case 2:
		m_rbSol.SetCheck(1);
		break;
	default:
		CheckRadioButton(IDC_RADIO3,IDC_RADIO4,IDC_RADIO4);
	}

	m_rbLink.SetCheck(m_bLinked);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgColorize::OnOK() 
{
	m_bHSL = 0;
	if (m_rbHSL.GetCheck()) m_bHSL=1;
	if (m_rbSol.GetCheck()) m_bHSL=2;
	
	m_bLinked = (BYTE)m_rbLink.GetCheck();
	
	CDialog::OnOK();
}

void DlgColorize::OnColors() 
{
	UpdateData(1);

	int bHSL = m_rbHSL.GetCheck();

	COLORREF c;
	RGBQUAD rgb,hsl;

	if (bHSL){
		hsl.rgbRed = m_hue;
		hsl.rgbGreen = m_sat;
		hsl.rgbBlue = 128;
		rgb = CxImage::HSLtoRGB(hsl);
		c = RGB(rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	} else {
		c = RGB(m_r,m_g,m_b);
	}

	CColorDialog dlg(c,	CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		c = dlg.GetColor();
		//if (bHSL){
			rgb.rgbRed = GetRValue(c);
			rgb.rgbGreen = GetGValue(c);
			rgb.rgbBlue = GetBValue(c);
			hsl = CxImage::RGBtoHSL(rgb);
			m_hue = hsl.rgbRed;
			m_sat = hsl.rgbGreen;
		//} else {
			m_r=GetRValue(c);
			m_g=GetGValue(c);
			m_b=GetBValue(c);
		//}
			m_sol = (BYTE)RGB2GRAY(m_r,m_g,m_b);
	}

	UpdateData(0);
}
