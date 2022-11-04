// IsoCurDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "IsoCurDlg.h"
#include "afxdialogex.h"


// CIsoCurDlg 对话框

IMPLEMENT_DYNAMIC(CIsoCurDlg, CDialogEx)

CIsoCurDlg::CIsoCurDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIsoCurDlg::IDD, pParent)
{
	m_selectedSur = FALSE ;
	m_pRefSur = NULL ;
	m_umin = 0.0;
	m_umax = 0.0;
	m_vmin = 0.0;
	m_vmax = 0.0;
	m_uvFlag = 0;
	m_u = 0.0;
	m_v = 0.0;
}

CIsoCurDlg::~CIsoCurDlg()
{
}

void CIsoCurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_selectedSur);
	DDX_Text(pDX, IDC_EDIT1, m_umin);
	DDV_MinMaxDouble(pDX, m_umin, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT2, m_umax);
	DDV_MinMaxDouble(pDX, m_umax, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT3, m_vmin);
	DDV_MinMaxDouble(pDX, m_vmin, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT4, m_vmax);
	DDV_MinMaxDouble(pDX, m_vmax, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT5, m_u);
	DDV_MinMaxDouble(pDX, m_u, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT6, m_v);
	DDV_MinMaxDouble(pDX, m_v, -1000000, 1000000);
}

BEGIN_MESSAGE_MAP(CIsoCurDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()

// CIsoCurDlg 消息处理程序
BOOL CIsoCurDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if( m_uvFlag == 0 )
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}

	if( m_pRefSur )
		m_selectedSur = TRUE ;

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CIsoCurDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CIsoCurDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CIsoCurDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

void CIsoCurDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_uvFlag = 0 ;
}

void CIsoCurDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_uvFlag = 1 ;
}

void CIsoCurDlg::SetBoxUV(double umin,
						  double umax,
						  double vmin,
						  double vmax)
{
	m_umin = umin ;
	m_umax = umax ;
	m_vmin = vmin ;
	m_vmax = vmax ;
}

void CIsoCurDlg::OnSelectedSur(void* pRefSur)
{
	if( pRefSur )
		m_selectedSur = TRUE ;
	else
		m_selectedSur = FALSE ;
	m_pRefSur = pRefSur ;
}