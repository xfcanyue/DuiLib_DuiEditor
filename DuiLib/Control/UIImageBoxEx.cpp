#include "stdafx.h"
#include "UIImageBoxEx.h"

#include "mmsystem.h"
#pragma comment( lib,"winmm.lib" )

namespace DuiLib
{
	static std::map<UINT_PTR, CControlUI *> g_MapTimerID_TO_CImageBoxExUI;
	//static CMapPtrToPtr g_MapTimerID_TO_CImageBoxExUI;

	IMPLEMENT_DUICONTROL(CImageBoxExUI)
///////////////////////////////////////////////////////////////////////////////

CImageBoxExUI::CImageBoxExUI(void) 
	: m_iCurAnimType(kNone)
	, m_iDuration(2000)
	, m_iRetention(2000)
	, m_iIndex(0)
	, m_pOffScreenImage(NULL)
	, m_dwAnimStartTick(0)
	, m_dwDelayStartTick(0)
	, m_bIsDelayed(FALSE)
	, m_bIsAnimating(FALSE)
	, m_bPlay(FALSE)
	, m_nTimerEscape(200)
	, m_idEventTimer(0)
{

}


CImageBoxExUI::~CImageBoxExUI(void)
{	
	if(m_idEventTimer != 0)
	{
		::KillTimer(NULL, m_idEventTimer);
		std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CImageBoxExUI.find(m_idEventTimer);
		if(it != g_MapTimerID_TO_CImageBoxExUI.end())
			g_MapTimerID_TO_CImageBoxExUI.erase(it);
		m_idEventTimer = 0;
	}

	for(int i = 0; i < GetSize(); i++)
	{
		CImage* pImage = GetImageAt(i);
		pImage->Destroy();
		delete pImage;
	}

	if(m_pOffScreenImage != NULL)
	{
		m_pOffScreenImage->Destroy();
		delete m_pOffScreenImage;
	}
}

LPCTSTR CImageBoxExUI::GetClass() const
{
	return DUI_CTR_IMAGEBOXEX;
}

LPVOID CImageBoxExUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_IMAGEBOXEX) == 0 ) return static_cast<CImageBoxExUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CImageBoxExUI::DoInit()
{
	BOOL bCreate = FALSE;
	if(m_pOffScreenImage == NULL)
	{
		m_pOffScreenImage = new CImage;
		CRect rcClient = GetPos();
		bCreate = m_pOffScreenImage->Create(100, 100, 24, 0);
	}

	//初始动画
	SetAnimationType(CImageBoxExUI::kAnimationCoverLeft);

	// 动画过渡时间(ms)
	//SetDuration(2000);
	// 每张图片停留时间
	//SetRetention(5000);

	//LoadImageList();
	Play();
}

bool CImageBoxExUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	::SetStretchBltMode(hDC, COLORONCOLOR);
	//::SetStretchBltMode(hDC, STRETCH_HALFTONE);
	CRect rcClient = GetPos();
	m_pOffScreenImage->Draw(hDC, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height());

	return true;
}

void CImageBoxExUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_TIMER)
	{
		OnTimer((UINT_PTR)event.wParam);
	}
	CControlUI::DoEvent(event);
}

void CImageBoxExUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	m_pOffScreenImage->Destroy();
	m_pOffScreenImage->Create(rc.right-rc.left, rc.bottom-rc.top, 24, 0);
}

void CImageBoxExUI::Move(SIZE szOffset, bool bNeedInvalidate)
{
	CControlUI::Move(szOffset, bNeedInvalidate);
}

void CImageBoxExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("imagepath")) == 0)
	{
		m_strImagePath = pstrValue;
		LoadImageList();
	}
	else if (_tcscmp(pstrName, _T("refresh")) == 0)
	{
		m_nTimerEscape = _ttoi(pstrValue);
	}
	else if (_tcscmp(pstrName, _T("duration")) == 0)
	{
		SetDuration(_ttoi(pstrValue));
	}
	else if (_tcscmp(pstrName, _T("retention")) == 0)
	{
		SetRetention(_ttoi(pstrValue));
	}
	else 
		CControlUI::SetAttribute(pstrName,pstrValue);
}

void CImageBoxExUI::LoadImageList()
{
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
			CImage *img = new CImage;
			HRESULT hr = img->Load(strPath + fileName);
			if(hr == S_OK)
				m_arrImages.push_back(img);
			else
				delete img;
		}
	}
}

