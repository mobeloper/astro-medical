#pragma once
#include "afxwin.h"

///////////////////////////////////////////
//CImageInfo dialog
///////////////////////////////////////////


class CImageInfo : public CDialog
{
	DECLARE_DYNAMIC(CImageInfo)

public:
	CImageInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageInfo();

// Dialog Data
	enum { IDD = IDD_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox wndTypes;
	CString sFormatType;
	CButton pCheckEndian;
	CEdit sWidth;
	CEdit sHeight;
	int intWidth;
	int intHeight;

	int intType;
	int nBigEndian;
	
	BOOL OnInitDialog();
	
	afx_msg void OnBnClickedOk();

	
};
