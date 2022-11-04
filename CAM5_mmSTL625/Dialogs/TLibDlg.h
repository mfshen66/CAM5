#pragma once


// CTLibDlg 对话框

class CTLibDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTLibDlg)

public:
	CTLibDlg(CCAMDoc* pDoc, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTLibDlg();

// 对话框数据
	enum { IDD = IDD_TLIBDLG };
	CCAMDoc* m_pDoc ; // nt add 2010/9/27
	CImageList m_images ;
	CTGroup* m_pTG ;
	CTSubGroup* m_pTSG ;
	CTool* m_pTool ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();
	afx_msg void OnClickedButton4();
	afx_msg void OnClickedMfcButton1(); // nt add 2010/9/27
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	HTREEITEM GetSelected() ; // 结果存于m_pTG, m_pTSG, m_pTool
};
