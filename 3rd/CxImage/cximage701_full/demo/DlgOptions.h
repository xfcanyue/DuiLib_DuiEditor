#if !defined(AFX_DLGOPTIONS_H__218AAA01_FAC3_11D5_8DD7_00E07D8144D0__INCLUDED_)
#define AFX_DLGOPTIONS_H__218AAA01_FAC3_11D5_8DD7_00E07D8144D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptions.h : header file
//

#include "xTargetButton.h"

#if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
class CxImageJPG;
#endif
/////////////////////////////////////////////////////////////////////////////
// DlgOptions dialog
class DlgOptions : public CDialog
{
// Construction
public:
	DWORD m_Opt_tif;
	DWORD m_Opt_gif;
	DWORD m_Opt_jpg;
	DWORD m_Opt_png;
	DWORD m_Opt_raw;
#if CXIMAGE_SUPPORT_EXIF && CXIMAGE_SUPPORT_JPG
	CxImageJPG* m_exif;
#endif 
	DlgOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgOptions)
	enum { IDD = IDD_OPTIONS };
	CButton	m_rEnc25;
	CButton	m_rEnc24;
	CButton	m_rEnc23;
	CButton	m_rEnc22;
	CButton	m_chkJpgOpt;
	CxTargetButton	m_info;
	CButton	m_rEnc21;
	CButton	m_rEnc20;
	CButton	m_rEnc19;
	CButton	m_rEnc18;
	CButton	m_rEnc17;
	CButton	m_rEnc16;
	CButton	m_rEnc15;
	CButton	m_rEnc14;
	CButton	m_rEnc13;
	CButton	m_rEnc12;
	CButton	m_rEnc11;
	CButton	m_rEnc10;
	CButton	m_rEnc9;
	CButton	m_rEnc8;
	CButton	m_rEnc7;
	CButton	m_rEnc5;
	CButton	m_rEnc4;
	CButton	m_rEnc3;
	CButton	m_rEnc2;
	CButton	m_rEnc1;
	CButton	m_rEnc0;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	float	m_jpeg_quality;
	long	m_xres;
	DWORD	m_yres;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgOptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnExif();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONS_H__218AAA01_FAC3_11D5_8DD7_00E07D8144D0__INCLUDED_)
