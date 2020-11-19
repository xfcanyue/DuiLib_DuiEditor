#pragma once


// CImageEditorPaneDialog 对话框

class CImageEditorAdjust : public CDialogEx
{
	DECLARE_DYNAMIC(CImageEditorAdjust)

public:
	CImageEditorAdjust(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageEditorAdjust();

	BOOL m_bDoEditChanged;
	BOOL m_bDoInitData;

	int GetIntPara(UINT nID);
	void InitData();
	void UpdatePara(RECT rc, LPCTSTR attrName);
	RECT GetAttributeRect(LPCTSTR attrName);
	void SetAttributeRect(LPCTSTR attrName, RECT &rc);
// 对话框数据
	enum { IDD = IDD_FORM_IMAGE_PANE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	afx_msg void OnChangeEditDestWidth();
	afx_msg void OnChangeEditDestHeight();
	afx_msg void OnChangeEditSourceWidth();
	afx_msg void OnChangeEditSourceHeight();
	afx_msg void OnChangeEditCornerWidth();
	afx_msg void OnChangeEditCornerHeight();

	afx_msg void OnBnClickedButtonDestLeft();
	afx_msg void OnBnClickedButtonDestRight();
	afx_msg void OnBnClickedButtonDestTop();
	afx_msg void OnBnClickedButtonDestBottom();

	afx_msg void OnBnClickedButtonSourceLeft();
	afx_msg void OnBnClickedButtonSourceRight();
	afx_msg void OnBnClickedButtonSourceTop();
	afx_msg void OnBnClickedButtonSourceBottom();

	afx_msg void OnBnClickedButtonCornerLeft();
	afx_msg void OnBnClickedButtonCornerRight();
	afx_msg void OnBnClickedButtonCornerTop();
	afx_msg void OnBnClickedButtonCornerBottom();
	afx_msg void OnBnClickedButtonDestReset();
	afx_msg void OnBnClickedButtonSourceReset();
	afx_msg void OnBnClickedButtonCornerReset();
};
