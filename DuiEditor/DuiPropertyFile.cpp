#include "StdAfx.h"
#include "DuiEditor.h"
#include "DuiPropertyFile.h"
#include "DuiEditorDoc.h"
#include "UIManager.h"

#include <typeinfo>

CDuiPropertyFile::CDuiPropertyFile(void)
{
}


CDuiPropertyFile::~CDuiPropertyFile(void)
{
}

BOOL CDuiPropertyFile::LoadPropertyFile()
{
	if(!xml.load_file(g_strAppPath + _T("DuiLib.xml"), XML_PARSER_OPTIONS))
	{
		AfxMessageBox(_T("载入DuiLib属性文件失败!"));
		return FALSE;
	}

	/* //修改duilib.xml属性文件
	for (xml_node node=GetRoot().first_child(); node; node=node.next_sibling())
	{
		for (xml_node nodeAttr=node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
		{
			CString attrName = nodeAttr.attribute(_T("name")).as_string();
			CString attrComment = nodeAttr.attribute(_T("comment")).as_string();

			if(attrName == (_T("bordersize")))
			{
				nodeAttr.attribute(_T("default")).set_value(_T("0"));
			}

// 			if(attrName.Find(_T("image")) >= 0)
// 			{
// 				nodeAttr.attribute(_T("type")).set_value(_T("IMAGE"));
// 			}
// 
// 			if(attrName.Find(_T("lign")) >= 0)
// 			{
// 				if(attrComment.Find(_T("left")) >= 0)
// 				{
// 					if(!nodeAttr.attribute(_T("value")))
// 					{
// 						xml_attribute attrValue = nodeAttr.insert_attribute_after(_T("value"), nodeAttr.attribute(_T("type")));
// 						attrValue.set_value(_T("left,right,center"));
// 					}
// 				}
// 				if(attrComment.Find(_T("top")) >= 0)
// 				{
// 					if(!nodeAttr.attribute(_T("value")))
// 					{
// 						xml_attribute attrValue = nodeAttr.insert_attribute_after(_T("value"), nodeAttr.attribute(_T("type")));
// 						attrValue.set_value(_T("top,bottom,center"));
// 					}
// 				}
// 			}
		}
	}
	xml.save_to_default_file();
	*/

	InitToolBoxIcon();
	return TRUE;
}

xml_node CDuiPropertyFile::GetRoot()
{
	return xml.child(XTEXT("Controls"));
}
/*
BOOL CDuiPropertyFile::IsControlUI(LPCTSTR className)
{
	if(CompareString(className, _T("Control")))
		return TRUE;

	xml_node nodeFind = FindControl(className);
	xml_attribute attr = nodeFind.attribute(XTEXT("parent"));
	if(attr)
	{
		xml_node nodeFind = FindControl(XML2T(attr.value()));
		if(nodeFind)
		{
			return IsControlUI(XML2T(nodeFind.name()));
		}
	}
	return FALSE;
}


BOOL CDuiPropertyFile::IsContainerUI(LPCTSTR className)
{
	if(CompareString(className, _T("Container")))
		return TRUE;

	xml_node nodeFind = FindControl(className);
	xml_attribute attr = nodeFind.attribute(XTEXT("parent"));
	if(attr)
	{
		return IsContainerUI(XML2T(attr.value()));
	}
	return FALSE;
}


BOOL CDuiPropertyFile::IsWindowForm(LPCTSTR className)
{
	if(CompareString(className, XTEXT("Window")))
		return TRUE;
	return FALSE;
}
*/
BOOL CDuiPropertyFile::IsControlUI(xml_node node)
{
	if(CompareString(node.name(), XTEXT("Control")))
		return TRUE;

	xml_node nodeFind = FindControl(XML2T(node.name()));
	xml_attribute attr = nodeFind.attribute(XTEXT("parent"));
	if(attr)
	{
		xml_node nodeFind = FindControl(XML2T(attr.value()));
		if(nodeFind)
		{
			return IsControlUI(nodeFind);
		}
	}
	return FALSE;
}


