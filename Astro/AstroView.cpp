
// AstroView.cpp : implementation of the CAstroView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Astro.h"
#endif


//	ADDED BY ERIC

#include "AstroDoc.h"		//This order of the "includes" is extremelly important
#include "AstroView.h"		
#include "CDib.h"

#include "DicomInfo.h"

#include "MainFrm.h"
#include "afxwin.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAstroView
//IMPLEMENT_DYNCREATE(CAstroView, CScrollView)
//BEGIN_MESSAGE_MAP(CAstroView, CScrollView)
IMPLEMENT_DYNCREATE(CAstroView, CView)
BEGIN_MESSAGE_MAP(CAstroView, CView)

	 
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	// Standard printing commands
	//ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAstroView::OnFilePrintPreview)

	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_PROCESS_IMAGE, &CAstroView::OnProcessImage)	//cambiada a otra clase
	
	ON_WM_ACTIVATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	ON_COMMAND(ID_EDIT_ORIGINALIMAGE, &CAstroView::OnEditOriginalimage)
	ON_COMMAND(ID_EDIT_IMAGEINFO, &CAstroView::OnEditImageinfo)

END_MESSAGE_MAP()




//Call external variables
extern CAstroView *ActiveWnd;
extern CProcessTool pProcessDlg;



// CAstroView construction/destruction

CAstroView::CAstroView()
{
	// TODO: add construction code here
				
	point1.x=0;
	point1.y=0;
	deltaX=0;
	deltaY=0;
	
	fZoomRate = 1;

	nEdge_temp = nEdge=0;
	nSmooth_temp = nSmooth=0;
	nSharp_temp = nSharp=0;
	nFilter_temp = nFilter=0;

	ActiveWnd = this;
	//nProcDlgActive = 0;
	
	hasBackground = false; 	// redraw background
	//PersistenceMode = false;
	//CursorPosition = -1;	// don't plot if not set
	
	isLDown = false;
	isRDown = false;
	isMove = false;
}

CAstroView::~CAstroView()
{
	/* Free all the used variables
	if (m_Raw) free(m_Raw);
	if (m_RawBits) free(m_RawBits);
	if (m_BitMapInfo) free(m_BitMapInfo);
	*/
}

BOOL CAstroView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//return CScrollView::PreCreateWindow(cs);
	return CView::PreCreateWindow(cs);
}

// CAstroView drawing



