#pragma once


// CDlgOptions 对话框

class CDlgOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOptions)

public:
	CDlgOptions(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOptions();

// 对话框数据
	enum { IDD = IDD_DLG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	BOOL m_bUpdateSizeWhenModifyPos;
	UINT m_nTreeDeep;
	afx_msg void OnBnClickedButton1();
	CString m_strAttachTestCommand;
};
