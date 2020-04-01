#if !defined(AFX_DLGCOLORIZE_H__CB0FCA81_FBC0_11D5_8DD7_00E07D8144D0__INCLUDED_)
#define AFX_DLGCOLORIZE_H__CB0FCA81_FBC0_11D5_8DD7_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgColorize.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgColorize dialog
class DlgColorize : public CDialog
{
// Construction
public:
	BYTE m_bHSL;
	BYTE m_bLinked;
	DlgColorize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgColorize)
	enum { IDD = IDD_COLORIZE };
	CButton	m_rbLink;
	CButton	m_rbHSL;
	CButton	m_rbSol;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CxTargetButton	m_colors;
	BYTE	m_hue;
	BYTE	m_sat;
	long	m_r;
	long	m_g;
	long	m_b;
	BYTE	m_blend;
	BYTE	m_sol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgColorize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgColorize)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnColors();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOLORIZE_H__CB0FCA81_FBC0_11D5_8DD7_00E07D8144D0__INCLUDED_)
