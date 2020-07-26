#pragma once

#include "DockXmlView.h"

// CDockXmlWnd

class CDockXmlWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockXmlWnd)
public:
	class CClassTabCtrl : public CMFCTabCtrl
	{
		virtual int GetTabsHeight() const { return 0; }
	};
public:
	CDockXmlWnd();
	virtual ~CDockXmlWnd();

	CDockXmlView *CreateSciWnd();
	void RemoveSciWnd(CDockXmlView *pTreeView);
	void SetActiveSciWnd(CDockXmlView *pTreeView);

	CClassTabCtrl m_tabClass;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


