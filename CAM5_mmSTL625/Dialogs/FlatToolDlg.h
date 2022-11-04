#pragma once


// CFlatToolDlg 对话框

class CFlatToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlatToolDlg)

public:
	CFlatToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlatToolDlg();

// 对话框数据
	enum { IDD = IDD_FLATTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
};
