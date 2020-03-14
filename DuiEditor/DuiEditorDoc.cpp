
// DuiEditorDoc.cpp : CDuiEditorDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DuiEditor.h"
#endif

#include "DuiEditorDoc.h"
#include "DuiEditorViewDesign.h"
#include "DuiEditorViewCode.h"

#include <propkey.h>

#include "MainFrm.h"
#include "DockControlTreeWnd.h"
#include "DockOutputWnd.h"
#include "DlgCreateDuiDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDuiEditorDoc

IMPLEMENT_DYNCREATE(CDuiEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDuiEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_REOPEN, &CDuiEditorDoc::OnFileReopen)
	ON_COMMAND(ID_EDIT_XML, &CDuiEditorDoc::OnEditXml)
	ON_UPDATE_COMMAND_UI(ID_EDIT_XML, &CDuiEditorDoc::OnUpdateEditXml)
	ON_UPDATE_COMMAND_UI(ID_FILE_REOPEN, &CDuiEditorDoc::OnUpdateFileReopen)
END_MESSAGE_MAP()


// CDuiEditorDoc 构造/析构

CDuiEditorDoc::CDuiEditorDoc()
{
	// TODO: 在此添加一次性构造代码
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	m_pTreeView = pMain->m_wndControl.CreateTreeView();
	m_pTreeView->m_pDoc = this;
	m_bMenuWnd = FALSE;
}

CDuiEditorDoc::~CDuiEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->m_wndControl.RemoveTreeView(m_pTreeView);
	InitFileView(this);
}

