
// DuiEditorDoc.cpp : CDuiEditorDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DuiEditor.h"
#endif

#include "DuiEditorDoc.h"
#include "DuiEditorView.h"
#include "DuiEditorViewDesign.h"
#include "DuiEditorViewCode.h"

#include <propkey.h>

#include "MainFrm.h"
#include "UIManager.h"
#include "DlgCreateDuiDocument.h"

#include "ThreadTest.h"
#include "ThreadPipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDuiEditorDoc

IMPLEMENT_DYNCREATE(CDuiEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDuiEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_REOPEN, &CDuiEditorDoc::OnFileReopen)
	ON_COMMAND(ID_EDIT_INSERT_FONT_NODE, &CDuiEditorDoc::OnEditInsertFont)
	ON_COMMAND(ID_EDIT_INSERT_STYLE_NODE, &CDuiEditorDoc::OnEditInsertStyleNode)
	ON_COMMAND(ID_EDIT_INSERT_DEFAULT_NODE, &CDuiEditorDoc::OnEditInsertDefault)
	ON_COMMAND(ID_EDIT_UNDO, &CDuiEditorDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDuiEditorDoc::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDuiEditorDoc::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDuiEditorDoc::OnUpdateEditRedo)
	ON_COMMAND(ID_UIFORM_DEBUG, &CDuiEditorDoc::OnUiformDebug)
	ON_UPDATE_COMMAND_UI(ID_UIFORM_DEBUG, &CDuiEditorDoc::OnUpdateUiformDebug)
	ON_COMMAND(ID_UIFORM_DEBUG_END, &CDuiEditorDoc::OnUiformDebugEnd)
	ON_UPDATE_COMMAND_UI(ID_UIFORM_DEBUG_END, &CDuiEditorDoc::OnUpdateUiformDebugEnd)
END_MESSAGE_MAP()


// CDuiEditorDoc 构造/析构

CDuiEditorDoc::CDuiEditorDoc()
{
	// TODO: 在此添加一次性构造代码
	m_bMenuWnd = FALSE;

	CUIManager *pManager = new CUIManager;
	SetUIManager(pManager);

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	CDockControlTreeCtrl *pTreeView = pMain->m_wndControl.CreateTreeView();
	pTreeView->SetUIManager(GetUIManager());

	CUIPropertyGridCtrl *pPropList = pMain->m_wndProperty.CreatePropList();
	pPropList->SetUIManager(GetUIManager());

	GetUIManager()->_setPorpList(pPropList);
	GetUIManager()->_setControllTree(pTreeView);
	GetUIManager()->_setDocument(this);
}

CDuiEditorDoc::~CDuiEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->m_wndProperty.RemovePropList(GetUIManager()->GetPropList());
	GetUIManager()->_setPorpList(NULL);

	pMain->m_wndControl.RemoveTreeView(GetUIManager()->GetTreeView());
	GetUIManager()->_setControllTree(NULL);

	InitFileView(this);

	CUIManager *pUIManager = GetUIManager();
	if(pUIManager)
	{
		delete pUIManager;
		SetUIManager(NULL);
	}
}

BOOL CDuiEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	xml_node nodeWindow = m_doc.child_auto(XTEXT("Window"));
	nodeWindow.append_attribute(XTEXT("size")).set_value(XTEXT("400,300"));
	nodeWindow.append_child(XTEXT("VerticalLayout"));

	//GetUIManager()->GetTreeView()->InitTreeContent();
	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

