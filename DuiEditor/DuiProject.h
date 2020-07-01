#pragma once
class CDuiProject
{
public:
	CDuiProject(void);
	~CDuiProject(void);

	void InitProject(LPCTSTR szFolderPath);
	void Save();

	CString GetProjectPath();
	CString GetStartupFile();

	void SetStartupFile(LPCTSTR fileName);

	void LoadConfig(CPaintManagerUI* pManager);
	BOOL IsLoadedConfig();

private:
	xml_document projFile;
	BOOL m_bHasLoadConfig;
};

