#pragma once

namespace DuiLib
{

class CDateTimeExWnd : public CWindowWnd
{
public:
	CDateTimeExWnd();
	~CDateTimeExWnd();

	void Init(CDateTimeExUI* pOwner, UINT uFormatStyle);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CDateTimeExUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
	bool m_bDropOpen;
	SYSTEMTIME m_oldSysTime;
	UINT m_uFormatStyle;
	SYSTEMTIME m_dtTemp;
	CStdRefPtr<UIFont> m_font;
};

}