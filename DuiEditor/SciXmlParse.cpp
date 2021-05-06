#include "StdAfx.h"
#include "DuiEditor.h"
#include "SciXmlParse.h"
#include "UIManager.h"

CSciXmlParse::CSciXmlParse(void)
{
}

CSciXmlParse::~CSciXmlParse(void)
{
}

void CSciXmlParse::Parse(xml_node doc, xml_node xml)
{
	dest = doc;
	src = xml;

	if(dest.type() != src.type() && dest.type() != node_document)
	{
		AfxMessageBox(_T("根节点类型不同，无法提交更新。"));
		return;
	}

	ParseNode(xml_node(), xml_node(), dest, src);
}

void CSciXmlParse::ParseNode(xml_node nodeParent1, xml_node nodeParent2, xml_node node1, xml_node node2)
{
	if(!node1 && !node2)
		return;

	if(!node1 && node2) //node1没有的，node2有的，插入到node1中
	{
		xml_node nodeNew = nodeParent1.append_child(node2.type());
		nodeNew.set_name(node2.name());
		nodeNew.set_pos(node2);
		AddControl(nodeNew, node2);
		ParseNode(nodeParent1, nodeParent2, nodeNew, node2);
	}
	else if(node1 && !node2) //node有的，node2没有的，node1中删除掉
	{
		xml_node nodeDelete = node1;
		node1 = node1.next_sibling();
		DeleteControl(nodeDelete);
		nodeParent1.remove_child(nodeDelete);
		ParseNode(nodeParent1, nodeParent2, node1, node2);
	}
	else //node1, node2都有的。
	{
		if(node1.type() != node2.type()) //他们的类型不一致，插入新的
		{
			xml_node nodeNew = nodeParent1.insert_child_before(node2.type(), node1);
			nodeNew.set_name(node2.name());
			nodeNew.set_pos(node2);
			node1 = nodeNew;
			AddControl(nodeNew, node2);
			ParseNode(nodeParent1, nodeParent2, node1, node2);
		}
		else
		{
			switch (node1.type())
			{
			case node_document:
				{
					ParseNode(node1, node2, node1.first_child(), node2.first_child());
				}
				break;
			case node_comment:
				node1.set_value(node2.value());
				node1.set_pos(node2);
				ParseNode(nodeParent1, nodeParent2, node1.next_sibling(), node2.next_sibling());
				break;
			case node_declaration:
				node1.set_pos(node2);
				ParseAttributes(node1, node2, node1.first_attribute(), node2.first_attribute());
				ParseNode(nodeParent1, nodeParent2, node1.next_sibling(), node2.next_sibling());
				break;
			case node_element: //两个都是element
				{
					if(!CompareNodeName(node1, node2)) //NodeName不一致，node1插入新的
					{
						xml_node nodeNew = nodeParent1.insert_child_before(node2.type(), node1);
						nodeNew.set_name(node2.name());
						nodeNew.set_pos(node2);
						AddControl(nodeNew, node2);
						ParseNode(nodeParent1, nodeParent2, nodeNew, node2);
					}
					else //NodeName一样，开始匹配属性, 子节点
					{
						node1.set_pos(node2);

						if(g_duiProp.IsFontNode(node1))
						{
							ParseUIFontAttribute(node1, node2);
						}
						else if(g_duiProp.IsDefaultNode(node1))
						{
							ParseUIDefaultAttribute(node1, node2);
						}
						else if(g_duiProp.IsStyleNode(node1))
						{
							ParseUIStyleAttribute(node1, node2);
						}
						else	
							ParseAttributes(node1, node2, node1.first_attribute(), node2.first_attribute());
						ParseNode(node1, node2, node1.first_child(), node2.first_child());
						ParseNode(nodeParent1, nodeParent2, node1.next_sibling(), node2.next_sibling());
					}		
				}
				break;
			}			
		}
	}
}

