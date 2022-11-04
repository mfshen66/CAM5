// BallToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "BallToolDlg.h"
#include "afxdialogex.h"


// CBallToolDlg 对话框

IMPLEMENT_DYNAMIC(CBallToolDlg, CDialogEx)

CBallToolDlg::CBallToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBallToolDlg::IDD, pParent)
{

}

CBallToolDlg::~CBallToolDlg()
{
}

void CBallToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBallToolDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, &CBallToolDlg::OnApply)
END_MESSAGE_MAP()


// CBallToolDlg 消息处理程序


BOOL CBallToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CBallToolDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CBallToolDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CBallToolDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
}
