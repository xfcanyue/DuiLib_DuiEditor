#pragma once
#include "UITracker.h"

class CUIManager;
class CUITrackerMuliti : public CUITracker
{
public:
	class CTrackerElement
	{
	public:
		CTrackerElement(void)
		{
			m_pControl = NULL;
		}
		CTrackerElement(LPRECT rc)
		{	
			m_rect = rc;
		}
		virtual ~CTrackerElement(void)
		{
		}

		const RECT & GetPos()
		{
			return m_rect;
		}
		CRect m_rect;
		xml_node m_node;
		CControlUI *m_pControl;
	};

public:
	CUITrackerMuliti(void);
	virtual ~CUITrackerMuliti(void);

	int m_nHitTest;

public:
	CUIManager *GetUIManager() const { return m_pUIManager; }
	void SetUIManager(CUIManager *pManager) { m_pUIManager = pManager; }
private:
	CUIManager *m_pUIManager;

public:
	void Draw(CDC* pDC, CSize *szOffset);

	int HitTest(CPoint point);
	int HitTestHandles(CPoint point);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE, CDC* pDCClipTo = NULL);
	BOOL SetCursor(CWnd *pWnd, UINT nHitTest, const CPoint &ptDPtoLP, const CSize &szOffset);

	BOOL Add(xml_node node, CRect rect);
	BOOL Add(CTrackerElement * pTracker);
	BOOL Remove(xml_node node);
	void RemoveAll();
	BOOL IsSelected(xml_node node);
	BOOL IsSelected(CControlUI *pControl);

	BOOL IsEmpty() const { return m_arrTracker.IsEmpty(); }
	int  GetSize() const { return m_arrTracker.GetSize(); }

	virtual void OnChangedRect();
	virtual void OnChangedRect(const CRect& rectOld);

	virtual void UpdateRect(xml_node node);
	virtual void SetFocus(xml_node node);

	void OnkeyUp();
	void OnkeyDown();
	void OnkeyLeft();
	void OnkeyRight();

	void RefreshRect();
	BOOL CanbeMoveRect();
public:
	CArray<CTrackerElement*,CTrackerElement*> m_arrTracker; //Tracking controls array
	CTrackerElement* m_pFocused; // Control has focus
protected:
	void CopyTrackerRect();
	void ClearCopy();
private:
	
	CArray<RECT,const RECT&> m_arrCloneRect;//Copies of the tracking rectangle

	HCURSOR m_hNoDropCursor;
};

