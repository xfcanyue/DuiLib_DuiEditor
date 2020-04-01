// DlgCustomFilter.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgCustomFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgCustomFilter dialog


DlgCustomFilter::DlgCustomFilter(CWnd* pParent /*=NULL*/)
	: CDialog(DlgCustomFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgCustomFilter)
	m_Edit1x1 = 0;
	m_Edit1x2 = 0;
	m_Edit1x3 = 0;
	m_Edit1x4 = 0;
	m_Edit1x5 = 0;
	m_Edit2x1 = 0;
	m_Edit2x2 = 0;
	m_Edit2x3 = 0;
	m_Edit2x4 = 0;
	m_Edit2x5 = 0;
	m_Edit3x1 = 0;
	m_Edit3x2 = 0;
	m_Edit3x3 = 0;
	m_Edit3x4 = 0;
	m_Edit3x5 = 0;
	m_Edit4x1 = 0;
	m_Edit4x2 = 0;
	m_Edit4x3 = 0;
	m_Edit4x4 = 0;
	m_Edit4x5 = 0;
	m_Edit5x1 = 0;
	m_Edit5x2 = 0;
	m_Edit5x3 = 0;
	m_Edit5x4 = 0;
	m_Edit5x5 = 0;
	m_kSize=5;
	m_EditBias = 0;
	m_EditDivisor = 1;
	//}}AFX_DATA_INIT
}


void DlgCustomFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgCustomFilter)
	DDX_Control(pDX, IDC_COMBO10, m_cbPresets);
	DDX_Control(pDX, IDC_LOAD2, m_load);
	DDX_Control(pDX, IDC_SAVE2, m_save);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_canc);
	DDX_Text(pDX, IDC_EDIT1x1, m_Edit1x1);
	DDX_Text(pDX, IDC_EDIT1x2, m_Edit1x2);
	DDX_Text(pDX, IDC_EDIT1x3, m_Edit1x3);
	DDX_Text(pDX, IDC_EDIT1x4, m_Edit1x4);
	DDX_Text(pDX, IDC_EDIT1x5, m_Edit1x5);
	DDX_Text(pDX, IDC_EDIT2x1, m_Edit2x1);
	DDX_Text(pDX, IDC_EDIT2x2, m_Edit2x2);
	DDX_Text(pDX, IDC_EDIT2x3, m_Edit2x3);
	DDX_Text(pDX, IDC_EDIT2x4, m_Edit2x4);
	DDX_Text(pDX, IDC_EDIT2x5, m_Edit2x5);
	DDX_Text(pDX, IDC_EDIT3x1, m_Edit3x1);
	DDX_Text(pDX, IDC_EDIT3x2, m_Edit3x2);
	DDX_Text(pDX, IDC_EDIT3x3, m_Edit3x3);
	DDX_Text(pDX, IDC_EDIT3x4, m_Edit3x4);
	DDX_Text(pDX, IDC_EDIT3x5, m_Edit3x5);
	DDX_Text(pDX, IDC_EDIT4x1, m_Edit4x1);
	DDX_Text(pDX, IDC_EDIT4x2, m_Edit4x2);
	DDX_Text(pDX, IDC_EDIT4x3, m_Edit4x3);
	DDX_Text(pDX, IDC_EDIT4x4, m_Edit4x4);
	DDX_Text(pDX, IDC_EDIT4x5, m_Edit4x5);
	DDX_Text(pDX, IDC_EDIT5x1, m_Edit5x1);
	DDX_Text(pDX, IDC_EDIT5x2, m_Edit5x2);
	DDX_Text(pDX, IDC_EDIT5x3, m_Edit5x3);
	DDX_Text(pDX, IDC_EDIT5x4, m_Edit5x4);
	DDX_Text(pDX, IDC_EDIT5x5, m_Edit5x5);
	DDX_Text(pDX, IDC_EDIT_BIAS, m_EditBias);
	DDX_Text(pDX, IDC_EDIT_DIVISOR, m_EditDivisor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgCustomFilter, CDialog)
	//{{AFX_MSG_MAP(DlgCustomFilter)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio3x3)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio5x5)
	ON_BN_CLICKED(IDC_LOAD2, OnLoad)
	ON_BN_CLICKED(IDC_SAVE2, OnSave)
	ON_CBN_SELCHANGE(IDC_COMBO10, OnSelchangePresets)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgCustomFilter message handlers

