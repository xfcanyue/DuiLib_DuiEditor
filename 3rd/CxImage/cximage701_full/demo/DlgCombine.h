#if !defined(AFX_DLGCOMBINE_H__6619A9E1_AF0A_11D6_BB83_DFD6D1F64D7F__INCLUDED_)
#define AFX_DLGCOMBINE_H__6619A9E1_AF0A_11D6_BB83_DFD6D1F64D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCombine.h : header file
//

#include "xTargetButton.h"

class CDemoDoc;
/////////////////////////////////////////////////////////////////////////////
// DlgCombine dialog
class DlgCombine : public CDialog
{
// Construction
public:
	CDemoDoc* pDocR;
	CDemoDoc* pDocG;
	CDemoDoc* pDocB;
	CDemoDoc* pDocA;
	long	pChR;
	long	pChG;
	long	pChB;
	long	pChA;
	long	pChS;

	DlgCombine(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgCombine)
	enum { IDD = IDD_COMBINE };
	CComboBox	m_chs;
	CComboBox	m_cha;
	CComboBox	m_chb;
	CComboBox	m_chg;
	CComboBox	m_chr;
	CComboBox	m_imga;
	CComboBox	m_imgb;
	CComboBox	m_imgg;
	CComboBox	m_imgr;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgCombine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgCombine)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMBINE_H__6619A9E1_AF0A_11D6_BB83_DFD6D1F64D7F__INCLUDED_)
