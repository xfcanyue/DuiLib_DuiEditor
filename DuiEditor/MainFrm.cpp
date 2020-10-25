
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "DuiEditor.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DlgOptions.h"

#include "ThreadTest.h"
#include "ThreadPipe.h"
#include "UIManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_GET_TAB_TOOLTIP, &CMainFrame::OnGetTabTooltip)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, &CMainFrame::OnChangeActiveTab)
	ON_COMMAND(ID_FILE_CLOSE_ALL, &CMainFrame::OnFileCloseAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_ALL, &CMainFrame::OnUpdateFileCloseAll)
	ON_COMMAND(ID_FILE_SAVE_ALL, &CMainFrame::OnFileSaveAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_ALL, &CMainFrame::OnUpdateFileSaveAll)
	ON_COMMAND(ID_EDIT_OPTIONS, &CMainFrame::OnEditOptions)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_REOPEN_FILE, &CMainFrame::OnReOpenFile)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_CONTROL_TREE, &CMainFrame::OnViewControlTree)
	ON_COMMAND(ID_INDICATOR_CURSOR_POS, NULL)
	ON_COMMAND(ID_INDICATOR_XML_STATUS, NULL)
	ON_COMMAND(ID_INDICATOR_LINE, NULL)
	ON_COMMAND(ID_INDICATOR_COL, NULL)
	ON_COMMAND(ID_INDICATOR_CURRENT_POS, NULL)
	ON_COMMAND(ID_INDICATOR_LENGTH, NULL)
	ON_COMMAND(ID_INDICATOR_TOTAL_LINE, NULL)
	ON_COMMAND(ID_VIEW_OUTPUT_BAR, &CMainFrame::OnViewOutputBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT_BAR, &CMainFrame::OnUpdateViewOutputBar)
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器

	ID_INDICATOR_CURSOR_POS,	//鼠标位置
	ID_INDICATOR_XML_STATUS,	//XML文档状态
	ID_INDICATOR_CURRENT_POS,	//当前位置
	ID_INDICATOR_LINE,			//行号
	ID_INDICATOR_COL,			//列号
	ID_INDICATOR_LENGTH,		//文档总长度
	ID_INDICATOR_TOTAL_LINE,	//文档总行数

	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	theApp.m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_BLUE;
	m_uTimerReOpenFile = 0;
	m_pDocReOpen = NULL;

	m_pDlgFind = NULL;
	m_hDlgFind = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//设置正常字体
	LOGFONT lf;   
	afxGlobalData.fontRegular.GetLogFont(&lf);   
	afxGlobalData.fontRegular.DeleteObject();   
	lf.lfHeight =-12;   
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));     // using without style office 2007   
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);  

	//设置加粗的字体 
	afxGlobalData.fontBold.GetLogFont(&lf);   
	afxGlobalData.fontBold.DeleteObject();   
	lf.lfHeight =-12;   
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));   
	afxGlobalData.fontBold.CreateFontIndirect(&lf);

	afxGlobalData.fontTooltip.GetLogFont(&lf);   
	afxGlobalData.fontTooltip.DeleteObject();   
	lf.lfHeight =-12;   
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));   
	afxGlobalData.fontTooltip.CreateFontIndirect(&lf);

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE;	// 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;			// 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;						// 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;						// 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE;					// 在选项卡区域的右边缘启用文档菜单
	mdiTabParams.m_bTabCustomTooltips = TRUE;				//指定自定义工具提示是否启用。 
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//m_wndToolBar.EnableTextLabels(TRUE);

	m_wndToolBar.RemoveButton(2);
	m_wndToolBar.RemoveButton(2);
	m_wndToolBar.RemoveButton(2);
	m_wndToolBar.RemoveButton(2);
	m_wndToolBar.RemoveButton(2);
	m_wndToolBar.RemoveButton(2);
	
	CMenu menuNew, menuOpen;
	menuNew.LoadMenu(IDR_MENU_FILE_NEW);
	m_wndToolBar.ReplaceButton(ID_FILE_NEW, CMFCToolBarMenuButton(-1, menuNew.GetSubMenu(0)->GetSafeHmenu(), GetCmdMgr()->GetCmdImage (ID_FILE_NEW)));

	int npos = m_wndToolBar.CommandToIndex(ID_EDIT_DPI);
	m_wndToolBar.RemoveButton(npos+1);
	m_wndToolBar.RemoveButton(npos+1);
	m_wndToolBar.RemoveButton(npos+1);
	m_wndToolBar.RemoveButton(npos+1);
	m_wndToolBar.RemoveButton(npos+1);	

	CMenu menuNew2, menuOpen2;
	menuNew2.LoadMenu(IDR_MENU_DPI);
	m_wndToolBar.ReplaceButton(ID_EDIT_DPI, CMFCToolBarMenuButton(-1, menuNew2.GetSubMenu(0)->GetSafeHmenu(), GetCmdMgr()->GetCmdImage (ID_EDIT_DPI)));

	m_wndToolBar.AdjustLayout(); 
	//////////////////////////////////////////////////////////////////////////
	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar(IDR_TB_FORMAT))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_FORMAT_INSERT_NORMAL_CONTROL), _T("auto"));
	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_FORMAT_INSERT_FLOAT_CONTROL), _T("float"));

	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_DESIGNER_VIEW), _T("设计"));
	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_DESIGNER_CODE), _T("代码"));
	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_DESIGNER_SPLIT_UPDOWN), _T("上下拆分"));
	m_wndToolBar2.SetToolBarBtnText (m_wndToolBar2.CommandToIndex (ID_DESIGNER_SPLIT_LEFTRIGHT), _T("左右拆分"));

	m_wndToolBar2.AdjustLayout(); 
	//////////////////////////////////////////////////////////////////////////

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURSOR_POS, _T("Cursor: "));
	m_wndStatusBar.SetPaneWidthByID(ID_INDICATOR_XML_STATUS, 16);
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LINE, _T("row: "));
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_COL, _T("col: "));
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURRENT_POS, _T("pos: "));
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LENGTH, _T("TotalLength: "));
	m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_TOTAL_LINE, _T("TotalLines: "));

	if (!m_wndFileView.Create(_T("项目视图"), this, 
		CRect(0, 0, 300, 200), TRUE, ID_VIEW_FILE, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))//,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // 未能创建
	}
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	if (!m_wndControl.Create(_T("控件视图"), this, 
		CRect(0, 0, 300, 200), TRUE, ID_VIEW_CONTROL, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))//,AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		return FALSE; // 未能创建
	}
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndControl.SetIcon(hClassViewIcon, FALSE);

	if (!m_wndToolBox.Create(_T("控件箱"), this, CRect(0, 0, 150, 200), TRUE, ID_TOOLBOX_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT))
	{
		return FALSE; // 未能创建
	}
	m_wndToolBox.m_ctlToolList.SetImageList(&g_duiProp.m_ImageList);

	if (!m_wndProperty.Create(_T("属性"), this, CRect(0, 0, 300, 200), TRUE, ID_PROPERTY_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT))
	{
		return FALSE; // 未能创建
	}

	if (!m_wndOutput.Create(_T(" 输出 "), this, CRect(0, 0, 100, 100), TRUE, ID_OUTPUT_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM))
	{
		return FALSE; // 未能创建
	}
	m_wndOutput.SetIcon(hClassViewIcon, FALSE);


	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndToolBar2);

	m_wndControl.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBox.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndProperty.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndOutput.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockPane(&m_wndToolBox);
	DockPane(&m_wndProperty);
	DockPane(&m_wndFileView);
	DockPane(&m_wndOutput);

	CDockablePane* pTabbedBar = NULL;
	m_wndControl.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	//////////////////////////////////////////////////////////////////////////
