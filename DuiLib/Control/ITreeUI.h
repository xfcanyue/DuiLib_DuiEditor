#pragma once


namespace DuiLib
{

class TNodeData
{
	friend class ITreeUI;
public:
	TNodeData()
	{
		m_bSelected = false;
		m_bCheckBoxCheck = false;
		m_bExpand = true;
		m_tag = NULL;
		m_pParent = NULL;
		m_childs.SetSaveIndexMap(true);
	}

	CDuiString GetText() const { return m_sText; }
	int GetTextN() const { return _ttoi(m_sText); }

	void SetText(LPCTSTR pstrText) { m_sText = pstrText; }
	void SetTextN(int n) { m_sText.Format(_T("%d"), n); }
	void SetTextV(LPCTSTR lpszFormat, ...)
	{
		va_list argList;
		va_start(argList, lpszFormat);
		CDuiString s;
		s.InnerFormat(lpszFormat, argList);
		SetText(s);
		va_end(argList);
	}

	void SetImageIcon(LPCTSTR lpszImageIcon) { m_sImageIcon = lpszImageIcon; }
	CDuiString GetImageIcon() const { return m_sImageIcon; }

protected:
	void Select(bool bSelected=true) { m_bSelected = bSelected; }
public:
	bool IsSelected() const { return m_bSelected; }

	void SetCheckBoxCheck(bool bCheckBoxCheck=true) { m_bCheckBoxCheck = bCheckBoxCheck; }
	bool IsCheckBoxCheck() const { return m_bCheckBoxCheck; }

	void Expand(bool bExpand=true) { m_bExpand = bExpand; }
	bool IsExpand() const { return m_bExpand; }

protected:
	void SetTag(UINT_PTR tag) { m_tag = tag; }
public:
	UINT_PTR GetTag() const { return m_tag; }

	int GetLevel()
	{
		int nLevel = 0;
		TNodeData *pTempNode = this;
		while (pTempNode->m_pParent)
		{
			nLevel++;
			pTempNode = pTempNode->m_pParent;
		}
		return nLevel;
	}

	TNodeData *GetParent() const { return m_pParent; }
	BOOL NodeHasChildren() const { return m_childs.GetSize() > 0; }
	TNodeData *GetChild(int n) const { return (TNodeData *)m_childs.GetAt(n); }

	bool m_bSelected;
	bool m_bCheckBoxCheck;
	bool m_bExpand;
	UINT_PTR m_tag;
	CDuiString m_sText;
	CDuiString m_sImageIcon;

	TNodeData *m_pParent;
	CStdPtrArray m_childs;
};

#define TNODE_ROOT                ((TNodeData *)(ULONG_PTR)-0x10000)
#define TNODE_FIRST               ((TNodeData *)(ULONG_PTR)-0x0FFFF)
#define TNODE_LAST                ((TNodeData *)(ULONG_PTR)-0x0FFFE)
#define TNODE_SORT                ((TNodeData *)(ULONG_PTR)-0x0FFFD)

class UILIB_API ITreeUI
{
public:
	ITreeUI(void);
	virtual ~ITreeUI(void);

	void SetIndentWidth(int nWidth)					{ m_nIndentWidth = nWidth;				}
	int  GetIndentWidth()							{ return m_nIndentWidth;				}

	void SetViewLine(BOOL bView)					{ m_bViewLine = bView;					}
	BOOL IsViewLine() const							{ return m_bViewLine;					}

	void SetNodeHeight(int nHeight)					{ m_nNodeHeight = nHeight;				}
	int  GetNodeHeight() const						{ return m_nNodeHeight;					}

	void SetMultiSelect(BOOL bMultiSel)				{ m_bMultiSelect = bMultiSel;			}
	BOOL IsMultiSelect() const						{ return m_bMultiSelect;				}

	void SetStyleNode(LPCTSTR pstrValue)			{ m_sStyleNode = pstrValue;			}
	LPCTSTR GetStyleNode()							{ return m_sStyleNode;				}
	void SetStyleFolderBtn(LPCTSTR pstrValue)		{ m_sStyleFolder = pstrValue;		}
	LPCTSTR GetStyleFolderBtn()						{ return m_sStyleFolder;			}
	void SetStyleCheckBoxBtn(LPCTSTR pstrValue)		{ m_sStyleCheckBox = pstrValue;		}
	LPCTSTR GetStyleCheckBoxBtn()					{ return m_sStyleCheckBox;			}
	void SetStyleIconBtn(LPCTSTR pstrValue)			{ m_sStyleIcon = pstrValue;			}
	LPCTSTR GetStyleIconBtn()						{ return m_sStyleIcon;				}
	void SetStyleTextBtn(LPCTSTR pstrValue)			{ m_sStyleText = pstrValue;			}
	LPCTSTR GetStyleTextBtn()						{ return m_sStyleText;				}

public:
	virtual void Refresh(bool bNeedUpdate=false)		= 0;
	virtual void SendNotify(LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false) = 0;

	TNodeData *GetFocusNode() const { return m_pFocusNode; }
	void SetFocusNode(TNodeData *pNode) { m_pFocusNode = pNode; }

	void SetNodeTag(TNodeData *pNode, UINT_PTR tag);
	TNodeData *FindNodeByTag(UINT tag);

	//插入节点
	TNodeData *InsertNode(LPCTSTR lpszItem, LPCTSTR lpszIconImage, TNodeData *hParent = TNODE_ROOT, TNodeData *hInsertAfter = TNODE_LAST);
	TNodeData *InsertNode(LPCTSTR lpszItem, TNodeData *hParent = TNODE_ROOT, TNodeData *hInsertAfter = TNODE_LAST);

	//删除节点，连子项一起删除的。
	void DeleteNode(TNodeData *pNode);

	//删除所有节点
	void DeleteAllNode();

	//获取第一个根节点
	TNodeData *GetRootNode();

	//判断是否根节点 ( 根节点可能不止一个 )
	BOOL IsRootNode(TNodeData *pNode);

	//获得pNode的下一项，用于纵深方向遍历树。
	TNodeData *GetNextNode(TNodeData *pNode, bool bGetVisibleNode=false);

	//获得pNode第一个子项
	TNodeData *GetFirstChildren(TNodeData *pNode);

	//获得pNode的下一个兄弟
	TNodeData *GetNextSiblingNode(TNodeData *pNode);

	//获得pNode的上一个兄弟
	TNodeData *GetNextPrevNode(TNodeData *pNode);

	void ClearSeletedNodes();
	void SelectNode(TNodeData *pNode, bool bSeleted=true);
	bool IsSelectedNode(TNodeData *pNode);

	BOOL MoveNode(TNodeData *pNode, TNodeData *pNewParent, TNodeData *phInsertAfter = TNODE_LAST);
protected:
	TNodeData *AllocNodeData();
	void FreeNodeData(TNodeData *pTreeData);
	void _DeleteAllItem(TNodeData *pNodeData);
protected:
	int m_nIndentWidth;
	BOOL m_bViewLine;
	int  m_nNodeHeight;
	BOOL m_bMultiSelect;
	CDuiString m_sStyleNode;
	CDuiString m_sStyleFolder;
	CDuiString m_sStyleCheckBox;
	CDuiString m_sStyleIcon;
	CDuiString m_sStyleText;

protected:
	CStdPtrArray m_NodeData;
	std::map<TNodeData*, TNodeData *> m_mapSelectedNodes;
	TNodeData *m_pFocusNode;
	std::map<UINT_PTR, TNodeData *> m_mapTag;
};



}