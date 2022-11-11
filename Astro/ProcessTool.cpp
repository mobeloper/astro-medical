// ProcessTool.cpp : implementation file
//

#include "stdafx.h"
#include "Astro.h"
#include "ProcessTool.h"


#include "AstroDoc.h"
#include "AstroView.h"

#include "afxdialogex.h"



// CProcessTool dialog

extern CAstroView *ActiveWnd;


IMPLEMENT_DYNAMIC(CProcessTool, CDialogEx)

CProcessTool::CProcessTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcessTool::IDD, pParent)
{
	m_pParent = pParent;
	nWndDlgStat=0;
}

CProcessTool::~CProcessTool()
{
}

void CProcessTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMMA, GammaSlider);
	DDX_Control(pDX, IDC_WINDOW, WindowSlider);
	DDX_Control(pDX, IDC_LEVEL, LevelSlider);
	DDX_Control(pDX, IDC_EDGE, EdgeSlider);
	DDX_Control(pDX, IDC_SHARP, SharpSlider);
	DDX_Control(pDX, IDC_SMOOTH, SmoothSlider);
	DDX_Control(pDX, IDC_DENOISE, DenoiseSlider);
	DDX_Control(pDX, IDC_Gval, Gval);
	DDX_Control(pDX, IDC_Wval, Wval);
	DDX_Control(pDX, IDC_Lval, Lval);
	DDX_Control(pDX, IDC_Eval, Eval);
	DDX_Control(pDX, IDC_Shval, Shval);
	DDX_Control(pDX, IDC_Smval, Smval);
	DDX_Control(pDX, IDC_Fval, Fval);
	DDX_Control(pDX, IDC_INVERT_CONT, pInvertCntrst);
}


BEGIN_MESSAGE_MAP(CProcessTool, CDialogEx)
	ON_WM_HSCROLL()
	//ON_WM_MOUSEACTIVATE()
	//ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_WINDOW, &CProcessTool::OnNMReleasedcaptureWindow)
	ON_BN_CLICKED(IDC_ORIGINAL_IM, &CProcessTool::OnBnClickedOriginalIm)
	ON_BN_CLICKED(IDC_INVERT_CONT, &CProcessTool::OnBnClickedInvertCont)
END_MESSAGE_MAP()


// CProcessTool message handlers


BOOL CProcessTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	nGammaPos=ActiveWnd->nGamma;	
	nWndwPos=ActiveWnd->nWindow;
	nLvlPos=ActiveWnd->nLevel;

	nEdgePos=ActiveWnd->nEdge;
	nSmoothPos=ActiveWnd->nSmooth;
	nSharpPos=ActiveWnd->nSharp;
	nFiltPos=ActiveWnd->nFilter;


	CString s;

	s.Format(_T("%.2f"),ActiveWnd->fGamma);
	Gval.SetWindowText(s);

	s.Format(_T("%d"),nWndwPos);
	Wval.SetWindowText(s);

	s.Format(_T("%d"),nLvlPos);
	Lval.SetWindowText(s);

	s.Format(_T("%d"),nEdgePos);
	Eval.SetWindowText(s);

	s.Format(_T("%d"),nSmoothPos);
	Smval.SetWindowText(s);

	s.Format(_T("%d"),nSharpPos);
	Shval.SetWindowText(s);

	s.Format(_T("%d"),nFiltPos);
	Fval.SetWindowText(s);

	GammaSlider.SetRange(0, 10);

	WindowSlider.SetRange(ActiveWnd->nMinWindow, ActiveWnd->nMaxWindow);	// (1<<Bits allocated)
	LevelSlider.SetRange(ActiveWnd->nMinLevel, ActiveWnd->nMaxLevel);		// largest image pixel value

	
	EdgeSlider.SetRange(0, 30);
	SharpSlider.SetRange(0, 30);
	SmoothSlider.SetRange(0, 10);
	DenoiseSlider.SetRange(0, 10);


	GammaSlider.SetPos(nGammaPos);

	WindowSlider.SetPos(nWndwPos);
	LevelSlider.SetPos(nLvlPos);

	EdgeSlider.SetPos(nEdgePos);
	SmoothSlider.SetPos(nSmoothPos);
	SharpSlider.SetPos(nSharpPos);
	DenoiseSlider.SetPos(nFiltPos);

	pInvertCntrst.SetCheck(ActiveWnd->InvertPixels);

	
	nWndDlgStat=1; //A window dialog is active!
	
	return TRUE;  
}



