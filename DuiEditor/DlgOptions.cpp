// DlgOptions.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgOptions.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "DuiEditorTabView.h"

//////////////////////////////////////////////////////////////////////////
// CDlgOptions 对话框

IMPLEMENT_DYNAMIC(CDlgOptions, CDialogEx)

CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgOptions::IDD, pParent)
{

	m_nTreeDeep = 0;
	m_strAttachTestCommand = _T("");
}

CDlgOptions::~CDlgOptions()
{
}

void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bUpdateSizeWhenModifyPos);
	DDX_Text(pDX, IDC_EDIT1, m_nTreeDeep);
	DDX_Text(pDX, IDC_EDIT2, m_strAttachTestCommand);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ctrlBkColor);
	DDX_Control(pDX, IDC_MFCFONTCOMBO1, m_cbFont);
// 	DDX_Text(pDX, IDC_EDIT4, m_xmlEditorOpt.nEditorFontSize);
// 	DDX_Text(pDX, IDC_EDIT5, m_xmlEditorOpt.nEditorLineSpace);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_ctrlBkColor2);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, m_ctrlBkColor3);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, m_ctrlBkColor4);
}


BEGIN_MESSAGE_MAP(CDlgOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgOptions::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_MFCFONTCOMBO1, &CDlgOptions::OnSelchangeMfcfontcombo1)
	ON_EN_CHANGE(IDC_EDIT4, &CDlgOptions::OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CDlgOptions::OnChangeEdit5)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CDlgOptions::OnClickedMfccolorbutton2)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &CDlgOptions::OnClickedMfccolorbutton3)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON4, &CDlgOptions::OnClickedMfccolorbutton4)
END_MESSAGE_MAP()


// CDlgOptions 消息处理程序
/*
int CALLBACK EnumFontFamProc(const LPENUMLOGFONT lpelf,const LPNEWTEXTMETRIC lpntm, unsigned long nFontType, LPARAM lparam)
{
	CComboBox* pWnd = (CComboBox*) lparam;
	int nItem=0;
	if((nItem = pWnd->FindString(nItem, lpelf -> elfLogFont.lfFaceName)) != CB_ERR)
	{
		return 1; 
	}
	pWnd->AddString(lpelf -> elfLogFont.lfFaceName);
	return 1;
}
*/
BOOL CDlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	pSpin->SetBuddy(GetDlgItem(IDC_EDIT1));
	pSpin->SetRange(9999, 0);

	m_bUpdateSizeWhenModifyPos = TRUE; //AfxGetApp()->GetProfileInt(_T("Options"), _T("UpdateSizeWhenModifyPos"), 1);
	m_nTreeDeep = AfxGetApp()->GetProfileInt(_T("Options"), _T("TreeDeep"), 3);

	m_strAttachTestCommand = g_strAttachTestCommand;

	m_ctrlBkColor.SetColor(g_crBkDesign);
/*
	LOGFONT lf;
	lf.lfCharSet = DEFAULT_CHARSET; // Initialize the LOGFONT structure 
	_tcscpy(lf.lfFaceName, _T("")); 
	HDC hdc;
	hdc=::GetDC(m_hWnd);
	::EnumFontFamiliesEx(hdc,&lf, (FONTENUMPROC)::EnumFontFamProc, reinterpret_cast<LPARAM>(&m_cbFont), 0);
	::ReleaseDC(m_hWnd,hdc);
	*/
	m_xmlEditorOpt = g_xmlEditorOptions;
	m_cbFont.SelectString(-1, m_xmlEditorOpt.strEditorFontName);
	m_ctrlBkColor2.SetColor(m_xmlEditorOpt.crEditorBkColor);
	m_ctrlBkColor3.SetColor(m_xmlEditorOpt.crEditorSelBkColor);
	m_ctrlBkColor4.SetColor(m_xmlEditorOpt.crEditorCaretLineBkColor);

	CString temp;
	temp.Format(_T("%d"), m_xmlEditorOpt.nEditorFontSize);
	SetDlgItemText(IDC_EDIT4, temp);
	temp.Format(_T("%d"), m_xmlEditorOpt.nEditorLineSpace);
	SetDlgItemText(IDC_EDIT5, temp);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgOptions::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	AfxGetApp()->WriteProfileInt(_T("Options"), _T("UpdateSizeWhenModifyPos"), m_bUpdateSizeWhenModifyPos);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("TreeDeep"), m_nTreeDeep);

	if(m_strAttachTestCommand != g_strAttachTestCommand)
	{
		g_strAttachTestCommand = m_strAttachTestCommand;
		AfxGetApp()->WriteProfileString(_T("Options"), _T("TestCommand"), g_strAttachTestCommand);
	}

	g_crBkDesign = m_ctrlBkColor.GetColor();
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("DegsignBackColor"), g_crBkDesign);

	m_cbFont.GetLBText(m_cbFont.GetCurSel(), m_xmlEditorOpt.strEditorFontName);
	m_xmlEditorOpt.crEditorBkColor = m_ctrlBkColor2.GetColor();
	m_xmlEditorOpt.crEditorSelBkColor = m_ctrlBkColor3.GetColor();
	m_xmlEditorOpt.crEditorCaretLineBkColor = m_ctrlBkColor4.GetColor();
	g_xmlEditorOptions = m_xmlEditorOpt;

	AfxGetApp()->WriteProfileString(_T("Options"), _T("EditorFontName"), g_xmlEditorOptions.strEditorFontName);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("EditorFontSize"), g_xmlEditorOptions.nEditorFontSize);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("EditorLineSpace"), g_xmlEditorOptions.nEditorLineSpace);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("EditorBkColor"), g_xmlEditorOptions.crEditorBkColor);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("EditorSelBkColor"), g_xmlEditorOptions.crEditorSelBkColor);
	AfxGetApp()->WriteProfileInt(_T("Options"), _T("EditorCaretLineBkColor"), g_xmlEditorOptions.crEditorCaretLineBkColor);

	CDuiEditorViewDesign *pView = ((CMainFrame *)AfxGetMainWnd())->GetActiveUIView();
	if(pView)
	{
		pView->Invalidate();
	}

	CDialogEx::OnOK();
}