BOOL CDuiEditorDoc::OnNewDocumentFromUiTemplate()
{
	CDlgCreateDuiDocument dlg;
	if(dlg.DoModal() != IDOK) return FALSE;

	CString strFile = g_strAppPath + _T("DuiTemplate\\") + dlg.m_strModuleName + _T("\\skin.xml");
	if(!m_doc.load_file(strFile))
	{
		AfxMessageBox(_T("载入模板页失败!"));
		xml_node nodeWindow = m_doc.root().append_child(XTEXT("Window"));
		nodeWindow.append_attribute(XTEXT("size")).set_value(XTEXT("400,300"));
		nodeWindow.append_child(XTEXT("VerticalLayout"));
	}	

	//GetUIManager()->GetTreeView()->InitTreeContent();
	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

#ifdef SHARED_HANDLERS
// 缩略图的支持
void CDuiEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
// 	CView *pView;
// 	for (POSITION pos = GetFirstViewPosition(); pos != NULL;)
// 	{
// 		pView = GetNextView(pos);
// 		if(pView->IsKindOf(RUNTIME_CLASS(CDuiEditorViewDesign)))
// 		{
// 			CDuiEditorViewDesign *pViewDesign = (CDuiEditorViewDesign *)pView;
// 
// 			CImage image;
// 			CControlUI *pRoot = pViewDesign->m_Manager.GetManager()->GetRoot();
// 			CSize szForm = pViewDesign->m_Manager.GetManager()->GetInitSize();
// 			image.Create(szForm.cx, szForm.cy, 32);
// 			CRect rcPaint(0,0,szForm.cx,szForm.cy);
// 			pRoot->DoPaint(image.GetDC(), rcPaint, NULL);
// 			image.Draw(dc.m_hDC, rcPaint);
// 			image.ReleaseDC();
// 		}
// 	}	
}
#endif // SHARED_HANDLERS

// CDuiEditorDoc 命令
void CDuiEditorDoc::OnDocumentEvent(DocumentEvent deEvent)
{
	__super::OnDocumentEvent(deEvent);

	switch (deEvent)
	{
	case onAfterNewDocument:
		{
			break;
		}

	case onAfterOpenDocument:
		{
			InitFileView(NULL);
			break;
		}

	case onAfterSaveDocument:
		{
			InitFileView(NULL);
			break;
		}

	case onAfterCloseDocument:
		{
			//InitFileView();
			break;
		}
	}
}

void CDuiEditorDoc::InitFileView(CDocument *pDocCurrentClose)
{
	CDocument *pDoc = NULL;
	int nCount = 0;
	POSITION pos = GetDocTemplate()->GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument *p = GetDocTemplate()->GetNextDoc(pos);
		if(p != pDocCurrentClose)
		{
			pDoc = p;
			nCount++;
		}
	}

	if(pDoc && nCount == 1)
	{
		CString strPath;
		CString strTemp = pDoc->GetPathName();
		int nPos = strTemp.ReverseFind(_T('\\'));
		if(nPos != -1)
		{
			strPath = strTemp.Left(nPos + 1);
		}

		if(!strPath.IsEmpty())
		{
			((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_wndFileView.InitFolder(strPath);
		}
	}
}

BOOL CDuiEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	xml_parse_result ret = m_doc.load_file(lpszPathName, XML_PARSER_OPTIONS);
	if(ret.status != pugi::status_ok)
	{
		LSSTRING_CONVERSION;
		CString temp;
		temp.Format(_T("解析错误: \r\n位置: %d \r\n错误信息: %s"), 
			ret.offset, LSA2T(ret.description()));
		AfxMessageBox(temp);
	}

	xml_node root = m_doc.child(XTEXT("Window"));
	if(root)
	{
		xml_node node = root.child(XTEXT("Menu"));
		if(node)
		{
			m_bMenuWnd = TRUE;
		}
	}
	return TRUE;
}

BOOL CDuiEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	GetUIManager()->GetCodeView()->UpdateDocument();

	//过滤默认属性
	xml_node root = m_doc.root().child(XTEXT("Window"));
	FilterDefaultValue(root);
	FilterPosWidthHeight(root);

	bool bSave = m_doc.save_file(lpszPathName, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
	if(!bSave)	return FALSE;

	GetUIManager()->GetCodeView()->GetSciWnd()->sci_SetSavePoint();
	SetModifiedFlag(FALSE);
	return TRUE;
}


void CDuiEditorDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();

	POSITION pos = ((CDuiEditorApp *)AfxGetApp())->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		CDocTemplate *pDocTemplate = ((CDuiEditorApp *)AfxGetApp())->GetNextDocTemplate(pos);			
		POSITION pos1 = pDocTemplate->GetFirstDocPosition();
		if(pos1 != NULL)
		{
			return;
		}
	}

	//如果没有打开任何文档，左侧切换到文件列表
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
}

