#include "stdafx.h"
#include "CxPicture.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CxPicture, CStatic)
	//{{AFX_MSG_MAP(CxPicture)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CxPicture::CxPicture()
{
	
	m_bAutoWrapping		= TRUE;
	m_bAutoAdjustFont	= TRUE;
	m_pPicture			= new CxImage();
	m_nFrameCount		= 0;
	m_pThread			= NULL;
	m_hExitEvent		= CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hBitmap			= NULL;
	m_hMemDC			= NULL;

	m_hDispMemDC		= NULL;
	m_hDispMemBM		= NULL;
	m_hDispOldBM		= NULL;

	m_bTransparentBk	= FALSE;
	m_bNotifyParent		= FALSE;
	m_bIsInitialized	= FALSE;
	m_bExitThread		= FALSE;
	m_bIsPlaying		= FALSE;
	m_bIsGIF			= FALSE;
	m_clrBackground		= RGB(255,255,255); // white by default
	m_nGlobalCTSize		= 0;
	m_nCurrOffset		= 0;
	m_nCurrFrame		= 0;
	m_nDataSize			= 0;
	m_PictureSize.cx	= 0;
	m_PictureSize.cy	= 0;
	SetRect(&m_PaintRect,0,0,0,0);


	m_rgbBkgnd		  = ::GetSysColor(COLOR_BTNFACE);
	m_rgbText		  = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crHiColor		  =	m_rgbBkgnd;
	m_crLoColor		  =	m_rgbBkgnd;
	m_pBrush          = new CBrush(m_rgbBkgnd);
}

CxPicture::~CxPicture()
{
	UnLoad();
	CloseHandle(m_hExitEvent);


	// Clean up our Brush
	if (m_pBrush){
		delete m_pBrush;
		m_pBrush = NULL;
	}

	// Clean our main Picture
	if (m_pPicture){
		delete m_pPicture;
		m_pPicture = NULL;
	}


}




void CxPicture::UnLoad()
{

	Stop();
	if (m_pPicture){
		m_pPicture->Destroy();
		//m_pPicture = NULL;
	};

	std::vector<TFrame>::iterator it;
	for (it=m_arrFrames.begin();it<m_arrFrames.end();it++){
		(*it).m_pPicture->Destroy();
		if ( (*it).m_pPicture ){
			delete (*it).m_pPicture;
		}
	}

	m_arrFrames.clear();

	if (m_hMemDC){
		SelectObject(m_hMemDC,m_hOldBitmap);
		::DeleteDC(m_hMemDC); ::DeleteObject(m_hBitmap);
		m_hMemDC  = NULL;
		m_hBitmap = NULL;
	};

	if (m_hDispMemDC){
		SelectObject(m_hDispMemDC,m_hDispOldBM);
		::DeleteDC(m_hDispMemDC); ::DeleteObject(m_hDispMemBM);
		m_hDispMemDC  = NULL;
		m_hDispMemBM = NULL;
	};

	//SetRect(&m_PaintRect,0,0,0,0);
	m_pThread		   = NULL;
	m_bIsInitialized   = FALSE;
	m_bExitThread	   = FALSE;
	m_bIsGIF		   = FALSE;
	m_clrBackground    = RGB(255,255,255); // white by default
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
	m_nSizeMode		   = FitControl;
}

BOOL CxPicture::PrepareDC(int nWidth, int nHeight)
{
	if (m_nSizeMode == OriginalSize)
		SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE | SWP_NOZORDER);

	HDC hWinDC = ::GetDC(m_hWnd);
	if (!hWinDC) return FALSE;

	m_hMemDC = CreateCompatibleDC(hWinDC);
	if (!m_hMemDC)
	{
		::ReleaseDC(m_hWnd,hWinDC);
		return FALSE;
	};

	m_hBitmap  = CreateCompatibleBitmap(hWinDC,nWidth,nHeight);
	if (!m_hBitmap)
	{
		::ReleaseDC(m_hWnd,hWinDC);
		::DeleteDC(m_hMemDC);
		return FALSE;
	};

	m_hOldBitmap = reinterpret_cast<HBITMAP> (SelectObject(m_hMemDC,m_hBitmap));

	// fill the background
	m_clrBackground = GetSysColor(RGB(255,0,0));
	RECT rect = {0,0,nWidth,nHeight};
	FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

	::ReleaseDC(m_hWnd,hWinDC);
	m_bIsInitialized = TRUE;
	return TRUE;
}






BOOL CxPicture::RedrawWindow()
{
	/*CRect rect;

	GetClientRect( rect );
	InvalidateRect( rect );*/
	Invalidate();
	return TRUE;
}



