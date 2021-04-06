#include "StdAfx.h"
#include "UIPicture.h"

namespace DuiLib
{


static std::map<UINT, CControlUI *> g_MapTimerID_TO_CPictureUI;
static void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT idEvent,DWORD dwTime)  
{
	std::map<UINT, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureUI.find(idEvent);
	if(it != g_MapTimerID_TO_CPictureUI.end())
	{
		CPictureUI *pObject = (CPictureUI *)it->second;
		pObject->OnTimer(idEvent);
	}
}

IMPLEMENT_DUICONTROL(CPictureUI)
//////////////////////////////////////////////////////////////////////////
CPictureUI::CPictureUI(void)
{
	m_bIsAutoPlay = true;
	m_bIsAutoSize = false;

	m_idEventTimer = 0;		//动画定时器
	m_nDelay = 0;			//循环毫秒数
	m_nFramePosition = 0;	//当前放到第几帧
}


CPictureUI::~CPictureUI(void)
{
	StopAnim();
	RemoveAllImages();
}

LPCTSTR	CPictureUI::GetClass() const
{
	return DUI_CTR_PICTURE;
}

LPVOID	CPictureUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_PICTURE) == 0 ) return static_cast<CPictureUI*>(this);
	return __super::GetInterface(pstrName);
}

void CPictureUI::DoInit()
{
	
}

void CPictureUI::PaintBkImage(HDC hDC)
{
	__super::PaintBkImage(hDC);

	if(m_nFramePosition >= m_frames.GetSize())
		m_nFramePosition = 0;


	TImageInfo *pImageInfo = (TImageInfo *)m_frames.GetAt(m_nFramePosition);
	if(!pImageInfo) return;
	TDrawInfo defaultDrawInfo;
	TDrawInfo *pDrawInfo = &defaultDrawInfo;
	std::map<TImageInfo *, TDrawInfo *>::iterator it = m_map.find(pImageInfo);
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
		rcSource.right = pImageInfo->nX;
		rcSource.bottom = pImageInfo->nY;
	}
	if (rcSource.right > pImageInfo->nX) rcSource.right = pImageInfo->nX;
	if (rcSource.bottom > pImageInfo->nY) rcSource.bottom = pImageInfo->nY;

	RECT rcCorner = {0};
	CRenderEngine::DrawImage(hDC, pImageInfo->hBitmap, rcDest, m_rcPaint, rcSource, rcCorner, GetManager()->IsLayered() ? true : pImageInfo->bAlpha, pDrawInfo->uFade, pDrawInfo->bHole, pDrawInfo->bTiledX, pDrawInfo->bTiledY);

	if(IsAutoPlay() && m_idEventTimer == 0 && m_frames.GetSize() > 1)
	{
		StartAnim();
	}
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
	else
		__super::SetAttribute(pstrName, pstrValue);
}

bool CPictureUI::LoadHBitmap(HBITMAP hBitmap, int x, int y)
{
	RemoveAllImages();
	return __SetHBitmap(hBitmap, x, y);
}

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
		TImageInfo *bitmap = (TImageInfo *)m_frames.GetAt(i);
		CRenderEngine::FreeImage(bitmap);
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
			TImageInfo *pImageInfo = (TImageInfo *)m_frames.GetAt(i);
			if(pImageInfo->nX > sz.cx)	sz.cx = pImageInfo->nX;
			if(pImageInfo->nY > sz.cy)	sz.cy = pImageInfo->nY;
		}
		SetFixedWidth(sz.cx);
		SetFixedHeight(sz.cy);
	}
}
bool CPictureUI::IsAutoSize() const			{ return m_bIsAutoSize; }

void CPictureUI::StartAnim()
{
	if(m_idEventTimer == 0 && m_frames.GetSize() > 1)
	{
		int nDelay = m_nDelay;
		if(nDelay <= 0) {
			if(m_frames.GetSize() > 0) {
				TImageInfo *pImageInfo = (TImageInfo *)m_frames.GetAt(0);
				nDelay = pImageInfo->delay;
			}
		}
		if(nDelay > 0)
		{
			m_idEventTimer = ::SetTimer(NULL, NULL, m_nDelay, TimerProc);
			g_MapTimerID_TO_CPictureUI[m_idEventTimer] = this;
		}
	}
}

