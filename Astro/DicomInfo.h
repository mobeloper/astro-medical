#pragma once
#include "afxcmn.h"


// CDicomInfo dialog

class CDicomInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDicomInfo)

public:
	CDicomInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDicomInfo();

// Dialog Data
	enum { IDD = IDD_DCM_INFO };

protected:

	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_imagelist;
 	CFont m_font;

public:

	virtual BOOL OnInitDialog();
	CListCtrl m_DcmList;

};
