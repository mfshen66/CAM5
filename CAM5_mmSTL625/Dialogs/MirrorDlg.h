#pragma once


// CMirrorDlg 对话框

class CMirrorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMirrorDlg)

public:
	CMirrorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMirrorDlg();

// 对话框数据
	enum { IDD = IDD_MIRRORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_mirrorPlane;
	BOOL m_copy ;
	RFRAME m_lf ;
	int m_inited ;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApply();
	void OnSelectedMirrorPlane(RFRAME* pLF);
};
