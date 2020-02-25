// DefaultEditorFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DefaultEditorFrame.h"

#include "MainFrm.h"
#include "DefaultEditor.h"
// CDefaultEditorFrame

IMPLEMENT_DYNCREATE(CDefaultEditorFrame, CFrameWndEx)

CDefaultEditorFrame::CDefaultEditorFrame()
{

}

CDefaultEditorFrame::~CDefaultEditorFrame()
{
}


BEGIN_MESSAGE_MAP(CDefaultEditorFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_TB_EXIT, &CDefaultEditorFrame::OnTbExit)
	ON_UPDATE_COMMAND_UI(ID_TB_EXIT, &CDefaultEditorFrame::OnUpdateTbExit)
	ON_COMMAND(ID_TB_SAVE, &CDefaultEditorFrame::OnTbSave)
	ON_UPDATE_COMMAND_UI(ID_TB_SAVE, &CDefaultEditorFrame::OnUpdateTbSave)
	ON_COMMAND(ID_TB_UI_EFFECT, &CDefaultEditorFrame::OnTbUiEffect)
	ON_UPDATE_COMMAND_UI(ID_TB_UI_EFFECT, &CDefaultEditorFrame::OnUpdateTbUiEffect)
END_MESSAGE_MAP()


// CDefaultEditorFrame 消息处理程序


int CDefaultEditorFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDefaultEditor *pDlgMain = (CDefaultEditor *)GetParent();

	// TODO:  在此添加您专用的创建代码
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar.LoadToolBar(IDR_TB_DEFAULT_EDITOR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	m_wndToolBar.EnableTextLabels(TRUE);

	if (!m_wndProperty.Create(_T("属性"), this, 
		CRect(0, 0, 350, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT, 
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // 未能创建
	}
	m_wndProperty.m_wndPropList.SetOwner(this);

	EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperty.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndProperty);

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	m_wndView.CreateControl(pDlgMain->m_strClassName);

	m_wndProperty.InitProp(pDlgMain->m_nodeControl.child(pDlgMain->m_strClassName));
	return 0;
}

BOOL CDefaultEditorFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CFrameWndEx::OnEraseBkgnd(pDC);
}

LRESULT CDefaultEditorFrame::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	m_wndView.SetControlAttribute();
	return 0;
}

void CDefaultEditorFrame::OnTbExit()
{
	CDefaultEditor *pDlgMain = (CDefaultEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDCANCEL, 0);
}


void CDefaultEditorFrame::OnUpdateTbExit(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDefaultEditorFrame::OnTbSave()
{
	CDefaultEditor *pDlgMain = (CDefaultEditor *)GetParent();
	pDlgMain->SendMessage(WM_COMMAND, IDOK, 0);
}


void CDefaultEditorFrame::OnUpdateTbSave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CDefaultEditorFrame::OnTbUiEffect()
{
	m_wndView.m_bUiEffect = !m_wndView.m_bUiEffect;

	if(!m_wndView.m_bUiEffect)
	{
		m_wndView.m_tracker.m_rect.SetRectEmpty();
		m_wndView.GetManager()->Invalidate();
	}
}


void CDefaultEditorFrame::OnUpdateTbUiEffect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndView.m_bUiEffect);
}
