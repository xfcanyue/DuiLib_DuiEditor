#if !defined(AFX_DLGTHRESHOLD_H__FCFC4DC2_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_)
#define AFX_DLGTHRESHOLD_H__FCFC4DC2_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgThreshold.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgThreshold dialog
class DlgThreshold : public CDialog
{
// Construction
public:
	DlgThreshold(CWnd* pParent = NULL);   // standard constructor

	BYTE m_bPreserve;

// Dialog Data
	//{{AFX_DATA(DlgThreshold)
	enum { IDD = IDD_THRESHOLD };
	CButton	m_cPreserve;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	BYTE	m_level;
	BYTE	m_mean;
	long	m_thresh1;
	long	m_thresh2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgThreshold)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgThreshold)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTHRESHOLD_H__FCFC4DC2_FB70_11D5_8DD7_00E07D8144D0__INCLUDED_)
