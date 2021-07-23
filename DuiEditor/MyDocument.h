#pragma once

// CMyDocument 文档

class CMyDocument : public CDocument
{
	DECLARE_DYNCREATE(CMyDocument)

public:
	CMyDocument();
	virtual ~CMyDocument();

	CString m_strDefaultTitle;
public:
	virtual void OnDocumentEvent(DocumentEvent deEvent);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL SaveModified();

	//保存缓存文件
	virtual void SaveBackupFile(){}

	DECLARE_MESSAGE_MAP()
};