void CSciXmlParse::ParseAttributes(xml_node node1, xml_node node2, xml_attribute attr1, xml_attribute attr2)
{
	if(!attr1 && !attr2)
		return;

	if(!attr1 && attr2) //attr1没有的，attr2有的，要插入到attr1中
	{
		xml_attribute attrNew = node1.append_copy(attr2);
		attrNew.set_pos(attr2);
		AddControlAttribute(node1, attrNew);
		ParseAttributes(node1, node2, attr1, attr2.next_attribute()); //attr1无效的，attr2进入下一个
	}
	else if(attr1 && !attr2) //attr1有的，attr2没有的，attr1中的删除
	{
		xml_attribute attrDelete = attr1;
		attr1 = attr1.next_attribute();
		DeleteControlAttribute(node1, attrDelete);
		node1.remove_attribute(attrDelete);
		ParseAttributes(node1, node2, attr1, attr2); //attr1进入下一个, attr2无效的.
	}
	else
	{
		if(CompareAttrName(attr1, attr2)) //名字相同，同时进入下一个属性进行比较
		{
			if(!CompareAttrValue(attr1, attr2))
			{
				attr1.set_value(attr2.value());
				AddControlAttribute(node1, attr1);
			}

			attr1.set_pos(attr2);
			ParseAttributes(node1, node2, attr1.next_attribute(), attr2.next_attribute());
		}
		else //名字不同
		{
			XString attrName = attr2.name();			
			if(node1.attribute(attrName))	//node1中已经存在attr2，删除attr1。 更新为attr2的属性值
			{
				//删除attr1
				xml_attribute attrDelete = attr1;
				attr1 = attr1.next_attribute();
				DeleteControlAttribute(node1, attrDelete);
				node1.remove_attribute(attrDelete);

				ParseAttributes(node1, node2, attr1, attr2); //attr1进入下一个(被删除的下一个)，attr2不变
			}
			else //node1中不存在attr2，新增到attr1前面
			{
				xml_attribute attrNew = node1.insert_attribute_before(attr2.name(), attr1);
				attrNew.set_value(attr2.value());
				AddControlAttribute(node1, attrNew);
				attrNew.set_pos(attr2);

				//attr1不变, attr2进入下一个
				ParseAttributes(node1, node2, attr1, attr2.next_attribute()); 
			}		
		}
	}
}

BOOL CSciXmlParse::CompareNodeName(xml_node node1, xml_node node2)
{
	return CompareString(node1.name(), node2.name());
}

BOOL CSciXmlParse::CompareAttrName(xml_attribute attr1, xml_attribute attr2)
{
	return CompareString(attr1.name(), attr2.name());
}

BOOL CSciXmlParse::CompareAttrValue(xml_attribute attr1, xml_attribute attr2)
{
	return CompareString(attr1.value(), attr2.value());
}

BOOL CSciXmlParse::CompareNodeAttributes(xml_node node1, xml_node node2)
{
	BOOL bSame = TRUE;
	xml_attribute attr1 = node1.first_attribute();
	xml_attribute attr2 = node2.first_attribute();
	for (;;)
	{
		if(!attr1 && !attr2)
			break;

		if(!CompareAttrName(attr1, attr2) || !CompareAttrValue(attr1, attr2))
		{
			bSame = FALSE;
			break;
		}

		attr1 = attr1.next_attribute();
		attr2 = attr2.next_attribute();
	}
	return bSame;
}

void CSciXmlParse::AddControl(xml_node node, xml_node nodeSample)
{
	xml_node nodeParent = node.parent();
	CControlUI *pParentControl = (CControlUI *)nodeParent.get_tag();
	if(!pParentControl)
	{
		AfxMessageBox(_T("提交更新发生错误"));
		return;
	}


	if(g_duiProp.IsFontNode(node))
	{
		AddUIFont(node, nodeSample);
		return;
	}
	else if(g_duiProp.IsDefaultNode(node))
	{
		AddUIFont(node, nodeSample);
		return;
	}
	else if(g_duiProp.IsStyleNode(node))
	{
		AddUIStyle(node, nodeSample);
		return;
	}
	else if(!g_duiProp.IsControlUI(node))
	{
		return;
	}

	CString temp;
	CContainerUI *pContainer = (CContainerUI *)pParentControl->GetInterface(DUI_CTR_CONTAINER);
	if(!pContainer)
	{
// 		int errRow = GetUIManager()->GetCodeView()->GetSciWnd()->sci_LineFromPosition(node.get_open_start_pos());
// 		temp.Format(_T("提交更新发生错误 \r\n\r\n错误行: %d \r\n\r\n错误原因: 控件没有放置容器内"), errRow);
// 		AfxMessageBox(temp);
		return;
	}

	CControlUI *pNewControl = CUIBuilder::CreateControl(node);
	if(!pNewControl)
	{
// 		int errRow = GetUIManager()->GetCodeView()->GetSciWnd()->sci_LineFromPosition(node.get_open_start_pos());
// 		temp.Format(_T("提交更新发生错误 \r\n\r\n错误行: %d \r\n\r\n错误原因: 创建控件失败"), errRow);
// 		AfxMessageBox(temp);
		return;
	}
	pNewControl->SetTag((UINT_PTR)node.internal_object());
	node.set_tag((UINT_PTR)pNewControl);

	//尝试寻找前一个兄弟控件
	CControlUI *pPrevControl = NULL;
	xml_node nodeprev = node.previous_sibling();
	while (nodeprev)
	{
		if(nodeprev.type() == node_element)
		{
			if(g_duiProp.IsControlUI(nodeprev))
			{
				pPrevControl = (CControlUI *)nodeprev.get_tag();
				if(pPrevControl)
					break;
			}
		}
		nodeprev = nodeprev.previous_sibling();
	}

	if(pPrevControl)
	{
		int nIndex = pContainer->GetItemIndex(pPrevControl);
		if(nIndex >= 0)
		{
			if(!pContainer->AddAt(pNewControl, nIndex + 1))
			{
				CUIBuilder::DeleteControl(pNewControl);
				return;
			}
			GetUIManager()->UpdateControlUI(pNewControl);
			GetUIManager()->GetTreeView()->AddNewControl(node, nodeprev, TVI_NEXT);
		}
	}	
	else
	{
		if(!pContainer->AddAt(pNewControl, 0))
		{
			CUIBuilder::DeleteControl(pNewControl);
			return;
		}
		GetUIManager()->UpdateControlUI(pNewControl);
		GetUIManager()->GetTreeView()->AddNewControl(node);
	}
}

