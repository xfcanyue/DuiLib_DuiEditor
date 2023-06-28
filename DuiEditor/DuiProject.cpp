#include "StdAfx.h"
#include "DuiEditor.h"
#include "DuiProject.h"

#include "MainFrm.h"

CDuiProject::CDuiProject(void)
{
	m_bHasLoadConfig = FALSE;
}


CDuiProject::~CDuiProject(void)
{
}

void CDuiProject::InitProject(LPCTSTR lpszPathName)
{
	CString strPath = lpszPathName;

	//往上级文件夹寻找项目配置，最多回溯5层的文件夹
	int maxLevel = 5;
	int times = 1;
	while(!CMyFilePath::IsFileExist(strPath + _T("project.dui")) && times < maxLevel)
	{
		strPath += _T("..\\");
		times++;
	}

	if(times >= maxLevel) //如果找不到配置文件，从当前目录创建新的。
	{
		strPath = lpszPathName;
	}
	else 
	{
		strPath = CMyFilePath::Canonicalize(strPath);
	}

	m_projFile.load_file(strPath + _T("project.dui"));
	xml_node node = m_projFile.child_auto(XTEXT("Project")).child_auto(XTEXT("ProjectPath"));
	CString projPath = XML2T(node.text().get());
	if(projPath.IsEmpty() || strPath.CompareNoCase(projPath) != 0)
	{
		node.text().set(T2XML(strPath));
		Save();
	}
		
	CPaintManagerUI::SetResourcePath(strPath);
	((CMainFrame *)AfxGetMainWnd())->m_wndFileView.m_wndFileView.InitFolder();
	SetCurrentDirectory(strPath);
	m_bHasLoadConfig = FALSE;
}

void CDuiProject::Save()
{
	m_projFile.save_to_default_file(PUGIXML_TEXT("\t"), format_default, encoding_utf8);
}

CString CDuiProject::GetProjectPath()
{
	return XML2T(m_projFile.child(XTEXT("Project")).child(XTEXT("ProjectPath")).text().get());
}

CString CDuiProject::GetStartupFile()
{
	return XML2T(m_projFile.child(XTEXT("Project")).child(XTEXT("Startup")).child(XTEXT("File")).text().get());
}

void CDuiProject::SetStartupFile(LPCTSTR fileName)
{
	xml_node node = m_projFile.child_auto(XTEXT("Project")).child_auto(XTEXT("Startup")).child_auto(XTEXT("File"));
	node.text().set(T2XML(fileName));
	Save();
}


void CDuiProject::LoadConfig(CPaintManagerUI* pManager)
{
	if(!pManager)	return;

	xml_document xml;
	if(!xml.load_file(GetProjectPath() + _T("config.xml")))
		return;

	pugi::xml_node root;
	for (xml_node node=xml.first_child(); node; node=node.next_sibling())
	{
		if(node.type() == node_element)
		{
			root = node;
			break;
		}
	}
	if( !root) return;

	CString pstrClass = NULL;
	int nAttributes = 0;
	CString pstrName = NULL;
	CString pstrValue = NULL;
	LPTSTR pstr = NULL;
	for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) 
	{
		if(node.type() != node_element)	continue;
		pstrClass = XML2T(node.name());
		if( _tcsicmp(pstrClass, _T("Image")) == 0 ) {
			CString pImageName = NULL;
			CString pImageResType = NULL;
			bool shared = false;
			DWORD mask = 0;
			for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
				pstrName = XML2T(attr.name());
				pstrValue = XML2T(attr.value());
				if( _tcsicmp(pstrName, _T("name")) == 0 ) {
					pImageName = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("restype")) == 0 ) {
					pImageResType = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("mask")) == 0 ) {
					if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
					mask = _tcstoul(pstrValue, &pstr, 16);
				}
				else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
					shared = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
			}
			if( pImageName ) pManager->AddImage(pImageName, pImageResType, mask, false, shared);
		}
		else if( _tcsicmp(pstrClass, _T("Font")) == 0 ) {
			int id = -1;
			CString pFontName = NULL;
			int size = 12;
			bool bold = false;
			bool underline = false;
			bool italic = false;
			bool defaultfont = false;
			bool shared = false;
			for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
				pstrName = XML2T(attr.name());
				pstrValue = XML2T(attr.value());
				if( _tcsicmp(pstrName, _T("id")) == 0 ) {
					id = _tcstol(pstrValue, &pstr, 10);
				}
				else if( _tcsicmp(pstrName, _T("name")) == 0 ) {
					pFontName = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("size")) == 0 ) {
					size = _tcstol(pstrValue, &pstr, 10);
				}
				else if( _tcsicmp(pstrName, _T("bold")) == 0 ) {
					bold = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcsicmp(pstrName, _T("underline")) == 0 ) {
					underline = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcsicmp(pstrName, _T("italic")) == 0 ) {
					italic = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcsicmp(pstrName, _T("default")) == 0 ) {
					defaultfont = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
				else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
					shared = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
			}
			if( id >= 0 ) {
				pManager->AddFont(id, pFontName, size, bold, underline, italic, shared);
				if( defaultfont ) pManager->SetDefaultFont(pFontName, pManager->GetDPIObj()->ScaleInt(size), bold, underline, italic, shared);

			}
		}
		else if( _tcsicmp(pstrClass, _T("Default")) == 0 ) {
			CString pControlName = NULL;
			CString pControlValue = NULL;
			bool shared = false;
			for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
				pstrName = XML2T(attr.name());
				pstrValue = XML2T(attr.value());
				if( _tcsicmp(pstrName, _T("name")) == 0 ) {
					pControlName = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("value")) == 0 ) {
					pControlValue = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
					shared = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
			}
			if( pControlName ) {
				pManager->AddDefaultAttributeList(pControlName, pControlValue, shared);
			}
		}
		else if( _tcsicmp(pstrClass, _T("Style")) == 0 ) {
			CString pName = NULL;
			CString pStyle = NULL;
			bool shared = false;
			for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
				pstrName = XML2T(attr.name());
				pstrValue = XML2T(attr.value());
				if( _tcsicmp(pstrName, _T("name")) == 0 ) {
					pName = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("value")) == 0 ) {
					pStyle = pstrValue;
				}
				else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
					shared = (_tcsicmp(pstrValue, _T("true")) == 0);
				}
			}
			if( pName ) {
				pManager->AddStyle(pName, pStyle, shared);

			}
		}
		else if (_tcsicmp(pstrClass, _T("Import")) == 0) {
			CString pstrPath = NULL;
			for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
				pstrName = XML2T(attr.name());
				pstrValue = XML2T(attr.value());
				if (_tcsicmp(pstrName, _T("fontfile")) == 0) {
					pstrPath = pstrValue;
				}
			}
			if (pstrPath) {
				pManager->AddFontArray(pstrPath);

			}
		}
	}

	m_bHasLoadConfig = TRUE;
}

BOOL CDuiProject::IsLoadedConfig()
{
	return m_bHasLoadConfig;
}