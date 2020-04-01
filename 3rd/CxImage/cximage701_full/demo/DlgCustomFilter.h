#if !defined(AFX_DLGCUSTOMFILTER_H__874E0318_2478_4454_9BE5_6499C63E4147__INCLUDED_)
#define AFX_DLGCUSTOMFILTER_H__874E0318_2478_4454_9BE5_6499C63E4147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgCustomFilter.h : header file

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgCustomFilter dialog

class DlgCustomFilter : public CDialog
{
// Construction
public:
	DlgCustomFilter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCustomFilter)
	enum { IDD = IDD_CUSTOM_FILTER };
	CComboBox	m_cbPresets;
	CxTargetButton	m_load;
	CxTargetButton	m_save;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	long	m_Edit1x1;
	long	m_Edit1x2;
	long	m_Edit1x3;
	long	m_Edit1x4;
	long	m_Edit1x5;
	long	m_Edit2x1;
	long	m_Edit2x2;
	long	m_Edit2x3;
	long	m_Edit2x4;
	long	m_Edit2x5;
	long	m_Edit3x1;
	long	m_Edit3x2;
	long	m_Edit3x3;
	long	m_Edit3x4;
	long	m_Edit3x5;
	long	m_Edit4x1;
	long	m_Edit4x2;
	long	m_Edit4x3;
	long	m_Edit4x4;
	long	m_Edit4x5;
	long	m_Edit5x1;
	long	m_Edit5x2;
	long	m_Edit5x3;
	long	m_Edit5x4;
	long	m_Edit5x5;
	long	m_EditBias;
	long	m_EditDivisor;
	//}}AFX_DATA

	long	m_kSize;
	long	dlgkernel[25];
/*
	long	m_Edit1;
	long	m_Edit2;
	*/
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCustomFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle,DWORD dwFlags, BOOL bOpenFileDialog, int* pType);

	// Generated message map functions
	//{{AFX_MSG(DlgCustomFilter)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio3x3();
	afx_msg void OnRadio5x5();
	virtual void OnOK();
	afx_msg void OnLoad();
	afx_msg void OnSave();
	afx_msg void OnSelchangePresets();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCUSTOMFILTER_H__874E0318_2478_4454_9BE5_6499C63E4147__INCLUDED_)
