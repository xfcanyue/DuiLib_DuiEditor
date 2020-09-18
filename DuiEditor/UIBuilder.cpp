#include "StdAfx.h"
#include "DuiEditor.h"
#include "UIBuilder.h"

#include "DlgCustomControl.h"

#ifndef DUILIB_VERSION_ORIGINAL
CUIBuilder::CUIBuilder()  : m_pCallback(NULL), m_pstrtype(NULL)
{
	m_instance = NULL;
	m_bOpenConfigFile = false;
}


CUIBuilder::~CUIBuilder(void)
{
}
/*
CControlUI* CUIBuilder::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback,
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
			if( !m_xml.load_string(xml.m_lpstr, XML_PARSER_OPTIONS) ) return NULL;
		}
		else {
			if( !m_xml.load_file(xml.m_lpstr, XML_PARSER_OPTIONS) ) return NULL;
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

	return Create(m_xml, pCallback, pManager, pParent);
}
*/
CControlUI* CUIBuilder::Create(pugi::xml_node xmldoc, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
	m_pCallback = pCallback;
	//pugi::xml_node root = xmldoc.child(_T("Window"));
	//if( !root) return NULL;

	pugi::xml_node root;
	for (xml_node node=xmldoc.first_child(); node; node=node.next_sibling())
	{
		if(node.type() == node_element) //找到第一个节点, 而不是找Window节点, 因为手写的Include子窗口不带window节点
		{
			root = node;
			break;
		}
	}
	if( !root) return NULL;

	if(!g_proj.IsLoadedConfig())
	{
		g_proj.LoadConfig(pManager);
	}

	if( pManager ) {
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
					if( defaultfont ) pManager->SetDefaultFont(pFontName, pManager->GetDPIObj()->Scale(size), bold, underline, italic, shared);
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

		pstrClass = root.name();
		if( _tcsicmp(pstrClass, _T("Window")) == 0 ) 
		{
			if( pManager->GetPaintWindow()  && !pManager->IsInitWindowParameter()) //modify by liqs99
			{
				SetWindowAttribute(pManager, root);
				pManager->SetInitWindowParameter(true); //modify by liqs99
			}
		}
	}
	return _Parse(root, pParent, pManager);
}

