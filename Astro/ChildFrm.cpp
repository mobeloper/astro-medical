
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Astro.h"

#include "ChildFrm.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	
	m_wndSplitter.Create(this,
		2, 2,			// TODO: adjust the number of rows, columns		
		CSize(10, 10),	// TODO: adjust the minimum pane size
		pContext);

	m_wndSplitter.SetScrollStyle(0); //Hide the scrollbars

	return TRUE;

}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;


	//cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
	//	| FWS_ADDTOTITLE | WS_THICKFRAME | ~WS_MAXIMIZEBOX | WS_MAXIMIZE;

	

	//cs.style &= ~WS_BORDER; 	 
	//cs.style &= WS_CAPTION; 
	//cs.style &= WS_TILEDWINDOW;	
	//cs.style &= ~WS_THICKFRAME;
	//cs.style &= ~WS_SIZEBOX;	
	//cs.style &= ~WS_OVERLAPPED;	

	
	//cs.style &= ~WS_SYSMENU;	
	
	cs.style &= ~WS_MINIMIZEBOX;		// Create a child window without the minimize button 	
	//cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}




// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers


void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
  // TODO: Add your message handler code here and/or call default

  // Resets position and maximum child window size for full screen mode
  CSize Span = ((CMainFrame*) AfxGetMainWnd())->rectFull.Size();

 

  //if(m_WhoWantsToKnow == TRUE)
  //{
		lpMMI->ptMaxPosition = ((CMainFrame*) AfxGetMainWnd())->rectFull.TopLeft();
		lpMMI->ptMaxTrackSize = lpMMI->ptMaxSize = CPoint(Span.cx, Span.cy);
  //}
  
  //CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
  CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}
