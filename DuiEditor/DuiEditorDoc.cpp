
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
#include "DlgBuildLanguagePackage.h"
#include "DlgBuildLanguagePackageEx.h"

#include "ThreadTest.h"
#include "ThreadPipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDuiEditorDoc
static CString s_defdocument = _T("<Window size=\"400,300\">\r\n\t<VerticalLayout bkcolor=\"0xFFA6CAF0\" />\r\n</Window>");


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
	ON_COMMAND(ID_BUILD_LANG_PACKAGE, &CDuiEditorDoc::OnBuildLangPackage)
	ON_COMMAND(ID_BUILD_LANG_PACKAGE_EX, &CDuiEditorDoc::OnBuildLangPackageEx)
	ON_COMMAND(ID_BUILD_LANG_STRING_TABLE, &CDuiEditorDoc::OnBuildLangStringTable)
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

	for (int i=0; i<m_mLangPackage.GetSize(); i++)
	{
		LPCTSTR key = m_mLangPackage.GetAt(i);
		xml_document *xml = (xml_document *)m_mLangPackage.Find(key);
		delete xml; xml = NULL;
	}
}

BOOL CDuiEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	m_doc.load_string(T2XML(s_defdocument));
	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

BOOL CDuiEditorDoc::OnNewDocumentFromUiTemplate()
{
	CDlgCreateDuiDocument dlg;
	if(dlg.DoModal() != IDOK) return FALSE;

	CString strFile = g_strAppPath + _T("template\\") + dlg.m_strModuleName + _T("\\skin.xml");
	if(!m_doc.load_file(strFile))
	{
		AfxMessageBox(_T("载入模板页失败!"));
		m_doc.load_string(T2XML(s_defdocument));
	}	

	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

#ifdef SHARED_HANDLERS
// 缩略图的支持
void CDuiEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
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
	//if (!CDocument::OnOpenDocument(lpszPathName))
	//	return FALSE;

	//AfxMessageBox(_T("BOOL CDuiEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)"));

	// TODO:  在此添加您专用的创建代码
	m_strSessionFile = g_session.GetSessionFile(lpszPathName);

	xml_parse_result ret = m_doc.load_file(m_strSessionFile, XML_PARSER_OPTIONS);
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
	LoadLangPackage(lpszPathName);

	if(m_strSessionFile != lpszPathName)
	{
		SetModifiedFlag(TRUE);
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

	SaveLangPackage(lpszPathName);

	GetUIManager()->GetCodeView()->GetSciWnd()->sci_SetSavePoint();
	SetModifiedFlag(FALSE);
	return TRUE;
}

void CDuiEditorDoc::LoadLangPackage(LPCTSTR lpszPathName)
{
	if(!g_cfg.bLangManager)
		return;

	CString langFile;
	CString strSkinDir = lpszPathName;
	int nPos = strSkinDir.ReverseFind(_T('\\'));
	if(nPos != -1)
	{
		langFile = strSkinDir.Right(strSkinDir.GetLength() - nPos-1);
		strSkinDir = strSkinDir.Left(nPos + 1);
	}
	if(strSkinDir.IsEmpty()) return;
	if(langFile.IsEmpty()) return;

	strSkinDir += g_cfg.strLangPath;
	strSkinDir += _T("\\");
	langFile = langFile.Left(langFile.ReverseFind('.')) + _T(".lng");

	CFileFind finder;
	BOOL bFind = finder.FindFile(strSkinDir + _T("*.*"));
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())	//是文件夹，检查名称，把里面文件删除
		{
			CString lang = finder.GetFileName();

			CString langPathName = strSkinDir + lang + _T("\\") + langFile;
			CFileFind finder;
			if(!finder.FindFile(langPathName))
			{
				continue;
			}

			xml_document *xml = new xml_document;
			xml_parse_result ret = xml->load_file(langPathName);	
			if(ret.status != pugi::status_ok) //解析失败
			{
				AfxMessageBox(_T("Parse language xml file failed"));
				delete xml;
				continue;
			}
			if(!xml->child("Language")) //根节点不是Language
			{
				delete xml;
				continue;
			}
			m_mLangPackage.Insert(lang, xml);
		}
	}
}

