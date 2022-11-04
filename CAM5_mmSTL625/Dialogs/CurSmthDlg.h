#pragma once


// CCurSmthDlg 对话框

class CCurSmthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurSmthDlg)

public:
	CCurSmthDlg(CWnd* pParent = NULL);   // 标准构造函数
	CCurSmthDlg(double tol, BOOL g1);
	virtual ~CCurSmthDlg();

// 对话框数据
	enum { IDD = IDD_CURSMTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_tol;
	int m_factor;
	BOOL m_g1;
	BOOL m_bz ; // 1=use Bezier curve, nt add 2011/1/28
	int m_method ; // 0=smooth(能量法光顺), 1=optimize(遗传算法优化), 2=保凸
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnApply() ;
	afx_msg void OnButton1();
	afx_msg void OnButton2();
};
