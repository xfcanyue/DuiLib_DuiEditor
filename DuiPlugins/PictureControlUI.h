#pragma once

class UILIB_PLUGIN_API CPictureControlUI : public CContainerUI
{
	DECLARE_DUICONTROL(CPictureControlUI)
public:
	CPictureControlUI(void);
	virtual ~CPictureControlUI(void);

	LPCTSTR	GetClass() const;
	LPVOID	GetInterface(LPCTSTR pstrName);

	virtual void DoInit();
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	virtual void DoEvent(TEventUI& event);
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetFrameDelay(int nDelay);
	int  GetFrameDelay();

	void	SetAutoPlay(bool bIsAuto = true );
	bool	IsAutoPlay() const;
	void	SetAutoSize(bool bIsAuto = true );
	bool	IsAutoSize() const;

	void LoadFile(LPCTSTR fileName); //从硬盘文件绝对路径载入图片,不同于xml的fileimage属性

	void StartAnim();
	void StopAnim();
	void OnTimer(UINT_PTR idEvent);

protected:
	void InitImage();
protected:
	struct Imp;
	Imp* m_pImp;

private:
	bool			m_bIsAutoPlay;				// 是否自动播放gif
	bool			m_bIsAutoSize;				// 是否自动根据图片设置大小
	int				m_nDelay;					//循环毫秒数

private:
	int m_imagetype;	//图片类型
	UINT_PTR			m_idEventTimer;		//动画定时器
	UINT				m_nFrameCount;		//gif图片总帧数
	UINT				m_nFramePosition;	//当前放到第几帧
};

