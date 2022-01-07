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

	virtual LPCTSTR GetClass() const override;
	virtual LPVOID GetInterface(LPCTSTR pstrName) override;

	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;

	virtual void LineUp() override;
	virtual void LineDown() override;
	virtual void PageUp() override;
	virtual void PageDown() override;
	virtual void HomeUp() override;
	virtual void EndDown() override;

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
	virtual LPCTSTR GetClass() const override;
	virtual LPVOID	GetInterface(LPCTSTR pstrName) override;
	UINT GetControlFlags() const override;

	virtual void Refresh(bool bNeedUpdate = false) override;
	virtual void SendNotify(LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false) override;
public:
	virtual void DoInit() override;
	virtual void DoEvent(TEventUI& event) override;
	virtual void SetPos(RECT rc, bool bNeedInvalidate = true) override;
	void BuildRows(RECT rc, bool bNeedInvalidate = true);
	virtual void SetScrollPos(SIZE szPos, bool bMsg = true) override;
	virtual void ProcessScrollBar(RECT rc, int cxRequired, int cyRequired) override;

	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;	

protected:
	CTreeBodyUI *m_pBody;
};

}


#endif // UITreeViewEx_h__
