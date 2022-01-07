#include "StdAfx.h"
#include "UIPicture.h"

namespace DuiLib
{

IMPLEMENT_DUICONTROL(CPictureBoxUI)
//////////////////////////////////////////////////////////////////////////
CPictureBoxUI::CPictureBoxUI(void)
{
	
}


CPictureBoxUI::~CPictureBoxUI(void)
{
	
}

LPCTSTR	CPictureBoxUI::GetClass() const
{
	return _T("PictureBoxUI");
}

LPVOID	CPictureBoxUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_PICTUREBOX) == 0 ) return static_cast<CPictureBoxUI*>(this);
	return __super::GetInterface(pstrName);
}

void CPictureBoxUI::DoInit()
{
	
}

void CPictureBoxUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcsicmp(pstrName, _T("imagepath")) == 0 ) 
	{	
		RemoveAllImages();
		CDuiString strPath = GetManager()->GetResourcePath() + pstrValue;
		if(strPath[strPath.GetLength()-1] != '\\') strPath += _T("\\");

		WIN32_FIND_DATA FindData;  
		HANDLE hFind;  
		int FileCount = 0;  
		hFind = FindFirstFile(strPath + _T("*.*"), &FindData);  
		if (hFind == INVALID_HANDLE_VALUE)  
		{    
			return;  
		}  

		while (::FindNextFile(hFind, &FindData))
		{
			if((_tcsicmp(FindData.cFileName, _T(".")) == 0) ||
				(_tcsicmp(FindData.cFileName, _T("..")) == 0) )
				continue;

			if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			CDuiString fileName = FindData.cFileName;
			int po = fileName.ReverseFind('.');
			if(po < 0)	
				continue;

			CDuiString ExtName = fileName.Right(fileName.GetLength() - po - 1);
			if((_tcsicmp(ExtName, _T("jpg")) == 0) ||
				(_tcsicmp(ExtName, _T("bmp")) == 0) ||
				(_tcsicmp(ExtName, _T("png")) == 0) )
			{
				LoadFile(strPath + FindData.cFileName);
			}
		}
	}
	else
		__super::SetAttribute(pstrName, pstrValue);
}


bool CPictureBoxUI::LoadHBitmap(HBITMAP hBitmap)
{
	return __SetHBitmap(hBitmap);
}

bool CPictureBoxUI::LoadImageFromMemory(LPBYTE pData, DWORD dwSize)
{
	return __LoadImageFromMemory(pData, dwSize);
}

bool CPictureBoxUI::LoadFile(LPCTSTR pstrImage)
{
	return __LoadFile(pstrImage);
}


}