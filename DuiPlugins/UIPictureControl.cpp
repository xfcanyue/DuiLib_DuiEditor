#include "StdAfx.h"
#include "UIPictureControl.h"

#include "../DuiLib/Utils/unzip.cpp"

namespace DuiLib
{

static CMapPtrToPtr g_MapTimerID_TO_CPictureControlUI;
static void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT idEvent,DWORD dwTime)  
{
	CPictureControlUI *pObject;
	if (g_MapTimerID_TO_CPictureControlUI.Lookup((void *) idEvent, (void*&)pObject))
	{
		pObject->OnTimer(idEvent);
	}
}

struct CPictureControlUI::Imp
{
	CxImage img;
};

IMPLEMENT_DUICONTROL(CPictureControlUI)
//////////////////////////////////////////////////////////////////////////
CPictureControlUI::CPictureControlUI(void):m_pImp(new CPictureControlUI::Imp())
{
	m_bIsAutoPlay = true;
	m_bIsAutoSize = false;

	m_imagetype = 0;

	m_idEventTimer = 0;		//动画定时器
	m_nDelay = 0;			//循环毫秒数
	m_nFrameCount = 0;		//gif图片总帧数
	m_nFramePosition = 0;	//当前放到第几帧
}


CPictureControlUI::~CPictureControlUI(void)
{
	StopAnim();
	delete m_pImp;
}

LPCTSTR	CPictureControlUI::GetClass() const
{
	return DUI_CTR_PICTURECONTROL;
}

LPVOID	CPictureControlUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_PICTURECONTROL) == 0 ) return static_cast<CPictureControlUI*>(this);
	return __super::GetInterface(pstrName);
}

void CPictureControlUI::DoInit()
{
	
}

bool CPictureControlUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	bool bPaint = __super::DoPaint(hDC, rcPaint, pStopControl);
	if(bPaint)
	{
		if(m_imagetype == CXIMAGE_FORMAT_GIF)
		{
			CxImage *pImage = m_pImp->img.GetFrame(m_nFramePosition);
			if(pImage)
				pImage->Draw(hDC, m_rcPaint);
		}
		else if(m_imagetype == CXIMAGE_FORMAT_ICO)
		{
			m_pImp->img.Draw(hDC, m_rcPaint);
		}
		else
		{
			m_pImp->img.Draw(hDC, m_rcPaint);
		}
	}
	return bPaint;
}

void CPictureControlUI::DoEvent(TEventUI& event)
{
	
}

void CPictureControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("fileimage")) == 0 ) 
	{
		LoadGifImageX(m_pImp->img, pstrValue, 0, 0);
		InitImage();
	}
	else if( _tcsicmp(pstrName, _T("framedelay")) == 0 ) 
	{
		SetFrameDelay(_ttoi(pstrValue));
	}
	else if( _tcsicmp(pstrName, _T("autoplay")) == 0 ) {
		SetAutoPlay(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else if( _tcsicmp(pstrName, _T("autosize")) == 0 ) {
		SetAutoSize(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else
		__super::SetAttribute(pstrName, pstrValue);
}

void CPictureControlUI::SetFrameDelay(int nDelay)
{
	if(nDelay <= 0) return;

	if(nDelay != m_nDelay)
	{
		m_nDelay = nDelay;
		StopAnim();

		if(IsAutoPlay())
			StartAnim();
	}
}

int  CPictureControlUI::GetFrameDelay()
{
	return m_nDelay;
}

void CPictureControlUI::SetAutoPlay(bool bIsAuto)	{ m_bIsAutoPlay = bIsAuto; }
bool CPictureControlUI::IsAutoPlay() const			{ return m_bIsAutoPlay; }
void CPictureControlUI::SetAutoSize(bool bIsAuto)	
{ 
	if(m_bIsAutoSize == bIsAuto) return;
	m_bIsAutoSize = bIsAuto; 
	if(m_bIsAutoSize)
	{
		if(m_pImp->img.IsValid())
		{
			SetFixedWidth(m_pImp->img.GetWidth());
			SetFixedHeight(m_pImp->img.GetHeight());
		}
	}
}
bool CPictureControlUI::IsAutoSize() const			{ return m_bIsAutoSize; }

void CPictureControlUI::LoadFile(LPCTSTR fileName)
{
	m_pImp->img.DestroyFrames();
	m_pImp->img.Destroy();
	m_pImp->img.SetRetreiveAllFrames(true);
	m_pImp->img.Load(fileName);

	InitImage();
	Invalidate();
}

void CPictureControlUI::SetCxImage(const void *pCxImage)
{
	CxImage *pImage = (CxImage *)pCxImage;
	m_pImp->img.DestroyFrames();
	m_pImp->img.Destroy();
	m_pImp->img.SetRetreiveAllFrames(true);
	m_pImp->img.Transfer(*pImage);

	InitImage();
	Invalidate();
}

void CPictureControlUI::InitImage()
{
	m_imagetype = m_pImp->img.GetType();

	if(IsAutoSize())
	{
		SetFixedWidth(m_pImp->img.GetWidth());
		SetFixedHeight(m_pImp->img.GetHeight());
	}

	if(m_imagetype == CXIMAGE_FORMAT_GIF)
	{
		m_nFramePosition = 0;
		m_nFrameCount = m_pImp->img.GetNumFrames();
		if(m_nDelay <= 0)
			m_nDelay = m_pImp->img.GetFrameDelay();

		StopAnim();
		if(IsAutoPlay())
			StartAnim();
	}
	else if(m_imagetype == CXIMAGE_FORMAT_ICO)
	{
		
	}
}

void CPictureControlUI::StartAnim()
{
	if(m_idEventTimer == 0)
	{
		m_idEventTimer = ::SetTimer(NULL, NULL, m_nDelay, TimerProc);
		g_MapTimerID_TO_CPictureControlUI.SetAt((void *) m_idEventTimer, this);
	}
}

void CPictureControlUI::StopAnim()
{
	if(m_idEventTimer != 0)
	{
		::KillTimer(NULL, m_idEventTimer);
		g_MapTimerID_TO_CPictureControlUI.RemoveKey((void *) m_idEventTimer);
		m_idEventTimer = 0;
	}
}

void CPictureControlUI::OnTimer(UINT_PTR idEvent)
{
	if(m_idEventTimer == idEvent)
	{
		++m_nFramePosition;
		if(m_nFramePosition >= m_nFrameCount)
			m_nFramePosition = 0;
		if(!IsVisible())return;
		Invalidate();
	}
}

}