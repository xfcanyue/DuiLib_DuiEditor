


bool OnTestButtonInit(CControlUI &obj)
{
	obj.SetText(12345);
	//obj.SetText("control text");
	//print("print string");
	obj.SetName("line");
	//MsgBox(obj.GetName());
	obj.SetGradient("hor");
	//MsgBox(obj.GetGradient());
	
	//CControlUI @ctrl = obj.GetParent();
	//if(ctrl != null)
	//	ctrl.SetBkColor(0xFFFF0000);
	
	//obj.SetManager(obj.GetManager(), obj, true);
	
	//string s = obj.GetText_to_int();
	//MsgBox(s);
	obj.SetTimer(1,1000);
	//obj.SetText(UIEVENT_TIMER);
	DWORD dw = 0xFFFF0000;
	CheckAlphaColor(dw);
	return true;
}
bool OnTestButtonEvent(CControlUI &obj, TEventUI &ev)
{
	if(ev.Type == UIEVENT_TIMER)
	{
		//obj.SetText("timer");
	datetime dt;
	dt.GetCurrentTime();

	CPaintManagerUI @pManager = obj.GetManager();
	if(pManager == null)
	{
		MsgBox("pManager==null");
	}
	CControlUI @ctrl = pManager.FindControl("line");
	if(ctrl != null)
		ctrl.SetText(dt.Format("%Y-%m-%d %H:%M:%S"));
	}
	return false;
}
bool OnTestButtonNotify(CControlUI &obj, TNotifyUI &notify)
{
	//obj.SetText(notify.sType);
	return false;
}
bool OnTestButtonDestroy(CControlUI &obj)
{
	return false;
}
bool OnTestButtonSize(CControlUI &obj)
{
	return false;
}
bool On_Test_Paint(CControlUI &obj, HDC &hDC, const RECT& rcPaint, CControlUI@ pStopControl)
{
	CPaintManagerUI @pManager = obj.GetManager();
	DrawText(hDC, pManager, obj.GetPos(), "On_Test_Paint", 0xFFFF0000, -1, 0);
	return true;
}
