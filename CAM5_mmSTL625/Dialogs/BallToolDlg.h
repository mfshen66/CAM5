#pragma once


// CBallToolDlg 对话框

class CBallToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBallToolDlg)

public:
	CBallToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBallToolDlg();

// 对话框数据
	enum { IDD = IDD_BALLTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
};
