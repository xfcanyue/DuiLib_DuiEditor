#pragma once
#include "StdAfx.h"
#include "UIFrameWnd.h"

//namespace DuiLib{

static CUIApplication *g_pApplication = NULL;
UILIB_API CUIApplication * __stdcall UIGetApp()
{
	return g_pApplication;
}

CUIApplication::CUIApplication(void)
{
	g_pApplication = this;

	m_hInstance = NULL;
	m_pMainWnd = NULL;
	m_bCachedResourceZip = false;
	m_uZipResourceID = 0;

	m_bSingleApplication = false;
	m_hMutexApplication = NULL;
	m_UIAPP_SINGLEAPPLICATION_MSG = 0;
}

CUIApplication::~CUIApplication(void)
{
	
}

void CUIApplication::SetResType_File(LPCTSTR pstrPath)
{
	m_resType = UILIB_FILE;
	m_strSkinPath = pstrPath;
}

void CUIApplication::SetResType_Resource(LPCTSTR pstrPath, LPCTSTR resID, LPCTSTR type)
{
	m_resType = UILIB_RESOURCE;
	m_strSkinPath = pstrPath;
	m_idrID = resID;
	m_idrType = type;
}

void CUIApplication::SetResType_ZIP(LPCTSTR pstrPath, LPCTSTR pstrZip, bool bCachedResourceZip, LPCTSTR password)
{
	m_resType = UILIB_ZIP;
	m_strSkinPath = pstrPath;
	m_strZipFile = pstrZip;
	m_bCachedResourceZip = bCachedResourceZip;
	m_zipPassword = password;
}

void CUIApplication::SetResType_ZipResource(LPCTSTR pstrPath, LPCTSTR password, UINT resID, LPCTSTR type)
{
	m_resType = UILIB_ZIPRESOURCE;
	m_strSkinPath = pstrPath;
	m_uZipResourceID = resID;
	m_strZipResourceType = type;
	m_zipPassword = password;
}

bool CUIApplication::InitInstance(HINSTANCE hInstance)
{
	// COM
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return false;

	// OLE
	HRESULT hRes = ::OleInitialize(NULL);
	if( FAILED(Hr) ) return false;

	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	m_strAppPath = CPaintManagerUI::GetInstancePath();

	// 初始化资源
	InitResource();

	if(m_bSingleApplication && (!m_GuidAppName.IsEmpty()) )
	{
		m_UIAPP_SINGLEAPPLICATION_MSG = ::RegisterWindowMessage(m_GuidAppName);
		m_hMutexApplication = CreateMutex(NULL, FALSE, m_GuidAppName);
		if ( GetLastError() == ERROR_ALREADY_EXISTS )
		{
			DWORD dwRecipients = BSM_APPLICATIONS;
			::BroadcastSystemMessage( BSF_NOHANG, &dwRecipients, m_UIAPP_SINGLEAPPLICATION_MSG, 0, 0);
			return false;
		}
	}

	return true;
}

void CUIApplication::Run()
{
	if(m_pMainWnd)
	{
		m_pMainWnd->m_pApplication = this;
	}
	// 消息循环
	CPaintManagerUI::MessageLoop();
}

int CUIApplication::ExitInstance()
{
	if(m_pMainWnd != NULL) { delete m_pMainWnd; m_pMainWnd = NULL; }

	if(m_hMutexApplication != NULL)
	{
		CloseHandle(m_hMutexApplication); m_hMutexApplication = NULL;
	}

	// 清理资源
	CPaintManagerUI::Term();
	// OLE
	OleUninitialize();
	// COM
	::CoUninitialize();

	return 0;
}


void CUIApplication::InitResource()
{
	CPaintManagerUI::SetResourceType(m_resType);
	// 加载资源
	switch(CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			CPaintManagerUI::SetResourcePath(m_strAppPath + m_strSkinPath);
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			CPaintManagerUI::SetResourcePath(m_strAppPath + m_strSkinPath);
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(m_idrID.GetData(), m_idrType);
			break;
		}
	case UILIB_ZIP:
		{
			CPaintManagerUI::SetResourcePath(m_strAppPath + m_strSkinPath);
			// 加密
			CPaintManagerUI::SetResourceZip(m_strAppPath + m_strZipFile, true, NULL);
			// 加载资源管理器
			//CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			CPaintManagerUI::SetResourcePath(m_strAppPath + _T("Res\\"));
			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), MAKEINTRESOURCE(m_uZipResourceID), m_strZipResourceType);
			if( hResource != NULL ) {
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
				if( hGlobal != NULL ) {
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if( dwSize > 0 ) {
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize, m_zipPassword);
						// 加载资源管理器
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
					}
				}
				::FreeResource(hResource);
			}
		}
		break;
	}
}


bool CUIApplication::SetSingleApplication(bool bSingle, LPCTSTR szGuidName) //只允许运行单一实例
{
	if(szGuidName == NULL)	return false;
	if(szGuidName[0] == '\0')	return false;

	m_bSingleApplication = bSingle;
	m_GuidAppName = szGuidName;
	return true;
}