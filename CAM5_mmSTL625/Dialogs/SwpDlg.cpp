// SwpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\cam.h"
#include "SwpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwpDlg dialog


CSwpDlg::CSwpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSwpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSwpDlg)
	m_nSpine = 1 ;
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CSwpDlg, CDialogEx)
	//{{AFX_MSG_MAP(CSwpDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSwpDlg message handlers
BOOL CSwpDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_nSpine == 1 )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	if( m_nSpine == 2 )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
	}

	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSwpDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_nSpine = 1 ;
}

void CSwpDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_nSpine = 2 ;
}

void CSwpDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_nSpine = 3 ;
}

void CSwpDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CSwpDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CSwpDlg::OnApply()
{
	cmdDlgUI(2) ;
}