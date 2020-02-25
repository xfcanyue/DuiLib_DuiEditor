#pragma once


// CDlgCustomControl 对话框

class CDlgCustomControl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCustomControl)

public:
	CDlgCustomControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCustomControl();

// 对话框数据
	enum { IDD = IDD_DLG_CUSTOM_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strClassName;
	CString m_strParentControl;
	CComboBox m_cbParentControl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnChangeEdit1();
};
