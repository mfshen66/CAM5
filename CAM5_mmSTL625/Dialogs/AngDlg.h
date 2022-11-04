#if !defined(AFX_ANGDLG_H__8E1CB918_B674_4C9C_82C8_AB2018B97F05__INCLUDED_)
#define AFX_ANGDLG_H__8E1CB918_B674_4C9C_82C8_AB2018B97F05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AngDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAngDlg dialog

class CAngDlg : public CDialogEx
{
// Construction
public:
	CAngDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAngDlg)
	enum { IDD = IDD_ANGLE };
	double	m_ang;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAngDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAngDlg)
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

#endif // !defined(AFX_ANGDLG_H__8E1CB918_B674_4C9C_82C8_AB2018B97F05__INCLUDED_)
