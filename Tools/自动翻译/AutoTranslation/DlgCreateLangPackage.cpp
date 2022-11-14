#include "stdafx.h"
#include "DlgCreateLangPackage.h"

CDlgCreateLangPackage::CDlgCreateLangPackage()
{
	m_bFolder = TRUE;
	m_bFile = FALSE;
}

CDlgCreateLangPackage::~CDlgCreateLangPackage()
{
}

void CDlgCreateLangPackage::InitWindow()
{
	UI_BINDCONTROL(COptionUI, m_opt_folder, _T("opt_folder"));
	UI_BINDCONTROL(COptionUI, m_opt_file, _T("opt_file"));
	UI_BINDCONTROL(CEditUI, m_edit_path, _T("edit_path"));
	UI_BINDCONTROL(CEditUI, m_edit_file, _T("edit_file"));

	//从配置文件初始化控件内容
	CXmlNodeUI node = uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("DlgCreateLangPackage"));
	uiApp.Config.LoadControl(node, m_edit_path, _T("E:\\MyLibrary\\DuiLib\\bin\\skin\\lang\\ChineseSimplified"));
	uiApp.Config.LoadControl(node, m_opt_folder, TRUE);
	uiApp.Config.LoadControl(node, m_edit_file);
	uiApp.Config.LoadControl(node, m_opt_file, FALSE);

	//控件和变量的数据交换
	ddxSetManager(GetManager());
	ddxCheckBox(_T("opt_folder"), m_bFolder);
	ddxCheckBox(_T("opt_file"), m_bFile);
	ddxText(_T("edit_path"), m_sFolder);
	ddxText(_T("edit_file"), m_sFile);
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

void CDlgCreateLangPackage::OnNotifyClick(TNotifyUI& msg)
{

	if(IsControl(msg, _T("btn_select_path")))
	{
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
		m_edit_path->SetText(szDir);
		m_opt_folder->Selected(true);
		return;
	}


	if(IsControl(msg, _T("btn_select_file")))
	{
		static TCHAR BASED_CODE szFilter[] = _T("所有文件(*.*)|*.*|语言包文件(*.lng)|*.lng|StringRes(*.xml)|*.xml||");
		//CFileDialog fileDlg(FALSE, _T("lng"), GetUIManager()->GetManager()->GetResourcePath() + (LPCTSTR)file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		CFileDialog fileDlg(TRUE, _T(""), (LPCTSTR)NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,szFilter);
		fileDlg.m_ofn.lStructSize = 88;

		if(fileDlg.DoModal() != IDOK)	
		{
			return;
		}
		m_edit_file->SetText(fileDlg.GetPathName());
		m_opt_file->Selected(true);
		return;
	}
}

