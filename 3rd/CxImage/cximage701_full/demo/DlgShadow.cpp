// DlgShadow.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgShadow dialog


DlgShadow::DlgShadow(CWnd* pParent /*=NULL*/)
	: CDialog(DlgShadow::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgShadow)
	m_x = 0;
	m_y = 0;
	m_intensity = 0;
	m_relative = FALSE;
	m_radius = 0;
	//}}AFX_DATA_INIT
}


void DlgShadow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgShadow)
	DDX_Control(pDX, IDC_BUTTON2, m_btnBkg);
	DDX_Control(pDX, IDC_BUTTON1, m_btnShadow);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT4, m_intensity);
	DDV_MinMaxByte(pDX, m_intensity, 0, 255);
	DDX_Check(pDX, IDC_CHECK1, m_relative);
	DDX_Text(pDX, IDC_EDIT5, m_radius);
	DDV_MinMaxByte(pDX, m_radius, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgShadow, CDialog)
	//{{AFX_MSG_MAP(DlgShadow)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgShadow message handlers

BOOL DlgShadow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	m_btnShadow.SetBkgColor(m_shadow);
	m_btnBkg.SetBkgColor(m_bkg);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgShadow::OnButton1() 
{
	CColorDialog dlg(m_shadow, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_shadow = dlg.GetColor();
		m_btnShadow.SetBkgColor(m_shadow);
		m_btnShadow.Invalidate();
	}	
}

void DlgShadow::OnButton2() 
{
	CColorDialog dlg(m_bkg, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_bkg = dlg.GetColor();
		m_btnBkg.SetBkgColor(m_bkg);
		m_btnBkg.Invalidate();
	}	
}

