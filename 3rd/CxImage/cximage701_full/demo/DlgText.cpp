// DlgText.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgText dialog

#ifndef VATI_EXTENSIONS
// original dialog AFX stuff:
DlgText::DlgText(CWnd* pParent /*=NULL*/)
	: CDialog(DlgText::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgText)
	m_text = _T("");
	//}}AFX_DATA_INIT
	memset(&m_font,0,sizeof(m_font));
}

void DlgText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgText)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, ID_FONT, m_bfont);
	DDX_Text(pDX, IDC_EDIT1, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgText, CDialog)
	//{{AFX_MSG_MAP(DlgText)
	ON_BN_CLICKED(ID_FONT, OnFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// VATI_EXTENSIONS dialog AFX stuff:
#else
DlgText::DlgText(CWnd* pParent /*=NULL*/)
	: CDialog(DlgText::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgText)
	m_opaque = FALSE;
	m_textsmooth = FALSE;
	m_lbl_opac = _T("");
	m_lbl_radius = _T("");
	//}}AFX_DATA_INIT

    // Instantiate and initialize the background brush to black.
    m_pEditBkBrush = new CBrush(RGB(0,0,0));

    // font and color dialog behaviour
	m_cfflags = 0 | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_ENABLEHOOK;
    m_ccflags = 0x00000183 ; // 0 | CC_ANYCOLOR | CC_FULLOPEN | CC_SOLIDCOLOR | CC_RGBINIT ; 

	memset(&m_custColors,0,sizeof(COLORREF)*16);

}

void DlgText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgText)
	DDX_Control(pDX, IDC_STATIC_RADIUS, m_radiusTxt);
	DDX_Control(pDX, IDC_STATIC_OP, m_opacTxt);
	DDX_Control(pDX, IDC_SLIDER_RADIUS, m_sldr_radius);
	DDX_Control(pDX, IDC_SLIDER_OPACITY, m_sldr_opac);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_EDIT1, m_text);
	DDX_Control(pDX, IDC_BTN_TXTCOLOR, m_forecolor);
	DDX_Control(pDX, IDC_BTN_BCKCOLOR, m_backcolor);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Control(pDX, ID_FONT, m_bfont);
	DDX_Check(pDX, IDC_CHECK1, m_opaque);
	DDX_Text(pDX, IDC_STATIC_OP, m_lbl_opac);
	DDX_Text(pDX, IDC_STATIC_RADIUS, m_lbl_radius);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);
	DDX_Control(pDX, IDC_TEXT_SMOOTH, m_check2);
	DDX_Check(pDX, IDC_TEXT_SMOOTH, m_textsmooth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgText, CDialog)
	//{{AFX_MSG_MAP(DlgText)
	ON_BN_CLICKED(ID_FONT, OnFont)
	ON_BN_CLICKED(IDC_BTN_BCKCOLOR, OnBtnBckcolor)
	ON_BN_CLICKED(IDC_BTN_TXTCOLOR, OnBtnTxtcolor)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_TEXT_SMOOTH, OnCheck2)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// endif VATI_EXTENSIONS:
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgText message handlers

