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

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		bool Add(CControlUI* pControl);
		bool AddAt(CControlUI* pControl, int iIndex);
		bool Remove(CControlUI* pControl, bool bDoNotDestroy=false);
		void RemoveAll();
		int GetCurSel() const;
		virtual bool SelectItem(int iIndex);
		virtual bool SelectItem(CControlUI* pControl);

		virtual bool SelectForm(LPCTSTR lpstrFormName);
		CDuiString GetCurForm();
		
		virtual void DoInit();
		void SetPos(RECT rc, bool bNeedInvalidate = true);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual void OnAnimationStart(int nAnimationID, BOOL bFirstLoop);
		virtual void OnAnimationStep(int nTotalFrame, int nCurFrame, int nAnimationID);
		virtual void OnAnimationStop(int nAnimationID);
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
