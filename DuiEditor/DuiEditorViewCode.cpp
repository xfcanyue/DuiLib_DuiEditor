// DuiEditorCode.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DuiEditorViewCode.h"

#include "TinyXml2/tinyxml2.h"

#include "DuiEditorDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "UIManager.h"
// CDuiEditorCode

IMPLEMENT_DYNCREATE(CDuiEditorViewCode, CView)

CDuiEditorViewCode::CDuiEditorViewCode()
{
	m_pUIManager = NULL;
	m_pDlgFind = NULL;
	m_hDlgFind = NULL;
	m_nTargetLine = -1;
}

CDuiEditorViewCode::~CDuiEditorViewCode()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorViewCode, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_SCIWND_CLICK, OnSciClick)
	ON_COMMAND(ID_EDIT_UNDO, &CDuiEditorViewCode::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDuiEditorViewCode::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDuiEditorViewCode::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDuiEditorViewCode::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, &CDuiEditorViewCode::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDuiEditorViewCode::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CDuiEditorViewCode::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDuiEditorViewCode::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_FIND, &CDuiEditorViewCode::OnEditFind)
	ON_COMMAND(ID_EDIT_PASTE, &CDuiEditorViewCode::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDuiEditorViewCode::OnUpdateEditPaste)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCI_REFRESH, &CDuiEditorViewCode::OnEditRefresh)
	ON_COMMAND(ID_SCI_SELECT_ALL, &CDuiEditorViewCode::OnEditSelectAll)
	ON_COMMAND(ID_SCI_UPDATE_DESIGN, &CDuiEditorViewCode::OnSciUpdateDesign)
	ON_UPDATE_COMMAND_UI(ID_SCI_UPDATE_DESIGN, &CDuiEditorViewCode::OnUpdateSciUpdateDesign)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDuiEditorCode 绘图
inline CDuiEditorDoc* CDuiEditorViewCode::GetDocument() const
{ return reinterpret_cast<CDuiEditorDoc*>(m_pDocument); }

void CDuiEditorViewCode::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}

BOOL CDuiEditorViewCode::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}

// CDuiEditorCode 消息处理程序


int CDuiEditorViewCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetUIManager(GetDocument()->GetUIManager());
	GetUIManager()->_setDesignCode(this);
	CRect rectDummy;
	rectDummy.SetRect(0,0,100,100);
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		AfxMessageBox(_T("create SciLexer window error."));
		return -1;      // 未能创建
	}
	sci.InitXML(g_cfg);
	sci.sci_UsePopup(FALSE);

	return 0;
}

void CDuiEditorViewCode::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	sci.sci_ClearTextAll();
	LoadDocument();
}

void CDuiEditorViewCode::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(!sci.GetSafeHwnd())	return;

	CRect rcClient;
	rcClient.DeflateRect(1,1,1,1);
	GetClientRect(rcClient);
	sci.MoveWindow(rcClient);
}

void CDuiEditorViewCode::LoadDocument()
{
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();

	CSciXmlWriter writer(&sci);
	writer.print(pDoc->m_doc.child(_T("Window")));

	if(m_nTargetLine >= 0)
	{
		int line = m_nTargetLine;
		int lineposbegin = sci.sci_PositionFromLine(line);
		int lineposend = sci.sci_GetLineEndPosition(line);

		//想办法居中
		int firstline = sci.sci_GetFirstVisibleLine();
		int lastline = firstline + sci.sci_LineSonScreen();
		int centerline = firstline + (lastline - firstline)/2;
		if(line < centerline)
			sci.sci_LineScroll(0, line - centerline);
		else
			sci.sci_LineScroll(0, line - centerline);

		sci.sci_SetSel(lineposend, lineposbegin);

		m_nTargetLine = -1;
	}
}

