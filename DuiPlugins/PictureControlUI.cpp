#include "StdAfx.h"
#include "PictureControlUI.h"

#include "../DuiLib/Utils/unzip.cpp"

IMPLEMENT_DUICONTROL(CPictureControlUI)

#define GIFANIMUIEX_EVENT_TIEM_ID	100
struct CPictureControlUI::Imp
{

	bool				m_bRealStop			;//外部停止了
	bool				m_bLoadImg			;//是否加载过图片
	bool				m_bTimer			;//是否启动定时器
	bool				m_bAutoStart		;//是否自动开始
	int					m_nDelay			;//循环毫秒数
	UINT				m_nFrameCount		;//gif图片总帧数
	UINT				m_nFramePosition	;//当前放到第几帧
	CxImage*			m_pGifImage			;//gif对象
	CPaintManagerUI*&	m_pManager			;
	CPictureControlUI*			m_pOwer				;//拥有者


	Imp(CPaintManagerUI* & pManager):m_pManager(pManager),
		m_bLoadImg(false),m_bTimer(false),
		m_nDelay(0),m_pGifImage(NULL),m_nFrameCount(0U),
		m_nFramePosition(0U),
		m_pOwer(NULL),m_bRealStop(false),m_bAutoStart(true)
	{
	}
	~Imp()
	{
	}
	void SetOwer(CPictureControlUI *pOwer){ m_pOwer = pOwer; }
	inline bool IsLoadImage(){return m_bLoadImg;}
	virtual void LoadGifImage(CxImage *pImg)
	{
		if(pImg->GetType() != CXIMAGE_FORMAT_GIF)
			return;

		m_bLoadImg = true;
		m_pGifImage	= pImg;

		m_nFrameCount	=	m_pGifImage->GetNumFrames();
		m_nFramePosition = 0;
		if(m_nDelay == 0)
			m_nDelay = m_pGifImage->GetFrameDelay();
		if(!m_bAutoStart)
			m_bRealStop = true;
		if(m_bAutoStart && m_pOwer->IsVisible())
			StartAnim();
	}
	void SetAutoStart(bool bAuto)
	{
		m_bAutoStart = bAuto;
	}
	void StartAnim()
	{
		if(!m_bTimer)
		{
			if(m_pGifImage)
				m_pManager->SetTimer( m_pOwer, GIFANIMUIEX_EVENT_TIEM_ID, m_nDelay );
			m_bTimer = true;
		}
	}
	void StopAnim(bool bGoFirstFrame)//bGoFirstFrame 是否跑到第一帧
	{
		if(m_bTimer)
		{
			if(bGoFirstFrame)
			{
				m_nFramePosition = 0U;
				m_pOwer->Invalidate();
			}
			m_pManager->KillTimer( m_pOwer, GIFANIMUIEX_EVENT_TIEM_ID );
			m_bTimer = false;
		}
	}
	void EventOnTimer(const WPARAM idEvent )
	{
		if ( idEvent != GIFANIMUIEX_EVENT_TIEM_ID )
			return;
		++m_nFramePosition;
		if(m_nFramePosition >= m_nFrameCount)
			m_nFramePosition = 0;
		if(!m_pOwer->IsVisible())return;
		m_pOwer->Invalidate();
	}
	void DrawFrame( HDC hDC,const RECT& rcPaint,const RECT &rcItem)
	{
		if ( NULL == hDC || NULL == m_pGifImage ) return;
		if(m_pGifImage)
		{
			CxImage* pImage = m_pGifImage->GetFrame(m_nFramePosition);
			if(pImage)
				pImage->Draw2(hDC,rcItem);
		}
	}
	void EventSetVisible(bool bVisible)
	{
		if(bVisible)
		{
			if(!m_bRealStop)
				StartAnim();
		}
		else
			StopAnim(true);
	}
};

//////////////////////////////////////////////////////////////////////////
CPictureControlUI::CPictureControlUI(void):m_pImp(new CPictureControlUI::Imp(m_pManager)), m_uButtonState(0)
{
	m_pImage = NULL;
	m_pImp->SetOwer(this);
}


CPictureControlUI::~CPictureControlUI(void)
{
	delete m_pImp; m_pImp = NULL;
	if(m_pImage != NULL) { delete m_pImage; m_pImage = NULL; }
}

LPCTSTR	CPictureControlUI::GetClass() const
{
	return _T("PictureControl");
}

LPVOID	CPictureControlUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("PictureControl")) == 0 ) return static_cast<CPictureControlUI*>(this);
	return __super::GetInterface(pstrName);
}

void CPictureControlUI::Init()
{
	
}

void CPictureControlUI::SetVisible(bool bVisible/* = true*/)
{
	__super::SetVisible(bVisible);
	m_pImp->EventSetVisible(bVisible);
}

void CPictureControlUI::PaintForeImage(HDC hDC)
{
	if(m_pImage)
	{
		if(m_pImage->IsValid() && m_pImage->IsEnabled())
		{
			if(m_type == CXIMAGE_FORMAT_GIF)
				m_pImp->DrawFrame( hDC, m_rcPaint, m_rcItem);
			else
				m_pImage->Draw(hDC, m_rcPaint);
		}
	}
}

