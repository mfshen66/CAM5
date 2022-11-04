// FlatToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "FlatToolDlg.h"
#include "afxdialogex.h"


// CFlatToolDlg 对话框

IMPLEMENT_DYNAMIC(CFlatToolDlg, CDialogEx)

CFlatToolDlg::CFlatToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlatToolDlg::IDD, pParent)
{

}

CFlatToolDlg::~CFlatToolDlg()
{
}

void CFlatToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFlatToolDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, &CFlatToolDlg::OnApply)
END_MESSAGE_MAP()


// CFlatToolDlg 消息处理程序


BOOL CFlatToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFlatToolDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CFlatToolDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}


void CFlatToolDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
}
