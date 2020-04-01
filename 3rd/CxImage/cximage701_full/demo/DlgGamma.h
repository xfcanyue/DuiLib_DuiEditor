#if !defined(AFX_DLGGAMMA_H__16A60641_A8B3_11D6_BB83_444553540000__INCLUDED_)
#define AFX_DLGGAMMA_H__16A60641_A8B3_11D6_BB83_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGamma.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgGamma dialog

class DlgGamma : public CDialog
{
// Construction
public:
	DlgGamma(CWnd* pParent = NULL);   // standard constructor

	BYTE	m_bGammaMode;

// Dialog Data
	//{{AFX_DATA(DlgGamma)
	enum { IDD = IDD_GAMMA };
	CButton	m_rbIndep;
	CButton	m_rbLink;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	float	m_gamma;
	float	m_gammaR;
	float	m_gammaG;
	float	m_gammaB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgGamma)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgGamma)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGAMMA_H__16A60641_A8B3_11D6_BB83_444553540000__INCLUDED_)
