#include "StdAfx.h"
#include "OptionExUI.h"


IMPLEMENT_DUICONTROL(COptionExUI)

COptionExUI::COptionExUI(void) : m_bSelected(false), m_dwHotBkColor(0), m_dwSelectedBkColor(0)
{
	SetInset(CDuiRect(5,0,5,0));
	SetChildVAlign(DT_VCENTER);

	m_pLeftIcon = new CControlUI;
	m_pLeftIcon->SetFixedWidth(20);
	m_pLeftIcon->SetFixedHeight(20);
	Add(m_pLeftIcon);

	m_pCenterLabel = new CLabelUI;
	m_pCenterLabel->SetTextStyle(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//m_pCenterLabel->SetBkColor(UIRGB(0,0,255));
	Add(m_pCenterLabel);

	m_pRightIcon = new COptionUI;
	m_pRightIcon->SetFixedWidth(12);
	m_pRightIcon->SetFixedHeight(12);
	Add(m_pRightIcon);
}


COptionExUI::~COptionExUI(void)
{
}

LPCTSTR COptionExUI::GetClass() const
{
	return _T("OptionEx");
}

LPVOID COptionExUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcsicmp(pstrName, _T("OptionEx")) == 0 ) return static_cast<COptionExUI*>(this);
	return __super::GetInterface(pstrName);
}

UINT COptionExUI::GetControlFlags() const
{
	return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void COptionExUI::DoInit()
{
	
}

void COptionExUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else COptionExUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETFOCUS ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_KILLFOCUS ) 
	{
		Invalidate();
	}
	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) m_uButtonState |= UISTATE_PUSHED;
			else m_uButtonState &= ~UISTATE_PUSHED;
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( IsEnabled() ) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
	}

	__super::DoEvent(event);
}


bool COptionExUI::Activate()
{
	if( !m_sGroupName.IsEmpty() ) Selected(true);
	else Selected(!m_bSelected);
	return true;
}

void COptionExUI::PaintBkColor(HDC hDC)
{
	if(IsSelected()) {
		if(m_dwSelectedBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwSelectedBkColor));
		}
	}
	else if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		if(m_dwHotBkColor != 0) {
			CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwHotBkColor));
			return;
		}
	}
}

void COptionExUI::SetHotBkColor( DWORD dwColor )
{
	m_dwHotBkColor = dwColor;
}

DWORD COptionExUI::GetHotBkColor() const
{
	return m_dwHotBkColor;
}

void COptionExUI::SetSelectedBkColor( DWORD dwBkColor )
{
	m_dwSelectedBkColor = dwBkColor;
}

DWORD COptionExUI::GetSelectBkColor()
{
	return m_dwSelectedBkColor;
}

LPCTSTR COptionExUI::GetGroup() const
{
	return m_sGroupName;
}

void COptionExUI::SetGroup(LPCTSTR pStrGroupName)
{
	if( pStrGroupName == NULL ) {
		if( m_sGroupName.IsEmpty() ) return;
		m_sGroupName.Empty();
	}
	else {
		if( m_sGroupName == pStrGroupName ) return;
		if (!m_sGroupName.IsEmpty() && m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
		m_sGroupName = pStrGroupName;
	}

	if( !m_sGroupName.IsEmpty() ) {
		if (m_pManager) m_pManager->AddOptionGroup(m_sGroupName, this);
	}
	else {
		if (m_pManager) m_pManager->RemoveOptionGroup(m_sGroupName, this);
	}

	Selected(m_bSelected);
}

bool COptionExUI::IsSelected() const
{
	return m_bSelected;
}

void COptionExUI::Selected(bool bSelected)
{
	if(m_bSelected == bSelected) return;

	m_bSelected = bSelected;
	if( m_bSelected ) m_uButtonState |= UISTATE_SELECTED;
	else m_uButtonState &= ~UISTATE_SELECTED;

	if( m_pManager != NULL ) {
		if( !m_sGroupName.IsEmpty() ) {
			if( m_bSelected ) {
				CStdPtrArray* aOptionGroup = m_pManager->GetOptionGroup(m_sGroupName);
				for( int i = 0; i < aOptionGroup->GetSize(); i++ ) {
					COptionExUI* pControl = static_cast<COptionExUI*>(aOptionGroup->GetAt(i));
					if( pControl != this ) {
						pControl->Selected(false);
					}
				}
				m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
			}
		}
		else {
			m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
		}
	}

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
void COptionExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("leftimage")) == 0 ) 
	{
		m_pLeftIcon->SetBkImage(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("text")) == 0 ) 
	{
		m_pCenterLabel->SetText(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("rightnormalimage")) == 0 ) 
	{
		m_pRightIcon->SetNormalImage(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("rightselectedimage")) == 0 ) 
	{
		m_pRightIcon->SetSelectedImage(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("righthotimage")) == 0 ) 
	{
		m_pRightIcon->SetHotImage(pstrValue);
	}
	else if( _tcsicmp(pstrName, _T("hotbkcolor")) == 0 )
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetHotBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("selectedbkcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetSelectedBkColor(clrColor);
	}
	else if( _tcsicmp(pstrName, _T("group")) == 0 ) SetGroup(pstrValue);
	else if( _tcsicmp(pstrName, _T("selected")) == 0 ) Selected(_tcsicmp(pstrValue, _T("true")) == 0);

	__super::SetAttribute(pstrName, pstrValue);
}