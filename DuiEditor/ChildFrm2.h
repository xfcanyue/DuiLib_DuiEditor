
// ChildFrm2.h : CChildFrame2 类的接口
//

#pragma once

class CChildFrame2 : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame2)
public:
	CChildFrame2();

// 特性
public:
	//CMFCToolBar       m_wndToolBar;
// 操作
public:

// 重写
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildFrame2();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
