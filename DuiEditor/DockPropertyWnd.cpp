// PropertyWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockPropertyWnd.h"

#include "MainFrm.h"
#include "DockControlTreeWnd.h"
#include "DockOutputWnd.h"

CDockPropertyWnd *g_pPropWnd;
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
	ON_COMMAND(ID_PROPERTIES_FIND_BUTTON, &CDockPropertyWnd::OnPropertiesFindButton)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_FIND_BUTTON, &CDockPropertyWnd::OnUpdatePropertiesFindButton)
	ON_COMMAND(ID_PROPERTIES_FIND_BUTTON2, &CDockPropertyWnd::OnPropertiesFindButton2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES_FIND_BUTTON2, &CDockPropertyWnd::OnUpdatePropertiesFindButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CDockPropertyWnd::OnChangeEdit1)
END_MESSAGE_MAP()



// CPropertyWnd 消息处理程序




int CDockPropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rectDummy, this, 2))
	{
		TRACE0("未能创建属性网格\n");
		return -1;      // 未能创建
	}

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 锁定*/);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (!m_wndFindText.Create(dwViewStyle, rectDummy, this, IDC_EDIT1))
	{
		TRACE0("未能创建属性组合 \n");
		return -1;      // 未能创建
	}
	m_wndFindText.SetFont(&afxGlobalData.fontRegular);

	//m_wndFind.Create(_T("1"), WS_CHILD|WS_VISIBLE, rectDummy, this, 1);
	//m_wndFind.SetFont(&afxGlobalData.fontRegular);

	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.SetFont(&afxGlobalData.fontRegular);

	//m_wndPropList.SetAlphabeticMode(TRUE);

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
	m_wndFindText.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), cyTlb*2, SWP_NOACTIVATE | SWP_NOZORDER);

	//m_wndFind.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), cyTlb*2, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb*2, rectClient.Width(), rectClient.Height()-cyTlb*2, 
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDockPropertyWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

void CDockPropertyWnd::InitProp(xml_node TreeNode, LPCTSTR strFilter)
{
	m_wndPropList.RemoveAll();
	SetPropListFont();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	m_wndPropList.SetFont(&afxGlobalData.fontRegular);
	//m_wndPropList.SetAlphabeticMode(TRUE);

	CString strText = _T("属性 -- ");
	strText += TreeNode.name();
	SetWindowText(strText);

	m_wndPropList.m_strFilter = strFilter;
	m_wndPropList.InitProp(TreeNode);
	return;
}

LRESULT CDockPropertyWnd::OnPropertyChanged (WPARAM,LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDuiEditorViewDesign *pView = pMain->GetActiveUIView();
	pView->m_Manager.GetTreeView()->UpdateXmlNode(m_wndPropList.m_TreeNode);
	return 0;
}

void CDockPropertyWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CDockPropertyWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CDockPropertyWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CDockPropertyWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CDockPropertyWnd::OnPropertiesFindText()
{
	// TODO: 在此添加命令处理程序代码
}


void CDockPropertyWnd::OnPropertiesFindButton()
{
	// TODO: 在此添加命令处理程序代码
}


void CDockPropertyWnd::OnUpdatePropertiesFindButton(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDockPropertyWnd::OnPropertiesFindButton2()
{
	// TODO: 在此添加命令处理程序代码
}


void CDockPropertyWnd::OnUpdatePropertiesFindButton2(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CDockPropertyWnd::OnChangeEdit1()
{
	CString strFilter;
	m_wndFindText.GetWindowText(strFilter);

	InitProp(m_wndPropList.m_TreeNode, strFilter);
}
