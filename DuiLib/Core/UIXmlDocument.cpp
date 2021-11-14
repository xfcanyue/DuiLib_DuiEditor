#include "StdAfx.h"

namespace DuiLib {
///////////////////////////////////////////////////////////////////////////////////////
//
//
//
inline ui_pugi::xml_document *impxmldoc(PVOID pdoc) { return (ui_pugi::xml_document *)pdoc; }

CXmlDocumentUI::CXmlDocumentUI()
{
	_xml_document = new ui_pugi::xml_document;
}

CXmlDocumentUI::~CXmlDocumentUI()
{
	delete (ui_pugi::xml_document *)_xml_document;
}

bool CXmlDocumentUI::load_string(LPCTSTR pstrXML)
{
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_string(pstrXML, ui_pugi::parse_full);
	if(ret.status != ui_pugi::status_ok)
	{
		_root = impxmldoc(_xml_document)->root().internal_object();
		LSSTRING_CONVERSION;
		return _Failed(LSUTF82T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::load_buffer(const void* contents, size_t size)
{
	LSSTRING_CONVERSION;
#ifdef _UNICODE
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(contents, size, ui_pugi::parse_full);
#else
	LPCTSTR strContent = LSUTF82T(contents);
	ui_pugi::xml_parse_result ret = impxmldoc(_xml_document)->load_buffer(strContent, _tcslen(strContent), ui_pugi::parse_full);
#endif
	
	if(ret.status != ui_pugi::status_ok)
	{
		_root = impxmldoc(_xml_document)->root().internal_object();
		return _Failed(LSUTF82T(ret.description()));
	}
	_root = impxmldoc(_xml_document)->root().internal_object();
	return true;
}

bool CXmlDocumentUI::load_file(LPCTSTR pstrFilename)
{
// 	m_sFileName = pstrFilename;
// 
// 	CDuiString sFile = CPaintManagerUI::GetResourcePath();
// 	if( CPaintManagerUI::GetResourceZip().IsEmpty() ) {
// 		sFile += pstrFilename;
// 		HANDLE hFile = ::CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 		if( hFile == INVALID_HANDLE_VALUE ) return _Failed(_T("Error opening file"));
// 		DWORD dwSize = ::GetFileSize(hFile, NULL);
// 		if( dwSize == 0 ) return _Failed(_T("File is empty"));
// 		if ( dwSize > 4096*1024 ) return _Failed(_T("File too large"));
// 
// 		DWORD dwRead = 0;
// 		BYTE* pByte = new BYTE[ dwSize + 1 ];
// 		pByte[dwSize] = '\0';
// 		::ReadFile( hFile, pByte, dwSize, &dwRead, NULL );
// 		::CloseHandle( hFile );
// 		if( dwRead != dwSize ) {
// 			delete[] pByte;
// 			pByte = NULL;
// 			return _Failed(_T("Could not read file"));
// 		}
// 
// 		bool ret = load_buffer(pByte, dwSize);
// 		delete[] pByte;
// 		pByte = NULL;
// 
// 		return ret;
// 	}
// 	else {
// 		sFile += CPaintManagerUI::GetResourceZip();
// 		HZIP hz = NULL;
// 		if( CPaintManagerUI::IsCachedResourceZip() ) hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
// 		else {
// 			CDuiString sFilePwd = CPaintManagerUI::GetResourceZipPwd();
// 			LSSTRING_CONVERSION;
// 			hz = OpenZip(sFile.GetData(), LST2A(sFilePwd.GetData()));
// 		}
// 		if( hz == NULL ) return _Failed(_T("Error opening zip file"));
// 		ZIPENTRY ze; 
// 		int i = 0; 
// 		CDuiString key = pstrFilename;
// 		key.Replace(_T("\\"), _T("/"));
// 		if( FindZipItem(hz, key, true, &i, &ze) != 0 ) return _Failed(_T("Could not find ziped file"));
// 		DWORD dwSize = ze.unc_size;
// 		if( dwSize == 0 ) return _Failed(_T("File is empty"));
// 		if ( dwSize > 4096*1024 ) return _Failed(_T("File too large"));
// 		BYTE* pByte = new BYTE[ dwSize + 1];
// 		pByte[dwSize] = '\0';
// 		int res = UnzipItem(hz, i, pByte, dwSize);
// 		if( res != 0x00000000 && res != 0x00000600) {
// 			delete[] pByte;
// 			if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
// 			return _Failed(_T("Could not unzip file"));
// 		}
// 		if( !CPaintManagerUI::IsCachedResourceZip() ) CloseZip(hz);
// 		bool ret = load_buffer(pByte, dwSize);
// 		delete[] pByte;
// 		pByte = NULL;
// 		return ret;
// 	}

	m_sFileName = pstrFilename;

	LPBYTE pByte = NULL;
	DWORD dwSize = CRenderEngine::LoadImage2Memory(pstrFilename, NULL, pByte);
	//if(!pByte || dwSize == 0) return false;

	bool ret = load_buffer(pByte, dwSize);
	if(pByte) { delete []pByte; pByte = NULL; }
	return ret;
}


bool CXmlDocumentUI::save_file(LPCTSTR path)
{
	return impxmldoc(_xml_document)->save_file(path, _T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
}

bool CXmlDocumentUI::save_to_default_file()
{
	return impxmldoc(_xml_document)->save_file(m_sFileName.GetData(), _T("\t"), ui_pugi::format_default,  ui_pugi::encoding_utf8);
}

CDuiString CXmlDocumentUI::GetError()
{
	return m_sErrorMsg;
}

bool CXmlDocumentUI::_Failed(LPCTSTR pstrError)
{
	m_sErrorMsg = pstrError;
	return false; // Always return 'false'
}

} // namespace DuiLib
