#include "StdAfx.h"
#include "UIKeyBoard.h"

namespace DuiLib
{


CKeyButtonUI::CKeyButtonUI()
{
	m_ch = 0;
	m_chShift = 0;
	m_pKeyBoard = NULL;
	m_bControl = false;
}

LPCTSTR CKeyButtonUI::GetClass() const
{
	return _T("KeyButton");
}

LPVOID CKeyButtonUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("KeyButton")) == 0 ) return static_cast<CKeyButtonUI*>(this);
	return __super::GetInterface(pstrName);
}

void CKeyButtonUI::SetOwner(CKeyBoardUI *pKeyBoard) 
{ 
	m_pKeyBoard = pKeyBoard; 
}

CKeyBoardUI *CKeyButtonUI::GetOwner() const
{
	return m_pKeyBoard;
}

void CKeyButtonUI::SetChar(TCHAR ch, TCHAR chShift, BYTE vkCode)
{
	m_ch = ch;
	m_chShift = chShift;
	m_vkCode = vkCode;
}

TCHAR CKeyButtonUI::GetChar()
{
	if(m_ch >= 'a' && m_ch <='z' && m_pKeyBoard->IsCapsON())
	{
		return m_chShift;
	}

	if(m_pKeyBoard->IsShiftON())
		return m_chShift;

	return m_ch;
}

BYTE CKeyButtonUI::GetVkCode()
{
	return m_vkCode;
}

bool CKeyButtonUI::IsControlKey() const
{
	if(m_vkCode == VK_BACK || m_vkCode == VK_TAB || m_vkCode == VK_CAPITAL || m_vkCode == VK_RETURN ||
		m_vkCode == VK_SHIFT || m_vkCode == VK_CONTROL || m_vkCode == VK_MENU || m_vkCode == VK_ESCAPE ||
		m_vkCode == VK_DELETE || m_vkCode == VK_UP || m_vkCode == VK_DOWN || m_vkCode == VK_LEFT || 
		m_vkCode == VK_RIGHT)
	{
		return true;
	}
	return false;
}

bool CKeyButtonUI::IsOemKey() const
{
	return (m_vkCode>=VK_OEM_1 && m_vkCode<=VK_OEM_102) || (m_vkCode>=0x30 && m_vkCode<=0x39);
}

bool CKeyButtonUI::Activate()
{
	if(!__super::Activate()) return false;
	
	if(m_pKeyBoard)
	{
		m_pKeyBoard->OnKeyPress(this);
	}

	return true;
}

CDuiString CKeyButtonUI::GetText() const
{
	CDuiString sText = __super::GetText();
	if(!sText.IsEmpty()) return sText;

	if(IsControlKey())
		return GetOwner()->GetKeyText(m_vkCode);

	if(GetOwner()->IsCapsON() || GetOwner()->IsShiftON())
		return m_chShift;

	return m_ch;
}

bool CKeyButtonUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	if(m_vkCode == VK_CAPITAL)
	{
		if(GetOwner()->IsCapsON())
			m_uButtonState |= UISTATE_SELECTED;
		else
			m_uButtonState &= ~UISTATE_SELECTED;
	}

	if(m_vkCode == VK_SHIFT)
	{
		if(GetOwner()->IsShiftON())
			m_uButtonState |= UISTATE_SELECTED;
		else
			m_uButtonState &= ~UISTATE_SELECTED;
	}

	return __super::DoPaint(hDC, rcPaint, pStopControl);
}