void CDuiEditorDoc::SetModifiedFlag(BOOL bModified)
{
	if(m_bModified != bModified)
	{
		CString strTitle;
		if(GetPathName().IsEmpty())
		{
			strTitle = m_strDefaultTitle;
		}
		else
		{
			LPCTSTR lpszPathName = (LPCTSTR)GetPathName();

			// always capture the complete file name including extension (if present)
			LPTSTR lpszTemp = (LPTSTR)lpszPathName;
			for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
			{
				// remember last directory/drive separator
				if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
					lpszTemp = (LPTSTR)_tcsinc(lpsz);
			}

			strTitle = lpszTemp;
		}

		if(bModified)
			SetTitle(strTitle + " *");
		else
			SetTitle(strTitle);
	}

	__super::SetModifiedFlag(bModified);
}

BOOL CDuiEditorDoc::IsModified()
{
	return GetUIManager()->GetCodeView()->GetSciWnd()->sci_GetModify() || m_bModified;
}

CString CDuiEditorDoc::GetSkinPath()
{
	CString strTemp;
	strTemp = GetPathName();
	int nPos = strTemp.ReverseFind(_T('\\'));
	if(nPos != -1)
	{
		return strTemp.Left(nPos + 1);
	}

	return _T("");
}

CString CDuiEditorDoc::GetSkinFileName()
{
	CString strTemp;
	strTemp = GetPathName();
	int nPos = strTemp.ReverseFind(_T('\\'));
	if(nPos != -1)
	{
		return strTemp.Right(strTemp.GetLength() - nPos - 1);
	}

	return _T("");
}

