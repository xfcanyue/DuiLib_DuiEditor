#pragma once

namespace DuiLib {

#define TMD_ADD		1
#define TMD_MODIFY	2
#define TMD_DELETE	3

class UILIB_API CUIDialog : public CUIFrameWnd
{
public:
	CUIDialog(void);
	virtual ~CUIDialog(void);
	int m_nMode;

	virtual void OnFinalMessage( HWND hWnd );
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	//设置响应回车键和ESC的默认处理方式
	virtual void SetDefaultKeyEvent(BOOL bEnterCloseOK=TRUE, BOOL bEscCloseCancel=TRUE);
	virtual BOOL IsEnterCloseOK() const;
	virtual BOOL IsEscCloseCancel() const;

	//显示模态对话窗口
	virtual UINT DoModal(CUIFrmBase *pParentWnd = NULL);

	//显示非模态对话窗口, 注意非模态窗口必须是new出来的。
	virtual void ShowDialog(CUIFrmBase *pParentWnd = NULL);

	virtual void Notify(TNotifyUI& msg);

	virtual void InitWindow();

	virtual void OnClickOK();
	virtual void OnClickCancel();

	virtual void on_tmd_add_initwindow();
	virtual bool on_tmd_add();

	virtual void on_tmd_modify_initwindow();
	virtual bool on_tmd_modify();

	virtual void on_tmd_delete_initwindow();
	virtual bool on_tmd_delete();

private:
	bool _bModal;
	BOOL m_bEnterCloseOK;
	BOOL m_bEscCloseCancel;
};

} //namespace DuiLib {