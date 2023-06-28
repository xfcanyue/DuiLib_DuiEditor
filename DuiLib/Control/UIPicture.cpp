#include "StdAfx.h"
#include "UIPicture.h"

namespace DuiLib
{

// static std::map<UINT_PTR, CControlUI *> g_MapTimerID_TO_CPictureUI;
// static void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR idEvent, DWORD dwTime)  
// {
// 	std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureUI.find(idEvent);
// 	if(it != g_MapTimerID_TO_CPictureUI.end())
// 	{
// 		CPictureUI *pObject = (CPictureUI *)it->second;
// 		pObject->OnTimer(idEvent);
// 	}
// }

IMPLEMENT_DUICONTROL(CPictureUI)
//////////////////////////////////////////////////////////////////////////
CPictureUI::CPictureUI(void)
{
	m_bIsAutoPlay = true;
	m_bIsAutoSize = false;

	m_idEventTimer = 0;		//动画定时器
	m_nDelay = 0;			//循环毫秒数
	m_nFramePosition = 0;	//当前放到第几帧

	m_bTrackRect = false;
	m_dwTrackColor = 0xFF000000;
}


CPictureUI::~CPictureUI(void)
{
	StopAnim();
	RemoveAllImages();
}

LPCTSTR CPictureUI::GetClass() const
{
	return _T("PictureUI");
}

LPVOID	CPictureUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_PICTURE) == 0 ) return static_cast<CPictureUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

void CPictureUI::DoInit()
{
	
}

bool CPictureUI::Activate()
{
	if( !CContainerUI::Activate() ) return false;
	if( m_pManager != NULL )
	{
		m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
	}
	return true;
}

void CPictureUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CContainerUI::DoEvent(event);
		return;
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

	if( event.Type == UIEVENT_BUTTONDOWN )
	{
		if(m_rcItem.PtInRect(event.ptMouse))
		{
			SetCaptureState(true);
			if(IsEnableTrackRect())
			{
				m_rcTracker.left = event.ptMouse.x;
				m_rcTracker.top = event.ptMouse.y;
				m_rcTracker.right = event.ptMouse.x;
				m_rcTracker.bottom = event.ptMouse.y;
				Invalidate();
			}
		}
	}

	if( event.Type == UIEVENT_MOUSEMOVE )
	{
		if( IsCaptureState() ) 
		{
			if(m_rcItem.PtInRect(event.ptMouse) && !m_rcTracker.IsNull())
			{
				m_rcTracker.right = event.ptMouse.x;
				m_rcTracker.bottom = event.ptMouse.y;
				Invalidate();
			}
		}
		return;
	}

	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( IsCaptureState() ) 
		{
			SetCaptureState(false);
			if( m_rcItem.PtInRect(event.ptMouse) ) Activate();		
		}
	}

	if( event.Type == UIEVENT_TIMER )
		OnTimer( (UINT_PTR)event.wParam );

	CContainerUI::DoEvent(event);
}

void CPictureUI::PaintBkImage(UIRender *pRender)
{
	CContainerUI::PaintBkImage(pRender);

	if(m_nFramePosition >= m_frames.GetSize())
		m_nFramePosition = 0;

	UIImage *pImageInfo = (UIImage *)m_frames.GetAt(m_nFramePosition);
	if(!pImageInfo) return;
	TDrawInfo defaultDrawInfo;
	TDrawInfo *pDrawInfo = &defaultDrawInfo;
	std::map<UIImage *, TDrawInfo *>::iterator it = m_map.find(pImageInfo);
	if(it != m_map.end())
	{
		pDrawInfo = it->second;
	}

	RECT rcItem = m_rcItem;
	RECT rcDest = m_rcItem;
	if( pDrawInfo->rcDest.left != 0 || pDrawInfo->rcDest.top != 0 ||
		pDrawInfo->rcDest.right != 0 || pDrawInfo->rcDest.bottom != 0 ) {
			rcDest.left = rcItem.left + pDrawInfo->rcDest.left;
			rcDest.top = rcItem.top + pDrawInfo->rcDest.top;
			rcDest.right = rcItem.left + pDrawInfo->rcDest.right;
			if( rcDest.right > rcItem.right ) rcDest.right = rcItem.right;
			rcDest.bottom = rcItem.top + pDrawInfo->rcDest.bottom;
			if( rcDest.bottom > rcItem.bottom ) rcDest.bottom = rcItem.bottom;
	}

	RECT rcSource = pDrawInfo->rcSource;
	if( rcSource.left == 0 && rcSource.right == 0 && rcSource.top == 0 && rcSource.bottom == 0 ) {
		rcSource.right = pImageInfo->nWidth;
		rcSource.bottom = pImageInfo->nHeight;
	}
	if (rcSource.right > pImageInfo->nWidth) rcSource.right = pImageInfo->nWidth;
	if (rcSource.bottom > pImageInfo->nHeight) rcSource.bottom = pImageInfo->nHeight;

	RECT rcCorner = {0};
	pRender->DrawBitmap(pImageInfo->bitmap, rcDest, m_rcPaint, rcSource, rcCorner, GetManager()->IsLayered() ? true : pImageInfo->bAlpha, pDrawInfo->uFade, pDrawInfo->bHole, pDrawInfo->bTiledX, pDrawInfo->bTiledY);

	if(IsAutoPlay() && m_idEventTimer == 0 && m_frames.GetSize() > 1)
	{
		StartAnim();
	}

	if(m_rcTracker.IsNull()) return;
	if(m_rcTracker.GetWidth()==0 || m_rcTracker.GetHeight() == 0) return;

	CDuiRect rcRect = m_rcTracker;
	rcRect.Normalize();
	pRender->DrawRect(rcRect, 1, GetAdjustColor(GetTrackColor()), PS_SOLID);

	//绘制蒙版
	CDuiRect rcTop(m_rcPaint.left, m_rcPaint.top, m_rcPaint.right, rcRect.top);
	pRender->DrawColor(rcTop, CDuiSize(0,0), GetAdjustColor(0x77FFFFFF));

	CDuiRect rcLeft(m_rcPaint.left, rcRect.top, rcRect.left, rcRect.bottom);
	pRender->DrawColor(rcLeft, CDuiSize(0,0), GetAdjustColor(0x77FFFFFF));

	CDuiRect rcRight(rcRect.right, rcRect.top, m_rcPaint.right, rcRect.bottom);
	pRender->DrawColor(rcRight, CDuiSize(0,0), GetAdjustColor(0x77FFFFFF));

	CDuiRect rcBottom(m_rcPaint.left, rcRect.bottom, m_rcPaint.right, m_rcPaint.bottom);
	pRender->DrawColor(rcBottom, CDuiSize(0,0), GetAdjustColor(0x77FFFFFF));
}

void CPictureUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("fileimage")) == 0 ) 
	{
		LoadFile(pstrValue);
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
	else if( _tcsicmp(pstrName, _T("trackrect")) == 0 ) {
		EnableTrackRect(_tcsicmp(pstrValue, _T("true")) == 0);
	}
	else
		CContainerUI::SetAttribute(pstrName, pstrValue);
}

#ifdef DUILIB_WIN32

HBITMAP CPictureUI::GetHBitmap()
{
	if(m_nFramePosition >= m_frames.GetSize())
		m_nFramePosition = 0;
	UIImage *pImageInfo = (UIImage *)m_frames.GetAt(m_nFramePosition);
	if(!pImageInfo) return NULL;
	return pImageInfo->bitmap->GetHBITMAP();
}

bool CPictureUI::LoadHBitmap(HBITMAP hBitmap)
{
	RemoveAllImages();
	return __SetHBitmap(hBitmap);
}

#endif //#ifdef DUILIB_WIN32

bool CPictureUI::LoadImageFromMemory(LPBYTE pData, DWORD dwSize)
{
	RemoveAllImages();
	return __LoadImageFromMemory(pData, dwSize);
}

bool CPictureUI::LoadFile(LPCTSTR pstrImage)
{
	RemoveAllImages();
	return __LoadFile(pstrImage);
}

void CPictureUI::RemoveAllImages()
{
	StopAnim();

	m_map.clear();

	for (int i=0; i<m_frames.GetSize(); i++)
	{
		UIImage *bitmap = (UIImage *)m_frames.GetAt(i);
		bitmap->Release();
	}
	m_frames.Empty();

	for (int i=0; i<m_arrDrawInfo.GetSize(); i++)
	{
		TDrawInfo *pDrawInfo = (TDrawInfo *)m_arrDrawInfo.GetAt(i);
		delete pDrawInfo;
	}
	m_arrDrawInfo.Empty();
}

void CPictureUI::SetFrameDelay(int nDelay)
{
	if(nDelay != m_nDelay)
	{
		m_nDelay = nDelay;
		StopAnim();

		if(IsAutoPlay())
			StartAnim();
	}
}

int  CPictureUI::GetFrameDelay()
{
	return m_nDelay;
}

void CPictureUI::SetAutoPlay(bool bIsAuto)	{ m_bIsAutoPlay = bIsAuto; }
bool CPictureUI::IsAutoPlay() const			{ return m_bIsAutoPlay; }
void CPictureUI::SetAutoSize(bool bIsAuto)	
{
	m_bIsAutoSize = bIsAuto; 
	if(m_bIsAutoSize)
	{
		SIZE sz = {0};
		for (int i=0; i<m_frames.GetSize(); i++)
		{
			UIImage *pImageInfo = (UIImage *)m_frames.GetAt(i);
			if(pImageInfo->nWidth > sz.cx)	sz.cx = pImageInfo->nWidth;
			if(pImageInfo->nHeight > sz.cy)	sz.cy = pImageInfo->nHeight;
		}
		SetFixedWidth(sz.cx);
		SetFixedHeight(sz.cy);
	}
}
bool CPictureUI::IsAutoSize() const			{ return m_bIsAutoSize; }

CDuiRect CPictureUI::GetTrackRect()
{
	RECT rcPos = GetPos();
	RECT rc;
	rc.left = m_rcTracker.left - rcPos.left;
	rc.top = m_rcTracker.top - rcPos.top;
	rc.right = rc.left + m_rcTracker.GetWidth();
	rc.bottom = rc.top + m_rcTracker.GetHeight();
	return rc;
}

