#include "StdAfx.h"
#include "UIPictureControl.h"

namespace DuiLib
{


static std::map<UINT, CControlUI *> g_MapTimerID_TO_CPictureControlUI;
static void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT idEvent,DWORD dwTime)  
{
	std::map<UINT, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureControlUI.find(idEvent);
	if(it != g_MapTimerID_TO_CPictureControlUI.end())
	{
		CPictureControlUI *pObject = (CPictureControlUI *)it->second;
		pObject->OnTimer(idEvent);
	}
}

struct CPictureControlUI::Imp
{
	CxImage img;
};

static bool LoadGifImageX(CxImage &img, STRINGorID bitmap, LPCTSTR type , DWORD mask)
{
	LPBYTE pData = NULL;
	DWORD dwSize = CRenderEngine::LoadImage2Memory(bitmap,type,pData);
	if(dwSize == 0U || !pData)
		return false;

	img.DestroyFrames();
	img.Destroy();

	img.SetRetreiveAllFrames(TRUE);
	if(!img.Decode(pData,dwSize,CXIMAGE_FORMAT_UNKNOWN))
	{
		delete[] pData; pData = NULL;
		return false;
	}

	delete[] pData; pData = NULL;
	return true;
}
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

void CPictureControlUI::PaintForeImage(HDC hDC)
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

	__super::PaintForeImage(hDC);
}

bool CPictureControlUI::Activate()
{
	if( !__super::Activate() ) return false;
	if( m_pManager != NULL )
	{
		m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
	}
	return true;
}

void CPictureControlUI::DoEvent(TEventUI& event)
{
	if( event.Type == UIEVENT_KEYDOWN )
	{
		if (IsKeyboardEnabled()) {
			if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
				Activate();
				return;
			}
		}
	}		

	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( ::PtInRect(&m_rcItem, event.ptMouse) ) Activate();				
	}
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
		g_MapTimerID_TO_CPictureControlUI[m_idEventTimer] = this;
	}
}

void CPictureControlUI::StopAnim()
{
	if(m_idEventTimer != 0)
	{
		::KillTimer(NULL, m_idEventTimer);
		std::map<UINT, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureControlUI.find(m_idEventTimer);
		if(it != g_MapTimerID_TO_CPictureControlUI.end())
			g_MapTimerID_TO_CPictureControlUI.erase(it);
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