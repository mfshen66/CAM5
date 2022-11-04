// LenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "LenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLenDlg dialog
CLenDlg::CLenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLenDlg)
	m_len = 10.;
	//}}AFX_DATA_INIT
	m_name = "L" ;
}

CLenDlg::CLenDlg(CString& name, double r)
	: CDialogEx(CLenDlg::IDD, NULL)
{
	m_len = r ;
	m_name = name ;
}

void CLenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLenDlg)
	DDX_Text(pDX, IDC_EDIT1, m_len);
	DDV_MinMaxDouble(pDX, m_len, -100000., 100000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLenDlg, CDialogEx)
	//{{AFX_MSG_MAP(CLenDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLenDlg message handlers
BOOL CLenDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC) ;
	pStatic->SetWindowText(m_name) ;

	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLenDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_len -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CLenDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CLenDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CLenDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}