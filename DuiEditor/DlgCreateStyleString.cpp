// DlgCreateStyleString.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgCreateStyleString.h"
#include "afxdialogex.h"


// CDlgCreateStyleString 对话框

IMPLEMENT_DYNAMIC(CDlgCreateStyleString, CDialogEx)

CDlgCreateStyleString::CDlgCreateStyleString(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateStyleString::IDD, pParent)
{

	m_strStyleName = _T("");
	m_strClassName = _T("");
	m_strStyleString = _T("");
}

CDlgCreateStyleString::~CDlgCreateStyleString()
{
}

void CDlgCreateStyleString::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strStyleName);
	DDX_Text(pDX, IDC_EDIT7, m_strClassName);
	DDX_Check(pDX, IDC_CHECK1, m_bShared);
	DDX_Text(pDX, IDC_EDIT2, m_strStyleString);
}


BEGIN_MESSAGE_MAP(CDlgCreateStyleString, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCreateStyleString::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCreateStyleString::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgCreateStyleString::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgCreateStyleString::OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT7, &CDlgCreateStyleString::OnChangeEdit7)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgCreateStyleString::OnClickedCheck1)
END_MESSAGE_MAP()


// CDlgCreateStyleString 消息处理程序


BOOL CDlgCreateStyleString::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strStyleName.Format(_T("style_%s"), XML2T(m_node.name()));
	m_strClassName = XML2T(m_node.name());
	m_bShared = TRUE;
	UpdateData(FALSE);

	BuildString();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgCreateStyleString::BuildString()
{
	if(!UpdateData(TRUE))	return;

	xml_document doc;
	xml_node node = doc.append_child("Style");
	node.attribute_auto("name").set_value(T2XML(m_strStyleName));
	node.attribute_auto("class").set_value(T2XML(m_strClassName));
	node.attribute_auto("shared").set_value(m_bShared == TRUE);
	
	CStringA strValue;
	for (xml_attribute a=m_node.first_attribute(); a; a=a.next_attribute())
	{
		strValue += a.name();
		strValue += "=\"";
		strValue += a.value();
		strValue += "\" ";
	}
	node.attribute_auto("value").set_value(strValue);
	
	xml_writer_string writer;
	node.print(writer);
	m_strStyleString = XML2T(writer.strText.c_str());
	UpdateData(FALSE);
}

void CDlgCreateStyleString::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CDlgCreateStyleString::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgCreateStyleString::OnBnClickedButton1()
{
	if(!UpdateData(TRUE))	return;

	if(OpenClipboard())
	{
		EmptyClipboard();

		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (_tcslen(m_strStyleString)+1) * sizeof(TCHAR));
		TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
		_tcscpy(buffer, m_strStyleString);
		GlobalUnlock(clipbuffer);
#ifdef _UNICODE
		SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
		SetClipboardData(CF_TEXT, clipbuffer);
#endif
		CloseClipboard();

		AfxMessageBox(_T("样式内容已复制到剪贴板！"));
	}
}


void CDlgCreateStyleString::OnChangeEdit1()
{
	BuildString();
}


void CDlgCreateStyleString::OnChangeEdit7()
{
	BuildString();
}


void CDlgCreateStyleString::OnClickedCheck1()
{
	BuildString();
}
