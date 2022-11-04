// BarrelToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "BarrelToolDlg.h"
#include "afxdialogex.h"


// CBarrelToolDlg 对话框

IMPLEMENT_DYNAMIC(CBarrelToolDlg, CDialogEx)

CBarrelToolDlg::CBarrelToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarrelToolDlg::IDD, pParent)
{

}

CBarrelToolDlg::~CBarrelToolDlg()
{
}

void CBarrelToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBarrelToolDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, &CBarrelToolDlg::OnApply)
END_MESSAGE_MAP()


// CBarrelToolDlg 消息处理程序


BOOL CBarrelToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBarrelToolDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CBarrelToolDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CBarrelToolDlg::OnApply()
{
	// TODO: 在此添加命令处理程序代码
}
