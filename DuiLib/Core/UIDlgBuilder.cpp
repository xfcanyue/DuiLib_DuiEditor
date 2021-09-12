#include "StdAfx.h"

namespace DuiLib {

	CDialogBuilder::CDialogBuilder() : m_pCallback(NULL), m_pstrtype(NULL)
	{
		m_instance = NULL;
	}

	CControlUI* CDialogBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback, 
		CPaintManagerUI* pManager, CControlUI* pParent)
	{
		//资源ID为0-65535，两个字节；字符串指针为4个字节
		//字符串以<开头认为是XML字符串，否则认为是XML文件
		if(HIWORD(xml.m_lpstr) != NULL && *(xml.m_lpstr) != _T('<')) {
			LPCTSTR xmlpath = CResourceManager::GetInstance()->GetXmlPath(xml.m_lpstr);
			if (xmlpath != NULL) {
				xml = xmlpath;
			}
		}

		if( HIWORD(xml.m_lpstr) != NULL ) {
			if( *(xml.m_lpstr) == _T('<') ) {
				if( !m_xml.load_string(xml.m_lpstr) ) return NULL;
			}
			else {
				if( !m_xml.load_file(xml.m_lpstr) ) return NULL;
			}
		}
		else {
			HINSTANCE dll_instence = NULL;
			if (m_instance)
			{
				dll_instence = m_instance;
			}
			else
			{
				dll_instence = CPaintManagerUI::GetResourceDll();
			}
			HRSRC hResource = ::FindResource(dll_instence, xml.m_lpstr, type);
			if( hResource == NULL ) return NULL;
			HGLOBAL hGlobal = ::LoadResource(dll_instence, hResource);
			if( hGlobal == NULL ) {
				FreeResource(hResource);
				return NULL;
			}

			m_pCallback = pCallback;
			if( !m_xml.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(dll_instence, hResource) )) return NULL;
			::FreeResource(hResource);
			m_pstrtype = type;
		}
		CLangPackageUI *pkg = pManager->GetLangManager()->AddPackage(xml.m_lpstr);
		return Create(pCallback, pManager, pParent, pkg);
	}

	CControlUI* CDialogBuilder::Create(IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent, CLangPackageUI *pkg)
	{
		m_pCallback = pCallback;

		CXmlNodeUI root;
		for (CXmlNodeUI node= m_xml.first_child(); node; node=node.next_sibling())
		{
			if(node.type() == ui_node_element)
			{
				root = node;
				break;
			}
		}
		if(!root) return NULL;

		if( pManager ) {
			LPCTSTR pstrClass = NULL;
			int nAttributes = 0;
			LPCTSTR pstrName = NULL;
			LPCTSTR pstrValue = NULL;
			LPTSTR pstr = NULL;
			for( CXmlNodeUI node = root.first_child() ; node; node = node.next_sibling() ) 
			{
				pstrClass = node.name();
				if( _tcsicmp(pstrClass, _T("Image")) == 0 ) 
				{
					CDuiString ImageName	= node.attribute(_T("name")).as_string();
					LPCTSTR pImageResType	= node.attribute(_T("restype")).as_string();
					bool shared				= node.attribute(_T("shared")).as_bool();
					DWORD mask				= node.attribute(_T("mask")).as_uicolor();

					if( !ImageName.IsEmpty() ) 
						pManager->AddImage(ImageName.GetData(), pImageResType, mask, false, shared);
				}
				else if( _tcsicmp(pstrClass, _T("Font")) == 0 ) 
				{
					int id				= node.attribute(_T("id")).as_int(-1);
					LPCTSTR pFontName	= node.attribute(_T("name")).as_string();
					int size			= node.attribute(_T("size")).as_int(12);
					bool bold			= node.attribute(_T("bold")).as_bool();
					bool underline		= node.attribute(_T("underline")).as_bool();
					bool italic			= node.attribute(_T("italic")).as_bool();
					bool defaultfont	= node.attribute(_T("default")).as_bool();
					bool shared			= node.attribute(_T("shared")).as_bool();

					if( id >= 0 ) {
						pManager->AddFont(id, pFontName, size, bold, underline, italic, shared);
						if( defaultfont ) pManager->SetDefaultFont(pFontName, pManager->GetDPIObj()->Scale(size), bold, underline, italic, shared);
					}
				}
				else if( _tcsicmp(pstrClass, _T("Default")) == 0 ) 
				{
					CDuiString pControlName	= node.attribute(_T("name")).as_string();
					LPCTSTR pControlValue	= node.attribute(_T("value")).as_string();
					bool shared				= node.attribute(_T("shared")).as_bool();

					if( !pControlName.IsEmpty() ) {
						pManager->AddDefaultAttributeList(pControlName.GetData(), pControlValue, shared);
					}
				}
				else if( _tcsicmp(pstrClass, _T("Style")) == 0 ) 
				{
					CDuiString pName	= node.attribute(_T("name")).as_string();
					LPCTSTR pStyle	= node.attribute(_T("value")).as_string();
					bool shared		= node.attribute(_T("shared")).as_bool();

					if( !pName.IsEmpty() ) {
						pManager->AddStyle(pName.GetData(), pStyle, shared);
					}
				}
				else if (_tcsicmp(pstrClass, _T("Import")) == 0) {
					CDuiString strPath = node.attribute(_T("fontfile")).as_string();
					if (!strPath.IsEmpty()) {
						pManager->AddFontArray(strPath.GetData());
					}
				}

				//脚本不要在xml中引入，在程序中CPaintManager::AddScriptFile(...);
// 				else if( _tcsicmp(pstrClass, _T("Script")) == 0 ) 
// 				{
// 					LPCTSTR pstrIncludeFile = NULL;
// 					LPCTSTR pstrLanguage = NULL;
// 					bool shared = false;
// 					for( CXmlAttributeUI attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
// 						pstrName = attr.name();
// 						pstrValue = attr.value();
// 						if( _tcsicmp(pstrName, _T("source")) == 0 ) 
// 						{
// 							pstrIncludeFile = pstrValue;
// 						}
// 						else if( _tcsicmp(pstrName, _T("language")) == 0 ) 
// 						{
// 							pstrLanguage = pstrValue;
// 						}
// 					}
// 
// 					if(pstrIncludeFile)
// 					{
// 						pManager->AddScriptFile(pstrIncludeFile, pstrLanguage);
// 					}
// 				}
			}

			pstrClass = root.name();
			if( _tcsicmp(pstrClass, _T("Window")) == 0 ) 
			{
				//if( pManager->GetPaintWindow() ) 
				if( pManager->GetPaintWindow() && !pManager->IsInitWindowParameter())  //modify by liqs99
				{
					for( CXmlAttributeUI attr = root.first_attribute(); attr; attr=attr.next_attribute() ) 
					{
						pstrName = attr.name();
						if( _tcsicmp(pstrName, _T("size")) == 0 ) 
						{
							SIZE sz = attr.as_size();
							pManager->SetInitSize(pManager->GetDPIObj()->Scale(sz.cx), pManager->GetDPIObj()->Scale(sz.cy));
						} 
						else if( _tcsicmp(pstrName, _T("sizebox")) == 0 ) 
						{
							RECT rcSizeBox = attr.as_rect();   
							pManager->SetSizeBox(rcSizeBox);
						}
						else if( _tcsicmp(pstrName, _T("caption")) == 0 ) 
						{
							RECT rcCaption = attr.as_rect();  
							pManager->SetCaptionRect(rcCaption);
						}
						else if( _tcsicmp(pstrName, _T("roundcorner")) == 0 ) 
						{
							SIZE sz = attr.as_size();
							pManager->SetRoundCorner(sz.cx, sz.cy);
						}
						else if( _tcsicmp(pstrName, _T("mininfo")) == 0 ) 
						{
							SIZE sz = attr.as_size();
							pManager->SetMinInfo(sz.cx, sz.cy);
						}
						else if( _tcsicmp(pstrName, _T("maxinfo")) == 0 ) 
						{
							SIZE sz = attr.as_size();
							pManager->SetMaxInfo(sz.cx, sz.cy);
						}
						else if( _tcsicmp(pstrName, _T("showdirty")) == 0 ) {
							pManager->SetShowUpdateRect(attr.as_bool());
						} 
						else if( _tcsicmp(pstrName, _T("opacity")) == 0 || _tcsicmp(pstrName, _T("alpha")) == 0 ) 
						{
							pManager->SetOpacity(attr.as_int());
						} 
						else if( _tcscmp(pstrName, _T("layeredopacity")) == 0 ) 
						{
							pManager->SetLayeredOpacity(attr.as_int());
						} 
						else if( _tcscmp(pstrName, _T("layered")) == 0 || _tcscmp(pstrName, _T("bktrans")) == 0) 
						{
							pManager->SetLayered(attr.as_bool());
						}
						else if( _tcscmp(pstrName, _T("layeredimage")) == 0 ) 
						{
							pManager->SetLayered(true);
							pManager->SetLayeredImage(attr.as_string());
						} 
						else if( _tcscmp(pstrName, _T("noactivate")) == 0 ) 
						{
							pManager->SetNoActivate(attr.as_bool());
						}
						else if( _tcsicmp(pstrName, _T("disabledfontcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->SetDefaultDisabledColor(clrColor);
						} 
						else if( _tcsicmp(pstrName, _T("defaultfontcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->SetDefaultFontColor(clrColor);
						}
						else if( _tcsicmp(pstrName, _T("linkfontcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->SetDefaultLinkFontColor(clrColor);
						} 
						else if( _tcsicmp(pstrName, _T("linkhoverfontcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->SetDefaultLinkHoverFontColor(clrColor);
						} 
						else if( _tcsicmp(pstrName, _T("selectedcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->SetDefaultSelectedBkColor(clrColor);
						} 
						else if( _tcsicmp(pstrName, _T("shadowsize")) == 0 ) 
						{
							pManager->GetShadow()->SetSize(attr.as_int());
						}
						else if( _tcsicmp(pstrName, _T("shadowsharpness")) == 0 ) 
						{
							pManager->GetShadow()->SetSharpness(attr.as_int());
						}
						else if( _tcsicmp(pstrName, _T("shadowdarkness")) == 0 ) 
						{
							pManager->GetShadow()->SetDarkness(attr.as_int());
						}
						else if( _tcsicmp(pstrName, _T("shadowposition")) == 0 ) 
						{
							SIZE sz = attr.as_size();
							pManager->GetShadow()->SetPosition(sz.cx, sz.cy);
						}
						else if( _tcsicmp(pstrName, _T("shadowcolor")) == 0 ) 
						{
							DWORD clrColor = attr.as_uicolor();
							pManager->GetShadow()->SetColor(clrColor);
						}
						else if( _tcsicmp(pstrName, _T("shadowcorner")) == 0 ) 
						{
							RECT rcCorner = attr.as_rect();   
							pManager->GetShadow()->SetShadowCorner(rcCorner);
						}
						else if( _tcsicmp(pstrName, _T("shadowimage")) == 0 ) 
						{
							pManager->GetShadow()->SetImage(attr.as_string());
						}
						else if( _tcsicmp(pstrName, _T("showshadow")) == 0 ) 
						{
							pManager->GetShadow()->ShowShadow(attr.as_bool());
						} 
						else if( _tcsicmp(pstrName, _T("gdiplustext")) == 0 ) 
						{
							pManager->SetUseGdiplusText(attr.as_bool());
						} 
						else if( _tcsicmp(pstrName, _T("textrenderinghint")) == 0 ) 
						{
							pManager->SetGdiplusTextRenderingHint(attr.as_int());
						} 
						else if( _tcsicmp(pstrName, _T("tooltiphovertime")) == 0 ) 
						{
							pManager->SetHoverTime(attr.as_int());
						} 
					}


					pManager->SetInitWindowParameter(true); //modify by liqs99
				}
			}
		}

		//pManager->CompileScript();
		return _Parse(root, pParent, pManager, pkg);
	}

	CControlUI* CDialogBuilder::_Parse(CXmlNodeUI root, CControlUI* pParent, CPaintManagerUI* pManager, CLangPackageUI *pkg)
	{
		IContainerUI* pContainer = NULL;
		CControlUI* pReturn = NULL;
		for( CXmlNodeUI node = root.first_child(); node; node = node.next_sibling() ) 
		{
			if(node.type() != ui_node_element) continue;

			LPCTSTR pstrClass = node.name();
			if( _tcsicmp(pstrClass, _T("Image")) == 0 || _tcsicmp(pstrClass, _T("Font")) == 0 \
				|| _tcsicmp(pstrClass, _T("Default")) == 0 || _tcsicmp(pstrClass, _T("Style")) == 0 \
				|| _tcsicmp(pstrClass, _T("Script")) == 0) continue;

			CControlUI* pControl = NULL;
			if (_tcsicmp(pstrClass, _T("Import")) == 0) continue;
			if( _tcsicmp(pstrClass, _T("Include")) == 0 ) 
			{
				CXmlAttributeUI attrSource = node.attribute(_T("source"));
				if(!attrSource) continue;

				CXmlAttributeUI attrCount = node.attribute(_T("count"));
				int count = attrCount.as_int(1);

				for ( int i = 0; i < count; i++ ) 
				{
					CDialogBuilder builder;
					if( m_pstrtype != NULL ) { // 使用资源dll，从资源中读取
						WORD id = attrSource.as_int(); 
						pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
					}
					else {
						pControl = builder.Create(attrSource.as_string(), (UINT)0, m_pCallback, pManager, pParent);
					}
				}
				continue;
			}
			else {
				CDuiString strClass;
				strClass.Format(_T("C%sUI"), pstrClass);
				pControl = dynamic_cast<CControlUI*>(CControlFactory::GetInstance()->CreateControl(strClass));

				// 检查插件
				if( pControl == NULL ) {
					CStdPtrArray* pPlugins = CPaintManagerUI::GetPlugins();
					LPCREATECONTROL lpCreateControl = NULL;
					for( int i = 0; i < pPlugins->GetSize(); ++i ) {
						lpCreateControl = (LPCREATECONTROL)pPlugins->GetAt(i);
						if( lpCreateControl != NULL ) {
							pControl = lpCreateControl(pstrClass);
							if( pControl != NULL ) break;
						}
					}
				}
				// 回掉创建
				if( pControl == NULL && m_pCallback != NULL ) {
					pControl = m_pCallback->CreateControl(pstrClass);
				}
			}

			//设计器模式创建自定义控件
			if(pControl == NULL && (CPaintManagerUI::UIDESIGNMODE || CPaintManagerUI::UIDESIGNPREVIEW))
			{
				CXmlAttributeUI attr = node.attribute(_T("custombasedfrom"));
				if(attr)
				{
					CDuiString strClass;
					strClass.Format(_T("C%sUI"), attr.value());
					pControl = dynamic_cast<CControlUI*>(CControlFactory::GetInstance()->CreateControl(strClass));
				}
			}

			if( pControl == NULL ) {
				CDuiString sMsg;
				sMsg.Format(_T("CDialogBuilder::_Parse Error\r\nFile: %s\r\nControl: %s"), pkg->GetSkinFile(), pstrClass);
				MessageBox(NULL, sMsg, _T("Create Control Failed"), MB_OK);
				continue;
			}

			//因为duilib有大量的if(!GetParent())逻辑判断，额外设置一个GetExtraParent接口，方便子控件访问父控件
			pControl->SetExtraParent(pParent);

			if(pkg)
			{
				//记住pControl是从哪个文件创建的
				pControl->SetSkinFile(pkg->GetSkinFile());
			}

			// Add children
			if( node.first_child() ) {
				_Parse(node, pControl, pManager, pkg);
			}

			// Attach to parent
			if( pParent != NULL && pControl != NULL ) 
			{
				if( pContainer == NULL ) pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
				ASSERT(pContainer);
				if( pContainer == NULL ) return NULL;
				if( !pContainer->Add(pControl) ) 
				{
					delete pControl;
					continue;
				}
			}
			if( pControl == NULL ) continue;

			// Init default attributes
			if( pManager ) 
			{
				pControl->SetManager(pManager, NULL, false);
				LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
				if( pDefaultAttributes ) 
				{
					pControl->ApplyAttributeList(pDefaultAttributes);
				}
			}

			// Process attributes
			if( node.first_attribute() ) 
			{
				//first find style attribute  先设置style属性
				CXmlAttributeUI attr = node.attribute(_T("style"));
				if(attr)
				{
					pControl->SetAttribute(attr.name(), attr.value());
				}

				// Set ordinary attributes， 设置除了style之外的属性
				for( CXmlAttributeUI attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
					if(_tcsicmp(attr.name(), _T("style")) != 0)
						pControl->SetAttribute(attr.name(), attr.value());
				}
			}

			if( pManager ) 
			{
					pControl->SetManager(NULL, NULL, false);
			}

			// Return first item
			if( pReturn == NULL ) pReturn = pControl;
		}
		return pReturn;
	}

} // namespace DuiLib