#ifndef _DEBUG
	m_wndOutput.ShowPane(FALSE, FALSE, TRUE);
#endif

	CThreadPipe::CreatePipe();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 消息处理程序
void CMainFrame::OnApplicationLook(UINT id)
{
#ifdef DUILIB_VERSION_ORIGINAL
	DuiLib::CWaitCursor wait;
#else
	CWaitCursor wait;
#endif

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

LRESULT CMainFrame::OnGetTabTooltip(WPARAM wparam, LPARAM lparam)
{
	CMFCTabToolTipInfo *info = (CMFCTabToolTipInfo *)lparam;
	//info->m_strText = "1234";

	//CDuiEditorView *pView = (CDuiEditorView *)GetMDITabs().GetTabWnd(info->m_nTabIndex);
	const CObList &list = GetMDITabGroups();
	for (POSITION pos = list.GetHeadPosition(); pos != NULL;)
	{
		CMFCTabCtrl* pNextTabWnd = DYNAMIC_DOWNCAST(CMFCTabCtrl, list.GetNext(pos));
		ASSERT_VALID(pNextTabWnd);
		CChildFrame *pChildFrame = (CChildFrame *)pNextTabWnd->GetTabWnd(info->m_nTabIndex);
		if(pChildFrame)
		{
			CView *pView = (CView *)pChildFrame->GetActiveView();
			if (pView)
			{
				info->m_strText = pView->GetDocument()->GetPathName();
			}
		}
	}
	return 0;
}

LRESULT CMainFrame::OnChangeActiveTab(WPARAM wparam, LPARAM lparam)
{
	CMFCBaseTabCtrl * pTabCtrl = ((CMFCBaseTabCtrl*)lparam);
	if (pTabCtrl && pTabCtrl->GetSafeHwnd() == GetMDITabs().GetSafeHwnd())
	{
		int nTabIdx=pTabCtrl->GetActiveTab();
		CWnd * pWnd = pTabCtrl->GetActiveWnd();
// 		if (pWnd && pWnd->GetSafeHwnd())
// 		{
// 			pWnd->SetForegroundWindow();//激活当前tab
// 		}		
	}

	return 0;
}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(g_hThreadTestHandle != NULL)
	{
		TerminateProcess(g_pThreadTest->m_piProcInfo.hProcess, 0);
		WaitForSingleObject(g_pThreadTest->m_piProcInfo.hProcess, 1000);
	}

	CThreadPipe::ClosePipe();
	CMDIFrameWndEx::OnClose();
}

