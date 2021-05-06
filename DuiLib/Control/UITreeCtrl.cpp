#include "StdAfx.h"
#include "UITreeCtrl.h"

namespace DuiLib
{
	/*****************************************************************************/
	IMPLEMENT_DUICONTROL(CTreeCtrlUI)

	CTreeCtrlUI::CTreeCtrlUI( void ) : m_nIndent(16),
		m_dwItemBkColor(0), m_dwItemSelectedBkColor(0), m_dwItemHotBkColor(0xFFE9F5FF), m_dwItemLineColor(0),
		m_dwItemFocusBkColor(0xFFC1E3FF), m_pFocusNode(NULL)
	{
		SetAutoCalcHeight(true);
	}

	CTreeCtrlUI::~CTreeCtrlUI( void )
	{

	}

	LPCTSTR CTreeCtrlUI::GetClass() const
	{
		return _T("TreeCtrlUI");
	}

	LPVOID CTreeCtrlUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_TREECTRL) == 0 ) return static_cast<CTreeCtrlUI*>(this);
		return __super::GetInterface(pstrName);
	}

	UINT CTreeCtrlUI::GetControlFlags() const
	{
		return UIFLAG_TABSTOP;
	}

	CTreeItemUI *CTreeCtrlUI::GetFocusNode() const			
	{ 
		return m_pFocusNode;				
	}

	void CTreeCtrlUI::SetFocusNode(CTreeItemUI *pNode)		
	{ 
		if(pNode == m_pFocusNode) return;
		m_pFocusNode = pNode; Invalidate(); 
	}

	void CTreeCtrlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("horizattr")) == 0 )
		{
			SetStyleHoriz(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("folderattr")) == 0 )
		{
			SetStyleFolder(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("checkboxattr")) == 0 )
		{
			SetStyleCheckBox(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("iconattr")) == 0 )
		{
			SetStyleIcon(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("textattr")) == 0 )
		{
			SetStyleText(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("indent")) == 0 )
		{
			SetIndent(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("itembkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("itemselectedbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemSelectedBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("itemhotbkcolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemHotBkColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("itemlinecolor")) == 0 )
		{
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetItemLineColor(clrColor);
		}
		else
			__super::SetAttribute(pstrName, pstrValue);
	}

	bool CTreeCtrlUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		bool bPaint = __super::DoPaint(hDC, rcPaint, pStopControl);
		if(bPaint)
		{
			//画线啊
		}
		return bPaint;
	}

	bool CTreeCtrlUI::Add(CControlUI* pControl)
	{
		if(pControl->GetInterface(DUI_CTR_TREEITEM))
		{
			CTreeItemUI *pNode = static_cast<CTreeItemUI *>(pControl);
			pNode->SetParentNode(NULL);
			pNode->SetOwner(this);
			return __super::Add(pNode);
		}
		return false;
	}

	bool CTreeCtrlUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if(pControl->GetInterface(DUI_CTR_TREEITEM))
		{
			CTreeItemUI *pNode = static_cast<CTreeItemUI *>(pControl);
			pNode->SetParentNode(NULL);
			pNode->SetOwner(this);
			return __super::AddAt(pNode, iIndex);
		}
		return false;
	}

	bool CTreeCtrlUI::Remove(CControlUI* pControl, bool bDoNotDestroy)
	{
		if(pControl == m_pFocusNode) 
			m_pFocusNode = NULL;
		return __super::Remove(pControl, bDoNotDestroy);
	}

	void CTreeCtrlUI::RemoveAll()
	{
		m_pFocusNode = NULL;
		return __super::RemoveAll();
	}

	void CTreeCtrlUI::DoInit()
	{
		EnableScrollBar(true, true);
	}

	SIZE CTreeCtrlUI::EstimateSize(SIZE szAvailable)
	{
		return __super::EstimateSize(szAvailable);
	}

	CTreeItemUI *CTreeCtrlUI::GetRootNode()
	{
		if(GetCount() <= 0) return NULL;
		return (CTreeItemUI *)GetItemAt(0);
	}

	CTreeItemUI *CTreeCtrlUI::InsertNode(LPCTSTR lpszItem, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode)
	{
		CTreeItemUI *pNewNode = new CTreeItemUI;
		pNewNode->SetText(lpszItem);

		if(!InsertNode(pNewNode, pParentNode, pAfterNode))
		{
			delete pNewNode;
			return NULL;
		}
		return pNewNode;
	}

	BOOL CTreeCtrlUI::InsertNode(CTreeItemUI *pNewNode, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode)
	{
		if(pParentNode == NULL)
		{
			if(Add(pNewNode)) return TRUE;
			else return FALSE;
		}

		if(!pAfterNode)
		{
			if(pParentNode->Add(pNewNode)) return TRUE;
			else return FALSE;
		}

		int iIndex = pParentNode->GetItemIndex(pAfterNode);
		if(pParentNode->AddAt(pNewNode, iIndex+1)) return TRUE;
		else return FALSE;

		return FALSE;
	}

	BOOL CTreeCtrlUI::DeleteNode(CTreeItemUI *pNode)
	{
		CTreeItemUI *pNodeItem = GetRootNode();
		while (pNodeItem)
		{
			if(pNodeItem == pNode)
			{
				if(IsRootNode(pNode))
				{
					Remove(pNode);
					return TRUE;
				}

				CTreeItemUI *pParentNode = pNode->GetParentNode();
				if(pParentNode)
				{
					pParentNode->DeleteChildNode(pNode);
				}

				return TRUE;
			}
			pNodeItem = GetNextNode(pNodeItem);
		}
		return FALSE;
	}

	BOOL CTreeCtrlUI::IsRootNode(CTreeItemUI *pNode)
	{
		return pNode->GetParentNode() == NULL;
	}

	CTreeItemUI *CTreeCtrlUI::GetNextNode(CTreeItemUI *pNode)
	{
		if(!pNode)	return NULL;
		CTreeItemUI *pNodeNext = NULL;

		//有没有子项
		pNodeNext = GetFirstChildren(pNode);
		if(pNodeNext) return pNodeNext;

		//有没有兄弟项
		pNodeNext = GetNextSiblingNode(pNode);
		if(!pNodeNext)
		{
			//回滚寻找父项的兄弟项
			CTreeItemUI *pNodeTemp = pNode;
			while (TRUE)
			{
				CControlUI *pControl = pNodeTemp->GetParent();
				CTreeItemUI *pNodeParent = pNodeTemp->GetParentNode();

				if(!pControl) break;
				if(pControl == this) break;
				if(!pNodeParent) break;

				pNodeNext = GetNextSiblingNode(pNodeParent);
				if(pNodeNext) break;

				pNodeTemp = pNodeParent;
			}
		}
		return pNodeNext;
	}

	CTreeItemUI *CTreeCtrlUI::GetFirstChildren(CTreeItemUI *pNode)
	{
		if(pNode->GetChildNodeCount() <= 0) return NULL;
		return pNode->GetChildNodeAt(0);
	}

	CTreeItemUI *CTreeCtrlUI::GetNextSiblingNode(CTreeItemUI *pNode)
	{
		//有没有父项
		CTreeItemUI *pParentNode = pNode->GetParentNode();
		if(pParentNode)
		{
			int iIndex = pParentNode->GetChildNodeIndex(pNode);
			if(pParentNode->GetChildNodeCount() <= ++iIndex) return NULL;
			return pParentNode->GetChildNodeAt(iIndex);
		}

		//上一个节点是不是treeviewex
		if(pNode->GetParent() == this)
		{
			int iIndex = GetItemIndex(pNode);
			if(GetCount() <= ++iIndex) return NULL;
			return (CTreeItemUI *)GetItemAt(iIndex);
		}

		return NULL;
	}

	CTreeItemUI *CTreeCtrlUI::GetNextPrevNode(CTreeItemUI *pNode)
	{
		//有没有父项
		CTreeItemUI *pParentNode = pNode->GetParentNode();
		if(pParentNode)
		{
			int iIndex = pParentNode->GetChildNodeIndex(pNode);
			if(iIndex == 0) return NULL;
			return pParentNode->GetChildNodeAt(++iIndex);
		}

		//上一个节点是不是treeviewex
		if(pNode->GetParent() == this)
		{
			int iIndex = GetItemIndex(pNode);
			if(iIndex == 0) return NULL;
			return (CTreeItemUI *)GetItemAt(++iIndex);
		}

		return NULL;
	}

	CTreeItemUI *CTreeCtrlUI::FindNodeByTag(UINT_PTR pTag)
	{
		CTreeItemUI *pNode = GetRootNode();
		while (pNode)
		{
			if(pNode->GetTag() == pTag)
			{
				return pNode;
			}
			pNode = GetNextNode(pNode);
		}
		return NULL;
	}

	BOOL CTreeCtrlUI::MoveNode(CTreeItemUI *pNode, CTreeItemUI *pNewParent, CTreeItemUI *pAfterNode)
	{
		if(pNode == pNewParent) return FALSE;

		//先移出来，不能delete
		CTreeItemUI *pOldParentNode = pNode->GetParentNode();
		if(pOldParentNode)
		{
			pOldParentNode->m_pChildContainer->Remove(pNode, true);
		}
		else if(IsRootNode(pNode))
		{
			Remove(pNode, true);
		}

		if(!InsertNode(pNode, pNewParent, pAfterNode))
		{
			delete pNode;
			return FALSE;
		}
		return TRUE;
	}
}