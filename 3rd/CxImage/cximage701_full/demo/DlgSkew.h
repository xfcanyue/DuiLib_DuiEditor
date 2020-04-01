#if !defined(AFX_DLGSKEW_H__1450E80F_B2DF_4EB5_BEDC_4D8C6A609F7C__INCLUDED_)
#define AFX_DLGSKEW_H__1450E80F_B2DF_4EB5_BEDC_4D8C6A609F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSkew.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgSkew dialog

class DlgSkew : public CDialog
{
// Construction
public:
	DlgSkew(CWnd* pParent = NULL);   // standard constructor

	DWORD m_w;
	DWORD m_h;

// Dialog Data
	//{{AFX_DATA(DlgSkew)
	enum { IDD = IDD_SKEW };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	BOOL	m_bEnableInterpolation;
	long	m_pivotx;
	long	m_pivoty;
	float	m_skewx;
	float	m_skewy;
	float	m_anglex;
	float	m_angley;
	float	m_slopex;
	float	m_slopey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgSkew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateMyData();

	// Generated message map functions
	//{{AFX_MSG(DlgSkew)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSKEW_H__1450E80F_B2DF_4EB5_BEDC_4D8C6A609F7C__INCLUDED_)
