// DlgBuildLanguagePackageEx.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgBuildLanguagePackageEx.h"
#include "afxdialogex.h"

#include "UIManager.h"
// CDlgBuildLanguagePackageEx 对话框

IMPLEMENT_DYNAMIC(CDlgBuildLanguagePackageEx, CDialogEx)

CDlgBuildLanguagePackageEx::CDlgBuildLanguagePackageEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBuildLanguagePackageEx::IDD, pParent)
{

}

CDlgBuildLanguagePackageEx::~CDlgBuildLanguagePackageEx()
{
}

void CDlgBuildLanguagePackageEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbLang);
}


BEGIN_MESSAGE_MAP(CDlgBuildLanguagePackageEx, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgBuildLanguagePackageEx::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBuildLanguagePackageEx 消息处理程序


BOOL CDlgBuildLanguagePackageEx::OnInitDialog()
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
	xml.append_child(node_comment).set_value(T2XML(_T(" id属性对应于控件上的resourceid ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" value属性对应语言相关的文本 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" resourceid 由设计器维护递增, 这是以数字作为资源id关键字建立的表 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 为每个xml生成一个语言包 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 这个操作是打开语言包文件进行重新写入 ")));
	xml.append_child(node_comment).set_value(T2XML(_T(" 请做好备份！ ")));

	xml_node nodeRes = xml.child_auto("Language");
	xml_node nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(1);
	nodeText.attribute_auto("value").set_value(T2XML(_T("繁體中文")));

	nodeText = nodeRes.append_child("Text");
	nodeText.attribute_auto("id").set_value(2);
	nodeText.attribute_auto("value").set_value(T2XML(_T("DuiLib設計器")));

	xml_writer_string w;
	xml.print(w);
	sci.sci_SetText(w.strText.c_str());
	sci.sci_SetReadOnly(TRUE);

	CString strSkinDir = g_proj.GetProjectPath(); //GetUIManager()->GetDocument()->GetSkinPath();
	if(!strSkinDir.IsEmpty())
	{
		strSkinDir += g_cfg.strLangPath;
		strSkinDir += _T("\\");
	}
	CFileFind finder;
	BOOL bFind = finder.FindFile(strSkinDir + _T("*.*"));
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())	//是文件夹，文件夹就是语言包的名字
		{
			CString lang = finder.GetFileName();
			m_cbLang.AddString(lang);
		}
	}
	m_cbLang.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBuildLanguagePackageEx::OnBnClickedOk()
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

	CString strLang;
	m_cbLang.GetWindowText(strLang);
	if(GetUIManager()->GetDocument()->m_mLangPackage.Find(strLang))
	{
		AfxMessageBox(_T("已经存在相同语言的语言包！"));
		return;
	}

	xml_document *xml = new xml_document;
	GetUIManager()->GetDocument()->m_mLangPackage.Insert(strLang, xml);
	xml_node rootLang = xml->child_auto("Language");
	_buildLangPackage(rootLang, root);

	CDialogEx::OnOK();
}

void CDlgBuildLanguagePackageEx::_buildLangPackage(xml_node rootLang, xml_node nodeSkin)
{
	//只保存这3种类型
	CString text = XML2T(nodeSkin.attribute(XTEXT("text")).as_string());
	CString tooltip = XML2T(nodeSkin.attribute(XTEXT("tooltip")).as_string());
	CString tipvalue = XML2T(nodeSkin.attribute(XTEXT("tipvalue")).as_string());

	int resourceid = nodeSkin.attribute(XTEXT("resourceid")).as_int();
	if(resourceid > 0)
	{
		//已经存在的id不要重复写入
		xml_node nodeFind = rootLang.find_child_by_attribute("Text", "id", nodeSkin.attribute(XTEXT("resourceid")).value());
		if(!text.IsEmpty() || !tooltip.IsEmpty() || !tipvalue.IsEmpty())
		{
			if(!nodeFind)
			{
				nodeFind = rootLang.append_child("Text");
				nodeFind.attribute_auto("id").set_value(resourceid);
			}
		}

		if(!text.IsEmpty())
		{
			nodeFind.attribute_auto("text").set_value(T2XML(text));
		}
		if(!tooltip.IsEmpty())
		{
			nodeFind.attribute_auto("tooltip").set_value(T2XML(tooltip));
		}
		if(!tipvalue.IsEmpty())
		{
			nodeFind.attribute_auto("tipvalue").set_value(T2XML(tipvalue));
		}
	}

	for (xml_node node = nodeSkin.first_child(); node; node=node.next_sibling())
	{
		_buildLangPackage(rootLang, node);
	}	
}