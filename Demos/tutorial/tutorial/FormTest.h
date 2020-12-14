#pragma once
class CFormTest : public CUIForm
{
public:
	CFormTest(void);
	virtual ~CFormTest(void);

	virtual void InitWindow();
	virtual void OnNotifyClick(TNotifyUI& msg);
};

