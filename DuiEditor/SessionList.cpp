#include "StdAfx.h"
#include "DuiEditor.h"
#include "SessionList.h"

CSessionList g_session;

CSessionList::CSessionList(void)
{
}

CSessionList::~CSessionList(void)
{
}

void CSessionList::Init()
{
	xml.load_file(g_strAppPath + _T("session.xml"), pugi::parse_full);
}

BOOL CSessionList::Save()
{
	return xml.save_to_default_file();
}

xml_node CSessionList::GetSession(LPCTSTR lpszPathName)
{
	if(!lpszPathName || *lpszPathName=='\0') return xml_node();
	xml_node nodeFile;

	xml_node nodeSession = xml.child_auto("Session");
	for (xml_node node = nodeSession.child("File"); node; node=node.next_sibling("File"))
	{
		if(CompareString(LSUTF82T(node.attribute("filename").as_string()), lpszPathName))
		{
			nodeFile = node;
		}
	}

	if(!nodeFile)
	{
		nodeFile = nodeSession.append_child("File");
		nodeFile.attribute_auto("filename").set_value(LST2UTF8(lpszPathName));
		Save();
	}

	xml_attribute attrBackup = nodeFile.attribute_auto("backup");
	CString backupfile = LSUTF82T(attrBackup.as_string());
	if(backupfile.IsEmpty())
	{
		// always capture the complete file name including extension (if present)
		LPTSTR lpszTemp = (LPTSTR)lpszPathName;
		for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// remember last directory/drive separator
			if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
				lpszTemp = (LPTSTR)_tcsinc(lpsz);
		}
		CString fileTitle = lpszTemp;

		SYSTEMTIME st;
		GetLocalTime(&st);
		CString file;
		file.Format(_T("%s %04d-%02d-%02d %02d.%02d.%02d.%03d.xml"), fileTitle, 
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		backupfile = g_strAppPath + _T("backup\\");
		CreateDirectory(backupfile,NULL);
		backupfile += file;

		attrBackup.set_value(LST2UTF8(backupfile));
		Save();
	}

	return nodeFile;
}

CString CSessionList::GetSessionFile(LPCTSTR lpszPathName)
{
	CString strFileName = lpszPathName;
	xml_node fileSession = GetSession(lpszPathName);

	//对比时间戳，判断是否载入备份文件
	BOOL bModify = fileSession.attribute("ismodify").as_bool();
	BOOL bSafeClose = fileSession.attribute_auto(XTEXT("safeclose")).as_bool();
	CString backup = LSUTF82T(fileSession.attribute("backup").as_string());
	if(!backup.IsEmpty() && PathFileExists(backup))
	{
		CFileStatus sta1;
		CFileStatus sta2;
		if(CFile::GetStatus(lpszPathName, sta1, NULL) && CFile::GetStatus(backup, sta2, NULL) && bModify && !bSafeClose)
		{
			if(sta1.m_mtime > sta2.m_mtime)
			{
				CString temp;
				temp.Format(_T("%s\r\n\r\n上次关闭软件没有保存文件，但是当前文件比备份新。 点\"是\"载入备份文件，点\"否\"载入当前文件。"), lpszPathName);
				if(AfxMessageBox(temp, MB_OKCANCEL) == IDOK)
				{
					strFileName = backup;
				}
				else //如果不载入备份文件，备份文件留着，创建新的
				{
					fileSession.attribute("ismodify").set_value(false);
					fileSession.attribute("backup").set_value("");
					Save();
				}
			}
			else if(sta1.m_mtime < sta2.m_mtime)
			{
				CString temp;
				temp.Format(_T("%s\r\n\r\n由于上次关闭软件没有保存文件，是否选择从备份载入？"), lpszPathName);
				if(AfxMessageBox(temp, MB_OKCANCEL) == IDOK)
				{
					strFileName = backup;
				}
				else //如果不载入备份文件，备份文件留着，创建新的
				{
					fileSession.attribute("ismodify").set_value(false);
					fileSession.attribute("backup").set_value("");
					Save();
				}
			}
		}
	}

	return strFileName;
}

CString CSessionList::GetSessionBackup(LPCTSTR lpszPathName)
{
	xml_node node = GetSession(lpszPathName);
	if(!node) return _T("");

	if(node.attribute("ismodify").as_bool() == false)
	{
		node.attribute_auto("ismodify").set_value(true);
		Save();
	}
	return LSUTF82T(node.attribute("backup").as_string());
}

void CSessionList::DeleteSession(LPCTSTR lpszPathName)
{
	if(!lpszPathName || *lpszPathName=='\0') return;

	xml_node nodeSession = xml.child_auto("Session");
	for (xml_node node = nodeSession.child("File"); node; node=node.next_sibling("File"))
	{
		if(CompareString(LSUTF82T(node.attribute("filename").as_string()), lpszPathName))
		{
			//删除备份文件
			CString backup = LSUTF82T(node.attribute("backup").as_string());
			if(!backup.IsEmpty()) 
				::DeleteFile(backup);
			int err = GetLastError();

			//删除File节点
			nodeSession.remove_child(node);
			Save();
			return;
		}
	}
}

void CSessionList::OpenSession(LPCTSTR lpszPathName)
{
	xml_node fileSession = GetSession(lpszPathName);
	if(fileSession)
	{
		fileSession.attribute_auto(XTEXT("safeclose")).set_value(false);
		fileSession.attribute_auto(XTEXT("ismodify")).set_value(false);
		Save();
	}
}

//正常关闭文件
void CSessionList::CloseSession(LPCTSTR lpszPathName)
{
	xml_node fileSession = GetSession(lpszPathName);
	if(fileSession)
	{
		fileSession.attribute_auto(XTEXT("safeclose")).set_value(true);
		Save();
	}
}

void CSessionList::ProcessSessionFileList()
{
	xml_node nodeSession = xml.child_auto("Session");

	BOOL bMoveNext = TRUE;
	xml_node node=nodeSession.child("File");
	while (node)
	{
		CString filename = LSUTF82T(node.attribute("filename").as_string());
		if(PathFileExists(filename))
		{		
			CDocTemplate* pTemplate;
			POSITION pos = AfxGetApp()->m_pDocManager->GetFirstDocTemplatePosition();
			while(pos != NULL)
			{
				pTemplate = AfxGetApp()->m_pDocManager->GetNextDocTemplate(pos);
				CDocument *pDocument = NULL;
				CDocTemplate::Confidence match = pTemplate->MatchDocType(filename, pDocument);
				if(match == CDocTemplate::yesAlreadyOpen) 
				{
					//这个文件已经打开了，可能双击打开的，可能出错了。
					break;
				}
				if(match == CDocTemplate::yesAttemptNative)
				{
					pTemplate->OpenDocumentFile(filename, TRUE, TRUE);
					break;
				}
			}
		}
		else
		{
			//文件不存在，删除这个节点
			xml_node nodeTemp = node.next_sibling("File");
			nodeSession.remove_child(node);
			bMoveNext = FALSE;
			node = nodeTemp;
			Save();
		}

		if(bMoveNext)
			node=node.next_sibling("File");
		bMoveNext = TRUE;
	}
}