CDuiEditorViewDesign *CMainFrame::GetActiveUIView()
{
	CChildFrame *pFrame=(CChildFrame *)MDIGetActive();
	if(!pFrame)
		return NULL;

	return DYNAMIC_DOWNCAST(CDuiEditorViewDesign, pFrame->GetUIManager()->GetDesignView());
}

CUIManager *CMainFrame::GetActiveUIManager()
{
	CChildFrame *pFrame=(CChildFrame *)MDIGetActive();
	if(!pFrame)
		return NULL;
	return pFrame->GetUIManager();
}

void CMainFrame::ShowAllPane()
{
	m_wndFileView.ShowPane(TRUE, TRUE, TRUE);
	m_wndControl.ShowPane(TRUE, TRUE, TRUE);

	m_wndToolBox.ShowPane(TRUE, TRUE, TRUE);
	m_wndProperty.ShowPane(TRUE, TRUE, TRUE);
	SendMessage(WM_SIZE);
}

void CMainFrame::HideAllPane()
{
	m_wndFileView.ShowPane(FALSE, TRUE, TRUE);
	m_wndControl.ShowPane(FALSE, TRUE, TRUE);

	m_wndToolBox.ShowPane(FALSE, TRUE, TRUE);
	m_wndProperty.ShowPane(FALSE, TRUE, TRUE);
	SendMessage(WM_SIZE);
}

void CMainFrame::OnFileCloseAll()
{
	AfxGetApp()->CloseAllDocuments(TRUE);
}


void CMainFrame::OnUpdateFileCloseAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(MDIGetActive() ? TRUE:FALSE);
}


void CMainFrame::OnFileSaveAll()
{
	AfxGetApp()->SaveAllModified();
}


void CMainFrame::OnUpdateFileSaveAll(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

void CMainFrame::OnEditOptions()
{
	CDlgOptions dlg;
	dlg.DoModal();
}



BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CMDIFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


LRESULT CMainFrame::OnReOpenFile(WPARAM wparam, LPARAM lparam)
{
	if(m_uTimerReOpenFile != 0)
		return -1;

	m_strReOpenFile = (LPCTSTR)wparam;
	m_pDocReOpen = (CDuiEditorDoc *)lparam;
	m_uTimerReOpenFile = SetTimer(1, 200, NULL);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == m_uTimerReOpenFile)
	{
		POSITION pos = ((CDuiEditorApp *)AfxGetApp())->GetFirstDocTemplatePosition();
		while (pos != NULL)
		{
			CDocTemplate *pDocTemplate = ((CDuiEditorApp *)AfxGetApp())->GetNextDocTemplate(pos);			
			POSITION pos1 = pDocTemplate->GetFirstDocPosition();
			while (pos1 != NULL)
			{
				if(pDocTemplate->GetNextDoc(pos1) == m_pDocReOpen)
				{
					//InsertMsg(_T("还没关闭"));
					goto __LABEL_END;
				}
			}
		}
		KillTimer(m_uTimerReOpenFile); m_uTimerReOpenFile = 0;
		AfxGetApp()->OpenDocumentFile(m_strReOpenFile);
		m_strReOpenFile.Empty();
		m_pDocReOpen = NULL;

		__LABEL_END:;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnViewControlTree()
{
	if(m_wndControl.IsPaneVisible())
	{
		m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
	}
	else
	{
		m_wndControl.ShowPane(TRUE, FALSE,TRUE);
	}
}

void CMainFrame::OnViewOutputBar()
{
	m_wndOutput.ShowPane(!m_wndOutput.IsPaneVisible(), FALSE, TRUE);
}


void CMainFrame::OnUpdateViewOutputBar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsPaneVisible());
}

void CMainFrame::OnEditFind()
{
	if(::IsWindow(m_hDlgFind))
	{
		m_pDlgFind->CenterWindow(this);
		return;
	}

	m_pDlgFind = new CSciFind;
	m_pDlgFind->Create(IDD_SCI_FIND, this);
	m_pDlgFind->CenterWindow(this);
	m_pDlgFind->ShowWindow(SW_SHOW);

	m_hDlgFind = m_pDlgFind->m_hWnd;
}
