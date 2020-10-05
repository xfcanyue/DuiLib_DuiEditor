// ImageEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditor.h"
#include "afxdialogex.h"

#include "MainFrm.h"
// CImageEditor 对话框

IMPLEMENT_DYNAMIC(CImageEditor, CDialogEx)

CImageEditor::CImageEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageEditor::IDD, pParent)
{
	m_pFrame = NULL;
	m_pParentGrid = NULL;
}

CImageEditor::~CImageEditor()
{
}

void CImageEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageEditor, CDialogEx)
	ON_BN_CLICKED(IDOK, &CImageEditor::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CImageEditor::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CImageEditor 消息处理程序


BOOL CImageEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = (CDuiEditorViewDesign *)pMain->GetActiveUIView();
	m_pDoc = pView->GetDocument();

	CRect rectDummy;
	GetClientRect(rectDummy);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	m_pFrame = (CImageEditorFrame* )RUNTIME_CLASS(CImageEditorFrame)->CreateObject();
	m_pFrame->Create(NULL, _T(""), dwStyle, rectDummy, this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CImageEditor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CImageEditor::OnBnClickedCancel()
{

	CDialogEx::OnCancel();
}


void CImageEditor::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(!m_pFrame)	return;
	if(!m_pFrame->GetSafeHwnd())	return;
	m_pFrame->MoveWindow(0, 0, cx, cy);
}


void CImageEditor::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_pFrame && m_pFrame->GetSafeHwnd())
	{
		m_pFrame->DestroyWindow();
	}
}

BOOL CImageEditor::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CImageEditor::SetAttributeValue(LPCTSTR szAttribute)
{
	xml_node nodedata;

	CString strXML = _T("<IMAGE ");
	strXML += szAttribute;
	strXML += _T(" />");
	xml_parse_result ret = m_nodeImage.load(T2XML(strXML));
	if(ret.status != pugi::status_ok)//如果匹配不成功, 就认为只有文件名, 无其他参数
	{
		strXML = _T("<IMAGE />");
		ret = m_nodeImage.load(T2XML(strXML));
		nodedata = m_nodeImage.child(XTEXT("IMAGE"));

		g_duiProp.AddAttribute(nodedata, _T("file"), szAttribute, NULL);
	}
	else
	{
		nodedata = m_nodeImage.child(XTEXT("IMAGE"));
	}
}

CString CImageEditor::GetAttributeValue()
{
	xml_node node = m_nodeImage.child(XTEXT("IMAGE"));

	//过滤默认属性
	g_duiProp.FilterDefaultValue(node, NULL);
	g_duiProp.FilterPosWidthHeight(node, NULL);

	//判断source是否等于图像原始大小, 等于则删除source属性定义
	xml_attribute attr = node.attribute(XTEXT("source"));
	if(attr)
	{
		CDuiRect rc(XML2T(attr.value()));
		if(m_rcImage == rc)
			node.remove_attribute(attr);
	}

	CString strImage, temp;
	//判断是否只有文件名属性
	BOOL bCustomDefine = FALSE;
	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		if(CompareString(attr.name(), _T("file")))
		{
			continue;
		}
		bCustomDefine = TRUE;
	}

	if(bCustomDefine)
	{
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			temp.Format(_T("%s='%s' "), XML2T(attr.name()), XML2T(attr.value()));
			strImage += temp;
		}
	}
	else
	{
		strImage = node.attribute(XTEXT("file")).value();
	}

	return strImage;
}

LRESULT CImageEditor::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(message == WM_KICKIDLE)
	{
		if(m_pFrame && m_pFrame->GetSafeHwnd())
		{
			m_pFrame->SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
		}
	}

	return CDialogEx::DefWindowProc(message, wParam, lParam);
}
