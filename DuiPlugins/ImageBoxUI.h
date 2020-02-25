#pragma once
class CImageBoxUI : public CControlUI
{
	DECLARE_DUICONTROL(CImageBoxUI)
public:
	CImageBoxUI(void);
	virtual ~CImageBoxUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	void DrawFrame(HDC hDC);
	virtual void DoEvent(TEventUI& event);
	bool Activate();

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetRetention(int nRetention) { m_nRetention = nRetention; }
	int  GetRetention() { return m_nRetention; }

	void LoadImageList();
	void ClearImageList();

private:
	UINT m_uButtonState;
	CDuiString m_strImagePath;
	int m_nRetention;

	CStdPtrArray m_arrImages;
	int m_nCurrentFrame;
};

