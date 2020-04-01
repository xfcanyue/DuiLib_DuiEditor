// logomdi.h : header file
/////////////////////////////////////////////////////////////////////////////
// CLogoMdi window
#if !defined(_CLOGOMDI_H)
#define _CLOGOMDI_H

#include "ximage.h"

class CLogoMdi : public CWnd
{
// Construction
public:
    CLogoMdi();
// Implementation
public:
    virtual ~CLogoMdi();
    // Generated message map functions
protected:
    CSize m_sizeClient ;
	CFont m_LogoFont;
	CString sLogoString;
	CxImage* bgImage;
    //{{AFX_MSG(CLogoMdi)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
#endif