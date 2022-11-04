#if !defined(AFX_CHKDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_)
#define AFX_CHKDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChkDlg dialog

class CChkDlg : public CDialogEx
{
// Construction
public:
	CStringArray m_strs ; // array of CString will be showed in the list box. nt add 2001.12.1
	CChkDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL Flush() ; // 把m_strs中的内容显示出来
// Dialog Data
	//{{AFX_DATA(CChkDlg)
	enum { IDD = IDD_CHECK };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChkDlg)
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHKDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_)
