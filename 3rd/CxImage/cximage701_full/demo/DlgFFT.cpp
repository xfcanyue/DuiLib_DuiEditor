// DlgFFT.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "demoDoc.h"
#include "DlgFFT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgFFT dialog


DlgFFT::DlgFFT(CWnd* pParent /*=NULL*/)
	: CDialog(DlgFFT::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgFFT)
	//}}AFX_DATA_INIT
}


void DlgFFT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgFFT)
	DDX_Control(pDX, IDC_CHECK1, m_cinverse);
	DDX_Control(pDX, IDC_CHECK3, m_cmagnitude);
	DDX_Control(pDX, IDC_CHECK4, m_cforcefft);
	DDX_Control(pDX, IDC_IMAG, m_imag);
	DDX_Control(pDX, IDC_REAL, m_real);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgFFT, CDialog)
	//{{AFX_MSG_MAP(DlgFFT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgFFT message handlers

BOOL DlgFFT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);

	m_cforcefft.SetCheck(1);
	//m_cmagnitude.SetCheck(1);

	m_real.AddString(_T("(None)"));
	m_real.SetItemData(m_real.GetCount()-1, 0);
	m_imag.AddString(_T("(None)"));
	m_imag.SetItemData(m_imag.GetCount()-1, 0);

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL) {
			CDemoDoc* pDoc = (CDemoDoc*) pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
			CString title = pDoc->GetTitle();
			m_real.AddString(title);
			m_real.SetItemData(m_real.GetCount()-1, (DWORD)pDoc);
			m_imag.AddString(title);
			m_imag.SetItemData(m_imag.GetCount()-1, (DWORD)pDoc);
		}
	}
	m_real.SetCurSel(0);
	m_imag.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgFFT::OnOK() 
{
	pDocReal = (CDemoDoc*) m_real.GetItemData(m_real.GetCurSel());
	pDocImag = (CDemoDoc*) m_imag.GetItemData(m_imag.GetCurSel());

	if (pDocReal==0 && pDocImag==0) return;

	bInverse = m_cinverse.GetCheck() ? -1 : 1;
	bForceFFT = m_cforcefft.GetCheck();
	bMagnitude = m_cmagnitude.GetCheck();
	
	CDialog::OnOK();
}