CImage* CImageBoxExUI::GetImageAt(int iIndex) const
{
	ASSERT(iIndex < (int)m_arrImages.size());
	//CImage &img = m_arrImages[iIndex];
	return (CImage*)(m_arrImages[iIndex]);
}

int	CImageBoxExUI::GetSize() const
{
	return m_arrImages.size();
}

int CImageBoxExUI::GetNextFrameIndex() const
{
	return (m_iIndex < GetSize() - 1) ? m_iIndex + 1 : 0;

}

void CImageBoxExUI::SetDuration(int iTime)
{
	if(iTime < 5)
	{
		m_iDuration = 5;
		return;
	}
	m_iDuration = iTime;
}

void CImageBoxExUI::SetRetention(int iTime)
{
	if(iTime < 0)
	{
		iTime = 0;
		return;
	}
	m_iRetention = iTime;
}

void CImageBoxExUI::SetAnimationType(AnimationType iType)
{
	m_iCurAnimType = iType;
}

static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR idEvent, DWORD dwTime)  
{
	std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CImageBoxExUI.find(idEvent);
	if(it != g_MapTimerID_TO_CImageBoxExUI.end())
	{
		CImageBoxExUI *pObject = (CImageBoxExUI *)it->second;
		pObject->OnTimer(idEvent);
	}
}

void CImageBoxExUI::Play()
{
	m_bPlay = TRUE;
	//m_pManager->SetTimer(this, DEFAULT_TIMERID, m_nTimerEscape);
	m_idEventTimer = ::SetTimer(NULL, NULL, m_nTimerEscape, TimerProc);
	g_MapTimerID_TO_CImageBoxExUI[m_idEventTimer] = this;
}

void CImageBoxExUI::Stop()
{
	m_bPlay = FALSE;
	//m_pManager->KillTimer(this, DEFAULT_TIMERID);
	::KillTimer(NULL, m_idEventTimer);
	std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CImageBoxExUI.find(m_idEventTimer);
	if(it != g_MapTimerID_TO_CImageBoxExUI.end())
		g_MapTimerID_TO_CImageBoxExUI.erase(it);
	m_idEventTimer = 0;
}

void CImageBoxExUI::OnTimer(UINT_PTR idEvent)
{
	if(m_idEventTimer != idEvent)
		return;

	CRect rcClient = GetPos();
	if(rcClient.IsRectEmpty())
		return;

	if(!IsVisible()) return;
	
	// Frame delay toggle.
	if(m_bIsDelayed)
	{
		if(timeGetTime() - m_dwDelayStartTick > (DWORD)m_iRetention)
			m_bIsDelayed = FALSE;
		return;
	}

	switch(m_iCurAnimType)
	{
	case kAnimationFade:
		AnimationRender_Alpha(timeGetTime());
		break;
	case kAnimationSlideLeft:
	case kAnimationSlideRight:
	case kAnimationSlideTop:
	case kAnimationSlideBottom:
		AnimationRender_Slide(timeGetTime());
		break;
	case kAnimationBlindLeft:
	case kAnimationBlindRight:
	case kAnimationBlindTop:
	case kAnimationBlindBottom:
		AnimationRender_Blind(timeGetTime());
		break;
	case kAnimationCoverLeft:
	case kAnimationCoverRight:
	case kAnimationCoverTop:
	case kAnimationCoverBottom:
		AnimationRender_Cover(timeGetTime());
		break;
	}

	if(!m_bIsAnimating)
	{
		srand(GetTickCount());
		while (TRUE)
		{
			int type = rand()%13;
			if(type > 0)
			{
				SetAnimationType(CImageBoxExUI::AnimationType(type));
				break;
			}
		}
	}
}

void CImageBoxExUI::AnimationRender_Alpha(DWORD dwTick)
{
	if(GetSize() == 0)
		return;

	if (!m_bIsAnimating)
	{
		m_bIsAnimating = TRUE;
		m_dwAnimStartTick = dwTick;
	}
	CImage* pCurImage = GetImageAt(m_iIndex);
	CImage* pNextImage = GetImageAt(GetNextFrameIndex());
	CRect rcClient = GetPos();
	// DEST
	HDC hOffScreenDC = m_pOffScreenImage->GetDC();
	::SetStretchBltMode(hOffScreenDC, COLORONCOLOR);
	pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
	// SRC
	double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
	int iAlpha = GetLinearInterpolation(0, 255, delta);
	pCurImage->AlphaBlend(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height(), 
		0, 0, pCurImage->GetWidth(), pCurImage->GetHeight(), iAlpha);

	// Current frame animation ends.
	if(iAlpha == 0)
	{
		if(++m_iIndex >= GetSize())
			m_iIndex = 0;
		m_dwDelayStartTick = dwTick;
		m_bIsAnimating = FALSE;
		m_bIsDelayed = TRUE;
	}
	m_pOffScreenImage->ReleaseDC();
	Invalidate();
}	

