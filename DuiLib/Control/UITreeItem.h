#ifndef UITreeNodeEx_h__
#define UITreeNodeEx_h__

#pragma once

namespace DuiLib
{
	#define PANE_ANIMATION_TREENODE_ID	3
	class UILIB_API CTreeItemUI : public CVerticalLayoutUI
	{
		friend class CTreeCtrlUI;
		DECLARE_DUICONTROL(CTreeItemUI)
	public:
		CTreeItemUI();
		~CTreeItemUI(void);

	public:
		void SetOwner(CTreeCtrlUI *pTreeView) { m_pOwner = pTreeView; }
		CTreeCtrlUI *GetOwner() const { return m_pOwner; }

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		void SetParentNode(CTreeItemUI *pParentNode)	{ m_pParentNode = pParentNode;	}
		CTreeItemUI *GetParentNode()					{ return m_pParentNode;			}

		int GetChildNodeCount();
		CTreeItemUI *GetChildNodeAt(int iIndex);
		int GetChildNodeIndex(CTreeItemUI *pNode);
		BOOL DeleteChildNode(CTreeItemUI *pNode);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);
		virtual bool RemoveAt(int iIndex, bool bDoNotDestroy=false);
		virtual void RemoveAll();

		virtual CDuiString GetText() const;
		virtual void SetText(LPCTSTR pstrText);

		void ExpandItem(bool bExpand);
		bool IsExpandItem();
		bool Select(bool bSelect = true);
		bool IsSelected() const;

		virtual void DoInit();
		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual void DoEvent(TEventUI& event);
		virtual bool OnEventInnerControl(void* param);
		virtual bool OnPaintHoriz(void * param);

		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID);
		virtual void OnAnimationStop(int nAnimationID);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	private:
		CTreeCtrlUI		*m_pOwner;
		CTreeItemUI		*m_pParentNode;		//父节点
		int					 m_nLevel;

		CHorizontalLayoutUI *m_pHoriz;			//节点容器
		CCheckBoxUI			*m_pFolderButton;	//展开收缩按钮
		CCheckBoxUI			*m_pCheckBox;		//选中按钮
		COptionUI			*m_pIcon;			//图标
		COptionUI			*m_pText;			//文本框

		CVerticalLayoutUI	*m_pChildContainer;

		bool m_bSelected;
	};
}


#endif // UITreeNodeEx_h__
