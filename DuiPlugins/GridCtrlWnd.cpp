#include "StdAfx.h"
#include "GridCtrlWnd.h"

#include "GridCtrlUI.h"

CGridCtrlWnd::CGridCtrlWnd(void) : m_pGrid(NULL)
{
}


CGridCtrlWnd::~CGridCtrlWnd(void)
{
}

void CGridCtrlWnd::Init(CGridCtrlUI* pOwner)
{
	m_pOwner = pOwner;
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_CHILD|WS_VISIBLE, 0, m_rcWindow);
	ASSERT(m_hWnd);

	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME | WS_BORDER));  //È¥±ß¿ò
}

void CGridCtrlWnd::SetPos(RECT rc, bool bNeedInvalidate)
{
	m_rcWindow = rc;
	::MoveWindow(GetHWND(), rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);
}

LPCTSTR CGridCtrlWnd::GetWindowClassName() const
{
	return _T("GridCtrl");
}

void CGridCtrlWnd::OnFinalMessage(HWND hWnd)
{
	if(m_pGrid) { delete m_pGrid; m_pGrid = NULL; }
}

LRESULT CGridCtrlWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	if( uMsg == WM_CREATE ) 
	{
		m_pOwner->GetManager()->AddNativeWindow(m_pOwner, m_hWnd);

		CWnd *pWnd = CWnd::FromHandle(m_hWnd);
		m_pGrid = new CGridCtrl();
		m_pGrid->Create(m_rcWindow, pWnd, 1, WS_CHILD|WS_VISIBLE);
		ASSERT(m_pGrid);
		ASSERT(m_pGrid->m_hWnd);

	}
	else if( uMsg == WM_SIZE) 
	{
		CRect rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		m_pGrid->MoveWindow(rcClient);

		//m_pGrid->ExpandColumnsToFit(TRUE);
	}
	else if(uMsg == WM_NOTIFY)
	{
		m_pOwner->OnGridNotify(uMsg, wParam, lParam);
	}

	if( !bHandled ) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}