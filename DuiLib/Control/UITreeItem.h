#ifndef UITreeNodeEx_h__
#define UITreeNodeEx_h__

#pragma once

namespace DuiLib
{
	//Tree内部的的Option控件
	class UILIB_API CTreeInnerCheckBoxUI : public CCheckBoxUI
	{
	public:
		enum eType { BTN_FOLDER, BTN_CHECKBOX, BTN_ICON, BTN_TEXT };
		CTreeInnerCheckBoxUI();

		void SetOwner(CControlUI *pCellUI) { m_pOwner = pCellUI; }
		CControlUI *GetOwner() const { return m_pOwner; }

		CTreeInnerCheckBoxUI::eType GetType() const { return m_type; }

		virtual bool Activate() override;
		virtual bool IsSelected() const override;
		virtual void Selected(bool bSelected, bool bTriggerEvent=true) override;

		virtual bool IsSelectedState() const override;

		virtual CDuiString GetText() const override;

		virtual void DoEvent(TEventUI& event) override;

		//virtual void PaintStatusImage(UIRender *pRender) override;

		virtual void PaintBkColor(UIRender *pRender) override;
		virtual void PaintBkImage(UIRender *pRender) override;
		virtual void PaintStatusImage(UIRender *pRender) override;
		virtual void PaintForeColor(UIRender *pRender) override;
		virtual void PaintForeImage(UIRender *pRender) override;
		virtual void PaintText(UIRender *pRender) override;
		virtual void PaintBorder(UIRender *pRender) override;

		CControlUI *m_pOwner;
		eType m_type;
	};

	class UILIB_API CTreeItemUI : public CHorizontalLayoutUI
	{
		DECLARE_DUICONTROL(CTreeItemUI)
	public:
		CTreeItemUI();
		~CTreeItemUI(void);

		void SetOwner(ITreeUI *pGrid) { m_pOwner = pGrid; }
		ITreeUI *GetOwner() const { return m_pOwner; }

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;
		virtual UINT GetControlFlags() const override;

		void SetNodeData(TNodeData *pNodeData);
		TNodeData *GetNodeData() const { return m_pNodeData; }

		CTreeInnerCheckBoxUI *GetFolderButton() const	{ return m_pFolderButton; }
		CTreeInnerCheckBoxUI *GetCheckBoxButton() const { return m_pCheckBox; }
		CTreeInnerCheckBoxUI *GetIconButton() const		{ return m_pIcon; }
		CTreeInnerCheckBoxUI *GetTextButton() const		{ return m_pText; }


		virtual void DoInit() override;
		int GetCxNeeded(SIZE szAvailable);
		virtual void DoEvent(TEventUI& event) override;
	protected:
		ITreeUI *m_pOwner;
		TNodeData *m_pNodeData;
	
		CTreeInnerCheckBoxUI			*m_pFolderButton;	//展开收缩按钮
		CTreeInnerCheckBoxUI			*m_pCheckBox;		//选中按钮
		CTreeInnerCheckBoxUI			*m_pIcon;			//图标
		CTreeInnerCheckBoxUI			*m_pText;			//文本框
	};
}


#endif // UITreeNodeEx_h__
