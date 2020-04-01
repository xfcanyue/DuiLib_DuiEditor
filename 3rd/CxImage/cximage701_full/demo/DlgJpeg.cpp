// DlgJpeg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgJpeg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgJpeg dialog


DlgJpeg::DlgJpeg(CWnd* pParent /*=NULL*/)
	: CDialog(DlgJpeg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgJpeg)
	m_quality = 0.0f;
	//}}AFX_DATA_INIT
}


void DlgJpeg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgJpeg)
	DDX_Control(pDX, IDC_RADIO1, m_r1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_quality);
	DDV_MinMaxFloat(pDX, m_quality, 0.0f, 100.0f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgJpeg, CDialog)
	//{{AFX_MSG_MAP(DlgJpeg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgJpeg message handlers

BOOL DlgJpeg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	m_r1.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgJpeg::OnOK() 
{
	if (m_r1.GetCheck()) m_format = 0;
	else m_format = 1;
	
	CDialog::OnOK();
}
