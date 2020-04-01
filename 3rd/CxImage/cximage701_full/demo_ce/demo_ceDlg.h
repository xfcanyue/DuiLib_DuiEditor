// demo_ceDlg.h : header file
//

#pragma once
#include "cxpicture.h"

// Cdemo_ceDlg dialog
class Cdemo_ceDlg : public CDialog
{
// Construction
public:
	Cdemo_ceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMO_CE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	afx_msg void OnFileBtn();

	CxPicture m_staView;

	DECLARE_MESSAGE_MAP()
};
