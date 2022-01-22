#include "StdAfx.h"
#include "MainFrame.h"


CMainFrame::CMainFrame(void)
{
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::InitWindow()
{
	IScriptManager *pEngine = GetManager()->GetScriptEngine();
	if(pEngine)
	{
		//调用脚本函数
		//
		IScriptFunction fun;
		fun.SetReturnType(UIArg_void);					//返回值
		fun.SetFuctionName(_T("MainFrame_InitWindow")); //函数名
		fun.PushArgAddress(GetManager());				//参数入栈
		pEngine->ExecuteScript(&fun);
	}
	return;
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}
