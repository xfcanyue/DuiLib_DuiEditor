// DlgCreateDuiDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgCreateDuiDocument.h"
#include "afxdialogex.h"


// CDlgCreateDuiDocument 对话框

IMPLEMENT_DYNAMIC(CDlgCreateDuiDocument, CDialogEx)

CDlgCreateDuiDocument::CDlgCreateDuiDocument(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateDuiDocument::IDD, pParent)
{

}

CDlgCreateDuiDocument::~CDlgCreateDuiDocument()
{
}

void CDlgCreateDuiDocument::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_staPicture);
}


BEGIN_MESSAGE_MAP(CDlgCreateDuiDocument, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCreateDuiDocument::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgCreateDuiDocument::OnSelchangeList1)
END_MESSAGE_MAP()


// CDlgCreateDuiDocument 消息处理程序


BOOL CDlgCreateDuiDocument::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//遍历文件夹
	CFileFind finder;
	BOOL bFind;
	CString FileName;
	bFind = finder.FindFile(g_strAppPath + _T("DuiModules\\*.*"));
	while(bFind)
	{
		bFind = finder.FindNextFile();
		if(!finder.IsDots())	//是不是目录(.   ..)
		{
			if(finder.IsDirectory())	//是文件夹
			{
				FileName = finder.GetFileName();
				m_listBox.AddString(FileName);
			}
		}
	}
	finder.Close();	

	m_listBox.SelectString(-1, _T("基本页"));

	//m_listBox.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCreateDuiDocument::OnBnClickedOk()
{
	m_listBox.GetText(m_listBox.GetCurSel(), m_strModuleName);
	CDialogEx::OnOK();
}


void CDlgCreateDuiDocument::OnSelchangeList1()
{
	int nSel = m_listBox.GetCurSel();
	if(nSel >= 0)
	{
		CString strFolder;
		m_listBox.GetText(nSel, strFolder);

	}
}
