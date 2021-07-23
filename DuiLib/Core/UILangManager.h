#ifndef __UILANGUAGEMANAGER_H__
#define __UILANGUAGEMANAGER_H__
#pragma once

namespace DuiLib {
	class UILIB_API CLangPackageUI
	{
	public:
		CLangPackageUI();
		~CLangPackageUI();
	
	public:
		void SetSkinFile(LPCTSTR lpstrSkinFile);
		LPCTSTR GetSkinFile();
		BOOL LoadResource(STRINGorID xml, LPCTSTR type);
		BOOL LoadResource(CXmlNodeUI Root);
		void ResetResource();

		BOOL AddText(int resid, LPCTSTR lpstrText);
		LPCTSTR GetText(int resid);

		BOOL AddToolTip(int resid, LPCTSTR lpstrText);
		LPCTSTR GetToolTip(int resid);

		BOOL AddTipValue(int resid, LPCTSTR lpstrText);
		LPCTSTR GetTipValue(int resid);
	private:
		CDuiString m_sSkinFile;
		std::map<int, CDuiString *> m_mText;
		std::map<int, CDuiString *> m_mToolTip;
		std::map<int, CDuiString *> m_mTipValue;
	private:
	};

	class UILIB_API CLangManagerUI
	{
	protected:
		struct tagStringTable 
		{
			int id;
			CDuiString text1;
			CDuiString text2;
		};
	public:
		CLangManagerUI(void);
		~CLangManagerUI(void);

		void ReloadLanguage();
		CLangPackageUI *AddPackage(LPCTSTR lpstrSkinFile);
		CLangPackageUI *GetPackage(LPCTSTR lpstrSkinFile);
	
		static void SetLanguage(LPCTSTR lpstrPath, LPCTSTR lpstrname); //设置语言包文件夹
		static CDuiString GetLangPath();
		static CDuiString GetLangName();

		static void ReleaseStringTable();
		static LPCTSTR GetString(int id, LPCTSTR defaultstring = NULL);
		static LPCTSTR GetStringExtra(int id, LPCTSTR defaultstring = NULL);
	protected:
		void ResetPackage();
		CLangPackageUI *_addPackage(LPCTSTR lpstrSkinFile);
		static BOOL LoadStringTable();
		static BOOL LoadStringTableResource(STRINGorID xml, LPCTSTR type);
		static BOOL LoadStringTableResource(CXmlNodeUI root);
	private:
		CStdStringPtrMap m_mLangPackage;	//LangPackage
		CStdStringPtrMap m_mSkinFile;		//所有已加载的xml
		static CDuiString s_sLangPath;
		static CDuiString s_sLangName;
		static std::map<int, tagStringTable *> m_mStringTable;
	};

} // namespace DuiLib

#endif // __UILANGUAGEMANAGER_H__