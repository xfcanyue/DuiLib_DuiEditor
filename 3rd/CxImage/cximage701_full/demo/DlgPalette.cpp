// DlgPalette.cpp : implementation file
/* Based on CColourPicker and CColourPopup, by Chris Maunder */

#include "stdafx.h"
#include "demo.h"
#include "DlgPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgPalette dialog


DlgPalette::DlgPalette(CWnd* pParent /*=NULL*/)
	: CDialog(DlgPalette::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgPalette)
	m_editalpha = FALSE;
	//}}AFX_DATA_INIT
}


void DlgPalette::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgPalette)
	DDX_Control(pDX, IDC_BAR, m_bar);
	DDX_Control(pDX, IDC_RADIO3, m_r3);
	DDX_Control(pDX, IDC_RADIO2, m_r2);
	DDX_Control(pDX, IDC_RADIO1, m_r1);
	DDX_Control(pDX, IDC_LOAD, m_load);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Check(pDX, IDC_CHECK2, m_editalpha);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgPalette, CDialog)
	//{{AFX_MSG_MAP(DlgPalette)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK2, OnEditAlpha)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgPalette message handlers

BOOL DlgPalette::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_save.SetIcon(IDI_B,BS_LEFT);
	m_load.SetIcon(IDI_B,BS_LEFT);

	m_r1.SetCheck(1);

	m_boxgap=1;
	m_boxheight=11;
	m_boxwidth=11;
	m_yoffset=14;
	m_xoffset=14;
	m_numcolumns=16;
	m_numrows=16;

	m_changed=0;
	m_selected=-1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
long DlgPalette::GetColumn(long index) const
{
	return index % m_numcolumns; 
}
/////////////////////////////////////////////////////////////////////////////
long DlgPalette::GetRow(long index) const
{
	return index / m_numcolumns; 
}
/////////////////////////////////////////////////////////////////////////////
long DlgPalette::GetIndex(long row, long col) const
{
	long i=row*m_numcolumns + col;
	if (i >= m_numcolors || row!=GetRow(i) || col!=GetColumn(i))
		return -1;
	else
		return i;
}
/////////////////////////////////////////////////////////////////////////////
void DlgPalette::DrawCell(CDC *pDC, long index)
{
	CRect rect;
	if (!GetCellRect(index, rect)) return;

	CBrush brush;
	
	if (m_editalpha){
		brush.CreateSolidBrush(RGB(m_pal[index].rgbReserved,
					 m_pal[index].rgbReserved ,
					 m_pal[index].rgbReserved ));
	}else{
		brush.CreateSolidBrush(RGB(m_pal[index].rgbRed ,
					 m_pal[index].rgbGreen ,
					 m_pal[index].rgbBlue ));
	}

	CPen   pen;
	if (index==m_selected)
		pen.CreatePen(PS_SOLID, 2, ::GetSysColor(COLOR_3DDKSHADOW));
	else
		pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

	CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
	CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

	// Draw the cell colour
	pDC->Rectangle(rect);

	// restore DC and cleanup
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	brush.DeleteObject();
	pen.DeleteObject();
}
/////////////////////////////////////////////////////////////////////////////
bool DlgPalette::GetCellRect(long index, const LPRECT &rect)
{
	if (index < 0 || index >= m_numcolors) return false;

	rect->left = GetColumn(index) * (m_boxwidth+m_boxgap) + m_xoffset;
	rect->top  = GetRow(index) * (m_boxheight+m_boxgap) + m_yoffset;
	rect->right = rect->left + m_boxwidth;
	rect->bottom = rect->top + m_boxheight;

	return true;
}
/////////////////////////////////////////////////////////////////////////////



