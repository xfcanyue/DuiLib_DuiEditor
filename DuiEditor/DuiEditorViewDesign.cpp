
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
#include "UITrackerMuliti.h"

#include "DockControlTreeWnd.h"

#include "UIManager.h"

#include "DlgTemplateSave.h"
#include "UIDlgLangTextEdit.h"
#include "DlgCreateStyleString.h"

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

	ON_COMMAND(ID_EDIT_GENERATE_FINDCONTROL, &CDuiEditorViewDesign::OnEditGenerateCode_FindControl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_FINDCONTROL, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_FindControl)
	ON_COMMAND(ID_EDIT_GENERATE_BINDCONTROL, &CDuiEditorViewDesign::OnEditGenerateCode_BindControl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_BINDCONTROL, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_BindControl)

	ON_COMMAND(ID_EDIT_GENERATE_DDXTEXT, &CDuiEditorViewDesign::OnEditGenerateCode_ddxText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_DDXTEXT, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxText)
	ON_COMMAND(ID_EDIT_GENERATE_DDXCHECKBOX, &CDuiEditorViewDesign::OnEditGenerateCode_ddxCheckBox)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_DDXCHECKBOX, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxCheckBox)
	ON_COMMAND(ID_EDIT_GENERATE_DDXCOMBO, &CDuiEditorViewDesign::OnEditGenerateCode_ddxCombo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_DDXCOMBO, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxCombo)
	ON_COMMAND(ID_EDIT_GENERATE_DDXCOMBOITEMDATA, &CDuiEditorViewDesign::OnEditGenerateCode_ddxComboItemData)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_DDXCOMBOITEMDATA, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxComboItemData)

	ON_COMMAND(ID_EDIT_GENERATE_ISCONTROL, &CDuiEditorViewDesign::OnEditGenerateCode_IsControl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_ISCONTROL, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_IsControl)
	ON_COMMAND(ID_EDIT_GENERATE_ISMENUCOMMAND, &CDuiEditorViewDesign::OnEditGenerateCode_IsMenuCommand)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GENERATE_ISMENUCOMMAND, &CDuiEditorViewDesign::OnUpdateEditGenerateCode_IsMenuCommand)

	ON_COMMAND(ID_EDIT_CREATE_STYLE_STRING, &CDuiEditorViewDesign::OnEditCreateStyleString)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATE_STYLE_STRING, &CDuiEditorViewDesign::OnUpdateEditCreateStyleString)


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
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_CREATE_RESOURCE_ID, &CDuiEditorViewDesign::OnEditCreateResourceId)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CREATE_RESOURCE_ID, &CDuiEditorViewDesign::OnUpdateEditCreateResourceId)
	ON_COMMAND(ID_EDIT_CREATE_RESOURCEID_AUTO, &CDuiEditorViewDesign::OnEditCreateResourceidAuto)
	ON_COMMAND(ID_EDIT_LANG_TEXT, &CDuiEditorViewDesign::OnEditLangText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LANG_TEXT, &CDuiEditorViewDesign::OnUpdateEditLangText)

	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_STYLE_PROPERTY, &CDuiEditorViewDesign::OnUpdateStyleList)
	ON_COMMAND_EX_RANGE(ID_STYLE_LIST1, ID_STYLE_LIST32, &CDuiEditorViewDesign::OnStyleListRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_STYLE_LIST1, ID_STYLE_LIST32, &CDuiEditorViewDesign::OnUpdateStyleListRange)

	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_FONT_PROPERTY, &CDuiEditorViewDesign::OnUpdateFontList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_HOTFONT_PROPERTY, &CDuiEditorViewDesign::OnUpdateHotFontList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_PUSHEDFONT_PROPERTY, &CDuiEditorViewDesign::OnUpdatePushedFontList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_FOCUSEDFONT_PROPERTY, &CDuiEditorViewDesign::OnUpdateFocusedFontList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_SELECTEDFONT_PROPERTY, &CDuiEditorViewDesign::OnUpdateSelectedFontList)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_ITEMFONT_PROPERTY, &CDuiEditorViewDesign::OnUpdateItemFontList)
	ON_COMMAND_EX_RANGE(ID_FONT_LIST1, ID_FONT_LIST192, &CDuiEditorViewDesign::OnFontListRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FONT_LIST1, ID_FONT_LIST192, &CDuiEditorViewDesign::OnUpdateFontListRange)
END_MESSAGE_MAP()

// CDuiEditorView 构造/析构

CDuiEditorViewDesign::CDuiEditorViewDesign()
{
	m_pUIManager = NULL;
	m_zoom = 100;

	m_nFormatInsert = 0;
	m_bShowUiPreview = FALSE;		//是否在编辑界面显示UI效果
	m_bViewRuleBar	= TRUE;			//是否显示标尺
	m_bViewGrid		= FALSE;		//是否显示网格
	m_bViewMouse	= FALSE;		//是否显示鼠标线

	ZeroMemory( &m_piProcInfo, sizeof(PROCESS_INFORMATION) );
	ZeroMemory( &m_siStartInfo, sizeof(STARTUPINFO) );
	_nMaxResourceID = 0;
}

CDuiEditorViewDesign::~CDuiEditorViewDesign()
{
}

// CDuiEditorView 绘制

void CDuiEditorViewDesign::OnDraw(CDC* pDrawDC)
{
}


