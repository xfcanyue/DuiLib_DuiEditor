
// ChildFrm2.cpp : CChildFrame2 类的实现
//

#include "stdafx.h"
#include "DuiEditor.h"

#include "ChildFrm2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame2

IMPLEMENT_DYNCREATE(CChildFrame2, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame2, CMDIChildWndEx)
	ON_WM_MDIACTIVATE()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame2 构造/析构

CChildFrame2::CChildFrame2()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame2::~CChildFrame2()
{
}


BOOL CChildFrame2::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame2 消息处理程序


void CChildFrame2::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

	// TODO: 在此处添加消息处理程序代码
}


int CChildFrame2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(IDR_TB_UIDESIGNER_VIEW))
// 	{
// 		TRACE0("未能创建工具栏\n");
// 		return -1;      // 未能创建
// 	}
// 	m_wndToolBar.EnableTextLabels(TRUE);
// 
//  	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_VIEW), _T("设计"));
//  	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_CODE), _T("代码"));

// 	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_TB_UIDESIGNER_VIEW);
// 	m_wndToolBar.LoadToolBar(IDR_TB_UIDESIGNER_VIEW, 0, 0, TRUE /* 已锁定*/);
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
// 	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
// 
// 	m_wndToolBar.SetOwner(this);
// 	// 所有命令将通过此控件路由，而不是通过主框架路由:
// 	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
// 
//   	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_VIEW), _T("设计"));
// 	m_wndToolBar.SetToolBarBtnText (m_wndToolBar.CommandToIndex (ID_DESIGNER_CODE), _T("代码"));

	return 0;
}


void CChildFrame2::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
// 	if(!m_wndToolBar.GetSafeHwnd())
// 		return;

// 	CRect rectClient;
// 	GetClientRect(rectClient);
// 	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
// 
// 	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
// 
// 	CView *pView = GetActiveView();
// 	if(pView)
// 	{
// 		pView->SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOZORDER | SWP_NOACTIVATE);
// 	}
}
