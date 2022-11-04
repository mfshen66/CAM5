#pragma once

// a contaner of CDialogEx
// nt 2010/8/21
class CDlgPane : public CDockablePane
{
// 构造
public:
	CDlgPane(CDialogEx* pDlg, UINT ID) ;
	virtual ~CDlgPane() ;
	CDialogEx* GetDlg() ;
	UINT GetID() ;

// 特性
protected:
	CDialogEx* m_pDlg ;
	UINT m_ID ;

protected:
	void AdjustLayout() ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};

