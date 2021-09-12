#ifndef __UIXMLDOCUMENT_H__
#define __UIXMLDOCUMENT_H__

#pragma once

namespace DuiLib {

	class UILIB_API CXmlDocumentUI : public CXmlNodeUI
	{
	public:
		CXmlDocumentUI();
		~CXmlDocumentUI();

		bool load_string(LPCTSTR pstrXML);
		bool load_buffer(const void* contents, size_t size);
		bool load_file(LPCTSTR pstrFilename);

		bool save_file(LPCTSTR path);
		bool save_to_default_file(); //保存为载入时的文件名

		CDuiString GetError();
	protected:
		bool _Failed(LPCTSTR pstrError);

	private:
		PVOID _xml_document; //xml_document *
		CDuiString m_sFileName;
		CDuiString m_sErrorMsg;
	};

} // namespace DuiLib

#endif // __XMLDOCUMENT_H__
