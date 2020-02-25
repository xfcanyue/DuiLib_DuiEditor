// DuiEditorCode.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DuiEditorViewCode.h"

#include "TinyXml2/tinyxml2.h"

#include "DuiEditorDoc.h"
#include "MainFrm.h"
#include "ChildFrm.h"
// CDuiEditorCode

IMPLEMENT_DYNCREATE(CDuiEditorViewCode, CView)

CDuiEditorViewCode::CDuiEditorViewCode()
{
	m_pDlgFind = NULL;
	m_hDlgFind = NULL;

	m_pManager = NULL;
}

CDuiEditorViewCode::~CDuiEditorViewCode()
{
}

BEGIN_MESSAGE_MAP(CDuiEditorViewCode, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
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
END_MESSAGE_MAP()


// CDuiEditorCode 绘图

void CDuiEditorViewCode::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}
// CDuiEditorCode 消息处理程序


int CDuiEditorViewCode::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRect(0,0,100,100);
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		AfxMessageBox(_T("create SciLexer window error."));
		return -1;      // 未能创建
	}
	sci.InitXML();
	sci.sci_ClearTextAll();

	return 0;
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

void CDuiEditorViewCode::Init()
{
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();

	sci.sci_ClearTextAll();

	xml_string_writer writer;
	writer.pSciWnd = &sci;
	pDoc->m_doc.child(_T("Window")).print(writer);
	sci.sci_SetSavePoint();
	//sci.sci_EmptyUndoBuffer(); //不清理这个哦

	/*
	CXmlEditorFrame *pFrame = (CXmlEditorFrame *)GetParent();
	CXmlEditor *pEditor = (CXmlEditor *)pFrame->GetParent();

	xml_string_writer writer;
	writer.pSciWnd = &sci;
	pEditor->m_pDoc->m_doc.child(_T("Window")).print(writer);
	sci.sci_SetSavePoint();
	sci.sci_EmptyUndoBuffer();

	UpdateFrameStatus();
	*/
}

BOOL CDuiEditorViewCode::ApplyDocument()
{
	if(!sci.sci_GetModify())
		return TRUE;

	CStringA strTextA;
	sci.sci_GetTextAll(strTextA);
	CString strTextW = CStringToolExt::utf8ToCStrW(strTextA);

	//尝试解析修改后的xml文档
	tinyxml2::XMLDocument xml;
	tinyxml2::XMLError err = xml.LoadBuffer(strTextA, strTextA.GetLength());
	if(xml.Error())
	{
		CStringA strMsg;
		strMsg.Format("解析错误: \r\nErrorID:  %d \r\nErrorName:  %s \r\n\r\n错误所在行:  %d", 
			xml.ErrorID(), xml.ErrorIDToName(err), xml.GetErrorLineNum());
		MessageBoxA(NULL, strMsg, "XML ERROR", MB_OK);
		return FALSE;
	}

	//更新到主框架
	CDuiEditorDoc *pDoc = (CDuiEditorDoc *)GetDocument();
	pDoc->m_doc.load_string(strTextW, XML_PARSER_OPTIONS);
	pDoc->GetTreeView()->InitTreeContent();
	pDoc->SetModifiedFlag(TRUE);
	((CDuiEditorViewDesign *)pDoc->GetDesignView())->InitView();

	return TRUE;
}

void CDuiEditorViewCode::UpdateFrameStatus()
{
	CChildFrame *pFrame = (CChildFrame *)GetParent()->GetParent()->GetParent();

	int curLine = sci.sci_GetCurLine();
	int curPos = sci.sci_GetCurrentPos();

	CString temp;
	temp.Format(_T("row: %d"), curLine+1);
	pFrame->m_wndStatusBar.SetPaneText(1, temp);

	int col = curPos - sci.sci_PositionFromLine(curLine);
	temp.Format(_T("col: %d"), col+1);
	pFrame->m_wndStatusBar.SetPaneText(2, temp);

	temp.Format(_T("pos: %d"), curPos);
	pFrame->m_wndStatusBar.SetPaneText(3, temp);

	temp.Format(_T("length: %d"), sci.sci_GetLength());
	pFrame->m_wndStatusBar.SetPaneText(4, temp);

	temp.Format(_T("lines: %d"), sci.sci_GetLineCount());
	pFrame->m_wndStatusBar.SetPaneText(5, temp);
	
}

