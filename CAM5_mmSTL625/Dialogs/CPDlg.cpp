// CPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "CPDlg.h"
#include "afxdialogex.h"


// CCPDlg 对话框

IMPLEMENT_DYNAMIC(CCPDlg, CDialogEx)

CCPDlg::CCPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCPDlg::IDD, pParent)
{

}

CCPDlg::~CCPDlg()
{
}

void CCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCPDlg, CDialogEx)
END_MESSAGE_MAP()


// CCPDlg 消息处理程序


BOOL CCPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCPDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void CCPDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
