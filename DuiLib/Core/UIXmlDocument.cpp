#include "StdAfx.h"
#include "LsStringConverter.h"

#define PUGIXML_HEADER_ONLY
#ifdef _UNICODE
#define PUGIXML_WCHAR_MODE
#endif
#include "../Utils/pugixml/pugixml.hpp"
using namespace pugi;

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
inline xml_document *impxmldoc(PVOID pdoc) { return (xml_document *)pdoc; }

CXmlDocumentUI::CXmlDocumentUI()
{
	_xml_document = new xml_document;
}

CXmlDocumentUI::~CXmlDocumentUI()
{
	delete (xml_document *)_xml_document;
}

bool CXmlDocumentUI::Load(LPCTSTR pstrXML)
{
	xml_parse_result ret = impxmldoc(_xml_document)->load_string(pstrXML, pugi::parse_full);
	if(ret.status != pugi::status_ok)
	{
		LSSTRING_CONVERSION;
		return _Failed(LSA2T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::LoadFromMem(BYTE* pByte, DWORD dwSize)
{
	int x = sizeof(xml_node_struct);
#ifdef _UNICODE
	xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(pByte, dwSize, pugi::parse_full);
#else
	LSSTRING_CONVERSION;
	const char *pstrAscii = LSUTF82A((const char *)pByte);
	xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(pstrAscii, strlen(pstrAscii), pugi::parse_full);
#endif

	if(ret.status != pugi::status_ok)
	{
		LSSTRING_CONVERSION;
		return _Failed(LSA2T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::LoadFromFile(LPCTSTR pstrFilename)
{
	CDuiString sFile = CPaintManagerUI::GetResourcePath();
	if( CPaintManagerUI::GetResourceZip().IsEmpty() ) {
		sFile += pstrFilename;
		HANDLE hFile = ::CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFile == INVALID_HANDLE_VALUE ) return _Failed(_T("Error opening file"));
		DWORD dwSize = ::GetFileSize(hFile, NULL);
		if( dwSize == 0 ) return _Failed(_T("File is empty"));
		if ( dwSize > 4096*1024 ) return _Failed(_T("File too large"));

		DWORD dwRead = 0;
		BYTE* pByte = new BYTE[ dwSize ];
		::ReadFile( hFile, pByte, dwSize, &dwRead, NULL );
		::CloseHandle( hFile );
		if( dwRead != dwSize ) {
			delete[] pByte;
			pByte = NULL;
			return _Failed(_T("Could not read file"));
		}

		bool ret = LoadFromMem(pByte, dwSize);
		delete[] pByte;
		pByte = NULL;

		return ret;
	}
	else {
		sFile += CPaintManagerUI::GetResourceZip();
		HZIP hz = NULL;
		if( CPaintManagerUI::IsCachedResourceZip() ) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
		else {
			CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
#ifdef UNICODE
			char* pwd = w2a((wchar_t*)sFilePwd.GetData());
			hz = OpenZip(sFile.GetData(), pwd);
			if(pwd) delete[] pwd;
#else
			hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
		}
		if( hz == NULL ) return _Failed(_T("Error opening zip file"));
		ZIPENTRY ze; 
		int i = 0; 
		CDuiString key = pstrFilename;
		key.Replace(_T("\\"), _T("/"));
		if( FindZipItem(hz, key, true, &i, &ze) != 0 ) return _Failed(_T("Could not find ziped file"));
		DWORD dwSize = ze.unc_size;
		if( dwSize == 0 ) return _Failed(_T("File is empty"));
		if ( dwSize > 4096*1024 ) return _Failed(_T("File too large"));
		BYTE* pByte = new BYTE[ dwSize ];
		int res = UnzipItem(hz, i, pByte, dwSize);
		if( res != 0x00000000 && res != 0x00000600) {
			delete[] pByte;
			if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
			return _Failed(_T("Could not unzip file"));
		}
		if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
		bool ret = LoadFromMem(pByte, dwSize);
		delete[] pByte;
		pByte = NULL;
		return ret;
	}

	return false;
}

bool CXmlDocumentUI::_Failed(LPCTSTR pstrError)
{
	m_szErrorMsg = pstrError;
	return false; // Always return 'false'
}

} // namespace DuiLib
