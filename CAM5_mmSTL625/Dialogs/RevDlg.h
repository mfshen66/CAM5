#pragma once


// CRevDlg 对话框

class CRevDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRevDlg)

public:
	CRevDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRevDlg();

// 对话框数据
	enum { IDD = IDD_REVDLG };
	int m_mode ; // 0=rev u, 1=rev v, 2=uv

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog() ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedMfcbutton1();
	afx_msg void OnClickedMfcbutton2();
	afx_msg void OnClickedMfcbutton3();
	afx_msg void OnClickedMfcbutton4();
};
