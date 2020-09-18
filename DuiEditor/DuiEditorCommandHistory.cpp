#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIManager.h"
#include "DockPropertyWnd.h"

CDuiEditorCommandHistory::CDuiEditorCommandHistory(void)
{
	m_bCanUndo = FALSE;
	m_bCanRedo = FALSE;
	m_bInit = FALSE;
}


CDuiEditorCommandHistory::~CDuiEditorCommandHistory(void)
{
}

void CDuiEditorCommandHistory::Init()
{
	m_root = GetUIManager()->GetDocument()->m_doc.root().child(_T("History"));
	if(m_root)
	{
		m_nodeUndo = m_root.child(_T("UNDO"));
		m_nodeRedo = m_root.child(_T("REDO"));
	}
	else
	{
		m_root = GetUIManager()->GetDocument()->m_doc.root().append_child(_T("History"));
		m_nodeUndo = m_root.append_child(_T("UNDO"));
		m_nodeRedo = m_root.append_child(_T("REDO"));
	}

	m_bInit = TRUE;
}

void CDuiEditorCommandHistory::UnInit()
{
	m_bInit = FALSE;
}

void CDuiEditorCommandHistory::Undo()
{
	//like <ModifyControl .....>
	xml_node node = m_nodeUndo.first_child();
	if(!node)	return;

	LPCTSTR cmdType = node.name();

	if(CompareString(cmdType, _T("AddControl")))
	{
		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)node.attribute(_T("control")).get_tag());
		xml_node nodeParent = xml_node((xml_node_struct *)node.attribute(_T("parent")).get_tag());
		xml_node nodeSibling = xml_node((xml_node_struct *)node.attribute(_T("presibling")).get_tag());

		//保存当前文档节点到Redo
		xml_node nodeNewRedo = m_nodeRedo.prepend_child((_T("AddControl")));

		//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
		xml_attribute attrParent = nodeNewRedo.append_attribute(_T("parent"));
		attrParent.set_value(nodeParent.name());
		attrParent.set_tag((UINT_PTR)nodeParent.internal_object());

		if(nodeDocument.previous_sibling()) //如果有前一个兄弟项, 保存起来.
		{
			xml_attribute attrSibling = nodeNewRedo.append_attribute(_T("presibling"));
			attrSibling.set_value(nodeSibling.name());
			attrSibling.set_tag((UINT_PTR)nodeSibling.internal_object());
		}

		xml_attribute attrControl = nodeNewRedo.append_attribute(_T("control"));
		attrControl.set_value(nodeDocument.name());
		attrControl.set_tag((UINT_PTR)nodeDocument.internal_object());

		_delete(nodeDocument);

		//删除后, 把文档节点移动到Redo list;
		xml_node nodeRedoControl = nodeNewRedo.prepend_move(nodeDocument);

		//从现有的undo列表中移除
		m_nodeUndo.remove_child(node); 
	}
	else if(CompareString(cmdType, _T("ModifyControl")))
	{
		//like <VerticalLayout ......>
		xml_node nodeControl = node.first_child();

		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)nodeControl.get_tag());

		//保存当前文档节点到Redo
		xml_node nodeNewRedo = m_nodeRedo.prepend_child((_T("ModifyControl")));
		xml_node nodeRedoControl = nodeNewRedo.prepend_child(nodeControl.name());
		for (xml_attribute attr=nodeDocument.first_attribute(); attr; attr=attr.next_attribute())
		{
			nodeRedoControl.append_attribute(attr.name()).set_value(attr.value());
		}
		nodeRedoControl.set_tag((UINT_PTR)nodeDocument.internal_object());

		_modify(nodeControl);

		//从现有的undo列表中移除
		m_nodeUndo.remove_child(node); 
	}
	else if(CompareString(cmdType, _T("DeleteControl")))
	{
		xml_node nodeControl = node.first_child();

		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)node.attribute(_T("control")).get_tag());
		xml_node nodeParent = xml_node((xml_node_struct *)node.attribute(_T("parent")).get_tag());
		xml_node nodeSibling = xml_node((xml_node_struct *)node.attribute(_T("presibling")).get_tag());

		//保存当前文档节点到undo
		xml_node nodeNewRedo = m_nodeRedo.prepend_child((_T("DeleteControl")));

		//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
		xml_attribute attrParent = nodeNewRedo.append_attribute(_T("parent"));
		attrParent.set_value(nodeParent.name());
		attrParent.set_tag((UINT_PTR)nodeParent.internal_object());

		if(nodeSibling) //如果有前一个兄弟项, 保存起来.
		{
			xml_attribute attrSibling = nodeNewRedo.append_attribute(_T("presibling"));
			attrSibling.set_value(nodeSibling.name());
			attrSibling.set_tag((UINT_PTR)nodeSibling.internal_object());
		}

		xml_attribute attrControl = nodeNewRedo.append_attribute(_T("control"));
		attrControl.set_value(nodeDocument.name());
		attrControl.set_tag((UINT_PTR)nodeDocument.internal_object());


		if(nodeSibling)
			nodeParent.insert_move_after(nodeDocument, nodeSibling);
		else
			nodeParent.prepend_move(nodeDocument);

		_add(nodeControl, nodeParent, nodeSibling);

		//从现有的undo列表中移除
		m_nodeUndo.remove_child(node); 
	}
}

