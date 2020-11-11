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
#include "SciXmlParse.h"
// CDuiEditorCode

IMPLEMENT_DYNCREATE(CDuiEditorViewCode, CView)

CDuiEditorViewCode::CDuiEditorViewCode()
{
	m_pUIManager = NULL;
	m_nTargetLine = -1;
	m_bAutoUpdateDesign = TRUE;
	m_bNeedUpdate = FALSE;
	m_bUpdateDesignWhileModified = TRUE;
	m_xmlParseResult.status = pugi::status_ok;
}

CDuiEditorViewCode::~CDuiEditorViewCode()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorViewCode, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_SCIWND_CLICK, OnSciClick)
	ON_MESSAGE(WM_SCIWND_MOUSEMOVE, OnSciMouseMove)
	ON_COMMAND(ID_EDIT_CUT, &CDuiEditorViewCode::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDuiEditorViewCode::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CDuiEditorViewCode::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDuiEditorViewCode::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDuiEditorViewCode::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDuiEditorViewCode::OnUpdateEditPaste)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCI_REFRESH, &CDuiEditorViewCode::OnEditRefresh)
	ON_COMMAND(ID_SCI_SELECT_ALL, &CDuiEditorViewCode::OnEditSelectAll)
	ON_COMMAND(ID_SCI_UPDATE_DESIGN, &CDuiEditorViewCode::OnSciUpdateDesign)
	ON_UPDATE_COMMAND_UI(ID_SCI_UPDATE_DESIGN, &CDuiEditorViewCode::OnUpdateSciUpdateDesign)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SCI_PARSE_XML, &CDuiEditorViewCode::OnSciParseXml)
	ON_COMMAND(ID_SCI_APPLY_DESIGN, &CDuiEditorViewCode::OnSciApplyDesign)
	ON_UPDATE_COMMAND_UI(ID_SCI_APPLY_DESIGN, &CDuiEditorViewCode::OnUpdateSciApplyDesign)
	ON_COMMAND(ID_SCI_WRAP_LINE, &CDuiEditorViewCode::OnSciWrapLine)
	ON_UPDATE_COMMAND_UI(ID_SCI_WRAP_LINE, &CDuiEditorViewCode::OnUpdateSciWrapLine)
	ON_COMMAND(ID_SCI_UPDATE_WHILE_MODIFY, &CDuiEditorViewCode::OnSciUpdateWhileModify)
	ON_UPDATE_COMMAND_UI(ID_SCI_UPDATE_WHILE_MODIFY, &CDuiEditorViewCode::OnUpdateSciUpdateWhileModify)
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
	LoadDocument();
	GetUIManager()->GetDocument()->SetModifiedFlag(FALSE);
	sci.sci_SetSavePoint();
	sci.sci_EmptyUndoBuffer(); //清理历史记录
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
#ifdef _DEBUG
	InsertMsg(_T("LoadDocument"));
#endif
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();

	m_bAutoUpdateDesign = FALSE;
	sci.sci_ClearTextAll();
	xml_writer_string w;
	pDoc->m_doc.print(w);
	sci.sci_InsertText(0, w.strText.c_str());
	m_bAutoUpdateDesign = TRUE;

//  CSciXmlWriter writer(&sci);
//  writer.print(pDoc->m_doc);//.child(_T("Window")));
// 	return;

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
	m_nTargetLine = sci.sci_GetCurLine();

	LSSTRING_CONVERSION;
	CStringA strTextUtf8;
	sci.sci_GetTextAll(strTextUtf8);
	CString strText = LSUTF82T(strTextUtf8);

	//尝试解析修改后的xml文档