void CPictureUI::StartAnim()
{
	if(m_idEventTimer == 0 && m_frames.GetSize() > 1)
	{
		int nDelay = m_nDelay;
		if(nDelay <= 0) {
			if(m_frames.GetSize() > 0) {
				UIImage *pImageInfo = (UIImage *)m_frames.GetAt(0);
				nDelay = pImageInfo->delay;
			}
		}
		if(nDelay > 0)
		{
// 			m_idEventTimer = ::SetTimer(NULL, NULL, m_nDelay, TimerProc);		
// 			g_MapTimerID_TO_CPictureUI[m_idEventTimer] = this;
			m_idEventTimer = 100;
			GetManager()->SetTimer(this, m_idEventTimer, m_nDelay);
		}
	}
}

void CPictureUI::StopAnim()
{
	if(m_idEventTimer != 0)
	{
// 		::KillTimer(NULL, m_idEventTimer);
// 		std::map<UINT_PTR, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureUI.find(m_idEventTimer);
// 		if(it != g_MapTimerID_TO_CPictureUI.end())
// 			g_MapTimerID_TO_CPictureUI.erase(it);
		GetManager()->KillTimer(this, m_idEventTimer);
		m_idEventTimer = 0;
	}
}

void CPictureUI::OnTimer(UINT_PTR idEvent)
{
	if(m_idEventTimer == idEvent)
	{
		++m_nFramePosition;
		if(m_nFramePosition >= m_frames.GetSize())
			m_nFramePosition = 0;
		if(!IsVisible())return;
		Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////

#ifdef DUILIB_WIN32
bool CPictureUI::__SetHBitmap(HBITMAP hBitmap)
{
//	BITMAP bm;
//	::GetObject(hBitmap, sizeof(bm), &bm);

	UIImage* data = UIGlobal::CreateImage();
	data->CreateImage(hBitmap, false);

	if(!m_frames.Add(data))
	{
		data->Release(); 
		return false;
	}
	SetAutoSize(IsAutoSize());
	return true;
}
#endif

bool CPictureUI::__LoadImageFromMemory(LPBYTE pData, DWORD dwSize)
{
	//尝试解析GIF
	CStdRefPtr<UIImage> gifLoader = UIGlobal::CreateImage();
	CStdPtrArray arrImageInfo;
	if(gifLoader->LoadGifImageFromMemory(pData, dwSize, arrImageInfo))
	{
		for (int i=0; i<arrImageInfo.GetSize(); i++)
		{
			UIImage *pImageInfo = (UIImage *)arrImageInfo.GetAt(i);
			if(!m_frames.Add(pImageInfo))
			{
				pImageInfo->Release(); 
				return false;
			}
		}
		return true;
	}

	UIImage *pImageInfo = UIGlobal::CreateImage();
	pImageInfo->LoadImageFromMemory(pData, dwSize, 0, 0, 0,0, GetManager());
	if(!pImageInfo) return false;
	if(!m_frames.Add(pImageInfo))
	{
		pImageInfo->Release(); 
		return false;
	}
	SetAutoSize(IsAutoSize());
	return true;
}

bool CPictureUI::__LoadFile(LPCTSTR pstrImage)
{
	bool bRet = false;
	CUIFile file;
	UIImage *pImageInfo = UIGlobal::CreateImage();

	TDrawInfo *pDrawInfo = new TDrawInfo;
	pDrawInfo->Parse(pstrImage, NULL, GetManager());

	do 
	{
		//文件载入内存
		if(!file.LoadFile(pDrawInfo->sImageName.GetData()))
			break;

		//尝试解析GIF
		CStdRefPtr<UIImage> gifLoader = MakeRefPtr<UIImage>(UIGlobal::CreateImage());
		CStdPtrArray arrImageInfo;
		if(gifLoader->LoadGifImageFromMemory(file.GetData(), file.GetSize(), arrImageInfo))
		{
			for (int i=0; i<arrImageInfo.GetSize(); i++)
			{
				UIImage *pImageInfo2 = (UIImage *)arrImageInfo.GetAt(i);
				m_frames.Add(pImageInfo2);
				m_map[pImageInfo2] = pDrawInfo;
			}
			m_arrDrawInfo.Add(pDrawInfo);
			pDrawInfo = NULL;
			bRet = true;
			break;
		}

		//尝试其他格式的图像
		if(!pImageInfo->LoadImageFromMemory(file.GetData(), file.GetSize(), pDrawInfo->dwMask, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, GetManager()))
		{
			break;
		}

		if(m_frames.Add(pImageInfo) && m_arrDrawInfo.Add(pDrawInfo))
		{
			m_map[pImageInfo] = pDrawInfo;
			pImageInfo = NULL;
			pDrawInfo = NULL;
			bRet = true;
			break;
		}

	} while (false);

	if(pImageInfo)	{ pImageInfo->Release(); }
	if(pDrawInfo)	delete pDrawInfo;
	SetAutoSize(IsAutoSize());
	return bRet;
}

}