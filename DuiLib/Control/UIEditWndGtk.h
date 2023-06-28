#ifndef __UIEDIT_WND_GTK_H__
#define __UIEDIT_WND_GTK_H__

#pragma once

#ifdef DUILIB_GTK
namespace DuiLib
{
	class CEditWndGtk : public CWindowWnd
	{
	public:
		CEditWndGtk();

		void Init(CEditUI* pOwner);
		RECT CalPos();

		void edit_SetPasswordMode(BOOL bPasswordMode);
		void edit_SetSel(int ichStart, int ichEnd);
		void edit_SetText(LPCTSTR pstrText);
		void edit_LimitText(UINT uMax);
		void edit_SetReadOnly(BOOL bReadOnly);
		void edit_SetPasswordChar(TCHAR cPasswordChar);
		void edit_ReplaceSel(LPCTSTR lpszReplace);
	};
} //namespace DuiLib
#endif //#ifdef DUILIB_GTK
#endif // __UIEDIT_H__