// MyDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "MyDocument.h"

#include "MainFrm.h"
// CMyDocument

IMPLEMENT_DYNCREATE(CMyDocument, CDocument)

CMyDocument::CMyDocument()
{
}

CMyDocument::~CMyDocument()
{
}


BEGIN_MESSAGE_MAP(CMyDocument, CDocument)
END_MESSAGE_MAP()

// CMyDocument 命令
void CMyDocument::OnDocumentEvent(DocumentEvent deEvent)
{
	CDocument::OnDocumentEvent(deEvent);

	switch (deEvent)
	{
	case onAfterNewDocument:
		{
			break;
		}

	case onAfterOpenDocument:
		{
			break;
		}

	case onAfterSaveDocument:
		{
			break;
		}

	case onAfterCloseDocument:
		{
			int opencount = ((CDuiEditorApp *)AfxGetApp())->GetOpenDocumentCount();
			//只剩最后一个文档了，这个关闭后，左侧切换到文件列表
			if(opencount == 1)
			{
				CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
				pMain->m_wndFileView.ShowPane(TRUE, FALSE,TRUE);
			}
			break;
		}
	}
}

BOOL CMyDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
	int opencount = ((CDuiEditorApp *)AfxGetApp())->GetOpenDocumentCount();
	if(opencount == 1)
	{
		g_proj.InitProject(CMyFilePath::GetPathName(lpszPathName));
	}
	return TRUE;
}

void CMyDocument::SetModifiedFlag(BOOL bModified)
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

	if(bModified)
	{
		SaveBackupFile();
	}

	CDocument::SetModifiedFlag(bModified);
}

BOOL CMyDocument::SaveModified()
{
	BOOL bRet = CDocument::SaveModified();
	g_session.CloseSession(GetPathName());
	return bRet;
}