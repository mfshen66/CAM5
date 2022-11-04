// MirrorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "MirrorDlg.h"
#include "afxdialogex.h"


// CMirrorDlg 对话框

IMPLEMENT_DYNAMIC(CMirrorDlg, CDialogEx)

CMirrorDlg::CMirrorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMirrorDlg::IDD, pParent)
{
	m_mirrorPlane = FALSE ;
	m_copy = TRUE ;
	mathInitRFrame(&m_lf) ;
	m_inited = 0 ;
}

CMirrorDlg::~CMirrorDlg()
{
}

void CMirrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_mirrorPlane);
	DDX_Check(pDX, IDC_CHECK2, m_copy);
}

BEGIN_MESSAGE_MAP(CMirrorDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()

// CMirrorDlg 消息处理程序
BOOL CMirrorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_inited = 1 ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMirrorDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CMirrorDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE) ;
	cmdDlgUI(0) ;
}

void CMirrorDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

void CMirrorDlg::OnSelectedMirrorPlane(RFRAME* pLF)
{
	if( pLF )
		m_lf = *pLF ;
	m_mirrorPlane = TRUE ;
	if( m_inited )
		UpdateData(FALSE) ;
}