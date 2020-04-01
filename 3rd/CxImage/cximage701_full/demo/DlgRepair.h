#if !defined(AFX_DLGREPAIR_H__399C37C1_B3B5_11D6_BB83_93FE8A487F7B__INCLUDED_)
#define AFX_DLGREPAIR_H__399C37C1_B3B5_11D6_BB83_93FE8A487F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRepair.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgRepair dialog
class DlgRepair : public CDialog
{
// Construction
public:
	long m_ncs;

	DlgRepair(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgRepair)
	enum { IDD = IDD_REPAIR };
	CComboBox	m_cs;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	float	m_radius;
	int		m_iterations;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgRepair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgRepair)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREPAIR_H__399C37C1_B3B5_11D6_BB83_93FE8A487F7B__INCLUDED_)