int CDuiEditorViewDesign::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetUIManager(GetDocument()->GetUIManager());
	GetUIManager()->_setDesignView(this);

	CUIWindow *pUiWindow = new CUIWindowDesignView;
	pUiWindow->SetUIManager(GetUIManager());
	pUiWindow->CreateUiWindow(this->m_hWnd, _T("ViewDesign"), UI_WNDSTYLE_CHILD, 0);

	CRect rc;
	pUiWindow->GetWindowRect(rc);
	if(m_bViewRuleBar)
		pUiWindow->MoveWindow(RULEBAR_SIZE_X, RULEBAR_SIZE_Y, rc.Width(), rc.Height(), TRUE);
	else
		pUiWindow->MoveWindow(0, 0, rc.Width(), rc.Height(), TRUE);

	GetUIManager()->_setUIWindow(pUiWindow);
	GetUIManager()->_setManager(pUiWindow->GetManager());

	GetUIManager()->SetScrollSize();

	if(!g_cfg.bDesignerShowShadow)
	{
		GetUIManager()->GetManager()->GetShadow()->DisableShadow(true);
	}
	return 0;
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
	GetUIManager()->GetUiFrom()->RemoveAll();

	CDuiEditorDoc *pDoc = GetUIManager()->GetDocument();
	//GetUIManager()->GetManager()->SetResourcePath(pDoc->GetSkinPath());

	CUIBuilder builder;
	CControlUI* pRoot = builder.Create(pDoc->m_doc, NULL, GetUIManager()->GetManager());
	if (pRoot==NULL)
	{
		//::MessageBox(m_hWnd, _T("Loading resource files error."), _T("Duilib"), MB_OK|MB_ICONERROR);
		//return 0;
	}

	xml_node nodeWindow = pDoc->m_doc.child(XTEXT("Window"));
	GetUIManager()->GetUiFrom()->SetTag((UINT_PTR)nodeWindow.internal_object());
	nodeWindow.set_tag((UINT_PTR)GetUIManager()->GetUiFrom());
	GetUIManager()->GetUiFrom()->Add(pRoot);

	CSize sz = GetUIManager()->GetManager()->GetInitSize();
	GetUIManager()->GetUiFrom()->SetInitSize(sz.cx, sz.cy);
	GetUIManager()->SetScrollSize();

	GetUIManager()->GetTreeView()->InitTreeContent();

	//设置定时器，刷新一下duilib窗口，不然duilib会抢焦点
	//SetTimer(1, 100, NULL);
}

void CDuiEditorViewDesign::OnDestroy()
{
	CScrollView::OnDestroy();

	if(GetUIManager()->GetUiWindow())
	{
		GetUIManager()->GetUiWindow()->DestroyUiWindow();
		delete GetUIManager()->GetUiWindow();
		GetUIManager()->_setUIWindow(NULL);
		GetUIManager()->_setManager(NULL);
	}
}

void CDuiEditorViewDesign::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//	InsertMsg(_T("CDuiEditorViewDesign::OnContextMenu"));
	if(!g_pToolBox->IsDefaultPoint())
	{
		CUIWindowDesignView *pUIWindow = (CUIWindowDesignView *)GetUIManager()->GetUiWindow();
		if(!pUIWindow->m_rcHot.IsRectEmpty())
		{
			pUIWindow->m_rcHot.SetRectEmpty();
			pUIWindow->Invalidate();
		}
		g_pToolBox->SetDefaultPoint();
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		return;
	}

	CPaintManagerUI *ppm = GetUIManager()->GetManager();
	xml_node root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Window"));
	if(!root) return;
	
	_nMaxResourceID = 0;
	GetMaxResourceID(root, _nMaxResourceID);

	CMenu popmenu;
	popmenu.LoadMenu(IDR_POPUP_EDIT);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, FALSE);
	return;
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
	if(GetUIManager()->GetUiWindow())
		GetUIManager()->GetUiWindow()->GetWindowRect(rcUiWnd);
	if(rectClient.Width() - rcUiWnd.Width() > 0) //刷新UI窗口右边的空白区域
	{
		CRect rc;
		if(m_bViewRuleBar)
			rc.SetRect(RULEBAR_SIZE_X+rcUiWnd.Width(), 0, rectClient.Width(), rectClient.Height());
		else
			rc.SetRect(rcUiWnd.Width(), 0, rectClient.Width(), rectClient.Height());


		pDC->FillSolidRect(rc, g_cfg.crDesignerBkColor);
	}

	if(rectClient.Height() - rcUiWnd.Height())	//刷新UI窗口下边的空白区域
	{
		CRect rc;
		if(m_bViewRuleBar)
			rc.SetRect(0, RULEBAR_SIZE_Y+rcUiWnd.Height(), rectClient.Width(), rectClient.Height());
		else
			rc.SetRect(0, rcUiWnd.Height(), rectClient.Width(), rectClient.Height());

		pDC->FillSolidRect(rc, g_cfg.crDesignerBkColor);
	}

	if(m_bViewRuleBar)
	{
		CRect rc1(0,0, rectClient.Width(), RULEBAR_SIZE_Y);
		pDC->FillSolidRect(rc1,g_cfg.crDesignerBkColor);

		CRect rc2(0,0, RULEBAR_SIZE_Y, rectClient.Height());
		pDC->FillSolidRect(rc2,g_cfg.crDesignerBkColor);

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

void CDuiEditorViewDesign::ShowShadowUI(CDuiEditorViewDesign *pView, BOOL bShow)
{
// 	CString temp;
// 	temp.Format(_T("%s, show=%s"), pView->GetDocument()->GetSkinFileName(), bShow?_T("是"):_T("否"));
// 	InsertMsg(temp);

// 	if(pView->m_Manager.m_pUiWindow && m_Manager.m_pUiWindow->GetSafeHwnd())
// 	{
// 		CShadowUI *pShadowUI = pView->m_Manager.GetManager()->GetShadow();
// 		if(bShow)
// 		{
// 			if(pShadowUI->IsShowShadow() && pShadowUI->IsDisableShadow() )
// 			{
// 				pShadowUI->DisableShadow(false);
// 			}
// 		}
// 		else
// 		{
// 			if(pShadowUI->IsShowShadow() && !pShadowUI->IsDisableShadow() )
// 			{
// 				pShadowUI->DisableShadow(true);
// 			}
// 		}
// 	}
}

void CDuiEditorViewDesign::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	//if(bActivate) InsertMsg(_T("CDuiEditorViewDesign::OnActivateView"));
	/*
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	if(bActivate && pActivateView==this && pDeactiveView!=this)
	{
		pMain->m_wndControl.SetActiveTreeView(GetUIManager()->GetTreeView());
		pMain->m_wndProperty.SetActivePropList(GetUIManager()->GetPropList());
		pMain->ShowAllPane();

		//主要是为了更新DuiLib的窗口阴影
		if(GetUIManager()->GetUiWindow() && GetUIManager()->GetUiWindow()->GetSafeHwnd())
		{
			::PostMessage(GetUIManager()->GetUiWindow()->GetSafeHwnd(), WM_SHOWWINDOW, bActivate, 0);
		}
	}
	*/
// 	CString temp;
// 	temp.Format(_T("%s, show=%s"), GetDocument()->GetSkinFileName(), bActivate?_T("是"):_T("否"));
// 	InsertMsg(temp);

// 	CDuiEditorViewDesign *pView = DYNAMIC_DOWNCAST(CDuiEditorViewDesign, pActivateView);
// 	if(!pView) return;
// 	ShowShadowUI(pView, bActivate);

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDuiEditorViewDesign::OnLButtonDown(UINT nFlags, CPoint pt)
{
	xml_node node((xml_node_struct *)GetUIManager()->GetUiFrom()->GetTag());

	GetUIManager()->GetPropList()->InitProp(node);
	GetUIManager()->SelectItem(node);
	GetUIManager()->GetTreeView()->SelectXmlNode(node);
	GetUIManager()->GetCodeView()->SelectXmlNode(node);

	//切换左边控件树
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_wndControl.IsPaneVisible())
	{
		pMain->m_wndControl.ShowPane(TRUE, TRUE, TRUE);
	}
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
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		GetUIManager()->DeleteControl(pTrackElem->m_node);
	}
	GetUIManager()->GetUiTracker()->RemoveAll();
	//GetDocument()->SetModifiedFlag(TRUE);
}

