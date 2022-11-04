#if !defined(AFX_RHDLG_H__4B0D1DB6_E2EF_4D21_B4A3_5110E67D4C5B__INCLUDED_)
#define AFX_RHDLG_H__4B0D1DB6_E2EF_4D21_B4A3_5110E67D4C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RHDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRHDlg dialog

class CRHDlg : public CDialogEx
{
// Construction
public:
	CRHDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRHDlg)
	enum { IDD = IDD_RHDLG };
	double	m_r;
	double	m_h;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRHDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RHDLG_H__4B0D1DB6_E2EF_4D21_B4A3_5110E67D4C5B__INCLUDED_)
