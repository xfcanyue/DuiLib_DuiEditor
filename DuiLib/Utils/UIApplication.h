#pragma once

//namespace DuiLib{
using namespace DuiLib;

/* _tWinMain里面调用顺序
//////////////////////////////////////////////////////////////////////////

//_CrtSetBreakAlloc(3297);

uiApp.SetAppName(_T("ChargeMachine"));

#ifdef _DEBUG
uiApp.SetResType_File(_T("skin"));
#else
//uiApp.SetResType_ZipResource(_T("skin"), IDR_ZIPRES1);
#endif

if(!uiApp.InitInstance(hInstance)) 
return 0;
uiApp.Run();
uiApp.ExitInstance();
//////////////////////////////////////////////////////////////////////////
*/

class CUIFrameWnd;
class UILIB_API CUIApplication
{
public:
	CUIApplication(void);
	virtual ~CUIApplication(void);

public:
	virtual void SetResType_File(LPCTSTR pstrPath=_T("skin"));
	virtual void SetResType_Resource(LPCTSTR pstrPath, LPCTSTR resID=_T("IDR_RES"), LPCTSTR type=_T("xml"));
	virtual void SetResType_ZIP(LPCTSTR pstrPath, LPCTSTR pstrZip, bool bCachedResourceZip = false, LPCTSTR password = NULL);
	virtual void SetResType_ZipResource(LPCTSTR pstrPath, LPCTSTR password, UINT resID, LPCTSTR type=_T("ZIPRES"));

	virtual bool InitInstance(HINSTANCE hInstance);
	virtual void Run();

public:
	HINSTANCE GetInstance() { return m_hInstance; }
	CUIFrameWnd *GetMainWnd() { return m_pMainWnd; }
	CDuiString GetAppPath() { return m_strAppPath; }

	void SetAppName(LPCTSTR pstrName) { m_strAppName = pstrName; }
	LPCTSTR GetAppName() { return m_strAppName.GetData(); }

	bool SetSingleApplication(bool bSingle, LPCTSTR szGuidName); //只允许运行单一实例
protected:
	virtual int ExitInstance();
	virtual void InitResource();

public:
	CUIFrameWnd *m_pMainWnd;

private:
	HINSTANCE m_hInstance;
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

private:
	bool m_bSingleApplication;
	CDuiString m_GuidAppName;
	HANDLE m_hMutexApplication; //单一实例互斥
public:
	UINT m_UIAPP_SINGLEAPPLICATION_MSG;
};

UILIB_API CUIApplication * __stdcall UIGetApp();

//}