#include "StdAfx.h"
#include "UISciEdit.h"


namespace DuiLib
{
	class CSciWndUI : public CWindowWnd
	{
	public:
		CSciWndUI(void) : m_hSciLexer(NULL), m_pSciWnd(NULL)
		{
			m_hSciLexer = ::LoadLibrary(g_strDuiPluginsPath + _T("SciLexer.dll"));
		}
		~CSciWndUI(void) 
		{
			if(m_hSciLexer) ::FreeLibrary(m_hSciLexer);
		}

		void Init(CSciEditUI* pOwner)
		{
			m_pOwner = pOwner;
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
			//return _T("Scintilla");
			return _T("CSciWndUI");
		}

		void OnFinalMessage(HWND hWnd)
		{
			if(m_pSciWnd) { delete m_pSciWnd; m_pSciWnd = NULL; }
		}

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lRes = 0;
			BOOL bHandled = FALSE;

			if( uMsg == WM_CREATE ) 
			{
				m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);

				CWnd *pWnd = CWnd::FromHandle(m_hWnd);
				m_pSciWnd = new CSciWnd();
				m_pSciWnd->Create(0, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, m_rcWindow, pWnd, 2);
				ASSERT(m_pSciWnd->m_hWnd);
			}
			else if( uMsg == WM_SIZE) 
			{
				CRect rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				m_pSciWnd->MoveWindow(rcClient);
			}
			else if(uMsg == WM_NOTIFY)
			{
				bHandled = TRUE;
				NMHDR *phDR = (NMHDR*)lParam;
				if (phDR != NULL && phDR->hwndFrom == m_pSciWnd->m_hWnd)
				{
					SCNotification *pMsg = (SCNotification*)lParam;
					m_pSciWnd->OnParentNotify(pMsg);
					
					m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_SCI_NOTIFY, (WPARAM)m_pSciWnd->m_hWnd, lParam);
				}
			}
// 			else if(uMsg == WM_SCIWND_LBUTTONDOWN)
// 			{
// 				::PostMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_LBUTTONDOWN, (WPARAM)m_pSciWnd->m_hWnd, lParam);
// 			}
// 			else if(uMsg == WM_SCIWND_LBUTTONUP)
// 			{
// 				::PostMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_LBUTTONUP, (WPARAM)m_pSciWnd->m_hWnd, lParam);
// 			}

			if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			return lRes;
		}

	public:
		RECT m_rcWindow;
		CSciEditUI *m_pOwner;
		HMODULE m_hSciLexer;
		CSciWnd *m_pSciWnd;
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

CSciWnd *CSciEditUI::GetSciWnd() const
{
	if(!m_pWindow) return NULL;
	return ((CSciWndUI *)m_pWindow)->m_pSciWnd;
}

void CSciEditUI::DoInit()
{
	m_pWindow = new CSciWndUI;
	((CSciWndUI *)m_pWindow)->Init(this);
	if(GetSciWnd())
	{
		GetSciWnd()->sci_SetCodePage(SC_CP_UTF8);	//默认使用UTF8编码
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

CDuiString CSciEditUI::GetText() const
{
	CSciWnd *pSciWnd = GetSciWnd();
	if(pSciWnd == NULL) return _T("");

	int code = pSciWnd->sci_GetCodePage();

	LSSTRING_CONVERSION;
	CStringA strA;
	pSciWnd->sci_GetTextAll(strA);

	if(code == SC_CP_UTF8)
		return LSUTF82T(strA);
	return LSA2T(strA);
}

void CSciEditUI::SetText(LPCTSTR pstrText)
{
	CSciWnd *pSciWnd = GetSciWnd();
	if(pSciWnd == NULL) return;

	int code = pSciWnd->sci_GetCodePage();

	LSSTRING_CONVERSION;

	if(code == SC_CP_UTF8)
		pSciWnd->sci_SetText(LST2UTF8(pstrText));
	else
		pSciWnd->sci_SetText(LST2A(pstrText));
}

}