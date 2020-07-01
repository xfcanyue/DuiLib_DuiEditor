#pragma once

// CScriptEditorDoc ÎÄµµ

class CScriptEditorDoc : public CDocument
{
	DECLARE_DYNCREATE(CScriptEditorDoc)

public:
	CScriptEditorDoc();
	virtual ~CScriptEditorDoc();

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};
