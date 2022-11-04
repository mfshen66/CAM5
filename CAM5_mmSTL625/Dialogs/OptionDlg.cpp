// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "OptionDlg.h"
#include "afxdialogex.h"


// COptionDlg 对话框

IMPLEMENT_DYNAMIC(COptionDlg, CDialogEx)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPTIONDLG, pParent)
{

}

COptionDlg::~COptionDlg()
{
	m_dTol = 0.1 ;
	m_minEdge = 1. ;
	m_maxEdge = 100. ;
	m_minAngle = 2. ;
	m_minArea = 1. ;
	m_bTol = 0.001 ;
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dTol); // 0.1
	DDV_MinMaxDouble(pDX, m_dTol, 0.01, 10.);
	DDX_Text(pDX, IDC_EDIT2, m_minEdge); // 1.
	DDV_MinMaxDouble(pDX, m_minEdge, 0.1, 100.);
	DDX_Text(pDX, IDC_EDIT3, m_maxEdge); // 100.
	DDV_MinMaxDouble(pDX, m_maxEdge, 10., 1000.);
	DDX_Text(pDX, IDC_EDIT4, m_minAngle); // 2.
	DDV_MinMaxDouble(pDX, m_minAngle, 0.2, 60.);
	DDX_Text(pDX, IDC_EDIT5, m_minArea); // 1.
	DDV_MinMaxDouble(pDX, m_minArea, 0.1, 10.);
	DDX_Text(pDX, IDC_EDIT6, m_bTol); // 0.001
	DDV_MinMaxDouble(pDX, m_bTol, 0.0001, 10.);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, OnDeltaposSpin6)
END_MESSAGE_MAP()


// COptionDlg 消息处理程序
void COptionDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_dTol -= pNMUpDown->iDelta*0.1 ;
	if( m_dTol < 0.01 )
		m_dTol = 0.01 ;
	else
	if( m_dTol > 10. )
		m_dTol = 10. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}

void COptionDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_minEdge -= pNMUpDown->iDelta*1. ;
	if( m_minEdge < 0.1 )
		m_minEdge = 0.1 ;
	else
	if( m_minEdge > 100. )
		m_minEdge = 100. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}

void COptionDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_maxEdge -= pNMUpDown->iDelta*10. ;
	if( m_maxEdge < 10. )
		m_maxEdge = 10. ;
	else
	if( m_maxEdge > 1000. )
		m_maxEdge = 1000. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}

void COptionDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_minAngle -= pNMUpDown->iDelta*0.2 ;
	if( m_minAngle < 0.2 )
		m_minAngle = 0.2 ;
	else
	if( m_minAngle > 60. )
		m_minAngle = 60. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}

void COptionDlg::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_minArea -= pNMUpDown->iDelta*0.1 ;
	if( m_minArea < 0.1 )
		m_minArea = 0.1 ;
	else
	if( m_minArea > 10. )
		m_minArea = 10. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}

void COptionDlg::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_bTol -= pNMUpDown->iDelta*0.0001 ;
	if( m_bTol < 0.0001 )
		m_bTol = 0.0001 ;
	else
	if( m_bTol > 10. )
		m_bTol = 10. ;
	UpdateData(FALSE) ;

	*pResult = 0;
}