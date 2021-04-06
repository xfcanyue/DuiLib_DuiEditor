#pragma once
// COutputWnd

class CDockOutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockOutputWnd)
public: 
	CDockOutputWnd();
	virtual ~CDockOutputWnd();

	CRichEditCtrl m_ctrlRichEdit;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnInsertMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnListmsgCopy();
	afx_msg void OnUpdateListmsgCopy(CCmdUI *pCmdUI);
	afx_msg void OnListmsgClear();
	afx_msg void OnUpdateListmsgClear(CCmdUI *pCmdUI);
	afx_msg void OnListmsgSaveAs();
	afx_msg void OnUpdateListmsgSaveAs(CCmdUI *pCmdUI);
};

