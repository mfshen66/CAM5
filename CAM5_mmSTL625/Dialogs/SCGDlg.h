#pragma once


// CSCGDlg 对话框

class CSCGDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSCGDlg)

public:
	CSCGDlg(CWnd* pParent = NULL); // 标准构造函数
	virtual ~CSCGDlg();

// 对话框数据
	enum { IDD = IDD_SCGDLG };
	SURFACE* m_surface ;
	double m_umin ;
	double m_umax ;
	double m_vmin ;
	double m_vmax ;
	int m_size ; // size of buffer m_ps
	double m_max;
	double m_min;
	int m_n ;
	double (*m_ps)[2] ; // x=paramter u or v, y=curvature or angle
	int m_uvflag ; // IDU or IDV
	double m_u ; // u
	double m_v ;
	int m_type ; // 显示何种曲率或其他信息
	int m_ns ; // 曲线分成m_ns段控制显示精度
	BOOL m_grid ; // 0=not display, 1=display
	int m_nu ; // >= 0
	int m_nv ; // >= 0
	WIN2D m_win ; // nt add 2011/4/22
	double m_mx ; // nt add 2011/4/22, mouse position x
	double m_my ; // nt add 2011/4/22, mouse position y
	// 计算一点处的曲率等信息，nt add 2011/4/23
	double m_s ;
	double m_t ;
	double m_k1 ;
	double m_k2 ;
	double m_a1 ;
	double m_a2 ;
	// end

	int Init(int size, SURFACE* surface, BOXUV* boxuv) ;
	void AddPt(double x, double y) ;
	void Clear() ;
	void SetMax(double max) ; // set max curvature
	void SetMin(double min) ; // set min curvature
	void GetBox(double min[2], double max[2]) ;
	void Update() ;
	void ResetRBs() ; // reset radio buttons
	void OnMM(int x) ; // 不需要y

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnCalAtPt();
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
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