// 	tinyxml2::XMLDocument xml;
// 	tinyxml2::XMLError err = xml.LoadBuffer(strTextUtf8, strTextUtf8.GetLength());
// 	if(xml.Error())
// 	{
// 		CStringA strMsg;
// 		strMsg.Format("解析错误: \r\nErrorID:  %d \r\nErrorName:  %s \r\n\r\n错误所在行:  %d", 
// 			xml.ErrorID(), xml.ErrorIDToName(err), xml.GetErrorLineNum());
// 		::MessageBoxA(NULL, strMsg, "XML ERROR", MB_OK);
// 		return FALSE;
// 	}

	xml_document doc;
	xml_parse_result ret = doc.load_string(T2XML(strText), XML_PARSER_OPTIONS);
	if(ret.status != pugi::status_ok)
	{
		LSSTRING_CONVERSION;
		CString temp;
		int row = sci.sci_LineFromPosition(ret.offset);
		int col = ret.offset - sci.sci_PositionFromLine(row);
		temp.Format(_T("解析错误: \r\n行: %d \r\n列: %d \r\n位置: %d \r\n错误信息: %s"), 
			row+1, col+1,
			ret.offset, 
			LSA2T(ret.description()));
		AfxMessageBox(temp);
		return FALSE;
	}

	//更新到主框架
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();
	pDoc->m_doc.load_string(T2XML(strText), XML_PARSER_OPTIONS);

	GetUIManager()->GetUiTracker()->RemoveAll();
	GetUIManager()->GetTreeView()->InitTreeContent();
	GetUIManager()->GetDesignView()->InitView();
	//pDoc->SetModifiedFlag(TRUE);
	return TRUE;
}

BOOL CDuiEditorViewCode::UpdateDocument()
{
	CStringA strTextUtf8;
	sci.sci_GetTextAll(strTextUtf8);
	CString strText = LSUTF82T(strTextUtf8);

	xml_document xml;
	xml_parse_result ret = xml.load_string(T2XML(strText), XML_PARSER_OPTIONS);
	if(ret.status != pugi::status_ok)
	{
		CString temp;
		int row = sci.sci_LineFromPosition(ret.offset);
		int col = ret.offset - sci.sci_PositionFromLine(row);
		temp.Format(_T("解析错误: \r\n行: %d \r\n列: %d \r\n位置: %d \r\n错误信息: %s"), 
			row+1, col+1,
			ret.offset, 
			LSA2T(ret.description()));
		AfxMessageBox(temp);
		return FALSE;
	}

	CSciXmlParse parse;
	parse.SetUIManager(GetUIManager());
	parse.Parse(GetUIManager()->GetDocument()->m_doc, xml);

	//重新计算控件大小，并且刷新Tracker
	GetUIManager()->GetUiFrom()->SetPos(GetUIManager()->GetUiFrom()->GetPos());
	GetUIManager()->GetUiTracker()->RefreshRect();

	GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
	//sci.sci_SetSavePoint();
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
	if(m_bNeedUpdate)
	{
		AfxMessageBox(_T("XML文档已修改，可能无法定位控件在文档中的位置。 请先提交更新。"));
	}

	if(node)
	{
		int lineposbegin = node.get_open_start_pos();
		int lineposend = node.get_open_end_pos();
		int line = sci.sci_LineFromPosition(lineposbegin);

		//想办法居中
		int firstline = sci.sci_GetFirstVisibleLine();
		int lastline = firstline + sci.sci_LineSonScreen();
		int centerline = firstline + (lastline - firstline)/2;
		if(line < centerline)
			sci.sci_LineScroll(0, line - centerline);
		else
			sci.sci_LineScroll(0, line - centerline);

		sci.sci_SetSel(lineposend, lineposbegin);
	}

#ifdef _DEBUG
	InsertMsgV(_T("node:%s, open_start_pos=%d, open_end_pos=%d, close_start_pos=%d, close_end_pos=%d"), 
		XML2T(node.name()), node.get_open_start_pos(), node.get_open_end_pos(), node.get_close_start_pos(), node.get_close_end_pos());
#endif
}

void CDuiEditorViewCode::AddNode(xml_node node)
{
	m_bAutoUpdateDesign = FALSE;
	CSciXmlWriter w(&sci);
	w.AddNode(node);
	m_bAutoUpdateDesign = TRUE;
}

void CDuiEditorViewCode::DeleteNode(xml_node node)
{
	m_bAutoUpdateDesign = FALSE;
	CSciXmlWriter w(&sci);
	w.DeleteNode(node);
	m_bAutoUpdateDesign = TRUE;
}

void CDuiEditorViewCode::AddAttribute(xml_node node, xml_attribute attr)
{
	m_bAutoUpdateDesign = FALSE;
	CSciXmlWriter w(&sci);
	w.AddAttribute(node, attr);
	m_bAutoUpdateDesign = TRUE;
}

void CDuiEditorViewCode::ModifyAttribute(xml_node node, xml_attribute attr)
{
	m_bAutoUpdateDesign = FALSE;
	CSciXmlWriter w(&sci);
	m_bAutoUpdateDesign = FALSE;;
	w.ModifyAttribute(node, attr);
	m_bAutoUpdateDesign = TRUE;
}