void CDuiEditorCommandHistory::Redo()
{
	xml_node node = m_nodeRedo.first_child();
	if(!node)	return;

	LPCTSTR cmdType = node.name();

	if(CompareString(cmdType, _T("AddControl")))
	{
		xml_node nodeControl = node.first_child();

		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)node.attribute(_T("control")).get_tag());
		xml_node nodeParent = xml_node((xml_node_struct *)node.attribute(_T("parent")).get_tag());
		xml_node nodeSibling = xml_node((xml_node_struct *)node.attribute(_T("presibling")).get_tag());

		//保存当前文档节点到undo
		xml_node nodeNewUndo = m_nodeUndo.prepend_child((_T("AddControl")));

		//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
		xml_attribute attrParent = nodeNewUndo.append_attribute(_T("parent"));
		attrParent.set_value(nodeParent.name());
		attrParent.set_tag((UINT_PTR)nodeParent.internal_object());

		if(nodeSibling) //如果有前一个兄弟项, 保存起来.
		{
			xml_attribute attrSibling = nodeNewUndo.append_attribute(_T("presibling"));
			attrSibling.set_value(nodeSibling.name());
			attrSibling.set_tag((UINT_PTR)nodeSibling.internal_object());
		}

		xml_attribute attrControl = nodeNewUndo.append_attribute(_T("control"));
		attrControl.set_value(nodeDocument.name());
		attrControl.set_tag((UINT_PTR)nodeDocument.internal_object());

		if(nodeSibling)
			nodeParent.insert_move_after(nodeDocument, nodeSibling);
		else
			nodeParent.prepend_move(nodeDocument);

		_add(nodeControl, nodeParent, nodeSibling);

		//从现有的undo列表中移除
		m_nodeRedo.remove_child(node); 
	}
	else if(CompareString(cmdType, _T("ModifyControl")))
	{
		xml_node nodeControl = node.first_child();

		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)nodeControl.get_tag());

		//保存当前文档节点到Undo.
		xml_node nodeNewUndo = m_nodeUndo.prepend_child(_T("ModifyControl"));//m_nodeUndo.prepend_child(nodeControl.name());
		xml_node nodeControlUndo = nodeNewUndo.prepend_child(nodeControl.name());
		for (xml_attribute attr=nodeDocument.first_attribute(); attr; attr=attr.next_attribute())
		{
			nodeControlUndo.append_attribute(attr.name()).set_value(attr.value());
		}
		nodeControlUndo.set_tag((UINT_PTR)nodeDocument.internal_object());

		_modify(nodeControl);

		//从现有的Redo列表中移除
		m_nodeRedo.remove_child(node);
	}
	else if(CompareString(cmdType, _T("DeleteControl")))
	{
		//获取原XML文档的节点
		xml_node nodeDocument = xml_node((xml_node_struct *)node.attribute(_T("control")).get_tag());
		xml_node nodeParent = xml_node((xml_node_struct *)node.attribute(_T("parent")).get_tag());
		xml_node nodeSibling = xml_node((xml_node_struct *)node.attribute(_T("presibling")).get_tag());

		//保存当前文档节点到Undo
		xml_node nodeNewUndo = m_nodeUndo.prepend_child((_T("DeleteControl")));

		//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
		xml_attribute attrParent = nodeNewUndo.append_attribute(_T("parent"));
		attrParent.set_value(nodeParent.name());
		attrParent.set_tag((UINT_PTR)nodeParent.internal_object());

		if(nodeDocument.previous_sibling()) //如果有前一个兄弟项, 保存起来.
		{
			xml_attribute attrSibling = nodeNewUndo.append_attribute(_T("presibling"));
			attrSibling.set_value(nodeSibling.name());
			attrSibling.set_tag((UINT_PTR)nodeSibling.internal_object());
		}

		xml_attribute attrControl = nodeNewUndo.append_attribute(_T("control"));
		attrControl.set_value(nodeDocument.name());
		attrControl.set_tag((UINT_PTR)nodeDocument.internal_object());

		_delete(nodeDocument);

		//删除后, 把文档节点移动到Redo list;
		xml_node nodeRedoControl = nodeNewUndo.prepend_move(nodeDocument);

		//从现有的undo列表中移除
		m_nodeRedo.remove_child(node); 
	}
}

