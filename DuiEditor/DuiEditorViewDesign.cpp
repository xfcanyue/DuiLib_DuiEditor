
// DuiEditorView.cpp : CDuiEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DuiEditor.h"
#endif

#include "MainFrm.h"
#include "DuiEditorDoc.h"
#include "DuiEditorViewDesign.h"

#include "DockControlTreeWnd.h"
#include "ThreadTest.h"
#include "ThreadPipe.h"

#include "UIWindowEx.h"
#include "UIWindowMenu.h"

#include "DlgTemplateSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDuiEditorView

IMPLEMENT_DYNCREATE(CDuiEditorViewDesign, CScrollView)

BEGIN_MESSAGE_MAP(CDuiEditorViewDesign, CScrollView)
	// 标准打印命令
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_EDIT_CUT, &CDuiEditorViewDesign::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDuiEditorViewDesign::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CDuiEditorViewDesign::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDuiEditorViewDesign::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDuiEditorViewDesign::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDuiEditorViewDesign::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, &CDuiEditorViewDesign::OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CDuiEditorViewDesign::OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_COPY_NAME, &CDuiEditorViewDesign::OnEditCopyName)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_NAME, &CDuiEditorViewDesign::OnUpdateEditCopyName)
	ON_COMMAND(ID_EDIT_COPY_NAME_EX, &CDuiEditorViewDesign::OnEditCopyNameEx)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_NAME_EX, &CDuiEditorViewDesign::OnUpdateEditCopyNameEx)
	ON_COMMAND(ID_EDIT_UNDO, &CDuiEditorViewDesign::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDuiEditorViewDesign::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDuiEditorViewDesign::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDuiEditorViewDesign::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_INSERT_FONT_NODE, &CDuiEditorViewDesign::OnEditInsertFont)
	ON_COMMAND(ID_EDIT_INSERT_DEFAULT_NODE, &CDuiEditorViewDesign::OnEditInsertDefault)
	ON_COMMAND(ID_UIFORM_DEBUG, &CDuiEditorViewDesign::OnUiformDebug)
	ON_UPDATE_COMMAND_UI(ID_UIFORM_DEBUG, &CDuiEditorViewDesign::OnUpdateUiformDebug)
	ON_COMMAND(ID_UIFORM_DEBUG_END, &CDuiEditorViewDesign::OnUiformDebugEnd)
	ON_UPDATE_COMMAND_UI(ID_UIFORM_DEBUG_END, &CDuiEditorViewDesign::OnUpdateUiformDebugEnd)
	ON_COMMAND(ID_EDIT_INSERT_STYLE_NODE, &CDuiEditorViewDesign::OnEditInsertStyleNode)
	ON_COMMAND_RANGE(ID_TABLAYOUT_SETSEL_00, ID_TABLAYOUT_SETSEL_19, &CDuiEditorViewDesign::OnCommandTabLayoutSetSel)
	ON_COMMAND(ID_FORMAT_ALIGN_LEFT, &CDuiEditorViewDesign::OnFormatAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_LEFT, &CDuiEditorViewDesign::OnUpdateFormatAlignLeft)
	ON_COMMAND(ID_FORMAT_ALIGN_RIGHT, &CDuiEditorViewDesign::OnFormatAlignRight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_RIGHT, &CDuiEditorViewDesign::OnUpdateFormatAlignRight)
	ON_COMMAND(ID_FORMAT_ALIGN_TOP, &CDuiEditorViewDesign::OnFormatAlignTop)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_TOP, &CDuiEditorViewDesign::OnUpdateFormatAlignTop)
	ON_COMMAND(ID_FORMAT_ALIGN_BOTTOM, &CDuiEditorViewDesign::OnFormatAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_BOTTOM, &CDuiEditorViewDesign::OnUpdateFormatAlignBottom)
	ON_COMMAND(ID_FORMAT_ALIGN_VERT, &CDuiEditorViewDesign::OnFormatAlignSameSpaceVert)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_VERT, &CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceVert)
	ON_COMMAND(ID_FORMAT_ALIGN_HORI, &CDuiEditorViewDesign::OnFormatAlignSameSpaceHori)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_HORI, &CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceHori)
	ON_COMMAND(ID_FORMAT_ALIGN_CENTER_VERT, &CDuiEditorViewDesign::OnFormatAlignCenterVert)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_CENTER_VERT, &CDuiEditorViewDesign::OnUpdateFormatAlignCenterVert)
	ON_COMMAND(ID_FORMAT_ALIGN_CENTER_HORI, &CDuiEditorViewDesign::OnFormatAlignCenterHori)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_ALIGN_CENTER_HORI, &CDuiEditorViewDesign::OnUpdateFormatAlignCenterHori)
	ON_COMMAND(ID_FORMAT_SAME_WIDTH, &CDuiEditorViewDesign::OnFormatSameWidth)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SAME_WIDTH, &CDuiEditorViewDesign::OnUpdateFormatSameWidth)
	ON_COMMAND(ID_FORMAT_SAME_HEIGHT, &CDuiEditorViewDesign::OnFormatSameHeight)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SAME_HEIGHT, &CDuiEditorViewDesign::OnUpdateFormatSameHeight)
	ON_COMMAND(ID_FORMAT_SAME_SIZE, &CDuiEditorViewDesign::OnFormatSameSize)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SAME_SIZE, &CDuiEditorViewDesign::OnUpdateFormatSameSize)
	ON_COMMAND(ID_EDIT_ZOOM_IN, &CDuiEditorViewDesign::OnEditZoomIn)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ZOOM_IN, &CDuiEditorViewDesign::OnUpdateEditZoomIn)
	ON_COMMAND(ID_EDIT_ZOOM_OUT, &CDuiEditorViewDesign::OnEditZoomOut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ZOOM_OUT, &CDuiEditorViewDesign::OnUpdateEditZoomOut)
	ON_COMMAND(ID_EDIT_ZOOM_FULL, &CDuiEditorViewDesign::OnEditZoomFull)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ZOOM_FULL, &CDuiEditorViewDesign::OnUpdateEditZoomFull)
	ON_WM_SIZE()
	ON_COMMAND(ID_FORMAT_INSERT_NORMAL_CONTROL, &CDuiEditorViewDesign::OnFormatInsertNormalControl)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_INSERT_NORMAL_CONTROL, &CDuiEditorViewDesign::OnUpdateFormatInsertNormalControl)
	ON_COMMAND(ID_FORMAT_INSERT_FLOAT_CONTROL, &CDuiEditorViewDesign::OnFormatInsertFloatControl)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_INSERT_FLOAT_CONTROL, &CDuiEditorViewDesign::OnUpdateFormatInsertFloatControl)
	ON_COMMAND(ID_FORMAT_SHOW_RULERBAR, &CDuiEditorViewDesign::OnFormatShowRulerbar)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SHOW_RULERBAR, &CDuiEditorViewDesign::OnUpdateFormatShowRulerbar)
	ON_COMMAND(ID_FORMAT_SHOW_GRID, &CDuiEditorViewDesign::OnFormatShowGrid)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SHOW_GRID, &CDuiEditorViewDesign::OnUpdateFormatShowGrid)
	ON_COMMAND(ID_FORMAT_SHOW_MOUSE, &CDuiEditorViewDesign::OnFormatShowMouse)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SHOW_MOUSE, &CDuiEditorViewDesign::OnUpdateFormatShowMouse)
	ON_COMMAND(ID_FORMAT_SHOW_UI_PREVIEW, &CDuiEditorViewDesign::OnFormatShowUiPreview)
	ON_UPDATE_COMMAND_UI(ID_FORMAT_SHOW_UI_PREVIEW, &CDuiEditorViewDesign::OnUpdateFormatShowUiPreview)
	ON_COMMAND_RANGE(ID_EDIT_DPI, ID_EDIT_DPI_200, OnSetDPI)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EDIT_DPI, ID_EDIT_DPI_200, OnUpdateSetDPI)
	ON_COMMAND(ID_FILE_SAVE_TEMPLATE, &CDuiEditorViewDesign::OnFileSaveTemplate)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_TEMPLATE, &CDuiEditorViewDesign::OnUpdateFileSaveTemplate)
