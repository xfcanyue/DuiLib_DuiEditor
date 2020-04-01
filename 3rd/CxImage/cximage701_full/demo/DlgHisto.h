#if !defined(AFX_DLGHISTO_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_)
#define AFX_DLGHISTO_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHisto.h : header file
//

#include "xTargetButton.h"

/////////////////////////////////////////////////////////////////////////////
// DlgHisto dialog
class DlgHisto : public CDialogBar
{
// Construction
public:
	DlgHisto();
	virtual ~DlgHisto();

	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID );
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID );

	void InitDlg();

	//{{AFX_VIRTUAL(DlgHisto)
	protected:
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	CxTargetButton	m_r;
	CxTargetButton	m_g;
	CxTargetButton	m_b;
	CxTargetButton	m_gray;
	int	m_ren;
	int	m_gen;
	int	m_ben;
	int	m_grayen;
	int	m_logen;
	bool m_initok;
	// Generated message map functions
	//{{AFX_MSG(DlgHisto)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTO_H__4EA4FEE4_AE24_11D6_BB83_E5D7B698D37E__INCLUDED_)
