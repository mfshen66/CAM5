#if !defined(AFX_ELLDLG_H__37FA5CCA_DE88_45BB_96E3_1C71296E6FAB__INCLUDED_)
#define AFX_ELLDLG_H__37FA5CCA_DE88_45BB_96E3_1C71296E6FAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EllDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEllDlg dialog

class CEllDlg : public CDialogEx
{
// Construction
public:
	CEllDlg(CWnd* pParent = NULL);   // standard constructor
	CEllDlg(double r1, double r2) ; // nt add 2010/8/29

// Dialog Data
	//{{AFX_DATA(CEllDlg)
	enum { IDD = IDD_ELLDLG };
	double	m_r1;
	double	m_r2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CEllDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELLDLG_H__37FA5CCA_DE88_45BB_96E3_1C71296E6FAB__INCLUDED_)