void CImageBoxExUI::AnimationRender_Slide(DWORD dwTick)
{
	if(GetSize() == 0)
		return;

	if (!m_bIsAnimating)
	{
		m_bIsAnimating = TRUE;
		m_dwAnimStartTick = timeGetTime();
	}
	CImage* pCurImage = GetImageAt(m_iIndex);
	CImage* pNextImage = GetImageAt(GetNextFrameIndex());
	CRect rcClient = GetPos();

	HDC hOffScreenDC = m_pOffScreenImage->GetDC();
	::SetStretchBltMode(hOffScreenDC, COLORONCOLOR);
	int iMode = ::SaveDC(hOffScreenDC);
	double delta = (double)(min(dwTick, m_dwAnimStartTick + m_iDuration) - m_dwAnimStartTick) / (double)m_iDuration;
	POINT ptOldViewportOrg;
	// Slide left.
	if(m_iCurAnimType == kAnimationSlideLeft)
	{
		int iOffsetX = GetLinearInterpolation(0, -rcClient.Width(), delta);
		::SetViewportOrgEx(hOffScreenDC, iOffsetX, 0, &ptOldViewportOrg);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, rcClient.Width(), 0, rcClient.Width(), rcClient.Height());
		if(iOffsetX == -rcClient.Width())
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationSlideRight)
	{
		int iOffsetX = GetLinearInterpolation(0, rcClient.Width(), delta);
		::SetViewportOrgEx(hOffScreenDC, iOffsetX, 0, &ptOldViewportOrg);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, -rcClient.Width(), 0, rcClient.Width(), rcClient.Height());
		if(iOffsetX == rcClient.Width())
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationSlideTop)
	{
		int iOffsetY = GetLinearInterpolation(0, -rcClient.Height(), delta);
		::SetViewportOrgEx(hOffScreenDC, 0, iOffsetY, &ptOldViewportOrg);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, 0, rcClient.Height(), rcClient.Width(), rcClient.Height());
		if(iOffsetY == -rcClient.Height())
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationSlideBottom)
	{
		int iOffsetY = GetLinearInterpolation(0, rcClient.Height(), delta);
		::SetViewportOrgEx(hOffScreenDC, 0, iOffsetY, &ptOldViewportOrg);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, 0, -rcClient.Height(), rcClient.Width(), rcClient.Height());
		if(iOffsetY == rcClient.Height())
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	::RestoreDC(hOffScreenDC, iMode);
	m_pOffScreenImage->ReleaseDC();
	Invalidate();
}

