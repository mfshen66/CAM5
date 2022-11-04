// ChkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "ChkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChkDlg dialog


CChkDlg::CChkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChkDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChkDlg, CDialogEx)
	//{{AFX_MSG_MAP(CChkDlg)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChkDlg message handlers

BOOL CChkDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;

	if( Flush() == FALSE )
		return FALSE ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChkDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CChkDlg::OnCancel()
{
	CDialogEx::OnOK();
	cmdDlgUI(0) ;
}

void CChkDlg::OnApply()
{
	cmdDlgUI(2) ;
}

BOOL CChkDlg::Flush()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1) ;
	if( pListBox == NULL )
		return FALSE ;
	pListBox->ResetContent() ;

	for( _int64 i = 0, n = m_strs.GetSize() ; i < n ; i++ )
		pListBox->AddString(m_strs[i]) ;

	return TRUE ;
}