BOOL CDuiEditorViewCode::ApplyDocument()
{
	if(!sci.sci_GetModify())
		return TRUE;

	LSSTRING_CONVERSION;
	CStringA strTextUtf8;
	sci.sci_GetTextAll(strTextUtf8);
	CString strText = LSUTF82T(strTextUtf8);

	//尝试解析修改后的xml文档
	tinyxml2::XMLDocument xml;
	tinyxml2::XMLError err = xml.LoadBuffer(strTextUtf8, strTextUtf8.GetLength());
	if(xml.Error())
	{
		CStringA strMsg;
		strMsg.Format("解析错误: \r\nErrorID:  %d \r\nErrorName:  %s \r\n\r\n错误所在行:  %d", 
			xml.ErrorID(), xml.ErrorIDToName(err), xml.GetErrorLineNum());
		::MessageBoxA(NULL, strMsg, "XML ERROR", MB_OK);
		return FALSE;
	}

	//更新到主框架
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();
	pDoc->m_doc.load_string(strText, XML_PARSER_OPTIONS);
	GetUIManager()->GetTreeView()->InitTreeContent();
	pDoc->SetModifiedFlag(TRUE);
	GetUIManager()->GetDesignView()->InitView();

	return TRUE;
}
void CDuiEditorViewCode::SelectXmlNode(CControlUI *pControl)
{
	//切换到行，设计界面中选中的控件
	xml_node node((xml_node_struct *)pControl->GetTag());
	SelectXmlNode(node);
}

void CDuiEditorViewCode::SelectXmlNode(xml_node node)
{
	//文档有变动，重新载入一次
	if(sci.sci_GetModify())
	{
		LoadDocument();
	}

	if(node)
	{
		int line = node.get_row();
		int lineposbegin = sci.sci_PositionFromLine(line);
		int lineposend = sci.sci_GetLineEndPosition(line);

		//想办法居中
		int firstline = sci.sci_GetFirstVisibleLine();
		int lastline = firstline + sci.sci_LineSonScreen();
		int centerline = firstline + (lastline - firstline)/2;
		if(line < centerline)
			sci.sci_LineScroll(0, line - centerline);
		else
			sci.sci_LineScroll(0, line - centerline);

		sci.sci_SetSel(lineposend, lineposbegin);


		// 		CString temp;
		// 		temp.Format(_T("line=%d, lineposbegin=%d, lineposend=%d"), line, lineposbegin, lineposend);
		// 		InsertMsg(temp);
	}
}

BOOL CDuiEditorViewCode::SelectControlUI(int line, xml_node node)
{
	CUIManager *pManager = GetUIManager();
	if(node.get_row() == line)
	{
		CControlUI *pControl = (CControlUI *)node.get_tag();
		pManager->SelectItem(pControl);
		pManager->GetTreeView()->SelectXmlNode(node);

		//切换左边控件树
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		if(!pMain->m_wndControl.IsPaneVisible())
		{
			pMain->m_wndControl.ShowPane(TRUE, TRUE, TRUE);
		}
		return TRUE;
	}

	for (xml_node nd=node.first_child(); nd; nd=nd.next_sibling())
	{
		if(SelectControlUI(line, nd))
			return TRUE;
	}
	return FALSE;
}

LRESULT CDuiEditorViewCode::OnSciClick(WPARAM WParam, LPARAM LParam)
{
	if(GetUIManager()->GetSplitterMode() == SPLIT_CODE)
		return 0;

	//点击文档内容时，同时选中设计界面中的控件 以及 树控件
	int line = (int)WParam;

	xml_node root = GetDocument()->m_doc.child(_T("Window"));
	SelectControlUI(line, root);

	return 0;
}

void CDuiEditorViewCode::UpdateFrameStatus()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int curLine = sci.sci_GetCurLine();
	int curPos = sci.sci_GetCurrentPos();

	CString temp;
	temp.Format(_T("row: %d"), curLine+1);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LINE, temp);

	int col = curPos - sci.sci_PositionFromLine(curLine);
	temp.Format(_T("col: %d"), col+1);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_COL, temp);

	temp.Format(_T("pos: %d"), curPos);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURRENT_POS, temp);

	temp.Format(_T("TotalLength: %d"), sci.sci_GetLength());
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LENGTH, temp);

	temp.Format(_T("TotalLines: %d"), sci.sci_GetLineCount());
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_TOTAL_LINE, temp);
}

