#pragma once

class UILIB_PLUGIN_API CPictureControlUI : public CControlUI
{
	DECLARE_DUICONTROL(CPictureControlUI)
public:
	CPictureControlUI(void);
	virtual ~CPictureControlUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void Init();
	virtual void SetVisible(bool bVisible = true);
	//virtual void SetInternVisible(bool bVisible = true);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void DoEvent(TEventUI& event);
	virtual bool Activate();
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetFrameDelay(int nDelay);
	int  GetFrameDelay();

	void SetPicture(LPCTSTR fileName); //从硬盘文件绝对路径载入图片,不同于xml的fileimage属性
	void SetPicture(CxImage &img);

	void StartAnim();
	void StopAnim();

	static DWORD LoadImage2Memory(const STRINGorID &bitmap, LPCTSTR type,LPBYTE &pData);
	static bool LoadGifImageX(CxImage &img, STRINGorID bitmap, LPCTSTR type , DWORD mask);

protected:
	struct Imp;
	Imp* m_pImp;

private:
	CxImage *m_pImage;
	int m_type;
	UINT m_uButtonState;

};

