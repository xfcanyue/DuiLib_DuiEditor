#pragma once

// CScriptEditorDoc 文档

class CScriptEditorDoc : public CDocument
{
	DECLARE_DYNCREATE(CScriptEditorDoc)

public:
	CScriptEditorDoc();
	virtual ~CScriptEditorDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};
