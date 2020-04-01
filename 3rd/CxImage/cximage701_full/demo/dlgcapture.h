#if !defined(AFX_DlgCapture_H__950ECDE3_A3D3_11D5_8DD4_9ED5C4640338__INCLUDED_)
#define AFX_DlgCapture_H__950ECDE3_A3D3_11D5_8DD4_9ED5C4640338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCapture.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgCapture dialog
class DlgCapture : public CDialog
{
// Construction
public:
	CString m_SelectedTitle;
	HWND m_SelectedWnd;
	DlgCapture(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCapture)
	enum { IDD = IDD_CAPTURE };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCapture)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCapture)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgCapture_H__950ECDE3_A3D3_11D5_8DD4_9ED5C4640338__INCLUDED_)
