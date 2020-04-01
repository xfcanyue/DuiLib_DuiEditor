// DlgDecBpp.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgDecBpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgDecBpp dialog


DlgDecBpp::DlgDecBpp(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDecBpp::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgDecBpp)
	m_maxcolors = 0;
	//}}AFX_DATA_INIT
}


void DlgDecBpp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgDecBpp)
	DDX_Control(pDX, IDC_CHECK2, m_limitcolors);
	DDX_Control(pDX, IDC_CHECK1, m_err);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_RADIO5, m_opt);
	DDX_Control(pDX, IDC_RADIO3, m_r8);
	DDX_Control(pDX, IDC_RADIO2, m_r4);
	DDX_Control(pDX, IDC_RADIO1, m_r1);
	DDX_Text(pDX, IDC_EDIT1, m_maxcolors);
	DDV_MinMaxLong(pDX, m_maxcolors, 2, 256);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgDecBpp, CDialog)
	//{{AFX_MSG_MAP(DlgDecBpp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgDecBpp message handlers

BOOL DlgDecBpp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	if (m_bit == 8) m_r8.SetCheck(1);
	else if (m_bit == 4) m_r4.SetCheck(1);
	else m_r1.SetCheck(1);

	if (m_method)
		CheckRadioButton(IDC_RADIO4,IDC_RADIO5,IDC_RADIO5);
	else
		CheckRadioButton(IDC_RADIO4,IDC_RADIO5,IDC_RADIO4);

	m_err.SetCheck(m_errordiffusion);
	m_limitcolors.SetCheck(m_bLimitColors);
	if (m_bLimitColors==0) m_maxcolors = 256;

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgDecBpp::OnOK() 
{
	if (m_r8.GetCheck()) m_bit=8;
	else if (m_r4.GetCheck()) m_bit=4;
	else m_bit=1;

	m_method=m_opt.GetCheck();

	m_errordiffusion = m_err.GetCheck();
	m_bLimitColors = m_limitcolors.GetCheck();

	UpdateData(1);

	if (m_limitcolors.GetCheck()==0){
		switch (m_bit){
		case 1:
			m_maxcolors = 2; break;
		case 4:
			m_maxcolors = 16; break;
		case 8:
			m_maxcolors = 256; break;
		}
	} else {
		switch (m_bit){
		case 1:
			if (m_maxcolors > 2) m_maxcolors = 2; break;
		case 4:
			if (m_maxcolors > 16) m_maxcolors = 16; break;
		case 8:
			if (m_maxcolors > 256) m_maxcolors = 256; break;
		}
	}

	UpdateData(0);

	CDialog::OnOK();
}