BOOL CDuiEditorCommandHistory::CanUndo()
{
// 	if(!m_bInit)
// 		return FALSE;
// 
// 	return m_nodeUndo.last_child() != NULL;
	return TRUE;
}

BOOL CDuiEditorCommandHistory::CanRedo()
{
// 	if(!m_bInit)
// 		return FALSE;
// 
// 	return m_nodeRedo.last_child() != NULL;
	return TRUE;
}

BOOL CDuiEditorCommandHistory::AddNewControl(xml_node node)
{
	PrepareAddCommand();

	xml_node nodeFlag = m_nodeUndo.prepend_child(_T("AddControl"));

	//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
	xml_attribute attrParent = nodeFlag.append_attribute(_T("parent"));
	attrParent.set_value(node.parent().name());
	attrParent.set_tag((UINT_PTR)node.parent().internal_object());

	if(node.previous_sibling()) //如果有前一个兄弟项, 保存起来.
	{
		xml_attribute attrSibling = nodeFlag.append_attribute(_T("presibling"));
		attrSibling.set_value(node.previous_sibling().name());
		attrSibling.set_tag((UINT_PTR)node.previous_sibling().internal_object());
	}

	xml_attribute attrControl = nodeFlag.append_attribute(_T("control"));
	attrControl.set_value(node.name());
	attrControl.set_tag((UINT_PTR)node.internal_object());

//	nodeControl.set_tag((UINT_PTR)node.internal_object());

	return TRUE;
//	return nodeControl;
}

xml_node CDuiEditorCommandHistory::AddModifyControl(xml_node node)
{
	PrepareAddCommand();

	xml_node nodeFlag = m_nodeUndo.prepend_child(_T("ModifyControl"));
	xml_node nodeControl = nodeFlag.prepend_child(node.name());

	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		nodeControl.append_attribute(attr.name()).set_value(attr.value());
	}

	nodeControl.set_tag((UINT_PTR)node.internal_object());

	return nodeControl;
}

BOOL CDuiEditorCommandHistory::AddDeleteControl(xml_node node)
{
	PrepareAddCommand();

	//保存当前文档节点到Redo
	xml_node nodeNewUndo = m_nodeUndo.prepend_child((_T("DeleteControl")));

	//保存在父节点的位置, 为了将来要搬回来. 需要父节点和兄弟的节点的信息
	xml_attribute attrParent = nodeNewUndo.append_attribute(_T("parent"));
	attrParent.set_value(node.parent().name());
	attrParent.set_tag((UINT_PTR)node.parent().internal_object());

	if(node.previous_sibling()) //如果有前一个兄弟项, 保存起来.
	{
		xml_attribute attrSibling = nodeNewUndo.append_attribute(_T("presibling"));
		attrSibling.set_value(node.previous_sibling().name());
		attrSibling.set_tag((UINT_PTR)node.previous_sibling().internal_object());
	}

	xml_attribute attrControl = nodeNewUndo.append_attribute(_T("control"));
	attrControl.set_value(node.name());
	attrControl.set_tag((UINT_PTR)node.internal_object());

	_delete(node);

	//删除后, 把文档节点移动到Redo list;
	xml_node nodeUndoControl = nodeNewUndo.prepend_move(node);

	return TRUE;
}

void CDuiEditorCommandHistory::PrepareAddCommand()
{
	//添加记录之前, 先删除REDO
	for (;;)
	{
		xml_node node=m_nodeRedo.first_child();
		if(!node) break;
		m_nodeRedo.remove_child(node);
	}
}

