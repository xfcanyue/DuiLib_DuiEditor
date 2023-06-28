#include "StdAfx.h"
#include "UIDateTimeExWndGtk.h"

#ifdef DUILIB_GTK
namespace DuiLib
{

CDateTimeExWndGtk::CDateTimeExWndGtk() : m_pOwner(NULL), m_bInit(false), m_bDropOpen(false), m_uFormatStyle(UIDTS_DATE)
{
}

CDateTimeExWndGtk::~CDateTimeExWndGtk()
{
}

void CDateTimeExWndGtk::Init(CDateTimeExUI* pOwner, UINT uFormatStyle)
{
	 
}

RECT CDateTimeExWndGtk::CalPos()
{
	CDuiRect rcPos;
	
	return rcPos;
}

LPCTSTR CDateTimeExWndGtk::GetWindowClassName() const
{
	return _T("DateTimeExWnd");
}


void CDateTimeExWndGtk::OnFinalMessage(UIWND hWnd)
{
	
}

LRESULT CDateTimeExWndGtk::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	
	return lRes;
}

}
#endif // #ifdef DUILIB_GTK



