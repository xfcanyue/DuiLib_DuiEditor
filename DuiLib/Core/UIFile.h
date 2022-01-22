#ifndef __UIFILE_H__
#define __UIFILE_H__

#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CUIFile
	{
	public:
		CUIFile();
		~CUIFile();

		LPBYTE GetData() const;
		DWORD  GetSize() const;
		void Empty();

		BOOL LoadFile(const STRINGorID &bitmap, LPCTSTR type=NULL, HINSTANCE instance=NULL);
		BOOL LoadFile(LPCTSTR pStrImage, LPCTSTR type=NULL, HINSTANCE instance=NULL);
		BOOL LoadFile(UINT nID, LPCTSTR type=NULL, HINSTANCE instance=NULL);

	private:
		LPBYTE m_pData; 
		DWORD m_dwSize;
	};

} // namespace DuiLib

#endif // __UIFILE_H__
