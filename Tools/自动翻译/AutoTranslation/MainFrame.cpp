#include "StdAfx.h"
#include "MainFrame.h"

#include "ThreadWork.h"
#include "DlgCreateLangPackage.h"
#include "DlgLoginBaiDu.h"

CMainFrame::CMainFrame(void)
{
	m_b_res_text = FALSE;
	m_b_res_xml_file = FALSE;
	m_b_res_auto = TRUE;

	m_b_dest_language_none = FALSE;
	m_b_dest_language_auto = TRUE;

	m_b_auto_translate_and_save_file = TRUE;
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	UI_BINDCONTROL(CComboExUI, m_cb_translation_tool, _T("cb_translation_tool"));
	UI_BINDCONTROL(CComboExUI, m_cb_source_language, _T("cb_source_language"));
	UI_BINDCONTROL(CComboExUI, m_cb_dest_language, _T("cb_dest_language"));
	UI_BINDCONTROL(CRichEditUI, m_edit_source_text, _T("edit_source_text"));
	UI_BINDCONTROL(CRichEditUI, m_edit_dest_text, _T("edit_dest_text"));

	ddxSetManager(GetManager());
	ddxCheckBox(_T("opt_res_text"), m_b_res_text);
	ddxCheckBox(_T("opt_res_xml_file"), m_b_res_xml_file);
	ddxCheckBox(_T("opt_res_auto"), m_b_res_auto);

	ddxCheckBox(_T("opt_auto_translate_and_no_save_file"), m_b_auto_translate_and_no_save_file);
	ddxCheckBox(_T("opt_auto_translate_and_save_file"), m_b_auto_translate_and_save_file);

	ddxCheckBox(_T("opt_dest_language_none"), m_b_dest_language_none);
	ddxCheckBox(_T("opt_dest_language_auto"), m_b_dest_language_auto);

	m_cb_translation_tool->AddString(_T("百度翻译"), 1);
	m_cb_translation_tool->SetCurSelFromItemData(1);

	m_arrLangCode.push_back(tagLanguageCode(_T("阿拉伯语"), _T("Arabic"),				_T("ara"),	_T("ar")));
	m_arrLangCode.push_back(tagLanguageCode(_T("保加利亚语"), _T("Bulgarian"),			_T("bul"),	_T("bg")));
	m_arrLangCode.push_back(tagLanguageCode(_T("简体中文"), _T("ChineseSimplified"),		_T("zh"),	_T("zh-cn")));
	m_arrLangCode.push_back(tagLanguageCode(_T("繁体中文"), _T("ChineseTraditional"),	_T("cht"),	_T("zh-tw")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("捷克语"), _T("Czech"),					_T("cs"),	_T("cs")));		
	m_arrLangCode.push_back(tagLanguageCode(_T("丹麦语"), _T("Danish"),					_T("dan"),	_T("da")));
	m_arrLangCode.push_back(tagLanguageCode(_T("荷兰语"), _T("Dutch"),					_T("nl"),	_T("nl")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("英语"), _T("English"),					_T("en"),	_T("en")));
	m_arrLangCode.push_back(tagLanguageCode(_T("爱沙尼亚语"), _T("Estonian"),			_T("est"),	_T("et")));
	m_arrLangCode.push_back(tagLanguageCode(_T("芬兰语"), _T("Finnish"),					_T("fin"),	_T("fi")));
	m_arrLangCode.push_back(tagLanguageCode(_T("法语"), _T("French"),					_T("fra"),	_T("fr")));
	m_arrLangCode.push_back(tagLanguageCode(_T("德语"), _T("German"),					_T("de"),	_T("de")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("希腊语"), _T("Greek"),					_T("el"),	_T("el")));		
	m_arrLangCode.push_back(tagLanguageCode(_T("匈牙利语"), _T("Hungarian"),				_T("hu"),	_T("hu")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("意大利语"), _T("Italian"),				_T("it"),	_T("it")));
	m_arrLangCode.push_back(tagLanguageCode(_T("日语"), _T("Japanese"),					_T("jp"),	_T("ja")));
	m_arrLangCode.push_back(tagLanguageCode(_T("韩语"), _T("Korean"),					_T("kor"),	_T("ko")));
	m_arrLangCode.push_back(tagLanguageCode(_T("波兰语"), _T("Polish"),					_T("pl"),	_T("pl")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("葡萄牙语"), _T("Portuguese"),			_T("pt"),	_T("pt")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("罗马尼亚语"), _T("Romanian"),			_T("rom"),	_T("ro")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("俄语"), _T("Russian"),					_T("ru"),	_T("ru")));
	m_arrLangCode.push_back(tagLanguageCode(_T("斯洛文尼亚语"), _T("Slovenian"),			_T("slo"),	_T("sl")));
	m_arrLangCode.push_back(tagLanguageCode(_T("西班牙语"), _T("Spanish"),				_T("spa"),	_T("es")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("瑞典语"), _T("Swedish"),					_T("swe"),	_T("sv")));
	m_arrLangCode.push_back(tagLanguageCode(_T("泰语"), _T("Thai"),						_T("th"),	_T("th")));	
	m_arrLangCode.push_back(tagLanguageCode(_T("越南语"), _T("Vietnamese"),				_T("vie"),	_T("vi")));	 

// 	CString sTest, temp;
// 	int xxx = 170;
	for (int i=0; i<m_arrLangCode.size(); i++)
	{
		tagLanguageCode &code = m_arrLangCode[i];
		m_cb_source_language->AddString(code.sName, i);
		m_cb_dest_language->AddString(code.sName, i);

// 		CString s = code.sNameEn;
// 		s.MakeUpper();
// 		//temp.Format(_T("#define IDR_LANG_%s\t%d\r\n"), s, xxx); 
// 		temp.Format(_T("IDR_LANG_%s\t\tXML\t\t\"res\\\\StringResource\\\\%s.xml\"\r\n"), s, code.sNameEn); 
// 		sTest += temp;
// 		xxx++;
	}
//	CMsgWndUI::InsertMsg(sTest);

	m_cb_source_language->SelectItem(_T("简体中文 ChineseSimplified"));
	m_cb_dest_language->SelectItem(_T("英语 English"));

	UpdateDataUI(false);
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_TRANSLATE_COMPLETION)
	{
		int flag = (int)lParam;

		if(flag == 0)
		{
			CXmlDocumentUI *xml = (CXmlDocumentUI *)wParam;
			if(m_sFileName.IsEmpty())
			{
				if(m_b_auto_translate_and_save_file && m_b_res_xml_file)
					xml->save_to_default_file();
				m_edit_dest_text->SetText(xml->ToString());
			}
			else
			{
				if(m_b_auto_translate_and_save_file && m_b_res_xml_file)
					xml->save_file(m_sFileName);
				m_edit_dest_text->SetText(xml->ToString());
			}
		}
		else
		{
			CString *pFileContent = (CString *)wParam;
			m_edit_dest_text->SetText(*pFileContent);
		}
		return true;
	}
	if(uMsg == WM_AUTO_SELECTED_DEST_LANGUAGE)
	{
		int sel = (int)wParam;
		m_cb_dest_language->SelectItem(sel);
	}
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{
	UI_COMMAND(_T("btn_open_file"), on_btn_open_file);
	UI_COMMAND(_T("btn_open_path"), on_btn_open_path);
	UI_COMMAND(_T("btn_start_translate"), on_btn_start_translate);
	UI_COMMAND(_T("btn_save_file"), on_btn_save_file);
	UI_COMMAND(_T("btn_create_lang_package"), on_btn_create_lang_package);
}

