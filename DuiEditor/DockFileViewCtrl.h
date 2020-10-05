#pragma once


// CDockFileViewCtrl

class CDockFileViewCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDockFileViewCtrl)

public:
	CDockFileViewCtrl();
	virtual ~CDockFileViewCtrl();

	BOOL InitFolder(LPCTSTR szFolderPath);
	CString GetItemPath(HTREEITEM htreeItem);
protected:
	BOOL GetChildItems(HTREEITEM hParentItem);
	virtual HRESULT EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
	static int CALLBACK CompareProc(LPARAM, LPARAM, LPARAM);

	virtual CString OnGetItemText(LPAFX_SHELLITEMINFO pItem);
	virtual int OnGetItemIcon(LPAFX_SHELLITEMINFO pItem, BOOL bSelected);
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
private:
	CImageList m_ImageList;
	int m_nIconRoot;

	LPSHELLFOLDER m_psfRootFolder; 

	HTREEITEM m_hStartupItem;
	CString m_strCurrentPath;
protected:
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnEditDebugFile();
	afx_msg void OnUpdateEditDebugFile(CCmdUI *pCmdUI);
	afx_msg void OnEditRefresh();
	afx_msg void OnEditDebugRun();
	afx_msg void OnUpdateEditDebugRun(CCmdUI *pCmdUI);
	afx_msg void OnEditCopyFileName();
	afx_msg void OnEditDebugNoFile();
	afx_msg void OnUpdateEditDebugNoFile(CCmdUI *pCmdUI);
};


