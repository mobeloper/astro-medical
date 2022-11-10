

#pragma once

#include "afxcmn.h"
#include "afxwin.h"



// CProcessTool dialog


class CProcessTool : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessTool)

public:	
	CProcessTool(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessTool();


// Dialog Data
	enum { IDD = IDD_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	CWnd* m_pParent;	
	BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	int nWndDlgStat;

	int nGammaPos;
	int nWndwPos;
	int nLvlPos;
	
	int nEdgePos;
	int nSharpPos;
	int nSmoothPos;
	int nFiltPos;

	CSliderCtrl GammaSlider;
	CSliderCtrl WindowSlider;
	CSliderCtrl LevelSlider;
	CSliderCtrl EdgeSlider;
	CSliderCtrl SharpSlider;
	CSliderCtrl SmoothSlider;
	CSliderCtrl DenoiseSlider;
	CEdit Gval;
	CEdit Wval;
	CEdit Lval;
	CEdit Eval;
	CEdit Shval;
	CEdit Smval;
	CEdit Fval;

	CButton pInvertCntrst;


	BOOL DisplayCurrValues();
	void SetOriginalIm();

	afx_msg void OnBnClickedOriginalIm();

	
	afx_msg void OnBnClickedInvertCont();
	
	
};