BOOL CDuiEditorViewCode::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LSSTRING_CONVERSION;

	NMHDR *phDR;
	phDR = (NMHDR*)lParam;

	if (phDR == NULL || phDR->hwndFrom != sci.m_hWnd)
	{
		return CWnd::OnNotify(wParam, lParam, pResult);
	}

	//输入"<",  列出Control
	//输入"<x",  列出包含'x'的Control
	//输入"</", 反括号, 回溯寻找未封闭的左括号, 自动列出未封闭的控件名
	//<Control a, 找到Control名 列出Control的属性列表


	SCNotification *pMsg = (SCNotification*)lParam;
	sci.OnParentNotify(pMsg);

	switch (pMsg->nmhdr.code)
	{
	case SCK_BACK:
		{
			//InsertMsg(_T("SCK_BACK"));
		}
		break;
	case SCN_CHARADDED:	//输入字符
		{
			if(pMsg->ch == '<' || pMsg->ch == '/')
			{
				AutoCompleteNode(_T(""));
			}
			else if(pMsg->ch == ' ')
			{
				CString strShow = AutoCompleteProperty(GetNodeName(), _T(""));
				if(!strShow.IsEmpty())
					sci.sci_AutocShow(0, LST2UTF8(strShow));
			}
			else if(pMsg->ch == '>')
			{
			}
			else if(pMsg->ch == '=')
			{

			}
			else if(pMsg->ch == '"')
			{

			}
			else
			{
				int pos = sci.sci_GetCurrentPos();
				int startPos = sci.sci_WordStartPosition(pos-1, TRUE);
				int endPos = sci.sci_WordEndPosition(pos-1, TRUE);

				CStringA objectNameA;
				sci.sci_GetTextRange(startPos, endPos, objectNameA);

				CStringA strLeftA;
				sci.sci_GetTextRange(startPos-1, startPos, strLeftA);
				if(strLeftA == '<' || strLeftA == '/')
				{
					AutoCompleteNode(LSUTF82T(objectNameA));
				}
				else
				{
					CString strShow = AutoCompleteProperty(GetNodeName(), LSUTF82T(objectNameA));
					if(!strShow.IsEmpty())
					{
						sci.sci_AutocShow(0, LST2UTF8(strShow));
					}
				}
			}			
		}
		break;
	case SCN_MODIFIED:
		{
			UpdateFrameStatus();
		}
		break;
	case SCN_SAVEPOINTREACHED:	//文件被保存
		{

		}
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		{

		}
		break;
	case SCN_UPDATEUI:
		UpdateFrameStatus();
		break;
	}	
	return __super::OnNotify(wParam, lParam, pResult);
}

CString CDuiEditorViewCode::GetNodeName()
{
	CString strRet;
	int pos = sci.sci_GetCurrentPos();

	CStringA strText;
	sci.sci_GetTextRange(0, pos, strText);
	int nSpacePos = -1;
	for (int i=strText.GetLength()-1; i>=0; i--)
	{
		if(strText[i] == ' ')
		{
			nSpacePos = i;
		}
		else if(strText[i] == '<')
		{
			int nodePos = i-1;
			if(nSpacePos >= 0)
			{
				CStringA strRetA;
				sci.sci_GetTextRange(nodePos+2, nSpacePos, strRetA);
				//InsertMsg(CStringToolExt::CStrA2CStrW(strRetA));
				LSSTRING_CONVERSION;
				strRet = LSUTF82T(strRetA);
				break;
			}
		}
	}


	//InsertMsg(CStringToolExt::CStrA2CStrW(strText));

	return strRet;
}

