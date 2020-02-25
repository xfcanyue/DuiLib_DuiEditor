#include "StdAfx.h"
#include "MenuElementExUI.h"

#include "UIWindowMenu.h"

namespace DuiLib {

CMenuElementExUI::CMenuElementExUI(void)
{
}


CMenuElementExUI::~CMenuElementExUI(void)
{
}


void CMenuElementExUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		CListContainerElementUI::DoEvent(event);
		if( m_pWindow ) return;
		bool hasSubMenu = false;
		for( int i = 0; i < GetCount(); ++i )
		{
			if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

				hasSubMenu = true;
			}
		}
		if( hasSubMenu )
		{
			m_pOwner->SelectItem(GetIndex(), true);
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 2;
			CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}
		return;
	}


	if (event.Type == UIEVENT_MOUSELEAVE) {

		bool hasSubMenu = false;
		for (int i = 0; i < GetCount(); ++i)
		{
			if (GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL)
			{

				hasSubMenu = true;
			}
		}

		if (!hasSubMenu) 
		{
			m_pOwner->SelectItem(-1, true);
		}
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( IsEnabled() ){
			CListContainerElementUI::DoEvent(event);

			if( m_pWindow ) return;

			bool hasSubMenu = false;
			for( int i = 0; i < GetCount(); ++i ) {
				if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL ) {
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
					(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);

					hasSubMenu = true;
				}
			}
			if( hasSubMenu )
			{
				CreateMenuWnd();
			}
			else
			{
				SetChecked(!GetChecked());


				bool isClosing = false;
				CMenuUI* menuUI=static_cast<CMenuUI*>(GetManager()->GetRoot());
				isClosing = (menuUI->m_pWindow->isClosing);
				if (IsWindow(GetManager()->GetPaintWindow()) && !isClosing) {
					if (CMenuWnd::GetGlobalContextMenuObserver().GetManager() != NULL)
					{

						MenuCmd* pMenuCmd = new MenuCmd();
						lstrcpy(pMenuCmd->szName, GetName().GetData());
						lstrcpy(pMenuCmd->szUserData, GetUserData().GetData());
						lstrcpy(pMenuCmd->szText, GetText().GetData());
						pMenuCmd->bChecked = GetChecked();
						if (!PostMessage(CMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), WM_MENUCLICK, (WPARAM)pMenuCmd, (LPARAM)this))
						{
							delete pMenuCmd;
							pMenuCmd = NULL;
						}
					}
				}
				ContextMenuParam param;
				param.hWnd = m_pManager->GetPaintWindow();
				param.wParam = 1;
				CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
			}
		}

		return;
	}

	if ( event.Type == UIEVENT_KEYDOWN && event.chKey == VK_RIGHT )
	{
		if( m_pWindow ) return;
		bool hasSubMenu = false;
		for( int i = 0; i < GetCount(); ++i )
		{
			if( GetItemAt(i)->GetInterface(_T("MenuElement")) != NULL )
			{
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetVisible(true);
				(static_cast<CMenuElementUI*>(GetItemAt(i)->GetInterface(_T("MenuElement"))))->SetInternVisible(true);
				hasSubMenu = true;
			}
		}
		if( hasSubMenu )
		{
			m_pOwner->SelectItem(GetIndex(), true);
			CreateMenuWnd();
		}
		else
		{
			ContextMenuParam param;
			param.hWnd = m_pManager->GetPaintWindow();
			param.wParam = 2;
			CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
			m_pOwner->SelectItem(GetIndex(), true);
		}

		return;
	}

	CListContainerElementUI::DoEvent(event);
}


void CMenuElementExUI::CreateMenuWnd()
{
	if( m_pWindow ) return;

	m_pWindow = new CUIWindowMenu();
	ASSERT(m_pWindow);

	ContextMenuParam param;
	param.hWnd = m_pManager->GetPaintWindow();
	param.wParam = 2;
	CMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);

	m_pWindow->Init(static_cast<CMenuElementUI*>(this), _T(""), CDuiPoint(), NULL);
}

}