void CSciXmlParse::DeleteControl(xml_node node)
{
	CString temp;
	xml_node nodeParent = node.parent();
	CControlUI *pParentControl = (CControlUI *)nodeParent.get_tag();
	if(!pParentControl)
	{
		//AfxMessageBox(_T("提交更新发生错误"));
		return;
	}

	if(g_duiProp.IsFontNode(node))
	{
		DeleteUIFont(node);
		return;
	}
	else if(g_duiProp.IsDefaultNode(node))
	{
		DeleteUIDefault(node);
		return;
	}
	else if(g_duiProp.IsStyleNode(node))
	{
		DeleteUIStyle(node);
		return;
	}
	else if(!g_duiProp.IsControlUI(node))
	{
		return;
	}

	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)
	{
		return;
	}

	CContainerUI *pContainer = (CContainerUI *)pParentControl->GetInterface(DUI_CTR_CONTAINER);
	if(pContainer)
	{
		pContainer->Remove(pControl);
	}
	GetUIManager()->GetTreeView()->DeleteXmlNode(node);
}

void CSciXmlParse::AddControlAttribute(xml_node node, xml_attribute attr)
{
	if(!g_duiProp.IsControlUI(node) && !g_duiProp.IsWindowForm(node))
		return;

	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl) return;
	pControl->SetAttribute(XML2T(attr.name()), XML2T(attr.value()));

	if(CompareString(_T("name"), attr.name()))
	{
		//修改控件名字，需要更新控件树
		GetUIManager()->GetTreeView()->UpdateXmlNode(node);
	}
	else if(CompareString(_T("resourceid"), attr.name()))
	{
		//手动修改resourceid
	}
}

void CSciXmlParse::DeleteControlAttribute(xml_node node, xml_attribute attr)
{
	if(!g_duiProp.IsControlUI(node))
		return;

	//删除属性，只是把属性值设为默认值
	xml_node nodeProp = g_duiProp.FindControl(XML2T(node.name()));
	if(!nodeProp) return;

	xml_node nodeAttr = g_duiProp.FindAttribute(XML2T(node.name()), XML2T(attr.name()));
	if(!nodeAttr) return;
	
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl) return;
	pControl->SetAttribute(XML2T(attr.name()), XML2T(nodeAttr.attribute(XTEXT("default")).value()));

	if(CompareString(_T("name"), attr.name()))
	{
		//修改控件名字，需要更新控件树
		node.attribute(XTEXT("name")).set_value(nodeAttr.attribute(XTEXT("default")).value());
		GetUIManager()->GetTreeView()->UpdateXmlNode(node);
	}
	else if(CompareString(_T("resourceid"), attr.name()))
	{
		//删除resourceid
	}

	GetUIManager()->UpdateControlUI(pControl);
}