BOOL CDuiPropertyFile::IsContainerUI(xml_node node)
{
	if(CompareString(node.name(), XTEXT("Container")))
		return TRUE;

	xml_node nodeFind = FindControl(XML2T(node.name()));
	xml_attribute attr = nodeFind.attribute(XTEXT("parent"));
	if(attr)
	{
		xml_node nodeFind2 = FindControl(XML2T(attr.value()));
		if(nodeFind2)
		{
			return IsContainerUI(nodeFind2);
		}
	}
	return FALSE;
}


BOOL CDuiPropertyFile::IsWindowForm(xml_node node)
{
	if(CompareString(node.name(), XTEXT("Window")))
		return TRUE;
	return FALSE;
}

BOOL CDuiPropertyFile::IsFontNode(xml_node nodeNode)
{
	if(CompareString(nodeNode.name(), _T("Font")))
		return TRUE;
	return FALSE;
}

BOOL CDuiPropertyFile::IsDefaultNode(xml_node node)
{
	if(CompareString(node.name(), _T("Default")))
		return TRUE;
	return FALSE;
}

BOOL CDuiPropertyFile::IsStyleNode(xml_node node)
{
	if(CompareString(node.name(), _T("Style")))
		return TRUE;
	return FALSE;
}

BOOL CDuiPropertyFile::IsNeedInputText(xml_node node)
{
	if(IsWindowForm(node))
		return FALSE;

	return TRUE;
}

xml_node CDuiPropertyFile::FindControl(LPCTSTR className)
{
	xml_node retnode;

	pugi::xml_node root = g_duiProp.GetRoot();
	for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling())
	{
		if(CompareString(node.name(), className))
		{
			retnode = node;
			break;
		}
	}
	return retnode;
}

xml_node CDuiPropertyFile::FindAttribute(LPCTSTR className, LPCTSTR attrName)
{
	xml_node node;
	node = FindControl(className);
	
	for (;;)
	{
		if(!node) return xml_node();

		xml_node nodeAttr = node.find_child_by_attribute(XTEXT("name"), T2XML(attrName));
		if(nodeAttr)
		{
			return nodeAttr;
		}
		else
		{
			CString parentName = XML2T(node.attribute(XTEXT("parent")).value());
			if(parentName.IsEmpty())
			{
				return xml_node();
			}
			else
			{
				node = FindControl(parentName);
			}
		}
	}	
}

void CDuiPropertyFile::InitToolBoxIcon()
{
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_ImageList.Create(16,   16,   ILC_COLORDDB|ILC_MASK,   2,2);  
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_ACTIVE)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_BUTTON)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_COMBO)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CONTAINER)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CONTROL)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_DEFAULT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_EDIT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_FONT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_HORIZONTALLAYOUT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_LABEL)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_OPTION)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_POINTER)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_PROGRESS)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_SCROLLBAR)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_SLIDER)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_TABLAYOUT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_TEXT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_TILELAYOUT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_UNKNOW)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_VERTICALLAYOUT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_WINDOW)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_LIST)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_RICHEDIT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CHECKBOX)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_GRID)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_IE)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CEF)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_TREECTRL)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_TREENODE)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_DATETIME)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CUSTOMCONTROL)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_CHILDLAYOUT)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_IMAGEBOX)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_GROUPBOX)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_MENU)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_GIFANIM)));
	m_ImageList.Add(::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOLBOX_STYLE)));
}