END_MESSAGE_MAP()

// CDuiEditorView 构造/析构

CDuiEditorViewDesign::CDuiEditorViewDesign()
{
	m_zoom = 100;

	m_nFormatInsert = 0;
	m_bShowUiPreview = FALSE;		//是否在编辑界面显示UI效果
	m_bViewRuleBar	= TRUE;			//是否显示标尺
	m_bViewGrid		= FALSE;		//是否显示网格
	m_bViewMouse	= FALSE;		//是否显示鼠标线

	ZeroMemory( &m_piProcInfo, sizeof(PROCESS_INFORMATION) );
	ZeroMemory( &m_siStartInfo, sizeof(STARTUPINFO) );
}

CDuiEditorViewDesign::~CDuiEditorViewDesign()
{
}

// CDuiEditorView 绘制

void CDuiEditorViewDesign::OnDraw(CDC* pDrawDC)
{
}

void CDuiEditorViewDesign::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);	

	InitView();
}

void CDuiEditorViewDesign::InitView()
{
	if(m_Manager.GetUiWindow())
	{
		m_Manager.GetUiWindow()->DestroyUiWindow();
		delete m_Manager.m_pUiWindow;
		m_Manager.m_pUiWindow = NULL;
	}


	SetScrollSizes(MM_TEXT, CSize(0,0));

	m_cmdHistory.m_pManager = &m_Manager;
	m_cmdHistory.Init(GetDocument());

	m_Manager.m_pView = this;
	m_Manager.m_pDoc = GetDocument();
	m_Manager.m_pCmdHistory = &m_cmdHistory;
	m_Manager.m_pTreeView = GetDocument()->GetTreeView();

// 	CPoint pos = GetScrollPosition();
// 	CString temp;
// 	temp.Format(_T("GetScrollPosition(), x=%d, y=%d"), pos.x, pos.y);
// 	InsertMsg(temp);

	CUIWindow *pUiWindow;	
//  	if(GetDocument()->m_bMenuWnd)
//  		pUiWindow = new CUIWindowMenu;
//  	else
		pUiWindow = new CUIWindowEx;

	pUiWindow->m_pManager = &m_Manager;
	
	pUiWindow->CreateUiWindow(this->m_hWnd, _T("ViewDesign"), UI_WNDSTYLE_CHILD, 0);

	CRect rc;
	pUiWindow->GetWindowRect(rc);
	if(m_bViewRuleBar)
		pUiWindow->MoveWindow(RULEBAR_SIZE_X, RULEBAR_SIZE_Y, rc.Width(), rc.Height(), TRUE);
	else
		pUiWindow->MoveWindow(0, 0, rc.Width(), rc.Height(), TRUE);

	m_Manager.m_pUiWindow = pUiWindow;
	m_Manager.m_pManager = pUiWindow->GetManager();
	m_Manager.m_pUiTracker = pUiWindow->GetUiTracker();

	m_Manager.SetScrollSize();

	m_Manager.GetTreeView()->m_pManager = &m_Manager;
	m_Manager.GetTreeView()->InitTreeContent();

	//((CMainFrame *)AfxGetMainWnd())->ShowAllPane();
}

