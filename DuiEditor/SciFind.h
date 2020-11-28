#pragma once
#include "Resource.h"
// CSciFind 对话框

#define FIND_RANGE_CURRENT	0		//查找当前文档
#define FIND_RANGE_OPENED	1		//查找当前已经打开的文档
#define FIND_RANGE_PROJECT	2		//查找整个项目

#define FIND_DIRECT_DOWN	0
#define FIND_DIRECT_UP		1
#define FIND_DIRECT_ALL		2

class CSciFind : public CDialogEx
{
	DECLARE_DYNAMIC(CSciFind)

public:
	CSciFind(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSciFind();

// 对话框数据
	enum { IDD = IDD_SCI_FIND };

	BOOL sciFind(CSciWnd *pSciWnd);
	BOOL sciReplaceCurrent(CSciWnd *pSciWnd);
	int sciReplaceAll(CSciWnd *pSciWnd);

	void SaveText();
	void LoadText();
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
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
};
