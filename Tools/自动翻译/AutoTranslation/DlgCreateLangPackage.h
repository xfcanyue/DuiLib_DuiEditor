#pragma once
class CDlgCreateLangPackage : public CUIDialog, public CUIDataExchange
{
public:
	CDlgCreateLangPackage();
	virtual ~CDlgCreateLangPackage();
	virtual LPCTSTR GetWindowClassName() const { return _T("DlgCreateLangPackage"); }
	virtual CDuiString GetSkinFile() { return _T("DlgCreateLangPackage.xml"); }

	virtual void InitWindow();
	virtual void OnNotifyClick(TNotifyUI& msg);

	virtual void OnClickOK();

	void CopyFromPath(LPCTSTR sBaseFolder, LPCTSTR newFolder, LPCTSTR sParentPath);
	void UpdateXmlFile(LPCTSTR sBaseFile, LPCTSTR sNewFile);

	BOOL m_bFolder;
	CDuiString m_sFolder;

	BOOL m_bFile;
	CDuiString m_sFile;

	COptionUI *m_opt_folder;
	COptionUI *m_opt_file;
	CEditUI *m_edit_path;
	CEditUI *m_edit_file;
};