void CDlgOptions::OnBnClickedButton1()
{
	static TCHAR BASED_CODE szFilter[] = _T("exe(*.exe)|*.exe|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(dlg.DoModal()==IDOK)
	{
		GetDlgItem(IDC_EDIT2)->SetWindowText(dlg.GetPathName());
	}
}

void CDlgOptions::SetXmlOptions(const tagXmlEditorOpt &opt)
{
	POSITION pos = ((CDuiEditorApp *)AfxGetApp())->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *pDocTemplate = ((CDuiEditorApp *)AfxGetApp())->GetNextDocTemplate(pos);			
		POSITION pos1 = pDocTemplate->GetFirstDocPosition();
		while (pos1 != NULL)
		{
			CDocument *pDoc = pDocTemplate->GetNextDoc(pos1);
			if(!pDoc->IsKindOf(RUNTIME_CLASS(CDuiEditorDoc)))
				continue;

			for (POSITION pos = pDoc->GetFirstViewPosition(); pos != NULL;)
			{
				CView *pView = pDoc->GetNextView(pos);
				if(pView->IsKindOf(RUNTIME_CLASS(CDuiEditorTabView)))
				{
					CSciWnd *pSciWnd = NULL;
					CDuiEditorTabView *pTabView = (CDuiEditorTabView *)pView;
					SetXmlOptions(&pTabView->m_pFormCode->sci, opt);
					SetXmlOptions(&pTabView->m_pFormDesign->m_Manager.GetXmlPane()->sci, opt);
				}
			}
		}
	}
}

void CDlgOptions::SetXmlOptions(CSciWnd *pSciWnd, const tagXmlEditorOpt &opt)
{
	LSSTRING_CONVERSION;
	pSciWnd->SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)LST2UTF8(opt.strEditorFontName));
	pSciWnd->sci_StyleSetSize(STYLE_DEFAULT, opt.nEditorFontSize);
	pSciWnd->sci_StyleSetBack(STYLE_DEFAULT, opt.crEditorBkColor);
	pSciWnd->sci_StyleSetBack(STYLE_LINENUMBER, opt.crEditorBkColor);
	pSciWnd->sci_StyleSetBack(STYLE_INDENTGUIDE, opt.crEditorBkColor);
	for (int i = SCE_H_DEFAULT; i <= SCE_HPHP_OPERATOR; i++)
	{
		pSciWnd->sci_StyleSetBack(i,	opt.crEditorBkColor);
	}
	pSciWnd->sci_SetSelBack(STYLE_DEFAULT, opt.crEditorSelBkColor);
	pSciWnd->sci_SetExtraDescent(opt.nEditorLineSpace);
	pSciWnd->sci_SetExtraAscent(opt.nEditorLineSpace);
	pSciWnd->sci_SetCaretLineBack(opt.crEditorCaretLineBkColor);
}

void CDlgOptions::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	SetXmlOptions(g_xmlEditorOptions);
}


void CDlgOptions::OnSelchangeMfcfontcombo1()
{
	m_cbFont.GetLBText(m_cbFont.GetCurSel(), m_xmlEditorOpt.strEditorFontName);
	SetXmlOptions(m_xmlEditorOpt);
}


void CDlgOptions::OnChangeEdit4()
{
	CString temp;
	GetDlgItemText(IDC_EDIT4, temp);
	m_xmlEditorOpt.nEditorFontSize = _ttoi(temp);
	SetXmlOptions(m_xmlEditorOpt);
}


void CDlgOptions::OnChangeEdit5()
{
	CString temp;
	GetDlgItemText(IDC_EDIT5, temp);
	m_xmlEditorOpt.nEditorLineSpace = _ttoi(temp);
	SetXmlOptions(m_xmlEditorOpt);
}

void CDlgOptions::OnClickedMfccolorbutton2()
{
	m_xmlEditorOpt.crEditorBkColor = m_ctrlBkColor2.GetColor();
	SetXmlOptions(m_xmlEditorOpt);
}

void CDlgOptions::OnClickedMfccolorbutton3()
{
	m_xmlEditorOpt.crEditorSelBkColor = m_ctrlBkColor3.GetColor();
	SetXmlOptions(m_xmlEditorOpt);
}

void CDlgOptions::OnClickedMfccolorbutton4()
{
	m_xmlEditorOpt.crEditorCaretLineBkColor = m_ctrlBkColor4.GetColor();
	SetXmlOptions(m_xmlEditorOpt);
}
