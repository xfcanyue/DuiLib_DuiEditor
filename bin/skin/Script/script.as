
bool OnClockControlInit(CControlUI &obj)
{
	obj.SetTimer(1,1000);
	return true;
}

bool OnClockControlEvent(CControlUI &obj, TEventUI &ev)
{
	if(ev.Type == UIEVENT_TIMER) //在定时器事件中，更新显示文本。
	{
		datetime dt;
		dt.GetCurrentTime();
		string s = "时间:" + dt.Format("%Y-%m-%d %H:%M:%S");
		obj.SetText(s);
	}
	return false;
}