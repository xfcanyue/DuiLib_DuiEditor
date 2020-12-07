#include "StdAfx.h"
#include "UILangManager.h"

namespace DuiLib {
	
	CLangPackage::CLangPackage()
	{

	}

	CLangPackage::~CLangPackage()
	{
		ResetResource();
	}

	void CLangPackage::SetSkinFile(LPCTSTR lpstrSkinFile)
	{
		m_sSkinFile = lpstrSkinFile;
	}

	LPCTSTR CLangPackage::GetSkinFile()
	{
		return m_sSkinFile.GetData();
	}

	BOOL CLangPackage::LoadResource(STRINGorID xml, LPCTSTR type)
	{
		CMarkup xmlMarkup;
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !xmlMarkup.Load(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !xmlMarkup.LoadFromFile(xml.m_lpstr) ) return NULL;
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

			if( !xmlMarkup.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hResource);
		}

		return LoadResource(xmlMarkup.GetRoot());
	}

	BOOL CLangPackage::LoadResource(CMarkupNode Root)
	{	
		if( !Root.IsValid() ) return FALSE;

		//加载资源
		for( CMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
		{
			LPCTSTR pstrClass = node.GetName();
			CMarkupNode ChildNode = node.GetChild();
			if(ChildNode.IsValid()) LoadResource(node);
			else if ((_tcsicmp(pstrClass,_T("Text")) == 0) && node.HasAttributes())
			{
				LPCTSTR pstrId = NULL;
				LPCTSTR pstrText = NULL;
				LPCTSTR pstrToolTip = NULL;
				LPCTSTR pstrTipValue = NULL;

				int nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) 
				{
					LPCTSTR pstrName = node.GetAttributeName(i);
					LPCTSTR pstrValue = node.GetAttributeValue(i);

					if( _tcsicmp(pstrName, _T("id")) == 0 ) 
					{
						pstrId = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("text")) == 0 ) 
					{
						pstrText = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("tooltip")) == 0 ) 
					{
						pstrToolTip = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("tipvalue")) == 0 ) 
					{
						pstrTipValue = pstrValue;
					}
				}
				if( pstrId == NULL) continue;

				if(pstrText != NULL)
					AddText(_ttoi(pstrId), pstrText);
				if(pstrToolTip != NULL)
					AddToolTip(_ttoi(pstrId), pstrToolTip);
				if(pstrTipValue != NULL)
					AddTipValue(_ttoi(pstrId), pstrTipValue);
			}
			else continue;
		}
		return TRUE;
	}

	void CLangPackage::ResetResource()
	{
		CDuiString* lpStr;
		for (std::map<int, CDuiString *>::iterator it=m_mText.begin(); it!=m_mText.end(); it++)
		{
			lpStr = it->second; delete lpStr;
		}
		m_mText.clear();

		m_mToolTip.clear();
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

	BOOL CLangPackage::AddText(int resid, LPCTSTR lpstrText)
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

	LPCTSTR CLangPackage::GetText(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mText.find(resid);
		if(it != m_mText.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}	

	BOOL CLangPackage::AddToolTip(int resid, LPCTSTR lpstrText)
	{
		std::map<int, CDuiString *>::iterator it = m_mToolTip.find(resid);
		if(it != m_mToolTip.end())
		{
			return FALSE;
		}
		m_mToolTip[resid] = new CDuiString(lpstrText);
		return TRUE;
	}

	LPCTSTR CLangPackage::GetToolTip(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mToolTip.find(resid);
		if(it != m_mToolTip.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}

	BOOL CLangPackage::AddTipValue(int resid, LPCTSTR lpstrText)
	{
		std::map<int, CDuiString *>::iterator it = m_mTipValue.find(resid);
		if(it != m_mTipValue.end())
		{
			return FALSE;
		}
		m_mTipValue[resid] = new CDuiString(lpstrText);
		return TRUE;
	}

	LPCTSTR CLangPackage::GetTipValue(int resid)
	{
		std::map<int, CDuiString *>::iterator it = m_mTipValue.find(resid);
		if(it != m_mTipValue.end())
		{
			return it->second->GetData();
		}
		return _T("");
	}

	//////////////////////////////////////////////////////////////////////////
	CDuiString CLangManager::s_sLangPath;
	CDuiString CLangManager::s_sLangName;
	std::map<int, CLangManager::tagStringTable *> CLangManager::m_mStringTable;
	CLangManager::CLangManager(void)
	{
		
	}

	CLangManager::~CLangManager(void)
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
	}

	void CLangManager::ResetPackage()
	{
		for( int i = 0; i< m_mLangPackage.GetSize(); i++ )
		{
			if(LPCTSTR key = m_mLangPackage.GetAt(i))
			{
				CLangPackage *pkg = static_cast<CLangPackage *>(m_mLangPackage.Find(key));
				delete pkg; pkg = NULL;
			}
		}
		m_mLangPackage.RemoveAll();	
	}

	CLangPackage *CLangManager::_addPackage(LPCTSTR lpstrSkinFile)
	{
		CLangPackage *pkg = static_cast<CLangPackage *>(m_mLangPackage.Find(lpstrSkinFile));
		if(pkg) return FALSE;

		pkg = new CLangPackage;
		pkg->SetSkinFile(lpstrSkinFile);

		CDuiString sPath = CLangManager::GetLangPath();
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

	void CLangManager::ReloadLanguage()
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

	CLangPackage *CLangManager::AddPackage(LPCTSTR lpstrSkinFile)
	{
		CDuiString *pstr = static_cast<CDuiString *>(m_mSkinFile.Find(lpstrSkinFile));
		if(pstr) return FALSE;
		m_mSkinFile.Insert(lpstrSkinFile, new CDuiString(lpstrSkinFile));

		return _addPackage(lpstrSkinFile);
	}

	CLangPackage *CLangManager::GetPackage(LPCTSTR lpstrSkinFile)
	{
		return static_cast<CLangPackage *>(m_mLangPackage.Find(lpstrSkinFile));
	}
	
	void CLangManager::SetLanguage(LPCTSTR lpstrPath, LPCTSTR lpstrname) //设置语言包文件夹
	{
		s_sLangPath = lpstrPath;
		s_sLangName = lpstrname;
		LoadStringTable();
	}

	CDuiString CLangManager::GetLangPath()
	{
		return s_sLangPath.GetData();
	}

	CDuiString CLangManager::GetLangName()
	{
		return s_sLangName;
	}

	BOOL CLangManager::LoadStringTable()
	{
		ReleaseStringTable();
		CDuiString sPath = CLangManager::GetLangPath();
		if(sPath.GetAt(sPath.GetLength()-1) != '\\')	sPath += '\\';
		sPath += _T("StringTable.lng");

		STRINGorID xml(sPath.GetData());
		return LoadStringTableResource(xml, NULL);
	}

	BOOL CLangManager::LoadStringTableResource(STRINGorID xml, LPCTSTR type)
	{
		CMarkup xmlMarkup;
		if( HIWORD(xml.m_lpstr) != NULL ) 
		{
			if( *(xml.m_lpstr) == _T('<') ) 
			{
				if( !xmlMarkup.Load(xml.m_lpstr) ) return NULL;
			}
			else 
			{
				if( !xmlMarkup.LoadFromFile(xml.m_lpstr) ) return NULL;
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

			if( !xmlMarkup.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) {
				return NULL;
			}
			::FreeResource(hResource);
		}

		return LoadStringTableResource(xmlMarkup.GetRoot());
	}

	BOOL CLangManager::LoadStringTableResource(CMarkupNode Root)
	{
		if( !Root.IsValid() ) return FALSE;

		//加载StringTable
		for( CMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling() ) 
		{
			LPCTSTR pstrClass = node.GetName();
			CMarkupNode ChildNode = node.GetChild();
			if(ChildNode.IsValid()) LoadStringTableResource(node);
			else if ((_tcsicmp(pstrClass,_T("String")) == 0) && node.HasAttributes())
			{
				LPCTSTR pstrId = NULL;
				LPCTSTR pstrText1 = NULL;
				LPCTSTR pstrText2 = NULL;
				LPCTSTR pstrText3 = NULL;

				int nAttributes = node.GetAttributeCount();
				for( int i = 0; i < nAttributes; i++ ) 
				{
					LPCTSTR pstrName = node.GetAttributeName(i);
					LPCTSTR pstrValue = node.GetAttributeValue(i);

					if( _tcsicmp(pstrName, _T("id")) == 0 ) 
					{
						pstrId = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("text1")) == 0 ) 
					{
						pstrText1 = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("text2")) == 0 ) 
					{
						pstrText2 = pstrValue;
					}
					else if( _tcsicmp(pstrName, _T("text3")) == 0 ) 
					{
						pstrText3 = pstrValue;
					}
				}
				if( pstrId == NULL) continue;

				int id = _ttoi(pstrId);
				std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
				if(it == m_mStringTable.end())
				{
					tagStringTable *st = new tagStringTable;
					st->id = id;
					st->text1 = pstrText1;
					st->text2 = pstrText2;
					st->text3 = pstrText3;
					m_mStringTable[id] = st;
				}
			}
			else continue;
		}
		return TRUE;
	}

	void CLangManager::ReleaseStringTable()
	{
		if(m_mStringTable.size() == 0) return;
		for (std::map<int, tagStringTable *>::iterator it=m_mStringTable.begin(); it!=m_mStringTable.end(); it++)
		{
			tagStringTable *lpStr = it->second; 
			delete lpStr;
		}
		m_mStringTable.clear();
	}

	LPCTSTR CLangManager::GetString1(int id, LPCTSTR defaultstring)
	{
		if(m_mStringTable.size() == 0) return _T("");
		std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
		if(it == m_mStringTable.end()) return _T("");
		return it->second->text1.GetData();
	}

	LPCTSTR CLangManager::GetString2(int id, LPCTSTR defaultstring)
	{
		if(m_mStringTable.size() == 0) return _T("");
		std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
		if(it == m_mStringTable.end()) return _T("");
		return it->second->text2.GetData();
	}

	LPCTSTR CLangManager::GetString3(int id, LPCTSTR defaultstring)
	{
		if(m_mStringTable.size() == 0) return _T("");
		std::map<int, tagStringTable *>::iterator it = m_mStringTable.find(id);
		if(it == m_mStringTable.end()) return _T("");
		return it->second->text3.GetData();
	}
} // namespace DuiLib