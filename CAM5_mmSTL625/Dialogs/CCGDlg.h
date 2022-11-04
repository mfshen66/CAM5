#pragma once


// CCCGDlg 对话框

class CCCGDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCCGDlg)

public:
	CCCGDlg(int size, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCCGDlg();

// 对话框数据
	enum { IDD = IDD_CCGDLG };
	int m_size ; // size of buffer m_ps
	int m_n ;
	double (*m_ps)[2] ; // x=arclength, y=curvature
	void AddPt(double x, double y) ;
	void Clear() ;
	void SetMax(double max) ; // set max curvature
	void SetMin(double min) ; // set min curvature
	void GetBox(double min[2], double max[2]) ;
	void Update() ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_max;
	double m_min;
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
