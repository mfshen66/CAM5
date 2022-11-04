#pragma once


// CColorDlg 对话框

class CColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CColorDlg();
	int m_colorIndex ; // from 0 to 255
	int m_materialIndex ; // from 0 to 40

// 对话框数据
	enum { IDD = IDD_COLORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply() ;
	void UpdateCMIndex() ;
};
