// DlgInsertControl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgInsertControl.h"
#include "afxdialogex.h"


// CDlgInsertControl 对话框

IMPLEMENT_DYNAMIC(CDlgInsertControl, CDialogEx)

CDlgInsertControl::CDlgInsertControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInsertControl::IDD, pParent)
{
	m_nPosition = 0;
}

CDlgInsertControl::~CDlgInsertControl()
{
}

void CDlgInsertControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInsertControl, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgInsertControl::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInsertControl 消息处理程序


BOOL CDlgInsertControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton *pBtn1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pBtn2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pBtn3 = (CButton *)GetDlgItem(IDC_RADIO3);

	if( g_duiProp.IsContainerUI(nodeControl) || g_duiProp.IsWindowForm(nodeControl) )
	{
		pBtn1->SetCheck(TRUE);
		return TRUE;
	}

	CControlUI *pControl = (CControlUI *)nodeControl.get_tag();
	CContainerUI *pContainer = (CContainerUI *)pControl->GetInterface(DUI_CTR_CONTAINER);
	if(pContainer)
	{
		pBtn1->SetCheck(TRUE);
		return TRUE;
	}

	pBtn1->EnableWindow(FALSE);
	pBtn2->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgInsertControl::OnBnClickedOk()
{
	CButton *pBtn1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pBtn2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pBtn3 = (CButton *)GetDlgItem(IDC_RADIO3);

	if(pBtn1->GetCheck())
		m_nPosition = 0;
	else if(pBtn2->GetCheck())
		m_nPosition = 1;
	else if(pBtn3->GetCheck())
		m_nPosition = 2;
	
	CDialogEx::OnOK();
}
