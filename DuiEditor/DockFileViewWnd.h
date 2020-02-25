#pragma once
#include "DockFileViewCtrl.h"

// CDockFileViewWnd

class CDockFileViewWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockFileViewWnd)

public:
	CDockFileViewWnd();
	virtual ~CDockFileViewWnd();

	CDockFileViewCtrl m_wndFileView;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


