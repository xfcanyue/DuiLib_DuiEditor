#if !defined(AFX_DLGURL_H__A5267584_6592_11D6_BB82_F6642F586A63__INCLUDED_)
#define AFX_DLGURL_H__A5267584_6592_11D6_BB82_F6642F586A63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgURL.h : header file
//
#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgURL dialog
//const UINT WM_HTTPDOWNLOAD_THREAD_FINISHED = WM_APP + 123;

class DlgURL : public CDialog
{
// Construction
public:
	DlgURL(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgURL)
	enum { IDD = IDD_DWNURL };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CString	m_url;
	CString m_data;
	long m_size;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgURL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgURL)
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void*   m_hInternetSession;
	void*   m_hHttpConnection;
	void*   m_hHttpFile;

	BOOL    m_bAbort;
	BOOL    m_bSafeToClose;
	CString m_sError;

	void SetStatus(LPCTSTR message);
	void ThreadError(LPCTSTR message);
	LRESULT OnThreadFinished(WPARAM wParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGURL_H__A5267584_6592_11D6_BB82_F6642F586A63__INCLUDED_)