void CDuiEditorViewDesign::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::CopyToClipboard(LPCTSTR strText)
{
	if(OpenClipboard())
	{
		EmptyClipboard();

		HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE,   (_tcslen(strText)+1) * sizeof(TCHAR));
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
}

void CDuiEditorViewDesign::OnEditCopyName()
{
	for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CString strText = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CopyToClipboard(strText);
		break;	//只复制第一个control
	}
}

void CDuiEditorViewDesign::OnUpdateEditCopyName(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditCopyNameEx()
{
	for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CString strText = _T("_T(\"");
		strText += pTrackElem->m_node.attribute(XTEXT("name")).as_string();
		strText += _T("\")");

		CopyToClipboard(strText);
		break;	//只复制第一个control
	}
}

void CDuiEditorViewDesign::OnUpdateEditCopyNameEx(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_FindControl()
{
	//期望创建的程序代码
	//statc_cast<CControlUI *>(GetManager()->FindControl(_T("..."))

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sClassName = _T("C");
		sClassName += XML2T(pTrackElem->m_node.name());
		sClassName += _T("UI");

		CString sVarName = _T("*p");
		sVarName += XML2T(pTrackElem->m_node.name());

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("%s %s = static_cast<%s *>(GetManager()->FindControl(_T(\"%s\")));\r\n"), sClassName, sVarName, sClassName, sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_FindControl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_BindControl()
{
	//期望创建的程序代码
	//UI_BINDCONTROL(CComboExUI, m_pControl, _T("control_name"));

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sClassName = _T("C");
		sClassName += XML2T(pTrackElem->m_node.name());
		sClassName += _T("UI");

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("UI_BINDCONTROL(%s, m_, _T(\"%s\"));\r\n"), sClassName, sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_BindControl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_ddxText()
{
	//期望创建的程序代码
	//ddxText(_T("..."), var);

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("ddxText(_T(\"%s\"), m_obj.);\r\n"), sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxText(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_ddxCheckBox()
{
	//期望创建的程序代码
	//ddxCheckBox(_T("..."), var);

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("ddxCheckBox(_T(\"%s\"), m_obj.);\r\n"), sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxCheckBox(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_ddxCombo()
{
	//期望创建的程序代码
	//ddxCombo(_T("..."), var);

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("ddxCombo(_T(\"%s\"), m_obj.);\r\n"), sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxCombo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_ddxComboItemData()
{
	//期望创建的程序代码
	//ddxComboItemData(_T("..."), var);

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];

		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("ddxComboItemData(_T(\"%s\"), m_obj.);\r\n"), sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_ddxComboItemData(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_IsControl()
{
	//期望创建的程序代码
	//IsControl(msg, _T("..."));

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		if(i > 0) strMuiltiText += _T("\r\n\r\n");

		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("if(IsControl(msg, _T(\"%s\")))\r\n{\r\n\treturn;\r\n}"), sControlName);
		strMuiltiText += strText;
	}

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_IsControl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditGenerateCode_IsMenuCommand()
{
	//期望创建的程序代码
	//IsMenuCommand(cmd, _T("..."));

	CString strMuiltiText;

	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	for (int i=0; i<count; i++)
	{
		if(i > 0) strMuiltiText += _T("\r\n\r\n");

		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CString sControlName = XML2T(pTrackElem->m_node.attribute(XTEXT("name")).as_string());

		CString strText;
		strText.Format(_T("if(IsMenuCommand(cmd, _T(\"%s\")))\r\n{\r\n\treturn;\r\n}"), sControlName);
		strMuiltiText += strText;
	}	

	CopyToClipboard(strMuiltiText);
}

void CDuiEditorViewDesign::OnUpdateEditGenerateCode_IsMenuCommand(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize()>0);
}

void CDuiEditorViewDesign::OnEditCreateStyleString()
{
	int count = GetUIManager()->GetUiTracker()->m_arrTracker.GetSize();
	if(count != 1) return;

	CDlgCreateStyleString dlg;
	CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[0];
	dlg.m_node = pTrackElem->m_node;
	dlg.DoModal();
}

void CDuiEditorViewDesign::OnUpdateEditCreateStyleString(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() == 1);
}

void CDuiEditorViewDesign::OnCommandTabLayoutSetSel(UINT nID)
{
	int nSel = nID - ID_TABLAYOUT_SETSEL_00;

	HTREEITEM hItem = GetUIManager()->GetTreeView()->GetSelectedItem();
	if(hItem)
	{
		xml_node node((xml_node_struct *)GetUIManager()->GetTreeView()->GetItemData(hItem));
		if(node)
		{
			CTabLayoutUI *pTab = (CTabLayoutUI *)node.get_tag();
			if(pTab->GetInterface(DUI_CTR_TABLAYOUT))
			{
				g_duiProp.AddAttribute(node,  _T("selectedid"), nSel,	GetUIManager());
				GetUIManager()->UpdateControlUI(node, node.attribute(XTEXT("selectedid")));
			}
		}
	}
}


void CDuiEditorViewDesign::OnFormatAlignLeft()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcFocus.left;
			rc.right	= rc.left + rcElem.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignLeft(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignRight()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem =GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcFocus.right - rcElem.Width();
			rc.right	= rcFocus.right;
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignRight(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignTop()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcFocus.top;
			rc.bottom	= rcFocus.top + rcElem.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignTop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatAlignBottom()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcFocus.bottom - rcElem.Height();
			rc.bottom	= rcFocus.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignBottom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}

void CDuiEditorViewDesign::OnFormatAlignCenterVert()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CControlUI *pParent = elem->m_pControl->GetParent();
		if(!pParent) continue;
		CRect rcParent = pParent->GetPos();

		CRect rc;
		rc.left		= rcElem.left;
		rc.right	= rc.left + rcElem.Width();
		rc.top		= rcParent.top + (rcParent.Height() - rcElem.Height())/2;
		rc.bottom	= rc.top + rcElem.Height();
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
		GetUIManager()->UpdateControlUI(elem->m_node, attr);
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignCenterVert(CCmdUI *pCmdUI)
{

}


void CDuiEditorViewDesign::OnFormatAlignCenterHori()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	//多选和单选的居中, 是不一样的
	if(GetUIManager()->GetUiTracker()->GetSize() > 1)
	{

	}
	else
	{

	}

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CControlUI *pParent = elem->m_pControl->GetParent();
		if(!pParent) continue;
		CRect rcParent = pParent->GetPos();

		CRect rc;
		rc.left		= rcParent.left + (rcParent.Width() - rcElem.Width())/2;
		rc.right	= rc.left + rcElem.Width();
		rc.top		= rcElem.top;
		rc.bottom	= rc.top + rcElem.Height();
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
		GetUIManager()->UpdateControlUI(elem->m_node,  attr);
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignCenterHori(CCmdUI *pCmdUI)
{

}

void CDuiEditorViewDesign::OnFormatAlignSameSpaceVert()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	if(GetUIManager()->GetUiTracker()->GetSize() <= 2)	return;

	//创建一个从上到下的数组
	CArray<CUITrackerMuliti::CTrackerElement*, CUITrackerMuliti::CTrackerElement*> arrTracker;
	arrTracker.Copy(GetUIManager()->GetUiTracker()->m_arrTracker);
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

	//调整中间控件的位置, 最后一个控件也要调整位置，因为控件间隔余数的问题。
	for (int i=1; i<arrTracker.GetSize(); i++)
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
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
		GetUIManager()->UpdateControlUI(elem->m_node,  attr);
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceVert(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 2);
}


void CDuiEditorViewDesign::OnFormatAlignSameSpaceHori()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	if(GetUIManager()->GetUiTracker()->GetSize() <= 2)	return;

	//创建一个从左到右的数组
	CArray<CUITrackerMuliti::CTrackerElement*, CUITrackerMuliti::CTrackerElement*> arrTracker;
	arrTracker.Copy(GetUIManager()->GetUiTracker()->m_arrTracker);
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
	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();
		nWidthAll += rcElem.Width();
	}

	//控件的间隔
	int nSpace = (rcElemLast.right - rcElemFirst.left - nWidthAll) / (arrTracker.GetSize()-1);

	//调整中间控件的位置
	for (int i=1; i<GetUIManager()->GetUiTracker()->GetSize()-1; i++)
	{
		CRect rcPre = ((CControlUI *)arrTracker[i-1]->m_node.get_tag())->GetPos();

		CUITrackerMuliti::CTrackerElement *elem = arrTracker[i];
		CRect rcElem = elem->m_pControl->GetPos();

		CRect rc;
		rc.left = rcPre.right + nSpace;
		rc.right = rc.left + rcElem.Width();
		rc.top	 = rcElem.top;
		rc.bottom = rcElem.bottom;
		xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
		GetUIManager()->UpdateControlUI(elem->m_node,  attr);
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatAlignSameSpaceHori(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 2);
}

void CDuiEditorViewDesign::OnFormatSameWidth()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rc.left + rcFocus.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rcElem.bottom;
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatSameWidth(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatSameHeight()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rcElem.right;
			rc.top		= rcElem.top;
			rc.bottom	= rc.top + rcFocus.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatSameHeight(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnFormatSameSize()
{
	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	CRect rcFocus = GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetPos();

	for (int i=0; i<GetUIManager()->GetUiTracker()->GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *elem = GetUIManager()->GetUiTracker()->m_arrTracker[i];
		if(elem != GetUIManager()->GetUiTracker()->m_pFocused)
		{
			CRect rcElem = elem->m_pControl->GetPos();

			CRect rc;
			rc.left		= rcElem.left;
			rc.right	= rc.left + rcFocus.Width();
			rc.top		= rcElem.top;
			rc.bottom	= rc.top + rcFocus.Height();
			xml_attribute attr = g_duiProp.AddAttribute(elem->m_node, _T("pos"), RectToString(&rc),	GetUIManager());
			GetUIManager()->UpdateControlUI(elem->m_node,  attr);
		}
	}
	GetUIManager()->GetUiTracker()->RefreshRect();
}


void CDuiEditorViewDesign::OnUpdateFormatSameSize(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetUiTracker()->GetSize() > 1);
}


void CDuiEditorViewDesign::OnEditZoomIn()
{
	if(m_zoom >= 400)	return;
	m_zoom += 20;

	CLockWindowUpdate lock(this);
	int dpi = MulDiv(96, m_zoom, 100);
	GetUIManager()->SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomIn(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnEditZoomOut()
{
	if(m_zoom <= 20)	return;
	m_zoom -= 20;

	CLockWindowUpdate lock(this);
	int dpi = MulDiv(96, m_zoom, 100);
	GetUIManager()->SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomOut(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnEditZoomFull()
{
	m_zoom = 100;

	CLockWindowUpdate lock(this);
	int dpi = 96;
	GetUIManager()->SetZoom(dpi);
}


void CDuiEditorViewDesign::OnUpdateEditZoomFull(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewDesign::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!GetUIManager()) return;
	if(!GetUIManager()->GetUiFrom()) return;
	if(cx == 0 || cy == 0) 
		return;

	CString strSize = XML2T(GetUIManager()->GetDocument()->m_doc.child(XTEXT("Window")).attribute(XTEXT("size")).value());
	CDuiSize szForm(strSize);
	if(szForm.cx <= 0 || szForm.cy <= 0)
	{
		GetUIManager()->SetUIFormWindowSize(szForm.cx, szForm.cy);
	}
}


LRESULT CDuiEditorViewDesign::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_SIZE)
	{
		//InsertMsg(_T("WM_SIZE"));
		if(GetUIManager()->GetUiWindow() && GetUIManager()->GetUiWindow()->GetSafeHwnd())
		{
			::PostMessage(GetUIManager()->GetUiWindow()->GetSafeHwnd(), message, wParam, lParam);
		}
	}

	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_WINDOWPOSCHANGED)
	{
		//InsertMsg(_T("WM_WINDOWPOSCHANGED"));
		if(GetUIManager()->GetUiWindow() && GetUIManager()->GetUiWindow()->GetSafeHwnd())
		{
			::PostMessage(GetUIManager()->GetUiWindow()->GetSafeHwnd(), message, wParam, lParam);
		}
	}

	//主要是为了更新DuiLib的窗口阴影
	if(message == WM_SHOWWINDOW)
	{
		//InsertMsg(_T("WM_SHOWWINDOW"));
		if(GetUIManager()->GetUiWindow() && GetUIManager()->GetUiWindow()->GetSafeHwnd())
		{
			::PostMessage(GetUIManager()->GetUiWindow()->GetSafeHwnd(), message, wParam, lParam);
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
	GetUIManager()->GetUiWindow()->GetWindowRect(rc);

	if(m_bViewRuleBar)
		GetUIManager()->GetUiWindow()->MoveWindow(RULEBAR_SIZE_X, RULEBAR_SIZE_Y, rc.Width(), rc.Height(), TRUE);
	else
		GetUIManager()->GetUiWindow()->MoveWindow(0, 0, rc.Width(), rc.Height(), TRUE);

	GetUIManager()->SetScrollSize();
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
		GetUIManager()->GetUiTracker()->RemoveAll();
		Invalidate();
	}
}


void CDuiEditorViewDesign::OnUpdateFormatShowUiPreview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bShowUiPreview);
}


void CDuiEditorViewDesign::OnSetDPI(UINT id)
{
	CLockWindowUpdate lock(this);
	switch (id)
	{
	case ID_EDIT_DPI_100:
		GetUIManager()->SetZoom(96);
		break;
	case ID_EDIT_DPI_125:
		GetUIManager()->SetZoom(120);
		break;
	case ID_EDIT_DPI_150:
		GetUIManager()->SetZoom(144);
		break;
	case ID_EDIT_DPI_175:
		GetUIManager()->SetZoom(168);
		break;
	case ID_EDIT_DPI_200:
		GetUIManager()->SetZoom(192);
		break;
	}
}

void CDuiEditorViewDesign::OnUpdateSetDPI(CCmdUI *pCmdUI)
{

}

void CDuiEditorViewDesign::OnFileSaveTemplate()
{
	CImage image;
	CControlUI *pRoot = GetUIManager()->GetManager()->GetRoot();
	CSize szForm = GetUIManager()->GetManager()->GetInitSize();
	image.Create(szForm.cx, szForm.cy, 32);
	CRect rcPaint(0,0,szForm.cx,szForm.cy);
	pRoot->DoPaint(image.GetDC(), rcPaint, NULL);


	CDlgTemplateSave dlg;
	dlg.m_pDoc = GetUIManager()->GetDocument();
	dlg.m_staPicture.SetPreviewImage(image);
	image.ReleaseDC();

	dlg.DoModal();

}


void CDuiEditorViewDesign::OnUpdateFileSaveTemplate(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}



void CDuiEditorViewDesign::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(GetUIManager()->GetUiWindow()->GetSafeHwnd(), &pt);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CString temp;
	temp.Format(_T("坐标: %d, %d"), pt.x, pt.y);
	pMain->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURSOR_POS, temp);

	CScrollView::OnMouseMove(nFlags, point);
}


void CDuiEditorViewDesign::OnSetFocus(CWnd* pOldWnd)
{
	CScrollView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	//InsertMsg(_T("CDuiEditorViewDesign::OnSetFocus"));
}


BOOL CDuiEditorViewDesign::GetMaxResourceID(xml_node root, int &nMaxResourceID)
{
	int resourceid = root.attribute(XTEXT("resourceid")).as_int();
	if(resourceid > nMaxResourceID)
	{
		nMaxResourceID = resourceid;
	}

	for (xml_node node=root.first_child(); node; node=node.next_sibling())
	{
		GetMaxResourceID(node, nMaxResourceID);
	}
	
	return TRUE;
}

void CDuiEditorViewDesign::OnEditCreateResourceId()
{
	_nMaxResourceID++;
	CString strMaxID; 
	strMaxID.Format(_T("%d"), _nMaxResourceID);

	CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[0];
	g_duiProp.AddAttribute(pTrackElem->m_node, _T("resourceid"), strMaxID, GetUIManager());
	pTrackElem->m_pControl->SetAttribute(_T("resourceid"), strMaxID);
}


void CDuiEditorViewDesign::OnUpdateEditCreateResourceId(CCmdUI *pCmdUI)
{
	if(GetUIManager())
	{
		if(GetUIManager()->GetUiTracker()->m_arrTracker.GetSize() != 1)
		{
			pCmdUI->SetText(_T("生成ResourceID 请选择单个控件"));
			pCmdUI->Enable(FALSE);
			return;
		}
		
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker[0];
		int resourceid = pTrackElem->m_node.attribute(XTEXT("resourceid")).as_int();
		if(resourceid > 0)
		{
			CString temp;
			temp.Format(_T("生成ResourceID : 已设置当前ID=%d"), resourceid);
			pCmdUI->SetText(temp);
			pCmdUI->Enable(FALSE);
			return;
		}

		CString temp;
		temp.Format(_T("生成ResourceID : 最大值ID=%d"), pTrackElem->m_pControl->GetResourceID());
		pCmdUI->Enable(TRUE);
		return;
	}	
	pCmdUI->Enable(FALSE);
}


void CDuiEditorViewDesign::OnEditCreateResourceidAuto()
{
	xml_node root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Window"));
	if(!root)
	{
		root = GetUIManager()->GetDocument()->m_doc.child(XTEXT("Menu"));
		if(!root)
		{
			AfxMessageBox(_T("只有根节点为Window或Menu才能生成语言包！"));
			return;
		}
	}
	_CreateResourceIDAuto(root);
}

void CDuiEditorViewDesign::_CreateResourceIDAuto(xml_node root)
{
	xml_attribute attrText = root.attribute(XTEXT("text"));
	xml_attribute attrTooltip = root.attribute(XTEXT("tooltip"));
	xml_attribute attrTipvalue = root.attribute(XTEXT("tipvalue"));

	int resourceid = root.attribute(XTEXT("resourceid")).as_int();
	//有这3种属性，并且还没有定义resourceid的
	if((attrText || attrTooltip || attrTipvalue) && resourceid==0)
	{
		const char *pvalue = attrText.value();
		const char *ptoolvalue = attrTooltip.value();
		const char *ptipvalue = attrTipvalue.value();
		//内容为空的也不处理
		if((pvalue && *pvalue!='\0') || (ptoolvalue && *ptoolvalue!='\0') || (ptipvalue && *ptipvalue!='\0') )
		{
			_nMaxResourceID++;
			CString strMaxID; 
			strMaxID.Format(_T("%d"), _nMaxResourceID);
			g_duiProp.AddAttribute(root, _T("resourceid"), strMaxID, GetUIManager());
		}
	}

	for (xml_node node = root.first_child(); node; node=node.next_sibling())
	{
		_CreateResourceIDAuto(node);
	}	
}


void CDuiEditorViewDesign::OnEditLangText()
{
	CDlgLangTextEditUI dlg;
	dlg.SetUIManager(GetUIManager());
	dlg.DoModal();
}


void CDuiEditorViewDesign::OnUpdateEditLangText(CCmdUI *pCmdUI)
{
	if(GetUIManager()->GetUiTracker()->m_pFocused)
		pCmdUI->Enable(GetUIManager()->GetUiTracker()->m_pFocused->m_pControl->GetResourceID() > 0);
	else
		pCmdUI->Enable(FALSE);
}


void CDuiEditorViewDesign::OnUpdateStyleList(CCmdUI *pCmdUI)
{
	if(!GetUIManager()) return;
	CPaintManagerUI *pManager = GetUIManager()->GetManager();

	int nStyle = pManager->GetStyles(false).GetSize();
	int nStyleShared = pManager->GetStyles(true).GetSize();
	if(nStyle == 0 && nStyleShared == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pSubMenu == NULL)
		return;
	CMenu* pMenu = pCmdUI->m_pSubMenu;
	pMenu->DeleteMenu(ID_EDIT_SET_STYLE_PROPERTY, MF_BYCOMMAND);
	for (UINT id=ID_STYLE_LIST1; id<=ID_STYLE_LIST32; id++)
	{
		pMenu->DeleteMenu(id, MF_BYCOMMAND);
	}
	m_arrTempStyles.RemoveAll();

	int nID = ID_STYLE_LIST1;
	for (int i=0; i<nStyle; i++)
	{
		pMenu->InsertMenu(nID, MF_STRING|MF_BYCOMMAND, nID, pManager->GetStyles(false).GetAt(i));
		m_arrTempStyles.Add(pManager->GetStyles(false).GetAt(i));

		nID++;
		if(nID > ID_STYLE_LIST32) return;
	}

	for (int i=0; i<nStyleShared; i++)
	{
		pMenu->InsertMenu(nID, MF_STRING|MF_BYCOMMAND, nID, pManager->GetStyles(true).GetAt(i));
		m_arrTempStyles.Add(pManager->GetStyles(true).GetAt(i));

		nID++;
		if(nID > ID_STYLE_LIST32) return;
	}
}

BOOL CDuiEditorViewDesign::OnStyleListRange(UINT uID)
{
	int n = uID - ID_STYLE_LIST1;
	if(n < m_arrTempStyles.GetSize())
	{
		CString styleName = m_arrTempStyles.GetAt(n);

		CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
		for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
		{
			CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker.GetAt(i);
			if(pTrackElem)
			{
				g_duiProp.AddAttribute(pTrackElem->m_node, _T("style"), styleName, GetUIManager());
				pTrackElem->m_pControl->SetAttribute(_T("style"), styleName);
			}
		}
	}
	return TRUE;
}

void CDuiEditorViewDesign::OnUpdateStyleListRange(CCmdUI *pCmdUI)
{
	int n = pCmdUI->m_nID - ID_STYLE_LIST1;
	if(n < m_arrTempStyles.GetSize())
	{
		CString styleName = m_arrTempStyles.GetAt(n);

		CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
		for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
		{
			CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker.GetAt(i);
			if(pTrackElem)
			{
				if(CompareString(pTrackElem->m_node.attribute(XTEXT("style")).as_string(), styleName))
				{
					pCmdUI->SetCheck(TRUE);
				}
			}
		}
	}
}

CString CDuiEditorViewDesign::GetFontAttributeName(UINT uFontType)
{
	switch (uFontType)
	{
	case ID_EDIT_SET_FONT_PROPERTY:
		return _T("font");
	case ID_EDIT_SET_HOTFONT_PROPERTY:
		return _T("hotfont");
	case ID_EDIT_SET_PUSHEDFONT_PROPERTY:
		return _T("pushedfont");
	case ID_EDIT_SET_FOCUSEDFONT_PROPERTY:
		return _T("focuedfont");
	case ID_EDIT_SET_SELECTEDFONT_PROPERTY:
		return _T("selectedfont");
	case ID_EDIT_SET_ITEMFONT_PROPERTY:
		return _T("itemfont");
	}
	return _T("font");
}

void CDuiEditorViewDesign::UpdateFontPropertyMenu(CCmdUI *pCmdUI, UINT uFontType)
{
	if(!GetUIManager()) return;
	CPaintManagerUI *pManager = GetUIManager()->GetManager();

	int nFont = pManager->GetCustomFontCount(false);
	int nFontShared = pManager->GetCustomFontCount(true);
	if(nFont == 0 && nFontShared == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pSubMenu == NULL)
		return;

	//resource.h定义了很多菜单项 -_-!，根据菜单类型计算每个菜单对应的ID，最多32个。
	UINT id_font_begin	= ID_EDIT_SET_FONT_PROPERTY + 6 + (uFontType - ID_EDIT_SET_FONT_PROPERTY) * 32;
	UINT id_font_end	= id_font_end = id_font_begin + 31;
	//InsertMsgV(_T("%d - %d"), id_font_begin, id_font_end);

	CMenu* pMenu = pCmdUI->m_pSubMenu;
	pMenu->DeleteMenu(uFontType, MF_BYCOMMAND);
	for (UINT id=id_font_begin; id<=id_font_end; id++)
	{
		pMenu->DeleteMenu(id, MF_BYCOMMAND);
	}

	//如果在DuiLib.xml文件中没有对应相关的font属性，则不加载菜单啊。
	CString attrName = GetFontAttributeName(uFontType);
	CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker.GetAt(0);
	if(!pTrackElem)	return;
	if(!pTrackElem->m_node) return;
	xml_node nodeAttr = g_duiProp.FindAttribute(XML2T(pTrackElem->m_node.name()), attrName);
	if(!nodeAttr) return;

	UINT nID = id_font_begin;
	tagTFontInfo *pDefaultFont = (TFontInfo *)GetUIManager()->GetManager()->GetDefaultFontInfo();
	if(pDefaultFont)
	{
		CString sMenuFontName;
		sMenuFontName.Format(_T("默认 %s size=%d "), pDefaultFont->sFontName.GetData(), pDefaultFont->iSize);
		if(pDefaultFont->bBold) sMenuFontName += _T("Bold ");
		if(pDefaultFont->bUnderline) sMenuFontName += _T("Underline ");
		if(pDefaultFont->bItalic) sMenuFontName += _T("Italic ");

		pMenu->InsertMenu(nID, MF_STRING|MF_BYCOMMAND, nID, sMenuFontName);

		tagFontMenu *p = new tagFontMenu;
		p->cmdID = nID;
		p->attrName = GetFontAttributeName(uFontType);
		p->pFontInfo = pDefaultFont;
		m_mapFontMenu[nID] = p;

		nID++;
		if(nID > id_font_end) return;
	}

	for (int i=0; i<nFont; i++)
	{
		TFontInfo *pFontInfo = pManager->GetFontInfo(i, false);
		CString sMenuFontName;
		sMenuFontName.Format(_T("id=%d %s size=%d "), pFontInfo->id, pFontInfo->sFontName.GetData(), pFontInfo->iSize);
		if(pFontInfo->bBold) sMenuFontName += _T("Bold ");
		if(pFontInfo->bUnderline) sMenuFontName += _T("Underline ");
		if(pFontInfo->bItalic) sMenuFontName += _T("Italic ");

		pMenu->InsertMenu(nID, MF_STRING|MF_BYCOMMAND, nID, sMenuFontName);

		tagFontMenu *p = new tagFontMenu;
		p->cmdID = nID;
		p->attrName = GetFontAttributeName(uFontType);
		p->pFontInfo = pFontInfo;
		m_mapFontMenu[nID] = p;

		nID++;
		if(nID > id_font_end) return;
	}

	for (int i=0; i<nFontShared; i++)
	{
		TFontInfo *pFontInfo = pManager->GetFontInfo(i, true);
		CString sMenuFontName;
		sMenuFontName.Format(_T("id=%d %s size=%d "), pFontInfo->id, pFontInfo->sFontName.GetData(), pFontInfo->iSize);
		if(pFontInfo->bBold) sMenuFontName += _T("Bold ");
		if(pFontInfo->bUnderline) sMenuFontName += _T("Underline ");
		if(pFontInfo->bItalic) sMenuFontName += _T("Italic ");
		sMenuFontName += _T("Shared");

		pMenu->InsertMenu(nID, MF_STRING|MF_BYCOMMAND, nID, sMenuFontName);

		tagFontMenu *p = new tagFontMenu;
		p->cmdID = nID;
		p->attrName = GetFontAttributeName(uFontType);
		p->pFontInfo = pFontInfo;
		m_mapFontMenu[nID] = p;

		nID++;
		if(nID > id_font_end) return;
	}
}

void CDuiEditorViewDesign::OnUpdateFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_FONT_PROPERTY);
}

void CDuiEditorViewDesign::OnUpdateHotFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_HOTFONT_PROPERTY);
}