//void CAstroView::OnDraw(CDC* /*pDC*/)
void CAstroView::OnDraw(CDC* pDC)
{

	CAstroDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	
	//m_pDib = new CDib();

	CDib *pDib = pDoc->GetDib();
	
	if(pDib!=NULL)
	{
		
		//Keep the Original document PathName after saving the image
		if(pDib->nImageSaved == 1)
		{
			pDib->nImageSaved = 0;
			//pDoc->SetTitle(pDoc->strDocNameOr);	//Change the Child Frame Title
			pDoc->SetPathName(pDoc->strDocNameOr);	//Keep the original document path
		}

		
		//Invert Pixel Data values
		if(nInvert_temp != InvertPixels)
		{
			nInvert_temp = InvertPixels;
			//pDib->ImageTransformation(4,InvertPixels);

			if(InvertPixels!=(pDib->nInvPixelsOr))
			{
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);	//Invert Pixels
				//nWindow = (int)(pDib->nMaxHist - pDib->nMinHist);
				nLevel = (int)((pDib->nMaxHist + pDib->nMinHist)*0.5F);
			}
			else
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);	//Do not Invert Pixels

			//Change the Window and Level Limits...
			nMaxWindow = (int)(1.1F*(pDib->nMaxHist - pDib->nMinHist));
			nMinWindow = 0;

			nMaxLevel = (int)(1.08F*pDib->nMaxHist);		//Maximum value of the histogram plus 8%
			nMinLevel = (int)(pDib->nMinHist - 0.1F*pDib->nMinHist);	//Minimum value of the histogram minus 10%
			
			if(nLevel!=nLevelOr)
				nLevel = (int)((pDib->nMaxHist + pDib->nMinHist)*0.5F);
				

			//Display the current values in the Dialog
			ActiveWnd = this;
			if(pProcessDlg.GetSafeHwnd()!=NULL)
			{			
				pProcessDlg.DisplayCurrValues();			
				pProcessDlg.UpdateData(true);			
			}

			pDib->CreateScreenBits();
				
		}


		//Gamma Correction
		if(nGamma_temp != nGamma)
		{
			nGamma_temp = nGamma;

			//This following conditions are need to avoid false intructions in invert pixel values
			if((pDib->nInvPixelsOr)==1 && InvertPixels==0)
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else if((pDib->nInvPixelsOr)==1 && InvertPixels==1)
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else
				pDib->ImageTransformation(InvertPixels,nEdge,nSharp,nSmooth,nFilter,nGamma);

			//Change the Window and Level Limits...
			nMaxWindow = (int)(1.1F*(pDib->nMaxHist - pDib->nMinHist));
			nMinWindow = 0;


			nMaxLevel = (int)(1.08F*pDib->nMaxHist);		//Maximum value of the histogram plus 8%
			nMinLevel = (int)(pDib->nMinHist - 0.1F*pDib->nMinHist);	//Minimum value of the histogram minus 10%

			//Set the window and level		
			if(nGamma==5)
			{
				nWindow = nWindowOr;
				nLevel = nLevelOr;
			}
			else
			{
				nWindow = (int)(pDib->nMaxHist - pDib->nMinHist);
				nLevel = (int)((pDib->nMaxHist + pDib->nMinHist)*0.5F);
			}
			
			//Display the current values in the Dialog
			ActiveWnd = this;
			if(pProcessDlg.GetSafeHwnd()!=NULL)
			{			
				pProcessDlg.DisplayCurrValues();			
				pProcessDlg.UpdateData(true);			
			}

			pDib->CreateScreenBits();
		}


		if(nEdge_temp!= nEdge)
		{
			nEdge_temp = nEdge;

			//This following conditions are need to avoid false intructions in invert pixel values
			if((pDib->nInvPixelsOr)==1 && InvertPixels==0)
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else if((pDib->nInvPixelsOr)==1 && InvertPixels==1)
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else
				pDib->ImageTransformation(InvertPixels,nEdge,nSharp,nSmooth,nFilter,nGamma);


			pDib->CreateScreenBits();
		}

		if(nSmooth_temp!= nSmooth)
		{
			nSmooth_temp = nSmooth;

			//This following conditions are need to avoid false intructions in invert pixel values
			if((pDib->nInvPixelsOr)==1 && InvertPixels==0)
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else if((pDib->nInvPixelsOr)==1 && InvertPixels==1)
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else
				pDib->ImageTransformation(InvertPixels,nEdge,nSharp,nSmooth,nFilter,nGamma);

			pDib->CreateScreenBits();
		}

		if(nSharp_temp!= nSharp)
		{
			nSharp_temp = nSharp;

			//This following conditions are need to avoid false intructions in invert pixel values
			if((pDib->nInvPixelsOr)==1 && InvertPixels==0)
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else if((pDib->nInvPixelsOr)==1 && InvertPixels==1)
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else
				pDib->ImageTransformation(InvertPixels,nEdge,nSharp,nSmooth,nFilter,nGamma);

			pDib->CreateScreenBits();
		}

		if(nFilter_temp!= nFilter)	
		{
			nFilter_temp = nFilter;

			//This following conditions are need to avoid false intructions in invert pixel values
			if((pDib->nInvPixelsOr)==1 && InvertPixels==0)
				pDib->ImageTransformation(1,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else if((pDib->nInvPixelsOr)==1 && InvertPixels==1)
				pDib->ImageTransformation(0,nEdge,nSharp,nSmooth,nFilter,nGamma);
			else
				pDib->ImageTransformation(InvertPixels,nEdge,nSharp,nSmooth,nFilter,nGamma);

			pDib->CreateScreenBits();
		}



		// If the value was changed in the dialog
		//if(nWindow_temp != nWindow || nLevel_temp != nLevel ||nInvert_temp != InvertPixels)
		if(nWindow_temp != nWindow || nLevel_temp != nLevel)
		{
			nWindow_temp = nWindow;
			nLevel_temp = nLevel;
			//nInvert_temp = InvertPixels;
			
			//pDib->CreateLUT(nWindow,nLevel,InvertPixels);
			pDib->CreateLUT(nWindow,nLevel);


			pDib->CreateScreenBits();

			//Display the current values in the Dialog
			ActiveWnd = this;
			if(pProcessDlg.GetSafeHwnd()!=NULL)
			{			
				pProcessDlg.DisplayCurrValues();			
				pProcessDlg.UpdateData(true);			
			}


		}


		
		CRect client;		
		GetClientRect(client);

		CRect rect = client;
		
		
		//Get the display window size
		nWndWidth = rect.Width();
		nWnHeight = rect.Height();


		if(fZoomfit==-1)
		{
			int tpImsz = max(nImWidth,nImHeight);
			int tpWdsz = min(nWndWidth,nWnHeight);

			//Scale the image to fit the window
			fZoomfit = float((tpWdsz+1.0F)/(tpImsz+1.0F)*0.95);	// 90% of the window size			
			fZoomRate = fZoomfit;



		}



		///////////////////////////////////////////////
		//		Panning Controls
		///////////////////////////////////////////////


		//Maximum posible offset
		int MofstX = (int)(rect.Width() - nImWidth*fZoomRate);
		int MofstY = (int)(rect.Height() - nImHeight*fZoomRate);
	

		//If the image is bigger than the child window...
		if(MofstX<0) 
		{			
			ofstX = deltaX;

			//Make sure we don't go out of bounds
			if(ofstX > 0)
				ofstX = 0;
			if(ofstX < MofstX)
				ofstX = MofstX;
		}
		else	//If the image is smaller than the child window put it in the center
		{
			ofstX = (int)(0.5F*MofstX);	//Put the image in the middle

		}

		if(MofstY<0) 
		{			
			ofstY = deltaY;

			//Make sure we don't go out of bounds
			if(ofstY > 0)
				ofstY = 0;
			if(ofstY < MofstY)
				ofstY = MofstY;
		}
		else	//If the image is smaller than the child window put it in the center
		{
			ofstY = (int)(0.5F*MofstY);		//Put the image in the middle

		}





		///////////////////////////////////////////////
		//Double Buffering Image Display
		///////////////////////////////////////////////
		
		//To avoid flicking effect

		//	http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/0af87f37-d97d-4564-9d89-909f4457d570/
		//	http://msdn.microsoft.com/en-us/library/9t94971d(v=VS.80).aspx


		//Create the size of the Buffer to display image
		int auxWidth;
		int auxHeight;

		if(rect.Width()>nImWidth*fZoomRate) 
			auxWidth = rect.Width();
		else auxWidth = int(nImWidth*fZoomRate);

		if(rect.Height()>nImHeight*fZoomRate) 
			auxHeight = rect.Height();
		else auxHeight = int(nImHeight*fZoomRate);


		/// Use Double Buffering technique to avoid Flickering effect

		CDC dcBackBuffer;
		CBitmap bmpBuffer;
		CBitmap* pOldBmpBuffer = 0;

		dcBackBuffer.CreateCompatibleDC(pDC);
		bmpBuffer.CreateCompatibleBitmap(pDC, auxWidth, auxHeight);

		pOldBmpBuffer = dcBackBuffer.SelectObject(&bmpBuffer);

		// paint background
		CBrush brush;
		if (!brush.CreateSolidBrush(RGB(0, 0, 0)))
			return;

		dcBackBuffer.FillRect(rect, &brush);
		
		//Display the image	and information
		pDib->Draw(&dcBackBuffer,ofstX,ofstY,nImWidth,nImHeight,fZoomRate);

		pDC->BitBlt(0, 0, auxWidth,auxHeight, &dcBackBuffer, 0, 0, SRCCOPY);

		dcBackBuffer.SelectObject(pOldBmpBuffer);
		dcBackBuffer.DeleteDC();
		bmpBuffer.DeleteObject();
		

		//Foot Text of the Image:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);			// TRANSPARENT OPAQUE BKMODE_LAST


		if(nImgFmat==1) 	//If the image is DICOM		
		{	
			CString str;

			pDC->TextOut(1,nWnHeight-50,(CString)"Body Part:");
			if(pDib->DataElmntBPart[0].VF[0]== -51 || pDib->DataElmntBPart[0].VF[0] == '\0')
				pDC->TextOut(TXT_COL+T_INCREMENT,nWnHeight-50,(CString)"Not available");
			else
				pDC->TextOut(TXT_COL+T_INCREMENT,nWnHeight-50,(CString)pDib->DataElmntBPart[0].VF);
			

			pDC->TextOut(1,nWnHeight-35,(CString)"Study DateTime:");
			if(pDib->DataElmntStudyD[0].VF[0]== -51 || pDib->DataElmntStudyD[0].VF[0] == '\0')
				pDC->TextOut(TXT_COL+T_INCREMENT,nWnHeight-35,(CString)"00/00/0000    00:00:00");
			else
			{
				//pDC->TextOut(TXT_COL+T_INCREMENT,auxPos-35,(CString)DataElmntStudyD[0].VF);
				str.Format("%s%s/%s%s/%s%s%s%s",(CString)pDib->DataElmntStudyD[0].VF[4],
					(CString)pDib->DataElmntStudyD[0].VF[5],(CString)pDib->DataElmntStudyD[0].VF[6],
					(CString)pDib->DataElmntStudyD[0].VF[7],(CString)pDib->DataElmntStudyD[0].VF[0],
					(CString)pDib->DataElmntStudyD[0].VF[1],(CString)pDib->DataElmntStudyD[0].VF[2],
					(CString)pDib->DataElmntStudyD[0].VF[3]);					
				pDC->TextOut(TXT_COL+T_INCREMENT,nWnHeight-35,str);
			
				str.Format("%s%s:%s%s:%s%s",(CString)pDib->DataElmntStudyT[0].VF[0],
					(CString)pDib->DataElmntStudyT[0].VF[1],(CString)pDib->DataElmntStudyT[0].VF[2],
					(CString)pDib->DataElmntStudyT[0].VF[3],(CString)pDib->DataElmntStudyT[0].VF[4],
					(CString)pDib->DataElmntStudyT[0].VF[5]);
				pDC->TextOut(TXT_COL+T_INCREMENT+90,nWnHeight-35,str);
			}
		}

		//Cursor position... (Is suggested to better display the pixel position of the image and its correspondig values)
		char strF[200];
		sprintf_s(strF,"x=%d [%d mm]   y=%d [%d mm]   value=%d",(currPnt.x),0,(currPnt.y),0,0);
		pDC->TextOut(5,nWnHeight-20,strF);

	}	

}



