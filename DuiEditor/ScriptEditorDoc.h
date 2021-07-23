#pragma once
#include "MyDocument.h"

// CScriptEditorDoc ÎÄµµ

class CScriptEditorDoc : public CMyDocument
{
	DECLARE_DYNCREATE(CScriptEditorDoc)

public:
	CScriptEditorDoc();
	virtual ~CScriptEditorDoc();

	CMFCPropertyGridCtrl *m_pPropList;
	CString m_strSessionFile;
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};
