#if !defined(AFX_DLGDECBPP_H__55B7AF21_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_)
#define AFX_DLGDECBPP_H__55B7AF21_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDecBpp.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgDecBpp dialog
class DlgDecBpp : public CDialog
{
// Construction
public:
	BYTE m_method;
	BYTE m_bit;
	BYTE m_errordiffusion;
	BYTE m_bLimitColors;
	DlgDecBpp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgDecBpp)
	enum { IDD = IDD_DECBPP };
	CButton	m_limitcolors;
	CButton	m_err;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CButton	m_opt;
	CButton	m_r8;
	CButton	m_r4;
	CButton	m_r1;
	long	m_maxcolors;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDecBpp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgDecBpp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDECBPP_H__55B7AF21_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_)
