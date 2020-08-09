#pragma once

#ifndef DUILIB_VERSION_ORIGINAL
class CUIBuilder
{
public:
	CUIBuilder();
	virtual ~CUIBuilder(void);

	CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
		CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);

	CControlUI* Create(pugi::xml_node xmlnode, IDialogBuilderCallback* pCallback, CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);

	CControlUI* _Parse(pugi::xml_node root, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);

	static CControlUI* CreateControl(LPCTSTR pstrClass);
	static CControlUI* CreateControl(xml_node node);
	static void DeleteControl(CControlUI *pControl);

public:
	void SetWindowAttribute(CPaintManagerUI *pManager, xml_node node);

public:
	xml_document m_xml;
	IDialogBuilderCallback* m_pCallback;
	LPCTSTR m_pstrtype;
	HINSTANCE m_instance;

	bool m_bOpenConfigFile;
private:
};
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
class CUIBuilderOrginal
{
public:
	CUIBuilderOrginal();
	CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
		CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);
	CControlUI* Create(xml_node xmldoc, IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
		CControlUI* pParent = NULL);

	static CControlUI* CreateControl(LPCTSTR pstrClass);
	static CControlUI* CreateControl(xml_node node);
	static void DeleteControl(CControlUI *pControl);
private:
	CControlUI* _Parse(xml_node root, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL);

	xml_document m_xml;
	IDialogBuilderCallback* m_pCallback;
	LPCTSTR m_pstrtype;
};
typedef CUIBuilderOrginal CUIBuilder;
#endif
