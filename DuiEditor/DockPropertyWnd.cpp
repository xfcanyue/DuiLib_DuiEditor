// PropertyWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockPropertyWnd.h"

#include "MainFrm.h"
#include "DockControlTreeWnd.h"
#include "DockOutputWnd.h"
#include "UIManager.h"

// CPropertyWnd

IMPLEMENT_DYNAMIC(CDockPropertyWnd, CDockablePane)

CDockPropertyWnd::CDockPropertyWnd()
{
	
}

CDockPropertyWnd::~CDockPropertyWnd()
{
}


BEGIN_MESSAGE_MAP(CDockPropertyWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES_FIND_TEXT, &CDockPropertyWnd::OnPropertiesFindText)
	ON_BN_CLICKED(ID_PROPERTIES_FIND_BUTTON, &CDockPropertyWnd::OnPropertiesFindButton)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_FIND_BUTTON, &CDockPropertyWnd::OnUpdatePropertiesFindButton)
	ON_EN_CHANGE(IDC_EDIT1, &CDockPropertyWnd::OnChangeEdit1)
END_MESSAGE_MAP()



// CPropertyWnd 消息处理程序

CUIPropertyGridCtrl *CDockPropertyWnd::CreatePropList()
{
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	CUIPropertyGridCtrl *pPropList = new CUIPropertyGridCtrl;
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

	pPropList->m_pPropertyWnd = this;

	m_tabClass.AddTab(pPropList, _T(""));
	SetActivePropList(pPropList);
	return pPropList;
}

void CDockPropertyWnd::RemovePropList(CUIPropertyGridCtrl *pTreeView)
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

void CDockPropertyWnd::SetActivePropList(CUIPropertyGridCtrl *pTreeView)
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

CUIPropertyGridCtrl *CDockPropertyWnd::GetPropList(int nTab)
{
	return (CUIPropertyGridCtrl *)m_tabClass.GetTabWnd(nTab);
}

int CDockPropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();


	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndFindText.Create(dwViewStyle, rectDummy, this, IDC_EDIT1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}
	m_wndFindText.SetFont(&afxGlobalData.fontRegular);
	m_wndFindText.EnableFolderBrowseButton();
	m_wndFindText.SetImage();

	m_tabClass.Create (CMFCTabCtrl::STYLE_3D_ONENOTE,
		CRect(0,0,0,0), this, ID_VIEW_CONTROL_TAB, CMFCTabCtrl::LOCATION_TOP); 
	return 0;
}


void CDockPropertyWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	
	int top = cyTlb;
	m_wndFindText.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rc = CRect(0,0,0,0);
	m_wndFindText.GetClientRect(&rc);
	CDC* pDC = m_wndFindText.GetDC();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int nFontHeight  = tm.tmHeight + tm.tmExternalLeading;
	int nTopBottom  = (rc.Height() - nFontHeight) / 2;
	int nLeftRight     = 5;
	rc.DeflateRect(nLeftRight, nTopBottom);
	m_wndFindText.SetRectNP(&rc);
	ReleaseDC(pDC);

	m_tabClass.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb*2, rectClient.Width(), rectClient.Height()-cyTlb*2, 
		SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CDockPropertyWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
// 	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
// 
// 	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
// 	CDuiEditorViewDesign *pView = pMain->GetActiveUIView();
// 
// 	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
//	pView->GetUIManager()->GetTreeView()->UpdateXmlNode(pPropList->m_TreeNode);
	return 0;
}

void CDockPropertyWnd::OnExpandAllProperties()
{
	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
	pPropList->ExpandAll();
}

void CDockPropertyWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CDockPropertyWnd::OnSortProperties()
{
	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
	if(pPropList)
		pPropList->SetAlphabeticMode(!pPropList->IsAlphabeticMode());
}

void CDockPropertyWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
	if(pPropList)
		pCmdUI->SetCheck(pPropList->IsAlphabeticMode());
}

void CDockPropertyWnd::OnPropertiesFindText()
{
	// TODO: 在此添加命令处理程序代码
}


void CDockPropertyWnd::OnPropertiesFindButton()
{

	CString strFilter;
	m_wndFindText.GetWindowText(strFilter);
	if(strFilter.IsEmpty()) return;

	m_wndFindText.SetWindowText(_T(""));

	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
	if(pPropList)
	{
		pPropList->InitProp(pPropList->m_TreeNode);
	}
}


void CDockPropertyWnd::OnUpdatePropertiesFindButton(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CDockPropertyWnd::OnChangeEdit1()
{
	CUIPropertyGridCtrl *pPropList = GetPropList(m_tabClass.GetActiveTab());
	if(pPropList)
	{
		pPropList->InitProp(pPropList->m_TreeNode);
	}
}
