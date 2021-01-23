// ViewControlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockScriptStack.h"

#include "MainFrm.h"
// CViewControlWnd

IMPLEMENT_DYNAMIC(CDockScriptStackWnd, CDockablePane)

CDockScriptStackWnd::CDockScriptStackWnd()
{

}

CDockScriptStackWnd::~CDockScriptStackWnd()
{
}


BEGIN_MESSAGE_MAP(CDockScriptStackWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CViewControlWnd 消息处理程序
int CDockScriptStackWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CDockScriptStackWnd::OnSize(UINT nType, int cx, int cy)
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


void CDockScriptStackWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	m_tabClass.SetFocus();
}

CMFCPropertyGridCtrl *CDockScriptStackWnd::CreateStackView()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	CMFCPropertyGridCtrl *pPropList = new CMFCPropertyGridCtrl;
	if (!pPropList->Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rectDummy, &m_tabClass, 2))
	{
		TRACE0("未能创建属性网格\n");
		return NULL;      // 未能创建
	}

	pPropList->EnableHeaderCtrl(FALSE);
	pPropList->EnableDescriptionArea();
	pPropList->SetVSDotNetLook();
	pPropList->MarkModifiedProperties();
	pPropList->SetFont(&afxGlobalData.fontRegular);

	m_tabClass.AddTab(pPropList, _T(""));
	SetActiveStackView(pPropList);
	return pPropList;
}

void CDockScriptStackWnd::RemoveStackView(CMFCPropertyGridCtrl *pView)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pView)
		{
			m_tabClass.RemoveTab(i);
			delete pView;
			break;
		}
	}
}

void CDockScriptStackWnd::SetActiveStackView(CMFCPropertyGridCtrl *pView)
{
	for (int i=0; i<m_tabClass.GetTabsNum(); i++)
	{
		if(m_tabClass.GetTabWnd(i) == pView)
		{
			m_tabClass.SetActiveTab(i);
			break;
		}
	}
}

void CDockScriptStackWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
}