void CDuiEditorViewCode::DeleteAttribute(xml_node node, xml_attribute attr)
{
	m_bAutoUpdateDesign = FALSE;
	CSciXmlWriter w(&sci);
	w.DeleteAttribute(node, attr);
	m_bAutoUpdateDesign = TRUE;
}

BOOL CDuiEditorViewCode::SelectControlUI(int pos, xml_node node)
{
	CUIManager *pManager = GetUIManager();

	if((pos>=node.get_open_start_pos() && pos<=node.get_open_end_pos()) ||
		(pos>=node.get_close_start_pos() && pos<=node.get_close_end_pos()))
	{
		CControlUI *pControl = (CControlUI *)node.get_tag();
		GetUIManager()->SelectItem(pControl);
		GetUIManager()->EnsureVisible(node);
		GetUIManager()->GetTreeView()->SelectXmlNode(node);

		//切换左边控件树
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		if(!pMain->m_wndControl.IsPaneVisible())
		{
			pMain->m_wndControl.ShowPane(TRUE, TRUE, TRUE);
		}
		
#ifdef _DEBUG
		InsertMsgV(_T("node:%s, open_start_pos=%d, open_end_pos=%d, close_start_pos=%d, close_end_pos=%d"), 
			XML2T(node.name()), node.get_open_start_pos(), node.get_open_end_pos(), node.get_close_start_pos(), node.get_close_end_pos());
#endif
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			if(pos >= attr.get_name_start_pos() && pos <= attr.get_value_end_pos()+1)
			{
#ifdef _DEBUG
				InsertMsgV(_T("attr: %s, name_start_pos=%d, name_end_pos=%d, value_start_pos=%d, value_end_pos=%d"), 
					XML2T(attr.name()), attr.get_name_start_pos(), attr.get_name_end_pos(), attr.get_value_start_pos(), attr.get_value_end_pos());
#endif
				continue;
			}
		}
		
		return TRUE;
	}

	for (xml_node nd=node.first_child(); nd; nd=nd.next_sibling())
	{
		if(SelectControlUI(pos, nd))
			return TRUE;
	}
	
	return FALSE;
}

LRESULT CDuiEditorViewCode::OnSciClick(WPARAM WParam, LPARAM LParam)
{
	if(GetUIManager()->GetSplitterMode() == SPLIT_CODE)
		return 0;

	//点击文档内容时，同时选中设计界面中的控件 以及 树控件
	int pos = (int)WParam;

	xml_node root = GetDocument()->m_doc.child(XTEXT("Window"));
	SelectControlUI(pos, root);

	return 0;
}

LRESULT CDuiEditorViewCode::OnSciMouseMove(WPARAM WParam, LPARAM LParam)
{
	CPoint pt;
	::GetCursorPos(&pt);
	::ScreenToClient(sci.GetSafeHwnd(), &pt);

	int pos = sci.sci_PostionFromPoint(pt.x, pt.y);
	int row = sci.sci_LineFromPosition(pos);
	int col = pos - sci.sci_PositionFromLine(row);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
// 	CString temp;
// 	temp.Format(_T("坐标: 行=%d, 列=%d, 位置=%d"), row, col, pos);
// 	pMain->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURSOR_POS, temp);
	pMain->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURSOR_POS, _T(""));
	return 0;
}