void CAstroView::OnInitialUpdate()
{
	//CScrollView::OnInitialUpdate();
	CView::OnInitialUpdate();

	//m_pProcDlg = NULL;	// set pointer to NULL


	CAstroDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib *pDib = pDoc->GetDib();
	
	//CSize sizeTotal;
	//sizeTotal.cx = 3072;
	//sizeTotal.cy = 3072;


	if(pDib!=NULL)
	{

		pDib->nImageSaved = 0;	//Image has not been saved

		nAppCreated = pDib->nAppImage;		//Know if the image was made by this program
		
		InvertPixels = pDib->nInvPixelsOr;	//Know if the pixel data values are inverted

		nNumbTags = pDib->iNumbTags;		//Get the number of DICOM tags
		

		nImgFmat = pDib->nImageFormat;		//Get the image format type: DICOM or RAW
		if(nImgFmat==1)
			pDib->GetDicomTags(charDCMtags);	//Put the DICOM tags into printable characters


		nImWidth = pDib->nWidth;		//Get image width
		nImHeight = pDib->nHeight;		//Get image height

		//Set the initial position for GAMMA CORRECTION
		nGamma = 5;		
		fGamma = 1.0F;

		nWindowOr = pDib->WW;	//Backup the original Window
		nLevelOr = pDib->WC;	//Backup the original Level
		nWindow = nWindowOr;
		nLevel = nLevelOr;	

		//nMaxWindow = pDib->nNumbPixels;
		nMaxWindow = (int)(1.1F*(pDib->nMaxHist - pDib->nMinHist));
		nMinWindow = 0;
		

		nMaxLevel = (int)(1.08F*pDib->nMaxHist);		//Maximum value of the histogram plus 8%
		nMinLevel = (int)(pDib->nMinHist - 0.1F*pDib->nMinHist);	//Minimum value of the histogram minus 10%
		
		nBrightness = 0;

		//pDib->GetDicomTags(charDCMtags);
		

		//sizeTotal.cx = (long)(nImWidth*fZoomRate);
		//sizeTotal.cy = (long)(nImHeight*fZoomRate);
	}

	//SetScrollSizes(MM_TEXT, sizeTotal);
	
	fZoomfit = -1;

	//Backup to later detect any changes in this variables
	nWindow_temp = nWindow;
	nLevel_temp = nLevel;
	
	nInvert_temp = InvertPixels;
	nGamma_temp = nGamma;

	//pProcessDlg.m_View  = this;
	ActiveWnd = this;
	
	//ResizeParentToFit();	//Resize the image to show as maximized
	
}



