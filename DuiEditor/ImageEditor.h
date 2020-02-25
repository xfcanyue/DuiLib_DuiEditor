#pragma once
#include "DuiEditorDoc.h"
#include "ImageEditorFrame.h"
#include "ImageEditorView.h"

// CImageEditor 对话框

class CImageEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CImageEditor)

public:
	CImageEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageEditor();

// 对话框数据
	enum { IDD = IDD_DLG_IMAGE_EDITOR };

	CDuiEditorDoc *m_pDoc;
	CImageEditorFrame *m_pFrame;

	CMFCPropertyGridCtrl *m_pParentGrid;
	CMFCPropertyGridProperty *m_pParentProp;

	xml_document m_nodeImage;
	CRect m_rcImage;
public:
	void SetAttributeValue(LPCTSTR szAttribute);
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
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