void CDuiEditorCommandHistory::_add(xml_node nodeControl, xml_node nodeParent, xml_node nodeSibling)
{
	CUIManager *pManager = GetUIManager();

	//获取原XML文档的节点
	xml_node nodeDocument = nodeControl; //xml_node((xml_node_struct *)nodeControl.get_tag());

	//寻找上一个兄弟节点
	CContainerUI *pContainer = (CContainerUI *)nodeParent.get_tag();// = m_pManager->FindContainer(nodeParent);
	int nInsertAt = 0;
	if(nodeSibling)
	{
		CControlUI *pSiblingControl = (CControlUI *)nodeSibling.get_tag();
		if(pSiblingControl)
		{
			nInsertAt = pContainer->GetItemIndex(pSiblingControl);
			if(nInsertAt >= 0)
			{
				nInsertAt++;
			}
		}
	}

	//创建控件
	LPCTSTR pstrClass = nodeDocument.name();
	CControlUI *pNewControl = CUIBuilder::CreateControl(pstrClass);
	if(!pNewControl) return;
	if(!pContainer->Add(pNewControl))
	{
#ifdef DUILIB_VERSION_ORIGINAL
		pNewControl->Delete();
#else
		delete pNewControl;
#endif
		pNewControl = NULL;
		return;
	}

	//载入控件默认属性
	LPCTSTR pDefaultAttributes = pManager->GetManager()->GetDefaultAttributeList(pstrClass);
	if( pDefaultAttributes ) 
	{
#ifndef DUILIB_VERSION_ORIGINAL
		pNewControl->ApplyAttributeList(pDefaultAttributes);
#else
		pNewControl->SetAttributeList(pDefaultAttributes);
#endif
	}

	//载入控件当前属性
	for (xml_attribute attr = nodeDocument.first_attribute(); attr; attr=attr.next_attribute())
	{
		pNewControl->SetAttribute(attr.name(), attr.value());
	}


	//保存文档和控件的双向指针
	pNewControl->SetTag((UINT_PTR)nodeDocument.internal_object());
	nodeDocument.set_tag((UINT_PTR)pNewControl);

	//插入左边控件树
	HTREEITEM hTreeSibling = pManager->GetTreeView()->FindXmlNode(nodeSibling); 
	if(!hTreeSibling)	hTreeSibling = TVI_FIRST;
	HTREEITEM hNewControl = pManager->GetTreeView()->AddNewControl(nodeDocument, hTreeSibling);

	//清除选择，选中新控件
	pManager->GetUiTracker()->RemoveAll();
	pManager->GetUiTracker()->Add(nodeDocument, pNewControl->GetPos());

	//接下来创建子控件
	_addChild(nodeDocument, pNewControl, hNewControl);

	GetUIManager()->GetPropList()->InitProp(nodeDocument);
	pManager->UpdateControlUI(nodeDocument);
}

void CDuiEditorCommandHistory::_addChild(xml_node nodeDocument1, CControlUI *pParent, HTREEITEM hTreeParent)
{
	CUIManager *pManager = GetUIManager();
	for (xml_node node=nodeDocument1.first_child(); node; node=node.next_sibling())
	{
		//创建控件
		LPCTSTR pstrClass = node.name();
		CContainerUI *pContainer = static_cast<CContainerUI *>(pParent->GetInterface(DUI_CTR_CONTAINER));
		if(!pContainer) continue;

		CControlUI *pNewControl = CUIBuilder::CreateControl(pstrClass);
		if(!pNewControl) return;
		
		if(!pContainer->Add(pNewControl))
		{
			CUIBuilder::DeleteControl(pNewControl);
			return;
		}

		//载入控件默认属性
		LPCTSTR pDefaultAttributes = pManager->GetManager()->GetDefaultAttributeList(pstrClass);
		if( pDefaultAttributes ) 
		{
#ifndef DUILIB_VERSION_ORIGINAL
			pNewControl->ApplyAttributeList(pDefaultAttributes);
#else
			pNewControl->SetAttributeList(pDefaultAttributes);
#endif
		}

		//载入控件当前属性
		for (xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute())
		{
			pNewControl->SetAttribute(attr.name(), attr.value());
		}


		//保存文档和控件的双向指针
		pNewControl->SetTag((UINT_PTR)node.internal_object());
		node.set_tag((UINT_PTR)pNewControl);

		//插入左边控件树
		HTREEITEM hNewControl = pManager->GetTreeView()->AddNewControl(node, hTreeParent);

		//选中新控件
		pManager->GetUiTracker()->Add(node, pNewControl->GetPos());

		_addChild(node, pNewControl, hNewControl);
	}
}

void CDuiEditorCommandHistory::_modify(xml_node nodeControl)
{
	CUIManager *pManager = GetUIManager();

	//获取原XML文档的节点
	xml_node nodeDocument = xml_node((xml_node_struct *)nodeControl.get_tag());

	//先清除文档节点的属性
	for (;;)
	{
		xml_attribute attr = nodeDocument.first_attribute();
		if(!attr)	break;
		nodeDocument.remove_attribute(attr);
	}

	//重新写入文档节点的属性
	for (xml_attribute attr=nodeControl.first_attribute(); attr; attr=attr.next_attribute())
	{
		nodeDocument.append_attribute(attr.name()).set_value(attr.value());
	}

	//更新视图
	pManager->UpdateControlUI(nodeDocument);
	pManager->GetTreeView()->UpdateXmlNode(nodeDocument);
}

void CDuiEditorCommandHistory::_delete(xml_node nodeControl)
{
	CUIManager *pManager = GetUIManager();

	//获得要删除的控件
	CControlUI *pControl = (CControlUI *)nodeControl.get_tag();

	xml_node nodeContainer = nodeControl.parent();
	if(g_duiProp.IsBaseFromContainer(nodeContainer.name()))
	{
		CContainerUI *pContainer = (CContainerUI *)nodeContainer.get_tag();
		pContainer->Remove(pControl);
		pContainer->NeedUpdate();
	}

	pManager->GetTreeView()->DeleteXmlNode(nodeControl);
}