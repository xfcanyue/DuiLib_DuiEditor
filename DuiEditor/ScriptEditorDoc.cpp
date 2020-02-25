// ScriptEditorDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "ScriptEditorDoc.h"


// CScriptEditorDoc

IMPLEMENT_DYNCREATE(CScriptEditorDoc, CDocument)

CScriptEditorDoc::CScriptEditorDoc()
{
}

BOOL CScriptEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CScriptEditorDoc::~CScriptEditorDoc()
{
}


BEGIN_MESSAGE_MAP(CScriptEditorDoc, CDocument)
END_MESSAGE_MAP()

#ifndef _WIN32_WCE
// CScriptEditorDoc 序列化

void CScriptEditorDoc::Serialize(CArchive& ar)
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
#endif


// CScriptEditorDoc 命令


BOOL CScriptEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码

	return TRUE;
}


BOOL CScriptEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDocument::OnSaveDocument(lpszPathName);
}


void CScriptEditorDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDocument::OnCloseDocument();
}
