// AngDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "AngDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAngDlg dialog


CAngDlg::CAngDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAngDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAngDlg)
	m_ang = 90.0;
	//}}AFX_DATA_INIT
}


void CAngDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAngDlg)
	DDX_Text(pDX, IDC_EDIT1, m_ang);
	DDV_MinMaxDouble(pDX, m_ang, -360., 360.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAngDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAngDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAngDlg message handlers

void CAngDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_ang -= pNMUpDown->iDelta*5.0 ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

BOOL CAngDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAngDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CAngDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CAngDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}