void CMainFrame::OnNotifyItemSelect(TNotifyUI& msg)
{
// 	if(IsControl(msg, _T("cb_source_language")))
// 	{
// 		int i = m_cb_source_language->GetCurSelItemData();
// 		UISTRING_CONVERSION;
// 		m_transBaiDu.m_from = UIT2UTF8(m_arrLangCode[i].sCodeBaiDu);
// 		return;
// 	}
// 
// 	if(IsControl(msg, _T("cb_dest_language")))
// 	{
// 		int i = m_cb_dest_language->GetCurSelItemData();
// 		UISTRING_CONVERSION;
// 		m_transBaiDu.m_to = UIT2UTF8(m_arrLangCode[i].sCodeBaiDu);
// 		return;
// 	}
}

void CMainFrame::on_btn_open_file()
{
	if(g_hThreadWorkHandle != NULL)
	{
		CMsgWndUI::InsertMsg(_T("正在翻译，请稍候。"));
		return;
	}
	UpdateDataUI(true);

	m_sFileName.Empty();
	m_sPath.Empty();
	m_arrFilePaths.RemoveAll();

	static TCHAR BASED_CODE szFilter[] = _T("所有文件(*.*)|*.*|语言包文件(*.lng)|*.lng|StringRes(*.xml)|*.xml||");
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!m_b_res_auto)
	{
		dwFlags |= OFN_ALLOWMULTISELECT;
	}
	CFileDialog fileDlg(TRUE, _T("lng"), (LPCTSTR)NULL, dwFlags,szFilter);
	fileDlg.m_ofn.lStructSize = 88;
	fileDlg.m_ofn.nMaxFile = 1000 * MAX_PATH;

	CBufferUI buff;
	buff.AddBuffer(NULL, fileDlg.m_ofn.nMaxFile * sizeof(TCHAR));
	fileDlg.m_ofn.lpstrFile = (LPTSTR)buff.GetBuffer();//设定一个文件名缓存

	if(fileDlg.DoModal() != IDOK)	
	{
		return;
	}


	POSITION pos = fileDlg.GetStartPosition();
	while (pos != NULL)
		m_arrFilePaths.Add(fileDlg.GetNextPathName(pos));

	if(m_arrFilePaths.GetSize() == 1)
	{
		CString sSingleFile = m_arrFilePaths[0];
		if(!m_xmlSource.load_file(sSingleFile))
			return;

		CLabelUI *p_label_file_name = static_cast<CLabelUI *>(GetManager()->FindControl(_T("label_file_name")));
		p_label_file_name->SetText(sSingleFile);

		m_sFileName = sSingleFile;
		m_edit_source_text->SetText(m_xmlSource.ToString());
		return;
	}
	else
	{
		CDuiString s;
		for (int i=0; i<m_arrFilePaths.GetSize(); i++)
		{
			s += m_arrFilePaths[i];
			s += _T("\r\n");
		}
		m_edit_source_text->SetText(s);
	}
}

