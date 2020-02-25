#pragma once


// CDlgInsertControl 对话框

class CDlgInsertControl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInsertControl)

public:
	CDlgInsertControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInsertControl();

// 对话框数据
	enum { IDD = IDD_DLG_INSERT_CONTROL };

	int m_nPosition;
	xml_node nodeParent;	//父控件
	xml_node nodeControl;		//当前选中的控件
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
