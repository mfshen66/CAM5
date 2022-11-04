#if !defined(AFX_CURPTSDLG_H__76F7FFFF_BF2A_415A_9D3D_FFD6730CAD83__INCLUDED_)
#define AFX_CURPTSDLG_H__76F7FFFF_BF2A_415A_9D3D_FFD6730CAD83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurPtsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurPtsDlg dialog

class CCurPtsDlg : public CDialogEx
{
// Construction
public:
	CCurPtsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurPtsDlg)
	enum { IDD = IDD_CURPTSDLG };
	int		m_n;
	CString	m_outFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurPtsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurPtsDlg)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnApply() ;
	afx_msg void OnFile() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURPTSDLG_H__76F7FFFF_BF2A_415A_9D3D_FFD6730CAD83__INCLUDED_)