BOOL CDuiEditorDoc::OnNewDocument()
{
	CDlgCreateDuiDocument dlg;
	if(dlg.DoModal() != IDOK)
		return FALSE;
/*
	xml_node node, node1, node2, node3;

	if(dlg.m_nCreateStyle == CREATE_DIALOG_WINDOW)
	{
		xml_node nodeWindow = m_doc.root().append_child(_T("Window"));
		nodeWindow.append_attribute(_T("size")).set_value(_T("550,400"));
		nodeWindow.append_attribute(_T("sizebox")).set_value(_T("4,4,6,6"));
		nodeWindow.append_attribute(_T("caption")).set_value(_T("0,0,0,25"));
		nodeWindow.append_attribute(_T("roundcorner")).set_value(_T("5,5"));

		xml_node rootVert = nodeWindow.append_child(_T("VerticalLayout"));
		rootVert.append_attribute(_T("inset")).set_value(_T("1,1,1,1"));
		rootVert.append_attribute(_T("bkcolor")).set_value(_T("0xFFCFE7F4"));
		rootVert.append_attribute(_T("bordercolor")).set_value(_T("0xFF768D9B"));
		rootVert.append_attribute(_T("bordersize")).set_value(_T("1"));

		//////////////////////////////////////////////////////////////////////////
		//header
		xml_node nodeHori = rootVert.append_child(_T("HorizontalLayout"));
		nodeHori.append_attribute(_T("name")).set_value(_T("dialog_header"));
		nodeHori.append_attribute(_T("height")).set_value(_T("25"));
		nodeHori.append_attribute(_T("bkcolor")).set_value(_T("0xFFB8DFF2"));

		node1 = nodeHori.append_child(_T("HorizontalLayout"));
		node2 = node1.append_child(_T("Container"));
		node2.append_attribute(_T("name")).set_value(_T("dialog_icon"));
		node2.append_attribute(_T("width")).set_value(_T("25"));
		node3 = node1.append_child(_T("Label"));
		node3.append_attribute(_T("name")).set_value(_T("dialog_caption"));

		node1 = nodeHori.append_child(_T("HorizontalLayout"));
		node1.append_attribute(_T("width")).set_value(_T("97"));

		node = node1.append_child(_T("FadeButton"));
		node.append_attribute(_T("name")).set_value(_T("windowminbtn"));
		node.append_attribute(_T("width")).set_value(_T("26"));
		node.append_attribute(_T("height")).set_value(_T("17"));
		node.append_attribute(_T("normalimage")).set_value(_T("file='image\\sys\\sys_dlg_min.png' source='52,0,78,17'"));
		node.append_attribute(_T("hotimage")).set_value(_T("file='image\\sys\\sys_dlg_min.png' source='26,0,52,17'"));
		node.append_attribute(_T("pushedimage")).set_value(_T("file='image\\sys\\sys_dlg_min.png' source='0,0,26,17'"));

		node = node1.append_child(_T("FadeButton"));
		node.append_attribute(_T("name")).set_value(_T("windowmaxbtn"));
		node.append_attribute(_T("width")).set_value(_T("26"));
		node.append_attribute(_T("height")).set_value(_T("17"));
		node.append_attribute(_T("normalimage")).set_value(_T("file='image\\sys\\sys_dlg_max.png' source='52,0,78,17'"));
		node.append_attribute(_T("hotimage")).set_value(_T("file='image\\sys\\sys_dlg_max.png' source='26,0,52,17'"));
		node.append_attribute(_T("pushedimage")).set_value(_T("file='image\\sys\\sys_dlg_max.png' source='0,0,26,17'"));

		node = node1.append_child(_T("FadeButton"));
		node.append_attribute(_T("name")).set_value(_T("windowrestorebtn"));
		node.append_attribute(_T("width")).set_value(_T("26"));
		node.append_attribute(_T("height")).set_value(_T("17"));
		node.append_attribute(_T("normalimage")).set_value(_T("file='image\\sys\\sys_dlg_restore.png' source='52,0,78,17'"));
		node.append_attribute(_T("hotimage")).set_value(_T("file='image\\sys\\sys_dlg_restore.png' source='26,0,52,17'"));
		node.append_attribute(_T("pushedimage")).set_value(_T("file='image\\sys\\sys_dlg_restore.png' source='0,0,26,17'"));
		node.append_attribute(_T("visible")).set_value(false);

		node = node1.append_child(_T("FadeButton"));
		node.append_attribute(_T("name")).set_value(_T("windowclosebtn"));
		node.append_attribute(_T("width")).set_value(_T("45"));
		node.append_attribute(_T("height")).set_value(_T("17"));
		node.append_attribute(_T("normalimage")).set_value(_T("file='image\\sys\\sys_dlg_close.png' source='90,0,135,17'"));
		node.append_attribute(_T("hotimage")).set_value(_T("file='image\\sys\\sys_dlg_close.png' source='45,0,90,17'"));
		node.append_attribute(_T("pushedimage")).set_value(_T("file='image\\sys\\sys_dlg_close.png' source='0,0,45,17'"));

		//分割线1
		nodeHori = rootVert.append_child(_T("HorizontalLayout"));
		nodeHori.append_attribute(_T("height")).set_value(_T("2"));
		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));
		node = nodeHori.append_child(_T("HorizontalLayout"));
		node.append_attribute(_T("bkcolor")).set_value(_T("0xFFB8DFF2"));
		node.append_attribute(_T("bkcolor2")).set_value(_T("0xFFFFFFFF"));
		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));

		//body
		nodeHori = rootVert.append_child(_T("HorizontalLayout"));
		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));

		node = nodeHori.append_child(_T("HorizontalLayout"));
		node.append_attribute(_T("bkcolor")).set_value(_T("0xFFFFFFFF"));

		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));

		//分割线2
		nodeHori = rootVert.append_child(_T("HorizontalLayout"));
		nodeHori.append_attribute(_T("height")).set_value(_T("2"));
		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));
		node = nodeHori.append_child(_T("HorizontalLayout"));
		node.append_attribute(_T("bkcolor")).set_value(_T("0xFFFFFFFF"));
		node.append_attribute(_T("bkcolor2")).set_value(_T("0xFFCFE7F4"));
		node = nodeHori.append_child(_T("Control"));
		node.append_attribute(_T("width")).set_value(_T("5"));

		nodeHori = rootVert.append_child(_T("HorizontalLayout"));
		nodeHori.append_attribute(_T("name")).set_value(_T("dialog_bottom"));
		nodeHori.append_attribute(_T("height")).set_value(_T("50"));
		nodeHori.append_attribute(_T("inset")).set_value(_T("10,0,10,0"));
		nodeHori.append_attribute(_T("childpadding")).set_value(_T("10"));
		nodeHori.append_attribute(_T("childvalign")).set_value(_T("vcenter"));

		node = nodeHori.append_child(_T("Control"));
		node = nodeHori.append_child(_T("Button"));
		node.append_attribute(_T("name")).set_value(_T("btn_ok"));
		node.append_attribute(_T("width")).set_value(_T("100"));
		node.append_attribute(_T("height")).set_value(_T("30"));
		node.append_attribute(_T("hotbkcolor")).set_value(_T("0xFFB8CFE9"));
		node.append_attribute(_T("pushedbkcolor")).set_value(_T("0xFFC9DDF6"));
		node.append_attribute(_T("text")).set_value(_T("确定"));
		node.append_attribute(_T("bkcolor")).set_value(_T("0xFFDDDDDD"));
		node.append_attribute(_T("bordercolor")).set_value(_T("0xFF707070"));
		node.append_attribute(_T("bordersize")).set_value(_T("1"));
		node.append_attribute(_T("borderround")).set_value(_T("5,5"));
		node = nodeHori.append_child(_T("Button"));
		node.append_attribute(_T("name")).set_value(_T("btn_cancel"));
		node.append_attribute(_T("width")).set_value(_T("100"));
		node.append_attribute(_T("height")).set_value(_T("30"));
		node.append_attribute(_T("hotbkcolor")).set_value(_T("0xFFB8CFE9"));
		node.append_attribute(_T("pushedbkcolor")).set_value(_T("0xFFC9DDF6"));
		node.append_attribute(_T("text")).set_value(_T("取消"));
		node.append_attribute(_T("bkcolor")).set_value(_T("0xFFDDDDDD"));
		node.append_attribute(_T("bordercolor")).set_value(_T("0xFF707070"));
		node.append_attribute(_T("bordersize")).set_value(_T("1"));
		node.append_attribute(_T("borderround")).set_value(_T("5,5"));

		//nodeHori.append_attribute(_T("")).set_value(_T(""));
	}
*/

	CString strFile = g_strAppPath + _T("DuiTemplate\\") + dlg.m_strModuleName + _T("\\skin.xml");
	if(!m_doc.load_file(strFile))
	{
		AfxMessageBox(_T("载入模板页失败!"));
		xml_node nodeWindow = m_doc.root().append_child(_T("Window"));
		nodeWindow.append_attribute(_T("size")).set_value(_T("400,300"));
		nodeWindow.append_child(_T("VerticalLayout"));
	}
	else
	{
		
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	GetTreeView()->InitTreeContent();
	return TRUE;
}

// CDuiEditorDoc 序列化

void CDuiEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDuiEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDuiEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDuiEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
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
	m_doc.load_file(lpszPathName, XML_PARSER_OPTIONS);

	xml_node root = m_doc.child(_T("Window"));
	if(root)
	{
		xml_node node = root.child(_T("Menu"));
		if(node)
		{
			m_bMenuWnd = TRUE;
		}
	}
	return TRUE;
}


