#include "StdAfx.h"
#include "UIEditWndGtk.h"

#ifdef DUILIB_GTK
namespace DuiLib
{
	CEditWndGtk::CEditWndGtk()
	{
		
	}

	void CEditWndGtk::Init(CEditUI* pOwner)
	{
		 
	}

	RECT CEditWndGtk::CalPos()
	{
		CDuiRect rcPos;
		return rcPos;
	}

	void CEditWndGtk::edit_SetPasswordMode(BOOL bPasswordMode)
	{

	}

	void CEditWndGtk::edit_SetSel(int ichStart, int ichEnd)
	{

	}

	void CEditWndGtk::edit_SetText(LPCTSTR pstrText)
	{

	}

	void CEditWndGtk::edit_LimitText(UINT uMax)
	{

	}

	void CEditWndGtk::edit_SetReadOnly(BOOL bReadOnly)
	{

	}

	void CEditWndGtk::edit_SetPasswordChar(TCHAR cPasswordChar)
	{

	}

	void CEditWndGtk::edit_ReplaceSel(LPCTSTR lpszReplace)
	{

	}
}
#endif //#ifdef DUILIB_GTK