// CAstroView printing


void CAstroView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CAstroView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAstroView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAstroView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


void CAstroView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}



// CAstroView diagnostics

#ifdef _DEBUG
void CAstroView::AssertValid() const
{
	CView::AssertValid();
}

void CAstroView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAstroDoc* CAstroView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAstroDoc)));
	return (CAstroDoc*)m_pDocument;
}
#endif //_DEBUG





// CAstroView message handlers

void CAstroView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isRDown=true;  
	isMove=false;
	point1=point;

	CView::OnLButtonDown(nFlags, point);
	//CScrollView::OnRButtonDown(nFlags, point);
}


void CAstroView::OnRButtonUp(UINT /*nFlags*/, CPoint point)
{

	isRDown=false;

	if(isMove==0)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}
}


void CAstroView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	isLDown = true;
	isMove = false;
	point1 = point;
	
	prevX = ofstX;
	prevY = ofstY;

	CView::OnLButtonDown(nFlags, point);
	//CScrollView::OnLButtonDown(nFlags, point);
}


void CAstroView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	isLDown=false;

	CView::OnLButtonUp(nFlags, point);
	//CScrollView::OnLButtonUp(nFlags, point);
}


void CAstroView::OnMouseMove(UINT nFlags, CPoint point)
{
	

	// TODO: Add your message handler code here and/or call default
	CAstroDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib *pDib = pDoc->GetDib();

	int dtY, dtX;

	currPnt = point;

	dtX = (point.x-point1.x);
	dtY = (point.y-point1.y);


	//If the Right botton was pressed...
	if(isRDown)
	{
		//Avoid small changes
		if(abs(dtX)<30)	dtX = 0;		
		if(abs(dtY)<30)	dtY = 0;		

		//Make sure that the cursor was moved
		if(dtX==0 && dtY==0)
		{
			isMove = false;
			return;
		}
		else
			isMove = true;


		nWindow += dtY;
		nLevel += dtX;
		
		if(nWindow<0)	nWindow = 0;		//Minimum window level is zero

	}


	//If the Left botton was pressed...
	if(isLDown)
	{

		//Set the origin of the new image
		deltaX = (int)(prevX + dtX);
		deltaY = (int)(prevY + dtY);

	}

	Invalidate();

	CView::OnMouseMove(nFlags, point);
	//CScrollView::OnMouseMove(nFlags, point);
}



