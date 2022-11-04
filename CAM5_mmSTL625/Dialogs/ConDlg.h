#pragma once


// CConDlg 对话框

class CConDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConDlg)

public:
	CConDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConDlg();

// 对话框数据
	enum { IDD = IDD_CONDLG };
	int m_mode ; // 0=method1, 1=method2

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog() ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
};