void CKeyButtonUI::PaintText(HDC hDC)
{
	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
	if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();


	DWORD clrColor = IsEnabled()?m_pKeyBoard->GetKeyTextColor():m_dwDisabledTextColor;

	if( ((m_uButtonState & UISTATE_PUSHED) != 0) && (m_pKeyBoard->GetKeyPushedTextColor() != 0) )
		clrColor = m_pKeyBoard->GetKeyPushedTextColor();
	else if( ((m_uButtonState & UISTATE_SELECTED) != 0) && (m_pKeyBoard->GetKeyPushedTextColor() != 0) )
		clrColor = m_pKeyBoard->GetKeyPushedTextColor();

	RECT rcTextPadding = GetTextPadding();
	GetManager()->GetDPIObj()->Scale(&rcTextPadding);
	RECT rc = m_rcItem;
	rc.left += rcTextPadding.left;
	rc.right -= rcTextPadding.right;
	rc.top += rcTextPadding.top;
	rc.bottom -= rcTextPadding.bottom;

	UINT uTextStyle = m_uTextStyle;
	uTextStyle |= DT_NOPREFIX;

	if(IsOemKey())
	{
		CDuiString sText1 = m_ch;
		RECT rc1;
		rc1.left = rc.left;
		rc1.right = rc.right;
		rc1.top = rc.bottom - (rc.bottom - rc.top)/2;
		rc1.bottom =  rc.bottom;
		CRenderEngine::DrawText(hDC, m_pManager, rc1, sText1, clrColor, m_pKeyBoard->GetFont(m_vkCode), uTextStyle);

		CDuiString sText2 = m_chShift;
		RECT rc2;
		rc2.left = rc.left;
		rc2.right = rc.right;
		rc2.top = rc.top;
		rc2.bottom =  rc.bottom - (rc.bottom - rc.top)/2;
		CRenderEngine::DrawText(hDC, m_pManager, rc2, sText2, clrColor, m_pKeyBoard->GetFont(m_vkCode), uTextStyle);

	}
	else
	{
		CDuiString sText = GetText();
		if( sText.IsEmpty() ) return;

		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, clrColor, m_pKeyBoard->GetFont(m_vkCode), m_uTextStyle);
	}
}

void CKeyButtonUI::PaintBkColor(HDC hDC)
{
	if( (m_uButtonState & UISTATE_DISABLED) != 0 ) {
		if(m_dwDisabledBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwDisabledBkColor));
			return;
		}
	}
	else if( (m_uButtonState & UISTATE_PUSHED) != 0 ) {
		if(m_pKeyBoard->GetKeyPushedBkColor() != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_pKeyBoard->GetKeyPushedBkColor()));
			return;
		}
	}
	else if( (m_uButtonState & UISTATE_SELECTED) != 0 ) {
		if(m_pKeyBoard->GetKeyPushedBkColor() != 0) 
		{
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_pKeyBoard->GetKeyPushedBkColor()));
			return;
		}
	}
	else {
		if(m_pKeyBoard->GetKeyBkColor() != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_pKeyBoard->GetKeyBkColor()));
			return;
		}
	}

	return CControlUI::PaintBkColor(hDC);
}

void CKeyButtonUI::PaintBorder(HDC hDC)
{
	DWORD dwColor = 0;

	if( (m_uButtonState & UISTATE_PUSHED) != 0 ) 
		dwColor = m_pKeyBoard->GetKeyPushedBorderColor();
	else
		dwColor = m_pKeyBoard->GetKeyBorderColor();

	if(dwColor == 0)	
	{
		__super::PaintBorder(hDC);
		return;
	}

	int nBorderSize;
	SIZE cxyBorderRound;
	if (m_pManager) {
		nBorderSize = GetManager()->GetDPIObj()->Scale(m_pKeyBoard->GetKeyBorderSize());
		cxyBorderRound = GetManager()->GetDPIObj()->Scale(m_pKeyBoard->GetKeyBorderRound());
	}
	else {
		nBorderSize = m_pKeyBoard->GetKeyBorderSize();
		cxyBorderRound = m_pKeyBoard->GetKeyBorderRound();
	}

	if(nBorderSize > 0)
	{
		if(cxyBorderRound.cx > 0 || cxyBorderRound.cy > 0 ) 
		{
			CRenderEngine::DrawRoundRect(hDC, m_rcItem, nBorderSize, cxyBorderRound.cx, cxyBorderRound.cy, GetAdjustColor(dwColor), m_nBorderStyle);
		}
		else
		{
			CRenderEngine::DrawRect(hDC, m_rcItem, nBorderSize, GetAdjustColor(dwColor), m_nBorderStyle);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DUICONTROL(CKeyBoardUI)

CKeyBoardUI::CKeyBoardUI(void)
{
	for (int i=0; i<6; i++)
		m_pLine[i] = NULL;
	
	m_nlines = 0;
	m_nKeyPadding = 5;
	m_nKeyWidth = 80;
	m_nKeyHeight = 80;
	m_iFont = -1;
	m_iFontEnterControl = -1;
	m_nKeyBorderSize = 1;
	m_cxKeyBorderRound.cx = 5;
	m_cxKeyBorderRound.cy = 5;
	m_dwKeyTextColor = 0xFFFFFFFF;
	m_dwKeyBkColor = 0xFF383742;
	m_dwKeyBorderColor = 0xFFFFFFFF;
	m_dwKeyPushedTextColor = 0xFF383742;
	m_dwKeyPushedBkColor = 0xFFFFFFFF;
	m_dwKeyPushedBorderColor = 0xFF000000;

	m_bCapsOn = false;
	m_bShiftOn = false;
	m_bCtrlOn = false;
	m_bAltOn = false;

	m_pFocus = NULL;
	m_pBindEnterButton = NULL;

}

CKeyBoardUI::~CKeyBoardUI(void)
{
}

LPCTSTR CKeyBoardUI::GetClass() const
{
	return _T("KeyBoard");
}

LPVOID CKeyBoardUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("KeyBoard")) == 0 ) return static_cast<CKeyBoardUI*>(this);
	return __super::GetInterface(pstrName);
}

