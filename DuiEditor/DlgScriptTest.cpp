// DlgScriptTest.cpp : 实现文件
//

#include "stdafx.h"
#include "DuiEditor.h"
#include "DlgScriptTest.h"
#include "afxdialogex.h"


// CDlgScriptTest 对话框
COleDateTime CDlgScriptTest::m_t1(2021, 1, 1, 0, 0, 0);
COleDateTime CDlgScriptTest::m_t2(2021, 1, 1, 0, 0, 0);

IMPLEMENT_DYNAMIC(CDlgScriptTest, CDialogEx)

CDlgScriptTest::CDlgScriptTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgScriptTest::IDD, pParent)
{
	m_dtBeginDate = m_t1;
	m_dtBeginTime = m_t1;
	m_dtEndDate = m_t2;
	m_dtEndTime = m_t2;
}

CDlgScriptTest::~CDlgScriptTest()
{
}

void CDlgScriptTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dtBeginDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dtBeginTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_dtEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_dtEndTime);
}


BEGIN_MESSAGE_MAP(CDlgScriptTest, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgScriptTest::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgScriptTest 消息处理程序


void CDlgScriptTest::OnBnClickedOk()
{
	if(!UpdateData(TRUE))	return;

	m_t1.SetDateTime(m_dtBeginDate.GetYear(), 
		m_dtBeginDate.GetMonth(), 
		m_dtBeginDate.GetDay(),
		m_dtBeginTime.GetHour(),
		m_dtBeginTime.GetMinute(),
		m_dtBeginTime.GetSecond());
	m_t2.SetDateTime(m_dtEndDate.GetYear(), 
		m_dtEndDate.GetMonth(), 
		m_dtEndDate.GetDay(),
		m_dtEndTime.GetHour(),
		m_dtEndTime.GetMinute(),
		m_dtEndTime.GetSecond());

	CDialogEx::OnOK();
}
