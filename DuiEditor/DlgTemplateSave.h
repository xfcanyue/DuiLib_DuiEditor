#pragma once
#include "StaticImageView.h"

// CDlgTemplateSave 对话框

class CDlgTemplateSave : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTemplateSave)

public:
	CDlgTemplateSave(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTemplateSave();

	CDuiEditorDoc *m_pDoc;
// 对话框数据
	enum { IDD = IDD_DLG_TEMPLATE_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImage m_img;
	CStatic m_staPicture;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strTemplateName;
	afx_msg void OnPaint();
};