void CKeyBoardUI::SetBindFocusControl(CControlUI *pFocusEdit)
{
	m_pFocus = pFocusEdit;
}

void CKeyBoardUI::SetBindEnterControl(CControlUI *pEnterButton)
{
	m_pBindEnterButton = pEnterButton;
}

void CKeyBoardUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("keywidth")) == 0 ) SetKeyWidth(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("keyheight")) == 0 ) SetKeyHeight(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("keypadding")) == 0 ) SetKeyPadding(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("font")) == 0 ) SetFont(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("keybordersize")) == 0 ) SetKeyBorderSize(_ttoi(pstrValue));
	else if( _tcsicmp(pstrName, _T("keyborderround")) == 0 ) {
		SIZE cxyRound = { 0 };
		LPTSTR pstr = NULL;
		cxyRound.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		cxyRound.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		SetKeyBorderRound(cxyRound);
	}
	else if( _tcsicmp(pstrName, _T("keytextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("keybkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("keybordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyBorderColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("keypushedtextcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyPushedTextColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("keypushedbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyPushedBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("keypushedbordercolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetKeyPushedBorderColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("bindfocuscontrolname")) == 0 )
	{
		SetBindFocusControlName(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("bindentercontrolname")) == 0 )
	{
		SetBindEnterControlName(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("entercontrolname")) == 0 )
	{
		SetEnterControlName(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("entercontroltext")) == 0 )
	{
		SetEnterControlText(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("entercontrolfont")) == 0 )
	{
		SetEnterControlFont(_ttoi(pstrValue));
	}
	else __super::SetAttribute(pstrName, pstrValue);
}

int CKeyBoardUI::CalcKeyBoardWidth()
{	
	return m_nKeyWidth*15 + m_nKeyPadding*15;
}

int CKeyBoardUI::CalcKeyBoardHeight()
{
	return m_nKeyHeight*5 + m_nKeyPadding*4;
}

void CKeyBoardUI::DoInit()
{
	InitKeyBoard(5);
	InitKeyButton();
}

void CKeyBoardUI::InitKeyBoard(int lines)
{
	m_nlines = lines;

	SetChildAlign(DT_CENTER);
	SetChildVAlign(DT_VCENTER);
	SetChildPadding(GetKeyPadding());
	for (int i=0; i<lines; i++)
	{
		m_pLine[i] = new CHorizontalLayoutUI;
		m_pLine[i]->SetFixedWidth(CalcKeyBoardWidth());
		m_pLine[i]->SetFixedHeight(GetKeyHeight());
		m_pLine[i]->SetChildPadding(GetKeyPadding());
		Add(m_pLine[i]);
	}
}

void CKeyBoardUI::InitKeyButton()
{
	CKeyButtonUI *pKeyButton = NULL;
	int nKeyWidth = GetKeyWidth();
	int nKeyHeight = GetKeyHeight();
	int nKeyPadding = GetKeyPadding();

	AddKey(0, '`',	'~',	VK_OEM_3); 
	AddKey(0, '1',	'!',	0x31);
	AddKey(0, '2',	'@',	0x32);
	AddKey(0, '3',	'#',	0x33);
	AddKey(0, '4',	'$',	0x34);
	AddKey(0, '5',	'%',	0x35);
	AddKey(0, '6',	'^',	0x36);
	AddKey(0, '7',	'&',	0x37);
	AddKey(0, '8',	'*',	0x38);
	AddKey(0, '9',	'(',	0x39);
	AddKey(0, '0',	')',	0x30);
	AddKey(0, '-',	'_',	VK_OEM_MINUS);
	AddKey(0, '=',	'+',	VK_OEM_PLUS);
	AddKey(0, VK_BACK, VK_BACK, VK_BACK, 0);

	AddKey(1, VK_TAB, VK_TAB, VK_TAB, nKeyWidth + nKeyWidth/2 + nKeyPadding);
	AddKey(1, 'q',	'Q',	0x51);
	AddKey(1, 'w',	'W',	0x57);
	AddKey(1, 'e',	'E',	0x45);
	AddKey(1, 'r',	'R',	0x52);
	AddKey(1, 't',	'T',	0x54);
	AddKey(1, 'y',	'Y',	0x59);
	AddKey(1, 'u',	'U',	0x55);
	AddKey(1, 'i',	'I',	0x49);
	AddKey(1, 'o',	'O',	0x4f);
	AddKey(1, 'p',	'P',	0x50);
	AddKey(1, '[',	'{', VK_OEM_4);
	AddKey(1, ']',	'}', VK_OEM_6);
	AddKey(1, '\\', '|', VK_OEM_5, 0);

	AddKey(2, VK_CAPITAL, VK_CAPITAL, VK_CAPITAL, nKeyWidth*2 + nKeyPadding);
	AddKey(2, 'a',	'A',	0x41);
	AddKey(2, 's',	'S',	0x53);
	AddKey(2, 'd',	'D',	0x44);
	AddKey(2, 'f',	'F',	0x46);
	AddKey(2, 'g',	'G',	0x47);
	AddKey(2, 'h',	'H',	0x48);
	AddKey(2, 'j',	'J',	0x4a);
	AddKey(2, 'k',	'K',	0x4b);
	AddKey(2, 'l',	'L',	0x4c);
	AddKey(2, ';',	':',	VK_OEM_1);
	AddKey(2, '\'',	'"',	VK_OEM_7);
	pKeyButton = AddKey(2, VK_RETURN,	VK_RETURN, VK_RETURN, 0);
	pKeyButton->SetName(GetEnterControlName());

	AddKey(3, VK_SHIFT,	VK_SHIFT,	VK_SHIFT, nKeyWidth*2 + nKeyWidth/2 + nKeyPadding*2);
	AddKey(3, 'z',	'Z',	0x5A);
	AddKey(3, 'x',	'X',	0x58);
	AddKey(3, 'c',	'C',	0x43);
	AddKey(3, 'v',	'V',	0x56);
	AddKey(3, 'b',	'B',	0x42);
	AddKey(3, 'n',	'N',	0x4e);
	AddKey(3, 'm',	'M',	0x4d);
	AddKey(3, ',',	'<',	VK_OEM_COMMA);
	AddKey(3, '.',	'>',	VK_OEM_PERIOD);
	AddKey(3, '/',	'?',	VK_OEM_2);
	AddKey(3, VK_SHIFT,	VK_SHIFT, VK_SHIFT, nKeyWidth*2 + nKeyWidth/2 + nKeyPadding*2);

	AddKey(4, VK_CONTROL,	VK_CONTROL, VK_CONTROL);
	AddKey(4, VK_MENU,		VK_MENU,	VK_MENU);
	AddKey(4, ' ',			' ',		VK_SPACE, 0);
	AddKey(4, VK_ESCAPE,	VK_ESCAPE,	VK_ESCAPE);
	AddKey(4, VK_DELETE,	VK_DELETE,	VK_DELETE);
	AddKey(4, VK_UP,		VK_UP,		VK_UP);
	AddKey(4, VK_LEFT,		VK_LEFT,	VK_LEFT);
	AddKey(4, VK_DOWN,		VK_DOWN,	VK_DOWN);
	AddKey(4, VK_RIGHT,		VK_RIGHT,	VK_RIGHT);
}

CKeyButtonUI* CKeyBoardUI::AddKey(int line, TCHAR ch, TCHAR chShift, BYTE vkCode, int width, int height)
{
	CKeyButtonUI *pKey = new CKeyButtonUI;
	pKey->SetChar(ch,	chShift, vkCode);
	pKey->SetOwner(this);
	m_pLine[line]->Add(pKey);

	if(width == -1) 
		pKey->SetFixedWidth(m_nKeyWidth);
	else if(width > 0)
		pKey->SetFixedWidth(width);

	if(height == -1) 
		pKey->SetFixedHeight(m_nKeyHeight);
	else if(height > 0)
		pKey->SetFixedHeight(height);

 	pKey->SetBorderRound(GetKeyBorderRound());
	return pKey;
}

void CKeyBoardUI::AddLabel(int line, int width, int height)
{
	CButtonUI *pKey = new CButtonUI;
	m_pLine[line]->Add(pKey);
	pKey->SetFixedWidth(width);
	pKey->SetFixedHeight(height);
}

void CKeyBoardUI::OnKeyPress(CKeyButtonUI *pKey)
{
	CControlUI *pFocus = GetBindFocusControl();
	if(pFocus == NULL)	return;

	TCHAR ch = pKey->GetChar();
	BYTE vkCode = pKey->GetVkCode();

	if(pKey->IsControlKey())
	{
		if(ch == VK_RETURN)		{ if(GetBindEnterControl()) GetBindEnterControl()->Activate();	return; }
		if(ch == VK_CONTROL)	{ SetCtrlON(!IsCtrlON());					return; }
		if(ch == VK_MENU)		{ SetAltON(!IsAltON());						return; }
		if(ch == VK_SHIFT)		{ SetShiftON(!IsShiftON()); Invalidate();	return; }
		if(ch == VK_CAPITAL)	{ SetCapsON(!IsCapsON());	Invalidate();	return; }
	}

	UINT uState = 0;
	if( IsCtrlON() ) uState |= MK_CONTROL;
	if( IsShiftON() ) uState |= MK_SHIFT;
	if( IsAltON() ) uState |= MK_ALT;

	CRichEditUI *pRichEdit = dynamic_cast<CRichEditUI *>(pFocus);
	if(pRichEdit)
	{
		pFocus->SetFocus();

		if( vkCode==VK_TAB && !pRichEdit->IsWantTab() )
			GetManager()->SetNextTabControl(!IsShiftON());
		else if(vkCode==VK_RETURN && !pRichEdit->IsWantReturn())
			pRichEdit->TxSendMessage(WM_CHAR, ch, 0, NULL);
		else if(pKey->IsControlKey())
			pRichEdit->TxSendMessage(WM_KEYDOWN, ch, 0, 0);
		else
			pRichEdit->TxSendMessage(WM_CHAR, ch, 0, NULL);
	}
	else
	{
		TEventUI event = { 0 };
		event.Type = UIEVENT_CHAR;
		event.pSender = pFocus;
		event.wParam = 0;
		event.lParam = 0;
		event.chKey = ch;
		event.ptMouse = GetManager()->GetMousePos();
		event.wKeyState = uState;
		event.dwTimestamp = ::GetTickCount();
		pFocus->Event(event);
	}		
}

CControlUI* CKeyBoardUI::GetBindFocusControl()
{
	if(!m_sBindFocusControlName.IsEmpty())
	{
		CControlUI *pFocus = GetManager()->FindControl(m_sBindFocusControlName.GetData());
		if(pFocus)	return pFocus;
		return NULL;
	}

	CControlUI *pFocus = GetManager()->GetLastFocus();
	if(pFocus == this)	return m_pFocus;
	for (int i=0; i<6; i++)
	{
		if(pFocus == m_pLine[i]) return m_pFocus;
	}
	if(_tcscmp(pFocus->GetClass(), _T("KeyButton")) == 0)
		return m_pFocus;

	m_pFocus = pFocus;
	return m_pFocus;
}

CControlUI* CKeyBoardUI::GetBindEnterControl()
{
	if(!m_sBindEnterControlName.IsEmpty())
	{
		CControlUI *pEnter = GetManager()->FindControl(m_sBindEnterControlName.GetData());
		if(pEnter)	return pEnter;
		return NULL;
	}
	return m_pBindEnterButton;
}

CDuiString CKeyBoardUI::GetKeyText(BYTE vkCode)
{
	CDuiString sText;
	switch (vkCode)
	{
	case  VK_BACK:
		sText = _T("¡û");
		break;
	case  VK_TAB: 
		sText = _T("Tab");
		break;
	case  VK_CAPITAL: 
		sText = _T("Caps");
		break;
	case  VK_RETURN: 
		{
			if(!m_sEnterControlText.IsEmpty())
				sText = m_sEnterControlText;
			else
				sText = _T("Enter");
		}
		break;
	case  VK_SHIFT: 
		sText = _T("Shift");
		break;
	case  VK_CONTROL: 
		sText = _T("Ctrl");
		break;
	case  VK_MENU: 
		sText = _T("Alt");
		break;
	case  VK_ESCAPE: 
		sText = _T("Esc");
		break;
	case  VK_DELETE: 
		sText = _T("Del");
		break;
	case  VK_UP: 
		sText = _T("¡ü");
		break;
	case  VK_DOWN: 
		sText = _T("¡ý");
		break;
	case  VK_LEFT: 
		sText = _T("¡û");
		break;
	case  VK_RIGHT:
		sText = _T("¡ú");
		break;
	default:
		break;
	}

	return sText;
}


int CKeyBoardUI::GetFont(BYTE vkCode)
{
	if(vkCode == VK_RETURN)
	{
		if(GetEnterControlFont() <= -1)
			return GetFont();

		return GetEnterControlFont();
	}
	
	return GetFont();
}

}