BOOL CAstroView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//if((nFlags&MK_CONTROL) != MK_CONTROL)
	////	return CScrollView::OnMouseWheel(nFlags,zDelta,pt);
	//	return CView::OnMouseWheel(nFlags,zDelta,pt);

	ActiveWnd = this;

	int tpImsz = max(nImWidth,nImHeight);
	int tpWdsz = min(nWndWidth,nWnHeight);
	
	fZoomfit = float((tpWdsz+1.0F)/(tpImsz+1.0F)*0.95);	// 90% of the window size
	//float fZoomMax = float((tpWdsz*8+1.0F)/(tpImsz+1.0F));	

	if(zDelta<0)
	{
		if(fZoomRate<0.4F) fZoomRate -= 0.04F;
		else fZoomRate -=0.08F;

	}
	else
	{

		if(fZoomRate<0.4F) fZoomRate += 0.04F;
		else fZoomRate +=0.08F;

		//if(fZoomRate>1.3F) fZoomRate=1.3F;		//	Up Zoom Limit
		if((tpImsz*fZoomRate)>3990) 
			fZoomRate -= 0.08F;		//	Up Zoom Limit
		
		//if(fZoomRate>fZoomMax)
		//	fZoomRate = fZoomMax;

	}

	if(fZoomRate<fZoomfit)			//Zoom to fit the window		
		fZoomRate = fZoomfit;
		

	//RedrawWindow();
	Invalidate();
	
	//return CScrollView::OnMouseWheel(nFlags,zDelta,pt);
	return CView::OnMouseWheel(nFlags,zDelta,pt);
}



