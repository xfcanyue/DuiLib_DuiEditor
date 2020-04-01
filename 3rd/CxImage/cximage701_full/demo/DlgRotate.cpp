// DlgRotate.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgRotate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// DlgRotate dialog

DlgRotate::DlgRotate(CWnd* pParent /*=NULL*/)
	: CDialog(DlgRotate::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgRotate)
	m_angle = 0.0f;
	//}}AFX_DATA_INIT
}

void DlgRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgRotate)
	DDX_Control(pDX, IDC_CHECK1, m_ch1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_angle);
	DDX_Control(pDX, IDC_COMBO_IM, inMethod);
	DDX_Control(pDX, IDC_COMBO_OM, ofMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgRotate, CDialog)
	//{{AFX_MSG_MAP(DlgRotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// DlgRotate message handlers

BOOL DlgRotate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	inMethod.SetCurSel(m_method);
	ofMethod.SetCurSel(m_overflow);

	m_ch1.SetCheck(m_keepsize);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgRotate::OnOK() 
{
	m_method=inMethod.GetCurSel();
	m_overflow=ofMethod.GetCurSel();

	m_keepsize=m_ch1.GetCheck();

	CDialog::OnOK();
}
