#ifndef __UICONTROL_H__
#define __UICONTROL_H__

#pragma once

namespace DuiLib {

	/////////////////////////////////////////////////////////////////////////////////////
	//

	typedef CControlUI* (CALLBACK* FINDCONTROLPROC)(CControlUI*, LPVOID);

	class UILIB_API CControlUI : public CUIAnimation
	{
		DECLARE_DUICONTROL(CControlUI)
	public:
		CControlUI();
		virtual ~CControlUI();

	public:
		virtual CDuiString GetName() const;
		virtual void SetName(LPCTSTR pstrName);
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface(LPCTSTR pstrName);
		virtual UINT GetControlFlags() const;

		virtual bool Activate();
		virtual CPaintManagerUI* GetManager() const;
		virtual void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);
		virtual CControlUI* GetParent() const;
	    void setInstance(HINSTANCE instance = NULL) {m_instance = instance;};
		
		// 定时器
		bool SetTimer(UINT nTimerID, UINT nElapse);
		void KillTimer(UINT nTimerID);

		// 文本相关
		virtual CDuiString GetText() const;
		virtual void SetText(LPCTSTR pstrText);

		virtual int  GetTextN();			//add by liqs99
		virtual void SetTextN(int n);		//add by liqs99

		virtual void SetTextV(LPCTSTR lpszFormat, ...);

		virtual bool IsResourceText() const;
		virtual void SetResourceText(bool bResource);

		virtual int  GetResourceID() const;			//add by liqs99
		virtual void SetResourceID(int resid);		//add by liqs99
		virtual CLangPackageUI *GetLangPackage();
		virtual void SetSkinFile(LPCTSTR lpstrSkinFile);
		virtual CDuiString GetSkinFile();

		virtual bool IsDragEnabled() const;
		virtual void SetDragEnable(bool bDrag);

		virtual bool IsDropEnabled() const;
		virtual void SetDropEnable(bool bDrop);

		// 图形相关
		LPCTSTR GetGradient();
		virtual void SetGradient(LPCTSTR pStrImage);
		DWORD GetBkColor() const;
		void SetBkColor(DWORD dwBackColor);
		DWORD GetBkColor2() const;
		void SetBkColor2(DWORD dwBackColor);
		DWORD GetBkColor3() const;
		void SetBkColor3(DWORD dwBackColor);
		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetFocusBkColor(DWORD dwColor);
		DWORD GetFocusBkColor() const;
		DWORD GetForeColor() const;
		void SetForeColor(DWORD dwForeColor);
		LPCTSTR GetBkImage();
		virtual void SetBkImage(LPCTSTR pStrImage);
		LPCTSTR GetForeImage() const;
		void SetForeImage(LPCTSTR pStrImage);

		DWORD GetFocusBorderColor() const;
		void SetFocusBorderColor(DWORD dwBorderColor);
		DWORD GetHotBorderColor() const; //add by liqs99
		void SetHotBorderColor(DWORD dwBorderColor); //add by liqs99
		DWORD GetDisableBorderColor() const;
		void SetDisableBorderColor(DWORD dwBorderColor);
		bool IsColorHSL() const;
		void SetColorHSL(bool bColorHSL);
		SIZE GetBorderRound() const;
		void SetBorderRound(SIZE cxyRound);
		bool DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

		//边框相关
		int GetBorderSize() const;
		void SetBorderSize(int nSize);
		DWORD GetBorderColor() const;
		void SetBorderColor(DWORD dwBorderColor);
		void SetBorderSize(RECT rc);
		int GetLeftBorderSize() const;
		void SetLeftBorderSize(int nSize);
		int GetTopBorderSize() const;
		void SetTopBorderSize(int nSize);
		int GetRightBorderSize() const;
		void SetRightBorderSize(int nSize);
		int GetBottomBorderSize() const;
		void SetBottomBorderSize(int nSize);
		int GetBorderStyle() const;
		void SetBorderStyle(int nStyle);

