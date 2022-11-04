#if !defined(AFX_CUREXTDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_)
#define AFX_CUREXTDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurExtDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurExtDlg dialog

class CCurExtDlg : public CDialogEx
{
// Construction
public:
	CCurExtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurExtDlg)
	enum { IDD = IDD_CUREXTDLG };
	double	m_s;
	double  m_r;
	int m_mode ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurExtDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurExtDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
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

#endif // !defined(AFX_CUREXTDLG_H__CDA2A244_EA89_4FDF_A0B4_B9BB891DC9C3__INCLUDED_)
