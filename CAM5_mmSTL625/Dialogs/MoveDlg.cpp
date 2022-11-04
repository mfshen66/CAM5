// MoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h"
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "MoveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoveDlg dialog
CMoveDlg::CMoveDlg(CCAMDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CMoveDlg::IDD, pParent)
{
	m_pDoc = pDoc ; // nt add 2011/6/24
	//{{AFX_DATA_INIT(CMoveDlg)
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_copy = TRUE;
	m_local = TRUE ;
	//}}AFX_DATA_INIT
	m_begin[0] = 0. ; // nt add 2011/6/24
	m_begin[1] = 0. ; // nt add 2011/6/24
	m_begin[2] = 0. ; // nt add 2011/6/24
	m_end[0] = 0. ; // nt add 2011/6/24
	m_end[1] = 0. ; // nt add 2011/6/24
	m_end[2] = 0. ; // nt add 2011/6/24
}


void CMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveDlg)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDV_MinMaxDouble(pDX, m_x, -1000., 1000.);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDV_MinMaxDouble(pDX, m_y, -1000., 1000.);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDV_MinMaxDouble(pDX, m_z, -1000., 1000.);
	DDX_Check(pDX, IDC_CHECK1, m_copy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveDlg, CDialogEx)
	//{{AFX_MSG_MAP(CMoveDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CMoveDlg::OnClickedMfcButton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CMoveDlg::OnClickedMfcButton2)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveDlg message handlers

void CMoveDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_local = TRUE ;
}

void CMoveDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_local = FALSE ;
}

BOOL CMoveDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
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

	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMoveDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CMoveDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CMoveDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

// nt add 2011/6/24
void CMoveDlg::OnClickedMfcButton1()
{
	if( m_pDoc )
	{
		int n = m_pDoc->GetSelected(ftRefPt, 1) ;
		if( n > 0 )
		{
			CRefPt* pRefPt = (CRefPt*)(m_pDoc->GetObj(0)) ;
			pRefPt->GetP(m_begin) ;
			m_x = m_end[0]-m_begin[0] ;
			m_y = m_end[1]-m_begin[1] ;
			m_z = m_end[2]-m_begin[2] ;
			UpdateData(FALSE) ;
		}
	}
}

// nt add 2011/6/24
void CMoveDlg::OnClickedMfcButton2()
{
	if( m_pDoc )
	{
		int n = m_pDoc->GetSelected(ftRefPt, 1) ;
		if( n > 0 )
		{
			CRefPt* pRefPt = (CRefPt*)(m_pDoc->GetObj(0)) ;
			pRefPt->GetP(m_end) ;
			m_x = m_end[0]-m_begin[0] ;
			m_y = m_end[1]-m_begin[1] ;
			m_z = m_end[2]-m_begin[2] ;
			UpdateData(FALSE) ;
		}
	}
}