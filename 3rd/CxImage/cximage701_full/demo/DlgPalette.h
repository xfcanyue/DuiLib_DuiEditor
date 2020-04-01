#if !defined(AFX_DLGPALETTE_H__76ED5361_AECA_11D6_BB83_E2CBB4CAE57F__INCLUDED_)
#define AFX_DLGPALETTE_H__76ED5361_AECA_11D6_BB83_E2CBB4CAE57F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPalette.h : header file
/* Based on CColourPicker and CColourPopup, by Chris Maunder */

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgPalette dialog
class DlgPalette : public CDialog
{
// Construction
public:
	long	m_selected;
	long	m_replace;
	long	m_changed;
	long	m_boxgap;
	long	m_boxheight;
	long	m_boxwidth;
	long	m_yoffset;
	long	m_xoffset;
	long	m_numcolumns;
	long	m_numrows;
	long	m_numcolors;
	RGBQUAD m_pal[256];
	DlgPalette(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgPalette)
	enum { IDD = IDD_PALETTE };
	CStatic	m_bar;
	CButton	m_r3;
	CButton	m_r2;
	CButton	m_r1;
	CxTargetButton	m_load;
	CxTargetButton	m_save;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	BOOL	m_editalpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgPalette)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool GetCellRect(long index, const LPRECT& rect);
	void DrawCell(CDC* pDC, long index);
	long GetColumn(long index) const;
	long GetRow(long index) const;
    long GetIndex(long row, long col) const;
	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle,DWORD dwFlags, BOOL bOpenFileDialog, int* pType);

	// Generated message map functions
	//{{AFX_MSG(DlgPalette)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnEditAlpha();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg void OnLoad();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPALETTE_H__76ED5361_AECA_11D6_BB83_E2CBB4CAE57F__INCLUDED_)
