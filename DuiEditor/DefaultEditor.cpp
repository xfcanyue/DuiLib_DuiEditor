// DefaultEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DefaultEditor.h"
#include "afxdialogex.h"


#include "MainFrm.h"
// CDefaultEditor 对话框

IMPLEMENT_DYNAMIC(CDefaultEditor, CDialogEx)

CDefaultEditor::CDefaultEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDefaultEditor::IDD, pParent)
{
	m_pFrame = NULL;
}

CDefaultEditor::~CDefaultEditor()
{
}

void CDefaultEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDefaultEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDefaultEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDefaultEditor::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDefaultEditor 消息处理程序


BOOL CDefaultEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
	m_pDoc = pView->GetDocument();


// 	CString strTitle;
// 	strTitle.Format(_T("编辑 %s 属性"), m_nodeImage.name());
// 	SetWindowText(strTitle);

	CRect rectDummy;
	GetClientRect(rectDummy);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	m_pFrame = (CDefaultEditorFrame *)RUNTIME_CLASS(CDefaultEditorFrame)->CreateObject();
	m_pFrame->Create(NULL, _T(""), dwStyle, rectDummy, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDefaultEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CDefaultEditor::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDefaultEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!m_pFrame)	return;
	if(!m_pFrame->GetSafeHwnd())	return;
	m_pFrame->MoveWindow(0, 0, cx, cy);
}


void CDefaultEditor::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_pFrame && m_pFrame->GetSafeHwnd())
	{
		m_pFrame->DestroyWindow();
	}
}


BOOL CDefaultEditor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CDefaultEditor::SetAttributeValue(LPCTSTR szControlName, LPCTSTR szAttribute)
{
	m_strClassName = szControlName;

	CString strXML = _T("<");
	strXML += m_strClassName;
	strXML += " ";
	strXML += szAttribute;
	strXML += _T(" />");
	xml_parse_result ret = m_nodeControl.load(T2XML(strXML));
	if(ret.status != pugi::status_ok)//如果匹配不成功, 就认为只有文件名, 无其他参数
	{
		strXML = _T("<");
		strXML += m_strClassName;
		strXML += _T(" />");
		ret = m_nodeControl.load(T2XML(strXML));
	}
}

CString CDefaultEditor::GetAttributeValue()
{
	//过滤默认属性
	g_duiProp.FilterDefaultValue(m_nodeControl.child(T2XML(m_strClassName)), NULL);
	g_duiProp.FilterPosWidthHeight(m_nodeControl.child(T2XML(m_strClassName)), NULL);

	CString strText, temp;
	xml_node root = m_nodeControl.child(T2XML(m_strClassName));
	for (xml_attribute attr=root.first_attribute(); attr; attr=attr.next_attribute())
	{
		temp.Format(_T("%s=\"%s\" "), XML2T(attr.name()), XML2T(attr.value()));
		strText += temp;
	}

	return strText;
}