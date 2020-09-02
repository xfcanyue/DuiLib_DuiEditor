// EditSearchCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "EditSearchCtrl.h"


// CEditSearchCtrl

IMPLEMENT_DYNAMIC(CEditSearchCtrl, CMFCEditBrowseCtrl)

CEditSearchCtrl::CEditSearchCtrl()
{

}

CEditSearchCtrl::~CEditSearchCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditSearchCtrl, CMFCEditBrowseCtrl)
END_MESSAGE_MAP()



// CEditSearchCtrl 消息处理程序
void CEditSearchCtrl::OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bHighlight)
{
	__super::OnDrawBrowseButton(pDC, rect, bIsButtonPressed, bHighlight);
}

void CEditSearchCtrl::SetImage()
{
	if (m_ImageBrowse.GetSafeHandle() != NULL)
	{
		m_ImageBrowse.DeleteImageList();
	}
	CBitmap bitmap;
	bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_DELETE));
	m_ImageBrowse.Create(16, 16, ILC_COLOR8|ILC_MASK, 0, 0);
	m_ImageBrowse.Add(&bitmap, RGB(255,0,255));
	bitmap.DeleteObject();
	m_bDefaultImage = FALSE;


}

void CEditSearchCtrl::OnBrowse()
{
	CWnd *pWnd = GetParent();
	if(pWnd)
		pWnd->SendMessage(WM_COMMAND, ID_PROPERTIES_FIND_BUTTON, 0);
}


