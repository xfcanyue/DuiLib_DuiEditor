#pragma once

namespace DuiLib {

/* _tWinMain里面调用顺序
//////////////////////////////////////////////////////////////////////////

//_CrtSetBreakAlloc(3297);

uiApp.SetAppName(_T("AppTitle"));

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

class UILIB_API CUIAppRegistryKey
{
public:
	CUIAppRegistryKey();
	virtual ~CUIAppRegistryKey();

	// Set registry key name
	// 1, 使用注册表, 如: SetRegistryKey(_T("Microsoft"), GetAppName()); 一般习惯方式，第一参数是公司名，第二参数是软件名。
	// 2, 使用ini文件, 如: SetRegistryKey(NULL, GetAppPath() + GetAppName() + _T(".ini"));
	void SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszAppName);

	// Retrieve an integer value from INI file or registry.
	virtual UINT ReadInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);

	// Sets an integer value to INI file or registry.
	virtual BOOL WriteInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

	// Retrieve a string value from INI file or registry.
	virtual CDuiString ReadString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);

	// Sets a string value to INI file or registry.
	virtual BOOL WriteString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);

	// Retrieve an arbitrary binary value from INI file or registry.
	virtual BOOL ReadBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);

	// Sets an arbitrary binary value to INI file or registry.
	virtual BOOL WriteBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);

	//...int
	virtual BOOL GetValue(LPCTSTR lpszSection, LPCTSTR lpszEntry, int &nValue, int nDefault = 0);
	virtual BOOL SetValue(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

	//....string
	virtual BOOL GetValue(LPCTSTR lpszSection, LPCTSTR lpszEntry, CDuiString &strValue, LPCTSTR lpszDefault = NULL);
	virtual BOOL SetValue(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR strValue);

protected:
	// returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName.
	HKEY GetAppRegistryKey();

	// returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\lpszSection.
	HKEY GetSectionKey(LPCTSTR lpszSection);

private:
	// Name of registry key for this application. See SetRegistryKey() member function.
	LPCTSTR m_pszRegistryKey;

	// Default based on this application's name.
	LPCTSTR m_pszProfileName;
};

//保存和载入控件数据，可以当初配置文件使用。
class UILIB_API CUIAppConfig
{
public:
	CUIAppConfig();
	virtual ~CUIAppConfig();

	CXmlNodeUI GetRoot();

	//默认载入程序目录下的AppName.xml
	void LoadConfig(LPCTSTR szPathName = NULL);
	void SaveConfig();

	BOOL SaveControl(CXmlNodeUI nodeParent, CEditUI *pControl);
	BOOL LoadControl(CXmlNodeUI nodeParent, CEditUI *pControl, LPCTSTR szDefault = _T(""));

	BOOL SaveControl(CXmlNodeUI nodeParent, CRichEditUI *pControl);
	BOOL LoadControl(CXmlNodeUI nodeParent, CRichEditUI *pControl, LPCTSTR szDefault = _T(""));

	BOOL SaveControl(CXmlNodeUI nodeParent, CComboUI *pControl, BOOL bSaveItems = FALSE);
	BOOL LoadControl(CXmlNodeUI nodeParent, CComboUI *pControl, LPCTSTR szDefault = _T(""), BOOL bLoadItems = FALSE);

	BOOL SaveControl(CXmlNodeUI nodeParent, CComboExUI *pControl, BOOL bSaveItems = FALSE);
	BOOL LoadControl(CXmlNodeUI nodeParent, CComboExUI *pControl, LPCTSTR szDefault = _T(""), BOOL bLoadItems = FALSE);

	BOOL SaveControl(CXmlNodeUI nodeParent, COptionUI *pControl);
	BOOL LoadControl(CXmlNodeUI nodeParent, COptionUI *pControl, BOOL bDefault = FALSE);

	BOOL SaveControl(CXmlNodeUI nodeParent, CTabLayoutUI *pControl);
	BOOL LoadControl(CXmlNodeUI nodeParent, CTabLayoutUI *pControl, int nDefault = 0);

private:
	CXmlDocumentUI m_xml;
};

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

	// Set registry key name
	// 1, 使用注册表, 如: SetRegistryKey(_T("Microsoft"), GetAppName()); 一般习惯方式，第一参数是公司名，第二参数是软件名。
	// 2, 使用ini文件, 如: SetRegistryKey(NULL, GetAppPath() + GetAppName() + _T(".ini"));
	void SetRegistryKey(LPCTSTR lpszRegistryKey, LPCTSTR lpszAppName);

	// Retrieve an integer value from INI file or registry.
	virtual UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);

	// Sets an integer value to INI file or registry.
	virtual BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

	// Retrieve a string value from INI file or registry.
	virtual CDuiString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);

	// Sets a string value to INI file or registry.
	virtual BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);

	// Retrieve an arbitrary binary value from INI file or registry.
	virtual BOOL GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);

	// Sets an arbitrary binary value to INI file or registry.
	virtual BOOL WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);
protected:
	virtual int ExitInstance();
	virtual void InitResource();

public:
	CUIAppRegistryKey RegistryKey;
	CUIAppConfig Config;
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

}  //namespace DuiLib {