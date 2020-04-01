// ExDialog.cpp : implementation file
//
#include "stdafx.h"
#include "Resourceppc.h"
#include "ExDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------------
//
//	CExDialog dialog
//
//---------------------------------------------------------------------------


CExDialog::CExDialog()
{
}


CExDialog::CExDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CExDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExDialog, CDialog)
	//{{AFX_MSG_MAP(CExDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CExDialog::EnableControl
//
//		Enable or disable a control given its ID
//
BOOL CExDialog::EnableControl(UINT nID, BOOL bEnable)
{
	CWnd*	pWnd;

	pWnd = GetDlgItem(nID);
	if(pWnd)
		pWnd->EnableWindow(bEnable);

	return pWnd != NULL;
}


//---------------------------------------------------------------------------
//
//	CExDialog message handlers
//
//---------------------------------------------------------------------------


// CExDialog::OnPaint
//
//		Paints the dialog
//
void CExDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//
	// Paints the header if available
	//
	if(!m_strTitle.IsEmpty())
		PaintHeader(dc);

	DoPaint(dc);
}


// CExDialog::DoPaint
//
//		Workhorse painting method
//
void CExDialog::DoPaint(CPaintDC &dc)
{
}
