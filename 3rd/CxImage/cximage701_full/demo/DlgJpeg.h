#if !defined(AFX_DLGJPEG_H__578E3AEA_8D08_432D_801C_BA0FA97E9E4C__INCLUDED_)
#define AFX_DLGJPEG_H__578E3AEA_8D08_432D_801C_BA0FA97E9E4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgJpeg.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgJpeg dialog

class DlgJpeg : public CDialog
{
// Construction
public:
	DlgJpeg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgJpeg)
	enum { IDD = IDD_JPEG };
	CButton	m_r1;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	float	m_quality;
	long	m_format;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgJpeg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgJpeg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGJPEG_H__578E3AEA_8D08_432D_801C_BA0FA97E9E4C__INCLUDED_)
