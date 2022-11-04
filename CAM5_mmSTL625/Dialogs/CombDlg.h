#pragma once

class CCombDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCombDlg)

// Construction
public:
	CCombDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCombDlg();

	enum { IDD = IDD_COMBDLG };
	int m_ns ;
	int m_nu ;
	int m_nv ;
	double m_factor ;
	BOOL m_comb ; // TRUE=按曲率梳方式显示，FALSE=按等值线方式显示
	int m_attType ; // see ISODEF
	int m_nZPlane ; // see ISODEF
	double m_attMin ; // see ISODEF
	double m_attMax ; // see ISODEF
	void ResetRBs() ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnApply() ;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
