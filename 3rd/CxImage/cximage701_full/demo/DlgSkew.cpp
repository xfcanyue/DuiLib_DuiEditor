// DlgSkew.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgSkew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgSkew dialog


DlgSkew::DlgSkew(CWnd* pParent /*=NULL*/)
	: CDialog(DlgSkew::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgSkew)
	m_bEnableInterpolation = FALSE;
	m_pivotx = 0;
	m_pivoty = 0;
	m_skewx = 5.5f;
	m_skewy = -5.0f;
	m_anglex = 0.4f;
	m_angley = 0.5f;
	m_slopex = 0.6f;
	m_slopey = 0.7f;
	//}}AFX_DATA_INIT
}


void DlgSkew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgSkew)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Check(pDX, IDC_CHECK1, m_bEnableInterpolation);
	DDX_Text(pDX, IDC_EDIT1, m_pivotx);
	DDX_Text(pDX, IDC_EDIT2, m_pivoty);
	DDX_Text(pDX, IDC_EDIT4, m_skewx);
	DDX_Text(pDX, IDC_EDIT5, m_skewy);
	DDX_Text(pDX, IDC_EDIT6, m_anglex);
	DDX_Text(pDX, IDC_EDIT7, m_angley);
	DDX_Text(pDX, IDC_EDIT8, m_slopex);
	DDX_Text(pDX, IDC_EDIT9, m_slopey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgSkew, CDialog)
	//{{AFX_MSG_MAP(DlgSkew)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgSkew message handlers

BOOL DlgSkew::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	UpdateMyData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgSkew::UpdateMyData()
{
	UpdateData(1);
	
	m_slopex = m_skewx/m_w;
	m_slopey = m_skewy/m_h;

	m_anglex = (float)(180*atan(m_slopex)/3.141592654);
	m_angley = (float)(180*atan(m_slopey)/3.141592654);

	UpdateData(0);
}

void DlgSkew::OnChangeEdit4() 
{
	UpdateMyData();
}

void DlgSkew::OnChangeEdit5() 
{
	UpdateMyData();
}
