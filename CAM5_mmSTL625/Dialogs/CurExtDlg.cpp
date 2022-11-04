// CurExtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "CurExtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurExtDlg dialog


CCurExtDlg::CCurExtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurExtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurExtDlg)
	m_s = 10.0;
	m_r = 10.0;
	m_mode = 0 ;
	//}}AFX_DATA_INIT
}

void CCurExtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurExtDlg)
	DDX_Text(pDX, IDC_EDIT1, m_s);
	DDV_MinMaxDouble(pDX, m_s, 0.001, 10000.);
	DDX_Text(pDX, IDC_EDIT2, m_r);
	DDV_MinMaxDouble(pDX, m_r, 0.001, 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurExtDlg, CDialogEx)
	//{{AFX_MSG_MAP(CCurExtDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurExtDlg message handlers
void CCurExtDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
	pE->EnableWindow(FALSE) ;
	m_mode = 0 ;
}

void CCurExtDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
	pE->EnableWindow(TRUE) ;
	m_mode = 1 ;
}

void CCurExtDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
	pE->EnableWindow(FALSE) ;
	m_mode = 2 ;
}

void CCurExtDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_s -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCurExtDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_r -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

BOOL CCurExtDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_mode == 0 )
	{
		CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
		pE->EnableWindow(FALSE) ;
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	if( m_mode == 1 )
	{
		CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
		pE->EnableWindow(TRUE) ;
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}
	else
	if( m_mode == 2 )
	{
		CEdit* pE = (CEdit*)GetDlgItem(IDC_EDIT2) ;
		pE->EnableWindow(FALSE) ;
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
	}

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurExtDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CCurExtDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CCurExtDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}