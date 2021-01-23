#pragma once

// CViewScriptStack

class CDockScriptStackWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockScriptStackWnd)

	class CClassTabCtrl : public CMFCTabCtrl
	{
		virtual int GetTabsHeight() const { return 0; }
	};
public:
	CDockScriptStackWnd();
	virtual ~CDockScriptStackWnd();
	virtual BOOL CanBeClosed() const { return FALSE; }

	CMFCPropertyGridCtrl *CreateStackView();
	void RemoveStackView(CMFCPropertyGridCtrl *pView);
	void SetActiveStackView(CMFCPropertyGridCtrl *pView);

	CClassTabCtrl m_tabClass;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


