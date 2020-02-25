#pragma once
#include "DuiEditorDoc.h"
#include "DefaultEditorFrame.h"
#include "DefaultEditorView.h"

// CDefaultEditor 对话框

class CDefaultEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDefaultEditor)

public:
	CDefaultEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDefaultEditor();

// 对话框数据
	enum { IDD = IDD_DLG_DEFAULT_EDITOR };
	CDuiEditorDoc *m_pDoc;
	CDefaultEditorFrame *m_pFrame;

	xml_document m_nodeControl;
	CString m_strClassName;
public:
	void SetAttributeValue(LPCTSTR szControlName, LPCTSTR szAttribute);
	CString GetAttributeValue();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
