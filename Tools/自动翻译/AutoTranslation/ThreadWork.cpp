// ThreadWork.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoTranslation.h"
#include "ThreadWork.h"

#include  "direct.h" //makedir need

// CThreadWork
DWORD g_nThreadWorkID = 0;
HANDLE g_hThreadWorkHandle = NULL;

IMPLEMENT_DYNCREATE(CThreadWork, CWinThread)

CThreadWork::CThreadWork() : m_langCode(_T(""), _T(""), _T(""), _T(""))
{
	m_bTerminateExit = FALSE;
}

CThreadWork::~CThreadWork()
{
}

BOOL CThreadWork::InitInstance()
{
	g_nThreadWorkID = m_nThreadID;
	g_hThreadWorkHandle = m_hThread;
	//////////////////////////////////////////////////////////////////////////
	if(::GetMainWnd()->m_b_res_text)
	{
		CString sDst;
		::GetMainWnd()->m_transBaiDu.Translate(m_sFileContent, sDst);
		::SendMessage(::GetMainWnd()->GetHWND(), WM_TRANSLATE_COMPLETION, (WPARAM)&sDst, 1);
	}
	else if(::GetMainWnd()->m_b_res_auto)
	{
		if(!::GetMainWnd()->m_sPath.IsEmpty())
		{
			CopyFromPath(::GetMainWnd()->m_sPath);
		}
		else if(!::GetMainWnd()->m_sFileName.IsEmpty())
		{
			CopyFromFile(::GetMainWnd()->m_sFileName);
		}
	}
	else
	{
		for (int i=0; i<m_arrFileList.GetSize(); i++)
		{
			CXmlDocumentUI xml;
			AutoSelectDestLanguage(m_arrFileList[i]);
			if(!xml.load_file(m_arrFileList[i]))
			{
				MessageBox(NULL, xml.GetError(), _T(""), MB_OK);
			}
			else
			{
				Trans(xml);
			}
		}
	}

	CMsgWndUI::InsertMsgV(_T("翻译完成"));
	return FALSE;
}

void CThreadWork::AutoSelectDestLanguage(LPCTSTR lpszFileName)
{
	if(::GetMainWnd()->m_b_dest_language_none)
		return;

	CString sFilePath = lpszFileName;

	//获得文件名，不包括扩展名
	CString sFileName = lpszFileName;
	PathStripPath(sFileName.GetBuffer());
	sFileName.ReleaseBuffer();
	PathRemoveExtension(sFileName.GetBuffer());
	sFileName.ReleaseBuffer();

	//获得所在目录
	CString sFileFolder = lpszFileName;
	PathRemoveFileSpec(sFileFolder.GetBuffer());
	sFileFolder.ReleaseBuffer();
	PathStripPath(sFileFolder.GetBuffer());
	sFileFolder.ReleaseBuffer();

	for (int i=0; i<::GetMainWnd()->m_arrLangCode.size(); i++)
	{
		tagLanguageCode &code = ::GetMainWnd()->m_arrLangCode[i];
		
		if((code.sName.Find(sFileName) >= 0) || code.sName.Find(sFileFolder) >= 0)
		{
			if(m_langCode.sISOcode != code.sISOcode)
			{
				m_langCode = code;
				::GetMainWnd()->m_transBaiDu.m_to = m_langCode.sCodeBaiDu;
				::PostMessage(::GetMainWnd()->GetHWND(), WM_AUTO_SELECTED_DEST_LANGUAGE, i, 0);
			}
			return;
		}
	}

}

void CThreadWork::Trans(CXmlDocumentUI &xml)
{
	CXmlNodeUI nodeRoot = xml.child(_T("Language"));
	if(nodeRoot)
	{
		nodeRoot.attribute_auto(_T("isocode")).set_value(m_langCode.sISOcode);
	}

	for (CXmlNodeUI node=nodeRoot.first_child(); node; node=node.next_sibling())
	{
		TransNode(node);
	}

	::SendMessage(::GetMainWnd()->GetHWND(), WM_TRANSLATE_COMPLETION, (WPARAM)&xml, 0);	
}

void CThreadWork::TransNode(CXmlNodeUI node)
{
	CXmlAttributeUI attrText = node.attribute(_T("text"));
	CXmlAttributeUI attrTooltip = node.attribute(_T("tooltip"));
	CXmlAttributeUI attrTipValue = node.attribute(_T("tipvalue"));

	::GetMainWnd()->m_transBaiDu.m_to = m_langCode.sCodeBaiDu;

	if(attrText)
	{
		CString sSrc = attrText.as_string();
		CString sDst;
		::GetMainWnd()->m_transBaiDu.Translate(sSrc, sDst);
		attrText.set_value(sDst);
	}
	if(attrTooltip)
	{
		CString sSrc = attrTooltip.as_string();
		CString sDst;
		::GetMainWnd()->m_transBaiDu.Translate(sSrc, sDst);
		attrTooltip.set_value(sDst);
	}
	if(attrTipValue)
	{
		CString sSrc = attrTipValue.as_string();
		CString sDst;
		::GetMainWnd()->m_transBaiDu.Translate(sSrc, sDst);
		attrTipValue.set_value(sDst);
	}
}

