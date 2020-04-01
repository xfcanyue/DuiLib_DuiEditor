// DlgCombine.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "demoDoc.h"
#include "DlgCombine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgCombine dialog


DlgCombine::DlgCombine(CWnd* pParent /*=NULL*/)
	: CDialog(DlgCombine::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCombine)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgCombine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCombine)
	DDX_Control(pDX, IDC_COMBO9, m_chs);
	DDX_Control(pDX, IDC_COMBO8, m_cha);
	DDX_Control(pDX, IDC_COMBO7, m_chb);
	DDX_Control(pDX, IDC_COMBO6, m_chg);
	DDX_Control(pDX, IDC_COMBO5, m_chr);
	DDX_Control(pDX, IDC_COMBO4, m_imga);
	DDX_Control(pDX, IDC_COMBO3, m_imgb);
	DDX_Control(pDX, IDC_COMBO2, m_imgg);
	DDX_Control(pDX, IDC_COMBO1, m_imgr);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCombine, CDialog)
	//{{AFX_MSG_MAP(DlgCombine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCombine message handlers

BOOL DlgCombine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	

	m_imga.AddString(_T("(None)"));
	m_imga.SetItemData(m_imga.GetCount()-1, 0);

	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	while (pos != NULL) {
		CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL) {
			CDemoDoc* pDoc = (CDemoDoc*) pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
			CString title = pDoc->GetTitle();
			m_imgr.AddString(title);
			m_imgr.SetItemData(m_imgr.GetCount()-1, (DWORD)pDoc);
			m_imgg.AddString(title);
			m_imgg.SetItemData(m_imgg.GetCount()-1, (DWORD)pDoc);
			m_imgb.AddString(title);
			m_imgb.SetItemData(m_imgb.GetCount()-1, (DWORD)pDoc);
			m_imga.AddString(title);
			m_imga.SetItemData(m_imga.GetCount()-1, (DWORD)pDoc);
		}
	}
	m_imgr.SetCurSel(0);
	m_imgg.SetCurSel(0);
	m_imgb.SetCurSel(0);
	m_imga.SetCurSel(0);
	m_chr.SetCurSel(1);
	m_chg.SetCurSel(2);
	m_chb.SetCurSel(3);
	m_cha.SetCurSel(0);
	m_chs.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCombine::OnOK() 
{
	pDocR = (CDemoDoc*) m_imgr.GetItemData(m_imgr.GetCurSel());
	pDocG = (CDemoDoc*) m_imgg.GetItemData(m_imgg.GetCurSel());
	pDocB = (CDemoDoc*) m_imgb.GetItemData(m_imgb.GetCurSel());
	pDocA = (CDemoDoc*) m_imga.GetItemData(m_imga.GetCurSel());

	pChR = m_chr.GetCurSel();
	pChG = m_chg.GetCurSel();
	pChB = m_chb.GetCurSel();
	pChA = m_cha.GetCurSel();
	pChS = m_chs.GetCurSel();

	CDialog::OnOK();
}
