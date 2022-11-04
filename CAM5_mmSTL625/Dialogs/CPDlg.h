#pragma once


// CCPDlg 对话框

class CCPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCPDlg)

public:
	CCPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCPDlg();

// 对话框数据
	enum { IDD = IDD_CPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
