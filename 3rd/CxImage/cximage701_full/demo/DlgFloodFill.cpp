// DlgFloodFill.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
//#include "MainFrm.h"
//#include "demodoc.h"
#include "DlgFloodFill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgFloodFill dialog


DlgFloodFill::DlgFloodFill(CWnd* pParent /*=NULL*/)
	: CDialog(DlgFloodFill::IDD, pParent)
{
	m_pParent=NULL;
	//{{AFX_DATA_INIT(DlgFloodFill)
	m_tol = 0;
	m_select = FALSE;
	m_opacity = 0;
	//}}AFX_DATA_INIT
}

DlgFloodFill::DlgFloodFill(CMainFrame * pParent)
{
	m_pParent=pParent;
	m_tol = 0;
	m_select = 0;
}
BOOL DlgFloodFill::Create()
{
	return CDialog::Create(DlgFloodFill::IDD);
}

void DlgFloodFill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgFloodFill)
	DDX_Control(pDX, IDC_BUTTON1, m_btnSelColor);
	DDX_Text(pDX, IDC_EDIT1, m_tol);
	DDV_MinMaxByte(pDX, m_tol, 0, 255);
	DDX_Check(pDX, IDC_CHECK1, m_select);
	DDX_Text(pDX, IDC_EDIT2, m_opacity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgFloodFill, CDialog)
	//{{AFX_MSG_MAP(DlgFloodFill)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgFloodFill message handlers

void DlgFloodFill::OnOK() 
{
	if (m_pParent != 0){ //modeless
		UpdateData(1);
		//((CWnd*)m_pDoc)->PostMessage(...);
		DestroyWindow();
	} else
		CDialog::OnOK();
}

void DlgFloodFill::OnCancel() 
{
	if (m_pParent != 0)	//modeless
		//((CWnd*)m_pDoc)->PostMessage(...);
		DestroyWindow();
	else
		CDialog::OnCancel();
}

void DlgFloodFill::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL DlgFloodFill::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_btnSelColor.SetBkgColor(m_color);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgFloodFill::OnButton1() 
{
	CColorDialog dlg(m_color, CC_FULLOPEN | CC_ANYCOLOR, this);

	if (dlg.DoModal() == IDOK){
		m_color = dlg.GetColor();
		m_btnSelColor.SetBkgColor(m_color);
		m_btnSelColor.Invalidate();
	}	
}
