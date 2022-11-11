
// AstroView.h : interface of the CAstroView class
//

#pragma once


#define NUM_TAGS 240



#include "..\\Astro\\Dicom.h"
class CView;


//class CAstroView : public CScrollView
class CAstroView : public CView
{
protected:				// create from serialization only
	CAstroView();
	DECLARE_DYNCREATE(CAstroView)

// Attributes
public:
	CAstroDoc* GetDocument() const;

// Operations
public:
	//void DocToClient(CRect& rect);	//For fast display the image

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:

	BOOL isRDown;		
	BOOL isLDown;		
	BOOL isMove;		
	
	CPoint currPnt;		
	CPoint point1;		
	

	virtual ~CAstroView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnProcessImage();

	float fZoomfit;
	float fZoomRate;

	//int flgWriteFootText;

	int nAppCreated;

	int nGamma_temp;
	float fGamma;
	int nGamma;

	int InvertPixels;
	int nInvert_temp;
	int nWndWidth;
	int	nWnHeight;

	
	int nImWidth;
	int nImHeight;

	int nMaxWindow;
	int nMinWindow;
	int nMaxLevel;
	int nMinLevel;

	int nBrightness;

	int nWindow;
	int nLevel;
	int nWindowOr;
	int nLevelOr;	
	int nWindow_temp;
	int nLevel_temp;
	

	int nFilter;
	int nEdge;
	int nSmooth;
	int nSharp;
	int nFilter_temp;
	int nEdge_temp;
	int nSmooth_temp;
	int nSharp_temp;

	int deltaX;
	int deltaY;

	int ofstX;
	int ofstY; 
	int prevX;
	int prevY; 

	int pixelX;
	int pixelY;

	bool hasBackground;

	
	CHAR_DICOM_ELEMENT charDCMtags[NUM_TAGS];		//Dicom Tags Structure

	int nNumbTags;
	int nImgFmat;
	

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEditOriginalimage();
	afx_msg void OnEditImageinfo();
};

#ifndef _DEBUG  // debug version in AstroView.cpp
inline CAstroDoc* CAstroView::GetDocument() const
   { return reinterpret_cast<CAstroDoc*>(m_pDocument); }
#endif

