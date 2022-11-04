#pragma once


// CBarrelToolDlg 对话框

class CBarrelToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBarrelToolDlg)

public:
	CBarrelToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarrelToolDlg();

// 对话框数据
	enum { IDD = IDD_BARRELTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
};
