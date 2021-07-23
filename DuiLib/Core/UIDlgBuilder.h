#ifndef __UIDLGBUILDER_H__
#define __UIDLGBUILDER_H__

#pragma once

namespace DuiLib {

	class IDialogBuilderCallback
	{
	public:
		virtual CControlUI* CreateControl(LPCTSTR pstrClass) = 0;
	};


	class UILIB_API CDialogBuilder
	{
	public:
		CDialogBuilder();
		CControlUI* Create(STRINGorID xml, LPCTSTR type = NULL, IDialogBuilderCallback* pCallback = NULL,
			CPaintManagerUI* pManager = NULL, CControlUI* pParent = NULL);
		CControlUI* Create(IDialogBuilderCallback* pCallback = NULL, CPaintManagerUI* pManager = NULL,
			CControlUI* pParent = NULL, CLangPackageUI *pkg = NULL);

	    void SetInstance(HINSTANCE instance){ m_instance = instance;};
	private:
		CControlUI* _Parse(CXmlNodeUI root, CControlUI* pParent = NULL, CPaintManagerUI* pManager = NULL, CLangPackageUI *pkg = NULL);

		CXmlDocumentUI m_xml;
		IDialogBuilderCallback* m_pCallback;
		LPCTSTR m_pstrtype;
    	HINSTANCE m_instance;
	};

} // namespace DuiLib

#endif // __UIDLGBUILDER_H__