BOOL DlgCustomFilter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ok.SetIcon(IDI_G,BS_LEFT);
	m_canc.SetIcon(IDI_R,BS_LEFT);
	m_save.SetIcon(IDI_B,BS_LEFT);
	m_load.SetIcon(IDI_B,BS_LEFT);

	m_Edit1x1 = dlgkernel[0];
	m_Edit1x2 = dlgkernel[1];
	m_Edit1x3 = dlgkernel[2];
	m_Edit1x4 = dlgkernel[3];
	m_Edit1x5 = dlgkernel[4];

	m_Edit2x1 = dlgkernel[5];
	m_Edit2x2 = dlgkernel[6];
	m_Edit2x3 = dlgkernel[7];
	m_Edit2x4 = dlgkernel[8];
	m_Edit2x5 = dlgkernel[9];

	m_Edit3x1 = dlgkernel[10];
	m_Edit3x2 = dlgkernel[11];
	m_Edit3x3 = dlgkernel[12];
	m_Edit3x4 = dlgkernel[13];
	m_Edit3x5 = dlgkernel[14];

	m_Edit4x1 = dlgkernel[15];
	m_Edit4x2 = dlgkernel[16];
	m_Edit4x3 = dlgkernel[17];
	m_Edit4x4 = dlgkernel[18];
	m_Edit4x5 = dlgkernel[19];

	m_Edit5x1 = dlgkernel[20];
	m_Edit5x2 = dlgkernel[21];
	m_Edit5x3 = dlgkernel[22];
	m_Edit5x4 = dlgkernel[23];
	m_Edit5x5 = dlgkernel[24];

	if (m_kSize==5){
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
		OnRadio5x5();
	} else {
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
		OnRadio3x3();
	}

	// TODO: Add extra initialization here
	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgCustomFilter::OnRadio3x3() 
{
	// TODO: Add your control notification handler code here
	if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT1x1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1x2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1x3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1x4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1x5)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT2x1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2x5)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT3x1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3x5)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT4x1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT4x5)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT5x1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT5x2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT5x3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT5x4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT5x5)->EnableWindow(FALSE);
	}
}

void DlgCustomFilter::OnRadio5x5() 
{
	// TODO: Add your control notification handler code here
	if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO2)
	{
		GetDlgItem(IDC_EDIT1x1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1x2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1x3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1x4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1x5)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT2x1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT2x5)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT3x1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT3x5)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT4x1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT4x5)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT5x1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5x2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5x3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5x4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5x5)->EnableWindow(TRUE);
	}
}

