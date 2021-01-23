#pragma once


// CDlgScriptTest 对话框

class CDlgScriptTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScriptTest)

public:
	CDlgScriptTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgScriptTest();

// 对话框数据
	enum { IDD = IDD_DLG_SCRIPT_TEST };
	static COleDateTime m_t1;
	static COleDateTime m_t2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	COleDateTime m_dtBeginDate;
	COleDateTime m_dtBeginTime;
	COleDateTime m_dtEndDate;
	COleDateTime m_dtEndTime;
};
