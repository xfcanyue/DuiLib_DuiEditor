#pragma once

#include <stack>
#include <map>
class CImageEditorListCtrl : public CMFCShellListCtrl
{
public:
	CImageEditorListCtrl();
	~CImageEditorListCtrl();

	virtual void OnSetColumns();
	virtual void DoDefault(int iItem);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual HRESULT DisplayFolder(LPCTSTR lpszPath);
	virtual HRESULT DisplayFolder(LPAFX_SHELLITEMINFO lpItemInfo);

	DECLARE_MESSAGE_MAP()
public:
	BOOL InitList();
	CxImage *GetThumbnail(int nItem);
	void ClearThumbnail();  
private:
	CImageList m_imageEmpty;
	CImageList m_imageShell;
	std::map<int, CxImage *> m_map;
};
/*
class CImageEditorListToolBarMenuButton : public CMFCToolBarMenuButton
{
	DECLARE_SERIAL(CImageEditorListToolBarMenuButton)
public:
	CImageEditorListToolBarMenuButton(HMENU hMenu = NULL);
	CImageEditorListToolBarMenuButton(UINT uiID, HMENU hMenu, int iImage, LPCTSTR lpszText = NULL, BOOL bUserButton = FALSE);

	virtual BOOL OpenPopupMenu(CWnd* pWnd = NULL);
};
*/
class CImageEditorPaneFile : public CDockablePane
{
	DECLARE_DYNAMIC(CImageEditorPaneFile)
public:
	class CFileViewToolBar : public CMFCToolBar
	{
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
		}
	};

public:
	CImageEditorPaneFile();
	virtual ~CImageEditorPaneFile();

	CImageEditorListCtrl	m_wndList;
	CImageList m_FileViewImages;

	CFileViewToolBar m_wndToolBar;

	CStringArray	m_back;
	CStringArray	m_forward;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnExplorerUp();
	afx_msg void OnExplorerBack();
	afx_msg void OnUpdateExplorerBack(CCmdUI *pCmdUI);
	afx_msg void OnExplorerForward();
	afx_msg void OnUpdateExplorerForward(CCmdUI *pCmdUI);
	afx_msg void OnUpdateExplorerUp(CCmdUI *pCmdUI);
	afx_msg void OnExplorerViewsLargeIcon();
	afx_msg void OnUpdateExplorerViewsLargeIcon(CCmdUI *pCmdUI);
	afx_msg void OnExplorerViewsSmallIcon();
	afx_msg void OnUpdateExplorerViewsSmallIcon(CCmdUI *pCmdUI);
	afx_msg void OnExplorerViewsDetails();
	afx_msg void OnUpdateExplorerViewsDetails(CCmdUI *pCmdUI);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnExplorerViewsList();
	afx_msg void OnUpdateExplorerViewsList(CCmdUI *pCmdUI);
};