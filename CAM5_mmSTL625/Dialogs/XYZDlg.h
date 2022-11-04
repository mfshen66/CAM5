#pragma once


// CXYZDlg 对话框

class CXYZDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXYZDlg)

public:
	CXYZDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXYZDlg();

// 对话框数据
	enum { IDD = IDD_XYZDLG };
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	double m_x;
	double m_y;
	double m_z;
};
