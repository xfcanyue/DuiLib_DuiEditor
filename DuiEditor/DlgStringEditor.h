#pragma once


// CDlgStringEditor 对话框

class CDlgStringEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStringEditor)

public:
	CDlgStringEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStringEditor();

// 对话框数据
	enum { IDD = IDD_DLG_STRING_EDITOR };

	CString m_string;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CRichEditCtrl m_ctrlRichEdit;
};
