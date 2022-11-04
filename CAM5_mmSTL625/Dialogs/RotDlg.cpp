// RotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\cam.h"
#include "RotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotDlg dialog


CRotDlg::CRotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotDlg)
	m_angle = 90.;
	m_copy = TRUE;
	m_local = TRUE ;
	m_axis = 2 ; // nt add 2011/6/12
	//}}AFX_DATA_INIT
}


void CRotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotDlg)
	DDX_Text(pDX, IDC_EDIT1, m_angle);
	DDV_MinMaxDouble(pDX, m_angle, -360., 360.);
	DDX_Check(pDX, IDC_CHECK1, m_copy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotDlg, CDialogEx)
	//{{AFX_MSG_MAP(CRotDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotDlg message handlers
// nt add 2011/6/12
void CRotDlg::UpdateRBs()
{
	if( m_local )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}

	// nt add 2011/6/12
	if( m_axis == 0 )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
	}
	else
	if( m_axis == 1 )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO4) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO5) ;
		pButton->SetCheck(1) ;
	}
}

BOOL CRotDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateRBs() ;

	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_angle -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CRotDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_local = TRUE ;
	UpdateRBs() ;
}

void CRotDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_local = FALSE ;
	UpdateRBs() ;
}

// nt add 2011/6/12
void CRotDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_axis = 0 ;
	UpdateRBs() ;
}

// nt add 2011/6/12
void CRotDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_axis = 1 ;
	UpdateRBs() ;
}

// nt add 2011/6/12
void CRotDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_axis = 2 ;
	UpdateRBs() ;
}

void CRotDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CRotDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CRotDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}