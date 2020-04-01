#include "StdAfx.h"
#include "ExcelRangeEx.h"

#include <comdef.h>

CExcelRangeEx::CExcelRangeEx()
{
}


CExcelRangeEx::~CExcelRangeEx()
{
}

CExcelRangeEx::CExcelRangeEx(LPDISPATCH pDispatch)
{
	_range.AttachDispatch(pDispatch);
}

CExcelRangeEx::CExcelRangeEx(const CExcelRangeEx& dispatchSrc)
{
	_range = dispatchSrc._range;
}

CExcelRangeEx& CExcelRangeEx::operator =(const CExcelRangeEx& dispatchSrc)
{
	_range = dispatchSrc._range;
	return *this;
}

BOOL CExcelRangeEx::SetRange(CExcelSheet &sheet, int nMinRow, int nMinCol, int nMaxRow, int nMaxCol)
{
	_range = sheet.get_Range(COleVariant(GetCellTitle(nMinRow, nMinCol)), COleVariant(GetCellTitle(nMaxRow, nMaxCol)));
	return TRUE;
}

BOOL CExcelRangeEx::Merge()
{
	_range.Merge(_variant_t(0));
	return TRUE;
}

BOOL CExcelRangeEx::SetBorder()
{
	_variant_t v1;  //线型
	_variant_t v2;  //宽度
	_variant_t v3;  //颜色

	v1.vt=VT_I2;
	v1.lVal=1; // 线的样式：- no line; 1-solid; 2-big dot;3-small dot;4-dash dot; 5-dash dot dot;  

	v2.vt=v1.vt;
	v2.lVal=2; // 线的粗细程度

	v3.vt=v1.vt;
	v3.lVal=1; // 1-black;2-white;3-red;4-green;5-blue; 6-yellow; 7-pink;8-dark blue;  

	_range.BorderAround(v1,v2,v3,vtMissing);//设置边框

// 	CExcelBorders bds;
// 	bds.AttachDispatch(_range.get_Borders());
// 	CExcelBorder bd;
// 	bd.AttachDispatch(bds.get_Item(1));
// 	bd.put_LineStyle(_variant_t(1));
// 	bd.put_Color(_variant_t(RGB(0,0,0)));
// 	bd.put_Weight(_variant_t(1));
	return TRUE;
}

BOOL CExcelRangeEx::SetFormat(DWORD nFormat)
{
	if(DT_SINGLELINE&nFormat)
		_range.put_WrapText(_variant_t(1));

	if(DT_CENTER&nFormat)
		_range.put_HorizontalAlignment(_variant_t(-4108));
	else if(DT_LEFT&nFormat)
		_range.put_HorizontalAlignment(_variant_t(-4131));
	else if(DT_RIGHT&nFormat)
		_range.put_HorizontalAlignment(_variant_t(-4152));

	if(DT_VCENTER&nFormat)
		_range.put_VerticalAlignment(_variant_t(-4108));
	else if(DT_TOP&nFormat)
		_range.put_VerticalAlignment(_variant_t(-4131));
	else if(DT_BOTTOM&nFormat)
		_range.put_VerticalAlignment(_variant_t(-4152));

	return TRUE;
}

BOOL CExcelRangeEx::SetBackColor(COLORREF clr)
{
	CExcelInterior Itor;
	Itor.AttachDispatch(_range.get_Interior());
	Itor.put_Color(_variant_t(clr));
	return TRUE;
}