void CUIBuilder::SetWindowAttribute(CPaintManagerUI *pManager, xml_node node)
{
	if( !pManager->GetPaintWindow() ) return;

	for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
	{
		LPCTSTR pstrName = attr.name();
		LPCTSTR pstrValue = attr.value();

		if( _tcsicmp(pstrName, _T("size")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetInitSize(pManager->GetDPIObj()->Scale(cx), pManager->GetDPIObj()->Scale(cy));
		} 
		else if( _tcsicmp(pstrName, _T("sizebox")) == 0 ) {
			RECT rcSizeBox = { 0 };
			LPTSTR pstr = NULL;
			rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->SetSizeBox(rcSizeBox);
		}
		else if( _tcsicmp(pstrName, _T("caption")) == 0 ) {
			RECT rcCaption = { 0 };
			LPTSTR pstr = NULL;
			rcCaption.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcCaption.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcCaption.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->SetCaptionRect(rcCaption);
		}
		else if( _tcsicmp(pstrName, _T("roundcorner")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetRoundCorner(cx, cy);
		} 
		else if( _tcsicmp(pstrName, _T("mininfo")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetMinInfo(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("maxinfo")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->SetMaxInfo(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("showdirty")) == 0 ) {
			pManager->SetShowUpdateRect(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("opacity")) == 0 || _tcsicmp(pstrName, _T("alpha")) == 0 ) {
			pManager->SetOpacity(_ttoi(pstrValue));
		} 
		else if( _tcscmp(pstrName, _T("layeredopacity")) == 0 ) {
			pManager->SetLayeredOpacity(_ttoi(pstrValue));
		} 
		else if( _tcscmp(pstrName, _T("layered")) == 0 || _tcscmp(pstrName, _T("bktrans")) == 0) {
			pManager->SetLayered(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcscmp(pstrName, _T("layeredimage")) == 0 ) {
			pManager->SetLayered(true);
			pManager->SetLayeredImage(pstrValue);
		} 
		else if( _tcscmp(pstrName, _T("noactivate")) == 0 ) {
			pManager->SetNoActivate(_tcsicmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("disabledfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultDisabledColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("defaultfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultFontColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("linkfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultLinkFontColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("linkhoverfontcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultLinkHoverFontColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("selectedcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->SetDefaultSelectedBkColor(clrColor);
		} 
		else if( _tcsicmp(pstrName, _T("shadowsize")) == 0 ) {
			pManager->GetShadow()->SetSize(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowsharpness")) == 0 ) {
			pManager->GetShadow()->SetSharpness(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowdarkness")) == 0 ) {
			pManager->GetShadow()->SetDarkness(_ttoi(pstrValue));
		}
		else if( _tcsicmp(pstrName, _T("shadowposition")) == 0 ) {
			LPTSTR pstr = NULL;
			int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
			pManager->GetShadow()->SetPosition(cx, cy);
		}
		else if( _tcsicmp(pstrName, _T("shadowcolor")) == 0 ) {
			if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
			LPTSTR pstr = NULL;
			DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
			pManager->GetShadow()->SetColor(clrColor);
		}
		else if( _tcsicmp(pstrName, _T("shadowcorner")) == 0 ) {
			RECT rcCorner = { 0 };
			LPTSTR pstr = NULL;
			rcCorner.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
			rcCorner.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
			rcCorner.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
			rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
			pManager->GetShadow()->SetShadowCorner(rcCorner);
		}
		else if( _tcsicmp(pstrName, _T("shadowimage")) == 0 ) {
			pManager->GetShadow()->SetImage(pstrValue);
		}
		else if( _tcsicmp(pstrName, _T("showshadow")) == 0 ) {
			pManager->GetShadow()->ShowShadow(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("gdiplustext")) == 0 ) {
			pManager->SetUseGdiplusText(_tcsicmp(pstrValue, _T("true")) == 0);
		} 
		else if( _tcsicmp(pstrName, _T("textrenderinghint")) == 0 ) {
			pManager->SetGdiplusTextRenderingHint(_ttoi(pstrValue));
		} 
		else if( _tcsicmp(pstrName, _T("tooltiphovertime")) == 0 ) {
			pManager->SetHoverTime(_ttoi(pstrValue));
		} 
	}
}


CControlUI* CUIBuilder::_Parse(pugi::xml_node root,  CControlUI* pParent, CPaintManagerUI* pManager)
{
	IContainerUI* pContainer = NULL;
	CControlUI* pReturn = NULL;
	for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) 
	{
		if(node.type() != node_element)	
			continue;

		LPCTSTR pstrClass = node.name();
		if( _tcsicmp(pstrClass, _T("Image")) == 0 || _tcsicmp(pstrClass, _T("Font")) == 0 \
			|| _tcsicmp(pstrClass, _T("Default")) == 0 || _tcsicmp(pstrClass, _T("Style")) == 0 ) continue;

		CControlUI* pControl = NULL;
		if (_tcsicmp(pstrClass, _T("Import")) == 0) continue;
		if( _tcsicmp(pstrClass, _T("Include")) == 0 ) {			//载入Include必须使用原来的CDialogBuilder
			int count = node.attribute(_T("count")).as_int(1);
			for ( int i = 0; i < count; i++ ) {
				CDialogBuilder builder;
				if( m_pstrtype != NULL ) { // 使用资源dll，从资源中读取
					WORD id = node.attribute(_T("source")).as_int(); 
					pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
				}
				else {
					LPCTSTR szValue = node.attribute(_T("source")).value();
					pControl = builder.Create(szValue, (UINT)0, m_pCallback, pManager, pParent);
				}
			}

			if(pControl)
			{
				pControl->SetTag((UINT_PTR)node.internal_object());
				node.set_tag((UINT_PTR)pControl);
			}
			continue;
		}
		else {
			//pControl = CreateControl(node);	//放到下面去，优先从回调创建

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

			if(pControl == NULL)
			{
				pControl = CreateControl(node);	//优先从回调创建
			}

		}

		if( pControl == NULL ) {
#ifdef _DEBUG
			DUITRACE(_T("未知控件:%s"), pstrClass);
			continue;
#else
			continue;
#endif
		}
	
		pControl->SetTag((UINT_PTR)node.internal_object());
		node.set_tag((UINT_PTR)pControl);

		// Add children
		if( node.first_child() ) {
			_Parse(node, pControl, pManager);
		}
		// Attach to parent
		// 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
		CTreeViewUI* pTreeView = NULL;
		if( pParent != NULL && pControl != NULL ) {
			CTreeNodeUI* pParentTreeNode = static_cast<CTreeNodeUI*>(pParent->GetInterface(_T("TreeNode")));
			CTreeNodeUI* pTreeNode = static_cast<CTreeNodeUI*>(pControl->GetInterface(_T("TreeNode")));
			pTreeView = static_cast<CTreeViewUI*>(pParent->GetInterface(_T("TreeView")));
			// TreeNode子节点
			if(pTreeNode != NULL) {
				if(pParentTreeNode) {
					pTreeView = pParentTreeNode->GetTreeView();
					if(!pParentTreeNode->Add(pTreeNode)) {
						delete pTreeNode;
						pTreeNode = NULL;
						continue;
					}
				}
				else {
					if(pTreeView != NULL) {
						if(!pTreeView->Add(pTreeNode)) {
							delete pTreeNode;
							pTreeNode = NULL;
							continue;
						}
					}
				}
			}
			// TreeNode子控件
			else if(pParentTreeNode != NULL) {
				pParentTreeNode->GetTreeNodeHoriznotal()->Add(pControl);
			}
			// 普通控件
			else {
				if( pContainer == NULL ) pContainer = static_cast<IContainerUI*>(pParent->GetInterface(_T("IContainer")));
				//ASSERT(pContainer);
				if( pContainer == NULL ) return NULL;
				if( !pContainer->Add(pControl) ) 
				{
					delete pControl;
					pControl = NULL;
					continue;
				}
			}
		}
		if( pControl == NULL ) continue;

		// Init default attributes
		if( pManager ) {
			if(pTreeView != NULL) {
				pControl->SetManager(pManager, pTreeView, true);
			}
			else {
				pControl->SetManager(pManager, NULL, false);
			}
			LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
			if( pDefaultAttributes ) {
				pControl->ApplyAttributeList(pDefaultAttributes);
			}
		}
		// Process attributes
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			pControl->SetAttribute(attr.name(), attr.value());
		}

		if( pManager ) {
			if(pTreeView == NULL) {
				pControl->SetManager(NULL, NULL, false);
			}
		}
		// Return first item
		if( pReturn == NULL ) pReturn = pControl;
	}
	return pReturn;
}

CControlUI* CUIBuilder::CreateControl(LPCTSTR pstrClass)
{
	CControlUI *pControl;

// 	if(CompareString(pstrClass, _T("Menu")))
// 	{
// 		pControl = new CMenuExUI;
// 		return pControl;
// 	}
// 
// 	if(CompareString(pstrClass, _T("MenuElement")))
// 	{
// 		pControl = new CMenuElementExUI;
// 		return pControl;
// 	}

	CDuiString strClass;
	strClass.Format(_T("C%sUI"), pstrClass);
	pControl = dynamic_cast<CControlUI*>(CControlFactory::GetInstance()->CreateControl(strClass));

	return pControl;
}

CControlUI* CUIBuilder::CreateControl(xml_node node)
{
	CControlUI *pControl = NULL;
	pControl = CreateControl(node.name());
	if(!pControl)
	{
		xml_attribute attr = node.attribute(_T("custombasedfrom"));
		if(attr)
		{
			pControl = CreateControl(attr.value());
		}
// 		else //不认识的控件，直接从CContainner创建, 不能这样子哦
// 		{
// 			pControl = CreateControl(DUI_CTR_CONTROL);
// 		}
	}

	return pControl;
}

void CUIBuilder::DeleteControl(CControlUI *pControl)
{
	delete pControl;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#else

//////////////////////////////////////////////////////////////////////////
CUIBuilderOrginal::CUIBuilderOrginal() : m_pCallback(NULL), m_pstrtype(NULL)
{

}

CControlUI* CUIBuilderOrginal::Create(STRINGorID xml, LPCTSTR type, IDialogBuilderCallback* pCallback, 
	CPaintManagerUI* pManager, CControlUI* pParent)
{
	//资源ID为0-65535，两个字节；字符串指针为4个字节
	//字符串以<开头认为是XML字符串，否则认为是XML文件

	if( HIWORD(xml.m_lpstr) != NULL ) {
		if( *(xml.m_lpstr) == _T('<') ) {
			if( !m_xml.load_string(xml.m_lpstr) ) return NULL;
		}
		else {
			if( !m_xml.load_file(xml.m_lpstr) ) return NULL;
		}
	}
	else {
		HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), xml.m_lpstr, type);
		if( hResource == NULL ) return NULL;
		HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
		if( hGlobal == NULL ) {
			FreeResource(hResource);
			return NULL;
		}

		m_pCallback = pCallback;
		if( !m_xml.load_buffer((BYTE*)::LockResource(hGlobal), ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource) )) return NULL;
		::FreeResource(hResource);
		m_pstrtype = type;
	}

	return Create(m_xml, pCallback, pManager, pParent);
}

CControlUI* CUIBuilderOrginal::Create(xml_node xmldoc, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager, CControlUI* pParent)
{
	m_pCallback = pCallback;

	pugi::xml_node root;
	for (xml_node node=xmldoc.first_child(); node; node=node.next_sibling())
	{
		if(node.type() == node_element) //找到第一个节点, 而不是找Window节点, 因为手写的Include子窗口不带window节点
		{
			root = node;
			break;
		}
	}
	if( !root) return NULL;

	if( pManager ) {
		LPCTSTR pstrClass = NULL;
		int nAttributes = 0;
		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;
		for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) 
		{
			if(node.type() != node_element)	continue;
			pstrClass = node.name();
			if( _tcsicmp(pstrClass, _T("Image")) == 0 ) {
				LPCTSTR pImageName = NULL;
				LPCTSTR pImageResType = NULL;
				DWORD mask = 0;
				bool shared = false;
				for(xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute() ) {
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
				if( pImageName ) pManager->AddImage(pImageName, pImageResType, mask, shared);
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
				for(xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute() ) {
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
				if( id >= 0 && pFontName ) {
					pManager->AddFont(id, pFontName, size, bold, underline, italic, shared);
					if( defaultfont ) pManager->SetDefaultFont(pFontName, size, bold, underline, italic, shared);
				}
			}
			else if( _tcsicmp(pstrClass, _T("Default")) == 0 ) {
				LPCTSTR pControlName = NULL;
				LPCTSTR pControlValue = NULL;
				bool shared = false;
				for(xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute() ) {
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
			else if( _tcsicmp(pstrClass, _T("MultiLanguage")) == 0 ) {
				int id = -1;
				LPCTSTR pMultiLanguage = NULL;
				for(xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
					if( _tcsicmp(pstrName, _T("id")) == 0 ) {
						id = _tcstol(pstrValue, &pstr, 10);
					}
					else if( _tcsicmp(pstrName, _T("value")) == 0 ) {
						pMultiLanguage = pstrValue;
					}
				}
				if (id >= 0 && pMultiLanguage ) {
					pManager->AddMultiLanguageString(id, pMultiLanguage);
				}
			}
		}

		pstrClass = root.name();
		if( _tcsicmp(pstrClass, _T("Window")) == 0 ) {
			if( pManager->GetPaintWindow() ) {
				for(xml_attribute attr=root.first_attribute(); attr; attr=attr.next_attribute() ) {
					pstrName = attr.name();
					pstrValue = attr.value();
					pManager->SetWindowAttribute(pstrName, pstrValue);
				}
			}
		}
	}
	return _Parse(root, pParent, pManager);
}

CControlUI* CUIBuilderOrginal::_Parse(xml_node root,  CControlUI* pParent, CPaintManagerUI* pManager)
{
	IContainerUI* pContainer = NULL;
	CControlUI* pReturn = NULL;
	for( xml_node node = root.first_child() ; node; node = node.next_sibling() ) {
		LPCTSTR pstrClass = node.name();
		if( _tcsicmp(pstrClass, _T("Image")) == 0 || _tcsicmp(pstrClass, _T("Font")) == 0 \
			|| _tcsicmp(pstrClass, _T("Default")) == 0 
			|| _tcsicmp(pstrClass, _T("MultiLanguage")) == 0 ) continue;

		CControlUI* pControl = NULL;
		if( _tcsicmp(pstrClass, _T("Include")) == 0 ) {
			if( !node.first_attribute() ) continue;
			int count = node.attribute(_T("count")).as_int(1);

			CDialogBuilder builder;
			for ( int i = 0; i < count; i++ ) {
				if (!m_xml)
				{
					if( m_pstrtype != NULL ) { // 使用资源dll，从资源中读取
						WORD id = node.attribute(_T("source")).as_int(); 
						pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, pManager, pParent);
					}
					else 
					{
						LPCTSTR szValue = node.attribute(_T("source")).value();
						pControl = builder.Create((LPCTSTR)szValue, (UINT)0, m_pCallback, pManager, pParent);
					}
				}
				else
					pControl = builder.Create(m_pCallback, pManager, pParent);
			}
			if(pControl)
			{
				pControl->SetTag((UINT_PTR)node.internal_object());
				node.set_tag((UINT_PTR)pControl);
			}
			continue;
		}
		//树控件XML解析
		else if( _tcsicmp(pstrClass, _T("TreeNode")) == 0 ) {
			CTreeNodeUI* pParentNode	= static_cast<CTreeNodeUI*>(pParent->GetInterface(_T("TreeNode")));
			CTreeNodeUI* pNode			= new CTreeNodeUI();
			if(pParentNode){
				if(!pParentNode->Add(pNode)){
					delete pNode;
					continue;
				}
			}

			// 若有控件默认配置先初始化默认属性
			if( pManager ) {
				pNode->SetManager(pManager, NULL, false);
				LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
				if( pDefaultAttributes ) {
					pNode->SetAttributeList(pDefaultAttributes);
				}
			}

			// 解析所有属性并覆盖默认属性
			if( node.first_attribute() ) {
				for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
				{
					pNode->SetAttribute(attr.name(), attr.value());
				}
			}

			//检索子节点及附加控件
			if(node.first_child()){
				CControlUI* pSubControl = _Parse(node,pNode,pManager);
				if(pSubControl && _tcsicmp(pSubControl->GetClass(),_T("TreeNodeUI")) != 0)
				{
					// 					pSubControl->SetFixedWidth(30);
					// 					CHorizontalLayoutUI* pHorz = pNode->GetTreeNodeHoriznotal();
					// 					pHorz->Add(new CEditUI());
					// 					continue;
				}
			}

			if(!pParentNode){
				CTreeViewUI* pTreeView = static_cast<CTreeViewUI*>(pParent->GetInterface(_T("TreeView")));
				ASSERT(pTreeView);
				if( pTreeView == NULL ) return NULL;
				if( !pTreeView->Add(pNode) ) {
					delete pNode;
					continue;
				}
			}
			continue;
		}
		else {
#ifdef _DEBUG
			DUITRACE(_T("Create Control: %s"), pstrClass);
#endif
			SIZE_T cchLen = _tcslen(pstrClass);
			switch( cchLen ) {
			case 4:
				if( _tcsicmp(pstrClass, DUI_CTR_EDIT) == 0 )                  pControl = new CEditUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_LIST) == 0 )             pControl = new CListUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TEXT) == 0 )             pControl = new CTextUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TREE) == 0 )             pControl = new CTreeViewUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_HBOX) == 0 )             pControl = new CHorizontalLayoutUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_VBOX) == 0 )             pControl = new CVerticalLayoutUI;
				break;
			case 5:
				if( _tcsicmp(pstrClass, DUI_CTR_COMBO) == 0 )                 pControl = new CComboUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_LABEL) == 0 )            pControl = new CLabelUI;
				//else if( _tcsicmp(pstrClass, DUI_CTR_FLASH) == 0 )           pControl = new CFlashUI;
				break;
			case 6:
				if( _tcsicmp(pstrClass, DUI_CTR_BUTTON) == 0 )                pControl = new CButtonUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_OPTION) == 0 )           pControl = new COptionUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_SLIDER) == 0 )           pControl = new CSliderUI;
				break;
			case 7:
				if( _tcsicmp(pstrClass, DUI_CTR_CONTROL) == 0 )               pControl = new CControlUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_ACTIVEX) == 0 )          pControl = new CActiveXUI;
				else if (_tcscmp(pstrClass, DUI_CTR_GIFANIM) == 0)            pControl = new CGifAnimUI;
				break;
			case 8:
				if( _tcsicmp(pstrClass, DUI_CTR_PROGRESS) == 0 )              pControl = new CProgressUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_RICHEDIT) == 0 )         pControl = new CRichEditUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_CHECKBOX) == 0 )		  pControl = new CCheckBoxUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_COMBOBOX) == 0 )		  pControl = new CComboUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_DATETIME) == 0 )		  pControl = new CDateTimeUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TREEVIEW) == 0 )         pControl = new CTreeViewUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TREENODE) == 0 )		  pControl = new CTreeNodeUI;
				break;
			case 9:
				if( _tcsicmp(pstrClass, DUI_CTR_CONTAINER) == 0 )             pControl = new CContainerUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TABLAYOUT) == 0 )        pControl = new CTabLayoutUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_SCROLLBAR) == 0 )        pControl = new CScrollBarUI; 
				break;
			case 10:
				if( _tcsicmp(pstrClass, DUI_CTR_LISTHEADER) == 0 )            pControl = new CListHeaderUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_TILELAYOUT) == 0 )       pControl = new CTileLayoutUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_WEBBROWSER) == 0 )       pControl = new CWebBrowserUI;
				break;
			case 11:
				if (_tcsicmp(pstrClass, DUI_CTR_CHILDLAYOUT) == 0)			  pControl = new CChildLayoutUI;
				break;
			case 14:
				if( _tcsicmp(pstrClass, DUI_CTR_VERTICALLAYOUT) == 0 )        pControl = new CVerticalLayoutUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_LISTHEADERITEM) == 0 )   pControl = new CListHeaderItemUI;
				break;
			case 15:
				if( _tcsicmp(pstrClass, DUI_CTR_LISTTEXTELEMENT) == 0 )       pControl = new CListTextElementUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_LISTHBOXELEMENT) == 0 )  pControl = new CListHBoxElementUI;
				break;
			case 16:
				if( _tcsicmp(pstrClass, DUI_CTR_HORIZONTALLAYOUT) == 0 )      pControl = new CHorizontalLayoutUI;
				else if( _tcsicmp(pstrClass, DUI_CTR_LISTLABELELEMENT) == 0 ) pControl = new CListLabelElementUI;
				break;
			case 20:
				if( _tcsicmp(pstrClass, DUI_CTR_LISTCONTAINERELEMENT) == 0 )  pControl = new CListContainerElementUI;
				break;
			}
			// User-supplied control factory
			if( pControl == NULL ) {
				CDuiPtrArray* pPlugins = CPaintManagerUI::GetPlugins();
				LPCREATECONTROL lpCreateControl = NULL;
				for( int i = 0; i < pPlugins->GetSize(); ++i ) {
					lpCreateControl = (LPCREATECONTROL)pPlugins->GetAt(i);
					if( lpCreateControl != NULL ) {
						pControl = lpCreateControl(pstrClass);
						if( pControl != NULL ) break;
					}
				}
			}
			if( pControl == NULL && m_pCallback != NULL ) {
				pControl = m_pCallback->CreateControl(pstrClass);
			}
		}

