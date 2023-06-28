#include "StdAfx.h"
#include "UIComboEditWndGtk.h"


#ifdef DUILIB_GTK
namespace DuiLib
{

CComboEditWndGtk::CComboEditWndGtk() : m_pOwner(NULL), m_bInit(false), m_bDrawCaret(false)
{
}

void CComboEditWndGtk::Init(CComboExUI* pOwner)
{
	
}

RECT CComboEditWndGtk::CalPos()
{
	CDuiRect rcPos = m_pOwner->GetPos();
	
	return rcPos;
}

void CComboEditWndGtk::OnFinalMessage(UIWND hWnd)
{
	
}

LRESULT CComboEditWndGtk::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	
	return lRes;
}

LRESULT CComboEditWndGtk::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;

	return lRes;
}

LRESULT CComboEditWndGtk::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

void CComboEditWndGtk::edit_SetPasswordMode(BOOL bPasswordMode)
{

}

void CComboEditWndGtk::edit_SetSel(int ichStart, int ichEnd)
{

}

void CComboEditWndGtk::edit_SetText(LPCTSTR pstrText)
{

}

void CComboEditWndGtk::edit_LimitText(UINT uMax)
{

}

void CComboEditWndGtk::edit_SetReadOnly(BOOL bReadOnly)
{

}

void CComboEditWndGtk::edit_SetPasswordChar(TCHAR cPasswordChar)
{

}

void CComboEditWndGtk::edit_ReplaceSel(LPCTSTR lpszReplace)
{

}

}

#endif //#ifdef DUILIB_GTK
