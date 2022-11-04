// RevDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "RevDlg.h"
#include "afxdialogex.h"


// CRevDlg 对话框

IMPLEMENT_DYNAMIC(CRevDlg, CDialogEx)

CRevDlg::CRevDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRevDlg::IDD, pParent)
{
	m_mode = -1 ;
}

CRevDlg::~CRevDlg()
{
}

void CRevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRevDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CRevDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CRevDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CRevDlg::OnClickedMfcbutton3)
	ON_BN_CLICKED(IDC_MFCBUTTON4, &CRevDlg::OnClickedMfcbutton4)
END_MESSAGE_MAP()


// CRevDlg 消息处理程序
BOOL CRevDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRevDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = 0 ;
	cmdDlgUI(2) ;
}


void CRevDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = 1 ;
	cmdDlgUI(2) ;
}

void CRevDlg::OnClickedMfcbutton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_mode = 2 ;
	cmdDlgUI(2) ;
}


void CRevDlg::OnClickedMfcbutton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}
