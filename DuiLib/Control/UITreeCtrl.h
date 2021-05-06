#ifndef UITreeViewEx_h__
#define UITreeViewEx_h__
#pragma once

namespace DuiLib
{
	class UILIB_API CTreeCtrlUI : public CVerticalLayoutUI//, public ITreeNodeUI
	{
		DECLARE_DUICONTROL(CTreeCtrlUI)
	public:
		CTreeCtrlUI(void);
		~CTreeCtrlUI(void);

		friend class CTreeItemUI;
	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID	GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		CTreeItemUI *GetFocusNode() const;
		void SetFocusNode(CTreeItemUI *pNode);

		void SetStyleHoriz(LPCTSTR pstrValue)		{ m_sStyleHoriz = pstrValue;		}
		LPCTSTR GetStyleHoriz()						{ return m_sStyleHoriz;				}
		void SetStyleFolder(LPCTSTR pstrValue)		{ m_sStyleFolder = pstrValue;		}
		LPCTSTR GetStyleFolder()					{ return m_sStyleFolder;			}
		void SetStyleCheckBox(LPCTSTR pstrValue)	{ m_sStyleCheckBox = pstrValue;		}
		LPCTSTR GetStyleCheckBox()					{ return m_sStyleCheckBox;			}
		void SetStyleIcon(LPCTSTR pstrValue)		{ m_sStyleIcon = pstrValue;			}
		LPCTSTR GetStyleIcon()						{ return m_sStyleIcon;				}
		void SetStyleText(LPCTSTR pstrValue)		{ m_sStyleText = pstrValue;			}
		LPCTSTR GetStyleText()						{ return m_sStyleText;				}

		void SetIndent(int nWidth)					{ m_nIndent = nWidth;				}
		int  GetIndent()							{ return m_nIndent;					}

		void SetItemBkColor(DWORD dwColor)			{ m_dwItemBkColor = dwColor;		}
		DWORD GetItemBkColor()						{ return m_dwItemBkColor;			}	
			
		void SetItemSelectedBkColor(DWORD dwColor)	{ m_dwItemSelectedBkColor = dwColor; }
		DWORD GetItemSelectedBkColor()				{ return m_dwItemSelectedBkColor;	}

		void SetItemHotBkColor(DWORD dwColor)		{ m_dwItemHotBkColor = dwColor;		}
		DWORD GetItemHotBkColor()					{ return m_dwItemHotBkColor;		}

		void SetItemFocusBkColor(DWORD dwColor)		{ m_dwItemFocusBkColor = dwColor;	}
		DWORD GetItemFocusBkColor()					{ return m_dwItemFocusBkColor;		}

		void SetItemLineColor(DWORD dwColor)		{ m_dwItemLineColor = dwColor;		}
		DWORD GetItemLineColor()					{ return m_dwItemLineColor;			}

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);	

		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);
		void RemoveAll();

		virtual void DoInit();
		virtual SIZE EstimateSize(SIZE szAvailable);

		//获取第一个根节点
		CTreeItemUI *GetRootNode();

		//插入节点，
		//pParentNode=NULL时， 插入到root下，若root不存在，当前插入为root
		//pAfterNode=NULL时，插入到pParentNode最后一个子项后面
		CTreeItemUI *InsertNode(LPCTSTR lpszItem, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode);
		BOOL InsertNode(CTreeItemUI *pNewNode, CTreeItemUI *pParentNode, CTreeItemUI *pAfterNode);

		//删除节点，连子项一起删除的。
		BOOL DeleteNode(CTreeItemUI *pNode);

		//判断是否根节点 ( 根节点可能不止一个 )
		BOOL IsRootNode(CTreeItemUI *pNode);

		//获得pNode的下一项，用于纵深方向遍历树。
		CTreeItemUI *GetNextNode(CTreeItemUI *pNode);

		//获得pNode第一个子项
		CTreeItemUI *GetFirstChildren(CTreeItemUI *pNode);

		//获得pNode的下一个兄弟
		CTreeItemUI *GetNextSiblingNode(CTreeItemUI *pNode);

		//获得pNode的上一个兄弟
		CTreeItemUI *GetNextPrevNode(CTreeItemUI *pNode);

		//根据Tag标记查找Node
		CTreeItemUI *FindNodeByTag(UINT_PTR pTag);

		//移动节点, pNewParent=NULL时，移动到根节点
		BOOL MoveNode(CTreeItemUI *pNode, CTreeItemUI *pNewParent, CTreeItemUI *pAfterNode);
	private:
		CTreeItemUI *m_pFocusNode;

		CDuiString m_sStyleHoriz;
		CDuiString m_sStyleFolder;
		CDuiString m_sStyleCheckBox;
		CDuiString m_sStyleIcon;
		CDuiString m_sStyleText;

		int m_nIndent;

		DWORD m_dwItemBkColor;
		DWORD m_dwItemSelectedBkColor;
		DWORD m_dwItemHotBkColor;
		DWORD m_dwItemFocusBkColor;
		DWORD m_dwItemLineColor;
	};
}


#endif // UITreeViewEx_h__
