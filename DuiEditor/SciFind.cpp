// SciFind.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "SciFind.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "UIManager.h"

static CString s_strFindText;
static CString s_strReplaceText;
static int s_nFindRange;
static BOOL s_bMatchCase	= TRUE;
static BOOL s_bWholeWord	= TRUE;
static BOOL s_bWordStart	= FALSE;
static BOOL s_bRegExp		= FALSE;
static int  s_nFindDirection = FIND_DIRECT_DOWN;
// CSciFind 对话框

IMPLEMENT_DYNAMIC(CSciFind, CDialogEx)

CSciFind::CSciFind(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSciFind::IDD, pParent)
{
	s_nFindRange = 0;
}

CSciFind::~CSciFind()
{
}

void CSciFind::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, s_bMatchCase);
	DDX_Check(pDX, IDC_CHECK2, s_bWholeWord);
	DDX_Check(pDX, IDC_CHECK3, s_bWordStart);
	DDX_Check(pDX, IDC_CHECK4, s_bRegExp);
	DDX_Text(pDX, IDC_COMBO1, s_strFindText);
	DDX_Text(pDX, IDC_COMBO2, s_strReplaceText);
	DDX_CBIndex(pDX, IDC_COMBO3, s_nFindRange);
}


BEGIN_MESSAGE_MAP(CSciFind, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_REPACE_CURRENT, &CSciFind::OnBtnRepaceCurrent)
	ON_BN_CLICKED(IDC_BTN_REPLACE_ALL, &CSciFind::OnBtnReplaceAll)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO3, &CSciFind::OnSelchangeCombo3)
	ON_COMMAND(IDC_RADIO1, &CSciFind::OnRadio1)
	ON_COMMAND(IDC_RADIO2, &CSciFind::OnRadio2)
	ON_COMMAND(IDC_RADIO3, &CSciFind::OnRadio3)
END_MESSAGE_MAP()


// CSciFind 消息处理程序


BOOL CSciFind::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LoadText();

	switch (s_nFindDirection)
	{
	case FIND_DIRECT_DOWN:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		break;
	case FIND_DIRECT_UP:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(1);
		break;
	case FIND_DIRECT_ALL:
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(1);
		break;
	}

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO3);
	pCombo->AddString(_T("当前文档"));
	//pCombo->AddString(_T("所有打开的文档"));
	//pCombo->AddString(_T("当前项目"));
	pCombo->SetCurSel(s_nFindRange);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CSciFind::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT_FIND ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TEXT_REPLACE)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CSciFind::sciFind(CSciWnd *pSciWnd)
{
	CStringA strFindTextA = LST2UTF8(s_strFindText);
	CStringA strReplaceTextA = LST2UTF8(s_strReplaceText);

	Sci_TextToFind ttf;
	ttf.lpstrText = (char *)(const char *)strFindTextA;
	
	//查找方向
	int len = pSciWnd->sci_GetLength();
	int nCurrentPos = pSciWnd->sci_GetCurrentPos();
	if(s_nFindDirection == FIND_DIRECT_DOWN)
	{
		ttf.chrg.cpMin = nCurrentPos;
		ttf.chrg.cpMax = len;
	}
	else if(s_nFindDirection == FIND_DIRECT_UP)
	{
		ttf.chrg.cpMin = nCurrentPos;
		ttf.chrg.cpMax = 0;
	}
	else if(s_nFindDirection == FIND_DIRECT_ALL)
	{
		ttf.chrg.cpMin = 0;
		ttf.chrg.cpMax = len;
	}

	//查找标志
	int searchFlag = 0;
	if(s_bMatchCase)
		searchFlag = searchFlag|SCFIND_MATCHCASE;
	if(s_bWholeWord)
		searchFlag = searchFlag|SCFIND_WHOLEWORD;
	if(s_bWordStart)
		searchFlag = searchFlag|SCFIND_WORDSTART;
	if(s_bRegExp)
		searchFlag = searchFlag|SCFIND_REGEXP;

	int find = pSciWnd->sci_FindText(searchFlag, ttf);
	if(find >= 0)
	{
		if(s_nFindDirection == FIND_DIRECT_DOWN)
		{
			pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
		}
		else if(s_nFindDirection == FIND_DIRECT_UP)
		{
			pSciWnd->sci_SetSel(find+strFindTextA.GetLength(), find);
		}
		else if(s_nFindDirection == FIND_DIRECT_ALL)
		{
			pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CSciFind::sciReplaceCurrent(CSciWnd *pSciWnd)
{
	CStringA strReplaceTextA = LST2UTF8(s_strReplaceText);

	int start = pSciWnd->sci_GetSelectionStart();
	int end = pSciWnd->sci_GetSelectionEnd();
	if(start-end != 0)
	{
		pSciWnd->sci_ReplaceSel(strReplaceTextA);
		return TRUE;
	}
	return FALSE;
}

int CSciFind::sciReplaceAll(CSciWnd *pSciWnd)
{
	CSciUndoBlock undoblock(pSciWnd);
	int nTimes = 0;
	CStringA strReplaceTextA = LST2UTF8(s_strReplaceText);
	for (;;)
	{
		if(!sciFind(pSciWnd))
		{
			break;
		}

		int start = pSciWnd->sci_GetSelectionStart();
		int end = pSciWnd->sci_GetSelectionEnd();
		if(start-end != 0)
		{
			pSciWnd->sci_ReplaceSel(strReplaceTextA);
			nTimes++;
		}
	}
	return nTimes;
}

void CSciFind::OnOK()
{
	if(!UpdateData(TRUE))	return;

	if(s_strFindText.IsEmpty()) return;
	SaveText();

	CSciWnd *pSciWnd = NULL;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(s_nFindRange == FIND_RANGE_CURRENT)
	{
		CUIManager *pUIManager = pMainFrame->GetActiveUIManager();
		if(!pUIManager) return;
		pSciWnd = pUIManager->GetCodeView()->GetSciWnd();
	}
	else if(s_nFindRange == FIND_RANGE_OPENED)
	{

	}
	else if(s_nFindRange == FIND_RANGE_PROJECT)
	{

	}
	

	if(!pSciWnd) return;
	if(sciFind(pSciWnd))
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T(""));
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T("未找到相关内容"));
	}

	//CDialogEx::OnOK();
}


