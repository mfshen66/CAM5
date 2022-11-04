#if !defined(AFX_ROTDLG_H__AA743F43_4BD3_498A_88C2_260E58A80E6B__INCLUDED_)
#define AFX_ROTDLG_H__AA743F43_4BD3_498A_88C2_260E58A80E6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotDlg dialog

class CRotDlg : public CDialogEx
{
// Construction
public:
	CRotDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotDlg)
	enum { IDD = IDD_ROTDLG };
	double m_angle ;
	BOOL m_copy ;
	BOOL m_local ;
	int m_axis ; // 0(X),1(Y),2(Z), nt add 2011/6/12
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void UpdateRBs() ; // nt add 2011/6/12
	// Generated message map functions
	//{{AFX_MSG(CRotDlg)
	virtual BOOL OnInitDialog() ;
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnApply() ;
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTDLG_H__AA743F43_4BD3_498A_88C2_260E58A80E6B__INCLUDED_)
