#include "stdafx.h"
#include "ExcelFile.h"

#include <comdef.h>

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #endif


CExcelFile::CExcelFile(void)
{
	InitExcel();
}


CExcelFile::~CExcelFile(void)
{
	UnInitExcel();
}

BOOL CExcelFile::InitExcel()
{
	UnInitExcel();
	if(!_app.CreateDispatch(_T("Excel.Application")))	
	{
		AfxMessageBox(_T("EXCEL Init Error!"),MB_OK|MB_ICONERROR);
		return FALSE;
	}

	_books.AttachDispatch(_app.get_Workbooks());
	return TRUE;
}

void CExcelFile::UnInitExcel()
{
	COleVariant covFalse((short)FALSE);
	COleVariant	covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 
	_range.ReleaseDispatch();
	_sheet.ReleaseDispatch();
	_sheets.ReleaseDispatch();
	_book.Close(covFalse,_variant_t(m_strPath),covOptional);
	_book.ReleaseDispatch();
	_books.Close();
	_books.ReleaseDispatch();
	_app.Quit();
	_app.ReleaseDispatch();
}

BOOL CExcelFile::OpenFile(LPCTSTR fileName)
{
	m_strPath = fileName;

	LPDISPATCH lpDis = NULL;

	lpDis = _books.Add(_variant_t(fileName));
	if(lpDis == NULL)	return FALSE;

	_book.AttachDispatch(lpDis);
	
	lpDis = _book.get_Worksheets();
	if(lpDis == NULL)
	{
		AfxMessageBox(_T("get_Worksheets Error"));
	}

	_sheets.AttachDispatch(lpDis);

	return TRUE;
}

BOOL CExcelFile::CreateFile()
{
	LPDISPATCH lpDis = NULL;
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);  
	lpDis = _books.Add(covOptional);
	if(lpDis == NULL)	return FALSE;

	_book.AttachDispatch(lpDis);
	_sheets.AttachDispatch(_book.get_Worksheets());

	return TRUE;
}

BOOL CExcelFile::Save()
{
	if(m_strPath.IsEmpty())	return FALSE;

	_book.Save();

	return TRUE;
}

BOOL CExcelFile::SaveAs(LPCTSTR fileName)
{
	_book.SaveAs(_variant_t(fileName), vtMissing,vtMissing,vtMissing,vtMissing,vtMissing
		,0,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing);
	return TRUE;
}

int CExcelFile::GetSheetCount()  
{  
	return _sheets.get_Count();  
}  

BOOL CExcelFile::LoadSheet(int iIndex)  
{  
	LPDISPATCH lpDis = NULL;
	lpDis = _sheets.get_Item(_variant_t((long)iIndex));  
	if (lpDis == NULL)	return FALSE;  

	_sheet.AttachDispatch(lpDis,true);  
	_range.AttachDispatch(_sheet.get_Cells());  //auto range
	_range.put_NumberFormat(_variant_t(_T("@")));

	return TRUE;  
}

int CExcelFile::GetColumnsCount()  
{  
	CExcelRange range;  
	CExcelRange usedRange;  
	usedRange.AttachDispatch(_sheet.get_UsedRange());  
	range.AttachDispatch(usedRange.get_Columns());  
	int count = range.get_Count();  
	usedRange.ReleaseDispatch();  
	range.ReleaseDispatch();  
	return count;  
}  

int CExcelFile::GetRowsCount()  
{  
	CExcelRange range;  
	CExcelRange usedRange;  
	usedRange.AttachDispatch(_sheet.get_UsedRange());  
	range.AttachDispatch(usedRange.get_Rows());  
	int count = range.get_Count();  
	usedRange.ReleaseDispatch();  
	range.ReleaseDispatch();  
	return count;  
}  

BOOL CExcelFile::SetCell(int iRow, int iColumn, const CString &ret)
{
	_range.put_Item(_variant_t(iRow), _variant_t(iColumn), _variant_t(ret));
	return TRUE;
}

BOOL CExcelFile::SetCell(int iRow, int iColumn, const int &ret)
{
	_range.put_Item(_variant_t(iRow), _variant_t(iColumn), _variant_t(ret));
	return TRUE;
}

BOOL CExcelFile::SetCell(int iRow, int iColumn, const COleDateTime &dt)
{
	return SetCell(iRow, iColumn, dt.Format(_T("%Y-%m-%d %H:%M:%S")));
}

BOOL CExcelFile::SetCell(int iRow, int iColumn, const COleCurrency &cy)
{
	return SetCell(iRow, iColumn, cy.Format());
}

BOOL CExcelFile::GetCell(int iRow, int iColumn, CString &ret)  
{  
	CExcelRange range;  
	range.AttachDispatch(_range.get_Item(COleVariant((long)iRow),COleVariant((long)iColumn)).pdispVal, true);  
	COleVariant vResult =range.get_Value2();  
	CString str;  
	if(vResult.vt == VT_BSTR)       //字符串   
	{  
		str=vResult.bstrVal;  
	}  
	else if (vResult.vt==VT_INT)  
	{  
		str.Format(_T("%d"),vResult.pintVal);  
	}  
	else if (vResult.vt==VT_R8)     //8字节的数字    
	{  
		str.Format(_T("%f"),vResult.dblVal);  
		//str.Format("%.0f",vResult.dblVal);   
		//str.Format("%1f",vResult.fltVal);   
	}  
	else if(vResult.vt==VT_DATE)    //时间格式   
	{  
		SYSTEMTIME st;  
		VariantTimeToSystemTime(vResult.date, &st); 
		str.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"));
	}  
	else if(vResult.vt==VT_EMPTY)   //单元格空的   
	{  
		str="";  
	}   
	range.ReleaseDispatch(); 

	ret = str;
	return TRUE;  
}  

BOOL CExcelFile::GetCell(int iRow, int iColumn, int &ret)  
{  
	CExcelRange range;  
	range.AttachDispatch(_range.get_Item(COleVariant((long)iRow),COleVariant((long)iColumn)).pdispVal);  
	COleVariant vResult =range.get_Value2();  
	ret = (int)vResult.date;  
	range.ReleaseDispatch();  
	return TRUE;  
}  

BOOL CExcelFile::GetCell(int iRow, int iColumn, COleDateTime &dt)
{
	CString temp;
	if(!GetCell(iRow, iColumn, temp))	return FALSE;
	if(temp.IsEmpty())	return FALSE;
	if(!dt.ParseDateTime(temp))	return FALSE;

	return TRUE;
}

BOOL CExcelFile::GetCell(int iRow, int iColumn, COleCurrency &cy)
{
	CString temp;
	if(!GetCell(iRow, iColumn, temp))	return FALSE;
	if(temp.IsEmpty())	return FALSE;
	if(!cy.ParseCurrency(temp))	return FALSE;
	return TRUE;
}

void CExcelFile::ShowInExcel(bool bShow)  
{  
	_app.put_Visible(bShow);  
}  

void CExcelFile::SetRowHeight(int row, int nHeight)
{
	CExcelRange range;  
	range.AttachDispatch(_range.get_Item(COleVariant((long)row),COleVariant((long)1)).pdispVal); 
	//double f = nHeight/8.08;
	//range.put_RowHeight((_variant_t)(double)f);
	range.put_RowHeight((_variant_t)(long)nHeight);
}

void CExcelFile::SetColumnWidth(int col, int nWidth)
{
	CExcelRange range;  
	range.AttachDispatch(_range.get_Item(COleVariant((long)1),COleVariant((long)col)).pdispVal); 
	double f = nWidth/8.08;
	range.put_ColumnWidth((_variant_t)(double)f);
}