BOOL CProcessTool::DisplayCurrValues()
{

	nGammaPos=ActiveWnd->nGamma;
	nWndwPos=ActiveWnd->nWindow;
	nLvlPos=ActiveWnd->nLevel;

	nEdgePos=ActiveWnd->nEdge;
	nSmoothPos=ActiveWnd->nSmooth;
	nSharpPos=ActiveWnd->nSharp;
	nFiltPos=ActiveWnd->nFilter;


	CString s;
	
	s.Format(_T("%.2f"),ActiveWnd->fGamma);
	Gval.SetWindowText(s);

	s.Format(_T("%d"),nWndwPos);
	Wval.SetWindowText(s);
	
	s.Format(_T("%d"),nLvlPos);
	Lval.SetWindowText(s);

	s.Format(_T("%d"),nEdgePos);
	Eval.SetWindowText(s);

	s.Format(_T("%d"),nSmoothPos);
	Smval.SetWindowText(s);

	s.Format(_T("%d"),nSharpPos);
	Shval.SetWindowText(s);

	s.Format(_T("%d"),nFiltPos);
	Fval.SetWindowText(s);

	
	GammaSlider.SetRange(0, 10);

	WindowSlider.SetRange(ActiveWnd->nMinWindow, ActiveWnd->nMaxWindow);	
	LevelSlider.SetRange(ActiveWnd->nMinLevel, ActiveWnd->nMaxLevel);		

	EdgeSlider.SetRange(0, 30);
	SharpSlider.SetRange(0, 50);
	SmoothSlider.SetRange(0, 30);
	DenoiseSlider.SetRange(0, 10);

	GammaSlider.SetPos(nGammaPos);
	WindowSlider.SetPos(nWndwPos);
	LevelSlider.SetPos(nLvlPos);

	EdgeSlider.SetPos(nEdgePos);
	SmoothSlider.SetPos(nSmoothPos);
	SharpSlider.SetPos(nSharpPos);
	DenoiseSlider.SetPos(nFiltPos);
	
	pInvertCntrst.SetCheck(ActiveWnd->InvertPixels);

	//return CProcessTool::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return true;
}


void CProcessTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	// TODO: Add your message handler code here and/or call default	
	CString s;
	int b=0;
	
	b=WindowSlider.GetPos();
	if (nWndwPos!=b)
	{	
		nWndwPos=b;
		s.Format(_T("%d"),nWndwPos);
		Wval.SetWindowText(s);

		ActiveWnd->nWindow=nWndwPos;
		//ActiveWnd->PostMessage(WM_COMMAND,0,0);	mensaje para indicar que la ventana fue manipulada, no se necesita porque tengo m_View
	}


	b=GammaSlider.GetPos();
	if (nGammaPos!=b)
	{	
		nGammaPos=b;

		if(nGammaPos==0)
			ActiveWnd->fGamma=0.04F;
		if(nGammaPos==1)
			ActiveWnd->fGamma=0.10F;
		if(nGammaPos==2)
			ActiveWnd->fGamma=0.20F;
		if(nGammaPos==3)
			ActiveWnd->fGamma=0.40F;
		if(nGammaPos==4)
			ActiveWnd->fGamma=0.67F;
		if(nGammaPos==5)
			ActiveWnd->fGamma=1.0F;
		if(nGammaPos==6)
			ActiveWnd->fGamma=1.5F;
		if(nGammaPos==7)
			ActiveWnd->fGamma=2.5F;
		if(nGammaPos==8)
			ActiveWnd->fGamma=5.0F;
		if(nGammaPos==9)
			ActiveWnd->fGamma=10.0F;
		if(nGammaPos==10)
			ActiveWnd->fGamma=25.0F;

		s.Format(_T("%.2f"),ActiveWnd->fGamma);
		Gval.SetWindowText(s);

		ActiveWnd->nGamma = nGammaPos;

	}




	b=LevelSlider.GetPos();
	if (nLvlPos!=b)
	{	
		nLvlPos=b;
		s.Format(_T("%d"),nLvlPos);
		Lval.SetWindowText(s);

		ActiveWnd->nLevel=nLvlPos;		
	}


	b=EdgeSlider.GetPos();
	if (nEdgePos!=b)
	{	
		nEdgePos=b;
		s.Format(_T("%d"),nEdgePos);
		Eval.SetWindowText(s);

		ActiveWnd->nEdge=nEdgePos;
	}

	b=SharpSlider.GetPos();
	if (nSharpPos!=b)
	{	
		nSharpPos=b;
		s.Format(_T("%d"),nSharpPos);
		Shval.SetWindowText(s);

		ActiveWnd->nSharp=nSharpPos;		
	}

	b=SmoothSlider.GetPos();
	if (nSmoothPos!=b)
	{	
		nSmoothPos=b;
		s.Format(_T("%d"),nSmoothPos);
		Smval.SetWindowText(s);

		ActiveWnd->nSmooth=nSmoothPos;		
	}

	b=DenoiseSlider.GetPos();
	if (nFiltPos!=b)
	{	
		nFiltPos=b;
		s.Format(_T("%d"),nFiltPos);
		Fval.SetWindowText(s);

		ActiveWnd->nFilter=nFiltPos;		
	}


	//Go to OnDraw Funtion to apply changes to the image...
	ActiveWnd->Invalidate();

	
}



