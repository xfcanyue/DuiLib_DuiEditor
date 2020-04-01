// demo_ceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo_ce.h"
#include "demo_ceDlg.h"
#include "FileOpenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Cdemo_ceDlg dialog

Cdemo_ceDlg::Cdemo_ceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cdemo_ceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cdemo_ceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICVIEW, m_staView);
}

BEGIN_MESSAGE_MAP(Cdemo_ceDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_BN_CLICKED(IDC_FILE_BTN, OnFileBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Cdemo_ceDlg message handlers

BOOL Cdemo_ceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void Cdemo_ceDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	DRA::RelayoutDialog(
		AfxGetInstanceHandle(), 
		this->m_hWnd, 
		DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_DEMO_CE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_DEMO_CE_DIALOG));
}
#endif




void Cdemo_ceDlg::OnFileBtn() 
{
	//CString strFilter = _T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*) |*.*||");
	//CFileDialog	dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_EXPLORER, strFilter, NULL); 
	
	CFileOpenDlg dlg;
	
	
	if (dlg.DoModal() == IDOK) {
        
		CString	strFullPath(dlg.GetPath());
		strFullPath += dlg.GetFileName();

		m_staView.SetBitmap( strFullPath );
		
        Invalidate();
    }
	
}