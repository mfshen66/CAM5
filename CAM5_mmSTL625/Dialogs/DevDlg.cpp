// DevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "DevDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDevDlg dialog


CDevDlg::CDevDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDevDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDevDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_line = TRUE;
	//}}AFX_DATA_INIT
}


void CDevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDevDlg)
	DDX_Check(pDX, IDC_CHECK1, m_line);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDevDlg, CDialogEx)
	//{{AFX_MSG_MAP(CDevDlg)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDevDlg message handlers

BOOL CDevDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;

	if( Flush() == FALSE )
		return FALSE ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDevDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CDevDlg::OnCancel()
{
	CDialogEx::OnOK();
	cmdDlgUI(0) ;
}

void CDevDlg::OnApply()
{
	cmdDlgUI(2) ;
}

BOOL CDevDlg::Flush()
{
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1) ;
	if( pListBox == NULL )
		return FALSE ;
	pListBox->ResetContent() ;

	for( INT_PTR i = 0, n = m_strs.GetSize() ; i < n ; i++ )
		pListBox->AddString(m_strs[i]) ;

	return TRUE ;
}