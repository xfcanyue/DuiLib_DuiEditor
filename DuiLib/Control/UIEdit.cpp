#include "StdAfx.h"
#include "UIEdit.h"

#include "UIEditWndWin32.h"
#include "UIEditWndGtk.h"

namespace DuiLib
{
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
			//::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
			GetManager()->SetCursor(DUI_IBEAM);
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
			((CEditWnd *)m_pWindow)->Init(this);
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
					((CEditWnd *)m_pWindow)->Init(this);

					//if( PtInRect(&m_rcItem, event.ptMouse) )
					if( m_rcItem.PtInRect(event.ptMouse) )
					{
						//int nSize = GetWindowTextLength(*m_pWindow);
						//if( nSize == 0 ) nSize = 1;
						//Edit_SetSel(*m_pWindow, 0, nSize);
						((CEditWnd *)m_pWindow)->edit_SetSel(0,-1);
					}
				}
				else if( m_pWindow != NULL )
				{
					if (!m_bAutoSelAll) {
						POINT pt = event.ptMouse;
						RECT rcTextPadding = GetTextPadding();
						pt.x -= m_rcItem.left + rcTextPadding.left;
						pt.y -= m_rcItem.top + rcTextPadding.top;
						//Edit_SetSel(*m_pWindow, 0, 0);
						//::SendMessage(m_pWindow->GetHWND(), WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
						((CEditWnd *)m_pWindow)->edit_SetSel(0,0);
						m_pWindow->SendMessage(WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
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
			//if( ::PtInRect(&m_rcItem, event.ptMouse ) ) {
			if( m_rcItem.PtInRect(event.ptMouse ) ) {
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

	CDuiString CEditUI::GetText() const
	{
		return m_sText;
	}

	void CEditUI::SetText(LPCTSTR pstrText)
	{
		m_sText = pstrText;
		if (m_pWindow != NULL) //Edit_SetText(*m_pWindow, m_sText);
			((CEditWnd*)m_pWindow)->edit_SetText(m_sText);
		Invalidate();
	}

	void CEditUI::SetMaxChar(UINT uMax)
	{
		m_uMaxChar = uMax;
		if (m_pWindow != NULL) //Edit_LimitText(*m_pWindow, m_uMaxChar);
			((CEditWnd*)m_pWindow)->edit_LimitText(m_uMaxChar);
	}

	UINT CEditUI::GetMaxChar()
	{
		return m_uMaxChar;
	}

	void CEditUI::SetReadOnly(bool bReadOnly)
	{
		if( IsReadOnly() == bReadOnly ) return;

		CControlUI::SetReadOnly(bReadOnly);
		if (m_pWindow != NULL) //Edit_SetReadOnly(*m_pWindow, bReadOnly);
			((CEditWnd*)m_pWindow)->edit_SetReadOnly(bReadOnly);
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
		if( m_pWindow != NULL ) 
		{
// 			LONG styleValue = ::GetWindowLong(*m_pWindow, GWL_STYLE);
// 			bPasswordMode ? styleValue |= ES_PASSWORD : styleValue &= ~ES_PASSWORD;
// 			::SetWindowLong(*m_pWindow, GWL_STYLE, styleValue);
			((CEditWnd*)m_pWindow)->edit_SetPasswordMode(bPasswordMode);
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
		if (m_pWindow != NULL)
		{
			//Edit_SetPasswordChar(*m_pWindow, m_cPasswordChar);
			((CEditWnd*)m_pWindow)->edit_SetPasswordChar(cPasswordChar);
		}
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
		if (m_pWindow != NULL) //Edit_SetSel(*m_pWindow, nStartChar,nEndChar);
			((CEditWnd*)m_pWindow)->edit_SetSel(nStartChar, nEndChar);
	}

	void CEditUI::SetSelAll()
	{
		SetSel(0,-1);
	}

	void CEditUI::SetReplaceSel(LPCTSTR lpszReplace)
	{
		if (m_pWindow != NULL) //Edit_ReplaceSel(*m_pWindow, lpszReplace);
			((CEditWnd*)m_pWindow)->edit_ReplaceSel(lpszReplace);
	}

	void CEditUI::SetTipValue( LPCTSTR pStrTipValue )
	{
		m_sTipValue	= pStrTipValue;
	}

	CDuiString CEditUI::GetTipValue()
	{
		if (IsResourceText()) 
		{
			CDuiString s = CResourceManager::GetInstance()->GetText(m_sTipValue);
			if(!s.IsEmpty()) return s;
		}

		CLangPackageUI *pkg = GetLangPackage();
		if(pkg && GetResourceID() > 0)
		{
			LPCTSTR s = pkg->GetTipValue(GetResourceID());
			if(s && *s!='\0') return s; 
		}
		else if (IsResourceText())
		{
			CDuiString s = CLangManagerUI::LoadString(m_sTipValue);
			if(!s.IsEmpty()) return s;
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
			RECT rcPos = ((CEditWnd *)m_pWindow)->CalPos();
			m_pWindow->SetWindowPos(NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
				rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);        
		}
	}

	void CEditUI::Move(SIZE szOffset, bool bNeedInvalidate)
	{
		CControlUI::Move(szOffset, bNeedInvalidate);
		if( m_pWindow != NULL ) {
			RECT rcPos = ((CEditWnd *)m_pWindow)->CalPos();
			m_pWindow->SetWindowPos(NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
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