void DlgCustomFilter::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(1);

	if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO2)
		m_kSize=5;
	else
		m_kSize=3;

	dlgkernel[0]=m_Edit1x1;
	dlgkernel[1]=m_Edit1x2;
	dlgkernel[2]=m_Edit1x3;
	dlgkernel[3]=m_Edit1x4;
	dlgkernel[4]=m_Edit1x5;

	dlgkernel[5]=m_Edit2x1;
	dlgkernel[6]=m_Edit2x2;
	dlgkernel[7]=m_Edit2x3;
	dlgkernel[8]=m_Edit2x4;
	dlgkernel[9]=m_Edit2x5;

	dlgkernel[10]=m_Edit3x1;
	dlgkernel[11]=m_Edit3x2;
	dlgkernel[12]=m_Edit3x3;
	dlgkernel[13]=m_Edit3x4;
	dlgkernel[14]=m_Edit3x5;

	dlgkernel[15]=m_Edit4x1;
	dlgkernel[16]=m_Edit4x2;
	dlgkernel[17]=m_Edit4x3;
	dlgkernel[18]=m_Edit4x4;
	dlgkernel[19]=m_Edit4x5;

	dlgkernel[20]=m_Edit5x1;
	dlgkernel[21]=m_Edit5x2;
	dlgkernel[22]=m_Edit5x3;
	dlgkernel[23]=m_Edit5x4;
	dlgkernel[24]=m_Edit5x5;

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////////
// prompt for file name - used for open and save as
// static function called from app
BOOL DlgCustomFilter::PromptForFileName(CString& fileName, UINT /*nIDSTitle*/, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) title=_T("Open file"); else title=_T("Save file");

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : 0;

	int nIndex = 0;

	dlgFile.m_ofn.nFilterIndex = nIndex +1;
	CString strDefExt = _T("dat");
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter = _T("*.dat");
	strFilter += (TCHAR)NULL;
	strFilter += _T("*.dat");
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
void DlgCustomFilter::OnLoad() 
{
	CString filename;
	if (!PromptForFileName(filename, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
		return; // open cancelled
	
	FILE* f;
	f=_tfopen(filename,_T("rb"));
	if (f==NULL) return;

	char key[20];
	fread(key,19,1,f);
	key[19]=0;

	if (strcmp(key,"CxImageCustomKernel")==0){

		fread(&m_kSize,4,1,f);
		fread(&m_EditBias,4,1,f);
		fread(&m_EditDivisor,4,1,f);

		fread(&m_Edit1x1,4,1,f);
		fread(&m_Edit1x2,4,1,f);
		fread(&m_Edit1x3,4,1,f);
		fread(&m_Edit1x4,4,1,f);
		fread(&m_Edit1x5,4,1,f);

		fread(&m_Edit2x1,4,1,f);
		fread(&m_Edit2x2,4,1,f);
		fread(&m_Edit2x3,4,1,f);
		fread(&m_Edit2x4,4,1,f);
		fread(&m_Edit2x5,4,1,f);

		fread(&m_Edit3x1,4,1,f);
		fread(&m_Edit3x2,4,1,f);
		fread(&m_Edit3x3,4,1,f);
		fread(&m_Edit3x4,4,1,f);
		fread(&m_Edit3x5,4,1,f);

		fread(&m_Edit4x1,4,1,f);
		fread(&m_Edit4x2,4,1,f);
		fread(&m_Edit4x3,4,1,f);
		fread(&m_Edit4x4,4,1,f);
		fread(&m_Edit4x5,4,1,f);

		fread(&m_Edit5x1,4,1,f);
		fread(&m_Edit5x2,4,1,f);
		fread(&m_Edit5x3,4,1,f);
		fread(&m_Edit5x4,4,1,f);
		fread(&m_Edit5x5,4,1,f);

		fclose(f);

		UpdateData(0);

		if (m_kSize==5){
			CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
			OnRadio5x5();
		} else {
			CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
			OnRadio3x3();
		}

		m_cbPresets.SetCurSel(-1);

		return;
	}

	fclose(f);
	AfxMessageBox(_T("Unknown contents!"));
}
//////////////////////////////////////////////////////////////////////////////
void DlgCustomFilter::OnSave() 
{
	CString filename;
	if (PromptForFileName(filename, 0,	OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, 0)){
		FILE* f;
		f=_tfopen(filename,_T("wb"));
		if (f) {
			UpdateData(1);

			if(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO2)==IDC_RADIO2)
				m_kSize=5;
			else
				m_kSize=3;

			fwrite("CxImageCustomKernel",19,1,f);

			fwrite(&m_kSize,4,1,f);
			fwrite(&m_EditBias,4,1,f);
			fwrite(&m_EditDivisor,4,1,f);

			fwrite(&m_Edit1x1,4,1,f);
			fwrite(&m_Edit1x2,4,1,f);
			fwrite(&m_Edit1x3,4,1,f);
			fwrite(&m_Edit1x4,4,1,f);
			fwrite(&m_Edit1x5,4,1,f);

			fwrite(&m_Edit2x1,4,1,f);
			fwrite(&m_Edit2x2,4,1,f);
			fwrite(&m_Edit2x3,4,1,f);
			fwrite(&m_Edit2x4,4,1,f);
			fwrite(&m_Edit2x5,4,1,f);

			fwrite(&m_Edit3x1,4,1,f);
			fwrite(&m_Edit3x2,4,1,f);
			fwrite(&m_Edit3x3,4,1,f);
			fwrite(&m_Edit3x4,4,1,f);
			fwrite(&m_Edit3x5,4,1,f);

			fwrite(&m_Edit4x1,4,1,f);
			fwrite(&m_Edit4x2,4,1,f);
			fwrite(&m_Edit4x3,4,1,f);
			fwrite(&m_Edit4x4,4,1,f);
			fwrite(&m_Edit4x5,4,1,f);

			fwrite(&m_Edit5x1,4,1,f);
			fwrite(&m_Edit5x2,4,1,f);
			fwrite(&m_Edit5x3,4,1,f);
			fwrite(&m_Edit5x4,4,1,f);
			fwrite(&m_Edit5x5,4,1,f);

			fclose(f);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
void DlgCustomFilter::OnSelchangePresets() 
{
	switch(m_cbPresets.GetCurSel())
	{
	case 0: //Blur 5x5
		m_kSize = 5;
		m_EditBias = 0;
		m_EditDivisor = 25;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 1;
		m_Edit2x1 = m_Edit2x2 = m_Edit2x3 = m_Edit2x4 = m_Edit2x5 = 1;
		m_Edit3x1 = m_Edit3x2 = m_Edit3x3 = m_Edit3x4 = m_Edit3x5 = 1;
		m_Edit4x1 = m_Edit4x2 = m_Edit4x3 = m_Edit4x4 = m_Edit4x5 = 1;
		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 1;

		break;
	case 1: //Blur 3x3
		m_kSize = 3;
		m_EditBias = 0;
		m_EditDivisor = 9;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = 1;
		m_Edit2x3 = 1;
		m_Edit2x4 = 1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = 1;
		m_Edit3x3 = 1;
		m_Edit3x4 = 1;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = 1;
		m_Edit4x3 = 1;
		m_Edit4x4 = 1;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	case 2: //Soften 5x5
		m_kSize = 5;
		m_EditBias = 0;
		m_EditDivisor = 144;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 1;

		m_Edit2x1 = 1;
		m_Edit2x2 = 8;
		m_Edit2x3 = 8;
		m_Edit2x4 = 8;
		m_Edit2x5 = 1;

		m_Edit3x1 = 1;
		m_Edit3x2 = 8;
		m_Edit3x3 = 64;
		m_Edit3x4 = 8;
		m_Edit3x5 = 1;

		m_Edit4x1 = 1;
		m_Edit4x2 = 8;
		m_Edit4x3 = 8;
		m_Edit4x4 = 8;
		m_Edit4x5 = 1;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 1;

		break;
	case 3: //Soften 3x3
		m_kSize = 3;
		m_EditBias = 0;
		m_EditDivisor = 16;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = 1;
		m_Edit2x3 = 1;
		m_Edit2x4 = 1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = 1;
		m_Edit3x3 = 8;
		m_Edit3x4 = 1;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = 1;
		m_Edit4x3 = 1;
		m_Edit4x4 = 1;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	case 4: //Gaussian 5x5
		m_kSize = 5;
		m_EditBias = 0;
		m_EditDivisor = 52;

		m_Edit1x1 = 0;
		m_Edit1x2 = 1;
		m_Edit1x3 = 2;
		m_Edit1x4 = 1;
		m_Edit1x5 = 0;

		m_Edit2x1 = 1;
		m_Edit2x2 = 3;
		m_Edit2x3 = 4;
		m_Edit2x4 = 3;
		m_Edit2x5 = 1;

		m_Edit3x1 = 2;
		m_Edit3x2 = 4;
		m_Edit3x3 = 8;
		m_Edit3x4 = 4;
		m_Edit3x5 = 2;

		m_Edit4x1 = 1;
		m_Edit4x2 = 3;
		m_Edit4x3 = 4;
		m_Edit4x4 = 3;
		m_Edit4x5 = 1;

		m_Edit5x1 = 0;
		m_Edit5x2 = 1;
		m_Edit5x3 = 2;
		m_Edit5x4 = 1;
		m_Edit5x5 = 0;

		break;
	case 5: //Gaussian 3x3
		m_kSize = 3;
		m_EditBias = 0;
		m_EditDivisor = 16;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = 1;
		m_Edit2x3 = 2;
		m_Edit2x4 = 1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = 2;
		m_Edit3x3 = 4;
		m_Edit3x4 = 2;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = 1;
		m_Edit4x3 = 2;
		m_Edit4x4 = 1;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	case 6: //Sharpen 3x3
		m_kSize = 3;
		m_EditBias = 0;
		m_EditDivisor = 7;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = -1;
		m_Edit2x3 = -1;
		m_Edit2x4 = -1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = -1;
		m_Edit3x3 = 15;
		m_Edit3x4 = -1;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = -1;
		m_Edit4x3 = -1;
		m_Edit4x4 = -1;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	case 7: //Edge 3x3
		m_kSize = 3;
		m_EditBias = 255;
		m_EditDivisor = -1;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = -1;
		m_Edit2x3 = -1;
		m_Edit2x4 = -1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = -1;
		m_Edit3x3 = 8;
		m_Edit3x4 = -1;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = -1;
		m_Edit4x3 = -1;
		m_Edit4x4 = -1;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	case 8: //Emboss 3x3
		m_kSize = 3;
		m_EditBias = 127;
		m_EditDivisor = -1;

		m_Edit1x1 = m_Edit1x2 = m_Edit1x3 = m_Edit1x4 = m_Edit1x5 = 0;

		m_Edit2x1 = 0;
		m_Edit2x2 = 0;
		m_Edit2x3 = 0;
		m_Edit2x4 = -1;
		m_Edit2x5 = 0;

		m_Edit3x1 = 0;
		m_Edit3x2 = 0;
		m_Edit3x3 = 0;
		m_Edit3x4 = 0;
		m_Edit3x5 = 0;

		m_Edit4x1 = 0;
		m_Edit4x2 = 1;
		m_Edit4x3 = 0;
		m_Edit4x4 = 0;
		m_Edit4x5 = 0;

		m_Edit5x1 = m_Edit5x2 = m_Edit5x3 = m_Edit5x4 = m_Edit5x5 = 0;

		break;
	}

	UpdateData(0);

	if (m_kSize==5){
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO2);
		OnRadio5x5();
	} else {
		CheckRadioButton(IDC_RADIO1,IDC_RADIO2,IDC_RADIO1);
		OnRadio3x3();
	}

}