void CDuiEditorViewDesign::OnUpdatePushedFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_PUSHEDFONT_PROPERTY);
}

void CDuiEditorViewDesign::OnUpdateFocusedFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_FOCUSEDFONT_PROPERTY);
}

void CDuiEditorViewDesign::OnUpdateSelectedFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_SELECTEDFONT_PROPERTY);
}

void CDuiEditorViewDesign::OnUpdateItemFontList(CCmdUI *pCmdUI)
{
	UpdateFontPropertyMenu(pCmdUI, ID_EDIT_SET_ITEMFONT_PROPERTY);
}

BOOL CDuiEditorViewDesign::OnFontListRange(UINT uID)
{
	std::map<UINT, tagFontMenu *>::iterator it = m_mapFontMenu.find(uID);
	if(it == m_mapFontMenu.end()) return FALSE;

	tagFontMenu *pFontMenu = it->second;
	int iFont = pFontMenu->pFontInfo->bDefault ? -1 : pFontMenu->pFontInfo->id; 

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker.GetAt(i);
		if(pTrackElem)
		{
			g_duiProp.AddAttribute(pTrackElem->m_node, pFontMenu->attrName, iFont, GetUIManager());
			TCHAR idBuffer[16];
			::ZeroMemory(idBuffer, sizeof(idBuffer));
			_itot(iFont, idBuffer, 10);
			pTrackElem->m_pControl->SetAttribute(pFontMenu->attrName, idBuffer);
		}
	}	

	for (it=m_mapFontMenu.begin(); it!=m_mapFontMenu.end(); it++)
	{
		delete it->second;
	}
	m_mapFontMenu.clear();

	return TRUE;
}

void CDuiEditorViewDesign::OnUpdateFontListRange(CCmdUI *pCmdUI)
{
	std::map<UINT, tagFontMenu *>::iterator it = m_mapFontMenu.find(pCmdUI->m_nID);
	if(it == m_mapFontMenu.end()) 
		return;

	tagFontMenu *pFontMenu = it->second;
	int iFont = pFontMenu->pFontInfo->bDefault ? -1 : pFontMenu->pFontInfo->id; 

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	for (int i=0; i<GetUIManager()->GetUiTracker()->m_arrTracker.GetSize(); i++)
	{
		CUITrackerMuliti::CTrackerElement *pTrackElem = GetUIManager()->GetUiTracker()->m_arrTracker.GetAt(i);
		if(pTrackElem)
		{
			if(pTrackElem->m_node.attribute(T2XML(pFontMenu->attrName)).as_int(-1) == iFont)
			{
				pCmdUI->SetCheck(TRUE);
			}
		}
	}	
}