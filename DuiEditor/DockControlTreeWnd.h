#pragma once
#include "DockControlTreeCtrl.h"
// CViewControlWnd

class CDockControlTreeWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockControlTreeWnd)

	class CClassTabCtrl : public CMFCTabCtrl
	{
		virtual int GetTabsHeight() const { return 0; }
	};
public:
	CDockControlTreeWnd();
	virtual ~CDockControlTreeWnd();

	CDockControlTreeCtrl *CreateTreeView();
	void RemoveTreeView(CDockControlTreeCtrl *pTreeView);
	void SetActiveTreeView(CDockControlTreeCtrl *pTreeView);

	//CDockControlTreeCtrl m_wndTree;
	CClassTabCtrl m_tabClass;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


