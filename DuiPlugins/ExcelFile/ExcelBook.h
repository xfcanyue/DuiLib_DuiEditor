#pragma once


class CExcelBook : public COleDispatchDriver
{
public:
	CExcelBook(){} // 调用 COleDispatchDriver 默认构造函数
	CExcelBook(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExcelBook(const CExcelBook& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// _Workbook 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_AcceptLabelsInFormulas()
	{
		BOOL result;
		InvokeHelper(0x5a1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AcceptLabelsInFormulas(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5a1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Activate()
	{
		InvokeHelper(0x130, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_ActiveChart()
	{
		LPDISPATCH result;
		InvokeHelper(0xb7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ActiveSheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x133, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Author()
	{
		CString result;
		InvokeHelper(0x23e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Author(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x23e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_AutoUpdateFrequency()
	{
		long result;
		InvokeHelper(0x5a2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_AutoUpdateFrequency(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5a2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoUpdateSaveChanges()
	{
		BOOL result;
		InvokeHelper(0x5a3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoUpdateSaveChanges(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5a3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ChangeHistoryDuration()
	{
		long result;
		InvokeHelper(0x5a4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ChangeHistoryDuration(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5a4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_BuiltinDocumentProperties()
	{
		LPDISPATCH result;
		InvokeHelper(0x498, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ChangeFileAccess(long Mode, VARIANT& WritePassword, VARIANT& Notify)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x3dd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Mode, &WritePassword, &Notify);
	}
	void ChangeLink(LPCTSTR Name, LPCTSTR NewName, long Type)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 ;
		InvokeHelper(0x322, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, NewName, Type);
	}
	LPDISPATCH get_Charts()
	{
		LPDISPATCH result;
		InvokeHelper(0x79, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Close(VARIANT& SaveChanges, VARIANT& Filename, VARIANT& RouteWorkbook)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x115, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SaveChanges, &Filename, &RouteWorkbook);
	}
	CString get_CodeName()
	{
		CString result;
		InvokeHelper(0x55d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get__CodeName()
	{
		CString result;
		InvokeHelper(0x80010000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put__CodeName(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x80010000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_Colors(VARIANT& Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x11e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &Index);
		return result;
	}
	void put_Colors(VARIANT& Index, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x11e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Index, &newValue);
	}
	LPDISPATCH get_CommandBars()
	{
		LPDISPATCH result;
		InvokeHelper(0x59f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Comments()
	{
		CString result;
		InvokeHelper(0x23f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Comments(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x23f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ConflictResolution()
	{
		long result;
		InvokeHelper(0x497, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ConflictResolution(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x497, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Container()
	{
		LPDISPATCH result;
		InvokeHelper(0x4a6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_CreateBackup()
	{
		BOOL result;
		InvokeHelper(0x11f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CustomDocumentProperties()
	{
		LPDISPATCH result;
		InvokeHelper(0x499, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_Date1904()
	{
		BOOL result;
		InvokeHelper(0x193, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Date1904(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x193, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void DeleteNumberFormat(LPCTSTR NumberFormat)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NumberFormat);
	}
	LPDISPATCH get_DialogSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x2fc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_DisplayDrawingObjects()
	{
		long result;
		InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_DisplayDrawingObjects(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x194, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL ExclusiveAccess()
	{
		BOOL result;
		InvokeHelper(0x490, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	long get_FileFormat()
	{
		long result;
		InvokeHelper(0x120, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void ForwardMailer()
	{
		InvokeHelper(0x3cd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_FullName()
	{
		CString result;
		InvokeHelper(0x121, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_HasMailer()
	{
		BOOL result;
		InvokeHelper(0x3d0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasMailer(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3d0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_HasPassword()
	{
		BOOL result;
		InvokeHelper(0x122, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_HasRoutingSlip()
	{
		BOOL result;
		InvokeHelper(0x3b6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HasRoutingSlip(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3b6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_IsAddin()
	{
		BOOL result;
		InvokeHelper(0x5a5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_IsAddin(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5a5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Keywords()
	{
		CString result;
		InvokeHelper(0x241, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Keywords(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x241, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT LinkInfo(LPCTSTR Name, long LinkInfo, VARIANT& Type, VARIANT& EditionRef)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x327, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Name, LinkInfo, &Type, &EditionRef);
		return result;
	}
	VARIANT LinkSources(VARIANT& Type)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x328, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Type);
		return result;
	}
	LPDISPATCH get_Mailer()
	{
		LPDISPATCH result;
		InvokeHelper(0x3d3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void MergeWorkbook(VARIANT& Filename)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x5a6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename);
	}
	LPDISPATCH get_Modules()
	{
		LPDISPATCH result;
		InvokeHelper(0x246, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_MultiUserEditing()
	{
		BOOL result;
		InvokeHelper(0x491, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Names()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ba, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH NewWindow()
	{
		LPDISPATCH result;
		InvokeHelper(0x118, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_OnSave()
	{
		CString result;
		InvokeHelper(0x49a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnSave(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x49a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnSheetActivate()
	{
		CString result;
		InvokeHelper(0x407, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnSheetActivate(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x407, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_OnSheetDeactivate()
	{
		CString result;
		InvokeHelper(0x439, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_OnSheetDeactivate(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x439, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void OpenLinks(LPCTSTR Name, VARIANT& ReadOnly, VARIANT& Type)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x323, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, &ReadOnly, &Type);
	}
	CString get_Path()
	{
		CString result;
		InvokeHelper(0x123, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_PersonalViewListSettings()
	{
		BOOL result;
		InvokeHelper(0x5a7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PersonalViewListSettings(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5a7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_PersonalViewPrintSettings()
	{
		BOOL result;
		InvokeHelper(0x5a8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PersonalViewPrintSettings(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5a8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH PivotCaches()
	{
		LPDISPATCH result;
		InvokeHelper(0x5a9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Post(VARIANT& DestName)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x48e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &DestName);
	}
	BOOL get_PrecisionAsDisplayed()
	{
		BOOL result;
		InvokeHelper(0x195, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_PrecisionAsDisplayed(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x195, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void _PrintOut(VARIANT& From, VARIANT& To, VARIANT& Copies, VARIANT& Preview, VARIANT& ActivePrinter, VARIANT& PrintToFile, VARIANT& Collate)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x389, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate);
	}
	void PrintPreview(VARIANT& EnableChanges)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x119, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &EnableChanges);
	}
	void _Protect(VARIANT& Password, VARIANT& Structure, VARIANT& Windows)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x11a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Password, &Structure, &Windows);
	}
	void ProtectSharing(VARIANT& Filename, VARIANT& Password, VARIANT& WriteResPassword, VARIANT& ReadOnlyRecommended, VARIANT& CreateBackup, VARIANT& SharingPassword)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x5aa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename, &Password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, &SharingPassword);
	}
	BOOL get_ProtectStructure()
	{
		BOOL result;
		InvokeHelper(0x24c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_ProtectWindows()
	{
		BOOL result;
		InvokeHelper(0x127, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_ReadOnly()
	{
		BOOL result;
		InvokeHelper(0x128, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get__ReadOnlyRecommended()
	{
		BOOL result;
		InvokeHelper(0x129, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void RefreshAll()
	{
		InvokeHelper(0x5ac, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Reply()
	{
		InvokeHelper(0x3d1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ReplyAll()
	{
		InvokeHelper(0x3d2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RemoveUser(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5ad, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	long get_RevisionNumber()
	{
		long result;
		InvokeHelper(0x494, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Route()
	{
		InvokeHelper(0x3b2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_Routed()
	{
		BOOL result;
		InvokeHelper(0x3b7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_RoutingSlip()
	{
		LPDISPATCH result;
		InvokeHelper(0x3b5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void RunAutoMacros(long Which)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x27a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Which);
	}
	void Save()
	{
		InvokeHelper(0x11b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void _SaveAs(VARIANT& Filename, VARIANT& FileFormat, VARIANT& Password, VARIANT& WriteResPassword, VARIANT& ReadOnlyRecommended, VARIANT& CreateBackup, long AccessMode, VARIANT& ConflictResolution, VARIANT& AddToMru, VARIANT& TextCodepage, VARIANT& TextVisualLayout)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x11c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename, &FileFormat, &Password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, AccessMode, &ConflictResolution, &AddToMru, &TextCodepage, &TextVisualLayout);
	}
	void SaveCopyAs(VARIANT& Filename)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xaf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename);
	}
	BOOL get_Saved()
	{
		BOOL result;
		InvokeHelper(0x12a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Saved(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_SaveLinkValues()
	{
		BOOL result;
		InvokeHelper(0x196, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_SaveLinkValues(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x196, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SendMail(VARIANT& Recipients, VARIANT& Subject, VARIANT& ReturnReceipt)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x3b3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Recipients, &Subject, &ReturnReceipt);
	}
	void SendMailer(VARIANT& FileFormat, long Priority)
	{
		static BYTE parms[] = VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x3d4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &FileFormat, Priority);
	}
	void SetLinkOnData(LPCTSTR Name, VARIANT& Procedure)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT ;
		InvokeHelper(0x329, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, &Procedure);
	}
	LPDISPATCH get_Sheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x1e5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShowConflictHistory()
	{
		BOOL result;
		InvokeHelper(0x493, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowConflictHistory(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x493, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Styles()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ed, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Subject()
	{
		CString result;
		InvokeHelper(0x3b9, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Subject(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_Title()
	{
		CString result;
		InvokeHelper(0xc7, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Title(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xc7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Unprotect(VARIANT& Password)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x11d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Password);
	}
	void UnprotectSharing(VARIANT& SharingPassword)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x5af, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SharingPassword);
	}
	void UpdateFromFile()
	{
		InvokeHelper(0x3e3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void UpdateLink(VARIANT& Name, VARIANT& Type)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x324, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Name, &Type);
	}
	BOOL get_UpdateRemoteReferences()
	{
		BOOL result;
		InvokeHelper(0x19b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UpdateRemoteReferences(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x19b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_UserControl()
	{
		BOOL result;
		InvokeHelper(0x4ba, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_UserControl(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x4ba, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_UserStatus()
	{
		VARIANT result;
		InvokeHelper(0x495, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CustomViews()
	{
		LPDISPATCH result;
		InvokeHelper(0x5b0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Windows()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ae, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Worksheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x1ee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_WriteReserved()
	{
		BOOL result;
		InvokeHelper(0x12b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	CString get_WriteReservedBy()
	{
		CString result;
		InvokeHelper(0x12c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Excel4IntlMacroSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x245, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Excel4MacroSheets()
	{
		LPDISPATCH result;
		InvokeHelper(0x243, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_TemplateRemoveExtData()
	{
		BOOL result;
		InvokeHelper(0x5b1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_TemplateRemoveExtData(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5b1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void HighlightChangesOptions(VARIANT& When, VARIANT& Who, VARIANT& Where)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x5b2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &When, &Who, &Where);
	}
	BOOL get_HighlightChangesOnScreen()
	{
		BOOL result;
		InvokeHelper(0x5b5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_HighlightChangesOnScreen(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5b5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_KeepChangeHistory()
	{
		BOOL result;
		InvokeHelper(0x5b6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_KeepChangeHistory(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5b6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ListChangesOnNewSheet()
	{
		BOOL result;
		InvokeHelper(0x5b7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ListChangesOnNewSheet(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x5b7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void PurgeChangeHistoryNow(long Days, VARIANT& SharingPassword)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x5b8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Days, &SharingPassword);
	}
	void AcceptAllChanges(VARIANT& When, VARIANT& Who, VARIANT& Where)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x5ba, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &When, &Who, &Where);
	}
	void RejectAllChanges(VARIANT& When, VARIANT& Who, VARIANT& Where)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x5bb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &When, &Who, &Where);
	}
	void PivotTableWizard(VARIANT& SourceType, VARIANT& SourceData, VARIANT& TableDestination, VARIANT& TableName, VARIANT& RowGrand, VARIANT& ColumnGrand, VARIANT& SaveData, VARIANT& HasAutoFormat, VARIANT& AutoPage, VARIANT& Reserved, VARIANT& BackgroundQuery, VARIANT& OptimizeCache, VARIANT& PageFieldOrder, VARIANT& PageFieldWrapCount, VARIANT& ReadData, VARIANT& Connection)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x2ac, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SourceType, &SourceData, &TableDestination, &TableName, &RowGrand, &ColumnGrand, &SaveData, &HasAutoFormat, &AutoPage, &Reserved, &BackgroundQuery, &OptimizeCache, &PageFieldOrder, &PageFieldWrapCount, &ReadData, &Connection);
	}
	void ResetColors()
	{
		InvokeHelper(0x5bc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_VBProject()
	{
		LPDISPATCH result;
		InvokeHelper(0x5bd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void FollowHyperlink(LPCTSTR Address, VARIANT& SubAddress, VARIANT& NewWindow, VARIANT& AddHistory, VARIANT& ExtraInfo, VARIANT& Method, VARIANT& HeaderInfo)
	{
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x5be, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Address, &SubAddress, &NewWindow, &AddHistory, &ExtraInfo, &Method, &HeaderInfo);
	}
	void AddToFavorites()
	{
		InvokeHelper(0x5c4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_IsInplace()
	{
		BOOL result;
		InvokeHelper(0x6e9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void PrintOut(VARIANT& From, VARIANT& To, VARIANT& Copies, VARIANT& Preview, VARIANT& ActivePrinter, VARIANT& PrintToFile, VARIANT& Collate, VARIANT& PrToFileName)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6ec, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
	}
	void WebPagePreview()
	{
		InvokeHelper(0x71a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_PublishObjects()
	{
		LPDISPATCH result;
		InvokeHelper(0x71b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_WebOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x71c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ReloadAs(long Encoding)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x71d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Encoding);
	}
	LPDISPATCH get_HTMLProject()
	{
		LPDISPATCH result;
		InvokeHelper(0x71f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_EnvelopeVisible()
	{
		BOOL result;
		InvokeHelper(0x720, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnvelopeVisible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x720, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_CalculationVersion()
	{
		long result;
		InvokeHelper(0x70e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Dummy17(long calcid)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7fc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, calcid);
	}
	void sblt(LPCTSTR s)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x722, DISPATCH_METHOD, VT_EMPTY, NULL, parms, s);
	}
	BOOL get_VBASigned()
	{
		BOOL result;
		InvokeHelper(0x724, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShowPivotTableFieldList()
	{
		BOOL result;
		InvokeHelper(0x7fe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowPivotTableFieldList(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7fe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_UpdateLinks()
	{
		long result;
		InvokeHelper(0x360, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_UpdateLinks(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x360, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void BreakLink(LPCTSTR Name, long Type)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x7ff, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Name, Type);
	}
	void Dummy16()
	{
		InvokeHelper(0x800, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SaveAs(VARIANT& Filename, VARIANT& FileFormat, VARIANT& Password, VARIANT& WriteResPassword, VARIANT& ReadOnlyRecommended, VARIANT& CreateBackup, long AccessMode, VARIANT& ConflictResolution, VARIANT& AddToMru, VARIANT& TextCodepage, VARIANT& TextVisualLayout, VARIANT& Local)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x785, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Filename, &FileFormat, &Password, &WriteResPassword, &ReadOnlyRecommended, &CreateBackup, AccessMode, &ConflictResolution, &AddToMru, &TextCodepage, &TextVisualLayout, &Local);
	}
	BOOL get_EnableAutoRecover()
	{
		BOOL result;
		InvokeHelper(0x801, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_EnableAutoRecover(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x801, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_RemovePersonalInformation()
	{
		BOOL result;
		InvokeHelper(0x802, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_RemovePersonalInformation(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x802, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_FullNameURLEncoded()
	{
		CString result;
		InvokeHelper(0x787, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void CheckIn(VARIANT& SaveChanges, VARIANT& Comments, VARIANT& MakePublic)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x803, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SaveChanges, &Comments, &MakePublic);
	}
	BOOL CanCheckIn()
	{
		BOOL result;
		InvokeHelper(0x805, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SendForReview(VARIANT& Recipients, VARIANT& Subject, VARIANT& ShowMessage, VARIANT& IncludeAttachment)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x806, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Recipients, &Subject, &ShowMessage, &IncludeAttachment);
	}
	void ReplyWithChanges(VARIANT& ShowMessage)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x809, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &ShowMessage);
	}
	void EndReview()
	{
		InvokeHelper(0x80a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_Password()
	{
		CString result;
		InvokeHelper(0x1ad, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Password(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1ad, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_WritePassword()
	{
		CString result;
		InvokeHelper(0x468, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_WritePassword(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x468, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_PasswordEncryptionProvider()
	{
		CString result;
		InvokeHelper(0x80b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString get_PasswordEncryptionAlgorithm()
	{
		CString result;
		InvokeHelper(0x80c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_PasswordEncryptionKeyLength()
	{
		long result;
		InvokeHelper(0x80d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetPasswordEncryptionOptions(VARIANT& PasswordEncryptionProvider, VARIANT& PasswordEncryptionAlgorithm, VARIANT& PasswordEncryptionKeyLength, VARIANT& PasswordEncryptionFileProperties)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x80e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &PasswordEncryptionProvider, &PasswordEncryptionAlgorithm, &PasswordEncryptionKeyLength, &PasswordEncryptionFileProperties);
	}
	BOOL get_PasswordEncryptionFileProperties()
	{
		BOOL result;
		InvokeHelper(0x80f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_ReadOnlyRecommended()
	{
		BOOL result;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ReadOnlyRecommended(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x7d5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void Protect(VARIANT& Password, VARIANT& Structure, VARIANT& Windows)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7ed, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Password, &Structure, &Windows);
	}
	LPDISPATCH get_SmartTagOptions()
	{
		LPDISPATCH result;
		InvokeHelper(0x810, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void RecheckSmartTags()
	{
		InvokeHelper(0x811, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Permission()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SharedWorkspace()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Sync()
	{
		LPDISPATCH result;
		InvokeHelper(0x8da, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SendFaxOverInternet(VARIANT& Recipients, VARIANT& Subject, VARIANT& ShowMessage)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x8db, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Recipients, &Subject, &ShowMessage);
	}
	LPDISPATCH get_XmlNamespaces()
	{
		LPDISPATCH result;
		InvokeHelper(0x8dc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_XmlMaps()
	{
		LPDISPATCH result;
		InvokeHelper(0x8dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long XmlImport(LPCTSTR Url, LPDISPATCH * ImportMap, VARIANT& Overwrite, VARIANT& Destination)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PDISPATCH VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x8de, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Url, ImportMap, &Overwrite, &Destination);
		return result;
	}
	LPDISPATCH get_SmartDocument()
	{
		LPDISPATCH result;
		InvokeHelper(0x8e1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DocumentLibraryVersions()
	{
		LPDISPATCH result;
		InvokeHelper(0x8e2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	BOOL get_InactiveListBorderVisible()
	{
		BOOL result;
		InvokeHelper(0x8e3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_InactiveListBorderVisible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8e3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_DisplayInkComments()
	{
		BOOL result;
		InvokeHelper(0x8e4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_DisplayInkComments(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x8e4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long XmlImportXml(LPCTSTR Data, LPDISPATCH * ImportMap, VARIANT& Overwrite, VARIANT& Destination)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_PDISPATCH VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x8e5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Data, ImportMap, &Overwrite, &Destination);
		return result;
	}
	void SaveAsXMLData(LPCTSTR Filename, LPDISPATCH Map)
	{
		static BYTE parms[] = VTS_BSTR VTS_DISPATCH ;
		InvokeHelper(0x8e6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Filename, Map);
	}
	void ToggleFormsDesign()
	{
		InvokeHelper(0x8e7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// _Workbook 属性
public:

};
