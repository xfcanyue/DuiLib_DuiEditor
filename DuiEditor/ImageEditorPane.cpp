// ImageEditorPane.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ImageEditorPane.h"


// CImageEditorPane

IMPLEMENT_DYNAMIC(CImageEditorPaneAdjust, CDockablePane)

CImageEditorPaneAdjust::CImageEditorPaneAdjust()
{
	m_pForm = NULL;
}

CImageEditorPaneAdjust::~CImageEditorPaneAdjust()
{
}


BEGIN_MESSAGE_MAP(CImageEditorPaneAdjust, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CImageEditorPane 消息处理程序




int CImageEditorPaneAdjust::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pForm = new CImageEditorAdjust;
	m_pForm->Create(IDD_FORM_IMAGE_PANE, this);
	m_pForm->ShowWindow(SW_SHOW);
	//SendMessage(WM_SIZE);
	return 0;
}


void CImageEditorPaneAdjust::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_pForm && m_pForm->GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_pForm->MoveWindow(rc);
	}
}


void CImageEditorPaneAdjust::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