void CDuiEditorViewDesign::OnDestroy()
{
	CScrollView::OnDestroy();

	if(m_Manager.GetUiWindow())
	{
		m_Manager.GetUiWindow()->DestroyUiWindow();
		delete m_Manager.m_pUiWindow;
		m_Manager.m_pUiWindow = NULL;
	}
}

void CDuiEditorViewDesign::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

#ifdef _DEBUG
CDuiEditorDoc* CDuiEditorViewDesign::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDuiEditorDoc)));
	return (CDuiEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CDuiEditorView 消息处理程序
BOOL CDuiEditorViewDesign::OnEraseBkgnd(CDC* pDC)
{
	//刷新标尺和空白区域
	CRect rectClient;
	GetClientRect(rectClient);
	CPoint point=GetScrollPosition();
	rectClient.OffsetRect(point);

	CRect rcUiWnd;
	m_Manager.GetUiWindow()->GetWindowRect(rcUiWnd);
	if(rectClient.Width() - rcUiWnd.Width() > 0) //刷新UI窗口右边的空白区域
	{
		CRect rc;
		if(m_bViewRuleBar)
			rc.SetRect(RULEBAR_SIZE_X+rcUiWnd.Width(), 0, rectClient.Width(), rectClient.Height());
		else
			rc.SetRect(rcUiWnd.Width(), 0, rectClient.Width(), rectClient.Height());


		pDC->FillSolidRect(rc, g_crBkDesign);
	}

	if(rectClient.Height() - rcUiWnd.Height())	//刷新UI窗口下边的空白区域
	{
		CRect rc;
		if(m_bViewRuleBar)
			rc.SetRect(0, RULEBAR_SIZE_Y+rcUiWnd.Height(), rectClient.Width(), rectClient.Height());
		else
			rc.SetRect(0, rcUiWnd.Height(), rectClient.Width(), rectClient.Height());

		pDC->FillSolidRect(rc, g_crBkDesign);
	}

	if(m_bViewRuleBar)
	{
		CRect rc1(0,0, rectClient.Width(), RULEBAR_SIZE_Y);
		pDC->FillSolidRect(rc1,g_crBkDesign);

		CRect rc2(0,0, RULEBAR_SIZE_Y, rectClient.Height());
		pDC->FillSolidRect(rc2,g_crBkDesign);

		for (int i=0; i<=200; i++)
		{
			pDC->MoveTo(RULEBAR_SIZE_X + i*10, 0);
			if(i*10%100 == 0)
				pDC->LineTo(RULEBAR_SIZE_X + i*10, 15);
			else
				pDC->LineTo(RULEBAR_SIZE_X + i*10, 5);

			pDC->MoveTo(0, RULEBAR_SIZE_Y + i*10);
			if(i*10%100 == 0)
				pDC->LineTo(15, RULEBAR_SIZE_Y + i*10);
			else
				pDC->LineTo(5, RULEBAR_SIZE_Y + i*10);
		}
	}

	return TRUE;
	return CScrollView::OnEraseBkgnd(pDC);
}

void CDuiEditorViewDesign::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	if(bActivate && pActivateView==this && pDeactiveView!=this)
	{
		pMain->m_wndControl.SetActiveTreeView(m_Manager.GetTreeView());
		pMain->ShowAllPane();

		//切换页面时, 也需要刷新属性窗口
// 		if(m_Manager.m_pUiTracker && m_Manager.m_pUiTracker->m_node)
// 		{
// 			g_pPropWnd->InitProp(m_Manager.m_pUiTracker->m_node);
// 		}

		//InsertMsg(_T("OnActivateDesign"));


		//主要是为了更新DuiLib的窗口阴影
		if(m_Manager.m_pUiWindow && m_Manager.m_pUiWindow->GetSafeHwnd())
		{
			::PostMessage(m_Manager.m_pUiWindow->GetSafeHwnd(), WM_SHOWWINDOW, bActivate, 0);
		}
	}

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDuiEditorViewDesign::OnLButtonDown(UINT nFlags, CPoint pt)
{
	xml_node node((xml_node_struct *)m_Manager.GetUiFrom()->GetTag());

	g_pPropWnd->InitProp(node);
	m_Manager.SelectItem(node);
	m_Manager.GetTreeView()->SelectXmlNode(node);
}

void CDuiEditorViewDesign::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
}

