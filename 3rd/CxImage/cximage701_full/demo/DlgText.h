#if !defined(AFX_DLGTEXT_H__1FEA8102_CF4A_11D6_BB84_8B1C37831B77__INCLUDED_)
#define AFX_DLGTEXT_H__1FEA8102_CF4A_11D6_BB84_8B1C37831B77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgText.h : header file
//

#include "xTargetButton.h"

// It seems to be the simplest #ifdef-ing all dialog stuff:
#ifndef VATI_EXTENSIONS

/////////////////////////////////////////////////////////////////////////////
// DlgText dialog

class DlgText : public CDialog
{
// Construction
public:
	DlgText(CWnd* pParent = NULL);   // standard constructor
	LOGFONT m_font;
	COLORREF m_color;
// Dialog Data
	//{{AFX_DATA(DlgText)
	enum { IDD = IDD_TEXT };
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CxTargetButton	m_bfont;
	CString	m_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgText)
	virtual BOOL OnInitDialog();
	afx_msg void OnFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#else
// if VATI_EXTENSIONS:

/////////////////////////////////////////////////////////////////////////////
// edit box control, painted with non-default colors
class CMyEdit : public CEdit

   {
   public:
      BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM Param,
          LRESULT* pLResult);
   protected:
      static CBrush m_brush;
   };


/////////////////////////////////////////////////////////////////////////////
// DlgText dialog

class DlgText : public CDialog
{
// Construction
public:
	DlgText(CWnd* pParent = NULL);   // standard constructor

    CFont     cfont;                 // font for edit control
    CBrush* m_pEditBkBrush;          // to store the new background brush for edit controls.

	CxImage::CXTEXTINFO  m_textdata;          // local copy of common text data 
    DWORD    m_ccflags;				 // color dialog behaviour
    DWORD    m_cfflags;				 // font dialog behaviour
    
	COLORREF m_custColors[16];       // for the color dialog
    
// Dialog Data
	//{{AFX_DATA(DlgText)
	enum { IDD = IDD_TEXT };
	CStatic	m_radiusTxt;
	CStatic	m_opacTxt;
	CSliderCtrl	m_sldr_radius;
	CSliderCtrl	m_sldr_opac;
	CButton	m_check1;
	CEdit	m_text;
	CxTargetButton	m_forecolor;
	CxTargetButton	m_backcolor;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CxTargetButton	m_bfont;
    BOOL            m_opaque;
    BOOL            m_textsmooth;
	CString	m_lbl_opac;
	CString	m_lbl_radius;
   	CButton	m_radio1;
	CButton	m_radio2;
	CButton	m_radio3;
	CButton	m_check2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void DlgText::ChangeCtrlStyle( CSliderCtrl * m_Slider, long lStyle, BOOL bSetBit);
	// Generated message map functions
	//{{AFX_MSG(DlgText)
	virtual BOOL OnInitDialog();
	afx_msg void OnFont();
	afx_msg void OnBtnBckcolor();
	afx_msg void OnBtnTxtcolor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnCheck1();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnCheck2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXT_H__1FEA8102_CF4A_11D6_BB84_8B1C37831B77__INCLUDED_)
