#include "StdAfx.h"

#include "UIMenuWndGtk.h"

#ifdef DUILIB_GTK
namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	CMenuWndGtk::CMenuWndGtk():
	m_pOwner(NULL),
		m_pLayout(),
		m_xml(_T("")),
		isClosing(false),
		m_bAutoDestroy(false)
	{
		m_dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;
	}

	CMenuWndGtk::~CMenuWndGtk()
	{
		
	}

	void CMenuWndGtk::Close(UINT nRet)
	{
		
	}


	BOOL CMenuWndGtk::Receive(ContextMenuParam param)
	{
		return TRUE;
	}

	CMenuWndGtk* CMenuWndGtk::CreateMenu(CMenuElementUI* pOwner, STRINGorID xml, POINT point, CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo /*= NULL*/, DWORD dwAlignment /*= eMenuAlignment_Left | eMenuAlignment_Top*/)
	{
		CMenuWndGtk* pMenu = new CMenuWndGtk;
		pMenu->Init(pOwner, xml, point, pMainPaintManager, pMenuCheckInfo, dwAlignment);
		return pMenu;
	}

	void CMenuWndGtk::DestroyMenu()
	{
		
	}
	
	MenuItemInfo* CMenuWndGtk::SetMenuItemInfo(LPCTSTR pstrName, bool bChecked)
	{	
		return NULL;
	}

	void CMenuWndGtk::Init(CMenuElementUI* pOwner, STRINGorID xml, POINT point,
		CPaintManagerUI* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo/* = NULL*/,
		DWORD dwAlignment/* = eMenuAlignment_Left | eMenuAlignment_Top*/)
	{
		
	}

	LPCTSTR CMenuWndGtk::GetWindowClassName() const
	{
		return _T("DuiMenuWnd");
	}


	void CMenuWndGtk::Notify(TNotifyUI& msg)
	{		

	}

	CControlUI* CMenuWndGtk::CreateControl( LPCTSTR pstrClassName )
	{
		if (_tcsicmp(pstrClassName, _T("Menu")) == 0)
		{
			return new CMenuUI();
		}
		else if (_tcsicmp(pstrClassName, _T("MenuElement")) == 0)
		{
			return new CMenuElementUI();
		}
		return NULL;
	}


	void CMenuWndGtk::OnFinalMessage(UIWND hWnd)
	{
		RemoveObserver();
		if( m_pOwner != NULL ) {
			for( int i = 0; i < m_pOwner->GetCount(); i++ ) {
				if( static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement"))) != NULL ) {
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
					(static_cast<CMenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(false);
				}
			}
			m_pOwner->m_pWindow = NULL;
			m_pOwner->SetPushedState(false);
			m_pOwner->Invalidate();

			// 内部创建的内部删除
			delete this;
		}
		else
		{
			if(m_bAutoDestroy)
			{
				DestroyMenu();
				// 外部创建的也删除
				delete this;
			}
		}
	}

	LRESULT CMenuWndGtk::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	CMenuUI* CMenuWndGtk::GetMenuUI()
	{
		return dynamic_cast<CMenuUI*>(m_pm.GetRoot());
	}

	void CMenuWndGtk::ResizeMenu()
	{
		
	}

	void CMenuWndGtk::ResizeSubMenu()
	{
		
	}

	void CMenuWndGtk::setDPI(int DPI) 
	{
		m_pm.SetDPI(DPI);
	}


	LRESULT CMenuWndGtk::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}
	LRESULT CMenuWndGtk::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT CMenuWndGtk::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg )
		{
		case WM_CREATE:       
			lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_KILLFOCUS:       
			lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
			break;
		case WM_KEYDOWN:
			if( wParam == VK_ESCAPE || wParam == VK_LEFT)
				Close();
			break;
		case WM_SIZE:
			lRes = OnSize(uMsg, wParam, lParam, bHandled);
			break;
		case WM_CLOSE:
			if( m_pOwner != NULL )
			{
				m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
				m_pOwner->SetPos(m_pOwner->GetPos());
				m_pOwner->SetFocus();
			}
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return 0L;
			break;
		default:
			bHandled = FALSE;
			break;
		}

		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

} // namespace DuiLib
#endif // #ifdef DUILIB_GTK


