
// ChildFrm.h : CChildFrame 类的接口
//

#pragma once
#include "MySplitterWnd.h"

class CUIManager;
class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 特性
public:
	CRect m_rcView;
	CMySplitterWnd m_split;

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

// 重写
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildFrame();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDesignerView();
	afx_msg void OnUpdateDesignerView(CCmdUI *pCmdUI);
	afx_msg void OnDesignerCode();
	afx_msg void OnUpdateDesignerCode(CCmdUI *pCmdUI);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnDesignerSplitUpdown();
	afx_msg void OnUpdateDesignerSplitUpdown(CCmdUI *pCmdUI);
	afx_msg void OnDesignerSplitLeftright();
	afx_msg void OnUpdateDesignerSplitLeftright(CCmdUI *pCmdUI);
};
