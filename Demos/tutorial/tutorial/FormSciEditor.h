#pragma once
class CFormSciEditor : public CUIForm
{
public:
	CFormSciEditor(void);
	virtual ~CFormSciEditor(void);

	virtual void InitWindow();
	virtual void OnNotifySciNotify(TNotifyUI &msg);

	CSciEditUI *m_pSci;
};

