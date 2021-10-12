#pragma once

namespace DuiLib
{

class UILIB_API CPictureUI : public CContainerUI
{
	DECLARE_DUICONTROL(CPictureUI)
public:
	CPictureUI(void);
	virtual ~CPictureUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();

	virtual bool Activate();
	virtual void DoEvent(TEventUI& event);

	virtual void PaintBkImage(HDC hDC);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetFrameDelay(int nDelay);
	int  GetFrameDelay();

	void	SetAutoPlay(bool bIsAuto = true );
	bool	IsAutoPlay() const;
	void	SetAutoSize(bool bIsAuto = true );
	bool	IsAutoSize() const;

	void EnableTrackRect(bool bSelectRect) { m_bTrackRect = bSelectRect; }
	bool IsEnableTrackRect() const { return m_bTrackRect; }
	void SetTrackColor(DWORD dwColor) { m_dwTrackColor = dwColor; }
	DWORD GetTrackColor() const { return m_dwTrackColor; }
	CDuiRect GetTrackRect();

	virtual HBITMAP GetHBitmap();
	virtual bool LoadHBitmap(HBITMAP hBitmap);
	virtual bool LoadImageFromMemory(LPBYTE pData, DWORD dwSize);
	virtual bool LoadFile(LPCTSTR pstrImage);
	virtual void RemoveAllImages();

	void StartAnim();
	void StopAnim();
	void OnTimer(UINT_PTR idEvent);

protected:
	bool __SetHBitmap(HBITMAP hBitmap);
	bool __LoadImageFromMemory(LPBYTE pData, DWORD dwSize);
	bool __LoadFile(LPCTSTR pstrImage);

private:
	bool m_bTrackRect;
	CDuiRect m_rcTracker;
	DWORD m_dwTrackColor;
private:
	bool			m_bIsAutoPlay;			// 是否自动播放
	bool			m_bIsAutoSize;			// 是否自动根据图片设置大小
	int				m_nDelay;				// 循环毫秒数

private:
	UINT_PTR		m_idEventTimer;		//动画定时器
	int				m_nFramePosition;	//当前放到第几帧

	CStdPtrArray		m_frames;
	CStdPtrArray		m_arrDrawInfo;
	std::map<TImageInfo *, TDrawInfo *> m_map;	//TImageInfo 和 TDrawInfo的对应表
};

}