// ExDlgEngine.cpp: implementation of the CExDlgEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExDlgEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExDlgEngine::CExDlgEngine()
:	m_nHeaderHeight	(22)
{
	LOGFONT		lf;

	lf.lfHeight			= -11;
	lf.lfWidth			= 0;
	lf.lfEscapement		= 0;
	lf.lfOrientation	= 0;
	lf.lfWeight			= FW_BOLD;
	lf.lfItalic			= FALSE;
	lf.lfUnderline		= FALSE;
	lf.lfStrikeOut		= 0;
	lf.lfCharSet		= ANSI_CHARSET;
	lf.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	lf.lfQuality		= DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
	lstrcpy(lf.lfFaceName, TEXT("Tahoma"));

	m_fontBold.CreateFontIndirect(&lf);
}


CExDlgEngine::~CExDlgEngine()
{

}


// CExDlgEngine::PaintHeader
//
//		Paints a PocketPC 2002 dialog header
//
void CExDlgEngine::PaintHeader(CWnd* pWnd, CDC &dc, LPCTSTR pszTitle)
{
	CRect		recWnd;
	int			nWidth;// = dc.GetDeviceCaps(HORZRES);

	pWnd->GetClientRect(&recWnd);
	nWidth = recWnd.right;

	//
	// Paint title
	//
	CFont *pSave = dc.SelectObject(&m_fontBold);
	dc.SetTextColor(RGB(0, 0, 156));
	dc.DrawText(pszTitle, -1, CRect(8, 0, nWidth, m_nHeaderHeight), DT_VCENTER | DT_SINGLELINE);
	dc.SelectObject(pSave);

	//
	// Paint line
	//
	CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
	CPen *pOldPen = dc.SelectObject(&blackPen);

	dc.MoveTo(0, m_nHeaderHeight);
	dc.LineTo(nWidth, m_nHeaderHeight);

	dc.SelectObject(pOldPen);	
}


// CExDlgEngine::SetBold
//
//		Sets a control's font to bold
//
BOOL CExDlgEngine::SetBold(CWnd *pWnd, UINT nID)
{
	CWnd*	pWndCtrl;

	pWndCtrl = pWnd->GetDlgItem(nID);
	if(pWndCtrl)
	{
		pWndCtrl->SetFont(&m_fontBold);
	}
	
	return pWndCtrl != NULL;
}


// CExDlgEngine::Underline
//
//		Underlines the list of controls in the zero-terminated array
//
void CExDlgEngine::Underline(CWnd* pWnd, CDC &dc, UINT *pArrID)
{
	CPen		penLine(PS_SOLID, 1, RGB(192,192,192)),
		*		pPenOld;
	CWnd*		pCtl;
	CRect		rcDlg,
				rcCtl;
	UINT*		pID;

	//
	// Initialize the variables
	//
	pWnd->GetClientRect(&rcDlg);

	pPenOld = dc.SelectObject(&penLine);

	//
	// Loop through all edit boxes and draw a bottom line
	//
	for(pID = pArrID; *pID; pID++)
	{
		pCtl = pWnd->GetDlgItem(*pID);
		if(pCtl)
		{
			pCtl->GetWindowRect(&rcCtl);
			pWnd->ScreenToClient(&rcCtl);

			dc.MoveTo(rcCtl.left - 2, rcCtl.bottom);
			dc.LineTo(rcDlg.right - 2, rcCtl.bottom);
		}
	}

	dc.SelectObject(pPenOld);
}


// CExDlgEngine::CreateDateTime
//
//		Creates a date time control using an existing control as reference
//
BOOL CExDlgEngine::CreateDateTime(CWnd* pWnd, CDateTimeCtrl &dtc, UINT nID, DWORD dwStyle)
{
	CWnd*	pCtl;
	CFont*	pFont;
	CRect	rc;

	pCtl = pWnd->GetDlgItem(nID);
	if(pCtl)
	{
		pFont = pCtl->GetFont();
		pCtl->GetWindowRect(rc);
		pCtl->DestroyWindow();		//pWnd was just a placeholder;
		pWnd->ScreenToClient(rc);
		dtc.Create(WS_CHILD|WS_VISIBLE/*|WS_BORDER*/|dwStyle, 
			rc, pWnd, nID);
		dtc.SetFont(pFont);
	}

	return (pCtl != NULL);
}
