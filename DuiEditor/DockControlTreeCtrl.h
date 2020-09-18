#pragma once

// CViewControlTree

#define TVI_BEFORE              ((HTREEITEM)(ULONG_PTR)-0x0FFFC)
#define TVI_NEXT                ((HTREEITEM)(ULONG_PTR)-0x0FFFB)
#define TVI_CHILD                ((HTREEITEM)(ULONG_PTR)-0x0FFFA)

class CUIManager;
class CDockControlTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDockControlTreeCtrl)

	struct xml_string_writer : pugi::xml_writer
	{
		virtual void write(const void* data, size_t size)
		{
			m_str += CStringA((const char *)data, size);
		}
		CStringA m_str;
	};

public:
	CDockControlTreeCtrl();
	virtual ~CDockControlTreeCtrl();

	void InitTreeContent();
	void OpenXmlDocument(pugi::xml_node elem, HTREEITEM hTreeParent);

	HTREEITEM AddNewControl(xml_node node, HTREEITEM hParent, HTREEITEM hInertAfter);
	HTREEITEM AddNewControl(xml_node node, HTREEITEM hInertAfter=TVI_LAST);
	HTREEITEM AddNewControl(xml_node nodeNewContrl, xml_node nodeCurrentControl, HTREEITEM htiAfter);

	void UpdateXmlNode(xml_node node);

	HTREEITEM FindXmlNode(xml_node node);

	HTREEITEM SelectXmlNode(xml_node node);
	xml_node GetSelectXmlNode();

	BOOL DeleteXmlNode(xml_node node);

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

protected:
	BOOL OnDragXmlNode(HTREEITEM src, HTREEITEM dest);

	//获取下一项
	HTREEITEM GetNextItemEx(HTREEITEM hItem);

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM m_hItemDragSrc;
	HTREEITEM m_hItemDragDest;
	BOOL m_bIsDragging;
	BOOL m_bValidDrop;
	CImageList *m_pDragImage;
	DWORD m_dwDragStart; //防止误拖拽的延时

	int m_nMaxDeep;
	int m_nCurrentDeep;	//树的深度

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginrdrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	BOOL OnDraging();
	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
};



