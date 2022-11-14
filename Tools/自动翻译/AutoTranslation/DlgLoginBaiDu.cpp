#include "stdafx.h"
#include "DlgLoginBaiDu.h"

CDlgLoginBaiDu::CDlgLoginBaiDu()
{
	m_sAppid = uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("BaiDu")).attribute_auto(_T("appid")).as_string();
	m_sKey = uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("BaiDu")).attribute_auto(_T("key")).as_string();
}

CDlgLoginBaiDu::~CDlgLoginBaiDu()
{
}

void CDlgLoginBaiDu::InitWindow()
{
	ddxSetManager(GetManager());
	ddxText(_T("text_appid"), m_sAppid);
	ddxText(_T("text_key"), m_sKey);
	UpdateDataUI(false);
}

void CDlgLoginBaiDu::OnClickOK()
{
	UpdateDataUI(true);

	uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("BaiDu")).attribute_auto(_T("appid")).set_value(m_sAppid);
	uiApp.Config.GetRoot().child_auto(_T("Config")).child_auto(_T("BaiDu")).attribute_auto(_T("key")).set_value(m_sKey);

	__super::OnClickOK();
}