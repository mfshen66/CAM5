#pragma once


// CCFGDlg 对话框

class CCFGDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCFGDlg)

public:
	CCFGDlg(int size, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCFGDlg();

// 对话框数据
	enum { IDD = IDD_CFGDLG };
	int m_size ; // size of buffer m_ps
	int m_n ;
	double (*m_ps)[2] ; // x=arclength, y=energy
	void AddPt(double x, double y) ;
	void Clear() ;
	void SetMax(double max) ; // set max energy
	void SetMin(double min) ; // set min energy
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