BOOL CDuiEditorViewCode::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
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
			InsertMsg(_T("SCK_BACK"));
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
				CString strShowW = AutoCompleteProperty(GetNodeName(), _T(""));
				CStringA strShowA = CStringToolExt::CStrT2CStrA(strShowW);
				if(!strShowA.IsEmpty())
					sci.sci_AutocShow(0, strShowA);
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
				CStringW objectNameW = CStringToolExt::utf8ToCStrW(objectNameA);

				CStringA strLeftA;
				sci.sci_GetTextRange(startPos-1, startPos, strLeftA);
				if(strLeftA == '<' || strLeftA == '/')
				{
					AutoCompleteNode(objectNameW);
				}
				else
				{
					CString strShowW = AutoCompleteProperty(GetNodeName(), objectNameW);
					CStringA strShowA = CStringToolExt::CStrT2CStrA(strShowW);
					if(!strShowA.IsEmpty())
						sci.sci_AutocShow(0, strShowA);
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

	// 	CString temp;
	// 	temp.Format(_T("sciwnd msg=%d"), pMsg->nmhdr.code);
	// 	InsertMsg(temp);
	return CWnd::OnNotify(wParam, lParam, pResult);
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
				strRet = CStringToolExt::CStrA2CStrW(strRetA);
				break;
			}
		}
	}


	//InsertMsg(CStringToolExt::CStrA2CStrW(strText));

	return strRet;
}

void CDuiEditorViewCode::AutoCompleteNode(CStringW objectNameW)		//自动完成控件名
{
	CStringW strShowW;

	for (xml_node node=g_duiProp.GetRoot().first_child(); node; node=node.next_sibling())
	{
		CStringW nodeName = node.name();
		nodeName.MakeUpper();
		objectNameW.MakeUpper();
		if(nodeName.Find(objectNameW) >= 0)
		{
			strShowW += node.name();
			strShowW += _T(" ");
		}
	}

	CStringA strShowA = CStringToolExt::CStrT2CStrA(strShowW);
	if(!strShowA.IsEmpty())
		sci.sci_AutocShow(0, strShowA);
}

CStringW CDuiEditorViewCode::AutoCompleteProperty(CStringW objectNameW, CStringW AttrName)	//自动完成属性名
{
	CStringW strShowW;

	xml_node node = g_duiProp.FindControl(objectNameW);
	for (xml_node nodeAttr = node.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
	{
		LPCTSTR className = nodeAttr.attribute(_T("name")).value();
		if(AttrName.IsEmpty())
		{
			strShowW += className;
			strShowW += _T(" ");
		}
		else
		{
			CString strClass = className;
			strClass.MakeUpper();
			AttrName.MakeUpper();
			if(strClass.Find(AttrName) >= 0)
			{
				strShowW += className;
				strShowW += _T(" ");
			}
		}
	}

	CString parentName = node.attribute(_T("parent")).as_string();
	if(!parentName.IsEmpty())
	{
		strShowW += AutoCompleteProperty(parentName, AttrName);
	}

	return strShowW;
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

	if(bActivate && pActivateView==this && pDeactiveView!=this)
	{
		pMain->m_wndControl.SetActiveTreeView(((CDuiEditorDoc *)GetDocument())->GetTreeView());
		pMain->HideAllPane();

		//InsertMsg(_T("OnActivateCode"));
	}

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//////////////////////////////////////////////////////////////////////////
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

