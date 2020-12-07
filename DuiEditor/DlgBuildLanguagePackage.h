#pragma once


// CDlgBuildLanguagePackage 对话框
class CUIManager;
class CDlgBuildLanguagePackage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBuildLanguagePackage)

public:
	CDlgBuildLanguagePackage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBuildLanguagePackage();

// 对话框数据
	enum { IDD = IDD_BUILD_LANG_PACKAGE };
	CSciWnd sci;

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }

private:
	CUIManager *m_pUIManager;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void _buildLangPackage(xml_node rootLang, xml_node nodeSkin);
};
