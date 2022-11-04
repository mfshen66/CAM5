// CCGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "CCGDlg.h"
#include "afxdialogex.h"


// CCCGDlg 对话框

IMPLEMENT_DYNAMIC(CCCGDlg, CDialogEx)

CCCGDlg::CCCGDlg(int size, CWnd* pParent /*=NULL*/)
	: CDialogEx(CCCGDlg::IDD, pParent)
{
	m_size = size>1000?size:1000 ;
	m_n = 0 ;
	m_ps = new double[m_size][2] ;
	m_max = 0.0;
	m_min = 0.0;
}

CCCGDlg::~CCCGDlg()
{
	if( m_ps )
		delete []m_ps ;
}

void CCCGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_max);
	DDV_MinMaxDouble(pDX, m_max, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT2, m_min);
	DDV_MinMaxDouble(pDX, m_min, -1000000, 1000000);
}


BEGIN_MESSAGE_MAP(CCCGDlg, CDialogEx)
	ON_BN_CLICKED(IDAPPLY, &CCCGDlg::OnApply)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// CCCGDlg 消息处理程序


void CCCGDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}


void CCCGDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}


BOOL CCCGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCCGDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}

void CCCGDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDCtl == IDC_BUTTON1 &&
		m_n > 0 )
	{
		CDC  *pDC = CDC::FromHandle(lpDrawItemStruct->hDC) ;
		CRect rect(lpDrawItemStruct->rcItem) ;
		CPen pen(PS_SOLID, 0, RGB(0, 0, 255)) ;
		CPen *pOldPen = pDC->SelectObject(&pen) ;
		pDC->SelectStockObject(NULL_BRUSH) ;
		WIN2D win ;
		win.w = rect.right-rect.left ;
		win.h = rect.bottom-rect.top ;
		GetBox(win.min, win.max) ;
		double dH = 1.25*mathMAX(fabs(win.max[1]), fabs(win.min[1])) ;
		if( dH < 0.001 )
			dH = 0.001 ;
		win.min[1] = -dH ;
		win.max[1] = dH ;
		if( win.max[0]-win.min[0] < 0.001 )
			win.max[0] = win.min[0]+0.001 ;
		pDC->MoveTo(rect.left, (rect.top+rect.bottom)/2) ;
		pDC->LineTo(rect.right, (rect.top+rect.bottom)/2) ;
		int x, y ;
		win2dToIJ(&win, m_ps[0], x, y) ;
		pDC->MoveTo(rect.left+x, rect.top+y) ;
		for( int i = 1 ; i < m_n ; i++ )
		{
			win2dToIJ(&win, m_ps[i], x, y) ;
			pDC->LineTo(rect.left+x, rect.top+y) ;
		}
		pDC->SelectObject(pOldPen) ;
	}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct) ;

	return ;
}

void CCCGDlg::AddPt(double x, double y)
{
	if( m_n < m_size )
	{
		m_ps[m_n][0] = x ;
		m_ps[m_n][1] = y ;
		m_n++ ;
	}
}

void CCCGDlg::GetBox(double min[2], double max[2])
{
	for( int k = 0 ; k < 2 ; k++ )
	{
		min[k] = MAX_DBL ;
		max[k] = -MAX_DBL ;
		for( int i = 0 ; i < m_n ; i++ )
		{
			if( min[k] > m_ps[i][k] )
				min[k] = m_ps[i][k] ;
			if( max[k] < m_ps[i][k] )
				max[k] = m_ps[i][k] ;
		}
	}
}

void CCCGDlg::Clear()
{
	m_n = 0 ;
}

void CCCGDlg::SetMax(double max)
{
	m_max = max ;
}

void CCCGDlg::SetMin(double min)
{
	m_min = min ;
}

void CCCGDlg::Update()
{
	UpdateData(FALSE) ;
	Invalidate(TRUE) ;
}