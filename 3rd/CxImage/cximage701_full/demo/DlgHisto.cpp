// DlgHisto.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "MainFrm.h"
#include "DlgHisto.h"
#include "demodoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgHisto dialog
DlgHisto::DlgHisto()
{ m_initok = false;}
/////////////////////////////////////////////////////////////////////////////
DlgHisto::~DlgHisto()
{ }
/////////////////////////////////////////////////////////////////////////////
BOOL DlgHisto::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
{
	BOOL bSuccess = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
	InitDlg();
	return bSuccess;
}
/////////////////////////////////////////////////////////////////////////////
BOOL DlgHisto::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	BOOL bSuccess = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	InitDlg();
	return bSuccess;
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(DlgHisto, CDialogBar)
	//{{AFX_MSG_MAP(DlgHisto)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgHisto message handlers
void DlgHisto::InitDlg()
{
	if (m_initok) return;

	RECT r1 = {10,8,59,35};
	m_r.Create(_T(""),WS_VISIBLE|WS_TABSTOP,r1,this,0);
	m_r.SetButtonStyle(BS_PUSHLIKE|BS_FLAT|BS_AUTOCHECKBOX,0);
	m_r.SetWindowText(_T("R"));
	m_r.EnableTarget(0);
	m_r.SetIcon(IDI_R,BS_LEFT);
	m_r.SetCheck(1);
	m_r.EnableWindow(1);

	RECT r2 = {60,8,109,35};
	m_g.Create(_T(""),WS_VISIBLE|WS_TABSTOP,r2,this,0);
	m_g.SetButtonStyle(BS_PUSHLIKE|BS_FLAT|BS_AUTOCHECKBOX,0);
	m_g.SetWindowText(_T("G"));
	m_g.EnableTarget(0);
	m_g.SetIcon(IDI_G,BS_LEFT);
	m_g.SetCheck(1);
	m_g.EnableWindow(1);

	RECT r3 = {110,8,159,35};
	m_b.Create(_T(""),WS_VISIBLE|WS_TABSTOP,r3,this,0);
	m_b.SetButtonStyle(BS_PUSHLIKE|BS_FLAT|BS_AUTOCHECKBOX,0);
	m_b.SetWindowText(_T("B"));
	m_b.EnableTarget(0);
	m_b.SetIcon(IDI_B,BS_LEFT);
	m_b.SetCheck(1);
	m_b.EnableWindow(1);

	RECT r4 = {160,8,209,35};
	m_gray.Create(_T(""),WS_VISIBLE|WS_TABSTOP,r4,this,0);
	m_gray.SetButtonStyle(BS_PUSHLIKE|BS_FLAT|BS_AUTOCHECKBOX,0);
	m_gray.SetWindowText(_T("gray"));
	m_gray.EnableTarget(0);
	m_gray.SetIcon(IDI_GRAY,BS_LEFT);
	m_gray.SetCheck(1);
	m_gray.EnableWindow(1);

	m_ren=m_gen=m_ben=m_grayen=1;

	CButton* plog = (CButton*)GetDlgItem(IDC_LOG);
	if (plog) { plog->SetCheck(1); m_logen=1; } else  m_logen=0;

	m_initok=true;
}

BOOL DlgHisto::OnCommand(WPARAM wparam, LPARAM lparam) 
{
	CDialogBar::OnCommand(wparam, lparam);

	m_ren=m_r.GetCheck();
	m_gen=m_g.GetCheck();
	m_ben=m_b.GetCheck();
	m_grayen=m_gray.GetCheck();

	CButton* plog = (CButton*)GetDlgItem(IDC_LOG);
	m_logen=plog->GetCheck();

	Invalidate();

	return TRUE;
}

