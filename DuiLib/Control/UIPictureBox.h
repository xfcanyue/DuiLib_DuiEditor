#pragma once

namespace DuiLib
{

class UILIB_API CPictureBoxUI : public CPictureUI
{
	DECLARE_DUICONTROL(CPictureBoxUI)
public:
	CPictureBoxUI(void);
	virtual ~CPictureBoxUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	virtual bool LoadHBitmap(HBITMAP hBitmap);
	virtual bool LoadImageFromMemory(LPBYTE pData, DWORD dwSize);
	virtual bool LoadFile(LPCTSTR pstrImage);
};

}