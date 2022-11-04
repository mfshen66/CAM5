// RefPtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "RefPtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRefPtDlg dialog


CRefPtDlg::CRefPtDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRefPtDlg::IDD, pParent)
{
	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
	m_n = 0 ;
	m_nSegment = 2 ;
	m_mode = 1 ; // 输入点
}


void CRefPtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDX_Text(pDX, IDC_EDIT4, m_nSegment);
	DDV_MinMaxDouble(pDX, m_nSegment, 1, 10000);
}


BEGIN_MESSAGE_MAP(CRefPtDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRefPtDlg message handlers
BOOL CRefPtDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRefPtDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_mode = 1 ; // 输入点
	if( m_n < 100 )
	{
		m_ps[m_n][0] = m_x ;
		m_ps[m_n][1] = m_y ;
		m_ps[m_n][2] = m_z ;
		m_n++ ;
	}

	return ;
}

void CRefPtDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_mode = 2 ; // 等分点
	OnApply() ;

	return ;
}

void CRefPtDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_mode = 3 ; // 圆心点
	OnApply() ;

	return ;
}

void CRefPtDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_mode = 4 ; // 端点
	OnApply() ;

	return ;
}

void CRefPtDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_mode = 5 ; // 中点
	OnApply() ;

	return ;
}

void CRefPtDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	m_mode = 6 ; // 开始交互
	OnApply() ;

	return ;
}

void CRefPtDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	m_mode = 7 ; // 结束交互
	OnApply() ;

	return ;
}

void CRefPtDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CRefPtDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CRefPtDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}