void CDuiEditorViewDesign::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDuiEditorViewDesign::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
}

void CDuiEditorViewDesign::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDuiEditorViewDesign::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
}

void CDuiEditorViewDesign::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CDuiEditorViewDesign::OnEditClear()
{
	for (int i=0; i<m_Manager.GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = m_Manager.GetUiTracker()->m_arrTracker[i];
		m_Manager.DeleteControl(pTrackElem->m_node);
	}
	m_Manager.GetUiTracker()->RemoveAll();
	GetDocument()->SetModifiedFlag(TRUE);
}

void CDuiEditorViewDesign::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditCopyName()
{
	for (int i=0; i<m_Manager.GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = m_Manager.GetUiTracker()->m_arrTracker[i];
		CString strText = pTrackElem->m_node.attribute(_T("name")).as_string();

		if(OpenClipboard())
		{
			EmptyClipboard();

			HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (strText.GetLength()+1) * sizeof(TCHAR));
			TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
			_tcscpy(buffer, strText);
			GlobalUnlock(clipbuffer);
#ifdef _UNICODE
			SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
			SetClipboardData(CF_TEXT, clipbuffer);
#endif
			CloseClipboard();
		}

		break;	//只复制第一个control
	}
}

void CDuiEditorViewDesign::OnUpdateEditCopyName(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditCopyNameEx()
{
	for (int i=0; i<m_Manager.GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = m_Manager.GetUiTracker()->m_arrTracker[i];
		//CString strText = pTrackElem->m_node.attribute(_T("name")).as_string();
		CString strText = _T("_T(\"");
		strText += pTrackElem->m_node.attribute(_T("name")).as_string();
		strText += _T("\")");

		if(OpenClipboard())
		{
			EmptyClipboard();

			HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (strText.GetLength()+1) * sizeof(TCHAR));
			TCHAR *buffer = (TCHAR *)GlobalLock(clipbuffer);
			_tcscpy(buffer, strText);
			GlobalUnlock(clipbuffer);
#ifdef _UNICODE
			SetClipboardData(CF_UNICODETEXT, clipbuffer);
#else
			SetClipboardData(CF_TEXT, clipbuffer);
#endif
			CloseClipboard();
		}

		break;	//只复制第一个control
	}
}