void CImageBoxExUI::AnimationRender_Blind(DWORD dwTick)
{
	if(GetSize() == 0)
		return;

	if (!m_bIsAnimating)
	{
		m_bIsAnimating = TRUE;
		m_dwAnimStartTick = timeGetTime();
	}
	CImage* pCurImage = GetImageAt(m_iIndex);
	CImage* pNextImage = GetImageAt(GetNextFrameIndex());
	CRect rcClient = GetPos();

	HDC hOffScreenDC = m_pOffScreenImage->GetDC();
	::SetStretchBltMode(hOffScreenDC, COLORONCOLOR);
	pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());

	// Resize image to fit the size of client.
	CImage imageFit;
	imageFit.Create(rcClient.Width(), rcClient.Height(), 24, 0);
	HDC hImageTempDC = imageFit.GetDC();
	::SetStretchBltMode(hImageTempDC, COLORONCOLOR);
	pCurImage->Draw(hImageTempDC, 0, 0, rcClient.Width(), rcClient.Height());
	imageFit.ReleaseDC();

	int iPiece = rcClient.Width() / BLIND_PIECE;
	if(m_iCurAnimType == kAnimationBlindLeft)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iPieceWidth = GetLinearInterpolation(0, iPiece, delta);
		for (int i = 0; i < iPiece; i++)
		{
			imageFit.BitBlt(hOffScreenDC, i * iPiece, 0, iPieceWidth+1, rcClient.Height(),  i * iPiece, 0, SRCCOPY);
		}
		if(iPieceWidth == 0)
		{
			// Erase the gaps.
			pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationBlindRight)
	{
		double delta = (double)(min(dwTick, m_dwAnimStartTick + m_iDuration) - m_dwAnimStartTick) / (double)m_iDuration;
		int iPieceWidth = GetLinearInterpolation(0, iPiece, delta);
		for (int i = 0; i < iPiece; i++)
		{
			imageFit.BitBlt(hOffScreenDC, i * iPiece + iPieceWidth, 0, iPiece - iPieceWidth + 1, rcClient.Height(),  i * iPiece + iPieceWidth, 0, SRCCOPY);
		}
		if(iPieceWidth == iPiece)
		{
			// Erase the gaps.
			pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	if(m_iCurAnimType == kAnimationBlindTop)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iPieceWidth = GetLinearInterpolation(0, iPiece, delta);
		for (int i = 0; i < iPiece; i++)
		{
			imageFit.BitBlt(hOffScreenDC, 0, i * iPiece, rcClient.Width(), iPieceWidth + 1,  0, i * iPiece, SRCCOPY);
		}
		if(iPieceWidth == 0)
		{
			// Erase the gaps.
			pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationBlindBottom)
	{
		double delta = (double)(min(dwTick, m_dwAnimStartTick + m_iDuration) - m_dwAnimStartTick) / (double)m_iDuration;
		int iPieceWidth = GetLinearInterpolation(0, iPiece, delta);
		for (int i = 0; i < iPiece; i++)
		{
			imageFit.BitBlt(hOffScreenDC, 0, i * iPiece + iPieceWidth, rcClient.Width(), iPiece - iPieceWidth + 1,  0, i * iPiece + iPieceWidth, SRCCOPY);
		}
		if(iPieceWidth == iPiece)
		{
			// Erase the gaps.
			pNextImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	imageFit.Destroy();
	m_pOffScreenImage->ReleaseDC();
	Invalidate();
}

void CImageBoxExUI::AnimationRender_Cover(DWORD dwTick)
{
	if(GetSize() == 0)
		return;

	if (!m_bIsAnimating)
	{
		m_bIsAnimating = TRUE;
		m_dwAnimStartTick = timeGetTime();
	}
	CImage* pCurImage = GetImageAt(m_iIndex);
	CImage* pNextImage = GetImageAt(GetNextFrameIndex());
	CRect rcClient = GetPos();

	HDC hOffScreenDC = m_pOffScreenImage->GetDC();
	::SetStretchBltMode(hOffScreenDC, COLORONCOLOR);
	// Cover left.
	if(m_iCurAnimType == kAnimationCoverLeft)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iX = GetLinearInterpolation(0, rcClient.Width(), delta);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, iX, 0, rcClient.Width(), rcClient.Height());
		if(iX == 0)
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationCoverRight)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iX = GetLinearInterpolation(0, -rcClient.Width(), delta);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, iX, 0, rcClient.Width(), rcClient.Height());
		if(iX == 0)
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationCoverTop)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iY = GetLinearInterpolation(0, rcClient.Height(), delta);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, 0, iY, rcClient.Width(), rcClient.Height());
		if(iY == 0)
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	else if(m_iCurAnimType == kAnimationCoverBottom)
	{
		double delta = (double)(m_dwAnimStartTick + m_iDuration - min(dwTick, m_dwAnimStartTick + m_iDuration)) / (double)m_iDuration;
		int iY = GetLinearInterpolation(0, -rcClient.Height(), delta);
		pCurImage->Draw(hOffScreenDC, 0, 0, rcClient.Width(), rcClient.Height());
		pNextImage->Draw(hOffScreenDC, 0, iY, rcClient.Width(), rcClient.Height());
		if(iY == 0)
		{
			if(++m_iIndex >= GetSize())
				m_iIndex = 0;
			m_dwDelayStartTick = dwTick;
			m_bIsAnimating = FALSE;
			m_bIsDelayed = TRUE;
		}
	}
	m_pOffScreenImage->ReleaseDC();
	Invalidate();
}

int CImageBoxExUI::GetLinearInterpolation(int x1, int x2, double delta)
{
	return (int)((double)x1 + (double)(x2 - x1) * delta);
}

} //namsespace