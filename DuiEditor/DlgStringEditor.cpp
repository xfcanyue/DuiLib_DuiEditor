// DlgStringEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgStringEditor.h"
#include "afxdialogex.h"


// CDlgStringEditor 对话框

IMPLEMENT_DYNAMIC(CDlgStringEditor, CDialogEx)

CDlgStringEditor::CDlgStringEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStringEditor::IDD, pParent)
{

}

CDlgStringEditor::~CDlgStringEditor()
{
}

void CDlgStringEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_ctrlRichEdit);
}


BEGIN_MESSAGE_MAP(CDlgStringEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgStringEditor::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgStringEditor 消息处理程序


BOOL CDlgStringEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctrlRichEdit.SetWindowText(m_string);

	SendMessage(WM_SIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgStringEditor::OnBnClickedOk()
{
	m_ctrlRichEdit.GetWindowText(m_string);
	CDialogEx::OnOK();
}


void CDlgStringEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!m_ctrlRichEdit.GetSafeHwnd())	return;

	CRect rcClient;
	GetClientRect(rcClient);

	CWnd *pBtn1 = GetDlgItem(IDOK);
	CWnd *pBtn2 = GetDlgItem(IDCANCEL);
	CRect rcBtn1, rcBtn2;
	int btnSpace=20;
	pBtn1->GetWindowRect(rcBtn1);
	pBtn2->GetWindowRect(rcBtn2);

	CWnd *pWndEdit = GetDlgItem(IDC_RICHEDIT21);
	pWndEdit->MoveWindow(0,0, rcClient.Width(), rcClient.Height() - rcBtn1.Height()*2);

	pBtn1->MoveWindow(rcClient.Width()-rcBtn1.Width()*2-btnSpace*2, 
		rcClient.Height()-btnSpace-rcBtn1.Height(),
		rcBtn1.Width(),
		rcBtn1.Height());

	pBtn2->MoveWindow(rcClient.Width()-rcBtn2.Width()-btnSpace, 
		rcClient.Height()-btnSpace-rcBtn1.Height(),
		rcBtn2.Width(),
		rcBtn2.Height());
}
