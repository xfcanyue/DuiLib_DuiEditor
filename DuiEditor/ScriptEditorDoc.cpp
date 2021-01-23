// ScriptEditorDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "MainFrm.h"

// CScriptEditorDoc

IMPLEMENT_DYNCREATE(CScriptEditorDoc, CDocument)

CScriptEditorDoc::CScriptEditorDoc()
{
	m_bHasSaveSession = FALSE;
	m_bLoadFileFromBackup = FALSE;
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	m_pPropList = pMain->m_wndStack.CreateStackView();
}

BOOL CScriptEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strDefaultTitle = m_strTitle;
	return TRUE;
}

CScriptEditorDoc::~CScriptEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	pMain->m_wndStack.RemoveStackView(m_pPropList);
	m_pPropList = NULL;
}


BEGIN_MESSAGE_MAP(CScriptEditorDoc, CDocument)
END_MESSAGE_MAP()

// CScriptEditorDoc 命令


BOOL CScriptEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	CString filename = lpszPathName;

	xml_node nodeSession = g_cfg.Session();
	for (xml_node node = nodeSession.child("File"); node; node=node.next_sibling("File"))
	{
		if(CompareString(LSUTF82T(node.attribute("filename").as_string()), lpszPathName))
		{
			m_fileSession = node;
		}
	}
	if(!m_fileSession)
	{
		m_fileSession = g_cfg.Session().append_child("File");
	}

	//对比时间戳，判断是否载入备份文件
	BOOL bModify = m_fileSession.attribute("ismodify").as_bool();
	CString backup = LSUTF82T(m_fileSession.attribute("backup").as_string());
	if(!backup.IsEmpty() && PathFileExists(backup))
	{
		CFileStatus sta1;
		CFileStatus sta2;
		if(CFile::GetStatus(lpszPathName, sta1, NULL) && CFile::GetStatus(backup, sta2, NULL) && bModify)
		{
			if(sta1.m_mtime > sta2.m_mtime)
			{
				CString temp;
				temp.Format(_T("%s\r\n\r\n上次关闭软件没有保存文件，但是当前文件比备份新。 点\"是\"载入备份文件，点\"否\"载入当前文件。"), lpszPathName);
				if(AfxMessageBox(temp, MB_OKCANCEL) != IDOK)
				{
					m_bLoadFileFromBackup = TRUE;
					filename = backup;
				}
			}
			else if(sta1.m_mtime < sta2.m_mtime)
			{
				CString temp;
				temp.Format(_T("%s\r\n\r\n由于上次关闭软件没有保存文件，是否选择从备份载入？"), lpszPathName);
				if(AfxMessageBox(temp, MB_OKCANCEL) == IDOK)
				{
					m_bLoadFileFromBackup = TRUE;
					filename = backup;
				}
			}
		}
	}

	m_strLoadFileName = filename;
	return TRUE;
}


BOOL CScriptEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRet = FALSE;
	POSITION pos = GetFirstViewPosition();
	CScriptEditorView *pFirstView = (CScriptEditorView*)GetNextView( pos ); // assume only one view
	if (pFirstView != NULL)
	{
		bRet = pFirstView->sci.SaveFile(lpszPathName);

		if(bRet) 
		{
			SetModifiedFlag(FALSE);
			pFirstView->sci.sci_SetSavePoint();
		}
	}
	return TRUE;
	//return CDocument::OnSaveDocument(lpszPathName);
}

void CScriptEditorDoc::OnDocumentEvent(DocumentEvent deEvent)
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

void CScriptEditorDoc::InitFileView(CDocument *pDocCurrentClose)
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

void CScriptEditorDoc::OnCloseDocument()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(pMain->IsClosingNow())
	{
		if(m_fileSession && !m_bHasSaveSession)
		{
			CString filename = GetPathName();
			if(filename.IsEmpty()) filename = m_strDefaultTitle;
			m_fileSession.attribute_auto("filename").set_value(LST2UTF8(filename));
			g_cfg.SaveConfig();
			m_bHasSaveSession = TRUE;
		}
	}
	else //正常关闭文件，不要保存session
	{
		if(m_fileSession)
		{
			g_cfg.Session().remove_child(m_fileSession);
			g_cfg.SaveConfig();
		}
	}

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
	pMain->m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
}

void CScriptEditorDoc::SetModifiedFlag(BOOL bModified)
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

	if(m_fileSession.attribute("ismodify").as_bool() != (bModified==TRUE))
	{
		m_fileSession.attribute_auto("ismodify").set_value(bModified);
		g_cfg.SaveConfig();
	}

	__super::SetModifiedFlag(bModified);
}