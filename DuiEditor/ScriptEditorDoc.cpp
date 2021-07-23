// ScriptEditorDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorDoc.h"
#include "ScriptEditorView.h"
#include "MainFrm.h"

// CScriptEditorDoc

IMPLEMENT_DYNCREATE(CScriptEditorDoc, CMyDocument)

CScriptEditorDoc::CScriptEditorDoc()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	m_pPropList = pMain->m_wndStack.CreateStackView();
}

BOOL CScriptEditorDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
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


BEGIN_MESSAGE_MAP(CScriptEditorDoc, CMyDocument)
END_MESSAGE_MAP()

// CScriptEditorDoc 命令


BOOL CScriptEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CMyDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_strSessionFile = g_session.GetSessionFile(lpszPathName);	
	if(m_strSessionFile != lpszPathName)
	{
		SetModifiedFlag(TRUE);
	}
	g_session.OpenSession(lpszPathName);
	// TODO:  在此添加您专用的创建代码
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

void CScriptEditorDoc::OnCloseDocument()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->IsClosingNow()) //正常关闭文件，不要保存session
	{
		g_session.DeleteSession(GetPathName());
	}

	CMyDocument::OnCloseDocument();
}
