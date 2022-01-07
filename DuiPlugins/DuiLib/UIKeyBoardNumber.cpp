#include "StdAfx.h"
#include "UIKeyBoardNumber.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CKeyBoardNumberUI)

CKeyBoardNumberUI::CKeyBoardNumberUI(void)
{
}


CKeyBoardNumberUI::~CKeyBoardNumberUI(void)
{
}

LPCTSTR CKeyBoardNumberUI::GetClass() const
{
	return _T("KeyBoardNumberUI");
}

LPVOID CKeyBoardNumberUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("KeyBoardNumber")) == 0 ) return static_cast<CKeyBoardNumberUI*>(this);
	return __super::GetInterface(pstrName);
}

int CKeyBoardNumberUI::CalcKeyBoardWidth()
{
	return GetKeyWidth()*3 + GetKeyPadding()*2;	
}

int CKeyBoardNumberUI::CalcKeyBoardHeight()
{
	return GetKeyHeight()*4 + GetKeyPadding()*3;
}

void CKeyBoardNumberUI::DoInit()
{
	InitKeyBoard(4);
	InitKeyButton();
}

void CKeyBoardNumberUI::InitKeyButton()
{
	CKeyButtonUI *pKeyButton = NULL;
	AddKey(3, '0', '0',	VK_NUMPAD0);
	AddKey(3, VK_BACK, VK_BACK, VK_BACK);
	pKeyButton = AddKey(3, VK_RETURN, VK_RETURN, VK_RETURN);
	pKeyButton->SetName(GetEnterControlName());
	AddKey(2, '1', '1',	VK_NUMPAD1);
	AddKey(2, '2', '2',	VK_NUMPAD2);
	AddKey(2, '3', '3',	VK_NUMPAD3);
	AddKey(1, '4', '4',	VK_NUMPAD4);
	AddKey(1, '5', '5',	VK_NUMPAD5);
	AddKey(1, '6', '6',	VK_NUMPAD6);
	AddKey(0, '7', '7',	VK_NUMPAD7);
	AddKey(0, '8', '8',	VK_NUMPAD8);
	AddKey(0, '9', '9',	VK_NUMPAD9);
}


CDuiString CKeyBoardNumberUI::GetKeyText(BYTE vkCode)
{
	CDuiString sText;
	switch (vkCode)
	{
	case  VK_BACK:
		sText = _T("¡û");
		break;
	case  VK_TAB: 
		sText = _T("Tab");
		break;
	case  VK_CAPITAL: 
		sText = _T("Caps");
		break;
	case  VK_RETURN: 
		{
			CDuiString temp = GetEnterControlText();
			if(!temp.IsEmpty())
				sText = temp;
			else
				sText = _T("En");
		}
		break;
	case  VK_SHIFT: 
		sText = _T("Shift");
		break;
	case  VK_CONTROL: 
		sText = _T("Ctrl");
		break;
	case  VK_MENU: 
		sText = _T("Alt");
		break;
	case  VK_ESCAPE: 
		sText = _T("Esc");
		break;
	case  VK_DELETE: 
		sText = _T("Del");
		break;
	case  VK_UP: 
		sText = _T("¡ü");
		break;
	case  VK_DOWN: 
		sText = _T("¡ý");
		break;
	case  VK_LEFT: 
		sText = _T("¡û");
		break;
	case  VK_RIGHT:
		sText = _T("¡ú");
		break;
	default:
		break;
	}

	return sText;
}

}