// ColorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "ColorDlg.h"
#include "afxdialogex.h"


// CColorDlg 对话框

IMPLEMENT_DYNAMIC(CColorDlg, CDialogEx)

CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColorDlg::IDD, pParent)
{
	m_colorIndex = 255 ;
	m_materialIndex = 0 ;
}

CColorDlg::~CColorDlg()
{
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorDlg, CDialogEx)
	ON_COMMAND(IDAPPLY, OnApply)
END_MESSAGE_MAP()


// CColorDlg 消息处理程序

BOOL CColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	unsigned char rgb[3] ;
	rgbsGet(m_colorIndex, rgb) ;
	COLORREF currentColor = RGB(rgb[0], rgb[1], rgb[2]) ;
	CMFCColorButton* pCD = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON1) ;
	pCD->SetColor(currentColor) ;

	CVSListBox* pLB = (CVSListBox*)GetDlgItem(IDC_MFCVSLISTBOX1) ;
	if( pLB )
	{
		int i ;
		CString name ;
		MATERIAL material ;
		for( i = 0 ; i < 256 ; i++ ) // 最多取这么多
		{
			materialGet2(&material, i) ;
			name = material.name ;
			if( 	name == "default" )
				break ;
			pLB->AddItem(name) ;
		}
		if( m_materialIndex >= 0 &&
			m_materialIndex < pLB->GetCount() )
			pLB->SelectItem(m_materialIndex) ;
	}

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CColorDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateCMIndex() ;
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CColorDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateCMIndex() ;
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CColorDlg::OnApply()
{
	UpdateData(TRUE) ;
	UpdateCMIndex() ;
	cmdDlgUI(2) ;
}

void CColorDlg::UpdateCMIndex()
{
	CMFCColorButton* pCD = (CMFCColorButton*)GetDlgItem(IDC_MFCCOLORBUTTON1) ;
	COLORREF currentColor = pCD->GetColor() ;
	unsigned char rgb[3] ;
	rgb[0] = GetRValue(currentColor) ;
	rgb[1] = GetGValue(currentColor) ;
	rgb[2] = GetBValue(currentColor) ;
	m_colorIndex = rgbsMatch(rgb) ;

	CVSListBox* pLB = (CVSListBox*)GetDlgItem(IDC_MFCVSLISTBOX1) ;
	m_materialIndex = pLB->GetSelItem() ;
}