void CDuiEditorViewDesign::OnUpdateEditCopyNameEx(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditUndo()
{
	m_cmdHistory.Undo();
}

void CDuiEditorViewDesign::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_cmdHistory.CanUndo());
	//pCmdUI->Enable(FALSE);
}

void CDuiEditorViewDesign::OnEditRedo()
{
	m_cmdHistory.Redo();
}

void CDuiEditorViewDesign::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_cmdHistory.CanRedo());
	//pCmdUI->Enable(FALSE);
}

void CDuiEditorViewDesign::OnEditInsertFont()
{
	xml_node nodeWinodw = GetDocument()->m_doc.child(_T("Window"));
	xml_node nodeFont = nodeWinodw.child(_T("Font"));
	if(nodeFont)
		nodeFont = nodeWinodw.insert_child_after(_T("Font"), nodeFont);
	else
		nodeFont = nodeWinodw.prepend_child(_T("Font"));

	g_duiProp.AddAttribute(nodeFont, _T("id"), 0,				this);
	g_duiProp.AddAttribute(nodeFont, _T("name"), _T("宋体"),	this);
	g_duiProp.AddAttribute(nodeFont, _T("size"), 12,			this);

	m_Manager.GetTreeView()->AddNewControl(nodeFont, TVI_FIRST);
	g_pPropWnd->InitProp(nodeFont);
}

void CDuiEditorViewDesign::OnEditInsertDefault()
{
	xml_node nodeWinodw = GetDocument()->m_doc.child(_T("Window"));
	xml_node nodeDefult = nodeWinodw.child(_T("Default"));
	if(nodeDefult)
		nodeDefult = nodeWinodw.insert_child_after(_T("Default"), nodeDefult);
	else
		nodeDefult = nodeWinodw.prepend_child(_T("Default"));

	m_Manager.GetTreeView()->AddNewControl(nodeDefult, TVI_FIRST);
	g_pPropWnd->InitProp(nodeDefult);
}

void CDuiEditorViewDesign::OnEditInsertStyleNode()
{
	xml_node nodeWinodw = GetDocument()->m_doc.child(_T("Window"));
	xml_node nodeStyle = nodeWinodw.child(_T("Style"));
	if(nodeStyle)
	{
		nodeStyle = nodeWinodw.insert_child_after(_T("Style"), nodeStyle);
	}
	else
		nodeStyle = nodeWinodw.prepend_child(_T("Style"));

	g_duiProp.AddAttribute(nodeStyle, _T("name"), _T(""),	this);
	g_duiProp.AddAttribute(nodeStyle, _T("class"), _T(""),	this);

	m_Manager.GetTreeView()->AddNewControl(nodeStyle, TVI_FIRST);
	g_pPropWnd->InitProp(nodeStyle);
}

void CDuiEditorViewDesign::OnUiformDebug()
{
	AfxGetApp()->SaveAllModified();

	g_pThreadTest = new CThreadTest;
	g_pThreadTest->m_pDoc = GetDocument();
	g_pThreadTest->CreateThread();
	return;
}

void CDuiEditorViewDesign::OnUpdateUiformDebug(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hThreadTestHandle == NULL);
}

void CDuiEditorViewDesign::OnUiformDebugEnd()
{
	if(g_hThreadTestHandle == NULL)
		return;

	//	PostThreadMessage(g_pThreadTest->m_piProcInfo.dwThreadId, WM_QUIT, 0, 0);
	//	if(WaitForSingleObject(g_pThreadTest->m_piProcInfo.hProcess, 1000) != WAIT_OBJECT_0)
	//	{
	TerminateProcess(g_pThreadTest->m_piProcInfo.hProcess, 0);
	InsertMsg(_T("TerminateProcess"));
	//	}
}

void CDuiEditorViewDesign::OnUpdateUiformDebugEnd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hThreadTestHandle != NULL);
}