void CSciXmlParse::AddUIFont(xml_node node, xml_node nodeSample)
{
	int id = nodeSample.attribute(XTEXT("id")).as_int(-1);
	CString FontName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	int size = nodeSample.attribute(XTEXT("size")).as_int(0);
	//这3个参数不合法的话，就不要创建了
	if(id <= -1 || FontName.IsEmpty() || size<=0)
		return;

	bool bold		= nodeSample.attribute(XTEXT("bold")).as_bool();
	bool underline	= nodeSample.attribute(XTEXT("underline")).as_bool();
	bool italic		= nodeSample.attribute(XTEXT("italic")).as_bool();
	bool defaultfont = nodeSample.attribute(XTEXT("default")).as_bool();
	bool shared		= nodeSample.attribute(XTEXT("shared")).as_bool();

	//把nodeSampe的属性都拷贝过来
	for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
	{
		xml_attribute attrx = node.append_attribute(attr.name());
		attrx.set_value(attr.value());
		attrx.set_pos(attr);
	}

	GetUIManager()->GetManager()->AddFont(id, FontName, size, bold, underline, italic, shared);
	if( defaultfont ) GetUIManager()->GetManager()->SetDefaultFont(FontName, GetUIManager()->GetManager()->GetDPIObj()->Scale(size), bold, underline, italic, shared);

	//目的是忽略注释
	xml_node nodeprev = node.previous_sibling();
	while(nodeprev)
	{
		if(nodeprev.type() == node_element) break;
		nodeprev = node.previous_sibling();
	}
	if(nodeprev)
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, nodeprev, TVI_NEXT);
	}
	else
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, TVI_FIRST);
	}
}

void CSciXmlParse::DeleteUIFont(xml_node node)
{
	int id	= node.attribute(XTEXT("id")).as_int(-1);
	CString FontName = XML2T(node.attribute(XTEXT("name")).value());
	int size = node.attribute(XTEXT("size")).as_int(0);
	bool shared		= node.attribute(XTEXT("shared")).as_bool();
	//这3个参数不合法的话，就不要创建了
	if(id <= -1 || FontName.IsEmpty() || size<=0)
		return;

	GetUIManager()->GetManager()->RemoveFont(id, shared);
	GetUIManager()->GetTreeView()->DeleteXmlNode(node);
}

void CSciXmlParse::ParseUIFontAttribute(xml_node node, xml_node nodeSample)
{
	int id = nodeSample.attribute(XTEXT("id")).as_int(-1);
	CString FontName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	int size = nodeSample.attribute(XTEXT("size")).as_int(0);
	//这参数不合法的话，就先忽略掉
	if(id <= -1 || FontName.IsEmpty() || size<=0)
		return;

	//属性不同，全部拷贝过来
	if(!CompareNodeAttributes(node, nodeSample))
	{
		while(node.first_attribute()) node.remove_attribute(node.first_attribute()); //删除所有属性
		for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
		{
			xml_attribute attrx = node.attribute_auto(attr.name());
			attrx.set_value(attr.value());
			attrx.set_pos(attr);
		}

		bool bold		= nodeSample.attribute(XTEXT("bold")).as_bool();
		bool underline	= nodeSample.attribute(XTEXT("underline")).as_bool();
		bool italic		= nodeSample.attribute(XTEXT("italic")).as_bool();
		bool defaultfont = nodeSample.attribute(XTEXT("default")).as_bool();
		bool shared		= nodeSample.attribute(XTEXT("shared")).as_bool();

		GetUIManager()->GetManager()->AddFont(id, FontName, size, bold, underline, italic, shared);
		if( defaultfont ) GetUIManager()->GetManager()->SetDefaultFont(FontName, GetUIManager()->GetManager()->GetDPIObj()->Scale(size), bold, underline, italic, shared);
		GetUIManager()->GetTreeView()->UpdateXmlNode(node);
	}
}

void CSciXmlParse::AddUIDefault(xml_node node, xml_node nodeSample)
{
	CString strName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	CString strValue = XML2T(nodeSample.attribute(XTEXT("value")).value());
	bool bShared = nodeSample.attribute(XTEXT("shared")).as_bool();

	if(strName.IsEmpty() || strValue.IsEmpty())
		return;

	//把nodeSampe的属性都拷贝过来
	for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
	{
		xml_attribute attrx = node.append_attribute(attr.name());
		attrx.set_value(attr.value());
		attrx.set_pos(attr);
	}

	GetUIManager()->GetManager()->AddDefaultAttributeList(strName, strValue, bShared);

	xml_node nodeprev = node.previous_sibling();
	while(nodeprev)
	{
		if(nodeprev.type() == node_element) break;
		nodeprev = node.previous_sibling();
	}
	if(nodeprev)
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, nodeprev, TVI_NEXT);
	}
	else
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, TVI_FIRST);
	}
}

