#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

CDuiString CMainFrame::GetSkinFile() 			
{ 
	return (LPCTSTR)g_strSkinFile; 
}

void CMainFrame::InitWindow()
{
	
}

//windowsԭ����Ϣ
LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:		
		DuiWriteConsole(_T("WM_CREATE"));
		LoadConfigXml();
		break;
	case WM_CLOSE:		
		DuiWriteConsole(_T("WM_CLOSE"));
		break;
	case WM_DESTROY:	
		DuiWriteConsole(_T("WM_DESTROY"));
		break;
#if defined(WIN32) && !defined(UNDER_CE)
	case WM_NCACTIVATE:		
		break;
	case WM_NCCALCSIZE:		
		break;
	case WM_NCPAINT:		
		break;
	case WM_NCHITTEST:		
		break;
	case WM_GETMINMAXINFO:	
		break;
	case WM_MOUSEWHEEL:		
		break;
#endif
	case WM_SIZE:
		break;
	case WM_CHAR:		
		//DuiWriteConsole(_T("WM_CHAR"));	
		if(VK_ESCAPE == wParam)
			Close();
		break;
	case WM_SYSCOMMAND:		
		break;
	case WM_KEYDOWN:	
		break;
	case WM_KILLFOCUS:		
		break;
	case WM_SETFOCUS:		
		break;
	case WM_LBUTTONUP:		
		DuiWriteConsole(_T("WM_LBUTTONUP"));
		break;
	case WM_LBUTTONDOWN:	
		DuiWriteConsole(_T("WM_LBUTTONDOWN"));
		break;
	case WM_RBUTTONUP:		
		break;
	case WM_RBUTTONDOWN:	
		break;
	case WM_MOUSEMOVE:	
		//DuiWriteConsole(_T("WM_MOUSEMOVE"));
		break;
	case WM_MOUSEHOVER:	
		//DuiWriteConsole(_T("WM_MOUSEHOVER"));
		break;
	default:				
		break;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	CString className = msg.pSender->GetClass();
	CString ControlName = msg.pSender->GetName();
	CString EventName = msg.sType;

	CString strText;
	strText.Format(_T("Notify: class='%s' name='%s' msg='%s'"), className, ControlName, EventName);
	DuiWriteConsole(strText);

	__super::Notify(msg);
}

void CMainFrame::LoadConfigXml()
{
	//load config.xml
	CString sConfigFile = m_pm.GetResourcePath() + _T("config.xml");
	xml_document xml;
	CPaintManagerUI *pManager = &m_pm;
	if(xml.load_file(sConfigFile))
	{
		xml_node root = xml.child(_T("Window"));
		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;
		for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) {
			if(node.type() != node_element)	continue;
			pstrClass = node.name();
			if( _tcsicmp(pstrClass, _T("Image")) == 0 ) {
				LPCTSTR pImageName = NULL;
				LPCTSTR pImageResType = NULL;
				bool shared = false;
				DWORD mask = 0;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
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
				LPCTSTR pFontName = NULL;
				int size = 12;
				bool bold = false;
				bool underline = false;
				bool italic = false;
				bool defaultfont = false;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
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
				LPCTSTR pControlName = NULL;
				LPCTSTR pControlValue = NULL;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
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
				LPCTSTR pName = NULL;
				LPCTSTR pStyle = NULL;
				bool shared = false;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
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
				LPCTSTR pstrPath = NULL;
				for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
					if (_tcsicmp(pstrName, _T("fontfile")) == 0) {
						pstrPath = pstrValue;
					}
				}
				if (pstrPath) {
					pManager->AddFontArray(pstrPath);
				}
			}
		}
	}
}