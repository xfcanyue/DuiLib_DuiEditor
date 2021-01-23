#pragma once
#include "DuiEditorDoc.h"
#include "ImageEditorFrame.h"
#include "ImageEditorView.h"

// CImageEditor 对话框
class CUIManager;
class CImageEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CImageEditor)

public:
	CImageEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageEditor();

// 对话框数据
	enum { IDD = IDD_DLG_IMAGE_EDITOR };

	CUIManager *m_pUIManager;
	CImageEditorFrame *m_pFrame;

	CMFCPropertyGridCtrl *m_pParentGrid;
	CMFCPropertyGridProperty *m_pParentProp;

	xml_document m_nodeImage;
	xml_node	 m_nodedata;

	CxImage m_image;	//载入的图片
	CRect m_rcImage;	//图片的大小

	CRect m_rcControl;		//背景的大小，也就是所属控件的大小
	CxImage m_imgControlX;	//背景图
public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }

	void SetAttributeValue(LPCTSTR szAttribute);
	CString GetAttributeValue();
	void SetControlImage(CxImage &img);
	void SetImageFile(LPCTSTR lpstrPathName);

	static BOOL svg_2_cximage(CxImage &cximg, LPCTSTR strPathName);
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
extern CImageEditor *g_pEditorImage;