#if !defined(AFX_DLGOPACITY_H__B7012944_0790_11D6_8DD7_00E07D8144D0__INCLUDED_)
#define AFX_DLGOPACITY_H__B7012944_0790_11D6_8DD7_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOpacity.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgOpacity dialog
class DlgOpacity : public CDialog
{
// Construction
public:
	DlgOpacity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgOpacity)
	enum { IDD = IDD_OPACITY };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	BYTE	m_level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgOpacity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgOpacity)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPACITY_H__B7012944_0790_11D6_8DD7_00E07D8144D0__INCLUDED_)
