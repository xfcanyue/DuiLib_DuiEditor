#include "StdAfx.h"
#include "UIFrameWnd.h"

#ifdef DUILIB_WIN32
namespace DuiLib{

CUIDialog::CUIDialog(void)
{
	m_nMode = 0;
	_bModal = false;
	m_bEnterCloseOK=TRUE;
	m_bEscCloseCancel=TRUE;
	__refCount = 1;
}

CUIDialog::CUIDialog(LPCTSTR sSkinFile)
{
	m_nMode = 0;
	_bModal = false;
	m_bEnterCloseOK=TRUE;
	m_bEscCloseCancel=TRUE;

	m_sSkinFile = sSkinFile;
	m_sWindowClassName = m_sSkinFile;
	__refCount = 1;
}

CUIDialog::~CUIDialog(void)
{
	m_mScriptNotify.clear();
}

LPCTSTR CUIDialog::GetWindowClassName() const
{
	return m_sSkinFile;
}

CDuiString CUIDialog::GetSkinFile()
{
	return m_sWindowClassName;
}

void CUIDialog::OnFinalMessage( HWND hWnd )
{
	if(::IsWindow(GetParent(hWnd)))
	{
		SetForegroundWindow(::GetParent(hWnd));
	}
	CUIFrameWnd::OnFinalMessage(hWnd);

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

		::SetFocus(GetManager()->GetPaintWindow()); //让主窗口获取焦点，使内部的子窗口隐藏，从而获取子窗口内容。

		CButtonUI *pBtnOk = dynamic_cast<CButtonUI *>(FindControl(_T("btn_ok")));
		if(pBtnOk)
			pBtnOk->Activate();
		else
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

		CButtonUI *pBtnCancel = dynamic_cast<CButtonUI *>(FindControl(_T("btn_cancel")));
		if(pBtnCancel)
			pBtnCancel->Activate();
		else
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

UINT CUIDialog::DoModal(HWND hWndParent)
{
	Create(hWndParent, GetWindowClassName(), UI_WNDSTYLE_DIALOG|WS_SIZEBOX, WS_EX_WINDOWEDGE);
	CenterWindow();
	_bModal = true;
	return ShowModal();
}

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

void CUIDialog::ShowDialog(HWND hWndParent)
{
	Create(hWndParent, GetWindowClassName(), WS_POPUP|UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	CenterWindow();
	_bModal = false;
	return ShowWindow();
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
	IScriptManager *pScriptEngine = CPaintManagerUI::GetScriptEngine();
	if(pScriptEngine)
	{
		std::map<CDuiString, CDuiString>::iterator it = m_mScriptNotify.find(msg.sType);
		if (it != m_mScriptNotify.end())
		{
			int r = 0;
			CAutoScriptContext ctx(pScriptEngine);
			r = ctx->SetFunByName(it->second);	assert( r >= 0 );
			r = ctx->SetArgObject(0, GetManager());		assert( r >= 0 );
			r = ctx->SetArgObject(1, &msg);		assert( r >= 0 );
			r = ctx->Execute();
		}
	}

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
	CUIFrameWnd::Notify(msg);
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

void CUIDialog::__AddRef()
{
	__refCount++;
}

void CUIDialog::__ReleaseRef()
{
	if( --__refCount == 0 )
		delete this;
}

void CUIDialog::RegScriptNotify(LPCTSTR sNotifyType, LPCTSTR sFunName)
{
	m_mScriptNotify[sNotifyType] = sFunName;
}

} // namespace DuiLib{
#endif //#ifdef DUILIB_WIN32
