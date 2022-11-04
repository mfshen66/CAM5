#if !defined(AFX_LENDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_)
#define AFX_LENDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLenDlg dialog

class CLenDlg : public CDialogEx
{
// Construction
public:
	CLenDlg(CWnd* pParent = NULL);   // standard constructor
	CLenDlg(CString& name, double r) ; // nt add 2010/8/29

// Dialog Data
	//{{AFX_DATA(CLenDlg)
	enum { IDD = IDD_LENGTH };
	double	m_len;
	//}}AFX_DATA

public:
	CString m_name ; // nt add 2010/8/29

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLenDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LENDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_)