void CDuiEditorViewCode::AutoCompleteNode(CString objectName)		//自动完成控件名
{
	CString strShow;

	objectName.MakeUpper();

	for (xml_node node=g_duiProp.GetRoot().first_child(); node; node=node.next_sibling())
	{
		CString nodeName = node.name();
		nodeName.MakeUpper();
		if(nodeName.Find(objectName) >= 0)
		{
			strShow += node.name();
			strShow += _T(" ");
		}
	}

	LSSTRING_CONVERSION;
	if(!strShow.IsEmpty())
	{
		sci.sci_AutocShow(0, LST2UTF8(strShow));
	}
}

CString CDuiEditorViewCode::AutoCompleteProperty(CString objectName, CString AttrName)	//自动完成属性名
{
	CString strShow;

	xml_node node = g_duiProp.FindControl(objectName);
	for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
	{
		LPCTSTR className = nodeAttr.attribute(_T("name")).value();
		if(AttrName.IsEmpty())
		{
			strShow += className;
			strShow += _T(" ");
		}
		else
		{
			CString strClass = className;
			strClass.MakeUpper();
			AttrName.MakeUpper();
			if(strClass.Find(AttrName) >= 0)
			{
				strShow += className;
				strShow += _T(" ");
			}
		}
	}

	CString parentName = node.attribute(_T("parent")).as_string();
	if(!parentName.IsEmpty())
	{
		strShow += AutoCompleteProperty(parentName, AttrName);
	}

	return strShow;
}


BOOL CDuiEditorViewCode::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return CView::PreCreateWindow(cs);
}

void CDuiEditorViewCode::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	/*
	if(bActivate && pActivateView==this && pDeactiveView!=this)
	{
		pMain->m_wndControl.SetActiveTreeView(GetUIManager()->GetTreeView());
		pMain->m_wndProperty.SetActivePropList(GetUIManager()->GetPropList());

		if(GetUIManager()->GetSplitterMode() == SPLIT_CODE)
		{
			pMain->HideAllPane();
		}

		//InsertMsg(_T("OnActivateCode"));
	}
	*/
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//////////////////////////////////////////////////////////////////////////
void CDuiEditorViewCode::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);

	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_DESIGN_CODE);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, this, TRUE);
}

void CDuiEditorViewCode::OnEditUndo()
{
	sci.sci_Undo();
}


void CDuiEditorViewCode::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanUndo());
}


void CDuiEditorViewCode::OnEditRedo()
{
	sci.sci_Redo();
}


void CDuiEditorViewCode::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanRedo());
}


void CDuiEditorViewCode::OnEditCut()
{
	sci.sci_Cut();
}


void CDuiEditorViewCode::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCut());
}


void CDuiEditorViewCode::OnEditCopy()
{
	sci.sci_Copy();
}


void CDuiEditorViewCode::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCopy());
}

void CDuiEditorViewCode::OnEditPaste()
{
	sci.sci_Paste();
}


void CDuiEditorViewCode::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanPaste());
}

void CDuiEditorViewCode::OnEditFind()
{
	if(::IsWindow(m_hDlgFind))
	{
		m_pDlgFind->CenterWindow(this);
		return;
	}

	m_pDlgFind = new CSciFind;
	m_pDlgFind->m_pSciWnd = &sci;
	m_pDlgFind->Create(IDD_SCI_FIND, this);
	m_pDlgFind->CenterWindow(this);
	m_pDlgFind->ShowWindow(SW_SHOW);

	m_hDlgFind = m_pDlgFind->m_hWnd;
}


void CDuiEditorViewCode::OnEditRefresh()
{
	LoadDocument();
}


void CDuiEditorViewCode::OnEditSelectAll()
{
	sci.sci_SelectAll();
}


void CDuiEditorViewCode::OnSciUpdateDesign()
{
	m_nTargetLine = sci.sci_GetCurLine();
	ApplyDocument();
}


void CDuiEditorViewCode::OnUpdateSciUpdateDesign(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_GetModify());
}

