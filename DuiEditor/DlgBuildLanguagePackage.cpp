// DlgBuildLanguagePackage.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgBuildLanguagePackage.h"
#include "afxdialogex.h"

#include "UIManager.h"
// CDlgBuildLanguagePackage 对话框

IMPLEMENT_DYNAMIC(CDlgBuildLanguagePackage, CDialogEx)

CDlgBuildLanguagePackage::CDlgBuildLanguagePackage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBuildLanguagePackage::IDD, pParent)
{

}

CDlgBuildLanguagePackage::~CDlgBuildLanguagePackage()
{
}

void CDlgBuildLanguagePackage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBuildLanguagePackage, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgBuildLanguagePackage::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBuildLanguagePackage 消息处理程序


BOOL CDlgBuildLanguagePackage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rectDummy;
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		AfxMessageBox(_T("create SciLexer window error."));
		return -1;      // 未能创建
	}
	sci.InitXML();
	sci.sci_SetMargins(0);
	sci.sci_UsePopup(FALSE);

	CRect rcClient, rc;
	GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_WND)->GetWindowRect(rc);
	sci.MoveWindow(0,0,rcClient.Width(), rc.Height());

	xml_document xml;
	xml.append_child(node_comment).set_value(T2XML(_T(" 这个接口生成的语言包的类型如下 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" id属性对应于控件上的文本 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" value属性对应语言相关的文本 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 简单的说，就是以控件text为关键字建立的一张表。 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 这个操作是打开语言包文件进行更新写入 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 多个语言包的同步维护建议使用NodePad++的Compare插件 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 请做好备份！ ")));

	xml_node nodeRes = xml.child_auto("Res");
	xml_node nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(T2XML(_T("简体中文")));
	nodeText.attribute_auto("value").set_value(T2XML(_T("繁體中文")));

	nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(T2XML(_T("DuiEditor")));
	nodeText.attribute_auto("value").set_value(T2XML(_T("DuiLib設計器")));

	xml_writer_string w;
	xml.print(w);
	sci.sci_SetText(w.strText.c_str());
	sci.sci_SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBuildLanguagePackage::OnBnClickedOk()
{
	xml_node root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Window"));
	if(!root)
	{
		root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Menu"));
		if(!root)
		{
			AfxMessageBox(_T("只有根节点为Window或Menu才能生成语言包！"));
			return;
		}
	}

	static TCHAR BASED_CODE szFilter[] = _T("xml(*.xml)|*.xml||");
	CFileDialog fileDlg(FALSE,_T("xml"), GetUIManager()->GetManager()->GetResourcePath() + _T("lang"),  
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return;

	xml_document xml;
	xml_parse_result ret = xml.load_file(fileDlg.GetPathName());

	xml_node rootLang = xml.child_auto("Res");
	_buildLangPackage(rootLang, root);
	xml.save_file(fileDlg.GetPathName());

	CDialogEx::OnOK();
}

void CDlgBuildLanguagePackage::_buildLangPackage(xml_node rootLang, xml_node nodeSkin)
{
	CString text = XML2T(nodeSkin.attribute(XTEXT("text")).as_string());
	CString tooltip = XML2T(nodeSkin.attribute(XTEXT("tooltip")).as_string());
	CString tipvalue = XML2T(nodeSkin.attribute(XTEXT("tipvalue")).as_string());
	BOOL bResourceText = nodeSkin.attribute(XTEXT("resourcetext")).as_bool();
	if(bResourceText)
	{
		if(!text.IsEmpty())
		{
			//已经存在的文本不要重复写入
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(text));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(text));
				nodeLang.attribute_auto("value").set_value("");
			}		
		}

		if(!tooltip.IsEmpty())
		{
			//已经存在的文本不要重复写入
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(tooltip));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(tooltip));
				nodeLang.attribute_auto("value").set_value("");
			}		
		}

		if(!tipvalue.IsEmpty())
		{
			//已经存在的文本不要重复写入
			xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", T2XML(tipvalue));
			if(!nodeFind)
			{
				xml_node nodeLang = rootLang.append_child("Text");
				nodeLang.attribute_auto("id").set_value(T2XML(tipvalue));
				nodeLang.attribute_auto("value").set_value("");
			}		
		}
	}

	for (xml_node node = nodeSkin.first_child(); node; node=node.next_sibling())
	{
		_buildLangPackage(rootLang, node);
	}	
}