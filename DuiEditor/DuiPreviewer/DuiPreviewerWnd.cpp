#include "StdAfx.h"
#include "DuiPreviewerWnd.h"

#include <iostream>

HANDLE g_hOutput = NULL;

CDuiPreviewerWnd::CDuiPreviewerWnd(void)
{
}


CDuiPreviewerWnd::~CDuiPreviewerWnd(void)
{
}

void CDuiPreviewerWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);

	delete this;
	::PostQuitMessage(0L);
}

//windows原生消息
LRESULT CDuiPreviewerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		
		DuiWriteConsole(_T("WM_CREATE"));
		break;
	case WM_CLOSE:		
		DuiWriteConsole(_T("WM_CLOSE"));
		break;
	case WM_DESTROY:	
		DuiWriteConsole(_T("WM_DESTROY"));
		break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		
		break;
	case WM_NCCALCSIZE:		
		break;
	case WM_NCPAINT:		
		break;
	case WM_NCHITTEST:		
		break;
	case WM_GETMINMAXINFO:	
		break;
	case WM_MOUSEWHEEL:		
		break;
#endif
	case WM_SIZE:	
		{
			int x = 0;
		}
		break;
	case WM_CHAR:			
		break;
	case WM_SYSCOMMAND:		
		break;
	case WM_KEYDOWN:	
		break;
	case WM_KILLFOCUS:		
		break;
	case WM_SETFOCUS:		
		break;
	case WM_LBUTTONUP:		
		break;
	case WM_LBUTTONDOWN:	
		break;
	case WM_RBUTTONUP:		
		break;
	case WM_RBUTTONDOWN:	
		break;
	case WM_MOUSEMOVE:	
		break;
	case WM_MOUSEHOVER:		
		break;
	default:				
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

//CPaintManagerUI消息
LRESULT CDuiPreviewerWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}


void CDuiPreviewerWnd::Notify(TNotifyUI& msg)
{	
	CString className = msg.pSender->GetClass();
	CString ControlName = msg.pSender->GetName();
	CString EventName = msg.sType;

 	CString strText;
 	strText.Format(_T("Notify: class='%s' name='%s' msg='%s'"), className, ControlName, EventName);
	DuiWriteConsole(strText);

	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		CDuiString sCtrlName = msg.pSender->GetName();
		if( sCtrlName == _T("windowclosebtn") )
		{
			Close(IDCANCEL);
		}
		else if( sCtrlName == _T("windowminbtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if( sCtrlName == _T("windowmaxbtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		}
		else if( sCtrlName == _T("windowrestorebtn"))
		{ 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		}
	}
	
	__super::Notify(msg);
}


LRESULT CDuiPreviewerWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == SC_CLOSE)
	{
		bHandled = TRUE;
		SendMessage(WM_CLOSE);
		return 0;
	}
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowmaxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("windowrestorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	return lRes;
}