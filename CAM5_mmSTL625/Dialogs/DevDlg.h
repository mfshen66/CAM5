#if !defined(AFX_DEVDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_)
#define AFX_DEVDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevDlg dialog

class CDevDlg : public CDialogEx
{
// Construction
public:
	CStringArray m_strs ; // array of CString will be showed in the list box. nt add 2001.12.1
	CDevDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL Flush() ; // 把m_strs中的内容显示出来
// Dialog Data
	//{{AFX_DATA(CDevDlg)
	enum { IDD = IDD_DEVDLG };
	BOOL m_line;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDevDlg)
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVDLG_H__C62C11B2_9CC8_40FF_A47D_C5D37BC67168__INCLUDED_)
