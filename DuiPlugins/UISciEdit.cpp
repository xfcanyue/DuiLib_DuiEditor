#include "StdAfx.h"
#include "UISciEdit.h"


namespace DuiLib
{
	class CSciWndUI : public CWindowWnd
	{
	public:
		CSciWndUI(void)
		{
			
		}
		~CSciWndUI(void) 
		{
			if(m_pOwner)
			{
				if(m_pOwner->m_hSciLexer) ::FreeLibrary(m_pOwner->m_hSciLexer);
				m_pOwner->m_hSciLexer = NULL;
			}
		}

		void Init(CSciEditUI* pOwner)
		{
			m_pOwner = pOwner;
			if(m_pOwner->m_hSciLexer == NULL)
			{
				m_pOwner->m_hSciLexer = ::LoadLibrary(g_strDuiPluginsPath + _T("SciLexer.dll"));
			}

			Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD|WS_VISIBLE, 0, m_rcWindow);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_BORDER));  //去边框		
		}

		void SetPos(RECT rc, bool bNeedInvalidate  = true)
		{
			m_rcWindow = rc;
			::MoveWindow(GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
		}

		LPCTSTR GetWindowClassName() const
		{
			return _T("SciWndUI");
		}

		void OnFinalMessage(HWND hWnd)
		{
			
		}

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lRes = 0;
			BOOL bHandled = FALSE;

			if( uMsg == WM_CREATE ) 
			{
				m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);

				m_hwndScintilla = CreateWindowEx(0, _T("Scintilla"),_T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, 0,0,0,0,
					GetHWND(),(HMENU)NULL, m_pOwner->GetManager()->GetInstance(),NULL);
				ASSERT(m_hwndScintilla);
				m_pOwner->InitSciApi(m_hwndScintilla);
			}
			else if( uMsg == WM_SIZE) 
			{
				CRect rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::MoveWindow(m_hwndScintilla, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), TRUE);
			}
			else if(uMsg == WM_NOTIFY)
			{
				bHandled = TRUE;
				NMHDR *phDR = (NMHDR*)lParam;
				if (phDR != NULL && phDR->hwndFrom == m_hwndScintilla)
				{
					SCNotification *pMsg = (SCNotification*)lParam;
					m_pOwner->OnSciNotify(pMsg);
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_SCI_NOTIFY, (WPARAM)m_hwndScintilla, lParam);
				}
			}

			if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			return lRes;
		}

	public:
		RECT m_rcWindow;
		CSciEditUI *m_pOwner;
		HWND m_hwndScintilla;
	};

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CSciEditUI)
CSciEditUI::CSciEditUI(void) : m_pWindow(0)
{
}


CSciEditUI::~CSciEditUI(void)
{
	if(m_pWindow) { delete m_pWindow; }
}

LPCTSTR CSciEditUI::GetClass() const
{
	return DUI_CTR_SCIEDIT;
}

LPVOID CSciEditUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_SCIEDIT) == 0 ) return static_cast<CSciEditUI*>(this);
	return __super::GetInterface(pstrName);
}

void CSciEditUI::DoInit()
{
	m_pWindow = new CSciWndUI;
	((CSciWndUI *)m_pWindow)->Init(this);
	if(IsValidSciApi())
	{
		sci_SetCodePage(SC_CP_UTF8);	//默认使用UTF8编码
	}
}

void CSciEditUI::SetPos(RECT rc, bool bNeedInvalidate /* = true */)
{
	__super::SetPos(rc, bNeedInvalidate);

	RECT rcItem = rc;

	// Adjust for inset
	RECT rcInset = m_rcInset;
	GetManager()->GetDPIObj()->Scale(&m_rcInset);
	rcItem.left += m_rcInset.left;
	rcItem.top += m_rcInset.top;
	rcItem.right -= m_rcInset.right;
	rcItem.bottom -= m_rcInset.bottom;

	if(m_pWindow)
		((CSciWndUI *)m_pWindow)->SetPos(rcItem, bNeedInvalidate);
}

void CSciEditUI::SetInternVisible(bool bVisible)
{
	if(m_pWindow)
		::ShowWindow(m_pWindow->GetHWND(), bVisible);
}

void CSciEditUI::OnSciNotify(SCNotification *pMsg)
{
	switch (pMsg->nmhdr.code)
	{
	case SCN_STYLENEEDED:
		break;
	case SCN_CHARADDED:
		break;
	case SCN_SAVEPOINTREACHED:
		break;
	case SCN_SAVEPOINTLEFT: //文件被修改
		break;
	case SCN_MODIFYATTEMPTRO:
		break;
	case SCN_KEY:
		break;
	case SCN_DOUBLECLICK:
		break;
	case SCN_UPDATEUI:
		break;
	case SCN_MODIFIED:
		break;
	case SCN_MACRORECORD:
		break;
	case SCN_MARGINCLICK:
		break;
	case SCN_NEEDSHOWN:
		break;
	case SCN_PAINTED:
		break;
	case SCN_USERLISTSELECTION:
		break;
	case SCN_URIDROPPED:
		break;
	case SCN_DWELLSTART:
		break;
	case SCN_DWELLEND:
		break;
	case SCN_ZOOM:
		break;
	case SCN_HOTSPOTCLICK:
		break;
	case SCN_HOTSPOTDOUBLECLICK:
		break;
	case SCN_HOTSPOTRELEASECLICK:
		break;
	case SCN_INDICATORCLICK:
		break;
	case SCN_INDICATORRELEASE:
		break;
	case SCN_CALLTIPCLICK:
		break;
	case SCN_AUTOCSELECTION:
		break;
	case SCN_AUTOCCANCELLED:
		break;
	case SCN_AUTOCCHARDELETED:
		break;
	case SCI_SETMODEVENTMASK:
		break;
	case SCI_GETMODEVENTMASK:
		break;
	case SCI_SETMOUSEDWELLTIME:
		break;
	case SCI_GETMOUSEDWELLTIME:
		break;
	case SCI_SETIDENTIFIER:
		break;
	case SCI_GETIDENTIFIER:
		break;
	case SCEN_CHANGE:
		break;
	case SCEN_SETFOCUS:
		break;
	case SCEN_KILLFOCUS:
		break;
	}
}

CDuiString CSciEditUI::GetText() const
{
	if(!IsValidSciApi()) return _T("");

	CSciEditUI *pThis = const_cast<CSciEditUI *>(this);

	int code = pThis->sci_GetCodePage();

	LSSTRING_CONVERSION;
	CStringA strA;
	pThis->sci_GetTextAll(strA);

	if(code == SC_CP_UTF8)
		return LSUTF82T(strA);
	return LSA2T(strA);
}

void CSciEditUI::SetText(LPCTSTR pstrText)
{
	if(!IsValidSciApi()) return;

	int code = sci_GetCodePage();

	LSSTRING_CONVERSION;

	if(code == SC_CP_UTF8)
		sci_SetText(LST2UTF8(pstrText));
	else
		sci_SetText(LST2A(pstrText));
}

}