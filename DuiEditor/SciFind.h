#pragma once
#include "SciWnd.h"

// CSciFind 对话框

class CSciFind : public CDialogEx
{
	DECLARE_DYNAMIC(CSciFind)

public:
	CSciFind(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSciFind();

// 对话框数据
	enum { IDD = IDD_SCI_FIND };
	CSciWnd *m_pSciWnd;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual void PostNcDestroy();
	afx_msg void OnBtnRepaceCurrent();
	afx_msg void OnBtnReplaceAll();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnFindNext();
	afx_msg void OnBnClickedBtnFindPre();
};
