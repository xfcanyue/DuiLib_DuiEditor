#include "StdAfx.h"
#include "UIEdit.h"

namespace DuiLib
{
	class CEditWnd : public CWindowWnd
	{
	public:
		CEditWnd();

		void Init(CEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		void InitNativeRender();
	protected:
		enum { 
			DEFAULT_TIMERID = 20,
		};

		CEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		bool m_bInit;
		bool m_bDrawCaret;
		CStdRefPtr<UIFont> m_font;
		CStdRefPtr<UIBitmap> m_bmpBrush;
		CStdRefPtr<UIBrush> m_brush;
	};


	CEditWnd::CEditWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false), m_bDrawCaret(false)
	{
		
	}

	void CEditWnd::Init(CEditUI* pOwner)
	{
		m_pOwner = pOwner;
		RECT rcPos = CalPos();
		UINT uStyle = 0;
		if(m_pOwner->GetManager()->IsLayered()) {
			uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
			RECT rcWnd={0};
			::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWnd);
			rcPos.left += rcWnd.left;
			rcPos.right += rcWnd.left;
			rcPos.top += rcWnd.top - 1;
			rcPos.bottom += rcWnd.top - 1;
		}
		else {
			uStyle = WS_CHILD | ES_AUTOHSCROLL;
		}
		UINT uTextStyle = m_pOwner->GetTextStyle();
		if(uTextStyle & DT_LEFT) uStyle |= ES_LEFT;
		else if(uTextStyle & DT_CENTER) uStyle |= ES_CENTER;
		else if(uTextStyle & DT_RIGHT) uStyle |= ES_RIGHT;
		if( m_pOwner->IsPasswordMode() ) uStyle |= ES_PASSWORD;
		if( m_pOwner->IsMultiLine()) uStyle |= ES_MULTILINE | ES_AUTOVSCROLL;
		if( m_pOwner->IsWantReturn()) uStyle |= ES_WANTRETURN;
		Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);

		m_font = MakeRefPtr<UIFont>(m_pOwner->GetManager()->CloneFont(m_pOwner->GetFont()));
		::SendMessage(m_hWnd, WM_SETFONT, (WPARAM)m_font->GetHFont(m_pOwner->GetManager()), (LPARAM)TRUE);
		Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
		if( m_pOwner->IsPasswordMode() ) Edit_SetPasswordChar(m_hWnd, m_pOwner->GetPasswordChar());
		Edit_SetText(m_hWnd, m_pOwner->GetText());
		Edit_SetModify(m_hWnd, FALSE);
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
		Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);
		Edit_SetReadOnly(m_hWnd, m_pOwner->IsReadOnly() == true);

		//Styls
		LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
		styleValue |= pOwner->GetWindowStyls();
		::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
		//Styls
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		::SetFocus(m_hWnd);
		if (m_pOwner->IsAutoSelAll()) {
			int nSize = GetWindowTextLength(m_hWnd);
			if( nSize == 0 ) nSize = 1;
			Edit_SetSel(m_hWnd, 0, nSize);
		}
		else {
			int nSize = GetWindowTextLength(m_hWnd);
			Edit_SetSel(m_hWnd, nSize, nSize);
		}

		m_bInit = true;   
	}

	RECT CEditWnd::CalPos()
	{
		CDuiRect rcPos = m_pOwner->GetPos();
		RECT rcInset = m_pOwner->GetTextPadding();
		rcPos.left += rcInset.left;
		rcPos.top += rcInset.top;
		rcPos.right -= rcInset.right;
		rcPos.bottom -= rcInset.bottom;
		if(m_pOwner && !m_pOwner->IsMultiLine())
		{
			LONG lEditHeight = m_pOwner->GetManager()->GetFontHeight(m_pOwner->GetFont());
			if( lEditHeight < rcPos.GetHeight() ) {
				rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
				rcPos.bottom = rcPos.top + lEditHeight;
			}
		}

		CControlUI* pParent = m_pOwner;
		RECT rcParent;
		while( pParent = pParent->GetParent() ) {
			if( !pParent->IsVisible() ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
			rcParent = pParent->GetClientPos();
			if( !::IntersectRect(&rcPos, &rcPos, &rcParent) ) {
				rcPos.left = rcPos.top = rcPos.right = rcPos.bottom = 0;
				break;
			}
		}

		return rcPos;
	}

	LPCTSTR CEditWnd::GetWindowClassName() const
	{
		return _T("EditWnd");
	}

	LPCTSTR CEditWnd::GetSuperClassName() const
	{
		return WC_EDIT;
	}

	void CEditWnd::OnFinalMessage(HWND hWnd)
	{
		m_pOwner->Invalidate();
		// Clear reference and die
		if( m_hBkBrush != NULL ) ::DeleteObject(m_hBkBrush);
		if (m_pOwner->GetManager()->IsLayered()) {
			m_pOwner->GetManager()->RemoveNativeWindow(hWnd);
		}
		m_pOwner->m_pWindow = NULL;
		delete this;
	}

	LRESULT CEditWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if( uMsg == WM_CREATE ) 
		{
			//m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//if( m_pOwner->GetManager()->IsLayered() ) {
			//	::SetTimer(m_hWnd, DEFAULT_TIMERID, ::GetCaretBlinkTime(), NULL);
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_KILLFOCUS ) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
		else if( uMsg == OCM_COMMAND ) {
			if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE ) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
			else if( GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE ) {
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
			}
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN )
		{
			if(!m_pOwner->IsWantReturn())
				m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);
		}
		else if( uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_TAB ){
			if (m_pOwner->GetManager()->IsLayered()) {
				m_pOwner->GetManager()->SetNextTabControl();
			}
		}
		else if( uMsg == OCM__BASE + WM_CTLCOLOREDIT  || uMsg == OCM__BASE + WM_CTLCOLORSTATIC ) {
			//if (m_pOwner->GetManager()->IsLayered() && !m_pOwner->GetManager()->IsPainting()) {
			//	m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//}
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			if( clrColor == 0xFFFFFFFF ) return 0;
			::SetBkMode((HDC)wParam, TRANSPARENT);
			DWORD dwTextColor = m_pOwner->GetTextColor();
			::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor),GetGValue(dwTextColor),GetRValue(dwTextColor)));
			return (LRESULT)m_brush->GetHBrush();
		}
		else if( uMsg == WM_SIZE)
		{
			//OutputDebugString(_T("InitNativeRender \r\n"));
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			if (clrColor < 0xFF000000) 
			{
				RECT rcWnd = m_pOwner->GetManager()->GetNativeWindowRect(m_hWnd);
				m_bmpBrush = MakeRefPtr<UIBitmap>(UIGlobal::CreateControlBackBitmap(m_pOwner, rcWnd, clrColor));
				m_brush = MakeRefPtr<UIBrush>(UIGlobal::CreateBrush());
				m_brush->CreateBitmapBrush(m_bmpBrush);
			}
			else 
			{
				m_brush = MakeRefPtr<UIBrush>(UIGlobal::CreateBrush());
				m_brush->CreateSolidBrush(clrColor);
			}
		}
		else if( uMsg == WM_PAINT) {
			//if (m_pOwner->GetManager()->IsLayered()) {
			//	m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_PRINT ) {
			//if (m_pOwner->GetManager()->IsLayered()) {
			//	lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
			//	if( m_pOwner->IsEnabled() && m_bDrawCaret ) {
			//		RECT rcClient;
			//		::GetClientRect(m_hWnd, &rcClient);
			//		POINT ptCaret;
			//		::GetCaretPos(&ptCaret);
			//		RECT rcCaret = { ptCaret.x, ptCaret.y, ptCaret.x, ptCaret.y+rcClient.bottom-rcClient.top };
			//		CRenderEngine::DrawLine((HDC)wParam, rcCaret, 1, 0xFF000000);
			//	}
			//	return lRes;
			//}
			bHandled = FALSE;
		}
		else if( uMsg == WM_TIMER ) {
			if (wParam == CARET_TIMERID) {
				m_bDrawCaret = !m_bDrawCaret;
				RECT rcClient;
				::GetClientRect(m_hWnd, &rcClient);
				::InvalidateRect(m_hWnd, &rcClient, FALSE);
				return 0;
			}
			bHandled = FALSE;
		}
		else bHandled = FALSE;

		if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		return lRes;
	}

	LRESULT CEditWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		//if ((HWND)wParam != m_pOwner->GetManager()->GetPaintWindow()) {
		//	::SendMessage(m_pOwner->GetManager()->GetPaintWindow(), WM_KILLFOCUS, wParam, lParam);
		//}
		PostMessage(WM_CLOSE);
		return lRes;
	}

	LRESULT CEditWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if( !m_bInit ) return 0;
		if( m_pOwner == NULL ) return 0;
		// Copy text back
		int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
		LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
		ASSERT(pstr);
		if( pstr == NULL ) return 0;
		::GetWindowText(m_hWnd, pstr, cchLen);
		m_pOwner->m_sText = pstr;
		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
		if( m_pOwner->GetManager()->IsLayered() ) m_pOwner->Invalidate();
		return 0;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//
	//
	IMPLEMENT_DUICONTROL(CEditUI)

	CEditUI::CEditUI() : m_pWindow(NULL), m_uMaxChar(255), 
		m_bPasswordMode(false), m_cPasswordChar(_T('*')), m_bAutoSelAll(false), 
		m_dwEditbkColor(0xFFFFFFFF), m_dwEditTextColor(0x00000000), m_iWindowStyls(0),m_dwTipValueColor(0xFFBAC0C5)
	{
		SetTextPadding(CDuiRect(4, 3, 4, 3));
		SetBkColor(0xFFFFFFFF);
		m_bMultiLine = false;
		m_bWantReturn = false;
	}

	LPCTSTR CEditUI::GetClass() const
	{
		return _T("EditUI");
	}

	LPVOID CEditUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcsicmp(pstrName, DUI_CTR_EDIT) == 0 ) return static_cast<CEditUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	UINT CEditUI::GetControlFlags() const
	{
		if( !IsEnabled() ) return CControlUI::GetControlFlags();

		return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
	}

	void CEditUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pParent != NULL ) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_SETCURSOR && IsEnabled() )
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			return;
		}
		if( event.Type == UIEVENT_WINDOWSIZE )
		{
			if( m_pWindow != NULL ) m_pManager->SetFocusNeeded(this);
		}
		if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			if( m_pWindow != NULL ) return;
		}
		if( event.Type == UIEVENT_SETFOCUS && IsEnabled() ) 
		{
			if( m_pWindow ) return;
			m_pWindow = new CEditWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
			Invalidate();
		}
		if( event.Type == UIEVENT_KILLFOCUS && IsEnabled() ) 
		{
			Invalidate();
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN) 
		{
			if( IsEnabled() ) {
				GetManager()->ReleaseCapture();
				if( IsFocused() && m_pWindow == NULL )
				{
					m_pWindow = new CEditWnd();
					ASSERT(m_pWindow);
					m_pWindow->Init(this);

					if( PtInRect(&m_rcItem, event.ptMouse) )
					{
						int nSize = GetWindowTextLength(*m_pWindow);
						if( nSize == 0 ) nSize = 1;
						Edit_SetSel(*m_pWindow, 0, nSize);
					}
				}
				else if( m_pWindow != NULL )
				{
					if (!m_bAutoSelAll) {
						POINT pt = event.ptMouse;
						RECT rcTextPadding = GetTextPadding();
						pt.x -= m_rcItem.left + rcTextPadding.left;
						pt.y -= m_rcItem.top + rcTextPadding.top;
						Edit_SetSel(*m_pWindow, 0, 0);
						::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
					}
				}
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE ) 
		{
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_CONTEXTMENU )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
                if( IsEnabled() ) {
                    if( !IsHotState() ) {
                        SetHotState(true);
                        Invalidate();
                    }
                }
            }
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( IsEnabled() ) {
				SetHotState(false);
				Invalidate();
			}
			return;

			//if( !::PtInRect(&m_rcItem, event.ptMouse ) ) {
   //             if( IsEnabled() ) {
   //                 if( (m_uButtonState & UISTATE_HOT) != 0  ) {
   //                     m_uButtonState &= ~UISTATE_HOT;
   //                     Invalidate();
   //                 }
   //             }
   //             if (m_pManager) m_pManager->RemoveMouseLeaveNeeded(this);
   //         }
   //         else {
   //             if (m_pManager) m_pManager->AddMouseLeaveNeeded(this);
   //             return;
   //         }
		}
		CLabelUI::DoEvent(event);
	}

	void CEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if( m_pWindow != NULL ) Edit_SetText(*m_pWindow, m_sText);
		Invalidate();
	}

	void CEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if( m_pWindow != NULL ) Edit_LimitText(*m_pWindow, m_uMaxChar);
	}

	UINT CEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CEditUI::SetReadOnly(bool bReadOnly)
	{
		if( IsReadOnly() == bReadOnly ) return;

		CControlUI::SetReadOnly(bReadOnly);
		if( m_pWindow != NULL ) Edit_SetReadOnly(*m_pWindow, bReadOnly);
		Invalidate();
	}

	void CEditUI::SetNumberOnly(bool bNumberOnly)
	{
		if( bNumberOnly )
		{
			m_iWindowStyls |= ES_NUMBER;
		}
		else
		{
			m_iWindowStyls &= ~ES_NUMBER;
		}
	}

	bool CEditUI::IsNumberOnly() const
	{
		return (m_iWindowStyls & ES_NUMBER) ? true:false;
	}

	void CEditUI::SetLowerCase(bool bLowerCase)
	{
		if( bLowerCase )
		{
			m_iWindowStyls |= ES_LOWERCASE;
		}
		else
		{
			m_iWindowStyls &= ~ES_LOWERCASE;
		}
	}
	bool CEditUI::IsLowerCase() const { return (m_iWindowStyls & ES_LOWERCASE) ? true:false; }

	void CEditUI::SetUpperCase(bool bUpperCase)
	{
		if( bUpperCase )
		{
			m_iWindowStyls |= ES_UPPERCASE;
		}
		else
		{
			m_iWindowStyls &= ~ES_UPPERCASE;
		}
	}
	bool CEditUI::IsUpperCase() const { return (m_iWindowStyls & ES_UPPERCASE) ? true:false; }

	void CEditUI::SetMultiLine(bool bMultiLine) { m_bMultiLine = bMultiLine; }
	bool CEditUI::IsMultiLine() const { return m_bMultiLine; }

	void CEditUI::SetWantReturn(bool bWantReturn) { m_bWantReturn = bWantReturn; }
	bool CEditUI::IsWantReturn() { return m_bWantReturn; }

	int CEditUI::GetWindowStyls() const 
	{
		return m_iWindowStyls;
	}

	void CEditUI::SetPasswordMode(bool bPasswordMode)
	{
		if( m_bPasswordMode == bPasswordMode ) return;
		m_bPasswordMode = bPasswordMode;
		Invalidate();
		if( m_pWindow != NULL ) {
			LONG styleValue = ::GetWindowLong(*m_pWindow, GWL_STYLE);
			bPasswordMode ? styleValue |= ES_PASSWORD : styleValue &= ~ES_PASSWORD;
			::SetWindowLong(*m_pWindow, GWL_STYLE, styleValue);
		}
	}

	bool CEditUI::IsPasswordMode() const
	{
		return m_bPasswordMode;
	}

	void CEditUI::SetPasswordChar(TCHAR cPasswordChar)
	{
		if( m_cPasswordChar == cPasswordChar ) return;
		m_cPasswordChar = cPasswordChar;
		if( m_pWindow != NULL ) Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
		Invalidate();
	}

	TCHAR CEditUI::GetPasswordChar() const
	{
		return m_cPasswordChar;
	}

	void CEditUI::SetNativeEditBkColor(DWORD dwBkColor)
	{
		m_dwEditbkColor = dwBkColor;
	}

	DWORD CEditUI::GetNativeEditBkColor() const
	{
		return m_dwEditbkColor;
	}

	void CEditUI::SetNativeEditTextColor( LPCTSTR pStrColor )
	{
		if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwEditTextColor = clrColor;
	}

	DWORD CEditUI::GetNativeEditTextColor() const
	{
		return m_dwEditTextColor;
	}
	
	bool CEditUI::IsAutoSelAll()
	{
		return m_bAutoSelAll;
	}

	void CEditUI::SetAutoSelAll(bool bAutoSelAll)
	{
		m_bAutoSelAll = bAutoSelAll;
	}

	void CEditUI::SetSel(long nStartChar, long nEndChar)
	{
		if( m_pWindow != NULL ) Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
	}

	void CEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if( m_pWindow != NULL ) Edit_ReplaceSel(*m_pWindow, lpszReplace);
	}

	void CEditUI::SetTipValue( LPCTSTR pStrTipValue )
	{
		m_sTipValue	= pStrTipValue;
	}

	CDuiString CEditUI::GetTipValue()
	{
		if (IsResourceText()) 
			return CResourceManager::GetInstance()->GetText(m_sTipValue);

		CLangPackageUI *pkg = GetLangPackage();
		if(pkg && GetResourceID() > 0)
		{
			LPCTSTR s = pkg->GetTipValue(GetResourceID());
			if(s && *s!='\0') return s; 
		}
		return m_sTipValue;
	}
	                    
	void CEditUI::SetTipValueColor( LPCTSTR pStrColor )
	{
		if( *pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

		m_dwTipValueColor = clrColor;
	}

	DWORD CEditUI::GetTipValueColor()
	{
		return m_dwTipValueColor;
	}
	
	
	void CEditUI::SetPos(RECT rc, bool bNeedInvalidate)
	{
		CControlUI::SetPos(rc, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = m_pWindow->CalPos();
			::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left, 
				rcPos.bottom - rcPos.top, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::SetVisible(bool bVisible)
	{
		CControlUI::SetVisible(bVisible);
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	void CEditUI::SetInternVisible(bool bVisible)
	{
		if( !IsVisible() && m_pWindow != NULL ) m_pManager->SetFocus(NULL);
	}

	SIZE CEditUI::EstimateSize(SIZE szAvailable)
	{
		if(IsAutoCalcHeight() && !IsMultiLine()) 
			return CDuiSize(m_cxyFixed.cx, m_pManager->GetFontHeight(GetFont()) + 6);
		return CControlUI::EstimateSize(szAvailable);
	}

	void CEditUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("numberonly")) == 0 ) SetNumberOnly(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcscmp(pstrName, _T("autoselall")) == 0 ) SetAutoSelAll(_tcscmp(pstrValue, _T("true")) == 0);	
		else if( _tcsicmp(pstrName, _T("password")) == 0 ) SetPasswordMode(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("passwordchar")) == 0 ) SetPasswordChar(*pstrValue);
		else if( _tcsicmp(pstrName, _T("maxchar")) == 0 ) SetMaxChar(_ttoi(pstrValue));
		else if( _tcsicmp(pstrName, _T("lowercase")) == 0 ) SetLowerCase(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("uppercase")) == 0 ) SetUpperCase(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("multiline")) == 0 ) SetMultiLine(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("wantreturn")) == 0 ) SetWantReturn(_tcsicmp(pstrValue, _T("true")) == 0);
		else if( _tcsicmp(pstrName, _T("tipvalue")) == 0 ) SetTipValue(pstrValue);
		else if( _tcsicmp(pstrName, _T("tipvaluecolor")) == 0 ) SetTipValueColor(pstrValue);
		else if( _tcsicmp(pstrName, _T("nativetextcolor")) == 0 ) SetNativeEditTextColor(pstrValue);
		else if( _tcsicmp(pstrName, _T("nativebkcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			SetNativeEditBkColor(clrColor);
		}
		else CLabelUI::SetAttribute(pstrName, pstrValue);
	}


	void CEditUI::PaintText(UIRender *pRender)
	{
		CDuiString sText = GetText();
		RECT rcText = m_rcItem;
		DWORD dwColor = 0;
		int iFont = -1;

		if(sText.IsEmpty())
		{
			sText = GetTipValue();
			dwColor = GetTipValueColor();
		}
		else 
		{
			if(IsPasswordMode())
			{
				int length = sText.GetLength();
				sText.Empty();
				for(int i=0; i<length; i++) 
					sText += GetPasswordChar();
			}

			if( !IsEnabled() )
				dwColor = GetDisabledTextColor();

			else if( IsHotState() )
				dwColor = GetHotTextColor();

			else if( IsPushedState() )
				dwColor = GetPushedTextColor();

			else if( IsFocused() )
				dwColor = GetFocusedTextColor();

			if(dwColor == 0)
				dwColor = GetTextColor();

			if(dwColor == 0 && m_pManager)
				dwColor = m_pManager->GetDefaultFontColor();
		}

		//////////////////////////////////////////////////////////////////////////
		if( !IsEnabled() )
			iFont = GetDisabledFont();

		else if( IsPushedState() )
			iFont = GetPushedFont();

		else if( IsHotState() )
			iFont = GetHotFont();

		else if( IsFocused() )
			iFont = GetFocusedFont();

		if(iFont == -1)
			iFont = GetFont();

		UINT uTextStyle = GetTextStyle();
		if(IsMultiLine())
			uTextStyle = DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL;
		else
			uTextStyle |= DT_SINGLELINE;

		pRender->DrawText(rcText, GetTextPadding(), sText, dwColor, iFont, uTextStyle);
	}


	bool CEditUI::OnEnableResponseDefaultKeyEvent(WPARAM wParam)
	{
		//按回车键时，如果焦点在当前控件，不响应默认按键事件。
		if(wParam == VK_RETURN && IsWantReturn() && IsFocused() && m_pWindow != NULL )
			return false;
		return true;
	}
}
