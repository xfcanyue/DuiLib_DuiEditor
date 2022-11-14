#pragma once

#include "TransBaiDu.h"

struct tagLanguageCode 
{
	tagLanguageCode(LPCTSTR nameChs, LPCTSTR nameEn, LPCTSTR baiducode, LPCTSTR isocode)
	{
		sNameChs = nameChs;
		sNameEn = nameEn;
		sName = sNameChs + _T(" ") + nameEn;
		sCodeBaiDu = baiducode;
		sISOcode = isocode;
	}
	CDuiString sName;	
	CDuiString sNameChs;
	CDuiString sNameEn;	
	CDuiString sCodeBaiDu;	//百度翻译的语言代号
	CDuiString sISOcode;	//ISO语言代码
};

class CMainFrame : public CUIFrameWnd, public CUIDataExchange
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile() 			{ return _T("MainFrame.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	virtual void InitWindow();
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnMenuCommand(const MenuCmd *cmd);
	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI);
	virtual void OnNotifyClick(TNotifyUI& msg);
	virtual void OnNotifyItemSelect(TNotifyUI& msg);

	void on_btn_open_file();

	void on_btn_open_path();
	void AddFileFromPath(LPCTSTR sPath);

	void on_btn_start_translate();
	void on_btn_save_file();

	void on_btn_create_lang_package();

	//CString Translate_BaiDu(LPCTSTR sSource);

	CComboExUI *m_cb_translation_tool;
	CComboExUI *m_cb_source_language;
	CComboExUI *m_cb_dest_language;
	CRichEditUI *m_edit_source_text;
	CRichEditUI *m_edit_dest_text;

	CDuiString m_sFileName;
	CDuiString m_sPath;
	CStringArray m_arrFilePaths;//将文件路径保存到CStringArray对象中

	CXmlDocumentUI m_xmlSource;


	BOOL m_b_res_text;
	BOOL m_b_res_xml_file;
	BOOL m_b_res_auto;

	BOOL m_b_auto_translate_and_no_save_file;
	BOOL m_b_auto_translate_and_save_file;

	BOOL m_b_dest_language_none;
	BOOL m_b_dest_language_auto;

	std::vector<tagLanguageCode> m_arrLangCode;
	CTransBaiDu m_transBaiDu;
};

