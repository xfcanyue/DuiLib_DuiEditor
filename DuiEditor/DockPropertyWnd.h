#pragma once
#include "UIPropertyGridCtrl.h"

// CPropertyWnd

class CDockPropertyWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CDockPropertyWnd)
public:
	class CPropertiesToolBar : public CMFCToolBar
	{
	public:
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
		}

		virtual BOOL AllowShowOnList() const { return FALSE; }
	};
public:
	CDockPropertyWnd();
	virtual ~CDockPropertyWnd();

	CFont m_fntPropList;
	CPropertiesToolBar m_wndToolBar;
	CEdit m_wndFindText;
// 	CButton m_wndFind;
// 	CButton m_wndFindEnd;
	CUIPropertyGridCtrl m_wndPropList;

	void SetPropListFont();
	void InitProp(xml_node TreeNode, LPCTSTR strFilter=NULL);
	LRESULT OnPropertyChanged (WPARAM,LPARAM lParam);

	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPropertiesFindText();
	afx_msg void OnPropertiesFindButton();
	afx_msg void OnUpdatePropertiesFindButton(CCmdUI *pCmdUI);
	afx_msg void OnPropertiesFindButton2();
	afx_msg void OnUpdatePropertiesFindButton2(CCmdUI *pCmdUI);
	afx_msg void OnChangeEdit1();
};

extern CDockPropertyWnd *g_pPropWnd;

