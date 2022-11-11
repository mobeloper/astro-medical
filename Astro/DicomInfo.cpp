// DicomInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Astro.h"
#include "DicomInfo.h"
#include "afxdialogex.h"

#include "AstroDoc.h"
#include "AstroView.h"
#include "CDib.h"


// CDicomInfo dialog

IMPLEMENT_DYNAMIC(CDicomInfo, CDialogEx)



//Call global variables
extern CAstroView *ActiveWnd;



CDicomInfo::CDicomInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDicomInfo::IDD, pParent)
{

}

CDicomInfo::~CDicomInfo()
{
}

void CDicomInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DCM, m_DcmList);
}


BEGIN_MESSAGE_MAP(CDicomInfo, CDialogEx)
END_MESSAGE_MAP()


// CDicomInfo message handlers


BOOL CDicomInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_font.CreateFont(16, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
	m_DcmList.SetFont(&m_font);


/*-----------------------------------------------------------*/	
	//m_DcmList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_DcmList.SetExtendedStyle(
		//LVS_EX_GRIDLINES	|		
		LVS_EX_HEADERINALLVIEWS	|
		//LVS_EX_AUTOSIZECOLUMNS |
		LVS_EX_FULLROWSELECT
		);
		

	m_DcmList.SetBkColor(RGB(0,0,0));
	m_DcmList.SetTextColor(RGB(255,255,255));	
	m_DcmList.SetTextBkColor(TRANSPARENT);
	
	//Column Names
	m_DcmList.InsertColumn(0, "Group, Element", LVCFMT_LEFT, 110);
	m_DcmList.InsertColumn(1, "Tag Description", LVCFMT_LEFT, 240);
	m_DcmList.InsertColumn(2, "VR", LVCFMT_LEFT, 40);
	m_DcmList.InsertColumn(3, "Length", LVCFMT_LEFT, 70);
	m_DcmList.InsertColumn(4, "Value", LVCFMT_LEFT, 500);

	//Fill the Rows
	int nNumTags = ActiveWnd->nNumbTags;	//Get Number of Tags
	int nImagFmat = ActiveWnd->nImgFmat;	//Get the Image format: DICOM or RAW

	CString str;

	if(nImagFmat==1)	//If the image is DICOM...
	{
	
		for(int nTN=0;nTN<nNumTags;nTN++)
		{
			str.Format("%s, %s",(CString)ActiveWnd->charDCMtags[nTN].group_tag,(CString)ActiveWnd->charDCMtags[nTN].element_tag);			
			m_DcmList.InsertItem(nTN,str);
			str.Format("%s",(CString)ActiveWnd->charDCMtags[nTN].Tname);
			m_DcmList.SetItemText(nTN,1,str);
			str.Format("%s",(CString)ActiveWnd->charDCMtags[nTN].VR);
			m_DcmList.SetItemText(nTN,2,str);

			str.Format("%s",(CString)ActiveWnd->charDCMtags[nTN].VL);
			m_DcmList.SetItemText(nTN,3,str);
			
			
			if(ActiveWnd->charDCMtags[nTN].VL[0] != '0')
			{

					if(ActiveWnd->charDCMtags[nTN].VR[0]=='D' && ActiveWnd->charDCMtags[nTN].VR[1]=='A')
					{
						str.Format("%s%s/%s%s/%s%s%s%s",(CString)ActiveWnd->charDCMtags[nTN].VF[4],
							(CString)ActiveWnd->charDCMtags[nTN].VF[5],(CString)ActiveWnd->charDCMtags[nTN].VF[6],
							(CString)ActiveWnd->charDCMtags[nTN].VF[7],(CString)ActiveWnd->charDCMtags[nTN].VF[0],
							(CString)ActiveWnd->charDCMtags[nTN].VF[1],(CString)ActiveWnd->charDCMtags[nTN].VF[2],
							(CString)ActiveWnd->charDCMtags[nTN].VF[3]);

					}
					else if(ActiveWnd->charDCMtags[nTN].VR[0]=='T' && ActiveWnd->charDCMtags[nTN].VR[1]=='M')
					{

						str.Format("%s%s:%s%s:%s%s",(CString)ActiveWnd->charDCMtags[nTN].VF[0],
							(CString)ActiveWnd->charDCMtags[nTN].VF[1],(CString)ActiveWnd->charDCMtags[nTN].VF[2],
							(CString)ActiveWnd->charDCMtags[nTN].VF[3],(CString)ActiveWnd->charDCMtags[nTN].VF[4],
							(CString)ActiveWnd->charDCMtags[nTN].VF[5]);

					}
					else if(ActiveWnd->charDCMtags[nTN].VR[0]=='D' && ActiveWnd->charDCMtags[nTN].VR[1]=='T')
					{
						str.Format("%s%s/%s%s/%s%s%s%s  %s%s:%s%s:%s%s",(CString)ActiveWnd->charDCMtags[nTN].VF[4],
							(CString)ActiveWnd->charDCMtags[nTN].VF[5],(CString)ActiveWnd->charDCMtags[nTN].VF[6],
							(CString)ActiveWnd->charDCMtags[nTN].VF[7],(CString)ActiveWnd->charDCMtags[nTN].VF[0],
							(CString)ActiveWnd->charDCMtags[nTN].VF[1],(CString)ActiveWnd->charDCMtags[nTN].VF[2],
							(CString)ActiveWnd->charDCMtags[nTN].VF[3],

							(CString)ActiveWnd->charDCMtags[nTN].VF[8],
							(CString)ActiveWnd->charDCMtags[nTN].VF[9],(CString)ActiveWnd->charDCMtags[nTN].VF[10],
							(CString)ActiveWnd->charDCMtags[nTN].VF[11],(CString)ActiveWnd->charDCMtags[nTN].VF[12],
							(CString)ActiveWnd->charDCMtags[nTN].VF[13]);
					}
					else
					{
						str.Format("%s",(CString)ActiveWnd->charDCMtags[nTN].VF);
					}


			}
			else
				str.Format("(no value available)");



			m_DcmList.SetItemText(nTN,4,str);
		}
	}
	else
	{		
		//m_DcmList.InsertItem(0,"");
		m_DcmList.InsertItem(0,"RAW file");		
		m_DcmList.SetItemText(0,1,"No DICOM Information Available");		

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


