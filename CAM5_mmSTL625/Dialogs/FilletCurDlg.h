#pragma once


// CFilletCurDlg 对话框

class CFilletCurDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFilletCurDlg)

public:
	CFilletCurDlg(void* pDoc, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilletCurDlg();

// 对话框数据
	enum { IDD = IDD_FILLETCURDLG };
	int m_type ; // nt add 2011/5/4
	double m_r;
	// in order to dynamically modify w1 and w2 parameter, 2011/8/21
	FILLETCUR m_fc ; // nt add 2011/8/21
	void* m_pDoc ; // nt add 2011/8/21
	void* m_pRefCur ; // nt add 2011/8/21
	double m_w1 ; // nt add 2011/8/21
	double m_w2 ; // nt add 2011/8/21
	// end 2011/8/21

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	int Update() ; // nt add 2011/8/21

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnApply();
	void ResetRBs() ;
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	afx_msg void OnRadio9();
	afx_msg void OnRadio10();
};
