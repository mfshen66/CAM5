#if !defined(AFX_SWPDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_)
#define AFX_SWPDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSwpDlg dialog

class CSwpDlg : public CDialogEx
{
// Construction
public:
	CSwpDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nSpine ;
// Dialog Data
	//{{AFX_DATA(CSwpDlg)
	enum { IDD = IDD_SWPDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSwpDlg)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSwpDlg)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWPDLG_H__D12E5383_DFB7_4A6F_9ED3_FB266E8DFE34__INCLUDED_)
