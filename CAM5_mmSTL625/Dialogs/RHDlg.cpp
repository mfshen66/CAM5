// RHDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "RHDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRHDlg dialog


CRHDlg::CRHDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRHDlg)
	m_r = 25.;
	m_h = 50.;
	//}}AFX_DATA_INIT
}


void CRHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRHDlg)
	DDX_Text(pDX, IDC_EDIT1, m_r);
	DDV_MinMaxDouble(pDX, m_r, 1.e-002, 10000.);
	DDX_Text(pDX, IDC_EDIT2, m_h);
	DDV_MinMaxDouble(pDX, m_h, 1.e-002, 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRHDlg, CDialogEx)
	//{{AFX_MSG_MAP(CRHDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRHDlg message handlers

void CRHDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_r -= pNMUpDown->iDelta ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

void CRHDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_h -= pNMUpDown->iDelta ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

BOOL CRHDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRHDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CRHDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CRHDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}