void CPictureControlUI::DoEvent(TEventUI& event)
{
	WPARAM nID = event.wParam;
	if( event.Type == UIEVENT_TIMER )
	{
		m_pImp->EventOnTimer(nID);
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

void CPictureControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("fileimage")) == 0 ) 
	{
		CxImage img;
		LoadGifImageX(img, pstrValue, 0, 0);
		SetPicture(img);
	}
	else if( _tcsicmp(pstrName, _T("framedelay")) == 0 ) 
	{
		SetFrameDelay(_ttoi(pstrValue));
	}
	else
		__super::SetAttribute(pstrName, pstrValue);
}

void CPictureControlUI::SetFrameDelay(int nDelay)
{
	if(nDelay != m_pImp->m_nDelay)
	{
		m_pImp->m_nDelay = nDelay;
		if(m_type == CXIMAGE_FORMAT_GIF)
		{
			if(m_pImp->m_nDelay <= 0)
			{
				m_pImp->m_nDelay = m_pImage->GetFrameDelay();
			}
			StopAnim();
			StartAnim();
		}
	}
}

int  CPictureControlUI::GetFrameDelay()
{
	return m_pImp->m_nDelay;
}

void CPictureControlUI::SetPicture(LPCTSTR fileName)
{
	CxImage img;
	img.SetRetreiveAllFrames(true);
	img.Load(fileName);
	SetPicture(img);
}

void CPictureControlUI::SetPicture(CxImage &img)
{
	if(m_pImage != NULL) { delete m_pImage; m_pImage = NULL; }
	m_pImage = new CxImage;
	*m_pImage = img;
	m_type = m_pImage->GetType();

	if(m_type == CXIMAGE_FORMAT_GIF)
	{
		m_pImp->LoadGifImage(m_pImage);
		StartAnim();
	}
	NeedUpdate();
}

void CPictureControlUI::StartAnim()
{
	m_pImp->m_bRealStop = false;
	m_pImp->StartAnim();
}

void CPictureControlUI::StopAnim()
{
	m_pImp->m_bRealStop = true;
	m_pImp->StopAnim(true);
}

DWORD CPictureControlUI::LoadImage2Memory(const STRINGorID &bitmap, LPCTSTR type,LPBYTE &pData)
{
	assert(pData == NULL);
	pData = NULL;
	DWORD dwSize(0U);
	do 
	{
		if( type == NULL )
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if( CPaintManagerUI::GetResourceZip().IsEmpty() )
			{
				sFile += bitmap.m_lpstr;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if( hFile == INVALID_HANDLE_VALUE ) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if( dwSize == 0 ) break;

				DWORD dwRead = 0;
				pData = new BYTE[ dwSize + 1 ];
				memset(pData,0,dwSize+1);
				::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
				::CloseHandle( hFile );

				if( dwRead != dwSize ) 
				{
					delete[] pData;
					pData = NULL;
					dwSize = 0U;
					break;
				}
			}
			else 
			{
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if( CPaintManagerUI::IsCachedResourceZip() ) 
					hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
				else {
					CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
#ifdef UNICODE
					char* pwd = w2a((wchar_t*)sFilePwd.GetData());
					hz = OpenZip(sFile.GetData(), pwd);
					if(pwd) delete[] pwd;
#else
					hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
				}
				if( hz == NULL ) break;
				ZIPENTRY ze; 
				int i = 0; 
				CDuiString key = bitmap.m_lpstr;
				key.Replace(_T("\\"), _T("/")); 
				if( FindZipItem(hz, key, true, &i, &ze) != 0 ) break;
				dwSize = ze.unc_size;
				if( dwSize == 0 ) break;
				pData = new BYTE[ dwSize ];
				int res = UnzipItem(hz, i, pData, dwSize);
				if( res != 0x00000000 && res != 0x00000600)
				{
					delete[] pData;
					pData = NULL;
					dwSize = 0U;
					if( !CPaintManagerUI::IsCachedResourceZip() )
						CloseZip(hz);
					break;
				}
				if( !CPaintManagerUI::IsCachedResourceZip() )
					CloseZip(hz);
			}
		}
		else 
		{
			HINSTANCE hDll = CPaintManagerUI::GetResourceDll();
			HRSRC hResource = ::FindResource(hDll, bitmap.m_lpstr, type);
			if( hResource == NULL ) break;
			HGLOBAL hGlobal = ::LoadResource(hDll, hResource);
			if( hGlobal == NULL ) 
			{
				FreeResource(hResource);
				break;
			}

			dwSize = ::SizeofResource(hDll, hResource);
			if( dwSize == 0 ) break;
			pData = new BYTE[ dwSize ];
			::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
			::FreeResource(hResource);
		}
	} while (0);

	while (!pData)
	{
		//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
		HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
			FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFile == INVALID_HANDLE_VALUE ) break;
		dwSize = ::GetFileSize(hFile, NULL);
		if( dwSize == 0 ) break;

		DWORD dwRead = 0;
		pData = new BYTE[ dwSize ];
		::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
		::CloseHandle( hFile );

		if( dwRead != dwSize ) 
		{
			delete[] pData;
			pData = NULL;
			dwSize = 0U;
		}
		break;
	}
	return dwSize;
}


bool CPictureControlUI::LoadGifImageX(CxImage &img, STRINGorID bitmap, LPCTSTR type , DWORD mask)
{
	LPBYTE pData = NULL;
	DWORD dwSize = LoadImage2Memory(bitmap,type,pData);
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