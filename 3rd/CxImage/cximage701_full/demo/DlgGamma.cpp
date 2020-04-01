// DlgGamma.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgGamma.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgGamma dialog


DlgGamma::DlgGamma(CWnd* pParent /*=NULL*/)
	: CDialog(DlgGamma::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgGamma)
	m_gamma = 0.0f;
	m_gammaR = 0.0f;
	m_gammaG = 0.0f;
	m_gammaB = 0.0f;
	//}}AFX_DATA_INIT
}


void DlgGamma::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgGamma)
	DDX_Control(pDX, IDC_RADIO2, m_rbIndep);
	DDX_Control(pDX, IDC_RADIO1, m_rbLink);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_gamma);
	DDX_Text(pDX, IDC_EDIT2, m_gammaR);
	DDX_Text(pDX, IDC_EDIT3, m_gammaG);
	DDX_Text(pDX, IDC_EDIT6, m_gammaB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgGamma, CDialog)
	//{{AFX_MSG_MAP(DlgGamma)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgGamma message handlers

BOOL DlgGamma::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	switch(m_bGammaMode)
	{
	case 1:
		m_rbIndep.SetCheck(1);
		break;
	default:
		m_rbLink.SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgGamma::OnOK() 
{
	m_bGammaMode = m_rbIndep.GetCheck();

	CDialog::OnOK();
}
