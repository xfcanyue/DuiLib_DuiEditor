// DlgExpand.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgExpand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgExpand dialog


DlgExpand::DlgExpand(CWnd* pParent /*=NULL*/)
	: CDialog(DlgExpand::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgExpand)
	m_newwidth = 0;
	m_newheight = 0;
	m_left = 0;
	m_right = 0;
	m_top = 0;
	m_bottom = 0;
	//}}AFX_DATA_INIT
}


void DlgExpand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgExpand)
	DDX_Control(pDX, IDC_COLORSEL, m_btnSelColor);
	DDX_Control(pDX, IDC_CHECK8, m_ckKeepRatio);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDC_CHECK7, m_ckUseImageBkg);
	DDX_Control(pDX, IDC_CHECK2, m_ckCenterV);
	DDX_Control(pDX, IDC_CHECK1, m_ckCenterH);
	DDX_Text(pDX, IDC_EDIT1, m_newwidth);
	DDX_Text(pDX, IDC_EDIT2, m_newheight);
	DDX_Text(pDX, IDC_EDIT6, m_left);
	DDX_Text(pDX, IDC_EDIT7, m_right);
	DDX_Text(pDX, IDC_EDIT8, m_top);
	DDX_Text(pDX, IDC_EDIT11, m_bottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgExpand, CDialog)
	//{{AFX_MSG_MAP(DlgExpand)
	ON_BN_CLICKED(IDC_COLORSEL, OnColorsel)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEditW)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEditH)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEditL)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT8, OnChangeEditT)
	ON_EN_CHANGE(IDC_EDIT11, OnChangeEditB)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgExpand message handlers

BOOL DlgExpand::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	if (m_Mode==1){
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
	} else {
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
	}

	m_ckCenterH.SetCheck(m_CenterH);
	m_ckCenterV.SetCheck(m_CenterV);
	m_ckKeepRatio.SetCheck(m_KeepRatio);
	m_ckUseImageBkg.SetCheck(m_UseImageBkg);

	m_btnSelColor.SetBkgColor(m_color);
	m_btnSelColor.SetToolTipText("click to select a new color");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnColorsel() 
{
	CColorDialog dlg(m_color, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_color = dlg.GetColor();
		m_btnSelColor.SetBkgColor(m_color);
		m_btnSelColor.Invalidate();
	}	
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnOK() 
{
	if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO2){
		m_Mode = 1;
	} else {
		m_Mode = 0;
	}

	m_CenterH = m_ckCenterH.GetCheck();
	m_CenterV = m_ckCenterV.GetCheck();
	m_KeepRatio = m_ckKeepRatio.GetCheck();
	m_UseImageBkg = m_ckUseImageBkg.GetCheck();
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnChangeEditW() 
{
	if (m_ckKeepRatio.GetCheck()){
		UpdateData(1);
		m_newheight = (DWORD)(m_newwidth / m_ratio + 0.5f);
		UpdateData(0);
	}
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnChangeEditH() 
{
	if (m_ckKeepRatio.GetCheck()){
		UpdateData(1);
		m_newwidth = (DWORD)(m_newheight * m_ratio + 0.5f);
		UpdateData(0);
	}
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnChangeEditL() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
}
void DlgExpand::OnChangeEditR() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
}
void DlgExpand::OnChangeEditT() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
}
void DlgExpand::OnChangeEditB() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnCheck8() 
{
	if (m_ckKeepRatio.GetCheck()){
		UpdateData(1);
		m_newheight = (DWORD)(m_newwidth / m_ratio + 0.5f);
		UpdateData(0);
	}
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
}
/////////////////////////////////////////////////////////////////////////////
void DlgExpand::OnCheck1() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
}
void DlgExpand::OnCheck2() 
{
	CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
}
/////////////////////////////////////////////////////////////////////////////
