#if !defined(AFX_CRAADLG_H__888D8E5B_DE1D_44DF_9E88_6F829F517369__INCLUDED_)
#define AFX_CRAADLG_H__888D8E5B_DE1D_44DF_9E88_6F829F517369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CRAADlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RAADlg dialog

class CRAADlg : public CDialogEx
{
// Construction
public:
	CRAADlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRAADlg)
	enum { IDD = IDD_RAADLG };
	double	m_r;
	double	m_a1;
	double	m_a2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRAADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(RAADlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRAADLG_H__888D8E5B_DE1D_44DF_9E88_6F829F517369__INCLUDED_)
