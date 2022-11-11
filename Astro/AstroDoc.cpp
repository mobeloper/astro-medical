
// AstroDoc.cpp : implementation of the CAstroDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Astro.h"
#endif


#include "AstroDoc.h"
#include "CDib.h"			//ADDED BY ERIC
#include "ImageInfo.h"


#include <propkey.h>


CString strSection       = _T("Defaults");
CString strStringItem    = _T("Dir");
CString strIntItem       = _T("Format");


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAstroDoc

IMPLEMENT_DYNCREATE(CAstroDoc, CDocument)

BEGIN_MESSAGE_MAP(CAstroDoc, CDocument)
	ON_COMMAND(ID_COLORTYPE_BLUE, &CAstroDoc::OnColortypeBlue)
	ON_COMMAND(ID_COLORTYPE_GRAY, &CAstroDoc::OnColortypeGray)
END_MESSAGE_MAP()



// CAstroDoc construction/destruction

CAstroDoc::CAstroDoc()
{
	// TODO: add one-time construction code here
	
}

CAstroDoc::~CAstroDoc()
{
	if(do_pDib!=NULL)
		do_pDib->DeleteObject();
}

BOOL CAstroDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	
	//do_pDib =new CDib();
	do_pDib=NULL;

	return TRUE;
}




// CAstroDoc serialization

void CAstroDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here

		CString file;
		file=ar.GetFile()->GetFileName();
		file.MakeLower();
		m_DicomFlag=file.Right(4)==".dcm";
		

		if (m_DicomFlag)
		{

		}
		else
		{	

		}


	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CAstroDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CAstroDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CAstroDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CAstroDoc diagnostics

#ifdef _DEBUG
void CAstroDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAstroDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAstroDoc commands




/////////////////////////////////////////////////////////////////////////////

CDib* CAstroDoc::GetDib()
{
	return do_pDib;
}


BOOL CAstroDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here


	strDocNameOr = (CString)lpszPathName;

		char faux[512];	
		lstrcpy(faux, lpszPathName);
		char *pfile = faux;
	
		int filNamSize=0;
		while(*pfile++!='\0')	
			filNamSize++;


		pfile = faux;
		pfile+=filNamSize;

		int idx=filNamSize;
		for(idx; idx>0; idx--)
		{
			if(*pfile--==0x5C)	//if is equal to "\"
				break;
		}

		//Save File Name and Directory Name
		pfile = faux;
		pfile+=idx+1;

		
		char cFileNme[250];
		char *pFnme=cFileNme;
		int naux=(filNamSize-idx);

		memcpy(pFnme,pfile,naux);			
		cFileNme[naux-1]='\0';

		//Decide what kind of open we will use...
		pfile = faux;
		pfile+=filNamSize-4;
	
		DWORD *dwType=(DWORD *)pfile;

		int tmpCheck;

		do_pDib =new CDib();


		if(*dwType=='mcd.')			// .dcm file
		{
			
			tmpCheck = do_pDib->LoadDCMfile(lpszPathName);


			if(tmpCheck==0)
			{
				AfxMessageBox(_T("Not compatible DICOM file!"));
				do_pDib = NULL;
				return FALSE;
			}

		}
		else if (*dwType=='war.')		// .raw file
		{
			
			CImageInfo pImageSettings;
			
			if( pImageSettings.DoModal() == IDOK )
			{

				tmpCheck = do_pDib->LoadRAWfile(lpszPathName,pImageSettings.intType,pImageSettings.intWidth,pImageSettings.intHeight,pImageSettings.nBigEndian);

				
				if(tmpCheck==0)
				{
					AfxMessageBox(_T("File parameters mismatch!"));
					do_pDib = NULL;
					return FALSE;
				}
				
			}
			else
			{
				return FALSE;
			}

		}
		else 
		{
			AfxMessageBox(_T("Not supported file!"));	
			do_pDib = NULL;
			return FALSE;
		}


	UpdateAllViews(NULL);
	return TRUE;

}

BOOL CAstroDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	//if (!CDocument::OnSaveDocument(lpszPathName))
	//	return FALSE;
	
	// TODO: Add your specialized creation code here

	if(do_pDib==NULL) 
		return FALSE;


		char faux[512];	
		lstrcpy(faux, lpszPathName);
		char *pfile = faux;
	
		int filNamSize=0;
		while(*pfile++!='\0')	
			filNamSize++;

		//Decide what kind of open we will use...
		pfile = faux;
		pfile+=filNamSize-4;
	
		DWORD *dwType=(DWORD *)pfile;

		int tmpCheck;

		if(*dwType=='mcd.')			// .dcm file
		{					
			tmpCheck = do_pDib->SaveDCMfile(lpszPathName);

			if(tmpCheck==0)
			{
				AfxMessageBox(_T("DICOM file was not saved!"));				
				return FALSE;
			}
		}
		else if (*dwType=='war.')		// .raw file
		{
			tmpCheck = do_pDib->SaveRAWfile(lpszPathName);
				
			if(tmpCheck==0)
			{
				AfxMessageBox(_T("RAW file was not saved!"));
				return FALSE;
			}
		}
		else 
		{
			AfxMessageBox(_T("Not supported file!"));
			return FALSE;
		}

	//SetTitle(strDocNameOr);

	UpdateAllViews(NULL);
	return TRUE;

}



void CAstroDoc::OnRecovery()
{
	// TODO: Add your command handler code here

	if(do_pDib==NULL) return;

	do_pDib->Recovery();
	UpdateAllViews(NULL);
}



void CAstroDoc::OnColortypeBlue()
{
	// TODO: Add your command handler code here

	if(do_pDib==NULL) return;


	//Blue Iron Color Palette
	for(int i=0;i<256;i++)
	{
		if(i<=127) 
			do_pDib->m_pPalette[i].rgbBlue=byte(i*2);
		else
			do_pDib->m_pPalette[i].rgbBlue=byte(255);

		if(i>128) 
			do_pDib->m_pPalette[i].rgbGreen=byte((i-128)*2);
		else
			do_pDib->m_pPalette[i].rgbGreen=byte(0);

		if(i>192) 
			do_pDib->m_pPalette[i].rgbRed=byte((i-192)*4);
		else
			do_pDib->m_pPalette[i].rgbRed=byte(0);
	}


	//do_pDib->FakeColor(10,20,120);

	UpdateAllViews(NULL);


	//CFakeColor fake;

	//if(fake.DoModal()==IDOK)
	//{
	//	do_pDib->FakeColor(fake.m_Blue,fake.m_Green,fake.m_Red);
	//	UpdateAllViews(NULL);
	//}
}


void CAstroDoc::OnColortypeGray()
{
	// TODO: Add your command handler code here

	if(do_pDib==NULL) return;

	////do_pDib->Recovery();

	for(int i=0;i<256;i++)
	{
		do_pDib->m_pPalette[i].rgbBlue=(byte)i;
		do_pDib->m_pPalette[i].rgbGreen=(byte)i;
		do_pDib->m_pPalette[i].rgbRed=(byte)i;
		do_pDib->m_pPalette[i].rgbReserved=0;
	}
		
	UpdateAllViews(NULL);

}
