#pragma once

struct tagConfigRet 
{
	CDuiString name;
	int value;
};

struct tag_DevPropKey
{
	DEVPROPKEY devPropKey;
	CDuiString sName;
	CDuiString sFirendlyName;
};

class CMainFrame : public CUIFrameWnd
{
public:
	CMainFrame(void);
	virtual ~CMainFrame(void);
	virtual LPCTSTR GetWindowClassName() const  { return _T("MainFrame"); }
	virtual CDuiString GetSkinFile() 			{ return _T("MainFrame.xml"); }
	virtual CDuiString GetSkinFolder()			{ return _T(""); }
	virtual void OnFinalMessage( HWND hWnd ) { __super::OnFinalMessage(hWnd); ::PostQuitMessage(0L); }

	void Add_DevPropKey_List(DEVPROPKEY devPropKey, LPCTSTR sName, LPCTSTR sFirendlyName);

	virtual void InitWindow();
	virtual bool OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool OnMenuCommand(const MenuCmd *cmd);
	virtual bool OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI);
	virtual void OnNotifyClick(TNotifyUI& msg);
	virtual void OnNotifySelectChanged(TNotifyUI& msg);

	CDuiString GetTreeNodeName(DEVINST devInst); //获取插入树中的名字
	void InitTree();
	void InitTree_SetupApi_Class(LPGUID pGuid, TNodeData *pParentNode);

	void SetCellText(int row, int col, CDuiString &sCellText);
	void InitGrid(TNodeData *pNode);

	void my_CM_Get_DevNode_Property(DEVINST devInst, tag_DevPropKey &devKeys);
	void my_CM_Get_Class_Property(LPGUID pClassGuid, tag_DevPropKey &devKeys);

	BOOL CompareDevPropKey(const DEVPROPKEY *key1, const DEVPROPKEY *key2);
	CDuiString GetDevKeyPropValue(int row, const tag_DevPropKey &devKeys, DEVPROPTYPE devPropType, LPBYTE PropertyBuffer, ULONG uPropertyBufferSize);
	CDuiString on_CM_Get_DevNode_Status(int row, ULONG ulStatus);
	CDuiString On_CM_DRP_DEVICE_POWER_DATA(int row, CM_POWER_DATA *pPowerData);
	CDuiString On_CM_DRP_CAPABILITIES(int row, DWORD dwCaps);


	CTreeDeviceUI *m_tree;
	CGridUI *m_grid;

	std::vector<tag_DevPropKey> m_arrDevPropKey;
};

