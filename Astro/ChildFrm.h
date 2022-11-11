
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once


//class CMainFrame;


class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

	//CMainFrame mFrame;

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// This makes the MDI child maximized.
virtual void ActivateFrame(int nCmdShow)
{ 
	// if another window is open, use default 
	if(GetMDIFrame()->MDIGetActive()) 
		CMDIChildWnd::ActivateFrame(nCmdShow); 
	else // else open maximized. 
		CMDIChildWnd::ActivateFrame(SW_SHOWMAXIMIZED); 
} 


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
};
