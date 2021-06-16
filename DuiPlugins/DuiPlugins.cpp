// DuiPlugins.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

UILIB_EXP_API void __stdcall DuiPluginsRegister()
{
 	REGIST_DUICONTROL(CGridCtrlUI);
	REGIST_DUICONTROL(CKeyBoardUI);
	REGIST_DUICONTROL(CKeyBoardNumberUI);
	REGIST_DUICONTROL(CKeyBoardSimpleUI);
	REGIST_DUICONTROL(CSciEditUI);
}


#ifndef UILIB_STATIC
UILIB_EXP_API CControlUI *CreateControl(LPCTSTR pstrClass)
{
	if( _tcscmp(pstrClass, _T("GridCtrl")) == 0 ) 
	{
		return new CGridCtrlUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoard")) == 0 ) 
	{
		return new CKeyBoardUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardNumber")) == 0 ) 
	{
		return new CKeyBoardNumberUI;
	}
	else if( _tcscmp(pstrClass, _T("KeyBoardSimple")) == 0 ) 
	{
		return new CKeyBoardSimpleUI;
	}
	else if( _tcscmp(pstrClass, _T("ComboEx")) == 0 ) 
	{
		return new CComboExUI;
	}
	else if( _tcscmp(pstrClass, _T("SciEdit")) == 0 ) 
	{
		return new CSciEditUI;
	}
	return NULL;
}
#endif

UILIB_EXP_API void __stdcall InsertMsgUI(LPCTSTR pstring, COLORREF cr)
{
	CMsgWndUI::InsertMsg(pstring, cr);
}

UILIB_EXP_API void __stdcall InsertMsgUiV(LPCTSTR lpszFormat, ...)
{
//	ASSERT(AfxIsValidString(lpszFormat));

	CString strText;

	va_list argList;
	va_start(argList, lpszFormat);
	strText.FormatV(lpszFormat, argList);
	va_end(argList);

	CMsgWndUI::InsertMsg(strText, RGB(0,0,0));
}

static void grid_to_xml(xml_document &xml, CGridUI *pGrid)
{
	xml_node root = xml.append_child(_T("Grid"));

	for (int i=pGrid->GetFixedRowCount(); i<pGrid->GetRowCount(); i++)
	{
		TRowData *pRowData = pGrid->GetRowData(i);
		if(!pRowData) continue;

		xml_node nodeRow = root.append_child(_T("Row"));;

		for (int j=0; j<pGrid->GetColumnCount(); j++)
		{
			TCellData *pCellData = pGrid->GetCellData(i,j);
			if(!pCellData) continue;

			xml_node nodeCell = nodeRow.append_child(_T("Cell"));
			nodeCell.text().set(pCellData->GetText());
		}
	}
}

static bool xml_to_grid(xml_document &xml, CGridUI *pGrid)
{
	xml_node root = xml.child(_T("Grid"));
	if(!root) return false;

	for (xml_node nodeRow = root.child(_T("Row")); nodeRow; nodeRow=nodeRow.next_sibling(_T("Row")))
	{
		int row = pGrid->InsertRow();
		int col = 0;
		for (xml_node nodeCell = nodeRow.child(_T("Cell")); nodeCell; nodeCell=nodeCell.next_sibling(_T("Cell")))
		{
			pGrid->Cell(row,col).SetText(nodeCell.text().get());
			col++;
		}
	}
	return true;
}

//load grid from xml string content, virtual grid will be failed
UILIB_EXP_API bool __stdcall UIGrid_LoadXmlString(CGridUI *pGrid, LPCTSTR lpstring)
{
	if(pGrid->IsVirtualGrid()) return false;
	
	return false;
}

//save grid to xml string content, virtual grid will be failed
UILIB_EXP_API bool __stdcall UIGrid_SaveXmlString(CGridUI *pGrid, CDuiString &strXml)
{
	if(pGrid->IsVirtualGrid()) return false;


	return true;
}

//load grid from xml file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_LoadXmlFile(CGridUI *pGrid, LPCTSTR filename, bool bOpenFileDialog)
{	
	if(pGrid->IsVirtualGrid()) return false;
	pGrid->ResetGridBody();
	
	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("XML(*.xml)|*.xml|All Files (*.*)|*.*||");
		CFileDialog fileDlg(TRUE,_T("xml"), _T(""),  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return false;
		strFileName = fileDlg.GetPathName();
	}
	else
	{
		strFileName = filename;
	}

	xml_document xml;
	xml_parse_result ret = xml.load_file(strFileName);
	if(ret.status != pugi::status_ok) return false;
	return xml_to_grid(xml, pGrid);

	return false;
}

//save grid to xml file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_SaveXmlFile(CGridUI *pGrid, LPCTSTR filename, bool bOpenFileDialog)
{
	
	if(pGrid->IsVirtualGrid()) return false;

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("XML(*.xml)|*.xml|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xml"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return false;
		strFileName = fileDlg.GetPathName();
	}
	else
	{
		strFileName = filename;
	}

	xml_document xml;
	grid_to_xml(xml, pGrid);
	return xml.save_file(strFileName);
	
	return false;
}

#include "ExcelFile/ExcelFile.h"
#include "ExcelFile/ExcelApplication.h"
#include "ExcelFile/ExcelBook.h"
#include "ExcelFile/ExcelBooks.h"
#include "ExcelFile/ExcelSheet.h"
#include "ExcelFile/ExcelSheets.h"
#include "ExcelFile/ExcelRange.h"
#include "ExcelFile/ExcelRangeEx.h"
//load grid from excel file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_LoadExcelFile(CGridUI *pGrid, LPCTSTR filename, bool bOpenFileDialog)
{
	return false;
	if(pGrid->IsVirtualGrid()) return false;
	pGrid->ResetGridBody();

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("Excel(*.xls)|*.xls|All Files (*.*)|*.*||");
		CFileDialog fileDlg(TRUE,_T("xls"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return FALSE;
		strFileName = fileDlg.GetPathName();
	}
	else
		strFileName = filename;

	CExcelFile f;
	if(!f.OpenFile(strFileName))	return false;
	f.LoadSheet(1);
	if(f.GetSheetCount() <= 0)	return false;

	for (int xlsRow=1; xlsRow<=f.GetRowsCount(); xlsRow++)
	{
		int row = pGrid->InsertRow();
		int col = 0;
		for (int xlsCol=1; xlsCol<=f.GetColumnsCount(); xlsCol++)
		{
			CString cellText;
			f.GetCell(xlsRow, xlsCol, cellText);
			pGrid->Cell(row,col).SetText(cellText);
		}
	}
	
	return true;
}

//save grid to excel file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_SaveExcelFile(CGridUI *pGrid, LPCTSTR filename, bool bOpenFileDialog)
{
	if(pGrid->IsVirtualGrid()) return false;

	CString strFileName;
	if(bOpenFileDialog)
	{
		static TCHAR BASED_CODE szFilter[] = _T("Excel(*.xls)|*.xls|All Files (*.*)|*.*||");
		CFileDialog fileDlg(FALSE,_T("xls"), filename,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
		fileDlg.m_ofn.lStructSize = 88;
		if(fileDlg.DoModal() != IDOK)	return FALSE;
		strFileName = fileDlg.GetPathName();
	}
	else
		strFileName = filename;

	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	if(bFind)	::DeleteFile(strFileName);

	CExcelFile f;
	if(!f.CreateFile())	return false;
	f.LoadSheet(1);

	int rowcount = pGrid->GetRowCount();
	int colcount = pGrid->GetColumnCount();
	CString cellText;
	int xlsRow=1;
	int xlsCol=1;
	for (int row=0; row<rowcount; row++)
	{
		xlsCol=1;
		for (int col=0; col<colcount; col++)
		{
			TCellData *pCell = pGrid->GetCellData(row,col);

			if(pCell->IsMergedWithOthers())	
			{
				xlsCol++;
				continue;
			}

			CExcelRangeEx rg;

			if(pGrid->IsMergedCell(row,col))
			{
				TGridMergeRange rc = pGrid->GetCellMergeRange(row,col);
				rg.SetRange(f._sheet, rc.begin_row, rc.begin_col, rc.end_row, rc.end_col);
				rg.Merge();
			}
			else
				rg.SetRange(f._sheet, row, col, row, col);

			cellText = pCell->GetText();

			f.SetCell(xlsRow, xlsCol, cellText);
			rg.SetBorder();
			rg.SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);

// 			DWORD clr = 0;
// 			if(pGrid->IsFixedRow(row) || pGrid->IsFixedCol(col))	
// 				clr = pGrid->GetFixedCellBkColor();
// 			else 
// 				clr = pGrid->GetCellBkColor();
// 			if(clr != 0)
// 				rg.SetBackColor(clr);

			xlsCol++;
		}
		xlsRow++;
	}

	f.SaveAs(strFileName);

	return true;
}