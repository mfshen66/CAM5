// CombDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\cam.h"
#include "CombDlg.h"
#include "afxdialogex.h"

// CCombDlg dialog
IMPLEMENT_DYNAMIC(CCombDlg, CDialogEx)

CCombDlg::CCombDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCombDlg::IDD, pParent)
{
	m_ns = 100 ;
	m_nu = 10 ;
	m_nv = 10 ;
	m_factor = 1000. ;
	m_comb = TRUE ;
	m_attType = 0 ;
	m_nZPlane = 40 ;
	m_attMin = 0. ;
	m_attMax = 0. ;
}

CCombDlg::~CCombDlg()
{
}

void CCombDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ns);
	DDV_MinMaxDouble(pDX, m_ns, 10, 2000);
	DDX_Text(pDX, IDC_EDIT2, m_nu);
	DDV_MinMaxDouble(pDX, m_nu, 2, 500);
	DDX_Text(pDX, IDC_EDIT3, m_nv);
	DDV_MinMaxDouble(pDX, m_nv, 2, 500);
	DDX_Text(pDX, IDC_EDIT4, m_factor);
	DDV_MinMaxDouble(pDX, m_factor, 0.001, 1000000);
	DDX_Text(pDX, IDC_EDIT5, m_nZPlane);
	DDV_MinMaxDouble(pDX, m_nZPlane, 2, 500);
	DDX_Text(pDX, IDC_EDIT6, m_attMin);
	DDV_MinMaxDouble(pDX, m_attMin, -1000000., 1000000.);
	DDX_Text(pDX, IDC_EDIT7, m_attMax);
	DDV_MinMaxDouble(pDX, m_attMax, -1000000., 1000000.);
}

BEGIN_MESSAGE_MAP(CCombDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, OnDeltaposSpin7)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()

// CCombDlg message handlers

void CCombDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_ns -= pNMUpDown->iDelta*50 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_nu -= pNMUpDown->iDelta*5 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_nv -= pNMUpDown->iDelta*5 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	if( pNMUpDown->iDelta < 0 )
		m_factor *= 2. ;
	else
		m_factor /= 2. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_nZPlane -= pNMUpDown->iDelta*5 ;
	if( m_nZPlane < 2 )
		m_nZPlane = 2 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	if( pNMUpDown->iDelta < 0 )
		m_attMin *= 2. ;
	else
		m_attMin /= 2. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CCombDlg::OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	if( pNMUpDown->iDelta < 0 )
		m_attMax *= 2. ;
	else
		m_attMax /= 2. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

BOOL CCombDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;
	ResetRBs() ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCombDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CCombDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CCombDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

void CCombDlg::ResetRBs()
{
	CButton* pButton = NULL ;

	if( m_comb )
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}

	switch(m_attType)
	{
	case 0:
		pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
		break ;
	case 1:
		pButton = (CButton*)GetDlgItem(IDC_RADIO4) ;
		pButton->SetCheck(1) ;
		break ;
	case 2:
		pButton = (CButton*)GetDlgItem(IDC_RADIO5) ;
		pButton->SetCheck(1) ;
		break ;
	default:
		break ;
	}
}

void CCombDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_comb = TRUE ;
	ResetRBs() ;
}

void CCombDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_comb = FALSE ;
	ResetRBs() ;
}

void CCombDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_attType = 0 ;
	ResetRBs() ;
}

void CCombDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_attType = 1 ;
	ResetRBs() ;
}

void CCombDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_attType = 2 ;
	ResetRBs() ;
}