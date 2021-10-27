#include "StdAfx.h"
#include "UILangManager.h"

namespace DuiLib {
	
	CLangPackageUI::CLangPackageUI()
	{

	}

	CLangPackageUI::~CLangPackageUI()
	{
		ResetResource();
	}

	void CLangPackageUI::SetSkinFile(LPCTSTR lpstrSkinFile)
	{
		m_sSkinFile = lpstrSkinFile;
	}

	LPCTSTR CLangPackageUI::GetSkinFile()
	{
		return m_sSkinFile.GetData();
	}

	BOOL CLangPackageUI::LoadResource(STRINGorID xml, LPCTSTR type)
	{
		CXmlDocumentUI xmlMarkup;
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !xmlMarkup.load_string(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !xmlMarkup.load_file(xml.m_lpstr) ) return NULL;
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

			if( !xmlMarkup.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hResource);
		}

		return LoadResource(xmlMarkup.root());
	}

	BOOL CLangPackageUI::LoadResource(CXmlNodeUI root)
	{	
		if( !root ) return FALSE;

		CXmlNodeUI nodeLanguage = root.child(_T("Language"));
		if(!nodeLanguage) return FALSE;

		//加载资源
		for( CXmlNodeUI node = nodeLanguage.first_child(); node; node = node.next_sibling() ) 
		{
			LPCTSTR pstrClass = node.name();
			if ((_tcsicmp(pstrClass,_T("Text")) == 0))
			{
				CXmlAttributeUI attrId = node.attribute(_T("id"));
				if(!attrId) continue;
				CXmlAttributeUI attrText = node.attribute(_T("text"));
				CXmlAttributeUI attrToolTip = node.attribute(_T("tooltip"));
				CXmlAttributeUI attrTipValue = node.attribute(_T("tipvalue"));

				int id = attrId.as_int();
				if(attrText != NULL)
					AddText(id, attrText.value());
				if(attrToolTip != NULL)
					AddToolTip(id, attrToolTip.value());
				if(attrTipValue != NULL)
					AddTipValue(id, attrTipValue.value());
			}
		}
		return TRUE;
	}

	void CLangPackageUI::ResetResource()
	{
		CDuiString* lpStr;
		for (std::map<int, CDuiString *>::iterator it=m_mText.begin(); it!=m_mText.end(); it++)
		{
			lpStr = it->second; delete lpStr;
		}
		m_mText.clear();

		for (std::map<int, CDuiString *>::iterator it=m_mToolTip.begin(); it!=m_mToolTip.end(); it++)
		{
			lpStr = it->second; delete lpStr;
		}
		m_mToolTip.clear();

		for (std::map<int, CDuiString *>::iterator it=m_mTipValue.begin(); it!=m_mTipValue.end(); it++)
		{
			lpStr = it->second; delete lpStr;
		}
		m_mTipValue.clear();
	}

	BOOL CLangPackageUI::AddText(int resid, LPCTSTR lpstrText)
	{
// 		if(resid == 39)
// 		{
// 			int x= 0; x++;
// 		}
		std::map<int, CDuiString *>::iterator it = m_mText.find(resid);
		if(it != m_mText.end())
		{
			return FALSE;
		}
		m_mText[resid] = new CDuiString(lpstrText);
		return TRUE;
	}

	LPCTSTR CLangPackageUI::GetText(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mText.find(resid);
		if(it != m_mText.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}	

	BOOL CLangPackageUI::AddToolTip(int resid, LPCTSTR lpstrText)
	{
		std::map<int, CDuiString *>::iterator it = m_mToolTip.find(resid);
		if(it != m_mToolTip.end())
		{
			return FALSE;
		}
		m_mToolTip[resid] = new CDuiString(lpstrText);
		return TRUE;
	}

	LPCTSTR CLangPackageUI::GetToolTip(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mToolTip.find(resid);
		if(it != m_mToolTip.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}

	BOOL CLangPackageUI::AddTipValue(int resid, LPCTSTR lpstrText)
	{
		std::map<int, CDuiString *>::iterator it = m_mTipValue.find(resid);
		if(it != m_mTipValue.end())
		{
			return FALSE;
		}
		m_mTipValue[resid] = new CDuiString(lpstrText);
		return TRUE;
	}

	LPCTSTR CLangPackageUI::GetTipValue(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mTipValue.find(resid);
		if(it != m_mTipValue.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}

	//////////////////////////////////////////////////////////////////////////
	CDuiString CLangManagerUI::s_sLangPath;
	CDuiString CLangManagerUI::s_sLangName;
	std::map<int, CLangManagerUI::tagStringTable *> CLangManagerUI::m_mStringTable;
	CLangManagerUI::CLangManagerUI(void)
	{
		
	}

	CLangManagerUI::~CLangManagerUI(void)
	{
		ResetPackage();

		CDuiString *pstr;
		for( int i = 0; i< m_mSkinFile.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mSkinFile.GetAt(i))
			{
				pstr = static_cast<CDuiString *>(m_mSkinFile.Find(key));
				delete pstr; pstr = NULL;
			}
		}
		m_mSkinFile.RemoveAll();
	}

	void CLangManagerUI::ResetPackage()
	{
		for( int i = 0; i< m_mLangPackage.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mLangPackage.GetAt(i))
			{
				CLangPackageUI *pkg = static_cast<CLangPackageUI *>(m_mLangPackage.Find(key));
				delete pkg; pkg = NULL;
			}
		}
		m_mLangPackage.RemoveAll();	
	}

	CLangPackageUI *CLangManagerUI::_addPackage(LPCTSTR lpstrSkinFile)
	{
		CLangPackageUI *pkg = static_cast<CLangPackageUI *>(m_mLangPackage.Find(lpstrSkinFile));
		if(pkg) return FALSE;

		pkg = new CLangPackageUI;
		pkg->SetSkinFile(lpstrSkinFile);

		CDuiString sPath = CLangManagerUI::GetLangPath();
		if(sPath.GetAt(sPath.GetLength()-1) != '\\')	sPath += '\\';
		CDuiString sFile = lpstrSkinFile;
		if(sFile.ReverseFind('.') >= 0)
		{
			sFile = sFile.Left(sFile.ReverseFind('.'));
			sFile += _T(".lng");
		}
		sPath += sFile;

		STRINGorID xml(sPath.GetData());
		pkg->LoadResource(xml, NULL);
		if(!m_mLangPackage.Insert(lpstrSkinFile, pkg))
		{
			delete pkg;
			return NULL;
		}
		return pkg;
	}

	void CLangManagerUI::ReloadLanguage()
	{
		ResetPackage();

		for( int i = 0; i< m_mSkinFile.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mSkinFile.GetAt(i))
			{
				CDuiString *pstr = static_cast<CDuiString *>(m_mSkinFile.Find(key));
				_addPackage(pstr->GetData());
			}
		}
	}

	CLangPackageUI *CLangManagerUI::AddPackage(LPCTSTR lpstrSkinFile)
	{
		CDuiString *pstr = static_cast<CDuiString *>(m_mSkinFile.Find(lpstrSkinFile));
		if(pstr) return FALSE;
		m_mSkinFile.Insert(lpstrSkinFile, new CDuiString(lpstrSkinFile));

		return _addPackage(lpstrSkinFile);
	}

	CLangPackageUI *CLangManagerUI::GetPackage(LPCTSTR lpstrSkinFile)
	{
		return static_cast<CLangPackageUI *>(m_mLangPackage.Find(lpstrSkinFile));
	}
	
	void CLangManagerUI::SetLanguage(LPCTSTR lpstrPath, LPCTSTR lpstrname) //设置语言包文件夹
	{
		s_sLangPath = lpstrPath;
		s_sLangName = lpstrname;
		LoadStringTable();
	}

	CDuiString CLangManagerUI::GetLangPath()
	{
		return s_sLangPath.GetData();
	}

	CDuiString CLangManagerUI::GetLangName()
	{
		return s_sLangName;
	}

	BOOL CLangManagerUI::LoadStringTable()
	{
		ReleaseStringTable();
		CDuiString sPath = CLangManagerUI::GetLangPath();
		if(sPath.GetAt(sPath.GetLength()-1) != '\\')	sPath += '\\';
		sPath += _T("StringTable.lng");

		STRINGorID xml(sPath.GetData());
		return LoadStringTableResource(xml, NULL);
	}

	BOOL CLangManagerUI::LoadStringTableResource(STRINGorID xml, LPCTSTR type)
	{
		CXmlDocumentUI xmlMarkup;
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !xmlMarkup.load_string(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !xmlMarkup.load_file(xml.m_lpstr) ) return NULL;
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

			if( !xmlMarkup.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hResource);
		}

		return LoadStringTableResource(xmlMarkup.root());
	}

	BOOL CLangManagerUI::LoadStringTableResource(CXmlNodeUI root)
	{
		if( !root ) return FALSE;

		CXmlNodeUI nodeLanguage = root.child(_T("Language"));
		if(!nodeLanguage) return FALSE;

		//加载StringTable
		for( CXmlNodeUI node = nodeLanguage.first_child() ; node; node = node.next_sibling() ) 
		{
			LPCTSTR pstrClass = node.name();
			if ((_tcsicmp(pstrClass,_T("String")) == 0))
			{
				CXmlAttributeUI attrId = node.attribute(_T("id"));
				CXmlAttributeUI attrText1 = node.attribute(_T("text1"));
				CXmlAttributeUI attrText2 = node.attribute(_T("text2"));

				if( !attrId ) continue;

				int id = attrId.as_int();
				std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
				if(it == m_mStringTable.end())
				{
					tagStringTable *st = new tagStringTable;
					st->id = id;
					st->text1 = attrText1.value();
					st->text2 = attrText2.value();
					m_mStringTable[id] = st;
				}
			}
		}
		return TRUE;
	}

	void CLangManagerUI::ReleaseStringTable()
	{
		if(m_mStringTable.size() == 0) return;
		for (std::map<int, tagStringTable *>::iterator it=m_mStringTable.begin(); it!=m_mStringTable.end(); it++)
		{
			tagStringTable *lpStr = it->second; 
			delete lpStr;
		}
		m_mStringTable.clear();
	}

	LPCTSTR CLangManagerUI::GetString(int id, LPCTSTR defaultstring)
	{
		if(m_mStringTable.size() == 0) return _T("");
		std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
		if(it == m_mStringTable.end()) return _T("");
		return it->second->text1.GetData();
	}

	LPCTSTR CLangManagerUI::GetStringExtra(int id, LPCTSTR defaultstring)
	{
		if(m_mStringTable.size() == 0) return _T("");
		std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
		if(it == m_mStringTable.end()) return _T("");
		return it->second->text2.GetData();
	}
} // namespace DuiLib