#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRefPtDlg dialog
class CRefPtDlg : public CDialogEx
{
public:
	CRefPtDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_REFPTDLG };
	double	m_x;
	double	m_y;
	double	m_z;
	int m_n ;
	double m_ps[100][3] ;
	int m_nSegment ; // nt add 2010/11/26
	int m_mode ; // nt add 2010/11/26

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply() ;
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
};
