// MachineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "MachineDlg.h"
#include "afxdialogex.h"


// CMachineDlg 对话框

IMPLEMENT_DYNAMIC(CMachineDlg, CDialogEx)

CMachineDlg::CMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMachineDlg::IDD, pParent)
{

}

CMachineDlg::~CMachineDlg()
{
}

void CMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMachineDlg, CDialogEx)
END_MESSAGE_MAP()


// CMachineDlg 消息处理程序


BOOL CMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMachineDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}


void CMachineDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}