int CThreadWork::ExitInstance()
{
	g_nThreadWorkID = 0;
	g_hThreadWorkHandle = NULL;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadWork, CWinThread)
END_MESSAGE_MAP()

void CThreadWork::EndWork()
{
	m_bTerminateExit = TRUE;
}

void CThreadWork::CopyFromPath(LPCTSTR sPath)
{
	CString strBasePath = sPath;
	PathAddBackslash(strBasePath.GetBuffer(MAX_PATH)); //加上尾部斜杠
	strBasePath.ReleaseBuffer();
	PathRemoveFileSpec(strBasePath.GetBuffer()); //去掉尾部斜杠
	strBasePath.ReleaseBuffer(); //这样就确保路径最后没有斜杠了 -_-!

	CString sFolderName = strBasePath; //当前文件夹名字
	PathStripPath(sFolderName.GetBuffer());
	sFolderName.ReleaseBuffer();

	CString sParentPath = strBasePath.Left(strBasePath.GetLength()-sFolderName.GetLength()); //父文件夹目录

	PathAddBackslash(strBasePath.GetBuffer(MAX_PATH)); //加上尾部斜杠，嗯，加回去
	strBasePath.ReleaseBuffer();

	for (int i=0; i<::GetMainWnd()->m_arrLangCode.size(); i++)
	{
		tagLanguageCode &code = ::GetMainWnd()->m_arrLangCode[i];

		CString newPath = CString(sParentPath) + code.sNameEn.toString() + _T('\\');
		if(newPath == strBasePath)
			continue;

		m_langCode = code;
		CopyFromPath(sFolderName, code.sNameEn, sParentPath);
	}
}

void CThreadWork::CopyFromPath(LPCTSTR sBaseFolder, LPCTSTR newFolder, LPCTSTR sParentPath)
{
	if(!PathFileExists(CString(sParentPath) + newFolder))
	{
		CreateDirectory(CString(sParentPath) + newFolder, NULL);
	}

	CFileFind finder;
	BOOL bFind = finder.FindFile(CString(sParentPath) + sBaseFolder + _T("\\*.*"));
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
			CString s = CString(sParentPath) + sBaseFolder + _T("\\");
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

				CXmlDocumentUI xml;
				if(!xml.load_file(sNewFilePath))
				{
					MessageBox(NULL, xml.GetError(), _T(""), MB_OK);
				}
				else
				{
					Trans(xml);
					xml.save_to_default_file();
				}
			}
		}
		else //如果文件已经存在，只更新内容，不能直接覆盖
		{
			UpdateXmlFile(finder.GetFilePath(), sNewFilePath);
		}		
	}	
}

void CThreadWork::CopyFromFile(LPCTSTR strBaseFile)
{
	CString sFileFolder = strBaseFile;
	PathRemoveFileSpec(sFileFolder.GetBuffer());
	sFileFolder.ReleaseBuffer();
	if(sFileFolder.GetAt(sFileFolder.GetLength()-1) != _T('\\')) sFileFolder += _T('\\');

	for (int i=0; i<::GetMainWnd()->m_arrLangCode.size(); i++)
	{
		tagLanguageCode &code = ::GetMainWnd()->m_arrLangCode[i];

		CString newFile = sFileFolder + code.sNameEn.toString() + _T(".xml");
		if(strBaseFile == newFile)
			continue;

		m_langCode = code;

		CFileFind finder;
		if(!finder.FindFile(newFile))
		{
			if(::CopyFile(strBaseFile, newFile, FALSE))
			{
				CMsgWndUI::InsertMsgV(_T("Copy new file: %s"), newFile);

				CXmlDocumentUI xml;
				if(!xml.load_file(newFile))
				{
					MessageBox(NULL, xml.GetError(), _T(""), MB_OK);
				}
				else
				{
					Trans(xml);
					xml.save_to_default_file();
				}
			}
		}
		else //如果文件已经存在，只更新内容，不能直接覆盖
		{
			UpdateXmlFile(strBaseFile, newFile);
		}
	}
}

void CThreadWork::UpdateXmlFile(LPCTSTR sBaseFile, LPCTSTR sNewFile)
{
	AutoSelectDestLanguage(sNewFile);

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

			TransNode(nodeNew);
		}					
	}
	xmlNew.save_to_default_file();
}