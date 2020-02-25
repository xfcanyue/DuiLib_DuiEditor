// SciFind.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "SciFind.h"
#include "afxdialogex.h"


static CString s_strFindText;
static CString s_strReplaceText;
static BOOL s_bMatchCase	= FALSE;
static BOOL s_bWholeWord	= FALSE;
static BOOL s_bWordStart	= FALSE;
static BOOL s_bRegExp		= FALSE;
static int  s_nFindDirection = 0;
// CSciFind 对话框

IMPLEMENT_DYNAMIC(CSciFind, CDialogEx)

CSciFind::CSciFind(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSciFind::IDD, pParent)
{

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
}


BEGIN_MESSAGE_MAP(CSciFind, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_REPACE_CURRENT, &CSciFind::OnBtnRepaceCurrent)
	ON_BN_CLICKED(IDC_BTN_REPLACE_ALL, &CSciFind::OnBtnReplaceAll)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_FIND_NEXT, &CSciFind::OnBnClickedBtnFindNext)
	ON_BN_CLICKED(IDC_BTN_FIND_PRE, &CSciFind::OnBnClickedBtnFindPre)
END_MESSAGE_MAP()


// CSciFind 消息处理程序


BOOL CSciFind::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	switch (s_nFindDirection)
	{
	case 0:
		((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		break;
	case 1:
		((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(1);
		break;
	case 2:
		((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(1);
		break;
	}

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


void CSciFind::OnOK()
{
	if(!UpdateData(TRUE))	return;
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
		s_nFindDirection = 0;
	if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
		s_nFindDirection = 1;
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
		s_nFindDirection = 2;

	CStringA strFindTextA = CStringToolExt::CStrT2CStrA(s_strFindText);
	CStringA strReplaceTextA = CStringToolExt::CStrT2CStrA(s_strReplaceText);

	Sci_TextToFind ttf;
	ttf.lpstrText = (char *)(const char *)strFindTextA;

	//查找方向
	int len = m_pSciWnd->sci_GetLength();
	int nCurrentPos = m_pSciWnd->sci_GetCurrentPos();
	if(s_nFindDirection == 0)
	{
		ttf.chrg.cpMin = nCurrentPos;
		ttf.chrg.cpMax = len;
	}
	else if(s_nFindDirection == 1)
	{
		ttf.chrg.cpMin = nCurrentPos;
		ttf.chrg.cpMax = 0;
	}
	else
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

	int find = m_pSciWnd->sci_FindText(searchFlag, ttf);
	if(find == -1)
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T("未找到相关内容"));
		return;
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T(""));

		m_pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
	}

	//CDialogEx::OnOK();
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


void CSciFind::OnBtnRepaceCurrent()
{
	if(!UpdateData(TRUE))
		return;

	CStringA strReplaceTextA = CStringToolExt::CStrT2CStrA(s_strReplaceText);
	if(strReplaceTextA.IsEmpty())	
		return;

	int start = m_pSciWnd->sci_GetSelectionStart();
	int end = m_pSciWnd->sci_GetSelectionEnd();
	if(start-end != 0)
	{
		m_pSciWnd->sci_ReplaceSel(strReplaceTextA);
	}
}


void CSciFind::OnBtnReplaceAll()
{
	if(!UpdateData(TRUE))
		return;
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
		s_nFindDirection = 0;
	if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
		s_nFindDirection = 1;
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
		s_nFindDirection = 2;

	CStringA strFindTextA = CStringToolExt::CStrT2CStrA(s_strFindText);
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T("查找内容不允许为空"));
		return;
	}
	CStringA strReplaceTextA = CStringToolExt::CStrT2CStrA(s_strReplaceText);
	if(strReplaceTextA.IsEmpty())	
	{
		GetDlgItem(IDC_STATIC_TEXT_REPLACE)->SetWindowText(_T("替换内容不允许为空"));
		return;
	}

	Sci_TextToFind ttf;
	ttf.lpstrText = (char *)(const char *)strFindTextA;
	ttf.chrg.cpMin = 0;
	ttf.chrg.cpMax = m_pSciWnd->sci_GetLength();

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

	int nTimes = 0;
	for (;;)
	{
		int find = m_pSciWnd->sci_FindText(searchFlag, ttf);
		if(find == -1)
		{
			break;
		}

		m_pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
		int start = m_pSciWnd->sci_GetSelectionStart();
		int end = m_pSciWnd->sci_GetSelectionEnd();
		if(start-end != 0)
		{
			m_pSciWnd->sci_ReplaceSel(strReplaceTextA);
			nTimes++;
		}
	}

	CString temp;
	temp.Format(_T("共替换 %d 次"), nTimes);
	GetDlgItem(IDC_STATIC_TEXT_REPLACE)->SetWindowText(temp);
}



void CSciFind::OnBnClickedBtnFindNext()
{
	if(!UpdateData(TRUE))	return;
	CStringA strFindTextA = CStringToolExt::CStrT2CStrA(s_strFindText);
	CStringA strReplaceTextA = CStringToolExt::CStrT2CStrA(s_strReplaceText);

	Sci_TextToFind ttf;
	ttf.lpstrText = (char *)(const char *)strFindTextA;

	//查找方向
	int len = m_pSciWnd->sci_GetLength();
	int nCurrentPos = m_pSciWnd->sci_GetCurrentPos();
	ttf.chrg.cpMin = nCurrentPos;
	ttf.chrg.cpMax = len;

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

	int find = m_pSciWnd->sci_FindText(searchFlag, ttf);
	if(find == -1)
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T("未找到相关内容"));
		return;
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T(""));

		m_pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
	}
}


void CSciFind::OnBnClickedBtnFindPre()
{
	if(!UpdateData(TRUE))	return;
	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck())
		s_nFindDirection = 0;
	if(((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
		s_nFindDirection = 1;
	if(((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck())
		s_nFindDirection = 2;

	CStringA strFindTextA = CStringToolExt::CStrT2CStrA(s_strFindText);
	CStringA strReplaceTextA = CStringToolExt::CStrT2CStrA(s_strReplaceText);

	Sci_TextToFind ttf;
	ttf.lpstrText = (char *)(const char *)strFindTextA;

	//查找方向
	int len = m_pSciWnd->sci_GetLength();
	int nCurrentPos = m_pSciWnd->sci_GetCurrentPos();
	ttf.chrg.cpMin = nCurrentPos-1;
	ttf.chrg.cpMax = 0;

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

	int find = m_pSciWnd->sci_FindText(searchFlag, ttf);
	if(find == -1)
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T("未找到相关内容"));
		return;
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT_FIND)->SetWindowText(_T(""));

		m_pSciWnd->sci_SetSel(find, find+strFindTextA.GetLength());
	}
}
