#pragma once
// CRange 包装类

class CExcelRange : public COleDispatchDriver
{
public:
	CExcelRange(){} // 调用 COleDispatchDriver 默认构造函数
	CExcelRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CExcelRange(const CExcelRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// Range 方法
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
	VARIANT Activate()
	{
		VARIANT result;
		InvokeHelper(0x130, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_AddIndent()
	{
		VARIANT result;
		InvokeHelper(0x427, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_AddIndent(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x427, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	CString get_Address(VARIANT& RowAbsolute, VARIANT& ColumnAbsolute, long ReferenceStyle, VARIANT& External, VARIANT& RelativeTo)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xec, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
		return result;
	}
	CString get_AddressLocal(VARIANT& RowAbsolute, VARIANT& ColumnAbsolute, long ReferenceStyle, VARIANT& External, VARIANT& RelativeTo)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1b5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, &RowAbsolute, &ColumnAbsolute, ReferenceStyle, &External, &RelativeTo);
		return result;
	}
	VARIANT AdvancedFilter(long Action, VARIANT& CriteriaRange, VARIANT& CopyToRange, VARIANT& Unique)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x36c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Action, &CriteriaRange, &CopyToRange, &Unique);
		return result;
	}
	VARIANT ApplyNames(VARIANT& Names, VARIANT& IgnoreRelativeAbsolute, VARIANT& UseRowColumnNames, VARIANT& OmitColumn, VARIANT& OmitRow, long Order, VARIANT& AppendLast)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x1b9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Names, &IgnoreRelativeAbsolute, &UseRowColumnNames, &OmitColumn, &OmitRow, Order, &AppendLast);
		return result;
	}
	VARIANT ApplyOutlineStyles()
	{
		VARIANT result;
		InvokeHelper(0x1c0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Areas()
	{
		LPDISPATCH result;
		InvokeHelper(0x238, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString AutoComplete(LPCTSTR String)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4a1, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, String);
		return result;
	}
	VARIANT AutoFill(LPDISPATCH Destination, long Type)
	{
		VARIANT result;
		static BYTE parms[] = VTS_DISPATCH VTS_I4 ;
		InvokeHelper(0x1c1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Destination, Type);
		return result;
	}
	VARIANT AutoFilter(VARIANT& Field, VARIANT& Criteria1, long Operator, VARIANT& Criteria2, VARIANT& VisibleDropDown)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x319, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Field, &Criteria1, Operator, &Criteria2, &VisibleDropDown);
		return result;
	}
	VARIANT AutoFit()
	{
		VARIANT result;
		InvokeHelper(0xed, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT AutoFormat(long Format, VARIANT& Number, VARIANT& Font, VARIANT& Alignment, VARIANT& Border, VARIANT& Pattern, VARIANT& Width)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x72, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Format, &Number, &Font, &Alignment, &Border, &Pattern, &Width);
		return result;
	}
	VARIANT AutoOutline()
	{
		VARIANT result;
		InvokeHelper(0x40c, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT BorderAround(VARIANT& LineStyle, long Weight, long ColorIndex, VARIANT& Color)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x42b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &LineStyle, Weight, ColorIndex, &Color);
		return result;
	}
	LPDISPATCH get_Borders()
	{
		LPDISPATCH result;
		InvokeHelper(0x1b3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Calculate()
	{
		VARIANT result;
		InvokeHelper(0x117, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Cells()
	{
		LPDISPATCH result;
		InvokeHelper(0xee, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Characters(VARIANT& Start, VARIANT& Length)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x25b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Start, &Length);
		return result;
	}
	VARIANT CheckSpelling(VARIANT& CustomDictionary, VARIANT& IgnoreUppercase, VARIANT& AlwaysSuggest, VARIANT& SpellLang)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1f9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &CustomDictionary, &IgnoreUppercase, &AlwaysSuggest, &SpellLang);
		return result;
	}
	VARIANT Clear()
	{
		VARIANT result;
		InvokeHelper(0x6f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearContents()
	{
		VARIANT result;
		InvokeHelper(0x71, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearFormats()
	{
		VARIANT result;
		InvokeHelper(0x70, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearNotes()
	{
		VARIANT result;
		InvokeHelper(0xef, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ClearOutline()
	{
		VARIANT result;
		InvokeHelper(0x40d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_Column()
	{
		long result;
		InvokeHelper(0xf0, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH ColumnDifferences(VARIANT& Comparison)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1fe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Comparison);
		return result;
	}
	LPDISPATCH get_Columns()
	{
		LPDISPATCH result;
		InvokeHelper(0xf1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_ColumnWidth()
	{
		VARIANT result;
		InvokeHelper(0xf2, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ColumnWidth(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xf2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT Consolidate(VARIANT& Sources, VARIANT& Function, VARIANT& TopRow, VARIANT& LeftColumn, VARIANT& CreateLinks)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1e2, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Sources, &Function, &TopRow, &LeftColumn, &CreateLinks);
		return result;
	}
	VARIANT Copy(VARIANT& Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x227, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination);
		return result;
	}
	long CopyFromRecordset(LPUNKNOWN Data, VARIANT& MaxRows, VARIANT& MaxColumns)
	{
		long result;
		static BYTE parms[] = VTS_UNKNOWN VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x480, DISPATCH_METHOD, VT_I4, (void*)&result, parms, Data, &MaxRows, &MaxColumns);
		return result;
	}
	VARIANT CopyPicture(long Appearance, long Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xd5, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Appearance, Format);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT CreateNames(VARIANT& Top, VARIANT& Left, VARIANT& Bottom, VARIANT& Right)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1c9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Top, &Left, &Bottom, &Right);
		return result;
	}
	VARIANT CreatePublisher(VARIANT& Edition, long Appearance, VARIANT& ContainsPICT, VARIANT& ContainsBIFF, VARIANT& ContainsRTF, VARIANT& ContainsVALU)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1ca, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Edition, Appearance, &ContainsPICT, &ContainsBIFF, &ContainsRTF, &ContainsVALU);
		return result;
	}
	LPDISPATCH get_CurrentArray()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CurrentRegion()
	{
		LPDISPATCH result;
		InvokeHelper(0xf3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Cut(VARIANT& Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x235, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination);
		return result;
	}
	VARIANT DataSeries(VARIANT& Rowcol, long Type, long Date, VARIANT& Step, VARIANT& Stop, VARIANT& Trend)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1d0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Rowcol, Type, Date, &Step, &Stop, &Trend);
		return result;
	}
	VARIANT get__Default(VARIANT& RowIndex, VARIANT& ColumnIndex)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RowIndex, &ColumnIndex);
		return result;
	}
	void put__Default(VARIANT& RowIndex, VARIANT& ColumnIndex, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RowIndex, &ColumnIndex, &newValue);
	}
	VARIANT Delete(VARIANT& Shift)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x75, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Shift);
		return result;
	}
	LPDISPATCH get_Dependents()
	{
		LPDISPATCH result;
		InvokeHelper(0x21f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT ExcelDialogBox()
	{
		VARIANT result;
		InvokeHelper(0xf5, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DirectDependents()
	{
		LPDISPATCH result;
		InvokeHelper(0x221, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_DirectPrecedents()
	{
		LPDISPATCH result;
		InvokeHelper(0x222, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT EditionOptions(long Type, long Option, VARIANT& Name, VARIANT& Reference, long Appearance, long ChartSize, VARIANT& Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x46b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Type, Option, &Name, &Reference, Appearance, ChartSize, &Format);
		return result;
	}
	LPDISPATCH get_End(long Direction)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1f4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, Direction);
		return result;
	}
	LPDISPATCH get_EntireColumn()
	{
		LPDISPATCH result;
		InvokeHelper(0xf6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_EntireRow()
	{
		LPDISPATCH result;
		InvokeHelper(0xf7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT FillDown()
	{
		VARIANT result;
		InvokeHelper(0xf8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillLeft()
	{
		VARIANT result;
		InvokeHelper(0xf9, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillRight()
	{
		VARIANT result;
		InvokeHelper(0xfa, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT FillUp()
	{
		VARIANT result;
		InvokeHelper(0xfb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Find(VARIANT& What, VARIANT& After, VARIANT& LookIn, VARIANT& LookAt, VARIANT& SearchOrder, long SearchDirection, VARIANT& MatchCase, VARIANT& MatchByte, VARIANT& SearchFormat)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x18e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &What, &After, &LookIn, &LookAt, &SearchOrder, SearchDirection, &MatchCase, &MatchByte, &SearchFormat);
		return result;
	}
	LPDISPATCH FindNext(VARIANT& After)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x18f, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &After);
		return result;
	}
	LPDISPATCH FindPrevious(VARIANT& After)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x190, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &After);
		return result;
	}
	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(0x92, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Formula()
	{
		VARIANT result;
		InvokeHelper(0x105, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Formula(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaArray()
	{
		VARIANT result;
		InvokeHelper(0x24a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaArray(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x24a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_FormulaLabel()
	{
		long result;
		InvokeHelper(0x564, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLabel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x564, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT get_FormulaHidden()
	{
		VARIANT result;
		InvokeHelper(0x106, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaHidden(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x106, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaLocal()
	{
		VARIANT result;
		InvokeHelper(0x107, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaLocal(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x107, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaR1C1()
	{
		VARIANT result;
		InvokeHelper(0x108, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x108, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_FormulaR1C1Local()
	{
		VARIANT result;
		InvokeHelper(0x109, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_FormulaR1C1Local(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x109, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT FunctionWizard()
	{
		VARIANT result;
		InvokeHelper(0x23b, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL GoalSeek(VARIANT& Goal, LPDISPATCH ChangingCell)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT VTS_DISPATCH ;
		InvokeHelper(0x1d8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &Goal, ChangingCell);
		return result;
	}
	VARIANT Group(VARIANT& Start, VARIANT& End, VARIANT& By, VARIANT& Periods)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Start, &End, &By, &Periods);
		return result;
	}
	VARIANT get_HasArray()
	{
		VARIANT result;
		InvokeHelper(0x10a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_HasFormula()
	{
		VARIANT result;
		InvokeHelper(0x10b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Height()
	{
		VARIANT result;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Hidden()
	{
		VARIANT result;
		InvokeHelper(0x10c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Hidden(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_HorizontalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x88, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_HorizontalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x88, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_IndentLevel()
	{
		VARIANT result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_IndentLevel(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void InsertIndent(long InsertAmount)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x565, DISPATCH_METHOD, VT_EMPTY, NULL, parms, InsertAmount);
	}
	VARIANT Insert(VARIANT& Shift, VARIANT& CopyOrigin)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xfc, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Shift, &CopyOrigin);
		return result;
	}
	LPDISPATCH get_Interior()
	{
		LPDISPATCH result;
		InvokeHelper(0x81, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Item(VARIANT& RowIndex, VARIANT& ColumnIndex)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RowIndex, &ColumnIndex);
		return result;
	}
	void put_Item(VARIANT& RowIndex, VARIANT& ColumnIndex, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xaa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RowIndex, &ColumnIndex, &newValue);
	}
	VARIANT Justify()
	{
		VARIANT result;
		InvokeHelper(0x1ef, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Left()
	{
		VARIANT result;
		InvokeHelper(0x7f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_ListHeaderRows()
	{
		long result;
		InvokeHelper(0x4a3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT ListNames()
	{
		VARIANT result;
		InvokeHelper(0xfd, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	long get_LocationInTable()
	{
		long result;
		InvokeHelper(0x2b3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Locked()
	{
		VARIANT result;
		InvokeHelper(0x10d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Locked(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	void Merge(VARIANT& Across)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x234, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Across);
	}
	void UnMerge()
	{
		InvokeHelper(0x568, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_MergeArea()
	{
		LPDISPATCH result;
		InvokeHelper(0x569, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_MergeCells()
	{
		VARIANT result;
		InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_MergeCells(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Name()
	{
		VARIANT result;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Name(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT NavigateArrow(VARIANT& TowardPrecedent, VARIANT& ArrowNumber, VARIANT& LinkNumber)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x408, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &TowardPrecedent, &ArrowNumber, &LinkNumber);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Next()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString NoteText(VARIANT& Text, VARIANT& Start, VARIANT& Length)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x467, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &Text, &Start, &Length);
		return result;
	}
	VARIANT get_NumberFormat()
	{
		VARIANT result;
		InvokeHelper(0xc1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormat(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xc1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_NumberFormatLocal()
	{
		VARIANT result;
		InvokeHelper(0x449, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_NumberFormatLocal(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x449, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Offset(VARIANT& RowOffset, VARIANT& ColumnOffset)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xfe, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &RowOffset, &ColumnOffset);
		return result;
	}
	VARIANT get_Orientation()
	{
		VARIANT result;
		InvokeHelper(0x86, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Orientation(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x86, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_OutlineLevel()
	{
		VARIANT result;
		InvokeHelper(0x10f, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_OutlineLevel(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x10f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	long get_PageBreak()
	{
		long result;
		InvokeHelper(0xff, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_PageBreak(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xff, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT Parse(VARIANT& ParseLine, VARIANT& Destination)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1dd, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &ParseLine, &Destination);
		return result;
	}
	VARIANT _PasteSpecial(long Paste, long Operation, VARIANT& SkipBlanks, VARIANT& Transpose)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x403, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Paste, Operation, &SkipBlanks, &Transpose);
		return result;
	}
	LPDISPATCH get_PivotField()
	{
		LPDISPATCH result;
		InvokeHelper(0x2db, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotItem()
	{
		LPDISPATCH result;
		InvokeHelper(0x2e4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_PivotTable()
	{
		LPDISPATCH result;
		InvokeHelper(0x2cc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Precedents()
	{
		LPDISPATCH result;
		InvokeHelper(0x220, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_PrefixCharacter()
	{
		VARIANT result;
		InvokeHelper(0x1f8, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Previous()
	{
		LPDISPATCH result;
		InvokeHelper(0x1f7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT _PrintOut(VARIANT& From, VARIANT& To, VARIANT& Copies, VARIANT& Preview, VARIANT& ActivePrinter, VARIANT& PrintToFile, VARIANT& Collate)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x389, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate);
		return result;
	}
	VARIANT PrintPreview(VARIANT& EnableChanges)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x119, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &EnableChanges);
		return result;
	}
	LPDISPATCH get_QueryTable()
	{
		LPDISPATCH result;
		InvokeHelper(0x56a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Range(VARIANT& Cell1, VARIANT& Cell2)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xc5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Cell1, &Cell2);
		return result;
	}
	VARIANT RemoveSubtotal()
	{
		VARIANT result;
		InvokeHelper(0x373, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	BOOL Replace(VARIANT& What, VARIANT& Replacement, VARIANT& LookAt, VARIANT& SearchOrder, VARIANT& MatchCase, VARIANT& MatchByte, VARIANT& SearchFormat, VARIANT& ReplaceFormat)
	{
		BOOL result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xe2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, &What, &Replacement, &LookAt, &SearchOrder, &MatchCase, &MatchByte, &SearchFormat, &ReplaceFormat);
		return result;
	}
	LPDISPATCH get_Resize(VARIANT& RowSize, VARIANT& ColumnSize)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x100, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &RowSize, &ColumnSize);
		return result;
	}
	long get_Row()
	{
		long result;
		InvokeHelper(0x101, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH RowDifferences(VARIANT& Comparison)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x1ff, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Comparison);
		return result;
	}
	VARIANT get_RowHeight()
	{
		VARIANT result;
		InvokeHelper(0x110, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_RowHeight(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x110, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Rows()
	{
		LPDISPATCH result;
		InvokeHelper(0x102, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT Run(VARIANT& Arg1, VARIANT& Arg2, VARIANT& Arg3, VARIANT& Arg4, VARIANT& Arg5, VARIANT& Arg6, VARIANT& Arg7, VARIANT& Arg8, VARIANT& Arg9, VARIANT& Arg10, VARIANT& Arg11, VARIANT& Arg12, VARIANT& Arg13, VARIANT& Arg14, VARIANT& Arg15, VARIANT& Arg16, VARIANT& Arg17, VARIANT& Arg18, VARIANT& Arg19, VARIANT& Arg20, VARIANT& Arg21, VARIANT& Arg22, VARIANT& Arg23, VARIANT& Arg24, VARIANT& Arg25, VARIANT& Arg26, VARIANT& Arg27, VARIANT& Arg28, VARIANT& Arg29, VARIANT& Arg30)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x103, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Arg1, &Arg2, &Arg3, &Arg4, &Arg5, &Arg6, &Arg7, &Arg8, &Arg9, &Arg10, &Arg11, &Arg12, &Arg13, &Arg14, &Arg15, &Arg16, &Arg17, &Arg18, &Arg19, &Arg20, &Arg21, &Arg22, &Arg23, &Arg24, &Arg25, &Arg26, &Arg27, &Arg28, &Arg29, &Arg30);
		return result;
	}
	VARIANT Select()
	{
		VARIANT result;
		InvokeHelper(0xeb, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Show()
	{
		VARIANT result;
		InvokeHelper(0x1f0, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ShowDependents(VARIANT& Remove)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x36d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Remove);
		return result;
	}
	VARIANT get_ShowDetail()
	{
		VARIANT result;
		InvokeHelper(0x249, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ShowDetail(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x249, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT ShowErrors()
	{
		VARIANT result;
		InvokeHelper(0x36e, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT ShowPrecedents(VARIANT& Remove)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x36f, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Remove);
		return result;
	}
	VARIANT get_ShrinkToFit()
	{
		VARIANT result;
		InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_ShrinkToFit(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0xd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT Sort(VARIANT& Key1, long Order1, VARIANT& Key2, VARIANT& Type, long Order2, VARIANT& Key3, long Order3, long Header, VARIANT& OrderCustom, VARIANT& MatchCase, long Orientation, long SortMethod, long DataOption1, long DataOption2, long DataOption3)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x370, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Key1, Order1, &Key2, &Type, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, SortMethod, DataOption1, DataOption2, DataOption3);
		return result;
	}
	VARIANT SortSpecial(long SortMethod, VARIANT& Key1, long Order1, VARIANT& Type, VARIANT& Key2, long Order2, VARIANT& Key3, long Order3, long Header, VARIANT& OrderCustom, VARIANT& MatchCase, long Orientation, long DataOption1, long DataOption2, long DataOption3)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x371, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, SortMethod, &Key1, Order1, &Type, &Key2, Order2, &Key3, Order3, Header, &OrderCustom, &MatchCase, Orientation, DataOption1, DataOption2, DataOption3);
		return result;
	}
	LPDISPATCH get_SoundNote()
	{
		LPDISPATCH result;
		InvokeHelper(0x394, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH SpecialCells(long Type, VARIANT& Value)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x19a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, Type, &Value);
		return result;
	}
	VARIANT get_Style()
	{
		VARIANT result;
		InvokeHelper(0x104, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Style(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT SubscribeTo(LPCTSTR Edition, long Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1e1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Edition, Format);
		return result;
	}
	VARIANT Subtotal(long GroupBy, long Function, VARIANT& TotalList, VARIANT& Replace, VARIANT& PageBreaks, long SummaryBelowData)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_I4 ;
		InvokeHelper(0x372, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, GroupBy, Function, &TotalList, &Replace, &PageBreaks, SummaryBelowData);
		return result;
	}
	VARIANT get_Summary()
	{
		VARIANT result;
		InvokeHelper(0x111, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Table(VARIANT& RowInput, VARIANT& ColumnInput)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x1f1, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &RowInput, &ColumnInput);
		return result;
	}
	VARIANT get_Text()
	{
		VARIANT result;
		InvokeHelper(0x8a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT TextToColumns(VARIANT& Destination, long DataType, long TextQualifier, VARIANT& ConsecutiveDelimiter, VARIANT& Tab, VARIANT& Semicolon, VARIANT& Comma, VARIANT& Space, VARIANT& Other, VARIANT& OtherChar, VARIANT& FieldInfo, VARIANT& DecimalSeparator, VARIANT& ThousandsSeparator, VARIANT& TrailingMinusNumbers)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x410, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &Destination, DataType, TextQualifier, &ConsecutiveDelimiter, &Tab, &Semicolon, &Comma, &Space, &Other, &OtherChar, &FieldInfo, &DecimalSeparator, &ThousandsSeparator, &TrailingMinusNumbers);
		return result;
	}
	VARIANT get_Top()
	{
		VARIANT result;
		InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT Ungroup()
	{
		VARIANT result;
		InvokeHelper(0xf4, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	VARIANT get_UseStandardHeight()
	{
		VARIANT result;
		InvokeHelper(0x112, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_UseStandardHeight(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x112, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_UseStandardWidth()
	{
		VARIANT result;
		InvokeHelper(0x113, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_UseStandardWidth(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x113, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH get_Validation()
	{
		LPDISPATCH result;
		InvokeHelper(0x56b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_Value(VARIANT& RangeValueDataType)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, &RangeValueDataType);
		return result;
	}
	void put_Value(VARIANT& RangeValueDataType, VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &RangeValueDataType, &newValue);
	}
	VARIANT get_Value2()
	{
		VARIANT result;
		InvokeHelper(0x56c, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_Value2(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x56c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_VerticalAlignment()
	{
		VARIANT result;
		InvokeHelper(0x89, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_VerticalAlignment(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x89, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	VARIANT get_Width()
	{
		VARIANT result;
		InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Worksheet()
	{
		LPDISPATCH result;
		InvokeHelper(0x15c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	VARIANT get_WrapText()
	{
		VARIANT result;
		InvokeHelper(0x114, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
		return result;
	}
	void put_WrapText(VARIANT& newValue)
	{
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x114, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &newValue);
	}
	LPDISPATCH AddComment(VARIANT& Text)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x56d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Text);
		return result;
	}
	LPDISPATCH get_Comment()
	{
		LPDISPATCH result;
		InvokeHelper(0x38e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ClearComments()
	{
		InvokeHelper(0x56e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Phonetic()
	{
		LPDISPATCH result;
		InvokeHelper(0x56f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_FormatConditions()
	{
		LPDISPATCH result;
		InvokeHelper(0x570, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_ReadingOrder()
	{
		long result;
		InvokeHelper(0x3cf, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_ReadingOrder(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3cf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_Hyperlinks()
	{
		LPDISPATCH result;
		InvokeHelper(0x571, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Phonetics()
	{
		LPDISPATCH result;
		InvokeHelper(0x713, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetPhonetic()
	{
		InvokeHelper(0x714, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_ID()
	{
		CString result;
		InvokeHelper(0x715, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ID(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x715, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	VARIANT PrintOut(VARIANT& From, VARIANT& To, VARIANT& Copies, VARIANT& Preview, VARIANT& ActivePrinter, VARIANT& PrintToFile, VARIANT& Collate, VARIANT& PrToFileName)
	{
		VARIANT result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x6ec, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, &From, &To, &Copies, &Preview, &ActivePrinter, &PrintToFile, &Collate, &PrToFileName);
		return result;
	}
	LPDISPATCH get_PivotCell()
	{
		LPDISPATCH result;
		InvokeHelper(0x7dd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Dirty()
	{
		InvokeHelper(0x7de, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_Errors()
	{
		LPDISPATCH result;
		InvokeHelper(0x7df, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_SmartTags()
	{
		LPDISPATCH result;
		InvokeHelper(0x7e0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Speak(VARIANT& SpeakDirection, VARIANT& SpeakFormulas)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7e1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &SpeakDirection, &SpeakFormulas);
	}
	VARIANT PasteSpecial(long Paste, long Operation, VARIANT& SkipBlanks, VARIANT& Transpose)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x788, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Paste, Operation, &SkipBlanks, &Transpose);
		return result;
	}
	BOOL get_AllowEdit()
	{
		BOOL result;
		InvokeHelper(0x7e4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_ListObject()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_XPath()
	{
		LPDISPATCH result;
		InvokeHelper(0x8d2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// Range 属性
public:

};
