#ifndef __UITABLAYOUT_H__
#define __UITABLAYOUT_H__

#pragma once

namespace DuiLib
{
	class UILIB_API CTabLayoutUI : public CContainerUI
	{
		DECLARE_DUICONTROL(CTabLayoutUI)
	public:
		CTabLayoutUI();

		virtual LPCTSTR GetClass() const override;
		virtual LPVOID GetInterface(LPCTSTR pstrName) override;

		virtual bool Add(CControlUI* pControl) override;
		virtual bool AddAt(CControlUI* pControl, int iIndex) override;
		virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy=false) override;
		virtual void RemoveAll() override;
		int GetCurSel() const;
		virtual bool SelectItem(int iIndex);
		virtual bool SelectItem(CControlUI* pControl);

		virtual bool SelectForm(LPCTSTR lpstrFormName);
		CDuiString GetCurForm();
		
		virtual void DoInit() override;
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;

		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop) override;
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID) override;
		virtual void OnAnimationStop(int nAnimationID) override;
	protected:
		int m_iCurSel;
		int m_iOldSel;
	private:
		int m_nMoveDirection;
		CControlUI *m_pCurControl;
		CControlUI *m_pLastControl;
		RECT m_rcCur;
		RECT m_rcLast;
	};
}
#endif // __UITABLAYOUT_H__
