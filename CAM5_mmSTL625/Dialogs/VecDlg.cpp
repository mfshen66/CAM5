// VecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "VecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVecDlg dialog


CVecDlg::CVecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVecDlg)
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_local = TRUE ;
	//}}AFX_DATA_INIT
}

void CVecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVecDlg)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDV_MinMaxDouble(pDX, m_x, -100., 100.);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDV_MinMaxDouble(pDX, m_y, -100., 100.);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDV_MinMaxDouble(pDX, m_z, -100., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVecDlg, CDialogEx)
	//{{AFX_MSG_MAP(CVecDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVecDlg message handlers
void CVecDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_local = FALSE ;
}

void CVecDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_local = TRUE ;
}

void CVecDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_x -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CVecDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_y -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CVecDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_z -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

BOOL CVecDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_local )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVecDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CVecDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CVecDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}