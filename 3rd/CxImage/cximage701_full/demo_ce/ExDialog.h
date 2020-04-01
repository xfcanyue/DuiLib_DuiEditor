#if !defined(AFX_EXDIALOG_H__FEBF3AC9_C158_4D53_B072_CF4B1B5F8B09__INCLUDED_)
#define AFX_EXDIALOG_H__FEBF3AC9_C158_4D53_B072_CF4B1B5F8B09__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExDialog.h : header file
//

#include "ExDlgEngine.h"


//---------------------------------------------------------------------------
//
//	CExDialog dialog
//
//---------------------------------------------------------------------------


//! Extended dialog.
//
class CExDialog : public CDialog
{
// Construction
public:
	CExDialog();
	CExDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor

	//! Sets the dialog title using literal string or variable.
	void SetTitle(LPCTSTR pszTitle)	{m_strTitle = pszTitle;	}

	//! Sets the dialog title using string resource.
	void SetTitle(UINT nTitleID) {m_strTitle.LoadString(nTitleID);}

// Dialog Data
	//{{AFX_DATA(CExDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString			m_strTitle;		//!< The dialog title string.
	CExDlgEngine	m_dlgEng;		//!< The extended dialog object.

	void	PaintHeader		(CDC &dc)				{m_dlgEng.PaintHeader(this, dc, m_strTitle);}
	void	Underline		(CDC &dc, UINT *pArrID)	{m_dlgEng.Underline(this, dc, pArrID);		}

	int GetHeaderHeight() {return m_dlgEng.GetHeaderHeight();}

	//! Creates a date time picker control in the place of resource nID.
	BOOL CreateDateTime(CDateTimeCtrl &dtc, UINT nID, DWORD dwStyle);

	//! Sets a control's font to bold
	BOOL SetBold(UINT nID) {return m_dlgEng.SetBold(this, nID);}

	BOOL EnableControl(UINT nID, BOOL bEnable = TRUE);

	//! Virtual method called by the painting engine.
	virtual	void DoPaint(CPaintDC &dc);

	// Generated message map functions
	//{{AFX_MSG(CExDialog)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


inline
BOOL CExDialog::CreateDateTime(CDateTimeCtrl &dtc, UINT nID, DWORD dwStyle)
{
	return m_dlgEng.CreateDateTime(this, dtc, nID, dwStyle);
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXDIALOG_H__FEBF3AC9_C158_4D53_B072_CF4B1B5F8B09__INCLUDED_)
