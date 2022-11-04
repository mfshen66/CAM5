#pragma once


// CMachineDlg 对话框

class CMachineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineDlg)

public:
	CMachineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachineDlg();

// 对话框数据
	enum { IDD = IDD_MACHINEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};