BOOL DlgText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
#ifndef VATI_EXTENSIONS
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_bfont.SetIcon(IDI_B,BS_LEFT);
#else
	m_ok.SetIcon(IDI_OK,BS_LEFT);
	m_canc.SetIcon(IDI_CANC,BS_LEFT);
	m_bfont.SetIcon(IDI_G,BS_LEFT);
	m_forecolor.SetIcon(IDI_B,BS_LEFT);
	m_backcolor.SetIcon(IDI_B,BS_LEFT);

    //set checkbox
    m_check1.SetCheck ( m_textdata.opaque );
    m_check2.SetCheck ( m_textdata.smooth );

    // set editbox text/font/color settings to the current
    cfont.CreateFontIndirect( &(m_textdata.lfont) );
    m_text.SetFont( &cfont, TRUE);
    m_text.SetWindowText( m_textdata.text );
    m_text.SetSel(0,9999);

    //radiobuttons and sliders
    if ( m_textdata.align==DT_LEFT )
        m_radio2.SetCheck(1);
    else if ( m_textdata.align==DT_RIGHT )
        m_radio3.SetCheck(1);
    else
        m_radio1.SetCheck(1);

	// opacity slider
    ChangeCtrlStyle( &m_sldr_opac, TBS_BOTH|TBS_AUTOTICKS, TRUE ); 
	m_sldr_opac.SetTicFreq( 5 );    // Send TBM_SETTICFREQ
	m_sldr_opac.SetLineSize( 10 );  // Send TBM_SETLINESIZE
	m_sldr_opac.SetPageSize( 20 );  // Send TBM_SETPAGESIZE
    m_sldr_opac.SetPos( (int)(100.*m_textdata.b_opacity) );
	m_sldr_opac.SetRange( 0,100, TRUE );
    m_lbl_opac.Format( _T("%.0f%%"), 100.*m_textdata.b_opacity );
    m_opacTxt.SetWindowText ( m_lbl_opac.GetBuffer(0) );
    m_opacTxt.UpdateWindow ();

    // rounding radius slider
    ChangeCtrlStyle( &m_sldr_radius, TBS_BOTH|TBS_AUTOTICKS, TRUE ); 
    m_sldr_radius.SetTicFreq(5);
    m_sldr_radius.SetLineSize(5);
    m_sldr_radius.SetPageSize(10);
    m_sldr_radius.SetPos( m_textdata.b_round );
    m_sldr_radius.SetRange (0,50,TRUE);
    m_lbl_radius.Format( _T("%d%%"), m_textdata.b_round );
    m_radiusTxt.SetWindowText ( m_lbl_radius.GetBuffer(0) );
    m_radiusTxt.UpdateWindow ();

    UpdateData(TRUE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgText::OnFont() 
{
#ifndef VATI_EXTENSIONS
	CFontDialog	dlg;
	memcpy(dlg.m_cf.lpLogFont, &m_font, sizeof(m_font));
	dlg.m_cf.rgbColors = m_color;
	if (dlg.DoModal()==IDOK){
		memcpy(&m_font,dlg.m_cf.lpLogFont, sizeof(m_font));
		m_color = dlg.GetColor();
	}
#else
	CFontDialog	dlg;
    LOGFONT tmpLogfont;
	memcpy( &tmpLogfont, &(m_textdata.lfont), sizeof(LOGFONT));
    dlg.m_cf.lpLogFont = &tmpLogfont ;
	dlg.m_cf.rgbColors = m_textdata.fcolor;
    dlg.m_cf.Flags = m_cfflags ;

	if (dlg.DoModal()==IDOK)
    {
		memcpy( &(m_textdata.lfont), &tmpLogfont, sizeof(LOGFONT));
        // set editbox text/font/color settings to the current
        cfont.Detach();
        cfont.CreateFontIndirect( &tmpLogfont );
        m_text.SetFont( &cfont, TRUE);
	}
	UpdateData(TRUE);
#endif
	return;
}

#ifdef VATI_EXTENSIONS
void DlgText::OnBtnBckcolor() 
{
	// TODO: Add your control notification handler code here
    CColorDialog	dlg(  m_textdata.bcolor, m_ccflags, NULL );

    dlg.m_cc.lpCustColors = m_custColors ;
    dlg.m_cc.rgbResult    = m_textdata.bcolor ;

    // show the dialog and get the user's response...
	if (dlg.DoModal() == IDOK)
    {
		m_textdata.bcolor = dlg.m_cc.rgbResult;
        m_text.SetFont( &cfont, TRUE);
    }
    UpdateData(TRUE);

}

void DlgText::OnBtnTxtcolor() 
{
	// TODO: Add your control notification handler code here
    CColorDialog	dlg(  m_textdata.fcolor, m_ccflags, NULL );

    dlg.m_cc.lpCustColors = m_custColors ;
    dlg.m_cc.rgbResult    = m_textdata.fcolor ;

    // show the dialog and get the user's response...
	if (dlg.DoModal() == IDOK)
    {
		m_textdata.fcolor = dlg.m_cc.rgbResult;
        m_text.SetFont( &cfont, TRUE);
    }
    UpdateData(TRUE);
}

HBRUSH DlgText::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    switch (nCtlColor)
    {
	//if painting the edit box in this dialof, change the brush colors
    case CTLCOLOR_EDIT:
        // Set color to green on black and return the background  brush.
        pDC->SetTextColor( m_textdata.fcolor );
        pDC->SetBkColor( m_textdata.bcolor );
        return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());

    default:
        return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void DlgText::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your control notification handler code here
    // Free the space allocated for the background brush
    delete m_pEditBkBrush;
}

