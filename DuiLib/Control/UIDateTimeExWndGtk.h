#pragma once

#ifdef DUILIB_GTK
namespace DuiLib
{

class CDateTimeExWndGtk : public CWindowWnd
{
public:
	CDateTimeExWndGtk();
	~CDateTimeExWndGtk();

	void Init(CDateTimeExUI* pOwner, UINT uFormatStyle);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(UIWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CDateTimeExUI* m_pOwner;
	bool m_bInit;
	bool m_bDropOpen;
	SYSTEMTIME m_oldSysTime;
	UINT m_uFormatStyle;
	SYSTEMTIME m_dtTemp;
	CStdRefPtr<UIFont> m_font;
};

}
#endif // #ifdef DUILIB_GTK