void CDuiEditorViewCode::UpdateFrameStatus()
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	int curLine = sci.sci_GetCurLine();
	int curPos = sci.sci_GetCurrentPos();

	if(m_bUpdateDesignWhileModified)
		pFrame->m_wndStatusBar.SetXmlUpdateStatus(!m_bNeedUpdate);
	else
		pFrame->m_wndStatusBar.SetXmlUpdateStatus(m_xmlParseResult.status == pugi::status_ok);

	CString temp;
	temp.Format(_T("行: %d"), curLine+1);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LINE, temp);

	int col = curPos - sci.sci_PositionFromLine(curLine);
	temp.Format(_T("列: %d"), col);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_COL, temp);

	temp.Format(_T("位置: %d"), curPos);
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_CURRENT_POS, temp);

	temp.Format(_T("总长度: %d"), sci.sci_GetLength());
	pFrame->m_wndStatusBar.SetPaneTextByID(ID_INDICATOR_LENGTH, temp);

	temp.Format(_T("总行数: %d"), sci.sci_GetLineCount());
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
			if((pMsg->modificationType & SC_MOD_INSERTTEXT) || 
				(pMsg->modificationType & SC_MOD_DELETETEXT))
			{
				//InsertMsg(_T("SCN_MODIFIED"));
				CStringA strTextUtf8;
				sci.sci_GetTextAll(strTextUtf8);
				CString strText = LSUTF82T(strTextUtf8);
				xml_document xml;
				m_xmlParseResult = xml.load_string(T2XML(strText), XML_PARSER_OPTIONS);

				//尝试实时提交修改
				if(m_bUpdateDesignWhileModified && m_bAutoUpdateDesign  && 
					(GetUIManager()->GetSplitterMode() == SPLIT_UPDOWN || 
					GetUIManager()->GetSplitterMode() == SPLIT_LEFTRIGHT) )
				{
					m_bNeedUpdate = TRUE;

					if(m_xmlParseResult.status == pugi::status_ok)
					{
						CSciXmlParse parse;
						parse.SetUIManager(GetUIManager());
						parse.Parse(GetUIManager()->GetDocument()->m_doc, xml);

						//重新计算控件大小，并且刷新Tracker
						GetUIManager()->GetUiFrom()->SetPos(GetUIManager()->GetUiFrom()->GetPos());
						GetUIManager()->GetUiTracker()->RefreshRect();
						m_bNeedUpdate = FALSE;
					}
				}
			}
		}
		break;
	case SCN_SAVEPOINTREACHED:	//文件被保存
		{
			//InsertMsg(_T("SCN_SAVEPOINTREACHED"));
			GetUIManager()->GetDocument()->SetModifiedFlag(FALSE);
		}
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		{			
			//InsertMsg(_T("SCN_SAVEPOINTLEFT"));
			GetUIManager()->GetDocument()->SetModifiedFlag(TRUE);
		}
		break;
	case SCN_UPDATEUI:
		{	
			UpdateFrameStatus();
		}
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
		CString nodeName = XML2T(node.name());
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
		CString className = XML2T(nodeAttr.attribute(XTEXT("name")).value());
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

	CString parentName = XML2T(node.attribute(XTEXT("parent")).as_string());
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
	//if(bActivate) InsertMsg(_T("CDuiEditorViewCode::OnActivateView"));

	/*
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

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
	
#ifndef _DEBUG
	CMenu *pMenu = popmenu.GetSubMenu(0);
	pMenu->DeleteMenu(ID_SCI_PARSE_XML, MF_BYCOMMAND);
#endif
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, AfxGetMainWnd(), TRUE);
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


void CDuiEditorViewCode::OnEditRefresh()
{
	LoadDocument();
	UpdateDocument();
}


void CDuiEditorViewCode::OnEditSelectAll()
{
	sci.sci_SelectAll();
}


void CDuiEditorViewCode::OnSciUpdateDesign()
{
	UpdateDocument();
}

void CDuiEditorViewCode::OnUpdateSciUpdateDesign(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bNeedUpdate);
}


void CDuiEditorViewCode::OnSciParseXml()
{
	return;
}

void CDuiEditorViewCode::OnSciApplyDesign()
{
	m_nTargetLine = sci.sci_GetCurLine();
	ApplyDocument();
}


void CDuiEditorViewCode::OnUpdateSciApplyDesign(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CDuiEditorViewCode::OnSciWrapLine()
{
	BOOL bAutoWrapLine = sci.sci_GetWrapMode() != SC_WRAP_NONE;

	g_cfg.bAutoWrapLine = !bAutoWrapLine;
	g_cfg.SaveConfig();

	if(g_cfg.bAutoWrapLine)
		sci.sci_SetWrapMode(SC_WRAP_WORD);
	else
		sci.sci_SetWrapMode(SC_WRAP_NONE);
}


void CDuiEditorViewCode::OnUpdateSciWrapLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(sci.sci_GetWrapMode() != SC_WRAP_NONE);
}


void CDuiEditorViewCode::OnSciUpdateWhileModify()
{
	m_bUpdateDesignWhileModified = !m_bUpdateDesignWhileModified;
}


void CDuiEditorViewCode::OnUpdateSciUpdateWhileModify(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bUpdateDesignWhileModified);
}
