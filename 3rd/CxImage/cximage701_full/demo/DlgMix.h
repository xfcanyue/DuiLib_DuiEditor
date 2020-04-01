#if !defined(AFX_DLGMIX_H__51F87317_7FCC_4396_8D88_1CFB0ACEEF90__INCLUDED_)
#define AFX_DLGMIX_H__51F87317_7FCC_4396_8D88_1CFB0ACEEF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMix.h : header file
//

#include "xTargetButton.h"

class CDemoDoc;
/////////////////////////////////////////////////////////////////////////////
// DlgMix dialog
class DlgMix : public CDialog
{
// Construction
public:
	CDemoDoc* pDocSrc;
	CDemoDoc* pDocDst;
	int OpType;

	DlgMix(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgMix)
	enum { IDD = IDD_MIX };
	CStatic	m_picture;
	CxTargetButton	m_preview;
	CxTargetButton	m_ok;
	CxTargetButton	m_canc;
	CComboBox	m_cbSrc;
	CComboBox	m_cbOpType;
	CComboBox	m_cbDst;
	int		m_xoffset;
	int		m_yoffset;
	BOOL	m_mixalpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgMix)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	HBITMAP m_bitmap;
	CxImage m_imageSrc,m_imageDst;
	void SetThumbs(CxImage* pSrc, CxImage* pDst);
	float m_ratio;
	void SetMix();

	// Generated message map functions
	//{{AFX_MSG(DlgMix)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeDst();
	afx_msg void OnSelchangeSrc();
	afx_msg void OnSelchangeOp();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMIX_H__51F87317_7FCC_4396_8D88_1CFB0ACEEF90__INCLUDED_)
