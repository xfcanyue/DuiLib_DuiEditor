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

		// 文本内容
		virtual CDuiString GetText() const;
		virtual void SetText(LPCTSTR pstrText);

		//数字类型的文本内容
		virtual int  GetTextN();
		virtual void SetTextN(int n);

		//格式化文本内容
		virtual void SetTextV(LPCTSTR lpszFormat, ...);

		//文本对齐
		void SetTextStyle(UINT uStyle);
		UINT GetTextStyle() const;

		//文本外边距
		virtual RECT GetTextPadding() const;
		virtual void SetTextPadding(RECT rc);
		virtual RECT GetPushedTextPadding() const;
		virtual void SetPushedTextPadding(RECT rc);

		//是否使用资源替换文本
		virtual bool IsResourceText() const;
		virtual void SetResourceText(bool bResource);

		//////////////////////////////////////////////////////////////////////////
		//另一种的多语言方案

		//资源id
		virtual int  GetResourceID() const;	
		virtual void SetResourceID(int resid);

		//多语言管理器
		virtual CLangPackageUI *GetLangPackage();

		//当前控件来自哪个xml文件
		virtual void SetSkinFile(LPCTSTR lpstrSkinFile);
		virtual CDuiString GetSkinFile();

		//////////////////////////////////////////////////////////////////////////
		//拖动

		//允许拖动
		virtual bool IsDragEnabled() const;
		virtual void SetDragEnable(bool bDrag);

		//接收拖动结果
		virtual bool IsDropEnabled() const;
		virtual void SetDropEnable(bool bDrop);

		//接受文件拖拽
		virtual bool IsAcceptDropFile() const;
		virtual void SetAcceptDropFile(bool bDrop);
		//////////////////////////////////////////////////////////////////////////
		//字体
		int GetFont() const;
		virtual void SetFont(int index);
		int GetHotFont() const;
		void SetHotFont(int index);
		int GetPushedFont() const;
		void SetPushedFont(int index);
		int GetFocusedFont() const;
		void SetFocusedFont(int index);
		int GetDisabledFont() const;
		void SetDisabledFont(int index);
		int GetSelectedFont() const;
		void SetSelectedFont(int index);

		//////////////////////////////////////////////////////////////////////////
		//字体颜色
		virtual DWORD GetTextColor() const;
		virtual void SetTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetFocusedTextColor() const;
		void SetFocusedTextColor(DWORD dwColor);
		DWORD GetDisabledTextColor() const;
		void SetDisabledTextColor(DWORD dwColor);
		DWORD GetSelectedTextColor() const;
		void SetSelectedTextColor(DWORD dwColor);

		//////////////////////////////////////////////////////////////////////////
		//背景渐变色

		//背景色渐变方向
		bool GetGradient();
		virtual void SetGradient(bool bVertical);

		//背景色，梯度
		DWORD GetBkColor2() const;
		void SetBkColor2(DWORD dwBackColor);

		//背景色，梯度
		DWORD GetBkColor3() const;
		void SetBkColor3(DWORD dwBackColor);

		//背景色
		DWORD GetBkColor() const;
		void SetBkColor(DWORD dwBackColor);
		DWORD GetHotBkColor() const;
		void SetHotBkColor(DWORD dwColor);
		DWORD GetFocusBkColor() const;
		void SetFocusBkColor(DWORD dwColor);
		DWORD GetPushedBkColor() const;
		void SetPushedBkColor(DWORD dwColor);
		DWORD GetDisabledBkColor() const;
		void SetDisabledBkColor(DWORD dwColor);
		DWORD GetSelectBkColor();
		void SetSelectedBkColor(DWORD dwBkColor);

		//////////////////////////////////////////////////////////////////////////
		//背景图
		CDuiString GetBkImage();
		virtual void SetBkImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//前景色
		DWORD GetForeColor() const;
		void SetForeColor(DWORD dwForeColor);

		//////////////////////////////////////////////////////////////////////////
		//前景图
		CDuiString	GetForeImage() const;
		void		SetForeImage(LPCTSTR pStrImage);
		CDuiString	GetHotForeImage() const;
		void		SetHotForeImage(LPCTSTR pStrImage);
		CDuiString	GetPushedForeImage() const;
		void		SetPushedForeImage(LPCTSTR pStrImage);
		CDuiString	GetFocusedForeImage() const;
		void		SetFocusedForeImage(LPCTSTR pStrImage);
		CDuiString	GetDisabledForeImage() const;
		void		SetDisabledForeImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedForeImage() const;
		void		SetSelectedForedImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//状态相关
		CDuiString	GetNormalImage() const;
		virtual void SetNormalImage(LPCTSTR pStrImage);
		CDuiString	GetHotImage() const;
		void		SetHotImage(LPCTSTR pStrImage);
		CDuiString	GetPushedImage() const;
		void		SetPushedImage(LPCTSTR pStrImage);
		CDuiString	GetFocusedImage() const;
		void		SetFocusedImage(LPCTSTR pStrImage);
		CDuiString	GetDisabledImage() const;
		void		SetDisabledImage(LPCTSTR pStrImage);

		CDuiString	GetSelectedImage() const;
		void		SetSelectedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedHotImage() const;
		void		SetSelectedHotImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedPushedImage() const;
		void		SetSelectedPushedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedFocusedImage() const;
		void		SetSelectedFocusedImage(LPCTSTR pStrImage);
		CDuiString	GetSelectedDisabledImage() const;
		void		SetSelectedDisabledImage(LPCTSTR pStrImage);

		//////////////////////////////////////////////////////////////////////////
		//边框相关

		//边框size
		int GetBorderSize() const;
		void SetBorderSize(int nSize);

		//边框size，按照左上右下设置
		void SetBorderRectSize(RECT rc);
		RECT GetBorderRectSize() const;

		//左边框
		int GetLeftBorderSize() const;
		void SetLeftBorderSize(int nSize);

		//上边框
		int GetTopBorderSize() const;
		void SetTopBorderSize(int nSize);

		//右边框
		int GetRightBorderSize() const;
		void SetRightBorderSize(int nSize);

		//下边框
		int GetBottomBorderSize() const;
		void SetBottomBorderSize(int nSize);

		//边框样式
		int GetBorderStyle() const;
		void SetBorderStyle(int nStyle);

		//边框圆角
		SIZE GetBorderRound() const;
		void SetBorderRound(SIZE cxyRound);

		//边框颜色，普通状态
		DWORD GetBorderColor() const;
		void SetBorderColor(DWORD dwBorderColor);

		//边框颜色，鼠标悬停
		DWORD GetHotBorderColor() const;
		void SetHotBorderColor(DWORD dwBorderColor);

		//边框颜色，鼠标按下
		DWORD GetPushedBorderColor() const;
		void SetPushedBorderColor(DWORD dwBorderColor);

		//边框颜色，焦点状态
		DWORD GetFocusBorderColor() const;
		void SetFocusBorderColor(DWORD dwBorderColor);

		//边框颜色，禁用时
		DWORD GetDisableBorderColor() const;
		void SetDisableBorderColor(DWORD dwBorderColor);

		//边框颜色，选中时
		DWORD GetSelectedBorderColor() const;
		void SetSelectedBorderColor(DWORD dwBorderColor);

		//////////////////////////////////////////////////////////////////////////

		//HSL颜色
		bool IsColorHSL() const;
		void SetColorHSL(bool bColorHSL);

		//绘图
		bool DrawImage(UIRender *pRender, LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);

		// 相对(父控件)位置, 已弃用
		virtual RECT GetRelativePos() const; 

		// 客户区域（除去scrollbar和inset）
		virtual RECT GetClientPos() const; 

		//控件位置大小
		virtual const CDuiRect& GetPos() const;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

		//移动，仅float有效
		virtual void Move(SIZE szOffset, bool bNeedInvalidate = true);

		//子控件调用, 询问父控件，你将会给我分配多大的rect。
		virtual bool CalcPos(CControlUI *pChildControl, RECT &rcChild); 

		//实际宽度和高度
		virtual int GetWidth() const;
		virtual int GetHeight() const;

		//实际位置左上角坐标
		virtual int GetX() const;
		virtual int GetY() const;

		//外边距，由上层窗口绘制
		virtual RECT GetPadding() const;
		virtual void SetPadding(RECT rcPadding); 

		//float控件左上角坐标，仅float为true时有效
		virtual SIZE GetFixedXY() const;
		virtual void SetFixedXY(SIZE szXY);

		//预设的宽度
		virtual int GetFixedWidth() const;
		virtual void SetFixedWidth(int cx);

		//预设的高度
		virtual int GetFixedHeight() const;
		virtual void SetFixedHeight(int cy);

		//预设的百分比宽度
		virtual int GetFixedWidthPercent() const;
		virtual void SetFixedWidthPercent(int cx);

		//预设的百分比高度
		virtual int GetFixedHeightPercent() const;
		virtual void SetFixedHeightPercent(int cy);

		//最小宽度
		virtual int GetMinWidth() const;
		virtual void SetMinWidth(int cx);
		//最大宽度
		virtual int GetMaxWidth() const;
		virtual void SetMaxWidth(int cx);
		//最小高度
		virtual int GetMinHeight() const;
		virtual void SetMinHeight(int cy);
		//最大高度
		virtual int GetMaxHeight() const;
		virtual void SetMaxHeight(int cy);

		//float控件百分比
		virtual TPercentInfo GetFloatPercent() const;
		virtual void SetFloatPercent(TPercentInfo piFloatPercent);

		//float控件对齐方式
		virtual void SetFloatAlign(UINT uAlign);
		virtual UINT GetFloatAlign() const;

		//float控件，相对左上角百分比位置
		virtual void SetFloatPosition(POINT ptPosition);
		virtual POINT GetFloatPosition() const;

		//根据子控件或者文本等等自动计算宽度
		virtual bool IsAutoCalcWidth() const;
		virtual void SetAutoCalcWidth(bool bAutoCalcWidth);

		//根据子控件或者文本等等自动计算高度
		virtual bool IsAutoCalcHeight() const;
		virtual void SetAutoCalcHeight(bool bAutoCalcHeight);

		// 鼠标提示
		virtual CDuiString GetToolTip() const;
		virtual void SetToolTip(LPCTSTR pstrText);
		// 多行ToolTip单行最长宽度
		virtual void SetToolTipWidth(int nWidth);
		virtual int GetToolTipWidth(void);	
		
		// 光标
		virtual WORD GetCursor();
		virtual void SetCursor(WORD wCursor);

		// 快捷键
		virtual TCHAR GetShortcut() const;
		virtual void SetShortcut(TCHAR ch);

		// 响应菜单消息，DUI_MSGTYPE_MENU
		virtual bool IsContextMenuUsed() const;
		virtual void SetContextMenuUsed(bool bMenuUsed);

		//用户自定义数据
		virtual CDuiString GetUserData(); 
		virtual void SetUserData(LPCTSTR pstrText);

		//用户自定义数据
		virtual UINT_PTR GetTag() const;
		virtual void SetTag(UINT_PTR pTag);

		//是否显示
		virtual bool IsVisible() const;
		virtual void SetVisible(bool bVisible = true);

		// 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
		virtual void SetInternVisible(bool bVisible = true); 

		//响应鼠标
		virtual bool IsMouseEnabled() const;
		virtual void SetMouseEnabled(bool bEnable = true);

		//响应键盘
		virtual bool IsKeyboardEnabled() const;
		virtual void SetKeyboardEnabled(bool bEnable = true);

		// 是否float控件
		virtual bool IsFloat() const;
		virtual void SetFloat(bool bFloat = true);

		//启用状态
		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool bEnable = true);

		//焦点状态
		virtual bool IsFocused() const;
		virtual void SetFocusState(bool bFocus);
		virtual void SetFocus();

		//是否鼠标悬停状态
		virtual bool IsHotState() const;
		virtual void SetHotState(bool bHot = true);

		virtual bool IsCaptureState() const;
		virtual void SetCaptureState(bool bCaptured);

		//鼠标按下状态
		virtual void SetPushedState(bool bPushed);
		virtual bool IsPushedState() const;

		//选中状态
		virtual void SetSelectedState(bool bSelected);
		virtual bool IsSelectedState() const;

		virtual void SetReadOnly(bool bSelected);
		virtual bool IsReadOnly() const;

		virtual CControlUI* FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags);

		//重画控件，但不会重新SetPos
		virtual void Invalidate();

		//调用SetPos，并且重画控件。，本控件的位置大小不会改变
		bool IsUpdateNeeded() const;
		void NeedUpdate();

		//父控件调用SetPos，并且重画控件。，本控件的位置大小会被父控件重新计算。
		void NeedParentUpdate();

		//调整HSL颜色
		DWORD GetAdjustColor(DWORD dwColor);

		virtual void Init();
		virtual void DoInit();

		virtual void Event(TEventUI& event);
		virtual void DoEvent(TEventUI& event);

		// 用户自定义的属性，UI内部并不处理
		void AddCustomAttribute(LPCTSTR pstrName, LPCTSTR pstrAttr);
		LPCTSTR GetCustomAttribute(LPCTSTR pstrName) const;
		bool RemoveCustomAttribute(LPCTSTR pstrName);
		void RemoveAllCustomAttribute();

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CControlUI* ApplyAttributeList(LPCTSTR pstrList);

		virtual SIZE EstimateSize(SIZE szAvailable);
		virtual bool Paint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl = NULL); // 返回要不要继续绘制
		virtual bool DoPaint(UIRender *pRender, const RECT& rcPaint, CControlUI* pStopControl);
		virtual void PaintBkColor(UIRender *pRender);
		virtual void PaintBkImage(UIRender *pRender);
		virtual void PaintStatusImage(UIRender *pRender);
		virtual void PaintForeColor(UIRender *pRender);
		virtual void PaintForeImage(UIRender *pRender);
		virtual void PaintText(UIRender *pRender);
		virtual void PaintBorder(UIRender *pRender);

		//绘制时不受控件本身大小的限制
		virtual void DoPostPaint(UIRender *pRender, const RECT& rcPaint);

		//虚拟窗口参数
		void SetVirtualWnd(LPCTSTR pstrValue);
		CDuiString GetVirtualWnd() const;

		//控件获得焦点试，是否允许窗口响应默认按键事件，比如Enter和Esc关闭窗口
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
		CControlUI* m_pParent;			//父控件
		CDuiString m_sVirtualWnd;		//虚拟窗口
		CDuiString m_sName;				//控件名字
		bool m_bUpdateNeeded;			//控件是否需要刷新
		bool m_bMenuUsed;				//是否响应菜单事件，DUI_MSGTYPE_MENU
		CDuiRect m_rcItem;					//控件的位置和大小，实际显示时。
		RECT m_rcPadding;				//外边距
		SIZE m_cXY;						//绝对定位时，控件左上角坐标
		SIZE m_cxyFixed;				//预设的控件大小
		SIZE m_cxyFixedPercent;			//预设的控件大小百分比
		SIZE m_cxyMin;					//宽高最小值
		SIZE m_cxyMax;					//宽高最大值
		bool m_bVisible;				//是否显示
		bool m_bInternVisible;			//内部调用， m_bVisible && m_bInternVisible = 实际是否显示。
		bool m_bPaneVisible;			//是否显示，用于动画
		bool m_bMouseEnabled;			//响应鼠标
		bool m_bKeyboardEnabled;		//响应键盘
		TButtonState m_state;			//控件状态

		bool m_bFloat;					//是否绝对定位
		TPercentInfo m_piFloatPercent;	//绝对定位百分比
		UINT m_uFloatAlign;				//绝对定位，对齐样式
		POINT m_ptFloatPosition;		//绝对定位, 父控件左上角的百分比位置

		bool m_bSetPos;					// 防止SetPos循环调用

		bool m_bAutoCalcWidth;			//根据子控件或者文本等等自动计算宽度
		bool m_bAutoCalcHeight;			//根据子控件或者文本等等自动计算高度

		bool m_bDragEnabled;			//允许拖动
		bool m_bDropEnabled;			//接受其它控件被拖动时放下到当前控件
		bool m_bAcceptDropFile;			//是否接受文件拖拽

		bool m_bResourceText;			//是否使用资源替换文本

		int  m_nResourceID;				//资源id
		CDuiString m_sSkinFile;			//我来自哪个文件， add by liqs99

		CDuiString m_sUserData;			//用户自定义数据
		UINT_PTR m_pTag;				//用户自定义数据

		CDuiString m_sText;				//文本内容
		UINT m_uTextStyle;				//文本对齐
		CDuiRect m_rcTextPadding;		//文本外边距
		CDuiRect m_rcPushedTextPadding;	//文本外边距
		CDuiString m_sToolTip;			//浮动提示框文本
		int m_nTooltipWidth;			//浮动提示框宽度
		TCHAR m_chShortcut;				//快捷键, Alt+字母

		POINT m_ptLastMouse;
		//////////////////////////////////////////////////////////////////////////
		//字体
		int	m_iFont;
		int	m_iHotFont;
		int	m_iPushedFont;
		int	m_iFocusedFont;
		int	m_iDisabledFont;
		int	m_iSelectedFont;

		//////////////////////////////////////////////////////////////////////////
		//文本颜色
		DWORD m_dwTextColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
		DWORD m_dwFocusedTextColor;
		DWORD m_dwDisabledTextColor;
		DWORD m_dwSelectedTextColor;

		//////////////////////////////////////////////////////////////////////////
		bool m_bGradientVertical;		//是否纵向渐变
		DWORD m_dwBackColor2;			//背景色，梯度
		DWORD m_dwBackColor3;			//背景色，梯度

		DWORD m_dwBackColor;			//背景色
		DWORD m_dwHotBkColor;			//背景色，悬停
		DWORD m_dwFocusBkColor;			//背景色，焦点
		DWORD m_dwPushedBkColor;		//背景色，鼠标按下
		DWORD m_dwDisabledBkColor;		//背景色，禁用时
		DWORD m_dwSelectedBkColor;		//背景色，选中时

		//////////////////////////////////////////////////////////////////////////
		DWORD m_dwForeColor;			//前景色

		//////////////////////////////////////////////////////////////////////////
		CDuiString m_sBkImage;			//背景图片

		//////////////////////////////////////////////////////////////////////////
		CDuiString m_sForeImage;		//前景图片
		CDuiString m_sHotForeImage;
		CDuiString m_sPushedForeImage;
		CDuiString m_sFocusedForeImage;
		CDuiString m_sDisabledForeImage;
		CDuiString m_sSelectedForeImage;

		//////////////////////////////////////////////////////////////////////////
		//状态图
		CDuiString m_sNormalImage;
		CDuiString m_sHotImage;
		CDuiString m_sPushedImage;
		CDuiString m_sFocusedImage;
		CDuiString m_sDisabledImage;

		CDuiString  m_sSelectedImage;
		CDuiString	m_sSelectedHotImage;
		CDuiString	m_sSelectedPushedImage;
		CDuiString  m_sSelectedFocusedImage;
		CDuiString  m_sSelectedDisabledImage;

		//////////////////////////////////////////////////////////////////////////
		//边框
		int m_nBorderSize;				//边框大小
		CDuiRect m_rcBorderSize;			//边框大小, 分别设置左上右下边框大小
		SIZE m_cxyBorderRound;			//边框圆角
		int m_nBorderStyle;				//边框样式
		DWORD m_dwBorderColor;			//边框颜色, 普通状态
		DWORD m_dwHotBorderColor;		//边框颜色, 悬停
		DWORD m_dwPushedBorderColor; 	//边框颜色, 鼠标按下
		DWORD m_dwFocusBorderColor;		//边框颜色, 焦点
		DWORD m_dwDisableBorderColor;	//边框颜色, 禁用
		DWORD m_dwSelectedBorderColor;	//边框颜色，选中时

		bool m_bColorHSL;				//HSL颜色

		WORD m_wCursor;					//鼠标样式

		CDuiRect m_rcPaint;					//实际需要绘制的区域
	    HINSTANCE m_instance;

		CStdStringPtrMap m_mCustomAttrHash;	//自定义属性列表

	public:
		virtual void SetPaneVisible(bool bVisible = true);
		virtual bool IsPaneVisible() const;
		virtual void SetAnimation(DuiAnim emAnim);
		virtual DuiAnim GetAnimation() const;
		virtual void SetFrameCount(int framecount);
		virtual int GetFrameCount() const;
		virtual void SetFrameDelay(int nDelay);
		virtual int GetFrameDelay() const;
		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) override;
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) override;
		virtual void OnAnimationStop(int nAnimationID) override;

	protected:
		DuiAnim m_animation;			//动画类型
		int m_nFrameCount;				//动画总帧数
		int m_nFrameDelay;				//动画帧延时
		SIZE m_szAnimationTotal;		//总的动画区域
		SIZE m_szAnimationCurrect;		//当前动画区域

	public:
		CDuiString m_asOnInit;		//事件发生时，调用脚本的函数指针
		CDuiString m_asOnEvent;		
		CDuiString m_asOnNotify;	
		CDuiString m_asOnDestroy;	
		CDuiString m_asOnSize;
		CDuiString m_asOnPaint;
		CDuiString m_asOnPaintBkColor;
		CDuiString m_asOnPaintBkImage;
		CDuiString m_asOnPaintStatusImage;
		CDuiString m_asOnPaintForeColor;
		CDuiString m_asOnPaintForeImage;
		CDuiString m_asOnPaintText;
		CDuiString m_asOnPaintBorder;

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
