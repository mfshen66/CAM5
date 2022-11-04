#pragma once


// CPDesignDlg 对话框

class CPDesignDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPDesignDlg)

public:
	CPDesignDlg(CCAMDoc* pDoc, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPDesignDlg();

// 对话框数据
	enum { IDD = IDD_PDESIGNDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	CCAMDoc* m_pDoc ;
	CStep* m_pStep ; // 当前编辑的工步
	CString m_name ; // 工步名称
	int m_gm ; // geometry model for CStep:0=workpiece,1=part,2=protect_surfaces

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
	afx_msg void OnClickedMfcbutton4();
	afx_msg void OnClickedMfcbutton5();
	afx_msg void OnClickedMfcbutton6();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
};
