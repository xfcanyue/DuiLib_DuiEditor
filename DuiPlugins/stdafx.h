// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////
// #include "ExcelFile/ExcelFile.h"
// #include "ExcelFile/ExcelFile.cpp"
// #include "ExcelFile/ExcelApplication.h"
// #include "ExcelFile/ExcelBook.h"
// #include "ExcelFile/ExcelBooks.h"
// #include "ExcelFile/ExcelSheet.h"
// #include "ExcelFile/ExcelSheets.h"
// #include "ExcelFile/ExcelRange.h"
// #include "ExcelFile/ExcelRangeEx.h"
// #include "ExcelFile/ExcelRangeEx.cpp"
//#define UILIB_PLUGIN_API __declspec(dllexport)

#include "../Duilib/Duilib.h"
#include "Include/DuiPlugins.h"

#ifdef _UNICODE
	#define PUGIXML_WCHAR_MODE
#endif
#define PUGIXML_HEADER_ONLY
#include "pugixml/pugixml.hpp"
using namespace pugi;

#ifdef UILIB_STATIC
#define CXIMAGE_STATIC
#endif
#include "../3rd/CxImage/Include/image.h"
#include "DuiLib/LsStringConverter.h"

inline DWORD LoadImage2Memory(const STRINGorID &bitmap, LPCTSTR type,LPBYTE &pData)
{
	assert(pData == NULL);
	pData = NULL;
	DWORD dwSize(0U);
	do 
	{
		if( type == NULL )
		{
			CDuiString sFile = CPaintManagerUI::GetResourcePath();
			if( CPaintManagerUI::GetResourceZip().IsEmpty() )
			{
				sFile += bitmap.m_lpstr;
				HANDLE hFile = ::CreateFile(sFile.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
					FILE_ATTRIBUTE_NORMAL, NULL);
				if( hFile == INVALID_HANDLE_VALUE ) break;
				dwSize = ::GetFileSize(hFile, NULL);
				if( dwSize == 0 ) break;

				DWORD dwRead = 0;
				pData = new BYTE[ dwSize + 1 ];
				memset(pData,0,dwSize+1);
				::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
				::CloseHandle( hFile );

				if( dwRead != dwSize ) 
				{
					delete[] pData;
					pData = NULL;
					dwSize = 0U;
					break;
				}
			}
			else 
			{
				sFile += CPaintManagerUI::GetResourceZip();
				HZIP hz = NULL;
				if( CPaintManagerUI::IsCachedResourceZip() ) 
					hz = (HZIP)CPaintManagerUI::GetResourceZipHandle();
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
				if( hz == NULL ) break;
				ZIPENTRY ze; 
				int i = 0; 
				CDuiString key = bitmap.m_lpstr;
				key.Replace(_T("\\"), _T("/")); 
				if( FindZipItem(hz, key, true, &i, &ze) != 0 ) break;
				dwSize = ze.unc_size;
				if( dwSize == 0 ) break;
				pData = new BYTE[ dwSize ];
				int res = UnzipItem(hz, i, pData, dwSize);
				if( res != 0x00000000 && res != 0x00000600)
				{
					delete[] pData;
					pData = NULL;
					dwSize = 0U;
					if( !CPaintManagerUI::IsCachedResourceZip() )
						CloseZip(hz);
					break;
				}
				if( !CPaintManagerUI::IsCachedResourceZip() )
					CloseZip(hz);
			}
		}
		else 
		{
			HINSTANCE hDll = CPaintManagerUI::GetResourceDll();
			HRSRC hResource = ::FindResource(hDll, bitmap.m_lpstr, type);
			if( hResource == NULL ) break;
			HGLOBAL hGlobal = ::LoadResource(hDll, hResource);
			if( hGlobal == NULL ) 
			{
				FreeResource(hResource);
				break;
			}

			dwSize = ::SizeofResource(hDll, hResource);
			if( dwSize == 0 ) break;
			pData = new BYTE[ dwSize ];
			::CopyMemory(pData, (LPBYTE)::LockResource(hGlobal), dwSize);
			::FreeResource(hResource);
		}
	} while (0);

	while (!pData)
	{
		//读不到图片, 则直接去读取bitmap.m_lpstr指向的路径
		HANDLE hFile = ::CreateFile(bitmap.m_lpstr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, \
			FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFile == INVALID_HANDLE_VALUE ) break;
		dwSize = ::GetFileSize(hFile, NULL);
		if( dwSize == 0 ) break;

		DWORD dwRead = 0;
		pData = new BYTE[ dwSize ];
		::ReadFile( hFile, pData, dwSize, &dwRead, NULL );
		::CloseHandle( hFile );

		if( dwRead != dwSize ) 
		{
			delete[] pData;
			pData = NULL;
			dwSize = 0U;
		}
		break;
	}
	return dwSize;
}

inline bool LoadGifImageX(CxImage &img, STRINGorID bitmap, LPCTSTR type , DWORD mask)
{
	LPBYTE pData = NULL;
	DWORD dwSize = LoadImage2Memory(bitmap,type,pData);
	if(dwSize == 0U || !pData)
		return false;

	img.DestroyFrames();
	img.Destroy();

	img.SetRetreiveAllFrames(TRUE);
	if(!img.Decode(pData,dwSize,CXIMAGE_FORMAT_UNKNOWN))
	{
		delete[] pData; pData = NULL;
		return false;
	}

	delete[] pData; pData = NULL;
	return true;
}


extern CString g_strDuiPluginsPath;
