// CRAADlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\cam.h"
#include "RAADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRAADlg dialog


CRAADlg::CRAADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRAADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRAADlg)
	m_r = 10.0;
	m_a1 = 0.0;
	m_a2 = 90.0;
	//}}AFX_DATA_INIT
}


void CRAADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRAADlg)
	DDX_Text(pDX, IDC_EDIT1, m_r);
	DDV_MinMaxDouble(pDX, m_r, 1.e-003, 10000.);
	DDX_Text(pDX, IDC_EDIT2, m_a1);
	DDV_MinMaxDouble(pDX, m_a1, 0., 360.);
	DDX_Text(pDX, IDC_EDIT3, m_a2);
	DDV_MinMaxDouble(pDX, m_a2, 0., 360.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRAADlg, CDialogEx)
	//{{AFX_MSG_MAP(CRAADlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRAADlg message handlers
BOOL CRAADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog() ;

	// initialize
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE ;
}

void CRAADlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_r -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CRAADlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_a1 -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

void CRAADlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_a2 -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;	
	*pResult = 0;
}

void CRAADlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CRAADlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CRAADlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}