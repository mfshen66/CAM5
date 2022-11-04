#if !defined(AFX_VECDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_)
#define AFX_VECDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VecDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVecDlg dialog

class CVecDlg : public CDialogEx
{
// Construction
public:
	CVecDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVecDlg)
	enum { IDD = IDD_VECTOR };
	double	m_x;
	double	m_y;
	double	m_z;
	BOOL m_local ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVecDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_)
