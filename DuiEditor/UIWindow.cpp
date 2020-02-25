// UIWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "UIWindow.h"

#include "wm.c"

// CUIWindow
CUIWindow::CUIWindow()
{

}

CUIWindow::~CUIWindow()
{
}

CString CUIWindow::GetMessageText(UINT message)
{
	int size = sizeof(gaMsgs) / sizeof(MSGDESC);
	for (int i=0; i<size; i++)
	{
		if(gaMsgs[i].msg == message)
		{
			return gaMsgs[i].pszMsg;
		}
	}
	return _T("");
}

BOOL CUIWindow::DestroyUiWindow()
{
	return ::DestroyWindow(GetSafeHwnd());
}

BOOL CUIWindow::GetWindowRect(LPRECT lpRect)
{
	return ::GetWindowRect(GetSafeHwnd(), lpRect);
}

void CUIWindow::Invalidate()
{
	InvalidateRect(GetSafeHwnd(), NULL, TRUE);
}

BOOL CUIWindow::MoveWindow(__in int X, __in int Y, __in int nWidth, __in int nHeight, __in BOOL bRepaint)
{
	return ::MoveWindow(GetSafeHwnd(), X, Y, nWidth, nHeight, bRepaint);
}