void CDuiEditorViewDesign::OnCommandTabLayoutSetSel(UINT nID)
{
	int nSel = nID - ID_TABLAYOUT_SETSEL_00;

	HTREEITEM hItem = m_Manager.GetTreeView()->GetSelectedItem();
	if(hItem)
	{
		xml_node node((xml_node_struct *)m_Manager.GetTreeView()->GetItemData(hItem));
		if(node)
		{
			CTabLayoutUI *pTab = (CTabLayoutUI *)node.get_tag();
			if(pTab->GetInterface(DUI_CTR_TABLAYOUT))
			{
				g_duiProp.AddAttribute(node,  _T("selectedid"), nSel,	this);
				m_Manager.UpdateControlUI(node, node.attribute(_T("selectedid")));
			}
		}
	}
}


void CDuiEditorViewDesign::OnFormatAlignLeft()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcFocus.left;
			rc.right	= rc.left + rcElem.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignLeft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignRight()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcFocus.right - rcElem.Width();
			rc.right	= rcFocus.right;
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignRight(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignTop()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcFocus.top;
			rc.bottom	= rcFocus.top + rcElem.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignTop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignBottom()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcFocus.bottom - rcElem.Height();
			rc.bottom	= rcFocus.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignBottom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}

void CDuiEditorViewDesign::OnFormatAlignCenterVert()
{
	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CControlUI *pParent = elem->m_pControl->GetParent();
		if(!pParent) continue;
		CRect rcParent = pParent->GetPos();

		CRect rc;
		rc.left		= rcElem.left;
		rc.right	= rc.left + rcElem.Width();
		rc.top		= rcParent.top + (rcParent.Height() - rcElem.Height())/2;
		rc.bottom	= rc.top + rcElem.Height();
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
		m_Manager.UpdateControlUI(elem->m_node, attr);
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignCenterVert(CCmdUI *pCmdUI)
{

}


void CDuiEditorViewDesign::OnFormatAlignCenterHori()
{
	//多选和单选的居中, 是不一样的
	if(m_Manager.GetUiTracker()->GetSize() > 1)
	{

	}
	else
	{

	}

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CControlUI *pParent = elem->m_pControl->GetParent();
		if(!pParent) continue;
		CRect rcParent = pParent->GetPos();

		CRect rc;
		rc.left		= rcParent.left + (rcParent.Width() - rcElem.Width())/2;
		rc.right	= rc.left + rcElem.Width();
		rc.top		= rcElem.top;
		rc.bottom	= rc.top + rcElem.Height();
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
		m_Manager.UpdateControlUI(elem->m_node,  attr);
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignCenterHori(CCmdUI *pCmdUI)
{

}

void CDuiEditorViewDesign::OnFormatAlignSameSpaceVert()
{
	if(m_Manager.GetUiTracker()->GetSize() <= 2)	return;

	//创建一个从上到下的数组
	CArray<CUITrackerMuliti::CTrackerElement*, CUITrackerMuliti::CTrackerElement*> arrTracker;
	arrTracker.Copy(m_Manager.GetUiTracker()->m_arrTracker);
	for (int i=0; i<arrTracker.GetSize(); i++)
	{
		for (int j=i+1; j<arrTracker.GetSize(); j++)
		{
			CRect rc1 = arrTracker[i]->m_pControl->GetPos();
			CRect rc2 = arrTracker[j]->m_pControl->GetPos();

			if(rc1.top > rc2.top)
			{
				CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
				arrTracker[i] = arrTracker[j];
				arrTracker[j] = elem;
			}
		}
	}


	//取得第一个和最后一个控件的位置
	CUITrackerMuliti::CTrackerElement *pFirstElem = arrTracker[0];
	CRect rcElemFirst = pFirstElem->m_pControl->GetPos();

	CUITrackerMuliti::CTrackerElement *pLastElem = arrTracker[arrTracker.GetSize()-1];
	CRect rcElemLast = pLastElem->m_pControl->GetPos();

	//计算控件占用的空间
	int nHeightAll = 0;
	for (int i=0; i<arrTracker.GetSize(); i++)
	{
		CRect rcElem = arrTracker[i]->m_pControl->GetPos();
		nHeightAll += rcElem.Height();
	}

	//控件的间隔
	int nSpace = (rcElemLast.bottom - rcElemFirst.top - nHeightAll) / (arrTracker.GetSize()-1);

	//调整中间控件的位置
	for (int i=1; i<arrTracker.GetSize()-1; i++)
	{
		CControlUI *pControlParent = arrTracker[i-1]->m_pControl;
		CRect rcPre = arrTracker[i-1]->m_pControl->GetPos();

		CUITrackerMuliti::CTrackerElement *elem = arrTracker[i]; 
		CRect rcElem = elem->m_pControl->GetPos();

		CRect rc;
		rc.left = rcElem.left;
		rc.right = rcElem.right;
		rc.top	 = rcPre.bottom + nSpace;
		rc.bottom = rc.top + rcElem.Height();
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
		m_Manager.UpdateControlUI(elem->m_node,  attr);
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceVert(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 2);
}


void CDuiEditorViewDesign::OnFormatAlignSameSpaceHori()
{
	if(m_Manager.GetUiTracker()->GetSize() <= 2)	return;

	//创建一个从左到右的数组
	CArray<CUITrackerMuliti::CTrackerElement*, CUITrackerMuliti::CTrackerElement*> arrTracker;
	arrTracker.Copy(m_Manager.GetUiTracker()->m_arrTracker);
	for (int i=0; i<arrTracker.GetSize(); i++)
	{
		for (int j=i+1; j<arrTracker.GetSize(); j++)
		{
			CRect rc1 = arrTracker[i]->m_pControl->GetPos();
			CRect rc2 = arrTracker[j]->m_pControl->GetPos();

			if(rc1.left > rc2.left)
			{
				CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
				arrTracker[i] = arrTracker[j];
				arrTracker[j] = elem;
			}
		}
	}

	//取得第一个和最后一个控件的位置
	CUITrackerMuliti::CTrackerElement *pFirstElem = arrTracker[0];
	CRect rcElemFirst = pFirstElem->m_pControl->GetPos();

	CUITrackerMuliti::CTrackerElement *pLastElem = arrTracker[arrTracker.GetSize()-1];
	CRect rcElemLast = pLastElem->m_pControl->GetPos();

	//计算控件占用的空间
	int nWidthAll = 0;
	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();
		nWidthAll += rcElem.Width();
	}

	//控件的间隔
	int nSpace = (rcElemLast.right - rcElemFirst.left - nWidthAll) / (arrTracker.GetSize()-1);

	//调整中间控件的位置
	for (int i=1; i<m_Manager.GetUiTracker()->GetSize()-1; i++)
	{
		CRect rcPre = ((CControlUI *)arrTracker[i-1]->m_node.get_tag())->GetPos();

		CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CRect rc;
		rc.left = rcPre.right + nSpace;
		rc.right = rc.left + rcElem.Width();
		rc.top	 = rcElem.top;
		rc.bottom = rcElem.bottom;
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
		m_Manager.UpdateControlUI(elem->m_node,  attr);
	}
}


void CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceHori(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 2);
}

void CDuiEditorViewDesign::OnFormatSameWidth()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rc.left + rcFocus.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatSameWidth(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatSameHeight()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcElem.top;
			rc.bottom	= rc.top + rcFocus.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatSameHeight(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatSameSize()
{
	CRect rcFocus = m_Manager.GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<m_Manager.GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = m_Manager.GetUiTracker()->m_arrTracker[i];
		if(elem != m_Manager.GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rc.left + rcFocus.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rc.top + rcFocus.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	this);
			m_Manager.UpdateControlUI(elem->m_node,  attr);
		}
	}
}


void CDuiEditorViewDesign::OnUpdateFormatSameSize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Manager.GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnEditZoomIn()
{
	if(m_zoom >= 400)	return;
	m_zoom += 20;

	int dpi = MulDiv(96, m_zoom, 100);
	m_Manager.SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomIn(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnEditZoomOut()
{
	if(m_zoom <= 20)	return;
	m_zoom -= 20;

	int dpi = MulDiv(96, m_zoom, 100);
	m_Manager.SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomOut(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnEditZoomFull()
{
	m_zoom = 100;

	int dpi = 96;
	m_Manager.SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomFull(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
// 	if(m_Manager.GetManager())
// 	{
// 		m_Manager.GetUiWindow()->SendMessage(WM_SIZE);
// 	}
}


LRESULT CDuiEditorViewDesign::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_SIZE)
	{
		//InsertMsg(_T("WM_SIZE"));
		if(m_Manager.m_pUiWindow && m_Manager.m_pUiWindow->GetSafeHwnd())
		{
			::PostMessage(m_Manager.m_pUiWindow->GetSafeHwnd(), message, wParam, lParam);
		}
	}

	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_WINDOWPOSCHANGED)
	{
		//InsertMsg(_T("WM_WINDOWPOSCHANGED"));
		if(m_Manager.m_pUiWindow && m_Manager.m_pUiWindow->GetSafeHwnd())
		{
			::PostMessage(m_Manager.m_pUiWindow->GetSafeHwnd(), message, wParam, lParam);
		}
	}

	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_SHOWWINDOW)
	{
		//InsertMsg(_T("WM_SHOWWINDOW"));
		if(m_Manager.m_pUiWindow && m_Manager.m_pUiWindow->GetSafeHwnd())
		{
			::PostMessage(m_Manager.m_pUiWindow->GetSafeHwnd(), message, wParam, lParam);
		}
	}
	return CScrollView::WindowProc(message, wParam, lParam);
}


BOOL CDuiEditorViewDesign::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: 在此添加专用代码和/或调用基类
	Invalidate();
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}


void CDuiEditorViewDesign::OnFormatInsertNormalControl()
{
	m_nFormatInsert = 0;
}


void CDuiEditorViewDesign::OnUpdateFormatInsertNormalControl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nFormatInsert==0);
}


