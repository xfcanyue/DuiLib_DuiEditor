#pragma once
#include "UIWindow.h"
#include "UIManager.h"
#include <list>
// CUIWindowEx

class CDuiEditorViewDesign;
class CUIWindowEx : public CUIWindow
{
	DECLARE_DYNAMIC(CUIWindowEx)

public:
	CUIWindowEx();
	virtual ~CUIWindowEx();

public:
	CUIManager *m_pManager;
	CUITrackerMuliti m_tracker;

public:
	BOOL SelectItem(xml_node node);
	BOOL SelectItem(CControlUI *pControl);

	void AddNewControlFromToolBox(xml_node nodeToolBox, CPoint pt);
	std::list<CControlUI *>	m_listTrackerSeleted;
	void OnSelectingControl(CControlUI *pControl, const CRect &rcTracker);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


