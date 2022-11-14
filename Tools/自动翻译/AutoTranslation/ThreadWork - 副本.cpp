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
			m_langCode = code;
			::GetMainWnd()->m_transBaiDu.m_to = code.sCodeBaiDu;
			::PostMessage(::GetMainWnd()->GetHWND(), WM_AUTO_SELECTED_DEST_LANGUAGE, i, 0);
			return;
		}
	}

}

void CThreadWork::Trans(CXmlDocumentUI &xml)
{
	if(::GetMainWnd()->m_b_res_xml_file)
	{
		CXmlNodeUI nodeRoot = xml.child(_T("Language"));
		if(nodeRoot)
		{
			nodeRoot.attribute_auto(_T("isocode")).set_value(m_langCode.sISOcode);
		}

		for (CXmlNodeUI node=nodeRoot.first_child(); node; node=node.next_sibling())
		{
			CXmlAttributeUI attrText = node.attribute(_T("text"));
			CXmlAttributeUI attrTooltip = node.attribute(_T("tooltip"));
			CXmlAttributeUI attrTipValue = node.attribute(_T("tipvalue"));

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

		::SendMessage(::GetMainWnd()->GetHWND(), WM_TRANSLATE_COMPLETION, (WPARAM)&xml, 0);
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