void CProcessTool::OnBnClickedOriginalIm()
{
	// TODO: Add your control notification handler code here
	ActiveWnd->OnEditOriginalimage();
	return;
}



void CProcessTool::SetOriginalIm()
{
	CString s;
	//char str[40];	//if we want to use string text instead of CEdit box
	
	nGammaPos= 5;
	s.Format(_T("%.2f"), 1.0F);
	Gval.SetWindowText(s);

	nWndwPos=ActiveWnd->nWindowOr;
	s.Format(_T("%d"),nWndwPos);
	Wval.SetWindowText(s);
	//ActiveWnd->nWindow=nWndwPos;
	
	nLvlPos=ActiveWnd->nLevelOr;
	s.Format(_T("%d"),nLvlPos);
	Lval.SetWindowText(s);
	//ActiveWnd->nLevel=nLvlPos;		
	
	nEdgePos=0;
	s.Format(_T("%d"),nEdgePos);
	Eval.SetWindowText(s);
	ActiveWnd->nEdge=nEdgePos;

	nSharpPos=0;
	s.Format(_T("%d"),nSharpPos);
	Shval.SetWindowText(s);
	ActiveWnd->nSharp=nSharpPos;		

	nSmoothPos=0;
	s.Format(_T("%d"),nSmoothPos);
	Smval.SetWindowText(s);
	ActiveWnd->nSmooth=nSmoothPos;		

	nFiltPos=0;
	s.Format(_T("%d"),nFiltPos);
	Fval.SetWindowText(s);
	ActiveWnd->nFilter=nFiltPos;			

	GammaSlider.SetPos(nGammaPos);
	WindowSlider.SetPos(nWndwPos);
	LevelSlider.SetPos(nLvlPos);

	EdgeSlider.SetPos(nEdgePos);
	SmoothSlider.SetPos(nSmoothPos);
	SharpSlider.SetPos(nSharpPos);
	DenoiseSlider.SetPos(nFiltPos);
	
	pInvertCntrst.SetCheck(ActiveWnd->InvertPixels);

	return;
}


// Check Box Control -------------------------------------------------------------------------------------------


void CProcessTool::OnBnClickedInvertCont()
{
	// TODO: Add your control notification handler code here
	

		//If this Image was created by this application...	
		if (ActiveWnd->nAppCreated>=1 || ActiveWnd->nImgFmat==0)	//Permit Invert Pixels
		{

			if(pInvertCntrst.GetCheck()) 		
				ActiveWnd->InvertPixels=TRUE;
			else						  
				ActiveWnd->InvertPixels=FALSE;

		}
		else
		{

			////AfxMessageBox ("Wrong button clicked!",MB_ICONQUESTION|MB_OK);

			if(pInvertCntrst.GetCheck()) 		
			{
				CWnd msg;
				int nResult = msg.MessageBox("We want to protect the original data. \nYou may want to save this image first with a different name.\n\nDo you want to use this function now?","Wait...",MB_YESNO|MB_ICONWARNING);
				if(nResult == IDNO)
				{
					ActiveWnd->InvertPixels=FALSE;
					pInvertCntrst.SetCheck(FALSE);
					return;
				}
				else
					ActiveWnd->InvertPixels=TRUE;
			}
			else
				ActiveWnd->InvertPixels=FALSE;
		}


		//Go to OnDraw Funtion to apply changes to the image...
		ActiveWnd->Invalidate();
	return;
}