void CDlgCreateLangPackage::OnClickOK()
{
	UpdateDataUI(true);

	//保存控件内容到配置文件
	CXmlNodeUI node = uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("DlgCreateLangPackage"));
	uiApp.Config.SaveControl(node, m_edit_path);
	uiApp.Config.SaveControl(node, m_opt_folder);
	uiApp.Config.SaveControl(node, m_edit_file);
	uiApp.Config.SaveControl(node, m_opt_file);

	// 从一份中文的语言包复制，制造每种语言的语言包。
	// 两种情况：
	// 1，每个语言包有自己的文件夹。约定必须是语言的英文名称，如：ChineseSimplified, English...
	// 2，每种语言包只存在一个文件夹，约定必须是语言的英文名称，比如res下放置的stringtable, 如ChineseSimplified.xml, English.xml。

	//////////////////////////////////////////////////////////////////////////
	//第一种情况。
	if(m_bFolder)
	{
		CString strBasePath = m_sFolder;
		PathAddBackslash(strBasePath.GetBuffer(MAX_PATH)); //加上尾部斜杠
		strBasePath.ReleaseBuffer();
		PathRemoveFileSpec(strBasePath.GetBuffer()); //去掉尾部斜杠
		strBasePath.ReleaseBuffer();

		CString sFolderName = strBasePath; //当前文件夹名字
		PathStripPath(sFolderName.GetBuffer());
		sFolderName.ReleaseBuffer();

		CString sParentPath = strBasePath.Left(strBasePath.GetLength()-sFolderName.GetLength()); //父文件夹目录

		PathAddBackslash(strBasePath.GetBuffer(MAX_PATH)); //加上尾部斜杠，嗯，加回去
		strBasePath.ReleaseBuffer();

		for (int i=0; i<GetMainWnd()->m_arrLangCode.size(); i++)
		{
			tagLanguageCode &code = GetMainWnd()->m_arrLangCode[i];

			CString newPath = CString(sParentPath) + code.sNameEn + _T('\\');
			if(newPath == strBasePath)
				continue;

// 			if(!PathFileExists(newPath))
// 			{
// 				CreateDirectory(newPath, NULL);
// 			}

			CopyFromPath(sFolderName, code.sNameEn, sParentPath);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//第二种情况。
	if(m_bFile)
	{
		CString strBaseFile = m_sFile;

		CString sFileFolder = strBaseFile;
		PathRemoveFileSpec(sFileFolder.GetBuffer());
		sFileFolder.ReleaseBuffer();
		if(sFileFolder.GetAt(sFileFolder.GetLength()-1) != _T('\\')) sFileFolder += _T('\\');

		for (int i=0; i<GetMainWnd()->m_arrLangCode.size(); i++)
		{
			tagLanguageCode &code = GetMainWnd()->m_arrLangCode[i];

			CString newFile = sFileFolder + code.sNameEn + _T(".xml");
			if(strBaseFile == newFile)
				continue;

			CFileFind finder;
			if(!finder.FindFile(newFile))
			{
				if(::CopyFile(strBaseFile, newFile, FALSE))
				{
					CMsgWndUI::InsertMsgV(_T("Copy new file: %s"), newFile);
				}
			}
			else //如果文件已经存在，只更新内容，不能直接覆盖
			{
				UpdateXmlFile(strBaseFile, newFile);
			}
		}
	}

	MessageBox(GetMainWnd()->GetHWND(), _T("任务完成"), _T(""), MB_OK);
	__super::OnClickOK();
}

void CDlgCreateLangPackage::CopyFromPath(LPCTSTR sBaseFolder, LPCTSTR newFolder, LPCTSTR sParentPath)
{
	if(!PathFileExists(CString(sParentPath) + newFolder))
	{
		CreateDirectory(CString(sParentPath) + newFolder, NULL);
	}

	CFileFind finder;
	BOOL bFind = finder.FindFile(CString(sParentPath) + sBaseFolder + ("\\*.*"));
	if(!bFind) return;
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if(finder.IsDots())
			continue;

		CString FileName = finder.GetFileName();
		CString FilePath = finder.GetFilePath();
		if(finder.IsDirectory())
		{
			//取出相对路径
			CString s = CString(sParentPath) + sBaseFolder + ("\\");
			CString s1 = finder.GetFilePath();
			int n = s1.Find(s);
			CString s2 = s1.Right(s1.GetLength()-s.GetLength());
			CopyFromPath(CString(sBaseFolder) + _T('\\') + s2, CString(newFolder)+ _T('\\') + s2, sParentPath);
			continue;
		}

		CString sNewFilePath = CString(sParentPath) + newFolder + _T('\\') + FileName;
		if(!PathFileExists(sNewFilePath))
		{
			if(::CopyFile(finder.GetFilePath(), sNewFilePath, FALSE))
			{
				CMsgWndUI::InsertMsgV(_T("Copy new file: %s"), sNewFilePath);
			}
		}
		else //如果文件已经存在，只更新内容，不能直接覆盖
		{
			UpdateXmlFile(finder.GetFilePath(), sNewFilePath);
		}		
	}	
}

void CDlgCreateLangPackage::UpdateXmlFile(LPCTSTR sBaseFile, LPCTSTR sNewFile)
{
	CXmlDocumentUI xmlBase;
	xmlBase.load_file(sBaseFile);

	CXmlDocumentUI xmlNew;
	xmlNew.load_file(sNewFile);
	for (CXmlNodeUI nodeBase=xmlBase.child(_T("Language")).first_child(); nodeBase; nodeBase=nodeBase.next_sibling())
	{
		CDuiString name = nodeBase.name();
		CDuiString id = nodeBase.attribute(_T("id")).as_string();

		CXmlNodeUI nodeNew = xmlNew.child_auto(_T("Language")).find_child_by_attribute(name, _T("id"), id);
		if(!nodeNew)
		{
			nodeNew = xmlNew.child_auto(_T("Language")).append_copy(nodeBase);
			CMsgWndUI::InsertMsgV(_T("Append node: name=%s, id=%s. %s."), name, id, sNewFile);
		}					
	}
	xmlNew.save_to_default_file();
}