#pragma once

namespace DuiLib 
{

class CUIFrameWnd;
class UILIB_API CUIApplicationBase
{
public:
	CUIApplicationBase(void);
	virtual ~CUIApplicationBase(void);

	virtual void SetAppPath(LPCTSTR sAppPath);
	virtual CDuiString GetAppPath();

	virtual void SetAppName(LPCTSTR pstrName);
	virtual LPCTSTR GetAppName();

	CUIFrameWnd *GetMainWnd() { return m_pMainWnd; }

	virtual void SetResType_File(LPCTSTR pstrPath=_T("skin"));
	virtual void SetResType_Resource(LPCTSTR pstrPath, LPCTSTR resID=_T("IDR_RES"), LPCTSTR type=_T("xml"));
	virtual void SetResType_ZIP(LPCTSTR pstrPath, LPCTSTR pstrZip, bool bCachedResourceZip = false, LPCTSTR password = NULL);
	virtual void SetResType_ZipResource(LPCTSTR pstrPath, LPCTSTR password, UINT resID, LPCTSTR type=_T("ZIPRES"));

#ifdef DUILIB_WIN32
	virtual bool InitInstance(HINSTANCE hInstance) = 0;
#elif defined DUILIB_GTK
	virtual bool InitInstance(int argc, char* argv[]) = 0;
#endif
	virtual void Run() = 0;

protected:
	virtual int ExitInstance() = 0;
	virtual void InitResource();

	CUIFrameWnd *m_pMainWnd;
protected:
	CDuiString m_strAppPath; //应用程序所在路径
	CDuiString m_strAppName; //应用程序名称，主窗口名称

	UILIB_RESTYPE m_resType;
	CDuiString m_strSkinPath;

	CDuiString m_idrID;
	CDuiString m_idrType;

	CDuiString m_strZipFile;
	bool m_bCachedResourceZip;
	CDuiString m_zipPassword;

	UINT m_uZipResourceID;
	CDuiString m_strZipResourceType;
};

UILIB_API CUIApplicationBase * __stdcall UIGetApp();

}  //namespace DuiLib {
