#ifndef __UIXMLDOCUMENT_H__
#define __UIXMLDOCUMENT_H__

#pragma once

namespace DuiLib {

	class UILIB_API CXmlDocumentUI : public CXmlNodeUI
	{
	public:
		CXmlDocumentUI();
		~CXmlDocumentUI();

		bool Load(LPCTSTR pstrXML);
		bool LoadFromMem(BYTE* pByte, DWORD dwSize);
		bool LoadFromFile(LPCTSTR pstrFilename);

		bool _Failed(LPCTSTR pstrError);

		PVOID _xml_document; //xml_document *
		CDuiString m_szErrorMsg;
	};

} // namespace DuiLib

#endif // __XMLDOCUMENT_H__
