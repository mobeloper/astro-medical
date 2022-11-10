// ImageInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Astro.h"
#include "ImageInfo.h"
#include "afxdialogex.h"


// CImageInfo dialog


IMPLEMENT_DYNAMIC(CImageInfo, CDialog)

CImageInfo::CImageInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CImageInfo::IDD, pParent)
	, sFormatType(_T(""))
	, intWidth()
	, intHeight()
{
	
}

CImageInfo::~CImageInfo()
{
}

void CImageInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMTYPE, wndTypes);
	DDX_CBString(pDX, IDC_IMTYPE, sFormatType);

	DDX_Text(pDX, IDC_WIDTH, intWidth);
	DDX_Text(pDX, IDC_HEIGHT, intHeight);
	DDX_Control(pDX, IDC_WIDTH, sWidth);
	DDX_Control(pDX, IDC_HEIGHT, sHeight);
	DDX_Control(pDX, IDC_ENDIAN, pCheckEndian);
}


BEGIN_MESSAGE_MAP(CImageInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CImageInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CImageInfo message handlers

BOOL CImageInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_IMTYPE);

	//0.char, 1.unsigned char, 2.short, 3.unsigned short, 4.int, 5.unsigned int, 6.float
	pComboBox->AddString(_T("Char 8-bit"));
	pComboBox->AddString(_T("Unsigned 8-bit"));
	pComboBox->AddString(_T("Short 16-bit"));
	pComboBox->AddString(_T("Unsigned 16-bit"));
	pComboBox->AddString(_T("int 32-bit"));
	pComboBox->AddString(_T("Unsigned 32-bit"));
	pComboBox->AddString(_T("Float 32-bit"));

	pComboBox->SetCurSel(3);	


	CString s;

	intWidth=3072;
	s.Format(_T("%d"),intWidth);
	sWidth.SetWindowText(s);
	
	intHeight=3072;
	s.Format(_T("%d"),intHeight);
	sHeight.SetWindowText(s);
	

	//wndTypes.AddString("usa");
	//wndTypes.SetEditSel(0,-1);
	//wndTypes.Clear();


	return TRUE;  
}


void CImageInfo::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();

	//Get the text
	intType = wndTypes.GetCurSel();
	
	if(intType != CB_ERR)
	{
	  wndTypes.GetLBText(intType,sFormatType);
	}

	intWidth = GetDlgItemInt(IDC_WIDTH);
	intHeight = GetDlgItemInt(IDC_HEIGHT);
	nBigEndian = pCheckEndian.GetCheck();

	return;
}