void CDuiEditorDoc::FilterDefaultValue(xml_node nodeDoc)
{
	xml_node nodeDuiProp = g_duiProp.FindControl(XML2T(nodeDoc.name()));
	for (xml_node nodeAttr=nodeDuiProp.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
	{
		xml_attribute attr = nodeDoc.attribute(nodeAttr.attribute(XTEXT("name")).value());
		if(attr)
		{
			if(attr.value() == '\0' || attr.value() == NULL)
			{
				nodeDoc.remove_attribute(attr);
			}
			else if(CompareString(attr.value(), nodeAttr.attribute(XTEXT("default")).value()))
			{
				nodeDoc.remove_attribute(attr);
			}
		}
	}

	for (pugi::xml_node node = nodeDoc.first_child(); node; node = node.next_sibling())
	{
		FilterDefaultValue(node);
	}
}

void CDuiEditorDoc::FilterPosWidthHeight(xml_node nodeDoc)
{
	if(!g_cfg.bFilterPosWidthHeight) return;

	//float控件不处理
	if(!nodeDoc.attribute(XTEXT("float")).as_bool())
	{
		//如果没有pos属性，不处理。
		xml_attribute attrPos = nodeDoc.attribute(XTEXT("pos"));
		if(attrPos) 
		{
			xml_attribute attrWidth = nodeDoc.attribute(XTEXT("width"));
			xml_attribute attrHeight = nodeDoc.attribute(XTEXT("height"));

			//pos属性无法转换成rect，不处理
			CDuiRect rc;
			if(rc.FromString(XML2T(attrPos.as_string())))
			{
				//pos属性的大小 不等于 width或height属性，不处理。 因为手写的同学可能写错了，不要擅自处理。
				if(rc.GetWidth() == attrWidth.as_int() && rc.GetHeight() == attrHeight.as_int())
				{
					//好了，把pos属性去掉吧
					nodeDoc.remove_attribute(attrPos);
				}	
			}	
		}
	}

	for (pugi::xml_node node = nodeDoc.first_child(); node; node = node.next_sibling())
	{
		FilterPosWidthHeight(node);
	}
}

void CDuiEditorDoc::OnFileReopen()
{
	if(!SaveModified())
		return;

	CString strFile = GetPathName();// + m_strFileName;
	if(!strFile.IsEmpty())
	{
		AfxGetMainWnd()->SendMessage(WM_REOPEN_FILE, (WPARAM)(LPCTSTR)strFile, (LPARAM)this);
	}

	OnFileClose();
	return;
}	

//////////////////////////////////////////////////////////////////////////

void CDuiEditorDoc::OnEditInsertFont()
{
	xml_node nodeWinodw = m_doc.child(XTEXT("Window"));
	xml_node nodeFont;
	BOOL bAdd = FALSE;
	for (xml_node node=nodeWinodw.last_child(); node; node=node.previous_sibling())
	{
		if(CompareString(XTEXT("Font"), node.name()))
		{
			nodeFont = nodeWinodw.insert_child_after(XTEXT("Font"), node);
			GetUIManager()->GetTreeView()->AddNewControl(nodeFont, node, TVI_NEXT);
			bAdd = TRUE;
			break;
		}
	}
	if(!bAdd)
	{
		nodeFont = nodeWinodw.prepend_child(XTEXT("Font"));
		GetUIManager()->GetTreeView()->AddNewControl(nodeFont, TVI_FIRST);
	}

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	GetUIManager()->GetCodeView()->AddNode(nodeFont);
	g_duiProp.AddAttribute(nodeFont, _T("id"), 0,				GetUIManager());
	g_duiProp.AddAttribute(nodeFont, _T("name"), _T("宋体"),	GetUIManager());
	g_duiProp.AddAttribute(nodeFont, _T("size"), 12,			GetUIManager());

	GetUIManager()->GetPropList()->InitProp(nodeFont);
}

void  CDuiEditorDoc::OnEditInsertDefault()
{
	xml_node nodeWinodw = m_doc.child(XTEXT("Window"));
	xml_node nodeDefault;
	BOOL bAdd = FALSE;
	for (xml_node node=nodeWinodw.last_child(); node; node=node.previous_sibling())
	{
		if(CompareString(XTEXT("Default"), node.name()))
		{
			nodeDefault = nodeWinodw.insert_child_after(XTEXT("Default"), node);
			GetUIManager()->GetTreeView()->AddNewControl(nodeDefault, node, TVI_NEXT);
			bAdd = TRUE;
			break;
		}
	}
	if(!bAdd)
	{
		nodeDefault = nodeWinodw.prepend_child(XTEXT("Default"));
		GetUIManager()->GetTreeView()->AddNewControl(nodeDefault, TVI_FIRST);
	}

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	GetUIManager()->GetCodeView()->AddNode(nodeDefault);
	GetUIManager()->GetPropList()->InitProp(nodeDefault);
}

void  CDuiEditorDoc::OnEditInsertStyleNode()
{
	xml_node nodeWinodw = m_doc.child(XTEXT("Window"));
	xml_node nodeStyle;
	BOOL bAdd = FALSE;
	for (xml_node node=nodeWinodw.last_child(); node; node=node.previous_sibling())
	{
		if(CompareString(XTEXT("Style"), node.name()))
		{
			nodeStyle = nodeWinodw.insert_child_after(XTEXT("Style"), node);
			GetUIManager()->GetTreeView()->AddNewControl(nodeStyle, node, TVI_NEXT);
			bAdd = TRUE;
			break;
		}
	}
	if(!bAdd)
	{
		nodeStyle = nodeWinodw.prepend_child(XTEXT("Style"));
		GetUIManager()->GetTreeView()->AddNewControl(nodeStyle, TVI_FIRST);
	}

	CSciUndoBlock lock(GetUIManager()->GetCodeView()->GetSciWnd());
	GetUIManager()->GetCodeView()->AddNode(nodeStyle);
	g_duiProp.AddAttribute(nodeStyle, _T("name"), _T(""),	GetUIManager());
	g_duiProp.AddAttribute(nodeStyle, _T("class"), _T(""),	GetUIManager());

	GetUIManager()->GetPropList()->InitProp(nodeStyle);
}


void CDuiEditorDoc::OnEditUndo()
{
	GetUIManager()->GetCodeView()->GetSciWnd()->sci_Undo();
}


void CDuiEditorDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetCodeView()->GetSciWnd()->sci_CanUndo());
}


void CDuiEditorDoc::OnEditRedo()
{
	GetUIManager()->GetCodeView()->GetSciWnd()->sci_Redo();
}


void CDuiEditorDoc::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetUIManager()->GetCodeView()->GetSciWnd()->sci_CanRedo());
}


void CDuiEditorDoc::OnUiformDebug()
{
	AfxGetApp()->SaveAllModified();

	g_pThreadTest = new CThreadTest;
	g_pThreadTest->m_nTestFrom = 0;
	g_pThreadTest->m_pDoc = this;
	g_pThreadTest->CreateThread();
	return;
}

void CDuiEditorDoc::OnUpdateUiformDebug(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hThreadTestHandle == NULL);
}

void CDuiEditorDoc::OnUiformDebugEnd()
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

void CDuiEditorDoc::OnUpdateUiformDebugEnd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_hThreadTestHandle != NULL);
}