#ifndef _DEBUG
		ASSERT(pControl);
#endif // _DEBUG
		if( pControl == NULL )
		{
#ifdef _DEBUG
			DUITRACE(_T("Unknow Control:%s"),pstrClass);
#endif
			continue;
		}

		pControl->SetTag((UINT_PTR)node.internal_object());
		node.set_tag((UINT_PTR)pControl);

		// Add children
		if( node.first_child() ) {
			_Parse(node, pControl, pManager);
		}

		// Attach to parent
		// 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
		if( pParent != NULL ) {
			bool bCover = node.attribute(_T("cover")).as_bool();
			if( bCover ) {
				pParent->SetCover(pControl);
			}
			else {
				CTreeNodeUI* pContainerNode = static_cast<CTreeNodeUI*>(pParent->GetInterface(DUI_CTR_TREENODE));
				if(pContainerNode)
					pContainerNode->GetTreeNodeHoriznotal()->Add(pControl);
				else
				{
					if( pContainer == NULL ) pContainer = static_cast<IContainerUI*>(pParent->GetInterface(DUI_CTR_ICONTAINER));
					ASSERT(pContainer);
					if( pContainer == NULL ) return NULL;
					if( !pContainer->Add(pControl) ) {
						pControl->Delete();
						continue;
					}
				}
			}
		}
		// Init default attributes
		if( pManager ) {
			pControl->SetManager(pManager, NULL, false);
			LPCTSTR pDefaultAttributes = pManager->GetDefaultAttributeList(pstrClass);
			if( pDefaultAttributes ) {
				pControl->SetAttributeList(pDefaultAttributes);
			}
		}
		// Process attributes
		if( node.first_attribute() ) {
			// Set ordinary attributes
			for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
			{
				pControl->SetAttribute(attr.name(), attr.value());
			}
		}
		if( pManager ) {
			pControl->SetManager(NULL, NULL, false);
		}
		// Return first item
		if( pReturn == NULL ) pReturn = pControl;
	}
	return pReturn;
}


