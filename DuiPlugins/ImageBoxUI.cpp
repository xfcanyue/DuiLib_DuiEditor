#include "StdAfx.h"
#include "ImageBoxUI.h"

IMPLEMENT_DUICONTROL(CImageBoxUI)
CImageBoxUI::CImageBoxUI(void)
	: m_uButtonState(0), m_nRetention(200), m_nCurrentFrame(0)
{
}

CImageBoxUI::~CImageBoxUI(void)
{
	ClearImageList();
}

LPCTSTR	CImageBoxUI::GetClass() const
{
	return _T("ImageBox");
}

LPVOID	CImageBoxUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("ImageBox")) == 0 ) return static_cast<CImageBoxUI*>(this);
	return __super::GetInterface(pstrName);
}

void CImageBoxUI::DoInit()
{
	SetTimer(99, GetRetention());
}

bool CImageBoxUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	DrawFrame(hDC);
	return __super::DoPaint(hDC, rcPaint, pStopControl);
}

void CImageBoxUI::DrawFrame(HDC hDC)
{
	CxImage* pImage = (CxImage *)m_arrImages[m_nCurrentFrame];
	pImage->Draw(hDC, m_rcItem);
}

void CImageBoxUI::DoEvent(TEventUI& event)
{
	if(event.Type == UIEVENT_TIMER)
	{
		if(event.wParam == 99)
		{
			if(!IsVisible()) return;
			m_nCurrentFrame++;
			if(m_nCurrentFrame >= m_arrImages.GetSize())
				m_nCurrentFrame = 0;
			Invalidate();
		}
	}

	if( event.Type == UIEVENT_KEYDOWN )
	{
		if (IsKeyboardEnabled()) {
			if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
				Activate();
				return;
			}
		}
	}		

	if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled() ) {
			m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
			Invalidate();
		}
		return;
	}	
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( (m_uButtonState & UISTATE_CAPTURED) != 0 ) {
			m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
			Invalidate();
			if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
		}
		return;
	}

	__super::DoEvent(event);
}

bool CImageBoxUI::Activate()
{
	if( !CControlUI::Activate() ) return false;
	if( m_pManager != NULL )
	{
		m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
	}
	return true;
}

void CImageBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("imagepath")) == 0)
	{
		m_strImagePath = pstrValue;
		LoadImageList();
	}
	else if (_tcscmp(pstrName, _T("retention")) == 0)
	{
		SetRetention(_ttoi(pstrValue));
	}
	else 
		__super::SetAttribute(pstrName,pstrValue);
}


void CImageBoxUI::LoadImageList()
{
	ClearImageList();
	if(m_strImagePath.IsEmpty()) return;

	CDuiString strPath = GetManager()->GetResourcePath() + m_strImagePath;
	if(strPath[strPath.GetLength()-1] != '\\') strPath += _T("\\");

	WIN32_FIND_DATA FindData;  
	HANDLE hFind;  
	int FileCount = 0;  
	hFind = FindFirstFile(strPath + _T("*.*"), &FindData);  
	if (hFind == INVALID_HANDLE_VALUE)  
	{    
		return;  
	}  

	while (::FindNextFile(hFind, &FindData))
	{
		if((_tcsicmp(FindData.cFileName, _T(".")) == 0) ||
			(_tcsicmp(FindData.cFileName, _T("..")) == 0) )
			continue;

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		CDuiString fileName = FindData.cFileName;
		int po = fileName.ReverseFind('.');
		if(po < 0)	
			continue;

		CDuiString ExtName = fileName.Right(fileName.GetLength() - po - 1);
		if((_tcsicmp(ExtName, _T("jpg")) == 0) ||
			(_tcsicmp(ExtName, _T("bmp")) == 0) ||
			(_tcsicmp(ExtName, _T("png")) == 0) )
		{
			CxImage *img = new CxImage;
			if(img->Load(strPath + fileName))
				m_arrImages.Add(img);
			else
				delete img;
		}
	}
}

void CImageBoxUI::ClearImageList()
{
	for(int i = 0; i < m_arrImages.GetSize(); i++)
	{
		CxImage* pImage = (CxImage *)m_arrImages[i];
		delete pImage;
	}
	m_arrImages.Empty();
}