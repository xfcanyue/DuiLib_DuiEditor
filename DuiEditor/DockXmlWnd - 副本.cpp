// DockXmlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockXmlWnd.h"


// CDockXmlWnd

IMPLEMENT_DYNAMIC(CDockXmlWnd, CDockablePane)

CDockXmlWnd::CDockXmlWnd()
{

}

CDockXmlWnd::~CDockXmlWnd()
{
}


BEGIN_MESSAGE_MAP(CDockXmlWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CDockXmlWnd 消息处理程序




int CDockXmlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_tabClass.Create (CMFCTabCtrl::STYLE_3D_ONENOTE,
		CRect(0,0,0,0), this, ID_VIEW_CONTROL_TAB, CMFCTabCtrl::LOCATION_TOP); 

	return 0;
}


void CDockXmlWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);

	m_tabClass.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

// 	if(!sci.GetSafeHwnd())	return;
// 
// 	CRect rcClient;
// 	rcClient.DeflateRect(1,1,1,1);
// 	GetClientRect(rcClient);
// 	sci.MoveWindow(rcClient);
}


void CDockXmlWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_tabClass.SetFocus();
}


void CDockXmlWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

CDockXmlView *CDockXmlWnd::CreateSciWnd()
{
	CRect rectDummy;
	rectDummy.SetRect(0,0,200,200);

	CDockXmlView *pView = new CDockXmlView;
	pView->Create(afxGlobalData.RegisterWindowClass(_T("Afx:DockSciWndParent")), _T(""), WS_CHILD | WS_VISIBLE, rectDummy, &m_tabClass, ID_SCI_WND);

	m_tabClass.AddTab(pView, _T(""));
	return pView;
}

void CDockXmlWnd::RemoveSciWnd(CDockXmlView *pSciWnd)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pSciWnd)
		{
			m_tabClass.RemoveTab(i);
			delete pSciWnd;
			break;
		}
	}
}

void CDockXmlWnd::SetActiveSciWnd(CDockXmlView *pSciWnd)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pSciWnd)
		{
			m_tabClass.SetActiveTab(i);
			break;
		}
	}
}