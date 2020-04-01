// DlgURL.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgURL.h"

#include <afxinet.h>
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgURL dialog


DlgURL::DlgURL(CWnd* pParent /*=NULL*/)
	: CDialog(DlgURL::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgURL)
	m_url = _T("http://www.xdp.it/tmp/test.jp2");
	//}}AFX_DATA_INIT

	m_hInternetSession = NULL;
	m_hHttpConnection = NULL;
	m_hHttpFile = NULL;

	m_bAbort = FALSE;
	m_bSafeToClose = TRUE;
}


void DlgURL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgURL)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_URL, m_url);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgURL, CDialog)
	//{{AFX_MSG_MAP(DlgURL)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgURL message handlers
void DlgURL::OnOK() 
{
    UpdateData();
    if (m_url.IsEmpty()) return;
	m_ok.EnableWindow(0);
	m_bSafeToClose = FALSE;

	SetStatus(_T("Connecting to site..."));
	CString* m_strBuffer = &m_data;

	CString       m_sServer=_T(""); 
	CString       m_sObject; 
	INTERNET_PORT m_nPort = INTERNET_DEFAULT_HTTP_PORT;
	DWORD         m_dwServiceType = INTERNET_SERVICE_HTTP;
	AfxParseURL(m_url, m_dwServiceType, m_sServer, m_sObject, m_nPort);

	//Create the Internet session handle
	m_hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (m_hInternetSession == NULL){ ThreadError(_T("cannot open internet session")); return; }

	if (m_bAbort) { OnThreadFinished(1); return; }  

	//Make the connection to the HTTP server          
	m_hHttpConnection = ::InternetConnect(m_hInternetSession, m_sServer, m_nPort , NULL, 
                                          NULL, m_dwServiceType , 0, (DWORD) this);

	if (m_hHttpConnection == NULL){ ThreadError(_T("cannot connect to remote server")); return; }

	if (m_bAbort) { OnThreadFinished(1); return; }  

	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;

	m_hHttpFile = HttpOpenRequest(m_hHttpConnection, NULL, m_sObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
                                INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, (DWORD) this);

	if (m_hHttpFile == NULL){ ThreadError(_T("Failed in call to HttpOpenRequest")); return; }

	if (m_bAbort) { OnThreadFinished(1); return; }  

	//Issue the request
	BOOL bSend = ::HttpSendRequest(m_hHttpFile, NULL, 0, NULL, 0);

	// Get the length of the file.            
	TCHAR szContentLength[32];
	DWORD dwInfoSize = 32;
	DWORD dwFileSize = 0;
	if (::HttpQueryInfo(m_hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL)){
		dwFileSize = (DWORD) _ttol(szContentLength);
	}

	//Now do the actual read of the file
	DWORD dwBytesRead = 0;
	char szReadBuf[1025];
	DWORD dwBytesToRead = 1024;
	DWORD dwTotalBytesRead = 0;
	do {
		if (::InternetReadFile(m_hHttpFile, szReadBuf, dwBytesToRead, &dwBytesRead)){
			if (dwBytesRead && !m_bAbort) {
				//Write the data to file
				szReadBuf[dwBytesRead]=0;
				LPTSTR ptr = m_strBuffer->GetBufferSetLength(dwTotalBytesRead + dwBytesRead + 1);
				memcpy(ptr+dwTotalBytesRead, szReadBuf, dwBytesRead);

				//Increment the total number of bytes read
				dwTotalBytesRead += dwBytesRead;
				m_strBuffer->ReleaseBuffer(dwTotalBytesRead+1);

				CString s;
				s.Format(_T("%d/%d"),dwTotalBytesRead,dwFileSize);
				SetStatus(s);
			}
		} else { ThreadError(_T("An error occurred while downloading the file")); return; }
	} while (dwBytesRead && !m_bAbort);

	m_size = dwTotalBytesRead;

	//We're finished
	OnThreadFinished(0);
}

void DlgURL::OnDestroy() 
{
	//Free up the internet handles we may be using
	if (m_hHttpFile){
		::InternetCloseHandle(m_hHttpFile);
		m_hHttpFile = NULL;
	}
	if (m_hHttpConnection){
		::InternetCloseHandle(m_hHttpConnection);
		m_hHttpConnection = NULL;
	}
	if (m_hInternetSession){
		::InternetCloseHandle(m_hInternetSession);
		m_hInternetSession = NULL;
	}
	CDialog::OnDestroy();
}

void DlgURL::OnClose() 
{
	if (m_bSafeToClose) CDialog::OnClose();
	else {
		//Just set the abort flag to TRUE and disable the cancel button
		m_bAbort = TRUE;	
		m_canc.EnableWindow(0);
		SetStatus(_T("Aborting transfer..."));
	}
}

LRESULT DlgURL::OnThreadFinished(WPARAM wParam)
{
	//It's now safe to close since the thread has signaled us
	m_bSafeToClose = TRUE;
	//If an error occured display the message box
	if (m_bAbort) EndDialog(IDCANCEL);
	else if (wParam) {
		AfxMessageBox(m_sError);
		EndDialog(IDCANCEL);
	} else EndDialog(IDOK);
	return 0L;
}

void DlgURL::SetStatus(LPCTSTR message)
{
	CWnd* statwnd = GetDlgItem(IDC_STATUS);
	statwnd->SetWindowText(message);
}

void DlgURL::ThreadError(LPCTSTR message)
{
  m_sError=message;
  OnThreadFinished(1);
}

BOOL DlgURL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