void CAstroView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//CScrollView::OnActivate(nState, pWndOther, bMinimized);
	CView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	ActiveWnd = this;

}


int CAstroView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	ActiveWnd = this;

	if(pProcessDlg.GetSafeHwnd()!=NULL)
	{			
		pProcessDlg.DisplayCurrValues();			
		pProcessDlg.UpdateData(true);
		//pProcessDlg.ShowWindow(SW_SHOW);
	}

	//return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}





BOOL CAstroView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CScrollView::OnEraseBkgnd(pDC);

	//return FALSE;
	return TRUE;
}



void CAstroView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SHOW_FULLSCREEN);
	

	//Call the function to Switch between Full screen and normal view
	((CMainFrame*) AfxGetMainWnd())->ToggleView();		


	//CScrollView::OnLButtonDblClk(nFlags, point);
	CView::OnLButtonDblClk(nFlags, point);
}


void CAstroView::OnEditOriginalimage()
{
	// TODO: Add your command handler code here

	CAstroDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CDib *pDib = pDoc->GetDib();
	pDib->Recovery();	//Recover the original Image

	nGamma = 5;
	fGamma = 1.0F;

	nWindow = nWindowOr;
	nLevel = nLevelOr;

	InvertPixels = pDib->nInvPixelsOr;
	
	//Update original limits

	//Display the current values in the Dialog
	if(pProcessDlg.GetSafeHwnd()!=NULL)
	{
		pProcessDlg.SetOriginalIm();
		pProcessDlg.UpdateData(true);
	}

	Invalidate();
}



void CAstroView::OnEditImageinfo()
{
	// TODO: Add your command handler code here

	//AfxMessageBox("DICOM HEADER INFORMATION");

	CDicomInfo m_pDicomTags;

	if( m_pDicomTags.DoModal() == IDOK )
	{

	}

	return;
}






