
// AstroDoc.h : interface of the CAstroDoc class
//


#pragma once


class CDib;	


class CAstroDoc : public CDocument
{
protected: // create from serialization only
	CAstroDoc();
	DECLARE_DYNCREATE(CAstroDoc)

// Attributes
public:
	BOOL m_DicomFlag;	


// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);	
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:

	CDib* GetDib();	
	

	virtual ~CAstroDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()




private:
	CDib* do_pDib;//ADDED BY ERIC


	
public:
	
	afx_msg void OnRecovery();
	afx_msg void OnFakeColor();

	afx_msg void OnColortypeBlue();
	afx_msg void OnColortypeGray();


	CString strDocNameOr;



#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


};