void CDuiEditorDoc::SaveLangPackage(LPCTSTR lpszPathName)
{
	if(!g_cfg.bLangManager)
		return;

	CString langFile;
	CString strSkinDir = lpszPathName;
	int nPos = strSkinDir.ReverseFind(_T('\\'));
	if(nPos != -1)
	{
		langFile = strSkinDir.Right(strSkinDir.GetLength() - nPos-1);
		strSkinDir = strSkinDir.Left(nPos + 1);
	}
	if(strSkinDir.IsEmpty()) return;
	if(langFile.IsEmpty()) return;

	strSkinDir += g_cfg.strLangPath;
	strSkinDir += _T("\\");
	langFile = langFile.Left(langFile.ReverseFind('.')) + _T(".lng");

	for (int i=0; i<m_mLangPackage.GetSize(); i++)
	{
		LPCTSTR key = m_mLangPackage.GetAt(i);
		xml_document *xml = (xml_document *)m_mLangPackage.Find(key);
		CString langPath = strSkinDir + key + _T("\\");
		CreateDirectory(langPath, NULL);
		xml->save_file(langPath + langFile);
	}
}

void CDuiEditorDoc::OnCloseDocument()
{
 	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->IsClosingNow()) //正常关闭文件，不要保存session
	{
		g_session.DeleteSession(GetPathName());
	}

	CDocument::OnCloseDocument();
	if(pMain->IsClosingNow()) return;

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
	pMain->m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
}

void CDuiEditorDoc::SetModifiedFlag(BOOL bModified)
{
	if(m_bModified != bModified)
	{
		if(GetPathName().IsEmpty())
		{
			m_strMyTitle = m_strDefaultTitle;
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

			m_strMyTitle = lpszTemp;
		}

		if(bModified)
			SetTitle(m_strMyTitle + " *");
		else
			SetTitle(m_strMyTitle);
	}

	if(IsModified())
	{
		SaveBackupFile();
	}
	__super::SetModifiedFlag(bModified);
}

void CDuiEditorDoc::SaveBackupFile()
{
	//没有保存过的文件，不要备份
	CString backupfile = g_session.GetSessionBackup(GetPathName());
	if(!backupfile.IsEmpty())
	{
		GetUIManager()->GetCodeView()->GetSciWnd()->SaveFile(backupfile);
	}
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

void CDuiEditorDoc::OnBuildLangPackage()
{
	CDlgBuildLanguagePackage dlg;
	dlg.SetUIManager(GetUIManager());
	dlg.DoModal();
}

void CDuiEditorDoc::OnBuildLangPackageEx()
{
	CDlgBuildLanguagePackageEx dlg;
	dlg.SetUIManager(GetUIManager());
	dlg.DoModal();
}


void CDuiEditorDoc::OnBuildLangStringTable()
{
	CString file = GetUIManager()->GetDocument()->GetSkinPath();
	if(file.IsEmpty())
	{
		AfxMessageBox(_T("请先保存文件。"));
		return;
	}
	file += _T("StringTable");

	static TCHAR BASED_CODE szFilter[] = _T("语言包文件(*.lng)|*.lng||");
	CFileDialog fileDlg(FALSE, _T("lng"), GetUIManager()->GetManager()->GetResourcePath() + (LPCTSTR)file,  
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	if(fileDlg.DoModal() != IDOK)	return;

	xml_document xml;
	xml_parse_result ret = xml.load_file(fileDlg.GetPathName());

	xml_node rootLang = xml.child_auto("Language");
	if(!rootLang.first_child())
	{
		xml_node node1 = rootLang.append_child("String");
		node1.append_attribute("id").set_value("1");
		node1.append_attribute("text1").set_value("");
		node1.append_attribute("text2").set_value("");
	}
	xml.save_file(fileDlg.GetPathName());
}
