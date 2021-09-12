#include "StdAfx.h"
#include "UIResourceManager.h"

namespace DuiLib {
	
	CResourceManager::CResourceManager(void)
	{
		m_pQuerypInterface = NULL;
		
	}

	CResourceManager::~CResourceManager(void)
	{
		ResetResourceMap();
	}

	BOOL CResourceManager::LoadResource(STRINGorID xml, LPCTSTR type)
	{
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !m_xml.load_string(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !m_xml.load_file(xml.m_lpstr) ) return NULL;
			}
		}
		else
		{
			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), xml.m_lpstr, type);
			if( hResource == NULL ) return NULL;
			HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
			if( hGlobal == NULL ) 
			{
				FreeResource(hResource);
				return NULL;
			}

			if( !m_xml.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hResource);
		}

		return LoadResource(m_xml.root());
	}

	BOOL CResourceManager::LoadResource(CXmlNodeUI root)
	{
		if( !root ) return FALSE;

		CXmlNodeUI nodeRes = root.child(_T("Res"));
		if(!nodeRes) return FALSE;

		//加载图片资源
		for( CXmlNodeUI node = nodeRes.first_child() ; node; node = node.next_sibling() ) 
		{
			LPCTSTR pstrClass = node.name();
			if ((_tcsicmp(pstrClass,_T("Image")) == 0))
			{
				//加载图片资源
				CXmlAttributeUI attrId = node.attribute(_T("id"));
				CXmlAttributeUI attrPath = node.attribute(_T("path"));

				LPCTSTR pstrId = attrId.value();
				LPCTSTR pstrPath = attrPath.value();
				if( pstrId == NULL ||  pstrPath == NULL) continue;

				CDuiString * pstrFind = static_cast<CDuiString *>(m_mImageHashMap.Find(pstrId));
				if(pstrFind != NULL) continue;
				m_mImageHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrPath));
			}
			else if( _tcsicmp(pstrClass,_T("Xml")) == 0) 
			{
				//加载XML配置文件
				CXmlAttributeUI attrId = node.attribute(_T("id"));
				CXmlAttributeUI attrPath = node.attribute(_T("path"));

				LPCTSTR pstrId = attrId.value();
				LPCTSTR pstrPath = attrPath.value();
				if( pstrId == NULL ||  pstrPath == NULL) continue;

				CDuiString * pstrFind = static_cast<CDuiString *>(m_mXmlHashMap.Find(pstrId));
				if(pstrFind != NULL) continue;
				m_mXmlHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrPath));
			}
			else continue;
		}
		return TRUE;
	}

	LPCTSTR CResourceManager::GetImagePath(LPCTSTR lpstrId)
	{
		CDuiString * lpStr = static_cast<CDuiString *>(m_mImageHashMap.Find(lpstrId));
		return lpStr == NULL? NULL:lpStr->GetData();
	}

	LPCTSTR CResourceManager::GetXmlPath(LPCTSTR lpstrId)
	{
		CDuiString * lpStr = static_cast<CDuiString *>(m_mXmlHashMap.Find(lpstrId));
		return lpStr == NULL? NULL:lpStr->GetData();
	}

	void CResourceManager::ResetResourceMap()
	{
		CDuiString* lpStr;
		for( int i = 0; i< m_mImageHashMap.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mImageHashMap.GetAt(i))
			{
				lpStr = static_cast<CDuiString *>(m_mImageHashMap.Find(key));
				delete lpStr;
				lpStr = NULL;
			}
		}
		for( int i = 0; i< m_mXmlHashMap.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mXmlHashMap.GetAt(i))
			{
				lpStr = static_cast<CDuiString *>(m_mXmlHashMap.Find(key));
				delete lpStr;
				lpStr = NULL;
			}
		}
		for( int i = 0; i< m_mTextResourceHashMap.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mTextResourceHashMap.GetAt(i))
			{
				lpStr = static_cast<CDuiString *>(m_mTextResourceHashMap.Find(key));
				delete lpStr;
				lpStr = NULL;
			}
		}
	}

	BOOL CResourceManager::LoadLanguage(LPCTSTR pstrXml)
	{
		CXmlDocumentUI xml;
		if( *(pstrXml) == _T('<') ) {
			if( !xml.load_string(pstrXml) ) return FALSE;
		}
		else {
			if( !xml.load_file(pstrXml) ) return FALSE;
		}

		CXmlNodeUI nodeRes = xml.child(_T("Res"));
		if( !nodeRes ) return FALSE;

		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;

		//加载图片资源
		LPCTSTR pstrId = NULL;
		LPCTSTR pstrText = NULL;
		for( CXmlNodeUI node = nodeRes.first_child(); node; node = node.next_sibling() ) 
		{
			pstrClass = node.name();
			if ((_tcsicmp(pstrClass,_T("Text")) == 0))
			{
				CXmlAttributeUI attrId = node.attribute(_T("id"));
				CXmlAttributeUI attrValue = node.attribute(_T("value"));

				LPCTSTR pstrId = attrId.value();
				LPCTSTR pstrText = attrValue.value();
				if( pstrId == NULL ||  pstrText == NULL) continue;

				CDuiString *lpstrFind = static_cast<CDuiString *>(m_mTextResourceHashMap.Find(pstrId));
				if(lpstrFind != NULL) {
					lpstrFind->Assign(pstrText);
				}
				else {
					m_mTextResourceHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrText));
				}
			}
			else continue;
		}

		return TRUE;
	}

	CDuiString CResourceManager::GetText(LPCTSTR lpstrId, LPCTSTR lpstrType)
	{
		if(lpstrId == NULL) return _T("");

		CDuiString *lpstrFind = static_cast<CDuiString *>(m_mTextResourceHashMap.Find(lpstrId));
		if (lpstrFind == NULL && m_pQuerypInterface)
		{
			lpstrFind = new CDuiString(m_pQuerypInterface->QueryControlText(lpstrId, lpstrType));
			m_mTextResourceHashMap.Insert(lpstrId, (LPVOID)lpstrFind);
		}
		return lpstrFind == NULL ? lpstrId : *lpstrFind;
	}

	void CResourceManager::ReloadText()
	{
		if(m_pQuerypInterface == NULL) return;
		//重载文字描述
		LPCTSTR lpstrId = NULL;
		LPCTSTR lpstrText;
		for( int i = 0; i < m_mTextResourceHashMap.GetSize(); i++ )
		{
			lpstrId = m_mTextResourceHashMap.GetAt(i);
			if (lpstrId == NULL) continue;
			lpstrText = m_pQuerypInterface->QueryControlText(lpstrId, NULL);
			if(lpstrText != NULL) {
				CDuiString * lpStr = static_cast<CDuiString *>(m_mTextResourceHashMap.Find(lpstrId));
				lpStr->Assign(lpstrText);
			}
		}
	}
	void CResourceManager::ResetTextMap()
	{
		CDuiString * lpStr;
		for( int i = 0; i< m_mTextResourceHashMap.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mTextResourceHashMap.GetAt(i))
			{
				lpStr = static_cast<CDuiString *>(m_mTextResourceHashMap.Find(key));
				delete lpStr;
			}
		}
	}

	
} // namespace DuiLib