void CSciFind::OnBtnRepaceCurrent()
{
	if(!UpdateData(TRUE)) return;

	if(s_strFindText.IsEmpty()) return;
	if(s_strReplaceText.IsEmpty()) return;
	SaveText();

	CSciWnd *pSciWnd = NULL;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(s_nFindRange == FIND_RANGE_CURRENT)
	{
		CUIManager *pUIManager = pMainFrame->GetActiveUIManager();
		if(!pUIManager) return;
		pSciWnd = pUIManager->GetCodeView()->GetSciWnd();
	}
	else if(s_nFindRange == FIND_RANGE_OPENED)
	{

	}
	else if(s_nFindRange == FIND_RANGE_PROJECT)
	{

	}

	if(!pSciWnd) return;
	sciReplaceCurrent(pSciWnd);
}


void CSciFind::OnBtnReplaceAll()
{
	if(!UpdateData(TRUE))
		return;

	if(s_strFindText.IsEmpty()) return;
	if(s_strReplaceText.IsEmpty()) return;
	SaveText();

	CSciWnd *pSciWnd = NULL;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if(s_nFindRange == FIND_RANGE_CURRENT)
	{
		CUIManager *pUIManager = pMainFrame->GetActiveUIManager();
		if(!pUIManager) return;
		pSciWnd = pUIManager->GetCodeView()->GetSciWnd();
	}
	else if(s_nFindRange == FIND_RANGE_OPENED)
	{

	}
	else if(s_nFindRange == FIND_RANGE_PROJECT)
	{

	}

	if(!pSciWnd) return;
	int nTimes = sciReplaceAll(pSciWnd);

	CString temp;
	temp.Format(_T("共替换 %d 次"), nTimes);
	GetDlgItem(IDC_STATIC_TEXT_REPLACE)->SetWindowText(temp);
}


void CSciFind::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
	DestroyWindow();
}


void CSciFind::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
	DestroyWindow();
}


void CSciFind::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


void CSciFind::OnSelchangeCombo3()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO3);
	s_nFindRange = pCombo->GetCurSel();
}


void CSciFind::OnRadio1()
{
	s_nFindDirection = FIND_DIRECT_DOWN;
}


void CSciFind::OnRadio2()
{
	s_nFindDirection = FIND_DIRECT_UP;
}


void CSciFind::OnRadio3()
{
	s_nFindDirection = FIND_DIRECT_ALL;
}

void CSciFind::SaveText()
{
	if(!s_strFindText.IsEmpty())
	{
		BOOL bSave = TRUE;
		int countText = 0;

		xml_node nodeFind = g_cfg.xml.child_auto(XTEXT("DuiEditor")).child_auto(XTEXT("FindText"));
		for (xml_node node=nodeFind.first_child(); node; node=node.next_sibling())
		{
			if(CompareString(s_strFindText,  node.text().get()))
			{
				bSave = FALSE;
				break;
			}
			countText++;
		}

		if(bSave)
		{
			if(countText >= 10) nodeFind.remove_child(nodeFind.last_child());
			nodeFind.prepend_child(XTEXT("Text")).text().set(T2XML(s_strFindText));
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if(!s_strReplaceText.IsEmpty())
	{
		BOOL bSave = TRUE;
		int countText = 0;

		xml_node nodeReplace = g_cfg.xml.child_auto(XTEXT("DuiEditor")).child_auto(XTEXT("ReplaceText"));
		for (xml_node node=nodeReplace.first_child(); node; node=node.next_sibling())
		{
			if(CompareString(s_strReplaceText,  node.text().get()))
			{
				bSave = FALSE;
				break;
			}
			countText++;
		}

		if(bSave)
		{
			if(countText >= 10) nodeReplace.remove_child(nodeReplace.last_child());	
			nodeReplace.prepend_child(XTEXT("Text")).text().set(T2XML(s_strReplaceText));
		}
	}

	g_cfg.SaveConfig();
}

void CSciFind::LoadText()
{
	CComboBox *pComboFind = (CComboBox *)GetDlgItem(IDC_COMBO1);
	CComboBox *pComboReplace = (CComboBox *)GetDlgItem(IDC_COMBO2);

	xml_node nodeFind = g_cfg.xml.child(XTEXT("DuiEditor")).child(XTEXT("FindText"));
	for (xml_node node=nodeFind.first_child(); node; node=node.next_sibling())
	{
		pComboFind->AddString(XML2T(node.text().get()));
	}

	xml_node nodeReplace = g_cfg.xml.child(XTEXT("DuiEditor")).child(XTEXT("ReplaceText"));
	for (xml_node node=nodeReplace.first_child(); node; node=node.next_sibling())
	{
		pComboReplace->AddString(XML2T(node.text().get()));
	}
}