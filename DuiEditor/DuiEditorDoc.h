
// DuiEditorDoc.h : CDuiEditorDoc 类的接口
//
#pragma once

class CUIManager;
class CDuiEditorDoc : public CDocument
{
protected: // 仅从序列化创建
	CDuiEditorDoc();
	DECLARE_DYNCREATE(CDuiEditorDoc)

// 特性
public:
	pugi::xml_document m_doc;		 //布局文件
	CStdStringPtrMap m_mLangPackage; //相关语言包
	BOOL m_bMenuWnd;

	CString m_strMyTitle;
	CString m_strSessionFile;
public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

// 操作
public:
	void FilterDefaultValue(xml_node nodeDoc);	//过滤文档默认属性
	void FilterPosWidthHeight(xml_node nodeDoc);

	CString GetSkinPath();
	CString GetSkinFileName();

	void InitFileView(CDocument *pDocCurrentClose);

	void LoadLangPackage(LPCTSTR lpszPathName);
	void SaveLangPackage(LPCTSTR lpszPathName);

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnNewDocumentFromUiTemplate();
#ifdef SHARED_HANDLERS
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CDuiEditorDoc();

private:
	CString m_strDefaultTitle;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDocumentEvent(DocumentEvent deEvent);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual BOOL IsModified();
	void SaveBackupFile();
	afx_msg void OnFileReopen();	
	afx_msg void OnEditInsertFont();
	afx_msg void OnEditInsertDefault();
	afx_msg void OnEditInsertStyleNode();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUiformDebug();
	afx_msg void OnUpdateUiformDebug(CCmdUI *pCmdUI);
	afx_msg void OnUiformDebugEnd();
	afx_msg void OnUpdateUiformDebugEnd(CCmdUI *pCmdUI);
	afx_msg void OnBuildLangPackage();
	afx_msg void OnBuildLangPackageEx();
	afx_msg void OnBuildLangStringTable();
};