BOOL CxPicture::SetBitmap(HBITMAP hBitmap, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	bool bRet = m_pPicture->CreateFromHBITMAP(hBitmap);
	if (!bRet) return FALSE;

	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();
	m_nFrameCount = m_pPicture->GetNumFrames();

	bRet = (PrepareDC(m_PictureSize.cx, m_PictureSize.cy)!=0);
	if (!bRet) return FALSE;

	DrawBitmap();

	return TRUE;
}


void CxPicture::SetOriginalSize()
{


}

BOOL CxPicture::SetBitmap(UINT nIDResource, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	HBITMAP hBmp = ::LoadBitmap( AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIDResource));

	return SetBitmap(hBmp, Emode);
}

BOOL CxPicture::SetBitmap(LPCTSTR lpszFileName, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	TRACE(_T("m_arrFrames Size = %d\n"), m_arrFrames.size());
	CString csExt = lpszFileName;
	int nDotPos = csExt.ReverseFind('.');
	if (nDotPos != -1)
		csExt = csExt.Mid(++nDotPos);

	int nImgType = FindType(csExt);
	if (nImgType == CXIMAGE_FORMAT_GIF)
		m_bIsGIF = TRUE;

	// Load Image From disk
	bool bRet = m_pPicture->Load(lpszFileName, nImgType);
	if (!bRet) return FALSE;

	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();


	TFrame	frame;
	UINT	nCurFrame = 0;

	m_nFrameCount = m_pPicture->GetNumFrames();
	for(int i=0; i < (int)m_nFrameCount; i++){
		CxImage *newImage = new CxImage();
		newImage->SetFrame(i);
		newImage->Load(lpszFileName,nImgType);

		frame.m_pPicture = newImage;
		frame.m_frameOffset.cx= 0;
		frame.m_frameOffset.cy = 0;
		frame.m_frameSize.cx = newImage->GetWidth();
		frame.m_frameSize.cy = newImage->GetHeight();
		frame.m_nDelay = newImage->GetFrameDelay();
		frame.m_nDisposal = 1;

		m_arrFrames.push_back(frame);
	}
	

	
	bRet = (PrepareDC(m_pPicture->GetWidth(), m_pPicture->GetHeight())!=0);
	if (!bRet) return FALSE;

	DrawBitmap();
	return TRUE;
}

BOOL CxPicture::SetBitmap(const CByteArray& ImgBuffer, CString csFileName, EImageSize Emode)
{
	UnLoad();

	m_nSizeMode = Emode;

	CString csExt = csFileName;
	int nDotPos = csExt.ReverseFind('.');
	if (nDotPos != -1)
		csExt = csExt.Mid(++nDotPos);

	int nImgType = FindType(csExt);
	if (nImgType == CXIMAGE_FORMAT_GIF)
		m_bIsGIF = TRUE;

	ULONG nBufferLen = ImgBuffer.GetSize();
	BYTE* pBuffer = new BYTE [ nBufferLen ];
	memset(pBuffer, 0, nBufferLen);

	for (ULONG i = 0; i < nBufferLen; i++)
		pBuffer[i] = ImgBuffer.GetAt(i);

	CxMemFile memfile((BYTE*)pBuffer, nBufferLen);
	bool bRet = m_pPicture->Decode(&memfile,nImgType);
	m_PictureSize.cx = m_pPicture->GetWidth();
	m_PictureSize.cy = m_pPicture->GetHeight();

	TFrame	frame;
	UINT	nCurFrame = 0;

	m_nFrameCount = m_pPicture->GetNumFrames();
	for(int j=0; j < (int)m_nFrameCount; j++){
		CxImage *newImage = new CxImage();
		CxMemFile memfile((BYTE*)pBuffer, nBufferLen);
		newImage->SetFrame(j);
		bRet = newImage->Decode(&memfile,nImgType);

		frame.m_pPicture = newImage;
		frame.m_frameOffset.cx= 0;
		frame.m_frameOffset.cy = 0;
		frame.m_frameSize.cx = newImage->GetWidth();
		frame.m_frameSize.cy = newImage->GetHeight();
		frame.m_nDelay = newImage->GetFrameDelay();
		frame.m_nDisposal = 1;

		m_arrFrames.push_back(frame);
	}
	bRet = (PrepareDC(m_pPicture->GetWidth(), m_pPicture->GetHeight())!=0);
	if (!bRet) return FALSE;

	DrawBitmap();

	if (pBuffer){
		delete [] pBuffer;
	}

	return TRUE;
}




