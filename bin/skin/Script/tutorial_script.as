
bool m_bCheckmenu_cmd_2 = true;
bool m_bCheckmenu_cmd_22 = true;
bool m_bCheckmenu_cmd_41 = true;
bool m_bCheckmenu_cmd_42 = true;
int m_nMenuSelected = 0;

bool IsMenuCommand(MenuCmd &cmd, string sName)
{
	return cmd.GetName() == sName;
}
	
void MainFrame_InitWindow(CPaintManagerUI @pManager)
{
	string strText;
	strText += "\r\n我";
	strText += "\r\n我是";
	strText += "\r\n我是字";
	strText += "\r\n我是字幕";
	strText += "\r\n我是字幕，";
	strText += "\r\n我是字幕，我";
	strText += "\r\n我是字幕，我是";
	strText += "\r\n我是字幕，我是字";
	strText += "\r\n我是字幕，我是字幕";
	CControlUI @pRoll1 = pManager.FindControl("rolltext_1");
	CControlUI @pRoll2 = pManager.FindControl("rolltext_2");
	CControlUI @pRoll3 = pManager.FindControl("rolltext_3");
	CControlUI @pRoll4 = pManager.FindControl("rolltext_4");
	
	string s1 = pRoll1.GetText() + strText;
	pRoll1.SetText(s1);
	
	string s2 = pRoll2.GetText() + strText;
	pRoll2.SetText(s2);
	
	string s3 = pRoll3.GetText() + strText;
	pRoll3.SetText(s3);
	
	string s4 = pRoll4.GetText() + strText;
	pRoll4.SetText(s4);
	
	CControlUI @pClockLabel = pManager.FindControl("label_clock");
	pClockLabel.SetTimer(1, 1000);
}

bool MainFrame_OnMenuCommand(CPaintManagerUI &mgr, MenuCmd &cmd)
{
	if(IsMenuCommand(cmd, "menu_cmd_2"))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_2 = !m_bCheckmenu_cmd_2;
	}
	if(IsMenuCommand(cmd, "menu_cmd_22"))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_22 = !m_bCheckmenu_cmd_22;
	}
	if(IsMenuCommand(cmd, "menu_cmd_41"))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_41 = !m_bCheckmenu_cmd_41;
	}
	if(IsMenuCommand(cmd, "menu_cmd_42"))
	{
		//更新本地菜单选中状态
		m_bCheckmenu_cmd_42 = !m_bCheckmenu_cmd_42;
	}
	if(IsMenuCommand(cmd, "menu_cmd_5"))
	{
		//更新本地菜单选中状态
		m_nMenuSelected = 0;
	}
	if(IsMenuCommand(cmd, "menu_cmd_6"))
	{
		//更新本地菜单选中状态
		m_nMenuSelected = 1;
	}
	return false;
}

bool MainFrame_OnMenuUpdateCommandUI(CPaintManagerUI &mgr, CMenuCmdUI &cmdUI)
{
	if(cmdUI.GetName() == "menu_cmd_1")
	{
		cmdUI.Enable(FALSE);
		string sText = cmdUI.GetText();
		sText += "禁用";
		cmdUI.SetText(sText);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_2")
	{
		cmdUI.SetCheck(m_bCheckmenu_cmd_2);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_22")
	{
		cmdUI.SetCheck(m_bCheckmenu_cmd_22);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_41")
	{
		cmdUI.SetCheck(m_bCheckmenu_cmd_41);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_42")
	{
		cmdUI.SetCheck(m_bCheckmenu_cmd_42);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_5")
	{
		cmdUI.SetCheck(m_nMenuSelected == 0);
		return true;
	}
	if(cmdUI.GetName() == "menu_cmd_6")
	{
		cmdUI.SetCheck(m_nMenuSelected == 1);
		return true;
	}
	return false;
}

void MainFrame_OnNotifyClick(CPaintManagerUI &mgr, TNotifyUI& msg)
{
	if(msg.pSender.GetName() == "windowmenubtn")
	{
		string xml = "Menu.xml";
		CreateMenu(mgr, xml);
		return;
	}
}

void MainFrame_OnNotifyTimer(CPaintManagerUI &mgr, TNotifyUI& msg)
{
	if(msg.pSender.GetName() == "label_clock")
	{
		datetime dt;
		dt.GetCurrentTime();
		string s = dt.Format("%Y-%m-%d %H:%M:%S");
		msg.pSender.SetText(s);
	}
}



