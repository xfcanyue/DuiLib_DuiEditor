#pragma once

struct tagDeviceNodeTag 
{
	tagDeviceNodeTag()
	{
		devInst = NULL;
		hIcon = NULL;
	}

	int type;

	CDuiString classguid;

	DEVINST devInst;
	HICON hIcon;
};

class CTreeDeviceUI : public CTreeUI
{
	DECLARE_DUICONTROL(CTreeDeviceUI)
public:
	CTreeDeviceUI(void);
	virtual ~CTreeDeviceUI(void);

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID	GetInterface(LPCTSTR pstrName) override;

	virtual void OnDeleteNode(TNodeData *pNodeData) override;
	virtual bool OnPaintItemForeImage(UIRender *pRender, CControlUI *pItem);
};