void CSciXmlParse::DeleteUIDefault(xml_node node)
{
	CString strName = XML2T(node.attribute(XTEXT("name")).value());
	CString strValue = XML2T(node.attribute(XTEXT("value")).value());
	bool shared = node.attribute(XTEXT("shared")).as_bool();
	//参数不合法的话，就不要创建了
	if(strName.IsEmpty() || strValue.IsEmpty())
		return;

	GetUIManager()->GetManager()->RemoveDefaultAttributeList(strName, shared);
	GetUIManager()->GetTreeView()->DeleteXmlNode(node);
}

void CSciXmlParse::ParseUIDefaultAttribute(xml_node node, xml_node nodeSample)
{
	CString strName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	CString strValue = XML2T(nodeSample.attribute(XTEXT("value")).value());
	bool shared = nodeSample.attribute(XTEXT("shared")).as_bool();
	//参数不合法的话，就不要创建了
	if(strName.IsEmpty() || strValue.IsEmpty())
		return;

	if(!CompareNodeAttributes(node, nodeSample))
	{
		//把nodeSampe的属性都拷贝过来
		while(node.first_attribute()) node.remove_attribute(node.first_attribute()); //删除所有属性
		for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
		{
			xml_attribute attrx = node.attribute_auto(attr.name());
			attrx.set_value(attr.value());
			attrx.set_pos(attr);
		}

		GetUIManager()->GetManager()->RemoveDefaultAttributeList(strName, shared);
		GetUIManager()->GetManager()->AddDefaultAttributeList(strName, strValue, shared);
		GetUIManager()->GetTreeView()->UpdateXmlNode(node);
	}
}

void CSciXmlParse::AddUIStyle(xml_node node, xml_node nodeSample)
{
	CString strStyleName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	CString strStyleValue = XML2T(nodeSample.attribute(XTEXT("value")).value());
	bool bShared = nodeSample.attribute(XTEXT("shared")).as_bool();

	if(strStyleName.IsEmpty() || strStyleValue.IsEmpty())
		return;

	//把nodeSampe的属性都拷贝过来
	for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
	{
		xml_attribute attrx = node.append_attribute(attr.name());
		attrx.set_value(attr.value());
		attrx.set_pos(attr);
	}

	GetUIManager()->GetManager()->AddStyle(strStyleName, strStyleValue, bShared);

	xml_node nodeprev = node.previous_sibling();
	while(nodeprev)
	{
		if(nodeprev.type() == node_element) break;
		nodeprev = node.previous_sibling();
	}
	if(nodeprev)
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, nodeprev, TVI_NEXT);
	}
	else
	{
		GetUIManager()->GetTreeView()->AddNewControl(node, TVI_FIRST);
	}
}

void CSciXmlParse::DeleteUIStyle(xml_node node)
{
	CString strName = XML2T(node.attribute(XTEXT("name")).value());
	CString strValue = XML2T(node.attribute(XTEXT("value")).value());
	bool shared = node.attribute(XTEXT("shared")).as_bool();
	//参数不合法的话，就不要创建了
	if(strName.IsEmpty() || strValue.IsEmpty())
		return;

	GetUIManager()->GetManager()->RemoveStyle(strName, shared);
	GetUIManager()->GetTreeView()->DeleteXmlNode(node);
}

void CSciXmlParse::ParseUIStyleAttribute(xml_node node, xml_node nodeSample)
{
	CString strStyleName = XML2T(nodeSample.attribute(XTEXT("name")).value());
	CString strStyleValue = XML2T(nodeSample.attribute(XTEXT("value")).value());
	bool shared = nodeSample.attribute(XTEXT("shared")).as_bool();
	//参数不合法的话，就不要创建了
	if(strStyleName.IsEmpty() || strStyleValue.IsEmpty())
		return;

	if(!CompareNodeAttributes(node, nodeSample))
	{
		//把nodeSampe的属性都拷贝过来
		while(node.first_attribute()) node.remove_attribute(node.first_attribute()); //删除所有属性
		for (xml_attribute attr=nodeSample.first_attribute(); attr; attr = attr.next_attribute())
		{
			xml_attribute attrx = node.attribute_auto(attr.name());
			attrx.set_value(attr.value());
			attrx.set_pos(attr);
		}

		GetUIManager()->GetManager()->RemoveStyle(strStyleName, shared);
		GetUIManager()->GetManager()->AddStyle(strStyleName, strStyleValue, shared);
		GetUIManager()->GetTreeView()->UpdateXmlNode(node);
	}
}