		// 位置相关
		virtual RECT GetRelativePos() const; // 相对(父控件)位置
		virtual RECT GetClientPos() const; // 客户区域（除去scrollbar和inset）
		virtual const RECT& GetPos() const;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);
		virtual bool CalcPos(CControlUI *pChildControl, RECT &rcChild); //子控件调用询问父控件，你将会给我分配多大的rect。
		virtual int GetWidth() const;
		virtual int GetHeight() const;
		virtual int GetX() const;
		virtual int GetY() const;
		virtual RECT GetPadding() const;
		virtual void SetPadding(RECT rcPadding); // 设置外边距，由上层窗口绘制
		virtual SIZE GetFixedXY() const;         // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		virtual void SetFixedXY(SIZE szXY);      // 仅float为true时有效
		virtual int GetFixedWidth() const;       // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		virtual void SetFixedWidth(int cx);      // 预设的参考值
		virtual int GetFixedHeight() const;      // 实际大小位置使用GetPos获取，这里得到的是预设的参考值
		virtual void SetFixedHeight(int cy);     // 预设的参考值
		virtual int GetFixedWidthPercent() const;
		virtual void SetFixedWidthPercent(int cx);
		virtual int GetFixedHeightPercent() const;
		virtual void SetFixedHeightPercent(int cy);
		virtual int GetMinWidth() const;
		virtual void SetMinWidth(int cx);
		virtual int GetMaxWidth() const;
		virtual void SetMaxWidth(int cx);
		virtual int GetMinHeight() const;
		virtual void SetMinHeight(int cy);
		virtual int GetMaxHeight() const;
		virtual void SetMaxHeight(int cy);
		virtual TPercentInfo GetFloatPercent() const;
		virtual void SetFloatPercent(TPercentInfo piFloatPercent);
		virtual void SetFloatAlign(UINT uAlign);
		virtual UINT GetFloatAlign() const;

		virtual void SetFloatPosition(POINT ptPosition);
		virtual POINT GetFloatPosition() const;

		virtual bool IsAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);
		virtual bool IsAutoCalcHeight() const;
		virtual void SetAutoCalcHeight(bool bAutoCalcHeight);

		// 鼠标提示
		virtual CDuiString GetToolTip() const;
		virtual void SetToolTip(LPCTSTR pstrText);
		virtual void SetToolTipWidth(int nWidth);
		virtual int	  GetToolTipWidth(void);	// 多行ToolTip单行最长宽度
		
		// 光标
		virtual WORD GetCursor();
		virtual void SetCursor(WORD wCursor);

		// 快捷键
		virtual TCHAR GetShortcut() const;
		virtual void SetShortcut(TCHAR ch);

		// 菜单
		virtual bool IsContextMenuUsed() const;
		virtual void SetContextMenuUsed(bool bMenuUsed);

		// 用户属性
		virtual CDuiString GetUserData(); // 辅助函数，供用户使用
		virtual void SetUserData(LPCTSTR pstrText); // 辅助函数，供用户使用
		virtual UINT_PTR GetTag() const; // 辅助函数，供用户使用
		virtual void SetTag(UINT_PTR pTag); // 辅助函数，供用户使用

		// 一些重要的属性
		virtual bool IsVisible() const;
		virtual void SetVisible(bool bVisible = true);
		virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool bEnable = true);
		virtual bool IsMouseEnabled() const;
		virtual void SetMouseEnabled(bool bEnable = true);
		virtual bool IsKeyboardEnabled() const;
		virtual void SetKeyboardEnabled(bool bEnable = true);
		virtual bool IsFocused() const;
		virtual void SetFocus();
		virtual bool IsFloat() const;
		virtual void SetFloat(bool bFloat = true);

		virtual bool IsHot() const;
		virtual void SetHot(bool bHot = true);

		virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		void Invalidate();
		bool IsUpdateNeeded() const;
		void NeedUpdate();
		void NeedParentUpdate();
		DWORD GetAdjustColor(DWORD dwColor);

		virtual void Init();
		virtual void DoInit();

		virtual void Event(TEventUI& event);
		virtual void DoEvent(TEventUI& event);

		// 自定义(未处理的)属性
		void AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveCustomAttribute(LPCTSTR pstrName);
		void RemoveAllCustomAttribute();

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CControlUI* ApplyAttributeList(LPCTSTR pstrList);

		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual bool Paint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl = NULL); // 返回要不要继续绘制
		virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		virtual void PaintBkColor(HDC hDC);
		virtual void PaintBkImage(HDC hDC);
		virtual void PaintStatusImage(HDC hDC);
		virtual void PaintForeColor(HDC hDC);
		virtual void PaintForeImage(HDC hDC);
		virtual void PaintText(HDC hDC);
		virtual void PaintBorder(HDC hDC);

		//绘制时不受控件本身大小的限制
		virtual void DoPostPaint(HDC hDC, const RECT& rcPaint);

		//虚拟窗口参数
		void SetVirtualWnd(LPCTSTR pstrValue);
		CDuiString GetVirtualWnd() const;

		//焦点控件是否允许窗口响应默认按键事件
		virtual bool OnEnableResponseDefaultKeyEvent(WPARAM wParam) { return true; }
	public:
		CEventSource OnInit;
		CEventSource OnDestroy;
		CEventSource OnSize;
		CEventSource OnEvent;
		CEventSource OnNotify;
		CEventSource OnPaint;

	protected:
		CPaintManagerUI* m_pManager;
		CControlUI* m_pParent;
		CDuiString m_sVirtualWnd;
		CDuiString m_sName;
		bool m_bUpdateNeeded;
		bool m_bMenuUsed;
		RECT m_rcItem;
		RECT m_rcPadding;
		SIZE m_cXY;
		SIZE m_cxyFixed;
		SIZE m_cxyFixedPercent;
		SIZE m_cxyMin;
		SIZE m_cxyMax;
		bool m_bVisible;
		bool m_bInternVisible;
		bool m_bPaneVisible;
		bool m_bEnabled;
		bool m_bMouseEnabled;
		bool m_bKeyboardEnabled ;
		bool m_bFocused;
		bool m_bFloat;
		UINT m_uButtonState;
		TPercentInfo m_piFloatPercent;
		UINT m_uFloatAlign;
		POINT m_ptFloatPosition;	//绝对定位时, 父控件左上角的百分比位置
		bool m_bSetPos; // 防止SetPos循环调用

		bool	m_bAutoCalcWidth;	//根据子控件或者文本等等自动计算宽度
		bool	m_bAutoCalcHeight;	//根据子控件或者文本等等自动计算高度

		bool m_bDragEnabled;
		bool m_bDropEnabled;

		bool m_bResourceText;
		int  m_nResourceID;
		CDuiString m_sSkinFile;	//我来自哪个文件， add by liqs99

		CDuiString m_sText;
		CDuiString m_sToolTip;
		TCHAR m_chShortcut;
		CDuiString m_sUserData;
		UINT_PTR m_pTag;

		CDuiString m_sGradient;
		DWORD m_dwBackColor;
		DWORD m_dwBackColor2;
		DWORD m_dwBackColor3;
		DWORD m_dwHotBkColor;
		DWORD m_dwFocusBkColor;
		DWORD m_dwForeColor;
		CDuiString m_sBkImage;
		CDuiString m_sForeImage;
		DWORD m_dwBorderColor;
		DWORD m_dwFocusBorderColor;
		DWORD m_dwHotBorderColor;		//鼠标悬浮时边框颜色
		DWORD m_dwDisableBorderColor;	//禁用或只读时边框颜色
		bool m_bColorHSL;
		int m_nBorderSize;
		int m_nBorderStyle;
		int m_nTooltipWidth;
		WORD m_wCursor;
		SIZE m_cxyBorderRound;
		RECT m_rcPaint;
		RECT m_rcBorderSize;
	    HINSTANCE m_instance;

		CStdStringPtrMap m_mCustomAttrHash;

	public:
		virtual void SetPaneVisible(bool bVisible = true);
		virtual bool IsPaneVisible() const;
		virtual void SetAnimation(DuiAnim emAnim);
		virtual DuiAnim GetAnimation() const;
		virtual void SetFrameCount(int framecount);
		virtual int GetFrameCount() const;
		virtual void SetFrameDelay(int nDelay);
		virtual int GetFrameDelay() const;
		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop);
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID);
		virtual void OnAnimationStop(int nAnimationID);

	protected:
		DuiAnim m_animation;	//动画类型
		int m_nFrameCount;		//动画总帧数
		int m_nFrameDelay;		//动画帧延时
		SIZE m_szAnimationTotal;		//总的动画区域
		SIZE m_szAnimationCurrect;		//当前动画区域

	public:
		void *m_asOnInit;		//事件发生时，调用脚本的函数指针
		void *m_asOnEvent;		
		void *m_asOnNotify;	
		void *m_asOnDestroy;	
		void *m_asOnSize;
		void *m_asOnPaint;
		void *m_asOnPaintBkColor;
		void *m_asOnPaintBkImage;
		void *m_asOnPaintStatusImage;
		void *m_asOnPaintForeColor;
		void *m_asOnPaintForeImage;
		void *m_asOnPaintText;
		void *m_asOnPaintBorder;

		void __AddRef();
		void __ReleaseRef();
	private:
		int __refCount;

	public:
		void SetExtraParent(CControlUI *pControl);
		CControlUI *GetExtraParent() const;
	protected:
		CControlUI *m_pExtraParent;
	};

} // namespace DuiLib

#endif // __UICONTROL_H__
