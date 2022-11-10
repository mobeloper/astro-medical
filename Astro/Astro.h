
// Astro.h : main header file for the Astro application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols



class CAstroView;		
class CProcessTool;		
#include "ProcessTool.h"


// CAstroApp:
// See Astro.cpp for the implementation of this class
//

class CAstroApp : public CWinAppEx
{
public:
	CAstroApp();

	CDocument *m_UploadDoc;



// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();



	void DestroyModeless();
	void CreateModeless();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();

	void OnUpdateUpload(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnProcessImage();
	afx_msg void OnFileOpen();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CAstroApp theApp;
