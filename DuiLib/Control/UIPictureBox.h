#pragma once

#ifdef DUILIB_WIN32
namespace DuiLib
{

class UILIB_API CPictureBoxUI : public CPictureUI
{
	DECLARE_DUICONTROL(CPictureBoxUI)
public:
	CPictureBoxUI(void);
	virtual ~CPictureBoxUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID	GetInterface(LPCTSTR pstrName) override;

	virtual void DoInit() override;
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

	virtual bool LoadHBitmap(HBITMAP hBitmap);
	virtual bool LoadImageFromMemory(LPBYTE pData, DWORD dwSize);
	virtual bool LoadFile(LPCTSTR pstrImage);
};

}
#endif //#ifdef DUILIB_WIN32