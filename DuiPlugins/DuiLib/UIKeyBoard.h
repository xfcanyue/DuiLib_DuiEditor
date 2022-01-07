#pragma once

namespace DuiLib
{

class CKeyBoardUI;
class UILIB_API CKeyButtonUI : public CButtonUI
{
public:
	CKeyButtonUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetOwner(CKeyBoardUI *pKeyBoard);
	CKeyBoardUI *GetOwner() const;

	void SetChar(TCHAR ch, TCHAR chShift, BYTE vkCode);
	TCHAR GetChar();

	BYTE GetVkCode();

	bool IsControlKey() const;
	bool IsOemKey() const;
protected:
	bool Activate();
	virtual CDuiString GetText() const;
	virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void PaintText(UIRender *pRender);
	virtual void PaintBkColor(UIRender *pRender);
	virtual void PaintBorder(UIRender *pRender);

private:
	TCHAR m_ch; 
	TCHAR m_chShift;
	BYTE m_vkCode;
	bool m_bControl; //是否控制按钮
	CKeyBoardUI *m_pKeyBoard;
};

class UILIB_API CKeyBoardUI : public CVerticalLayoutUI//CHorizontalLayoutUI
{
	DECLARE_DUICONTROL(CKeyBoardUI)
	friend class CKeyButtonUI;
public:
	CKeyBoardUI(void);
	virtual ~CKeyBoardUI(void);

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	void SetBindFocusControl(CControlUI *pFocusEdit);
	void SetBindEnterControl(CControlUI *pEnterButton); //设置回车键绑定的控件

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetKeyWidth(int nWidth)	{ m_nKeyWidth = nWidth; }
	int  GetKeyWidth()				{ return m_nKeyWidth; }

	void SetKeyHeight(int nHeight)	{ m_nKeyHeight = nHeight; }
	int  GetKeyHeight()				{ return m_nKeyHeight; }

	void SetKeyPadding(int iFont)		{ m_nKeyPadding = iFont; }
	int  GetKeyPadding()				{ return m_nKeyPadding; }

	void SetKeyBorderSize(int nSize)	{ m_nKeyBorderSize = nSize; }
	DWORD GetKeyBorderSize()			{ return m_nKeyBorderSize; }

	void SetKeyBorderRound(SIZE cxyRound) { m_cxKeyBorderRound = cxyRound; }
	SIZE GetKeyBorderRound() const		{ return m_cxKeyBorderRound; }

	void SetKeyTextColor(DWORD nColor)	{ m_dwKeyTextColor = nColor; }
	DWORD GetKeyTextColor()				{ return m_dwKeyTextColor; }

	void SetKeyBkColor(DWORD nColor)	{ m_dwKeyBkColor = nColor; }
	DWORD GetKeyBkColor()				{ return m_dwKeyBkColor; }

	void SetKeyBorderColor(DWORD nColor)		{ m_dwKeyBorderColor = nColor; }
	DWORD GetKeyBorderColor()					{ return m_dwKeyBorderColor; }

	void SetKeyPushedTextColor(DWORD nColor)	{ m_dwKeyPushedTextColor = nColor; }
	DWORD GetKeyPushedTextColor()				{ return m_dwKeyPushedTextColor; }

	void SetKeyPushedBkColor(DWORD nColor)		{ m_dwKeyPushedBkColor = nColor; }
	DWORD GetKeyPushedBkColor()					{ return m_dwKeyPushedBkColor; }

	void SetKeyPushedBorderColor(DWORD nColor)	{ m_dwKeyPushedBorderColor = nColor; }
	DWORD GetKeyPushedBorderColor()				{ return m_dwKeyPushedBorderColor; }

	void SetBindFocusControlName(LPCTSTR sName)	{ m_sBindFocusControlName = sName; }
	LPCTSTR GetBindFocusControlName()			{ return m_sBindFocusControlName; }

	void SetBindEnterControlName(LPCTSTR sName)	{ m_sBindEnterControlName = sName; }
	LPCTSTR GetBindEnterControlName()			{ return m_sBindEnterControlName; }

	void SetEnterControlName(LPCTSTR pstrName)  { m_sEnterControlName = pstrName; } //设置回车键的控件名字
	LPCTSTR GetEnterControlName()				{ return m_sEnterControlName; }		//获取回车键的控件名字

	void SetEnterControlText(LPCTSTR pstrName)  { m_sEnterControlText = pstrName; } //设置回车键的文本
	LPCTSTR GetEnterControlText()				{ return m_sEnterControlText; }		//获取回车键的文本

	void SetEnterControlFont(int iFont)	{ m_iFontEnterControl = iFont; }
	int  GetEnterControlFont()			{ return m_iFontEnterControl; }
protected:
	virtual int CalcKeyBoardWidth();
	virtual int CalcKeyBoardHeight();

	virtual void DoInit();
	virtual void InitKeyBoard(int lines);
	virtual void InitKeyButton();

	CKeyButtonUI* AddKey(int line, TCHAR ch, TCHAR chShift, BYTE vkCode, int width=-1, int height=-1);
	void AddLabel(int line, int width, int height);

	void OnKeyPress(CKeyButtonUI *pKey);
	CControlUI* GetBindFocusControl();
	CControlUI* GetBindEnterControl(); //获得回车键绑定的按钮

public:
	void SetCapsON(bool bOn)	{ m_bCapsOn = bOn;	}
	void SetShiftON(bool bOn)	{ m_bShiftOn = bOn; }
	void SetCtrlON(bool bOn)	{ m_bCtrlOn = bOn;	}
	void SetAltON(bool bOn)		{ m_bAltOn = bOn;	}
	virtual bool IsCapsON()		{ return m_bCapsOn; }
	virtual bool IsShiftON()	{ return m_bShiftOn;}
	virtual bool IsCtrlON()		{ return m_bCtrlOn; }
	virtual bool IsAltON()		{ return m_bAltOn;	}
	virtual CDuiString GetKeyText(BYTE vkCode);
	virtual int GetFont(BYTE vkCode);
private:
	CHorizontalLayoutUI *m_pLine[6];
	int m_nlines;
	int m_nKeyWidth;
	int m_nKeyHeight;
	int m_nKeyPadding;
	int m_iFontEnterControl;
	int m_nKeyBorderSize;
	SIZE m_cxKeyBorderRound;
	DWORD m_dwKeyTextColor;
	DWORD m_dwKeyBkColor;
	DWORD m_dwKeyBorderColor;
	DWORD m_dwKeyPushedTextColor;
	DWORD m_dwKeyPushedBkColor;
	DWORD m_dwKeyPushedBorderColor;

	CDuiString m_sBindFocusControlName;
	CDuiString m_sBindEnterControlName;

	CDuiString m_sEnterControlName;		//回车键控件的名字
	CDuiString m_sEnterControlText;		//回车键的文字

	CControlUI *m_pFocus;		//输入窗口
	CControlUI *m_pBindEnterButton; //接收回车事件的按钮

private:
	bool m_bCapsOn;
	bool m_bShiftOn;
	bool m_bCtrlOn;
	bool m_bAltOn;
};

}