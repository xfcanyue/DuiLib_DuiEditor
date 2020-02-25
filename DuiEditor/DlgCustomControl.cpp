// DlgCustomControl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgCustomControl.h"
#include "afxdialogex.h"


// CDlgCustomControl 对话框

IMPLEMENT_DYNAMIC(CDlgCustomControl, CDialogEx)

CDlgCustomControl::CDlgCustomControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCustomControl::IDD, pParent)
{

	m_strClassName = _T("");
}

CDlgCustomControl::~CDlgCustomControl()
{
}

void CDlgCustomControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strClassName);
	DDX_Control(pDX, IDC_COMBO1, m_cbParentControl);
}


BEGIN_MESSAGE_MAP(CDlgCustomControl, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCustomControl::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgCustomControl::OnChangeEdit1)
END_MESSAGE_MAP()


// CDlgCustomControl 消息处理程序


BOOL CDlgCustomControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	xml_node root = g_duiProp.GetRoot();
	for (xml_node node=root.first_child(); node; node=node.next_sibling())
	{
		LPCTSTR ptrClass = node.name();
		if(g_duiProp.IsBaseFromControlUI(ptrClass))
		{
			m_cbParentControl.AddString(ptrClass);
		}
	}
	m_cbParentControl.SelectString(-1, _T("Container"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCustomControl::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	m_cbParentControl.GetLBText(m_cbParentControl.GetCurSel(), m_strParentControl);

	if(m_strParentControl.IsEmpty())
		return;

	if(m_strClassName.IsEmpty())
	{
		AfxMessageBox(_T("请输入自定义控件的类名."));
		return;
	}

	CDialogEx::OnOK();
}


void CDlgCustomControl::OnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
