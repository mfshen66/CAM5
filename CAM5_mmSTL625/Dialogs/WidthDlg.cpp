// WidthDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "WidthDlg.h"
#include "afxdialogex.h"

// 注意线宽的单位=mm/100
static LW lws[] =
{
	LW(0, _T("0.00mm")),
	LW(5, _T("0.05mm")),
	LW(9, _T("0.09mm")),
	LW(13, _T("0.13mm")),
	LW(15, _T("0.15mm")),
	LW(18, _T("0.18mm")),
	LW(20, _T("0.2mm")),
	LW(25, _T("0.25mm")),
	LW(30, _T("0.3mm")),
	LW(35, _T("0.35mm")),
	LW(40, _T("0.4mm")),
	LW(50, _T("0.5mm")),
	LW(53, _T("0.53mm")),
	LW(60, _T("0.6mm")),
	LW(70, _T("0.7mm")),
	LW(80, _T("0.8mm")),
	LW(90, _T("0.9mm")),
	LW(106, _T("1.06mm")),
	LW(120, _T("1.2mm")),
	LW(140, _T("1.4mm")),
	LW(158, _T("1.58mm")),
	LW(200, _T("2mm")),
	LW(211, _T("2.11mm")),
	LW(300, _T("3mm")),
	LW(400, _T("4mm")),
	LW(500, _T("5mm")),
	LW(750, _T("7.5mm")),
	LW(1000, _T("10mm")),
	LW(2500, _T("25mm")),
	LW(5000, _T("50mm")),
	LW(7500, _T("75mm")),
	LW(10000, _T("100mm")),
	LW(-1, _T("随层")),
	LW(-2, _T("随块"))
} ;

// CWidthDlg 对话框

IMPLEMENT_DYNAMIC(CWidthDlg, CDialogEx)

CWidthDlg::CWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWidthDlg::IDD, pParent)
{
	m_width = 0 ;
}

CWidthDlg::~CWidthDlg()
{
}

void CWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cb);
}


BEGIN_MESSAGE_MAP(CWidthDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()

// CWidthDlg 消息处理程序
BOOL CWidthDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	int i, j = -1, min = 10000 ;
	for( i = 0 ; i < 34 ; i++ )
	{
		m_cb.AddString(lws[i].strName) ;
		if( min > abs(lws[i].dLW-m_width) ) // match m_width
		{
			min = abs(lws[i].dLW-m_width) ;
			j = i ;
		}
	}
	if( j >= 0 )
		m_cb.SetCurSel(j) ;
	UpdateData(FALSE) ;

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWidthDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CWidthDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CWidthDlg::OnApply()
{
	cmdDlgUI(2) ;
}

void CWidthDlg::SetWidth(int width)
{
	m_width = width ;
}

int CWidthDlg::GetWidth()
{
	int i = m_cb.GetCurSel() ;
	if( i >= 0 &&
		i < 34 )
		m_width = lws[i].dLW ;
	else
		m_width = 0 ;
	return m_width ;
}