CControlUI* CUIBuilderOrginal::CreateControl(LPCTSTR pstrClass)
{
	CControlUI *pControl;

	SIZE_T cchLen = _tcslen(pstrClass);
	switch( cchLen ) {
	case 4:
		if( _tcsicmp(pstrClass, DUI_CTR_EDIT) == 0 )                  pControl = new CEditUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_LIST) == 0 )             pControl = new CListUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TEXT) == 0 )             pControl = new CTextUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TREE) == 0 )             pControl = new CTreeViewUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_HBOX) == 0 )             pControl = new CHorizontalLayoutUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_VBOX) == 0 )             pControl = new CVerticalLayoutUI;
		break;
	case 5:
		if( _tcsicmp(pstrClass, DUI_CTR_COMBO) == 0 )                 pControl = new CComboUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_LABEL) == 0 )            pControl = new CLabelUI;
		//else if( _tcsicmp(pstrClass, DUI_CTR_FLASH) == 0 )           pControl = new CFlashUI;
		break;
	case 6:
		if( _tcsicmp(pstrClass, DUI_CTR_BUTTON) == 0 )                pControl = new CButtonUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_OPTION) == 0 )           pControl = new COptionUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_SLIDER) == 0 )           pControl = new CSliderUI;
		break;
	case 7:
		if( _tcsicmp(pstrClass, DUI_CTR_CONTROL) == 0 )               pControl = new CControlUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_ACTIVEX) == 0 )          pControl = new CActiveXUI;
		else if (_tcscmp(pstrClass, DUI_CTR_GIFANIM) == 0)            pControl = new CGifAnimUI;
		break;
	case 8:
		if( _tcsicmp(pstrClass, DUI_CTR_PROGRESS) == 0 )              pControl = new CProgressUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_RICHEDIT) == 0 )         pControl = new CRichEditUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_CHECKBOX) == 0 )		  pControl = new CCheckBoxUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_COMBOBOX) == 0 )		  pControl = new CComboUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_DATETIME) == 0 )		  pControl = new CDateTimeUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TREEVIEW) == 0 )         pControl = new CTreeViewUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TREENODE) == 0 )		  pControl = new CTreeNodeUI;
		break;
	case 9:
		if( _tcsicmp(pstrClass, DUI_CTR_CONTAINER) == 0 )             pControl = new CContainerUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TABLAYOUT) == 0 )        pControl = new CTabLayoutUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_SCROLLBAR) == 0 )        pControl = new CScrollBarUI; 
		break;
	case 10:
		if( _tcsicmp(pstrClass, DUI_CTR_LISTHEADER) == 0 )            pControl = new CListHeaderUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_TILELAYOUT) == 0 )       pControl = new CTileLayoutUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_WEBBROWSER) == 0 )       pControl = new CWebBrowserUI;
		break;
	case 11:
		if (_tcsicmp(pstrClass, DUI_CTR_CHILDLAYOUT) == 0)			  pControl = new CChildLayoutUI;
		break;
	case 14:
		if( _tcsicmp(pstrClass, DUI_CTR_VERTICALLAYOUT) == 0 )        pControl = new CVerticalLayoutUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_LISTHEADERITEM) == 0 )   pControl = new CListHeaderItemUI;
		break;
	case 15:
		if( _tcsicmp(pstrClass, DUI_CTR_LISTTEXTELEMENT) == 0 )       pControl = new CListTextElementUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_LISTHBOXELEMENT) == 0 )  pControl = new CListHBoxElementUI;
		break;
	case 16:
		if( _tcsicmp(pstrClass, DUI_CTR_HORIZONTALLAYOUT) == 0 )      pControl = new CHorizontalLayoutUI;
		else if( _tcsicmp(pstrClass, DUI_CTR_LISTLABELELEMENT) == 0 ) pControl = new CListLabelElementUI;
		break;
	case 20:
		if( _tcsicmp(pstrClass, DUI_CTR_LISTCONTAINERELEMENT) == 0 )  pControl = new CListContainerElementUI;
		break;
	}

	return pControl;
}

CControlUI* CUIBuilderOrginal::CreateControl(xml_node node)
{
	CControlUI *pControl = NULL;
	pControl = CreateControl(node.name());
	if(!pControl)
	{
		xml_attribute attr = node.attribute(_T("custombasedfrom"));
		if(attr)
		{
			pControl = CreateControl(attr.value());
		}
		// 		else //不认识的控件，直接从CContainner创建, 不能这样子哦
		// 		{
		// 			pControl = CreateControl(DUI_CTR_CONTROL);
		// 		}
	}

	return pControl;
}

void CUIBuilder::DeleteControl(CControlUI *pControl)
{
	pControl->Delete();
}
#endif
