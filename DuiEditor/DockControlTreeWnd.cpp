// ViewControlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockControlTreeWnd.h"

#include "MainFrm.h"
// CViewControlWnd

IMPLEMENT_DYNAMIC(CDockControlTreeWnd, CDockablePane)

CDockControlTreeWnd::CDockControlTreeWnd()
{

}

CDockControlTreeWnd::~CDockControlTreeWnd()
{
}


BEGIN_MESSAGE_MAP(CDockControlTreeWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CViewControlWnd 消息处理程序
int CDockControlTreeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	m_tabClass.Create (CMFCTabCtrl::STYLE_3D_ONENOTE,
		CRect(0,0,0,0), this, ID_VIEW_CONTROL_TAB, CMFCTabCtrl::LOCATION_TOP); 

// 	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
// 	if (!m_wndTree.Create(dwViewStyle, rectDummy, &m_tabClass, 4))
// 	{
// 		return -1;      // 未能创建
// 	}
// 	m_wndTree.EnableToolTips(TRUE);
// 	g_pTree = &m_wndTree;

//	m_tabClass.AddTab(&m_wndTree, _T("Asd"));
	return 0;
}

void CDockControlTreeWnd::OnSize(UINT nType, int cx, int cy)
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
}


void CDockControlTreeWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_tabClass.SetFocus();
}

CDockControlTreeCtrl *CDockControlTreeWnd::CreateTreeView()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	CDockControlTreeCtrl *pTreeView = new CDockControlTreeCtrl;
	if (!pTreeView->Create(dwViewStyle, rectDummy, &m_tabClass, 4))
	{
		return NULL;      // 未能创建
	}
	pTreeView->EnableToolTips(TRUE);
	pTreeView->SetImageList(&g_duiProp.m_ImageList, TVSIL_NORMAL);

	m_tabClass.AddTab(pTreeView, _T(""));
	
	SetActiveTreeView(pTreeView);
	return pTreeView;
}

void CDockControlTreeWnd::RemoveTreeView(CDockControlTreeCtrl *pTreeView)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pTreeView)
		{
			m_tabClass.RemoveTab(i);
			delete pTreeView;
			break;
		}
	}
}

void CDockControlTreeWnd::SetActiveTreeView(CDockControlTreeCtrl *pTreeView)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pTreeView)
		{
			m_tabClass.SetActiveTab(i);
			break;
		}
	}
}

void CDockControlTreeWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
/*
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_EDIT);

	CMenu menutab;
	menutab.LoadMenu(IDR_MENU_TABLAYOUT_SETSEL);

	//TabLayout列出切换菜单项
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if(hItem)
	{
		xml_node node((xml_node_struct *)m_wndTree.GetItemData(hItem));
		if(node)
		{
			if(g_duiProp.IsTabLayoutNode(node))
			{
				if(node.first_child())
				{
					::DeleteMenu(menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, 0, MF_BYPOSITION);

					UINT nCmd = ID_TABLAYOUT_SETSEL_00;
					for (xml_node nd = node.first_child(); nd; nd=nd.next_sibling())
					{
						CString temp;
						temp.Format(_T("页面%d %s name=%s"), nCmd-ID_TABLAYOUT_SETSEL_00+1, nd.name(), nd.attribute(_T("name")).value());
						::InsertMenu(menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, -1, MF_BYPOSITION, nCmd, temp);
						nCmd++;
					}

					::InsertMenu(menu.GetSubMenu(0)->m_hMenu, -1, MF_BYPOSITION | MF_POPUP, 
						(UINT_PTR)menutab.GetSubMenu(0)->GetSubMenu(0)->m_hMenu, _T("切换TabLayout页面"));
				}
			}
		}
	}
	theApp.GetContextMenuManager()->ShowPopupMenu(menu.GetSubMenu(0)->m_hMenu, point.x, point.y, AfxGetMainWnd(), TRUE);
	*/
}