void DlgHisto::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDemoDoc* pDoc = (CDemoDoc*) ((CMainFrame*)AfxGetMainWnd())->GetActiveFrame()->GetActiveDocument();

	RECT r;
	GetClientRect(&r);

	CPen gridPen(PS_DOT,1,::GetSysColor(COLOR_APPWORKSPACE));
	CPen* pOldPen = dc.SelectObject(&gridPen);
	for (int gx=9+32;gx<265;gx+=32){
		dc.MoveTo(gx,40);
		dc.LineTo(gx,r.bottom - 6);
	}
	for (int gy=0;gy<(r.bottom - 40);gy+=((r.bottom - 40)/10)){
		dc.MoveTo(8,r.bottom - 9 - gy);
		dc.LineTo(268,r.bottom - 9 - gy);
	}
	dc.SelectObject(pOldPen);

	CPen axisPen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	pOldPen = dc.SelectObject(&axisPen);
	dc.MoveTo(9, 38);
	dc.LineTo(9, r.bottom - 9);
	dc.LineTo(266, r.bottom - 9);
	dc.LineTo(266, 38);
	dc.SelectObject(pOldPen);

	if (pDoc && pDoc->image) {

		if (pDoc->m_hmax){
			int ybase = r.bottom-10;
			if (m_logen){
				float yratio = (r.bottom - r.top - 50)/(float)log10((float)(pDoc->m_hmax+1));
				if (m_ren){
					CPen redPen(PS_SOLID, 1, RGB(222, 0, 0));
					pOldPen = dc.SelectObject(&redPen);
					dc.MoveTo(10, (int)(ybase-log10((float)(1+pDoc->m_hr[0]))*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-log10((float)(1+pDoc->m_hr[x]))*yratio));
				}
				if (m_gen){
					CPen greenPen(PS_SOLID, 1, RGB(0, 222, 0));
					pOldPen = dc.SelectObject(&greenPen);
					dc.MoveTo(10, (int)(ybase-log10((float)(1+pDoc->m_hg[0]))*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-log10((float)(1+pDoc->m_hg[x]))*yratio));
				}
				if (m_ben){
					CPen bluePen(PS_SOLID, 1, RGB(0, 0, 222));
					pOldPen = dc.SelectObject(&bluePen);
					dc.MoveTo(10, (int)(ybase-log10((float)(1+pDoc->m_hb[0]))*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-log10((float)(1+pDoc->m_hb[x]))*yratio));
				}
				if (m_grayen){
					CPen grayPen(PS_SOLID, 1, RGB(64, 64, 64));
					pOldPen = dc.SelectObject(&grayPen);
					dc.MoveTo(10, (int)(ybase-log10((float)(1+pDoc->m_hgray[0]))*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-log10((float)(1+pDoc->m_hgray[x]))*yratio));
				}
				dc.SelectObject(pOldPen);
			} else {
				float yratio = (r.bottom - r.top - 50)/(float)pDoc->m_hmax;
				if (m_ren){
					CPen redPen(PS_SOLID, 1, RGB(222, 0, 0));
					pOldPen = dc.SelectObject(&redPen);
					dc.MoveTo(10, (int)(ybase-pDoc->m_hr[0]*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-pDoc->m_hr[x]*yratio));
				}
				if (m_gen){
					CPen greenPen(PS_SOLID, 1, RGB(0, 222, 0));
					pOldPen = dc.SelectObject(&greenPen);
					dc.MoveTo(10, (int)(ybase-pDoc->m_hg[0]*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-pDoc->m_hg[x]*yratio));
				}
				if (m_ben){
					CPen bluePen(PS_SOLID, 1, RGB(0, 0, 222));
					pOldPen = dc.SelectObject(&bluePen);
					dc.MoveTo(10, (int)(ybase-pDoc->m_hb[0]*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-pDoc->m_hb[x]*yratio));
				}
				if (m_grayen){
					CPen grayPen(PS_SOLID, 1, RGB(64, 64, 64));
					pOldPen = dc.SelectObject(&grayPen);
					dc.MoveTo(10, (int)(ybase-pDoc->m_hgray[0]*yratio));
					for (int x=1; x<256; x++)
						dc.LineTo(x+10, (int)(ybase-pDoc->m_hgray[x]*yratio));
				}
				dc.SelectObject(pOldPen);
			}
		}
		else
		{
			pDoc->m_hmax=pDoc->GetImage()->Histogram(pDoc->m_hr,pDoc->m_hg,pDoc->m_hb,pDoc->m_hgray);
			if (pDoc->m_hmax) Invalidate();
		}
	}
}
