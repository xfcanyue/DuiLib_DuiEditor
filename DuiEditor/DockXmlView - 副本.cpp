// DockXmlView.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DockXmlView.h"
#include "SciXmlWriter.h"

// CDockXmlView

IMPLEMENT_DYNAMIC(CDockXmlView, CWnd)

CDockXmlView::CDockXmlView()
{

}

CDockXmlView::~CDockXmlView()
{
}


BEGIN_MESSAGE_MAP(CDockXmlView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_UNDO, &CDockXmlView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDockXmlView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDockXmlView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDockXmlView::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_CUT, &CDockXmlView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CDockXmlView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CDockXmlView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CDockXmlView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDockXmlView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CDockXmlView::OnUpdateEditPaste)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SCI_UPDATE_DESIGN, &CDockXmlView::OnSciUpdateDesign)
	ON_UPDATE_COMMAND_UI(ID_SCI_UPDATE_DESIGN, &CDockXmlView::OnUpdateSciUpdateDesign)
END_MESSAGE_MAP()



// CDockXmlView 消息处理程序




int CDockXmlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRect(0,0,0,0);

	// TODO:  在此添加您专用的创建代码
	if (!sci.Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, rectDummy, this, ID_SCI_WND))
	{
		AfxMessageBox(_T("create SciLexer window error."));
		return NULL;      // 未能创建
	}
	sci.InitXML();
	sci.sci_UsePopup(FALSE);
	sci.sci_ClearTextAll();

	return 0;
}


void CDockXmlView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(sci.GetSafeHwnd())	
	{
		CRect rcClient;
		rcClient.DeflateRect(1,1,1,1);
		GetClientRect(rcClient);
		sci.MoveWindow(rcClient);
	}
}


void CDockXmlView::Init()
{
	CDuiEditorDoc *pDoc  = (CDuiEditorDoc *)m_pDoc;

	CSciXmlWriter writer(&sci);
	writer.print(pDoc->m_doc.child(_T("Window")));
}

void CDockXmlView::SelectControlNode(CControlUI *pControl)
{
	//切换到行，设计界面中选中的控件
	xml_node node((xml_node_struct *)pControl->GetTag());
	SelectControlNode(node);
}

void CDockXmlView::SelectControlNode(xml_node node)
{
	//文档有变动，重新载入一次
	if(sci.sci_GetModify())
	{
		Init();
	}

	if(node)
	{
		int line = node.get_row();
		int lineposbegin = sci.sci_PositionFromLine(line);
		int lineposend = sci.sci_GetLineEndPosition(line);
		CString temp;
		temp.Format(_T("line=%d, lineposbegin=%d, lineposend=%d"), line, lineposbegin, lineposend);
		InsertMsg(temp);
		sci.sci_SetSel(lineposend, lineposbegin);
	}
}

BOOL CDockXmlView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
			//UpdateFrameStatus();
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
		//UpdateFrameStatus();
		break;
	}	
	return __super::OnNotify(wParam, lParam, pResult);
}

CString CDockXmlView::GetNodeName()
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

void CDockXmlView::AutoCompleteNode(CString objectName)		//自动完成控件名
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

CString CDockXmlView::AutoCompleteProperty(CString objectName, CString AttrName)	//自动完成属性名
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

//////////////////////////////////////////////////////////////////////////
void CDockXmlView::OnEditUndo()
{
	sci.sci_Undo();
}


void CDockXmlView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanUndo());
}


void CDockXmlView::OnEditRedo()
{
	sci.sci_Redo();
}


void CDockXmlView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanRedo());
}


void CDockXmlView::OnEditCut()
{
	sci.sci_Cut();
}


void CDockXmlView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCut());
}


void CDockXmlView::OnEditCopy()
{
	sci.sci_Copy();
}


void CDockXmlView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanCopy());
}

void CDockXmlView::OnEditPaste()
{
	sci.sci_Paste();
}


void CDockXmlView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_CanPaste());
}

void CDockXmlView::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&pt);

	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_XML_PANE);
	theApp.GetContextMenuManager()->ShowPopupMenu(popmenu.GetSubMenu(0)->m_hMenu, point.x, point.y, AfxGetMainWnd(), TRUE);
}


void CDockXmlView::OnSciUpdateDesign()
{
	// TODO: 在此添加命令处理程序代码
}


void CDockXmlView::OnUpdateSciUpdateDesign(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(sci.sci_GetModify());
}
