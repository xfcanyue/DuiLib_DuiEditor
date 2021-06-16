#ifndef UITreeViewEx_h__
#define UITreeViewEx_h__
#pragma once

namespace DuiLib
{

class UILIB_API CTreeBodyUI : public CVerticalLayoutUI
{
	DECLARE_DUICONTROL( CTreeBodyUI)
public:
	CTreeBodyUI(void);
	virtual ~CTreeBodyUI(void);

	void SetOwner(ITreeUI *pTree) { m_pOwner = pTree; }
	ITreeUI *GetOwner() const { return m_pOwner; }

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

	virtual void LineUp();
	virtual void LineDown();
	virtual void PageUp();
	virtual void PageDown();
	virtual void HomeUp();
	virtual void EndDown();

	void SetCxNeeded(int cxNeeded) { m_nCxNeeded = cxNeeded; }
	int GetCxNeeded() const { return m_nCxNeeded; }

	void SetCyNeeded(int cyNeeded) { m_nCyNeeded = cyNeeded; }
	int GetCyNeeded() const { return m_nCyNeeded; }
protected:
	ITreeUI *m_pOwner;
	int m_nCxNeeded;
	int m_nCyNeeded;
};

class UILIB_API CTreeUI : public CContainerUI, public ITreeUI
{
	DECLARE_DUICONTROL(CTreeUI)
public:
	CTreeUI(void);
	~CTreeUI(void);

public:
	virtual LPCTSTR GetClass() const;
	virtual LPVOID	GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	virtual void Refresh(bool bNeedUpdate = false);
	virtual void SendNotify(LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);
public:
	virtual void DoInit();
	virtual void DoEvent(TEventUI& event);
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true);
	virtual void BuildRows(RECT rc, bool bNeedInvalidate = true);
	virtual void SetScrollPos(SIZE szPos, bool bMsg = true);
	virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired);

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);	

protected:
	CTreeBodyUI *m_pBody;
};

}


#endif // UITreeViewEx_h__
