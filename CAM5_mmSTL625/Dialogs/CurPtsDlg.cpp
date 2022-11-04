// CurPtsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "CurPtsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CCurPtsDlg dialog


CCurPtsDlg::CCurPtsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurPtsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurPtsDlg)
	m_n = 10;
	m_outFile = _T("c:\\points.txt");
	//}}AFX_DATA_INIT
}


void CCurPtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurPtsDlg)
	DDX_Text(pDX, IDC_EDIT1, m_n);
	DDV_MinMaxInt(pDX, m_n, 2, 100000);
	DDX_Text(pDX, IDC_EDIT2, m_outFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurPtsDlg, CDialogEx)
	//{{AFX_MSG_MAP(CCurPtsDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_COMMAND(IDAPPLY, OnApply)
	ON_COMMAND(IDC_MFCBUTTON1, OnFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurPtsDlg message handlers
BOOL CCurPtsDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurPtsDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_n -= pNMUpDown->iDelta ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCurPtsDlg::OnFile()
{
	CString szFilter = _T("Points Files (*.TXT)|*.txt|") ;
	CFileDialog dlg(TRUE, _T("*.txt"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		m_outFile = dlg.GetPathName() ;
		UpdateData(FALSE) ;
	}
}

void CCurPtsDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CCurPtsDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CCurPtsDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}