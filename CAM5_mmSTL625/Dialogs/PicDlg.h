#if !defined(AFX_PICDLG_H__99DD6092_2DA8_41C8_80BF_1A873AD875FB__INCLUDED_)
#define AFX_PICDLG_H__99DD6092_2DA8_41C8_80BF_1A873AD875FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPicDlg dialog

class CPicDlg : public CDialogEx
{
// Construction
public:
	CPicDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPicDlg)
	enum { IDD = IDD_PICDLG };
	int		m_min;
	int		m_max;
	int		m_transparent ;
	int		m_tdeg ;
	CString	m_fileName;
	int		m_dpi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPicDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	afx_msg void OnButton1();
	afx_msg void OnRadio1();
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio2();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICDLG_H__99DD6092_2DA8_41C8_80BF_1A873AD875FB__INCLUDED_)
