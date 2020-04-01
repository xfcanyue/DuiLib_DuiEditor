// DlgIncBpp.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgIncBpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgIncBpp dialog


DlgIncBpp::DlgIncBpp(CWnd* pParent /*=NULL*/)
	: CDialog(DlgIncBpp::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgIncBpp)
	//}}AFX_DATA_INIT
}


void DlgIncBpp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgIncBpp)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_RADIO1, m_r4);
	DDX_Control(pDX, IDC_RADIO3, m_r24);
	DDX_Control(pDX, IDC_RADIO2, m_r8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgIncBpp, CDialog)
	//{{AFX_MSG_MAP(DlgIncBpp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgIncBpp message handlers

BOOL DlgIncBpp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	if (m_bit == 4) m_r4.SetCheck(1);
	else if (m_bit == 8) m_r8.SetCheck(1);
	else m_r24.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgIncBpp::OnOK() 
{
	if (m_r4.GetCheck()) m_bit=4;
	else if (m_r8.GetCheck()) m_bit=8;
	else m_bit=24;
	
	CDialog::OnOK();
}
