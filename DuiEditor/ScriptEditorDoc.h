#pragma once

// CScriptEditorDoc ÎÄµµ

class CScriptEditorDoc : public CDocument
{
	DECLARE_DYNCREATE(CScriptEditorDoc)

public:
	CScriptEditorDoc();
	virtual ~CScriptEditorDoc();

	CMFCPropertyGridCtrl *m_pPropList;
	CString m_strDefaultTitle;
protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	void OnDocumentEvent(DocumentEvent deEvent);
	void InitFileView(CDocument *pDocCurrentClose);
	virtual void OnCloseDocument();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
};