UINT CDuiPropertyFile::GetControlIconIndex(xml_node nodeDoc)
{
	if(nodeDoc.attribute(XTEXT("custombasedfrom")))
	{
		return TREENODETYPE_CUSTOMCONTROL;
	}

	CString nodeName = XML2T(nodeDoc.name());

	if(CompareString(nodeName, _T("ActiveX")))
	{
		return TREENODETYPE_ACTIVE;
	}
	else if(CompareString(nodeName, _T("Button")))
	{
		return TREENODETYPE_BUTTON;
	}
	else if(CompareString(nodeName, _T("Combo")))
	{
		return TREENODETYPE_COMBO;
	}
	else if(CompareString(nodeName, _T("ComboEx")))
	{
		return TREENODETYPE_COMBO;
	}
	else if(CompareString(nodeName, _T("Container")))
	{
		return TREENODETYPE_CONTAINER;
	}
	else if(CompareString(nodeName, _T("Control")))
	{
		return TREENODETYPE_CONTROL;
	}
	else if(CompareString(nodeName, _T("Default")))
	{
		return TREENODETYPE_DEFAULT;
	}
	else if(CompareString(nodeName, _T("Edit")))
	{
		return TREENODETYPE_EDIT;
	}
	else if(CompareString(nodeName, _T("Font")))
	{
		return TREENODETYPE_FONT;
	}
	else if(CompareString(nodeName, _T("HorizontalLayout")))
	{
		return TREENODETYPE_HORIZONTALLAYOUT;
	}
	else if(CompareString(nodeName, _T("Label")))
	{
		return TREENODETYPE_LABEL;
	}
	else if(CompareString(nodeName, _T("Option")))
	{
		return TREENODETYPE_OPTION;
	}
	else if(CompareString(nodeName, _T("Pointer")))
	{
		return TREENODETYPE_POINTER;
	}
	else if(CompareString(nodeName, _T("Progress")))
	{
		return TREENODETYPE_PROGRESS;
	}
	else if(CompareString(nodeName, _T("ScrollBar")))
	{
		return TREENODETYPE_SCROLLBAR;
	}
	else if(CompareString(nodeName, _T("HScrollBar")))
	{
		return TREENODETYPE_SCROLLBAR;
	}
	else if(CompareString(nodeName, _T("VScrollBar")))
	{
		return TREENODETYPE_SCROLLBAR;
	}
	else if(CompareString(nodeName, _T("Slider")))
	{
		return TREENODETYPE_SLIDER;
	}
	else if(CompareString(nodeName, _T("TabLayout")))
	{
		return TREENODETYPE_TABLAYOUT;
	}
	else if(CompareString(nodeName, _T("AnimationTabLayout")))
	{
		return TREENODETYPE_TABLAYOUT;
	}
	else if(CompareString(nodeName, _T("Text")))
	{
		return TREENODETYPE_TEXT;
	}
	else if(CompareString(nodeName, _T("TileLayout")))
	{
		return TREENODETYPE_TILELAYOUT;
	}
	else if(CompareString(nodeName, _T("VerticalLayout")))
	{
		return TREENODETYPE_VERTICALLAYOUT;
	}
	else if(CompareString(nodeName, _T("Window")))
	{
		return TREENODETYPE_WINDOW;
	}
	else if(CompareString(nodeName, _T("ChildWindow")))
	{
		return TREENODETYPE_WINDOW;
	}
	else if(CompareString(nodeName, _T("List")))
	{
		return TREENODETYPE_LIST;
	}
	else if(CompareString(nodeName, _T("RichEdit")))
	{
		return TREENODETYPE_RICHEDIT;
	}
	else if(CompareString(nodeName, _T("CheckBox")))
	{
		return TREENODETYPE_CHECKBOX;
	}
	else if(CompareString(nodeName, _T("GridCtrl")))
	{
		return TREENODETYPE_GRIDCTRL;
	}
	else if(CompareString(nodeName, _T("WebBrowser")))
	{
		return TREENODETYPE_IEXPLORER;
	}
	else if(CompareString(nodeName, _T("WebKitCef")))
	{
		return TREENODETYPE_LIBCEF;
	}
	else if(CompareString(nodeName, _T("TreeView")))
	{
		return TREENODETYPE_TREEVIEW;
	}
	else if(CompareString(nodeName, _T("TreeNode")))
	{
		return TREENODETYPE_TREENODE;
	}
	else if(CompareString(nodeName, _T("DateTime")))
	{
		return TREENODETYPE_DATETIME;
	}
	else if(CompareString(nodeName, _T("CustomControl")))
	{
		return TREENODETYPE_CUSTOMCONTROL;
	}
	else if(CompareString(nodeName, _T("ChildLayout")))
	{
		return TREENODETYPE_CHILDLAYOUT;
	}
	else if(CompareString(nodeName, _T("ImageBox")))
	{
		return TREENODETYPE_IMAGEBOX;
	}
	else if(CompareString(nodeName, _T("GroupBox")))
	{
		return TREENODETYPE_GROUPBOX;
	}
	else if(CompareString(nodeName, _T("Menu")))
	{
		return TREENODETYPE_MENU;
	}
	else if(CompareString(nodeName, _T("MenuElement")))
	{
		return TREENODETYPE_MENU;
	}
	else if(CompareString(nodeName, _T("GifAnim")))
	{
		return TREENODETYPE_GIFANIM;
	}
	else if(CompareString(nodeName, _T("Style")))
	{
		return TREENODETYPE_STYLE;
	}
	else
	{
		return TREENODETYPE_UNKNOW;
	}
}

