#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

class CScaleDlg : public CDialog
{
// Construction
public:
	CScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleDlg)
	enum { IDD = IDD_SCALE };
	double	m_scale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
