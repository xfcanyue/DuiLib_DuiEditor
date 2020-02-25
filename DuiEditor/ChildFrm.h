
// ChildFrm.h : CChildFrame 类的接口
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 特性
public:
	CMFCToolBar			m_wndToolBar;
	CMFCStatusBar	m_wndStatusBar;
	CRect m_rcView;

	int m_nCurrentView;
// 操作
public:

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
};