void CDuiPropertyFile::FilterDefaultValue(xml_node nodeDoc, CUIManager *pUIManager)
{
	xml_node nodeDuiProp = g_duiProp.FindControl(XML2T(nodeDoc.name()));
	if(!nodeDuiProp)
	{
		//找不到, 会不会是自定义控件
		xml_attribute attrCustom = nodeDoc.attribute(XTEXT("custombasedfrom"));
		if(attrCustom)
		{
			nodeDuiProp = g_duiProp.FindControl(XML2T(attrCustom.value()));
		}
	}

	xml_attribute attr=nodeDoc.first_attribute();
	while (attr)
	{
		xml_attribute attr2 = attr.next_attribute();

		CString pstrValue = XML2T(attr.value());
		if(pstrValue.IsEmpty())
		{
			if(pUIManager) pUIManager->GetCodeView()->DeleteAttribute(nodeDoc, attr);
			nodeDoc.remove_attribute(attr);
		}
		else
			_FilterDefaultValue(nodeDoc, attr, nodeDuiProp, pUIManager);

		attr = attr2;
	}
}

void CDuiPropertyFile::_FilterDefaultValue(xml_node nodeDoc, xml_attribute attr, xml_node nodeDuiProp, CUIManager *pUIManager)
{
	xml_node nodeAttr = nodeDuiProp.find_child_by_attribute(XTEXT("name"), attr.name());//nodeDuiProp.child(attr.name());
	if(nodeAttr)
	{
		if(CompareString(attr.value(), nodeAttr.attribute(XTEXT("default")).value()))
		{
			if(pUIManager) pUIManager->GetCodeView()->DeleteAttribute(nodeDoc, attr);
			nodeDoc.remove_attribute(attr);
			return;
		}
		return;
	}

	CString pstrParent = XML2T(nodeDuiProp.attribute(XTEXT("parent")).value());
	if(pstrParent.IsEmpty())	return;

	xml_node nodeParentDuiProp = FindControl(pstrParent);
	if(nodeParentDuiProp)
	{
		_FilterDefaultValue(nodeDoc, attr, nodeParentDuiProp, pUIManager);
	}
}

void CDuiPropertyFile::FilterPosWidthHeight(xml_node nodeDoc, CUIManager *pUIManager)
{
	if(!g_cfg.bFilterPosWidthHeight) return;

	//float控件不处理
	if(!nodeDoc.attribute(XTEXT("float")).as_bool())
	{
		//如果没有pos属性，不处理。
		xml_attribute attrPos = nodeDoc.attribute(XTEXT("pos"));
		if(attrPos) 
		{
			xml_attribute attrWidth = nodeDoc.attribute(XTEXT("width"));
			xml_attribute attrHeight = nodeDoc.attribute(XTEXT("height"));

			//pos属性无法转换成rect，不处理
			CDuiRect rc;
			if(rc.FromString(XML2T(attrPos.as_string())))
			{
				//pos属性的大小 不等于 width或height属性，不处理。 因为手写的同学可能写错了，不要擅自处理。
				if(rc.GetWidth() == attrWidth.as_int() && rc.GetHeight() == attrHeight.as_int())
				{
					//好了，把pos属性去掉吧
					if(pUIManager) pUIManager->GetCodeView()->DeleteAttribute(nodeDoc, attrPos);
					nodeDoc.remove_attribute(attrPos);
				}	
			}	
		}
	}
}

