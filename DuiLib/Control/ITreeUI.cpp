#include "StdAfx.h"
#include "IGridUI.h"

namespace DuiLib
{

static CStdControlPool<TNodeData> poolNode;

//////////////////////////////////////////////////////////////////////////
ITreeUI::ITreeUI(void)
{
	m_nIndentWidth = 32;
	m_bViewLine = TRUE;
	m_nNodeHeight = 32;
	m_bMultiSelect = FALSE;

	m_pFocusNode = NULL;
	m_NodeData.SetSaveIndexMap(true);
}
	
ITreeUI::~ITreeUI(void)
{
//	DWORD dw = GetTickCount();
	DeleteAllNode();
// 	dw = GetTickCount() - dw;
// 	CDuiString s;
// 	s.Format(_T("%d"), dw);
// 	MessageBox(NULL, s, s, MB_OK);
}

void ITreeUI::SetNodeTag(TNodeData *pNode, UINT_PTR tag)
{
	m_mapTag[tag] = pNode;
	pNode->SetTag(tag);
}

TNodeData *ITreeUI::FindNodeByTag(UINT tag)
{
	std::map<UINT_PTR, TNodeData *>::iterator it = m_mapTag.find(tag);
	if (it!=m_mapTag.end()) 
	{
		return it->second;
	}
	return NULL;
}

TNodeData *ITreeUI::InsertNode(LPCTSTR lpszItem, LPCTSTR lpszIconImage, TNodeData *hParent, TNodeData *hInsertAfter)
{
	TNodeData *pRowParent = NULL;
	TNodeData *pRowInsertAfter = NULL;
	TNodeData *pNewRow = NULL;

	if(hParent != TNODE_ROOT)
	{
		pRowParent = (TNodeData *)hParent;
	}

	pNewRow = AllocNodeData();
	pNewRow->m_pParent = pRowParent;
	pNewRow->SetText(lpszItem);
	pNewRow->SetImageIcon(lpszIconImage);

	if(pRowParent == NULL) //没有root，直接插入一行
	{
		m_NodeData.Add(pNewRow);
	}
	else
	{
		if(hInsertAfter == TNODE_LAST)
		{
			int row = pRowParent->m_childs.GetSize();
			pRowParent->m_childs.InsertAt(row, pNewRow);
		}
		else if(hInsertAfter == TNODE_FIRST)
		{
			pRowParent->m_childs.InsertAt(0, pNewRow);
		}
		else
		{
			pRowInsertAfter = (TNodeData *)hInsertAfter;
			int row = pRowParent->m_childs.Find(pRowInsertAfter);
			if(row < 0) 
			{
				FreeNodeData(pNewRow);
				return NULL;
			}
			pRowParent->m_childs.InsertAt(row+1, pNewRow);
		}
	}

	return pNewRow;
}

TNodeData *ITreeUI::InsertNode(LPCTSTR lpszItem, TNodeData *hParent, TNodeData *hInsertAfter)
{
	return InsertNode(lpszItem, NULL, hParent, hInsertAfter);
}

void ITreeUI::DeleteNode(TNodeData *pNodeData)
{
	for (int i=0; i<pNodeData->m_childs.GetSize(); i++)
	{
		DeleteNode(pNodeData->GetChild(i));
	}

	std::map<UINT_PTR, TNodeData *>::iterator it2 = m_mapTag.find(pNodeData->GetTag());
	if(it2 != m_mapTag.end()) {
		m_mapTag.erase(it2);
	}

	if(GetFocusNode() == pNodeData) 
		SetFocusNode(NULL);

	if(IsRootNode(pNodeData))
	{
		m_NodeData.Remove(pNodeData);
	}
	else
	{
		pNodeData->m_pParent->m_childs.Remove(pNodeData);
	}
	SendNotify(DUI_MSGTYPE_DELETEITEM, (WPARAM)pNodeData, 0, false);
	FreeNodeData(pNodeData);
	Refresh(true);
}

void ITreeUI::DeleteAllNode()
{
	m_mapSelectedNodes.clear();
	m_mapTag.clear();
	m_pFocusNode = NULL;
	for (int i=0; i<m_NodeData.GetSize(); i++)
	{
		TNodeData *pNodeData = (TNodeData *)m_NodeData.GetAt(i);
		_DeleteAllItem(pNodeData);
	}
	m_NodeData.Empty();
}

TNodeData *ITreeUI::GetRootNode()
{
	return (TNodeData *)m_NodeData.GetAt(0);
}

BOOL ITreeUI::IsRootNode(TNodeData *pNode)
{
	return pNode->GetParent() == NULL;
}

TNodeData *ITreeUI::GetNextNode(TNodeData *pNode, bool bGetVisibleNode)
{
	if(!pNode)	return NULL;
	TNodeData *pNodeNext = NULL;

	//有没有子项
	if(!bGetVisibleNode || pNode->IsExpand())
	{
		pNodeNext = GetFirstChildren(pNode);
		if(pNodeNext) return pNodeNext;
	}

	//有没有兄弟项
	pNodeNext = GetNextSiblingNode(pNode);
	if(!pNodeNext)
	{
		//回滚寻找父项的兄弟项
		TNodeData *pNodeTemp = pNode;
		while (TRUE)
		{
			TNodeData *pNodeParent = pNodeTemp->GetParent();

			if(!pNodeParent) break;

			pNodeNext = GetNextSiblingNode(pNodeParent);
			if(pNodeNext) break;

			pNodeTemp = pNodeParent;
		}
	}
	return pNodeNext;
	/*
	if(!pNode)	return NULL;
	TNodeData *pNodeNext = NULL;

	//有没有子项
	pNodeNext = GetFirstChildren(pNode);
	if(pNodeNext) return pNodeNext;

	//有没有兄弟项
	pNodeNext = GetNextSiblingNode(pNode);
	if(!pNodeNext)
	{
		//回滚寻找父项的兄弟项
		TNodeData *pNodeTemp = pNode;
		while (TRUE)
		{
			TNodeData *pNodeParent = pNodeTemp->GetParentNode();

			if(!pNodeParent) break;

			pNodeNext = GetNextSiblingNode(pNodeParent);
			if(pNodeNext) break;

			pNodeTemp = pNodeParent;
		}
	}
	return pNodeNext;
	*/
}

TNodeData *ITreeUI::GetFirstChildren(TNodeData *pNode)
{
	return pNode->GetChild(0);
}

TNodeData *ITreeUI::GetNextSiblingNode(TNodeData *pNode)
{
	//有没有父项
	TNodeData *pParentNode = pNode->GetParent();
	if(pParentNode)
	{
		int iIndex = pParentNode->m_childs.Find(pNode);
		return pParentNode->GetChild(++iIndex);
	}
	int iIndex = m_NodeData.Find(pNode);
	if(iIndex >= 0)
	{
		return (TNodeData *)m_NodeData.GetAt(++iIndex);
	}
	return NULL;
}

TNodeData *ITreeUI::GetNextPrevNode(TNodeData *pNode)
{
	//有没有父项
	TNodeData *pParentNode = pNode->GetParent();
	if(pParentNode)
	{
		int iIndex = pParentNode->m_childs.Find(pNode);
		if(iIndex < 0) return NULL;
		return pParentNode->GetChild(--iIndex);
	}
	int iIndex = m_NodeData.Find(pNode);
	if(iIndex >= 0)
	{
		return (TNodeData *)m_NodeData.GetAt(--iIndex);
	}
	return NULL;
}

void ITreeUI::ClearSeletedNodes()
{
	std::map<TNodeData *, TNodeData *>::iterator it;
	for (it=m_mapSelectedNodes.begin(); it!=m_mapSelectedNodes.end(); it++) 
	{
		it->second->Select(false);
	}
	m_mapSelectedNodes.clear();
	SetFocusNode(NULL);
}

void ITreeUI::SelectNode(TNodeData *pNode, bool bSeleted)
{
	if(bSeleted)
	{
		m_mapSelectedNodes[pNode] = pNode;
		pNode->Select(bSeleted);
	}
	else
	{
		std::map<TNodeData *, TNodeData *>::iterator it = m_mapSelectedNodes.find(pNode);
		if (it!=m_mapSelectedNodes.end()) 
		{
			m_mapSelectedNodes.erase(it);
		}
		pNode->Select(bSeleted);
	}
}

bool ITreeUI::IsSelectedNode(TNodeData *pNode)
{
	return pNode->IsSelected();
}

BOOL ITreeUI::MoveNode(TNodeData *pNode, TNodeData *pNewParent, TNodeData *phInsertAfter)
{
	return FALSE;
}

TNodeData *ITreeUI::AllocNodeData()
{
	TNodeData *pTreeData = poolNode.Alloc();
	pTreeData->m_bSelected = FALSE;
	return pTreeData;
}

void ITreeUI::FreeNodeData(TNodeData *pTreeData)
{
	poolNode.Free(pTreeData);
}

void ITreeUI::_DeleteAllItem(TNodeData *pNodeData)
{
	for (int i=0; i<pNodeData->m_childs.GetSize(); i++)
	{
		_DeleteAllItem(pNodeData->GetChild(i));
	}
	pNodeData->m_childs.Empty();
	FreeNodeData(pNodeData);
}



} //namespace