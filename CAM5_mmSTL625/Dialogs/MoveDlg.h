#if !defined(AFX_MOVEDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_)
#define AFX_MOVEDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoveDlg dialog

class CMoveDlg : public CDialogEx
{
// Construction
public:
	CMoveDlg(CCAMDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	BOOL m_local ;
	CCAMDoc* m_pDoc ; // nt add 2011/6/24
// Dialog Data
	//{{AFX_DATA(CMoveDlg)
	enum { IDD = IDD_MOVEDLG };
	double	m_x;
	double	m_y;
	double	m_z;
	BOOL m_copy;
	//}}AFX_DATA
	double m_begin[3] ; // nt add 2011/6/24
	double m_end[3] ; // nt add 2011/6/24

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CMoveDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickedMfcButton1();
	afx_msg void OnClickedMfcButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVEDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_)
