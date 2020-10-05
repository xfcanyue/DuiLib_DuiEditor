#pragma once
#include "SciWnd.h"

// CDockXmlView

class CDockXmlView : public CWnd
{
	DECLARE_DYNAMIC(CDockXmlView)

public:
	CDockXmlView();
	virtual ~CDockXmlView();

	void Init();
	void SelectControlNode(CControlUI *pControl);
	void SelectControlNode(xml_node node);

	CUIManager *m_pManager;
	CDocument *m_pDoc;
	CSciWnd sci;

protected:
	CString GetNodeName();
	void AutoCompleteNode(CString objectName);		//自动完成控件名
	CString AutoCompleteProperty(CString objectName, CString AttrName);	//自动完成属性名
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSciUpdateDesign();
	afx_msg void OnUpdateSciUpdateDesign(CCmdUI *pCmdUI);
};