void CPictureUI::StopAnim()
{
	if(m_idEventTimer != 0)
	{
		::KillTimer(NULL, m_idEventTimer);
		std::map<UINT, CControlUI *>::iterator it = g_MapTimerID_TO_CPictureUI.find(m_idEventTimer);
		if(it != g_MapTimerID_TO_CPictureUI.end())
			g_MapTimerID_TO_CPictureUI.erase(it);
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

bool CPictureUI::__SetHBitmap(HBITMAP hBitmap, int x, int y)
{
	TImageInfo* data = new TImageInfo;
	data->pBits = NULL;
	data->pSrcBits = NULL;
	data->hBitmap = hBitmap;
	data->nX = x;
	data->nY = y;
	data->bAlpha = false;
	data->delay = 0;
	if(!m_frames.Add(data))
	{
		delete data;
		return false;
	}
	SetAutoSize(IsAutoSize());
	return true;
}

bool CPictureUI::__LoadImageFromMemory(LPBYTE pData, DWORD dwSize)
{
	//尝试解析GIF
	CStdPtrArray arrImageInfo;
	if(CRenderEngine::LoadGifImageFromMemory(pData, dwSize, arrImageInfo))
	{
		for (int i=0; i<arrImageInfo.GetSize(); i++)
		{
			TImageInfo *pImageInfo = (TImageInfo *)arrImageInfo.GetAt(i);
			if(!m_frames.Add(pImageInfo))
			{
				delete pImageInfo;
				return false;
			}
		}
		return true;
	}

	TImageInfo *pImageInfo = CRenderEngine::LoadImageFromMemory(pData, dwSize, 0, 0, 0,0, GetManager());
	if(!pImageInfo) return false;
	if(!m_frames.Add(pImageInfo))
	{
		delete pImageInfo;
		return false;
	}
	SetAutoSize(IsAutoSize());
	return true;
}

bool CPictureUI::__LoadFile(LPCTSTR pstrImage)
{
	bool bRet = false;
	TImageInfo *pImageInfo = NULL;
	LPBYTE pData = NULL; 
	DWORD dwSize = 0;

	TDrawInfo *pDrawInfo = new TDrawInfo;
	pDrawInfo->Parse(pstrImage, NULL, GetManager());

	do 
	{
		//文件载入内存
		dwSize = CRenderEngine::LoadImage2Memory(pDrawInfo->sImageName.GetData(), NULL, pData);
		if(!pData || dwSize == 0) break;

		//尝试解析GIF
		CStdPtrArray arrImageInfo;
		if(CRenderEngine::LoadGifImageFromMemory(pData, dwSize, arrImageInfo))
		{
			for (int i=0; i<arrImageInfo.GetSize(); i++)
			{
				TImageInfo *pImageInfo2 = (TImageInfo *)arrImageInfo.GetAt(i);
				m_frames.Add(pImageInfo2);
				m_map[pImageInfo2] = pDrawInfo;
			}
			m_arrDrawInfo.Add(pDrawInfo);
			pDrawInfo = NULL;
			bRet = true;
			break;
		}

		//尝试其他格式的图像
		pImageInfo = CRenderEngine::LoadImageFromMemory(pData, dwSize, pDrawInfo->dwMask, pDrawInfo->width, pDrawInfo->height, pDrawInfo->fillcolor, GetManager());
		if(!pImageInfo) break;

		if(m_frames.Add(pImageInfo) && m_arrDrawInfo.Add(pDrawInfo))
		{
			m_map[pImageInfo] = pDrawInfo;
			pImageInfo = NULL;
			pDrawInfo = NULL;
			bRet = true;
			break;
		}

	} while (false);

	if(pImageInfo)		delete pImageInfo;
	if(pDrawInfo)	delete pDrawInfo;
	if(pData)		CRenderEngine::FreeMemory(pData);
	SetAutoSize(IsAutoSize());
	return bRet;
}

}