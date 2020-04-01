#pragma once

#include "ExcelApplication.h"
#include "ExcelBook.h"
#include "ExcelBooks.h"
#include "ExcelSheet.h"
#include "ExcelSheets.h"
#include "ExcelRange.h"
#include "ExcelCnterior.h"
#include "CExcelBorder.h"
#include "CExcelBorders.h"

class CExcelRangeEx
{
public:
	CExcelRangeEx();
	CExcelRangeEx(LPDISPATCH pDispatch);
	CExcelRangeEx(const CExcelRangeEx& dispatchSrc);
	CExcelRangeEx& operator =(const CExcelRangeEx& dispatchSrc);
	~CExcelRangeEx();

	BOOL SetRange(CExcelSheet &sheet, int nMinRow, int nMinCol, int nMaxRow, int nMaxCol);

	BOOL Merge();

	BOOL SetBorder();
	BOOL SetFormat(DWORD nFormat);
	BOOL SetBackColor(COLORREF clr);
public:
	CString GetCellTitle(int row, int col)
	{
		CString strRet;
		strRet.Format(_T("%c%d"), col+65, row+1);
		return strRet;
	}
public:
	CExcelRange _range;
};

