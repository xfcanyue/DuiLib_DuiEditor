


void TestDialog(CPaintManagerUI &pm)
{
	CDialog dlg("BaseDialog.xml");
	dlg.RegisterNotify(DUI_MSGTYPE_WINDOWINIT, "OnInitDialog_TestDialog");
	dlg.RegisterNotify(DUI_MSGTYPE_CLICK, "OnNotifyClick_TestDialog");
	dlg.DoModal(pm.GetPaintWindow());
}

void OnInitDialog_TestDialog(CPaintManagerUI &pm, TNotifyUI& msg)
{
	//设置对话框标题栏
	CLabelUI @dlgTitle = cast<CLabelUI>(pm.FindControl("dialog_title"));
	dlgTitle.SetText("脚本窗口");
}

void OnNotifyClick_TestDialog(CPaintManagerUI &pm, TNotifyUI& msg)
{
	if(msg.pSender.GetName() == "btn_ok")
	{
		MsgBox("点击了确定");
	}
}