xml_attribute CDuiPropertyFile::AddAttribute(xml_node nodeControl, LPCTSTR attrName, RECT rc, CUIManager *pUIManager)
{
	CString strValue;
	strValue.Format(_T("%d,%d,%d,%d"), rc.left, rc.top, rc.right, rc.bottom);
	return AddAttribute(nodeControl, attrName, strValue, pUIManager);
}

xml_attribute CDuiPropertyFile::AddAttribute(xml_node nodeControl, LPCTSTR attrName, int attrValue, CUIManager *pUIManager)
{
	CString strValue;
	strValue.Format(_T("%d"), attrValue);
	return AddAttribute(nodeControl, attrName, strValue, pUIManager);
}

xml_attribute CDuiPropertyFile::AddAttribute(xml_node nodeControl, LPCTSTR attrName, bool attrValue, CUIManager *pUIManager)
{
	if(attrValue)
	{
		return AddAttribute(nodeControl, attrName, _T("true"), pUIManager);
	}
	return AddAttribute(nodeControl, attrName, _T("false"), pUIManager);
}

xml_attribute CDuiPropertyFile::AddAttribute(xml_node nodeControl, LPCTSTR attrName, LPCTSTR attrValue, CUIManager *pUIManager)
{		
	if(pUIManager)
	{
		//pView->GetUIManager()->GetCmdHistory()->AddModifyControl(nodeControl);
		//pUIManager->GetDocument()->SetModifiedFlag(TRUE);
	}

	xml_attribute attr = _addAttribute(nodeControl, attrName, attrValue);
	
	if(pUIManager)
	{
		//实时更新到XML页面
		pUIManager->GetCodeView()->AddAttribute(nodeControl, attr);
	}

	return attr;
}

xml_attribute CDuiPropertyFile::_addAttribute(xml_node nodeControl, LPCTSTR attrName, LPCTSTR attrValue)
{	
	CString ControlName = XML2T(nodeControl.name());
	
	{	//attribute已经存在, 写入attribute
		xml_attribute attr = nodeControl.attribute(T2XML(attrName));
		if(attr)
		{
			attr.set_value(T2XML(attrValue));
			return attr;
		}
	}

	if(!nodeControl.last_attribute()) //没有一个属性, 直接插入
	{
		xml_attribute attr = nodeControl.append_attribute(T2XML(attrName));
		attr.set_value(T2XML(attrValue));
		return attr;
	}

	for (xml_attribute attr=nodeControl.last_attribute(); attr; attr=attr.previous_attribute())
	{
		if(IsCorrectOrder(ControlName, attrName, XML2T(attr.name()))) //attrName在attr后面, 这插入到attr后面
		{
			xml_attribute attr2 = nodeControl.insert_attribute_after(T2XML(attrName), attr);
			attr2.set_value(T2XML(attrValue));
			return attr2;
		}
	}

	xml_attribute attr = nodeControl.prepend_attribute(T2XML(attrName));
	attr.set_value(T2XML(attrValue));
	return attr;
}


BOOL CDuiPropertyFile::IsCorrectOrder(LPCTSTR controlName, LPCTSTR attrNameAppend, LPCTSTR attrName)
{
	xml_node nodeprop = FindControl(controlName);
	if(!nodeprop)
	{
		return TRUE;
	}

	BOOL bAppendFind = FALSE;
	BOOL bAttrFind = FALSE;
	for (xml_node node=nodeprop.child(XTEXT("Attribute")); node; node=node.next_sibling(XTEXT("Attribute")))
	{
		CString a = XML2T(node.attribute(XTEXT("name")).value());
		if(CompareString(attrNameAppend, a))
		{
			bAppendFind = TRUE;
		}
		if(CompareString(attrName, a))
		{
			bAttrFind = TRUE;
		}

		if(bAppendFind || bAttrFind)
		{
			if(bAttrFind)			//先找到attrName
				return TRUE;

			if(bAppendFind)			//先找到attrNameAppend
				return FALSE;
		}
	}

	//如果都没找到, 回溯到父控件定义
	return IsCorrectOrder(XML2T(nodeprop.attribute(XTEXT("parent")).value()), attrNameAppend, attrName);
}