static int CALLBACK BrowserCallbackProc(HWND hWnd,UINT   uMsg, LPARAM   lParam, LPARAM   lpData)  
{  
	switch(uMsg)  
	{  
	case   BFFM_INITIALIZED: //设置默认选择的文件夹  
		::SendMessage(hWnd,BFFM_SETSELECTION,1,lpData);  
		break;  
	default:  
		break;  
	}  
	return   0;  
} 

void CMainFrame::on_btn_open_path()
{
	if(g_hThreadWorkHandle != NULL)
	{
		CMsgWndUI::InsertMsg(_T("正在翻译，请稍候。"));
		return;
	}
	UpdateDataUI(true);

	m_sFileName.Empty();
	m_sPath.Empty();
	m_arrFilePaths.RemoveAll();

	CString strPath;
	GetCurrentDirectory(MAX_PATH, strPath.GetBuffer(MAX_PATH));
	strPath.ReleaseBuffer();

	TCHAR szDir[MAX_PATH];  
	BROWSEINFO bi;   
	bi.hwndOwner      = GetHWND();  
	bi.pidlRoot       = NULL;  
	bi.pszDisplayName = NULL;   
	bi.lpszTitle      = TEXT("选择图象保存地址");   
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;  
	bi.lpfn           = BrowserCallbackProc;   
	bi.lParam         = (LPARAM)(LPCTSTR)strPath;  
	bi.iImage         = 0;   

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
	if (pidl == NULL) return;  

	if (!SHGetPathFromIDList(pidl, szDir))  
		return;

	m_sPath = szDir;
	AddFileFromPath(szDir);

	CDuiString s;
	for (int i=0; i<m_arrFilePaths.GetSize(); i++)
	{
		s += m_arrFilePaths[i];
		s += _T("\r\n");
	}
	m_edit_source_text->SetText(s);
}

void CMainFrame::AddFileFromPath(LPCTSTR sPath)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(CString(sPath) + _T("\\") + ("*.*"));
	if(!bFind) return;
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		CString FileName = finder.GetFileName();
		if(finder.IsDirectory())
		{
			AddFileFromPath(finder.GetFilePath());
			continue;
		}

		m_arrFilePaths.Add(finder.GetFilePath());
	}
}

void CMainFrame::on_btn_start_translate()
{
	if(g_hThreadWorkHandle != NULL)
	{
		CMsgWndUI::InsertMsg(_T("正在翻译，请稍候。"));
		return;
	}
	UpdateDataUI(true);

	UISTRING_CONVERSION;

	CDlgLoginBaiDu dlg;
	if(dlg.DoModal(this) != IDOK)
		return;

	if(m_transBaiDu.m_appid.IsEmpty() || m_transBaiDu.m_key.IsEmpty())
	{
		m_transBaiDu.m_appid = UIT2A(dlg.m_sAppid);
		m_transBaiDu.m_key = UIT2A(dlg.m_sKey);
	}

	int i1 = m_cb_source_language->GetCurSelItemData();
	m_transBaiDu.m_from = UIT2UTF8(m_arrLangCode[i1].sCodeBaiDu);
	int i2 = m_cb_dest_language->GetCurSelItemData();
	m_transBaiDu.m_to = UIT2UTF8(m_arrLangCode[i2].sCodeBaiDu);


	CThreadWork *pThread = new CThreadWork;
	pThread->m_arrFileList.Copy(m_arrFilePaths);
	pThread->m_langCode = m_arrLangCode[i2];
	if(m_b_res_text)
	{
		pThread->m_sFileContent = m_edit_source_text->GetText();
	}
	pThread->CreateThread();
}

void CMainFrame::on_btn_save_file()
{
	CXmlDocumentUI xml;
	if(xml.load_string(m_edit_dest_text->GetText()))
	{
		static TCHAR BASED_CODE szFilter[] = _T("所有文件(*.*)|*.*|语言包文件(*.lng)|*.lng|StringRes(*.xml)|*.xml||");
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		CFileDialog fileDlg(FALSE, _T("lng"), (LPCTSTR)NULL, dwFlags,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	
		{
			return;
		}

		if(xml.save_file(fileDlg.GetPathName()))
			CMsgWndUI::InsertMsgV(_T("保存文件成功: %s"), m_sFileName);
	}
	else
	{
		CString temp;
		temp.Format(_T("保存文件失败: %s"), m_sFileName);
		MessageBox(NULL, temp, _T(""), MB_OK);
	}
}

void CMainFrame::on_btn_create_lang_package()
{
	CDlgCreateLangPackage dlg;
	dlg.DoModal(this);
}