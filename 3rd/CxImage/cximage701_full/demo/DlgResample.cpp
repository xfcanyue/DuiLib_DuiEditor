// DlgResample.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgResample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// DlgResample dialog

DlgResample::DlgResample(CWnd* pParent /*=NULL*/)
	: CDialog(DlgResample::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgResample)
	m_factor = 0.0f;
	m_newheight = 0;
	m_newwidth = 0;
	//}}AFX_DATA_INIT
}

void DlgResample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgResample)
	DDX_Control(pDX, IDC_EDIT1, m_ed1);
	DDX_Control(pDX, IDC_CHECK2, m_ch2);
	DDX_Control(pDX, IDC_COMBO_IM, inMethod);
	DDX_Control(pDX, IDC_RADIO2, m_r2);
	DDX_Control(pDX, IDC_RADIO1, m_r1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1, m_factor);
	DDX_Text(pDX, IDC_EDIT3, m_newheight);
	DDX_Text(pDX, IDC_EDIT2, m_newwidth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DlgResample, CDialog)
	//{{AFX_MSG_MAP(DlgResample)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// DlgResample message handlers

BOOL DlgResample::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	m_r1.SetCheck(m_sizemode);
	m_r2.SetCheck(1-m_sizemode);

	inMethod.SetCurSel(m_mode);

	m_ch2.SetCheck(m_bKeepRatio);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgResample::OnOK() 
{
	m_mode=inMethod.GetCurSel();

	if (m_r1.GetCheck()) m_sizemode=1; //factor
	if (m_r2.GetCheck()) m_sizemode=0; //pixel

	m_bKeepRatio = m_ch2.GetCheck();

	CDialog::OnOK();
}

void DlgResample::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	if (m_ch2.GetCheck()){
		UpdateData(1);
		m_newheight = (DWORD)(m_newwidth / m_ratio + 0.5f);
		UpdateData(0);
	}
	m_r1.SetCheck(0);
	m_r2.SetCheck(1);
}

void DlgResample::OnChangeEdit3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	if (m_ch2.GetCheck()){
		UpdateData(1);
		m_newwidth = (DWORD)(m_newheight * m_ratio + 0.5f);
		UpdateData(0);
	}
	m_r1.SetCheck(0);
	m_r2.SetCheck(1);
}

void DlgResample::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	CString s;
	m_ed1.GetWindowText(s);
	bool hasdp = (s.Find('.')>=0);
	if (!(s.GetAt(s.GetLength()-1)=='.' || hasdp && s.GetAt(s.GetLength()-1)=='0')){
		UpdateData(1);
		m_newwidth = (DWORD)(m_w * fabs(m_factor));
		m_newheight = (DWORD)(m_h * fabs(m_factor));
		UpdateData(0);
	}
	m_r2.SetCheck(0);
	m_r1.SetCheck(1);
}

void DlgResample::OnCheck2() 
{
	if (m_ch2.GetCheck()){
		UpdateData(1);
		m_newheight = (DWORD)(m_newwidth / m_ratio + 0.5f);
		UpdateData(0);
	}
	m_r1.SetCheck(0);
	m_r2.SetCheck(1);
}
