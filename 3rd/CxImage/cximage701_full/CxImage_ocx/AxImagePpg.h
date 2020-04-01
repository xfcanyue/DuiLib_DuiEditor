#if !defined(AFX_AXIMAGEPPG_H__4FED1AD4_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_)
#define AFX_AXIMAGEPPG_H__4FED1AD4_6CF3_11D6_BB83_9FDD9223647D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AxImagePpg.h : Declaration of the AxImagePropPage property page class.

////////////////////////////////////////////////////////////////////////////
// AxImagePropPage : See AxImagePpg.cpp.cpp for implementation.

class AxImagePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(AxImagePropPage)
	DECLARE_OLECREATE_EX(AxImagePropPage)

// Constructor
public:
	AxImagePropPage();

// Dialog Data
	//{{AFX_DATA(AxImagePropPage)
	enum { IDD = IDD_PROPPAGE_AXIMAGE };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(AxImagePropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXIMAGEPPG_H__4FED1AD4_6CF3_11D6_BB83_9FDD9223647D__INCLUDED)
