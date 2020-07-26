
// DuiEditorDoc.h : CDuiEditorDoc 类的接口
//
#pragma once
#include "DockControlTreeCtrl.h"
#include "DockXmlView.h"

class CDuiEditorTabView;
class CDuiEditorViewDesign;
class CDuiEditorDoc : public CDocument
{
protected: // 仅从序列化创建
	CDuiEditorDoc();
	DECLARE_DYNCREATE(CDuiEditorDoc)

// 特性
public:
	pugi::xml_document m_doc;
	BOOL m_bMenuWnd;

// 操作
public:
	void FilterDefaultValue(xml_node nodeDoc);	//过滤文档默认属性
	CDockControlTreeCtrl *GetTreeView() { return m_pTreeView; }
	CDockXmlView *GetXmlPane() const { return m_pSciWnd; }
	CDuiEditorViewDesign *GetDesignView() const;
	CView *GetCodeView() const;
	CDuiEditorTabView *GetTabView() const;

	CString GetSkinPath();
	CString GetSkinFileName();

	void InitFileView(CDocument *pDocCurrentClose);
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

protected:
	CDockControlTreeCtrl *m_pTreeView;
	CDockXmlView *m_pSciWnd;
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
	afx_msg void OnFileReopen();
};
