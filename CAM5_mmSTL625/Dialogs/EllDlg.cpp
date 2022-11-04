// EllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "EllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEllDlg dialog


CEllDlg::CEllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEllDlg)
	m_r1 = 25.;
	m_r2 = 5.;
	//}}AFX_DATA_INIT
}

CEllDlg::CEllDlg(double r1, double r2)
	: CDialogEx(CEllDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CEllDlg)
	m_r1 = r1 ;
	m_r2 = r2 ;
	//}}AFX_DATA_INIT
}

void CEllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEllDlg)
	DDX_Text(pDX, IDC_EDIT1, m_r1);
	DDV_MinMaxDouble(pDX, m_r1, 1.e-002, 10000.);
	DDX_Text(pDX, IDC_EDIT2, m_r2);
	DDV_MinMaxDouble(pDX, m_r2, 1.e-002, 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEllDlg, CDialogEx)
	//{{AFX_MSG_MAP(CEllDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllDlg message handlers
BOOL CEllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog() ;

	// initialize
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE ;
}

void CEllDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_r1 -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

void CEllDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_r2 -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

void CEllDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CEllDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CEllDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}