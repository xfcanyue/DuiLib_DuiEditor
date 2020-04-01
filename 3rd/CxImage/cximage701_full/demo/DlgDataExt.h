#if !defined(AFX_DLGDATAEXT_H__A61C9532_A14F_4116_8D1E_94CD0087BAA5__INCLUDED_)
#define AFX_DLGDATAEXT_H__A61C9532_A14F_4116_8D1E_94CD0087BAA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataExt.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgDataExt dialog

class DlgDataExt : public CDialog
{
// Construction
public:
	CxImage* m_image;
	COLORREF m_color;
	bool	m_bDetect;
	bool	m_bMinmax;
	bool	m_bAvg;
	bool	m_bLogXaxis;
	bool	m_bLogYaxis;

	DlgDataExt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgDataExt)
	enum { IDD = IDD_DATAEXT };
	CButton	m_chk_preview;
	CButton	m_chk_logyaxis;
	CButton	m_chk_logxaxis;
	CButton	m_btn_matchcolor;
	CButton	m_btn_graylevel;
	CxTargetButton	m_btn_colorsel;
	CButton	m_chk_minmax;
	CButton	m_chk_avg;
	CxTargetButton	m_btn_process;
	CStatic	m_picture;
	CxTargetButton	m_ok;
	float	m_Ymax;
	float	m_Ymin;
	float	m_Xmin;
	float	m_Xmax;
	BYTE	m_thresh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgDataExt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HBITMAP m_bitmap;
	void UpdatePreview();

	// Generated message map functions
	//{{AFX_MSG(DlgDataExt)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnProcess();
	afx_msg void OnColorsel();
	virtual void OnOK();
	afx_msg void OnChangeThresh();
	afx_msg void OnPreview();
	afx_msg void OnGreylevel();
	afx_msg void OnRadio3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATAEXT_H__A61C9532_A14F_4116_8D1E_94CD0087BAA5__INCLUDED_)
