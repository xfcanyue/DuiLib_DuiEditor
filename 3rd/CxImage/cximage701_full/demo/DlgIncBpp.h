#if !defined(AFX_DLGINCBPP_H__55B7AF22_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_)
#define AFX_DLGINCBPP_H__55B7AF22_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIncBpp.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgIncBpp dialog
class DlgIncBpp : public CDialog
{
// Construction
public:
	BYTE m_bit;
	DlgIncBpp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgIncBpp)
	enum { IDD = IDD_INCBPP };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CButton	m_r4;
	CButton	m_r24;
	CButton	m_r8;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgIncBpp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgIncBpp)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINCBPP_H__55B7AF22_BB40_11D5_8DD4_00E07D8144D0__INCLUDED_)