void CDuiEditorViewDesign::OnFormatInsertFloatControl()
{
	m_nFormatInsert = 1;
}


void CDuiEditorViewDesign::OnUpdateFormatInsertFloatControl(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nFormatInsert==1);
}


void CDuiEditorViewDesign::OnFormatShowRulerbar()
{
	m_bViewRuleBar = !m_bViewRuleBar;

	CRect rc;
	m_Manager.GetUiWindow()->GetWindowRect(rc);

	if(m_bViewRuleBar)
		m_Manager.GetUiWindow()->MoveWindow(RULEBAR_SIZE_X, RULEBAR_SIZE_Y, rc.Width(), rc.Height(), TRUE);
	else
		m_Manager.GetUiWindow()->MoveWindow(0, 0, rc.Width(), rc.Height(), TRUE);

	m_Manager.SetScrollSize();
	Invalidate();
}


void CDuiEditorViewDesign::OnUpdateFormatShowRulerbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewRuleBar);
}


void CDuiEditorViewDesign::OnFormatShowGrid()
{
	m_bViewGrid = !m_bViewGrid;
	Invalidate();
}


void CDuiEditorViewDesign::OnUpdateFormatShowGrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewGrid);
}


void CDuiEditorViewDesign::OnFormatShowMouse()
{
	m_bViewMouse = !m_bViewMouse;
}


