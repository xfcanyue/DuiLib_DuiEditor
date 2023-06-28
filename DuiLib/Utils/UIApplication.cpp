#include "StdAfx.h"

namespace DuiLib{

CUIApplicationBase *g_pApplication = NULL;
UILIB_API CUIApplicationBase * __stdcall UIGetApp()
{
	return g_pApplication;
}

CUIApplicationBase::CUIApplicationBase(void)
{
	g_pApplication = this;
	m_pMainWnd = NULL;
}

CUIApplicationBase::~CUIApplicationBase(void)
{

}

void CUIApplicationBase::SetAppPath(LPCTSTR sAppPath)
{
	m_strAppPath = sAppPath;
}

CDuiString CUIApplicationBase::GetAppPath()
{
	return m_strAppPath;
}

void CUIApplicationBase::SetAppName(LPCTSTR pstrName) 
{ 
	m_strAppName = pstrName; 
}

LPCTSTR CUIApplicationBase::GetAppName() 
{ 
	return m_strAppName.GetData();
} 


void CUIApplicationBase::SetResType_File(LPCTSTR pstrPath)
{
	m_resType = UILIB_FILE;
	m_strSkinPath = pstrPath;
}

void CUIApplicationBase::SetResType_Resource(LPCTSTR pstrPath, LPCTSTR resID, LPCTSTR type)
{
	m_resType = UILIB_RESOURCE;
	m_strSkinPath = pstrPath;
	m_idrID = resID;
	m_idrType = type;
}

void CUIApplicationBase::SetResType_ZIP(LPCTSTR pstrPath, LPCTSTR pstrZip, bool bCachedResourceZip, LPCTSTR password)
{
	m_resType = UILIB_ZIP;
	m_strSkinPath = pstrPath;
	m_strZipFile = pstrZip;
	m_bCachedResourceZip = bCachedResourceZip;
	m_zipPassword = password;
}

void CUIApplicationBase::SetResType_ZipResource(LPCTSTR pstrPath, LPCTSTR password, UINT resID, LPCTSTR type)
{
	m_resType = UILIB_ZIPRESOURCE;
	m_strSkinPath = pstrPath;
	m_uZipResourceID = resID;
	m_strZipResourceType = type;
	m_zipPassword = password;
}

void CUIApplicationBase::InitResource()
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
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
#ifdef WIN32
			CPaintManagerUI::SetResourcePath(m_strAppPath + m_strSkinPath);
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
#endif
		}
		break;
	}
}
}//namespace DuiLib{
