#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

namespace DuiLib{

CUIDialog::CUIDialog(void)
{
	m_nMode = 0;
	_bModal = false;
	m_bEnterCloseOK=TRUE;
	m_bEscCloseCancel=TRUE;
}

CUIDialog::~CUIDialog(void)
{
	
}

void CUIDialog::OnFinalMessage( HWND hWnd )
{
	if(::IsWindow(GetParent(hWnd)))
	{
		SetForegroundWindow(::GetParent(hWnd));
	}
	__super::OnFinalMessage(hWnd);

	if(!_bModal)
	{
		delete this;
	}
}

LRESULT CUIDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		if(!IsEnterCloseOK())
			return S_FALSE;

		CControlUI *pFocus = GetManager()->GetFocus();
		if(pFocus && !pFocus->OnEnableResponseDefaultKeyEvent(wParam))
			return S_FALSE;

		OnClickOK();
		return S_OK;
	}
	else if (wParam == VK_ESCAPE)
	{
		if(!IsEscCloseCancel())
			return S_FALSE;

		CControlUI *pFocus = GetManager()->GetFocus();
		if(pFocus && !pFocus->OnEnableResponseDefaultKeyEvent(wParam))
			return S_FALSE;

		OnClickCancel();
		return S_OK;
	}

	return S_FALSE;
}

void CUIDialog::SetDefaultKeyEvent(BOOL bEnterCloseOK, BOOL bEscCloseCancel)
{
	m_bEnterCloseOK = bEnterCloseOK;
	m_bEscCloseCancel = bEscCloseCancel;
}

BOOL CUIDialog::IsEnterCloseOK() const { return m_bEnterCloseOK; }

BOOL CUIDialog::IsEscCloseCancel() const { return m_bEscCloseCancel; }

UINT CUIDialog::DoModal(CUIFrmBase *pParentWnd)
{
	if(pParentWnd)
		Create(pParentWnd->GetManager()->GetPaintWindow(), GetWindowClassName(), UI_WNDSTYLE_DIALOG|WS_SIZEBOX, WS_EX_WINDOWEDGE);
	else
		Create(NULL, GetWindowClassName(), UI_WNDSTYLE_DIALOG|WS_SIZEBOX, WS_EX_WINDOWEDGE);
	CenterWindow();
	_bModal = true;
	return ShowModal();
}

void CUIDialog::ShowDialog(CUIFrmBase *pParentWnd)
{
	if(pParentWnd)
		Create(pParentWnd->GetManager()->GetPaintWindow(), GetWindowClassName(), WS_POPUP|UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	else
		Create(NULL, GetWindowClassName(), WS_POPUP|UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	CenterWindow();
	_bModal = false;
	return ShowWindow();
}

void CUIDialog::InitWindow()
{
	switch (m_nMode)
	{
	case TMD_ADD:
		{
			on_tmd_add_initwindow();
		}
		break;
	case TMD_MODIFY:
		{
			on_tmd_modify_initwindow();
		}
		break;
	case TMD_DELETE:
		{
			on_tmd_delete_initwindow();
		}
		break;
	}
}

void CUIDialog::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK)
	{
		if(IsControl(msg, _T("btn_ok")))
		{
			OnClickOK();
		}
		else if(IsControl(msg, _T("btn_cancel")))
		{
			OnClickCancel();
		}
	}
	__super::Notify(msg);
}

void CUIDialog::OnClickOK()
{
	switch (m_nMode)
	{
	case TMD_ADD:
		{
			if(!on_tmd_add()) return;
		}
		break;
	case TMD_MODIFY:
		{
			if(!on_tmd_modify()) return;
		}
		break;
	case TMD_DELETE:
		{
			if(!on_tmd_delete()) return;
		}
		break;
	}

	Close(IDOK);
}

void CUIDialog::OnClickCancel()
{
	Close(IDCANCEL);
}

void CUIDialog::on_tmd_add_initwindow() 
{

}

void CUIDialog::on_tmd_modify_initwindow()
{

}

void CUIDialog::on_tmd_delete_initwindow()
{
}

bool CUIDialog::on_tmd_add()
{
	return true;
}

bool CUIDialog::on_tmd_modify()
{
	return true;
}

bool CUIDialog::on_tmd_delete()
{
	return true;
}

} // namespace DuiLib{