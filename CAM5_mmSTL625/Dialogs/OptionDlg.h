#pragma once


// OptionDlg 对话框

class COptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COptionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONDLG };
#endif
	double m_dTol ; // mm
	double m_minEdge ; // mm
	double m_maxEdge ; // mm
	double m_minAngle ; // 度
	double m_minArea ; // mm^2
	double m_bTol ; // mm

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult);
};
