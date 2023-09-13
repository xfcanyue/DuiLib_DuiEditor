#include "StdAfx.h"

#ifdef DUILIB_GTK
namespace DuiLib{

CUIApplicationGtk::CUIApplicationGtk(void)
{

}

CUIApplicationGtk::~CUIApplicationGtk(void)
{

}

bool CUIApplicationGtk::InitInstance(int argc, char* argv[])
{
	m_strAppPath = CPaintManagerUI::GetInstancePath();

	// 初始化资源
	InitResource();

	//gtk_init(&argc, &argv);
	gtk_init(0, NULL);

	return true;
}

void CUIApplicationGtk::Run()
{
	if (m_pMainWnd)
	{
		m_pMainWnd->m_pApplication = this;
	}

	// 消息循环
	CPaintManagerUI::MessageLoop();
}

int CUIApplicationGtk::ExitInstance()
{
	if (m_pMainWnd != NULL) { delete m_pMainWnd; m_pMainWnd = NULL; }
	CPaintManagerUI::Term();
	return 0;
}

} //namespace DuiLib{
#endif //#ifdef DUILIB_GTK