void DlgText::OnOK() 
{
	// TODO: Add extra validation here
    CString tmpS;
	
    // copy back control's values
    m_text.GetWindowText( tmpS );
	_tcscpy ( m_textdata.text, tmpS.GetBuffer(0) );
    m_textdata.opaque = (BYTE)m_check1.GetCheck();
    m_textdata.smooth = (BYTE)m_check2.GetCheck();

	CDialog::OnOK();
}

void DlgText::OnCheck1() 
{
	// TODO: Add your control notification handler code here
    m_textdata.opaque = (BYTE)( m_check1.GetCheck() );
    m_backcolor.EnableWindow( m_textdata.opaque == TRUE );
    m_sldr_opac.EnableWindow ( m_textdata.opaque == TRUE );
    m_sldr_radius.EnableWindow ( m_textdata.opaque == TRUE );
	m_opacTxt.EnableWindow( m_textdata.opaque == TRUE );
    m_radiusTxt.EnableWindow( m_textdata.opaque == TRUE );
}
void DlgText::OnCheck2() 
{
	// TODO: Add your control notification handler code here
    m_textdata.smooth = (BYTE)( m_check2.GetCheck() );
}

void DlgText::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_textdata.align = DT_CENTER;
}

void DlgText::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_textdata.align = DT_LEFT;
}

void DlgText::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_textdata.align = DT_RIGHT;
}

// handle the moving event of the sliders
void DlgText::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    if ( nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK )
    {
        // if the mouse is on the opacity slider
        if ( GetCapture() == GetDlgItem( IDC_SLIDER_OPACITY ) )
        {
            m_textdata.b_opacity = (float)( 0.01 * m_sldr_opac.GetPos() );
            m_lbl_opac.Format( _T("%.0f%%"), 100*m_textdata.b_opacity );
            m_opacTxt.SetWindowText ( m_lbl_opac.GetBuffer(0));
            m_opacTxt.UpdateWindow ();
        }
        // if the mouse is on the outline slider
        if ( GetCapture() == GetDlgItem( IDC_SLIDER_RADIUS ) )
        {
            m_textdata.b_round = (unsigned char)( m_sldr_radius.GetPos() );
            m_lbl_radius.Format( _T("%d%%"), m_sldr_radius.GetPos() );
            m_radiusTxt.SetWindowText ( m_lbl_radius.GetBuffer(0));
            m_radiusTxt.UpdateWindow ();
        }
    }
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// change the slider's appearance
void DlgText::ChangeCtrlStyle( CSliderCtrl * pSlider, long lStyle, BOOL bSetBit)
{
	long    lStyleOld;
	CRect   rect;

	lStyleOld = GetWindowLong( pSlider->GetSafeHwnd(), GWL_STYLE );
	if ( bSetBit )
		lStyleOld |= lStyle;
	else
		lStyleOld &= ~lStyle;
	SetWindowLong( pSlider->GetSafeHwnd(), GWL_STYLE, lStyleOld );
	pSlider->GetWindowRect(&rect);
	pSlider->InvalidateRect(&rect);
}
#endif