void CDuiEditorViewDesign::OnUpdateFormatShowMouse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewMouse);
}


void CDuiEditorViewDesign::OnFormatShowUiPreview()
{
	if(GetDocument()->m_bMenuWnd) //菜单不允许预览
	{
		m_bShowUiPreview = FALSE;
		return;
	}

	m_bShowUiPreview = !m_bShowUiPreview;
	if(m_bShowUiPreview)
	{
		m_Manager.GetUiTracker()->RemoveAll();
		Invalidate();
	}
}


void CDuiEditorViewDesign::OnUpdateFormatShowUiPreview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowUiPreview);
}


void CDuiEditorViewDesign::OnSetDPI(UINT id)
{
	switch (id)
	{
	case ID_EDIT_DPI_100:
		m_Manager.SetZoom(96);
		break;
	case ID_EDIT_DPI_125:
		m_Manager.SetZoom(120);
		break;
	case ID_EDIT_DPI_150:
		m_Manager.SetZoom(144);
		break;
	case ID_EDIT_DPI_175:
		m_Manager.SetZoom(168);
		break;
	case ID_EDIT_DPI_200:
		m_Manager.SetZoom(192);
		break;
	}
}

void CDuiEditorViewDesign::OnUpdateSetDPI(CCmdUI *pCmdUI)
{

}

void CDuiEditorViewDesign::OnFileSaveTemplate()
{
	CImage image;
	CControlUI *pRoot = m_Manager.m_pUiWindow->GetManager()->GetRoot();
	CSize szForm = m_Manager.m_pUiWindow->GetManager()->GetInitSize();
	image.Create(szForm.cx, szForm.cy, 32);
	CRect rcPaint(0,0,szForm.cx,szForm.cy);
	pRoot->DoPaint(image.GetDC(), rcPaint, NULL);


	CDlgTemplateSave dlg;
	dlg.m_pDoc = m_Manager.GetDocument();
	dlg.m_staPicture.SetPreviewImage(image);
	image.ReleaseDC();

	dlg.DoModal();

}


void CDuiEditorViewDesign::OnUpdateFileSaveTemplate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}
