// FilletCurDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "FilletCurDlg.h"
#include "afxdialogex.h"
#include "..\MainFrm.h"
#include "..\Models.h"
#include "..\Mills.h" // nt add 2010/4/1
#include "..\SelSet.h"
#include "..\CAMDoc.h"

// CFilletCurDlg 对话框

IMPLEMENT_DYNAMIC(CFilletCurDlg, CDialogEx)

CFilletCurDlg::CFilletCurDlg(void* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilletCurDlg::IDD, pParent)
{
	m_type = 5 ;
	m_r = 5.0;
	//filletcurInit(&m_fc) ; // nt add 2011/8/21
	m_pDoc = pDoc ; // nt add 2011/8/21
	m_pRefCur = NULL ; // nt add 2011/8/21
	m_w1 = 1. ; // nt add 2011/8/21
	m_w2 = 1. ; // nt add 2011/8/21
}

CFilletCurDlg::~CFilletCurDlg()
{
}

void CFilletCurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_r);
	DDV_MinMaxDouble(pDX, m_r, 0.001, 10000);
}


BEGIN_MESSAGE_MAP(CFilletCurDlg, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CFilletCurDlg::OnDeltaposSpin1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDAPPLY, &CFilletCurDlg::OnApply)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
END_MESSAGE_MAP()


// CFilletCurDlg 消息处理程序


BOOL CFilletCurDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ResetRBs() ;
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFilletCurDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}


void CFilletCurDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}


void CFilletCurDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	m_r -= pNMUpDown->iDelta*5. ;
    UpdateData(FALSE) ;
	*pResult = 0;
}

// nt add 2011/8/15
void CFilletCurDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar) ;
	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar ;
	UINT pos = pSlider->GetPos() ;
	CSliderCtrl* pSlider1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	CSliderCtrl* pSlider2 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2) ;
	if( pSlider1 == pSlider )
		m_w1 = (double)pos/50 ;
	else
		m_w2 = (double)pos/50 ;

	if( m_pDoc &&
		m_pRefCur &&
		m_fc.curve1 &&
		m_fc.curve2 &&
		(m_type == 2 ||
		m_type == 9) )
	{
		m_fc.w1 = m_w1 ;
		m_fc.w2 = m_w2 ;
		int rt = 0 ;
		//if( m_type == 2 )
		//	rt = filletcurInterpPTC(&m_fc) ;
		//else
		//	rt = filletcurInterpPTCAndPt(&m_fc) ;
		if( rt )
			Update() ;
	}
}

// nt add 2011/8/22
int CFilletCurDlg::Update()
{
	CCAMDoc* pDoc = (CCAMDoc*)m_pDoc ;
	if( pDoc )
		pDoc->ClearSelSet() ;
	CRefCur* pRefCur = (CRefCur*)m_pRefCur ;
	if( pRefCur )
	{
		pRefCur->Replace(m_fc.curve, &(m_fc.interval)) ;
		m_fc.curve = NULL ;
	}

	return 1 ;
}

void CFilletCurDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

void CFilletCurDlg::ResetRBs()
{
	CButton* pButton = NULL ;

	switch(m_type)
	{
	case 0:
		pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
		break ;
	case 1:
		pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
		break ;
	case 2:
		pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
		break ;
	case 3:
		pButton = (CButton*)GetDlgItem(IDC_RADIO4) ;
		pButton->SetCheck(1) ;
		break ;
	case 4:
		pButton = (CButton*)GetDlgItem(IDC_RADIO5) ;
		pButton->SetCheck(1) ;
		break ;
	case 5:
		pButton = (CButton*)GetDlgItem(IDC_RADIO6) ;
		pButton->SetCheck(1) ;
		break ;
	case 6:
		pButton = (CButton*)GetDlgItem(IDC_RADIO7) ;
		pButton->SetCheck(1) ;
		break ;
	case 7:
		pButton = (CButton*)GetDlgItem(IDC_RADIO8) ;
		pButton->SetCheck(1) ;
		break ;
	case 8:
		pButton = (CButton*)GetDlgItem(IDC_RADIO9) ;
		pButton->SetCheck(1) ;
		break ;
	case 9:
		pButton = (CButton*)GetDlgItem(IDC_RADIO10) ;
		pButton->SetCheck(1) ;
		break ;
	default:
		break ;
	}

	// nt add 2011/8/21
	CSliderCtrl* pSlider1 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	CSliderCtrl* pSlider2 = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2) ;
	CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_STATIC1) ;
	CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_STATIC2) ;
	if( m_type == 2 ||
		m_type == 9 )
	{
		pSlider1->EnableWindow(TRUE) ;
		pSlider1->ShowWindow(TRUE) ;
		pSlider2->EnableWindow(TRUE) ;
		pSlider2->ShowWindow(TRUE) ;
		pStatic1->EnableWindow(TRUE) ;
		pStatic1->ShowWindow(TRUE) ;
		pStatic2->EnableWindow(TRUE) ;
		pStatic2->ShowWindow(TRUE) ;
		pSlider1->SetRange(1, 100, TRUE) ;
		pSlider1->SetPos(50) ;
		pSlider2->SetRange(1, 100, TRUE) ;
		pSlider2->SetPos(50) ;
		m_w1 = 1. ;
		m_w2 = 1. ;
	}
	else
	{
		pSlider1->EnableWindow(FALSE) ;
		pSlider1->ShowWindow(FALSE) ;
		pSlider2->EnableWindow(FALSE) ;
		pSlider2->ShowWindow(FALSE) ;
		pStatic1->EnableWindow(FALSE) ;
		pStatic1->ShowWindow(FALSE) ;
		pStatic2->EnableWindow(FALSE) ;
		pStatic2->ShowWindow(FALSE) ;
	}

	// nt add 2011/8/21
	m_pRefCur = NULL ;
}

void CFilletCurDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_type = 0 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_type = 1 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_type = 2 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_type = 3 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_type = 4 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	m_type = 5 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_type = 6 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_type = 7 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio9() 
{
	// TODO: Add your control notification handler code here
	m_type = 8 ;
	ResetRBs() ;
}

void CFilletCurDlg::OnRadio10() 
{
	// TODO: Add your control notification handler code here
	m_type = 9 ;
	ResetRBs() ;
}