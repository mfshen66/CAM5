#if !defined(AFX_WaitDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_)
#define AFX_WaitDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog

class CWaitDlg : public CDialogEx
{
// Construction
public:
	CWaitDlg(CWnd* pParent = NULL);   // standard constructor
	CWaitDlg(CString& prompt) ; // nt add 2010/9/15

// Dialog Data
	//{{AFX_DATA(CWaitDlg)
	enum { IDD = IDD_WAITDLG };
	//}}AFX_DATA

public:
	CString m_prompt ; // nt add 2010/9/15

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitDlg)
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WaitDLG_H__18C89580_E0EC_11D5_BDC2_BF300A50FE7D__INCLUDED_)