void DlgPalette::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    for (int i = 0; i<m_numcolors; i++) DrawCell(&dc, i);
}
/////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnEditAlpha() 
{
	UpdateData(1);
	Invalidate(0);	
}
/////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    long x = point.x - m_xoffset;
    long y = point.y - m_yoffset;

	if (x>=0 && y>=0){
		long i = GetIndex(y / (m_boxheight+m_boxgap), x / (m_boxwidth+m_boxgap));

		if (i>=0 && i<m_numcolors){

			COLORREF c;

			if (m_editalpha)
				c=RGB(m_pal[i].rgbReserved,m_pal[i].rgbReserved,m_pal[i].rgbReserved);
			else
				c=RGB(m_pal[i].rgbRed,m_pal[i].rgbGreen,m_pal[i].rgbBlue);

	        CColorDialog dlg(c,	CC_FULLOPEN | CC_ANYCOLOR, this);

			if (dlg.DoModal() == IDOK){
				c = dlg.GetColor();
				if (m_editalpha){
					m_pal[i].rgbReserved=GetRValue(c);
				} else {
					m_pal[i].rgbRed=GetRValue(c);
					m_pal[i].rgbGreen=GetGValue(c);
					m_pal[i].rgbBlue=GetBValue(c);
				}
				m_changed=1;
				m_selected=i;
				Invalidate(1);
			}
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnMouseMove(UINT nFlags, CPoint point) 
{
    long x = point.x - m_xoffset;
    long y = point.y - m_yoffset;

	if (x>=0 && y>=0){
		long i = GetIndex(y / (m_boxheight+m_boxgap), x / (m_boxwidth+m_boxgap));

		if (i>=0 && i<m_numcolors){

			CString s;
			s.Format(_T(" I : %d, RGBA : %d, %d, %d, %d"),i,m_pal[i].rgbRed,m_pal[i].rgbGreen,m_pal[i].rgbBlue,m_pal[i].rgbReserved);
			m_bar.SetWindowText(s);
			m_bar.Invalidate(0);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnOK() 
{
	if (m_r1.GetCheck()) m_replace	=1;
	if (m_r2.GetCheck()) m_replace	=2;
	if (m_r3.GetCheck()) m_replace	=3;
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////
// prompt for file name - used for open and save as
// static function called from app
BOOL DlgPalette::PromptForFileName(CString& fileName, UINT /*nIDSTitle*/, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open file"); else title=_T("Save file");

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : 0;

	int nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = nIndex +1;
	CString strDefExt = _T("pal");
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter = _T("*.pal");
	strFilter += (TCHAR)NULL;
	strFilter += _T("*.pal");
	strFilter += (TCHAR)NULL;
	strFilter += _T("*.*");
	strFilter += (TCHAR)NULL;
	strFilter += _T("*.*");
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet){
		if (pType != NULL){
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
		}
	}
	return bRet;
}
//////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnLoad() 
{
	CString filename;
	if (!PromptForFileName(filename, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled
	
	FILE* f;
	f=_tfopen(filename,_T("rb"));
	if (f==NULL) return;

	char key[5];
	fread(key,4,1,f);
	key[4]=0;

	if (strcmp(key,"JASC")==0){
		long pos,size;
		pos = ftell(f);
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, pos,SEEK_SET);
		char* buffer = (char*)malloc(size);
		fread(buffer,size,1,f);
		fclose(f);
		char* token;
		token = strtok(buffer,"\n");
		if (token){
			token=strtok(NULL,"\n");
			token=strtok(NULL,"\n");
			long numcolors=atol(token);
			if (numcolors>256){
				free(buffer);
				AfxMessageBox(_T("Too much colors!"));
				return;
			}
			if (numcolors!=m_numcolors){
				numcolors = min(m_numcolors,numcolors);
				AfxMessageBox(_T("Warning: numcolors doesn't match"));
			}
			long i=0;
			while (token && i<numcolors){
				token=strtok(NULL," \n");
				if (token) m_pal[i].rgbRed=(BYTE)atoi(token);
				token=strtok(NULL," \n");
				if (token) m_pal[i].rgbGreen=(BYTE)atoi(token);
				token=strtok(NULL," \n");
				if (token) m_pal[i].rgbBlue=(BYTE)atoi(token);
				i++;
			}
		}
		free(buffer);
		m_changed=1;
		Invalidate(0);
		return;
	}

	if (strcmp(key,"RIFF")==0){
		fseek(f,22,SEEK_SET);
		long numcolors=0;
		fread(&numcolors,2,1,f);
		if (numcolors>256){
			fclose(f);
			AfxMessageBox(_T("Too much colors!"));
			return;
		}
		if (numcolors!=m_numcolors){
			numcolors = min(m_numcolors,numcolors);
			AfxMessageBox(_T("Warning: numcolors doesn't match"));
		}
		fread(m_pal,numcolors,sizeof(RGBQUAD),f);
		for (int i=0;i<numcolors;i++){
			BYTE tmp = m_pal[i].rgbBlue;
			m_pal[i].rgbBlue = m_pal[i].rgbRed;
			m_pal[i].rgbRed = tmp;
		}
		fclose(f);
		m_changed=1;
		Invalidate(0);
		return;
	}

	fclose(f);
	AfxMessageBox(_T("Unknown contents!"));
}
//////////////////////////////////////////////////////////////////////////////
void DlgPalette::OnSave() 
{
	CString filename;
	if (PromptForFileName(filename, 0,	OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, 0)){
		FILE* f;
		f=_tfopen(filename,_T("wb"));
		if (f) {
			long tmp;
			fwrite("RIFF",4,1,f);
			tmp = 20 + 4*m_numcolors;
			fwrite(&tmp,4,1,f);
			fwrite("PAL data",8,1,f);
			tmp = 8 + 4*m_numcolors;
			fwrite(&tmp,4,1,f);
			tmp=0x0300;
			fwrite(&tmp,2,1,f);
			fwrite(&m_numcolors,2,1,f);
			for(int i=0;i<m_numcolors;i++){
				fwrite(&m_pal[i].rgbRed,1,1,f);
				fwrite(&m_pal[i].rgbGreen,1,1,f);
				fwrite(&m_pal[i].rgbBlue,1,1,f);
				fwrite(&m_pal[i].rgbReserved,1,1,f);
			}
			tmp=0;
			fwrite(&tmp,4,1,f);
			fclose(f);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
