#if !defined(AFX_DLGSHADOW_H__452586AD_E3F2_48BE_AB13_47C4B6438DA1__INCLUDED_)
#define AFX_DLGSHADOW_H__452586AD_E3F2_48BE_AB13_47C4B6438DA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShadow.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgShadow dialog

class DlgShadow : public CDialog
{
// Construction
public:
	DlgShadow(CWnd* pParent = NULL);   // standard constructor

	COLORREF m_shadow;
	COLORREF m_bkg;

// Dialog Data
	//{{AFX_DATA(DlgShadow)
	enum { IDD = IDD_SHADOW };
	CxTargetButton	m_btnBkg;
	CxTargetButton	m_btnShadow;
	CxTargetButton	m_canc;
	CxTargetButton	m_ok;
	long	m_x;
	long	m_y;
	BYTE	m_intensity;
	BOOL	m_relative;
	BYTE	m_radius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgShadow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgShadow)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHADOW_H__452586AD_E3F2_48BE_AB13_47C4B6438DA1__INCLUDED_)