BOOL CxPicture::DrawBitmap()
{
	if (!m_bIsInitialized)
		return FALSE;

	if (IsAnimatedGIF()){
	// the picture needs animation
	// we'll start the thread that will handle it for us
	
		m_pThread = AfxBeginThread( _ThreadAnimation, 
									this, 
									THREAD_PRIORITY_NORMAL, 
									0, 
									CREATE_SUSPENDED, 
									NULL);
		if (!m_pThread){
			TRACE(_T("Draw: Couldn't start a GIF animation thread\n"));
			return FALSE;
		} 
		else{ 
			m_pThread->ResumeThread();
		}
	} 
	else{
		if ( m_pPicture ){
			long hmWidth  = m_pPicture->GetWidth();
			long hmHeight = m_pPicture->GetHeight();
			if (m_pPicture->Draw2(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy) == TRUE){
				Invalidate(FALSE);
				return TRUE;
			}
		}
	}

	return FALSE;
}

SIZE CxPicture::GetSize() const
{
	return m_PictureSize;
}




//============================================================================
// GIF ANIMATION THREAD
//============================================================================


UINT CxPicture::_ThreadAnimation(LPVOID pParam)
{
	ASSERT(pParam);
	CxPicture *pPic = reinterpret_cast<CxPicture *> (pParam);

	pPic->m_bIsPlaying = TRUE;
	pPic->ThreadAnimation();
	pPic->m_bIsPlaying = FALSE;

	// this thread has finished its work so we close the handle
	//CloseHandle(pPic->m_hThread);
	// and init the handle to zero (so that Stop() doesn't Wait on it)
	//pPic->m_hThread = 0;
	
	return 0;
}

void CxPicture::ThreadAnimation()
{
	while (!m_bExitThread)
	{
		if (m_arrFrames[m_nCurrFrame].m_pPicture)
		{
			long hmWidth = m_arrFrames[m_nCurrFrame].m_pPicture->GetWidth();
			long hmHeight= m_arrFrames[m_nCurrFrame].m_pPicture->GetHeight();

			if (m_arrFrames[m_nCurrFrame].m_pPicture->Draw2(m_hMemDC,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameSize.cy) == TRUE)
			{
				Invalidate(FALSE);
			};

			if (m_bExitThread) break;

			// if the delay time is too short (like in old GIFs), wait for 100ms
			if (m_arrFrames[m_nCurrFrame].m_nDelay < 5)
				WaitForSingleObject(m_hExitEvent, 100);
			else
				WaitForSingleObject(m_hExitEvent, 10*m_arrFrames[m_nCurrFrame].m_nDelay);

			if (m_bExitThread) break;

		}
		m_nCurrFrame++;
		if (m_nCurrFrame == m_arrFrames.size())
		{
			m_nCurrFrame = 0;
		
			// init the screen for the first frame,
			HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			if (hBrush)
			{
				RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
				FillRect(m_hMemDC,&rect,hBrush);
				DeleteObject(hBrush);
			}
		}
	}
}


void CxPicture::Stop()
{
	m_bIsPlaying = FALSE;
	m_bExitThread = TRUE;
	SetEvent(m_hExitEvent);
	
	

	// make it possible to Draw() again
	ResetEvent(m_hExitEvent);
	m_bExitThread = FALSE;
}


BOOL CxPicture::IsAnimatedGIF() const
{
	return (m_bIsGIF && (m_arrFrames.size() > 1));
}

BOOL CxPicture::IsPlaying() const
{
	return m_bIsPlaying;
}

int CxPicture::GetFrameCount() const
{
	if (!IsAnimatedGIF())
		return 0;

	return m_arrFrames.size();
}

COLORREF CxPicture::GetBkColor() const
{
	return m_clrBackground;
}


//============================================================================
// WINDOWS MESSAGE HANDLING
//============================================================================

void CxPicture::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	CBrush backBrush(RGB(255,255,255));
    dc.FillRect(&rect, &backBrush);

	// Render background color
	//if (m_fillmode == Normal)
	//	dc.FillRect(&rect, m_pBrush);
	//else
	//	DrawGradientFill(&dc, &rect, m_fillmode);

	// Render Bitmap
	::StretchBlt(dc.m_hDC, 0, 0, rect.Width(), rect.Height(),
		m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
}


void CxPicture::OnDestroy()
{
	UnLoad();
	CStatic::OnDestroy();
}

void CxPicture::OnSize(UINT nType, int cx, int cy)
{
  CStatic::OnSize(nType, cx, cy);
  Invalidate();
}

BOOL CxPicture::SetPaintRect(const RECT *lpRect)
{
	return CopyRect(&m_PaintRect, lpRect);
}

BOOL CxPicture::GetPaintRect(RECT *lpRect)
{
	return CopyRect(lpRect, &m_PaintRect);
}

int CxPicture::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}


