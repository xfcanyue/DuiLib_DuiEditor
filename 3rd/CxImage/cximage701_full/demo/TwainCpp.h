#ifndef __TWAINCPP_
#define __TWAINCPP_

#include "Twain.h"


#define TWCPP_ANYCOUNT		(-1)
#define TWCPP_CANCELTHIS	(1)
#define TWCPP_CANCELALL		(2)
#define TWCPP_DOTRANSFER	(0)

class CTwain
{
public:


	CTwain(HWND hWnd = NULL);
	virtual ~CTwain();
	BOOL InitTwain(HWND hWnd);
	void ReleaseTwain();


	/*  
	  This routine must be implemented by the dervied class 
	  After setting the required values in the m_AppId structure,
	  the derived class should call the parent class implementation
	  Refer Pg: 51 of the Twain Specification version 1.8
	*/
	virtual void GetIdentity();
	virtual BOOL SelectSource();
	virtual BOOL OpenSource(TW_IDENTITY *pSource=NULL);
	virtual int  ShouldTransfer(TW_IMAGEINFO& /*info*/) { return TWCPP_DOTRANSFER;};
	
	BOOL ProcessMessage(MSG msg);

	BOOL SelectDefaultSource();
	BOOL IsValidDriver() const;
	BOOL SourceSelected() const {return m_bSourceSelected;} ;
	BOOL DSMOpen() const;
	BOOL DSOpen() const;
	BOOL SourceEnabled() const { return m_bSourceEnabled;};
	BOOL ModalUI() const { return m_bModalUI; };

	TW_INT16 GetRC() const { return m_returnCode; };
	TW_STATUS GetStatus() const { return m_Status; };

	BOOL SetImageCount(TW_INT16 nCount = 1);
	BOOL Acquire(int numImages=1);

protected:
	BOOL CallTwainProc(pTW_IDENTITY pOrigin,pTW_IDENTITY pDest,
					   TW_UINT32 DG,TW_UINT16 DAT,TW_UINT16 MSG,
					   TW_MEMREF pData);
	TW_UINT16 CallDSMEntry(pTW_IDENTITY pApp, pTW_IDENTITY pSrc,
					   TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG,
					   TW_MEMREF pData);

	void CloseDSM();
	void CloseDS();

	BOOL GetCapability(TW_CAPABILITY& twCap,TW_UINT16 cap,TW_UINT16 conType=TWON_DONTCARE16);
	BOOL GetCapability(TW_UINT16 cap,TW_UINT32& value);
	BOOL SetCapability(TW_UINT16 cap,TW_UINT16 value,BOOL sign=FALSE);
	BOOL SetCapability(TW_CAPABILITY& twCap);
	BOOL EnableSource(BOOL showUI = TRUE);

	BOOL GetImageInfo(TW_IMAGEINFO& info);

	virtual BOOL DisableSource();
	virtual BOOL CanClose()  { return TRUE; };

	void TranslateMessage(TW_EVENT& twEvent);
	void TransferImage();
	BOOL EndTransfer();
	void CancelTransfer();
	BOOL ShouldContinue();
	BOOL GetImage(TW_IMAGEINFO& info);

	virtual void SetImage(HANDLE hBitmap,TW_IMAGEINFO& info)=0;
//	virtual void CopyImage(HANDLE hBitmap,TW_IMAGEINFO& info);

protected:
	HINSTANCE m_hTwainDLL;
	DSMENTRYPROC m_pDSMProc;

	TW_IDENTITY m_AppId;
	TW_IDENTITY m_Source;
	TW_STATUS m_Status;
	TW_INT16  m_returnCode;
	HWND m_hMessageWnd;

	BOOL m_bSourceSelected;
	BOOL m_bDSMOpen;
	BOOL m_bDSOpen;
	BOOL m_bSourceEnabled;
	BOOL m_bModalUI;

	int m_nImageCount;
};


#endif