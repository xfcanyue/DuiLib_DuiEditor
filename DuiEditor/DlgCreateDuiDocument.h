#pragma once


// CDlgCreateDuiDocument 对话框

class CDlgCreateDuiDocument : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateDuiDocument)

public:
	CDlgCreateDuiDocument(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCreateDuiDocument();

// 对话框数据
	enum { IDD = IDD_DLG_CREATE_DUI_DOCUMENT };
	CString m_strModuleName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBox;
	CStatic m_staPicture;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeList1();
};