BOOL CDuiEditorDoc::DoFileSave()
{
	if(!__super::DoFileSave())	return FALSE;

	//InsertMsg(_T("CDuiEditorDoc::OnSaveDocument"));

	//过滤默认属性
	xml_node root = m_doc.root().child(_T("Window"));
	FilterDefaultValue(root);

	//创建一个拷贝, 然后保存拷贝
	xml_document doc;
	doc.root().append_copy(root);
	bool bSave = doc.save_file(GetPathName(), PUGIXML_TEXT("\t"), format_default, encoding_utf8);
	if(!bSave)	return FALSE;

	SetModifiedFlag(FALSE);
	return TRUE;
}


BOOL CDuiEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
// 	InsertMsg(_T("CDuiEditorDoc::OnSaveDocument"));
// 
// 	//过滤默认属性
// 	xml_node root = m_doc.root().child(_T("Window"));
// 	FilterDefaultValue(root);
// 
// 	//创建一个拷贝, 然后保存拷贝
// 	xml_document doc;
// 	doc.root().append_copy(root);
// 	bool bSave = doc.save_file(lpszPathName, PUGIXML_TEXT("\t"), format_default, encoding_utf8);
// 	if(!bSave)	return FALSE;
// 
// 	SetModifiedFlag(FALSE);

// 	CString strTemp;
// 	strTemp = lpszPathName;
// 	int nPos = strTemp.ReverseFind(_T('\\'));
// 	if(nPos != -1)
// 	{
// 		m_strSkinDir = strTemp.Left(nPos + 1);
// 		m_strFileName = strTemp.Right(strTemp.GetLength() - nPos - 1);
// 	}

//	GetDesignView()->m_Manager.GetManager()->SetResourcePath(m_strSkinDir);

//	return TRUE;
	return CDocument::OnSaveDocument(lpszPathName);
}


void CDuiEditorDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();
}

CDuiEditorViewDesign *CDuiEditorDoc::GetDesignView() const
{
	CView *pView;
	for (POSITION pos = GetFirstViewPosition(); pos != NULL;)
	{
		pView = GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CDuiEditorViewDesign)))
			return (CDuiEditorViewDesign *)pView;
	}
	return NULL;
}

CView *CDuiEditorDoc::GetCodeView() const
{
	CView *pView;
	for (POSITION pos = GetFirstViewPosition(); pos != NULL;)
	{
		pView = GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CDuiEditorViewCode)))
			return pView;
	}
	return NULL;
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
	xml_node nodeDuiProp = g_duiProp.FindControl(nodeDoc.name());
	for (xml_node nodeAttr=nodeDuiProp.first_child(); nodeAttr; nodeAttr=nodeAttr.next_sibling())
	{
		xml_attribute attr = nodeDoc.attribute(nodeAttr.attribute(_T("name")).value());
		if(attr)
		{
			if(attr.value() == '\0' || attr.value() == NULL)
			{
				nodeDoc.remove_attribute(attr);
			}
			else if(CompareString(attr.value(), nodeAttr.attribute(_T("default")).value()))
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

void CDuiEditorDoc::OnUpdateFileReopen(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(FALSE);
}



void CDuiEditorDoc::OnEditXml()
{
	
}


void CDuiEditorDoc::OnUpdateEditXml(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}

