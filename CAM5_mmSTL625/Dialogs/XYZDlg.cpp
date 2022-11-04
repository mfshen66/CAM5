// Dialogs\XYZDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "XYZDlg.h"
#include "afxdialogex.h"


// CXYZDlg 对话框

IMPLEMENT_DYNAMIC(CXYZDlg, CDialogEx)

CXYZDlg::CXYZDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXYZDlg::IDD, pParent)
{

	m_x = 0.0;
	m_y = 0.0;
	m_z = 0.0;
}

CXYZDlg::~CXYZDlg()
{
}

void CXYZDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDV_MinMaxDouble(pDX, m_x, -100000000, 100000000);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDV_MinMaxDouble(pDX, m_y, -10000000, 100000000);
	DDX_Text(pDX, IDC_EDIT3, m_z);
	DDV_MinMaxDouble(pDX, m_z, -100000000, 100000000);
}


BEGIN_MESSAGE_MAP(CXYZDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CXYZDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog() ;

	// initialize
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE ;
}

// CXYZDlg 消息处理程序
void CXYZDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CXYZDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}