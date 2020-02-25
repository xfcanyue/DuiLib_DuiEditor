#include "StdAfx.h"
#include "KeyBoardSimpleUI.h"


IMPLEMENT_DUICONTROL(CKeyBoardSimpleUI)

CKeyBoardSimpleUI::CKeyBoardSimpleUI(void)
{
}


CKeyBoardSimpleUI::~CKeyBoardSimpleUI(void)
{
}

LPCTSTR CKeyBoardSimpleUI::GetClass() const
{
	return _T("KeyBoardSimple");
}

LPVOID CKeyBoardSimpleUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("KeyBoardSimple")) == 0 ) return static_cast<CKeyBoardSimpleUI*>(this);
	return __super::GetInterface(pstrName);
}

int CKeyBoardSimpleUI::CalcKeyBoardWidth()
{
	return GetKeyWidth()*10 + GetKeyPadding()*10 + GetKeyWidth()*2 + GetKeyWidth()/2 + GetKeyPadding()*2;	
}

int CKeyBoardSimpleUI::CalcKeyBoardHeight()
{
	return GetKeyHeight()*4 + GetKeyPadding()*3;
}

void CKeyBoardSimpleUI::DoInit()
{
	InitKeyBoard(4);
	InitKeyButton();
	SetCapsON(true);
	SetShiftON(true);
}

void CKeyBoardSimpleUI::InitKeyButton()
{
	CKeyButtonUI *pKeyButton = NULL;
	int nKeyWidth = GetKeyWidth();
	int nKeyHeight = GetKeyHeight();
	int nKeyPadding = GetKeyPadding();

	AddKey(0, '1',	'1',	VK_NUMPAD1);
	AddKey(0, '2',	'2',	VK_NUMPAD2);
	AddKey(0, '3',	'3',	VK_NUMPAD3);
	AddKey(0, '4',	'4',	VK_NUMPAD4);
	AddKey(0, '5',	'5',	VK_NUMPAD5);
	AddKey(0, '6',	'6',	VK_NUMPAD6);
	AddKey(0, '7',	'7',	VK_NUMPAD7);
	AddKey(0, '8',	'8',	VK_NUMPAD8);
	AddKey(0, '9',	'9',	VK_NUMPAD9);
	AddKey(0, '0',	'0',	VK_NUMPAD0);
	AddKey(0, VK_BACK, VK_BACK, VK_BACK, 0);

	AddLabel(1, nKeyWidth/2, nKeyHeight);
	AddKey(1, 'q',	'Q',	0x51);
	AddKey(1, 'w',	'W',	0x57);
	AddKey(1, 'e',	'E',	0x45);
	AddKey(1, 'r',	'R',	0x52);
	AddKey(1, 't',	'T',	0x54);
	AddKey(1, 'y',	'Y',	0x59);
	AddKey(1, 'u',	'U',	0x55);
	AddKey(1, 'i',	'I',	0x49);
	AddKey(1, 'o',	'O',	0x4f);
	AddKey(1, 'p',	'P',	0x50);
	pKeyButton = AddKey(1, VK_RETURN,	VK_RETURN, VK_RETURN, nKeyWidth*2+nKeyPadding, nKeyHeight);
	pKeyButton->SetName(GetEnterControlName());

	AddLabel(2, nKeyWidth, nKeyHeight);
	AddKey(2, 'a',	'A',	0x41);
	AddKey(2, 's',	'S',	0x53);
	AddKey(2, 'd',	'D',	0x44);
	AddKey(2, 'f',	'F',	0x46);
	AddKey(2, 'g',	'G',	0x47);
	AddKey(2, 'h',	'H',	0x48);
	AddKey(2, 'j',	'J',	0x4a);
	AddKey(2, 'k',	'K',	0x4b);
	AddKey(2, 'l',	'L',	0x4c);

	AddLabel(3, nKeyWidth*2+nKeyPadding, nKeyHeight);
	AddKey(3, 'z',	'Z',	0x5A);
	AddKey(3, 'x',	'X',	0x58);
	AddKey(3, 'c',	'C',	0x43);
	AddKey(3, 'v',	'V',	0x56);
	AddKey(3, 'b',	'B',	0x42);
	AddKey(3, 'n',	'N',	0x4e);
	AddKey(3, 'm',	'M',	0x4d);
}