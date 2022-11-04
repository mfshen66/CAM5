#pragma once


// CSurSmthDlg 对话框

class CSurSmthDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSurSmthDlg)

public:
	CSurSmthDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSurSmthDlg(double tol, BOOL g1);
	virtual ~CSurSmthDlg();

// 对话框数据
	enum { IDD = IDD_SURSMTHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_tol;
	int m_factor;
	BOOL m_g1;
	BOOL m_bz ; // 1=use Bezier surface
	int m_method ; // 0=smooth(能量法优化), 1=optimize(平滑), 2=保凸
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnApply() ;
	afx_msg void OnButton1();
	afx_msg void OnButton2();
};
