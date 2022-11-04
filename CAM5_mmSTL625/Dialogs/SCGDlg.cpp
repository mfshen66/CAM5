// CCGDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "SCGDlg.h"
#include "afxdialogex.h"


// CSCGDlg 对话框

IMPLEMENT_DYNAMIC(CSCGDlg, CDialogEx)

CSCGDlg::CSCGDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSCGDlg::IDD, pParent)
{
	m_size = 0 ;
	m_n = 0 ;
	m_ps = NULL ;
	m_max = 0. ;
	m_min = 0. ;
	m_umin = 0. ;
	m_umax = 1. ;
	m_vmin = 0. ;
	m_vmax = 1. ;
	m_grid = 1 ;
	m_nu = 11 ;
	m_nv = 11 ;
	m_surface = NULL ;
	m_uvflag = IDU ;
	m_u = 0. ;
	m_v = 0. ;
	m_type = 0 ;
	m_ns = 100 ;
	m_mx = 0. ; // nt add 2011/4/22
	m_my = 0. ; // nt add 2011/4/22
	m_win.w = 0 ;
	m_win.h = 0 ;
	// 计算一点处的曲率等信息，nt add 2011/4/23
	m_s = 0. ;
	m_t = 0. ;
	m_k1 = 0. ;
	m_k2 = 0. ;
	m_a1 = 0. ;
	m_a2 = 0. ;
	// end
}

CSCGDlg::~CSCGDlg()
{
	if( m_ps )
		delete []m_ps ;
}

int CSCGDlg::Init(int size, SURFACE* surface, BOXUV* boxuv)
{
	if( size < 512 )
		m_size = 512 ;
	else
	if( size > 2048 )
		m_size = 2048 ;
	else
		m_size = size ;
	m_n = 0 ;
	m_ps = new double[m_size][2] ;
	if( surface == NULL )
		return 0 ;
	m_surface = surface ;
	if( boxuv )
	{
		m_umin = boxuv->min[0] ;
		m_umax = boxuv->max[0] ;
		m_vmin = boxuv->min[1] ;
		m_vmax = boxuv->max[1] ;
	}

	return 1 ;
}

void CSCGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	DDX_Text(pDX, IDC_EDIT7, m_ns);
	DDV_MinMaxInt(pDX, m_ns, 10, 1000);
	DDX_Text(pDX, IDC_EDIT8, m_nu);
	DDV_MinMaxInt(pDX, m_nu, 0, 100);
	DDX_Text(pDX, IDC_EDIT9, m_nv);
	DDV_MinMaxInt(pDX, m_nv, 0, 100);
	DDX_Check(pDX, IDC_CHECK1, m_grid);
	DDX_Text(pDX, IDC_EDIT10, m_mx);
	DDV_MinMaxDouble(pDX, m_mx, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT11, m_my);
	DDV_MinMaxDouble(pDX, m_my, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT12, m_s);
	DDV_MinMaxDouble(pDX, m_s, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT13, m_t);
	DDV_MinMaxDouble(pDX, m_t, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT14, m_k1);
	DDV_MinMaxDouble(pDX, m_k1, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT15, m_k2);
	DDV_MinMaxDouble(pDX, m_k2, -1000000, 1000000);
	DDX_Text(pDX, IDC_EDIT16, m_a1);
	DDV_MinMaxDouble(pDX, m_a1, 0, 360);
	DDX_Text(pDX, IDC_EDIT17, m_a2);
	DDV_MinMaxDouble(pDX, m_a2, 0, 360);
}


BEGIN_MESSAGE_MAP(CSCGDlg, CDialogEx)
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
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, OnDeltaposSpin4)
	ON_BN_CLICKED(IDC_BUTTON2, &CSCGDlg::OnApply)
	ON_BN_CLICKED(IDC_BUTTON3, &CSCGDlg::OnCalAtPt)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CSCGDlg::ResetRBs()
{
	CButton* pButton = NULL ;

	if( m_uvflag == IDU )
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO1) ;
		pButton->SetCheck(1) ;
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_RADIO2) ;
		pButton->SetCheck(1) ;
	}

	switch(m_type)
	{
	case 0:
		pButton = (CButton*)GetDlgItem(IDC_RADIO3) ;
		pButton->SetCheck(1) ;
		break ;
	case 1:
		pButton = (CButton*)GetDlgItem(IDC_RADIO4) ;
		pButton->SetCheck(1) ;
		break ;
	case 2:
		pButton = (CButton*)GetDlgItem(IDC_RADIO5) ;
		pButton->SetCheck(1) ;
		break ;
	case 3:
		pButton = (CButton*)GetDlgItem(IDC_RADIO6) ;
		pButton->SetCheck(1) ;
		break ;
	case 4:
		pButton = (CButton*)GetDlgItem(IDC_RADIO7) ;
		pButton->SetCheck(1) ;
		break ;
	case 5:
		pButton = (CButton*)GetDlgItem(IDC_RADIO8) ;
		pButton->SetCheck(1) ;
		break ;
	case 6:
		pButton = (CButton*)GetDlgItem(IDC_RADIO9) ;
		pButton->SetCheck(1) ;
		break ;
	case 7:
		pButton = (CButton*)GetDlgItem(IDC_RADIO10) ;
		pButton->SetCheck(1) ;
		break ;
	default:
		break ;
	}
}

// CSCGDlg 消息处理程序
void CSCGDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_uvflag = IDU ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_uvflag = IDV ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_type = 0 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_type = 1 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_type = 2 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio6() 
{
	// TODO: Add your control notification handler code here
	m_type = 3 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_type = 4 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_type = 5 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio9() 
{
	// TODO: Add your control notification handler code here
	m_type = 6 ;
	ResetRBs() ;
}

void CSCGDlg::OnRadio10() 
{
	// TODO: Add your control notification handler code here
	m_type = 7 ;
	ResetRBs() ;
}

void CSCGDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_u -= pNMUpDown->iDelta*(m_umax-m_umin)/10 ; // 10???
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CSCGDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_v -= pNMUpDown->iDelta*(m_vmax-m_vmin)/10 ; // 10???
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CSCGDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_nu -= pNMUpDown->iDelta*10 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CSCGDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_nv -= pNMUpDown->iDelta*10 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CSCGDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
}

BOOL CSCGDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ResetRBs() ;
	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSCGDlg::OnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE) ;
	Update() ;
	Invalidate(TRUE) ;
}

void CSCGDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDCtl == IDC_BUTTON1 &&
		m_n > 0 )
	{
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC) ;
		CRect rect(lpDrawItemStruct->rcItem) ;
		CPen pen(PS_SOLID, 0, RGB(0, 0, 255)) ;
		CPen *pOldPen = pDC->SelectObject(&pen) ;
		pDC->SelectStockObject(NULL_BRUSH) ;
		WIN2D win ;
		win.w = rect.right-rect.left ;
		win.h = rect.bottom-rect.top ;
		GetBox(win.min, win.max) ;
		m_win = win ; // nt add 2011/4/22
		m_min = win.min[1] ;
		m_max = win.max[1] ;
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

		// nt add 2011/4/21
		if( m_grid )
		{
			CPen penDot(PS_DOT, 0, RGB(100, 0, 0)) ;
			pDC->SelectObject(&penDot) ;
			int i, n ;
			double d ;
			if( m_nu > 2 )
			{
				n = m_nu-1 ; // 段数
				d = ((double)win.w)/n ;
				for( i = 0 ; i <= n ; i++ )
				{
					x = (int)(i*d+0.5) ;
					pDC->MoveTo(x, 0) ;
					pDC->LineTo(x, win.h) ;
				}
			}
			if( m_nv > 2 )
			{
				n = m_nv-1 ; // 段数
				d = ((double)win.h)/n ;
				for( i = 0 ; i <= n ; i++ )
				{
					y = (int)(i*d+0.5) ;
					pDC->MoveTo(0, y) ;
					pDC->LineTo(win.w, y) ;
				}
			}
		}
		TCHAR buf[64] ;
		_stprintf_s(buf, _T("最小值 = %lf"), m_min) ;
		pDC->TextOut(25, 25, buf) ;
		_stprintf_s(buf, _T("最大值 = %lf"), m_max) ;
		pDC->TextOut(25, 50, buf) ;

		// nt add 2011/4/24 draw x axis
		int halfW = win.w/2, halfH = win.h/2 ;
		CPen pen2(PS_SOLID, 1, RGB(255, 0, 0)) ;
		pDC->SelectObject(&pen2) ;

		pDC->MoveTo(0, halfH) ;
		pDC->LineTo(win.w, halfH) ;
		double dd = (double)win.w/10 ;
		for( int i = 0 ; i <= 10 ; i++ )
		{
			x = (int)(dd*i+0.5) ;
			pDC->MoveTo(x, halfH) ;
			pDC->LineTo(x, halfH+10) ;
		}
		dd = (double)win.w/100 ;
		for( int i = 0 ; i <= 100 ; i++ )
		{
			x = (int)(dd*i+0.5) ;
			pDC->MoveTo(x, halfH) ;
			pDC->LineTo(x, halfH+5) ;
		}

		pDC->MoveTo(0, 0) ;
		pDC->LineTo(0, win.h) ;
		dd = (double)win.h/10 ;
		for( int i = 0 ; i <= 10 ; i++ )
		{
			y = (int)(dd*i+0.5) ;
			pDC->MoveTo(0, y) ;
			pDC->LineTo(10, y) ;
		}
		dd = (double)win.h/100 ;
		for( int i = 0 ; i <= 100 ; i++ )
		{
			y = (int)(dd*i+0.5) ;
			pDC->MoveTo(0, y) ;
			pDC->LineTo(5, y) ;
		}
		// end

		pDC->SelectObject(pOldPen) ;
	}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct) ;

	return ;
}

void CSCGDlg::AddPt(double x, double y)
{
	if( m_n < m_size )
	{
		m_ps[m_n][0] = x ;
		m_ps[m_n][1] = y ;
		m_n++ ;
	}
}

void CSCGDlg::GetBox(double min[2], double max[2])
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

void CSCGDlg::Clear()
{
	m_n = 0 ;
}

void CSCGDlg::SetMax(double max)
{
	m_max = max ;
}

void CSCGDlg::SetMin(double min)
{
	m_min = min ;
}

// nt add 2011/4/21
int curEval(VEC3D du,
	        VEC3D dv,
			VEC3D duu,
			VEC3D duv,
			VEC3D dvv,
		    VEC3D normal,
			VEC3D pri_dir1,
		    VEC3D pri_dir2,
		    double* pcur1,
			double* pcur2)
{
	int k, rt_value ;
    double E, F, G, L, M, N, ks[2], rs[2], a, b, c ;

	// curvatures : 1st and 2st foundamental form
    E = mathOProduct(du, du) ;
    F = mathOProduct(du, dv) ;
    G = mathOProduct(dv, dv) ;
    L = mathOProduct(normal, duu) ;
    M = mathOProduct(normal, duv) ;
    N = mathOProduct(normal, dvv) ;
    if( E*G-F*F < 1.e-20 )
		goto ExceExit ;

	a = E*G-F*F ;
	b = 2.0*F*M-G*L-E*N ;
	c = L*N-M*M ;
	if( mathSolQuadEqua(a,
						b,
						c,
						1.e-10,
						ks) == IDSOLU0 )
	{ // nt add 2011/4/24, 目的是解决无解(但实际应该有)的情况
		if( fabs(a) < 1.e-20 )
			goto ExceExit ;
		ks[0] = ks[1] = -b/(2*a) ;
	}
	if( ks[0] > ks[1] ) // nt add 2011/4/24
	{
		c = ks[0] ;
		ks[0] = ks[1] ;
		ks[1] = c ;
	}
	*pcur1 = ks[0] ;
	*pcur2 = ks[1] ;

	// principal directions
	if( fabs(ks[0]-ks[1]) < 1.e-5*fabs(ks[0]*ks[1]) )
	{ // 脐点
		memcpy(pri_dir1, du, sizeof(VEC3D)) ;
		if( mathUniVec(pri_dir1, 1.e-20) != ERSUCSS )
			goto ExceExit ;
		mathVProduct(normal, pri_dir1, pri_dir2) ;
		if( mathUniVec(pri_dir2, 1.e-20) != ERSUCSS )
			goto ExceExit ;
	}
	else
	{
		rt_value = mathSolHomoEqua22(ks[0]*E-L, // pri_dir1
									 ks[0]*F-M,
									 ks[0]*F-M,
									 ks[0]*G-N,
									 MIN_LEN, 
									 rs) ;
		if( rt_value == IDSOLU0 )
			goto ExceExit ;
		if( rt_value == IDINF ) // rs[0] = rs[1] = 0.0
			rs[0] = 1.0 ;
		for( k = 0 ; k < 3 ; k++ )
			pri_dir1[k] = rs[0]*du[k]+rs[1]*dv[k] ;
		if( mathUniVec(pri_dir1, 1.e-20) != ERSUCSS )
			goto ExceExit ;
		rt_value = mathSolHomoEqua22(ks[1]*E-L, // pri_dir2
									 ks[1]*F-M,
									 ks[1]*F-M,
									 ks[1]*G-N,
									 1.e-5,
									 rs) ;
		if( rt_value == IDSOLU0 )
			goto ExceExit ;
		if( rt_value == IDINF ) // rs[0] = rs[1] = 0.0
			rs[1] = 1.0 ;
		for( k = 0 ; k < 3 ; k++ )
			pri_dir2[k] = rs[0]*du[k]+rs[1]*dv[k] ;
		if( mathUniVec(pri_dir2, 1.e-20) != ERSUCSS )
			goto ExceExit ;
	}

	return ERSUCSS ;
ExceExit:
	pri_dir1[0] = 0. ;
	pri_dir1[1] = 0. ;
	pri_dir1[2] = 0. ;
	pri_dir2[0] = 0. ;
	pri_dir2[1] = 0. ;
	pri_dir2[2] = 0. ;
	*pcur1 = 0. ;
	*pcur2 = 0. ;
	return ERUNSUC ;
}

// 计算要显示的图形
void CSCGDlg::Update()
{
	int i ;
	double t, k1, k2, y, I, II ;
	PNT3D p ;
	VEC3D normal, dir1, dir2, du, dv, duu, duv, dvv ;
	INTERVAL interval ;

	m_n = 0 ;

	// 
	if( m_uvflag == IDU )
	{
		interval.t1 = m_vmin+0.0001*(m_vmax-m_vmin) ;
		interval.t2 = m_vmax-0.0001*(m_vmax-m_vmin) ;
	}
	else
	{
		interval.t1 = m_umin+0.0001*(m_umax-m_umin) ;
		interval.t2 = m_umax-0.0001*(m_umax-m_umin) ;
	}
	for( i = 0 ; i <= m_ns ; i++ )
	{
		t = interval.t1+i*(interval.t2-interval.t1)/m_ns ;
		y = 0. ;
		surfaceEvalDer2O(m_surface,
						 m_uvflag==IDU?m_u:t,
						 m_uvflag==IDU?t:m_v,
						 p,
						 du,
						 dv,
						 duu,
						 duv,
						 dvv) ;
		mathVProduct(du, dv, normal) ;
		if( mathUniVec(normal, MIN_DBL) == ERSUCSS )
		{
			switch(m_type)
			{
			case 0: // k1
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				y = k1 ;
				break ;
			case 1: // k2
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				y = k2 ;
				break ;
			case 2:
				I = du[0]*du[0]+du[1]*du[1]+du[2]*du[2] ;
				II = duu[0]*normal[0]+duu[1]*normal[1]+duu[2]*normal[2] ;
				y = II/I ;
				break ;
			case 3:
				I = dv[0]*dv[0]+dv[1]*dv[1]+dv[2]*dv[2] ;
				II = dvv[0]*normal[0]+dvv[1]*normal[1]+dvv[2]*normal[2] ;
				y = II/I ;
				break ;
			case 4: // k1*k2
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				y = k1*k2 ;
				break ;
			case 5: // 0.5*(k1+k2)
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				y = 0.5*(k1+k2) ;
				break ;
			case 6:
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				if( mathUniVec(du, MIN_DBL) == ERSUCSS )
					y = mathGetAngleUnit(dir1, du)*180/PI1 ;
				break ;
			case 7:
				curEval(du, dv, duu, duv, dvv, normal, dir1, dir2, &k1, &k2) ;
				if( mathUniVec(dv, MIN_DBL) == ERSUCSS )
					y = mathGetAngleUnit(dir1, dv)*180/PI1 ;
				break ;
			}
		}
		AddPt(t, y) ;
	}
}

void CSCGDlg::OnMM(int x) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_n > 0 &&
		m_win.w > 0 &&
		m_win.h > 0 )
	{
		double d ;
		d = (double)x/m_win.w ;
		if( d < 0.005 )
			d = 0. ;
		else
		if( d > 0.995 )
			d = 1. ;
		m_mx = m_win.min[0]+d*(m_win.max[0]-m_win.min[0]) ;
		m_my = 0. ;
		for( int i = 1 ; i <= m_n ; i++ )
		{
			if( m_ps[i-1][0]-MIN_DBL < m_mx &&
				m_mx < m_ps[i][0]+MIN_DBL )
			{
				m_my = ((m_mx-m_ps[i][0])*m_ps[i-1][1]-
					    (m_mx-m_ps[i-1][0])*m_ps[i][1])/
						(m_ps[i-1][0]-m_ps[i][0]);
				break ;
			}
		}
	}
	else
	{
		m_mx = 0. ;
		m_my = 0. ;
	}
	UpdateData(FALSE) ;
}

BOOL CSCGDlg::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_MOUSEMOVE )
    {
		CWnd* pWnd = GetDlgItem(IDC_BUTTON1) ;
		if( pMsg->hwnd == pWnd->m_hWnd )
		{
			RECT rect ;
			pWnd->GetWindowRect(&rect) ;
			int x = pMsg->pt.x-rect.left ;
			if( x > 0 )
				OnMM(x) ;
		}
	}

    return CDialogEx::PreTranslateMessage(pMsg);
}

// nt add 2011/4/23
void CSCGDlg::OnCalAtPt()
{
	PNT3D p ;
	VEC3D normal, dir1, dir2, du, dv, duu, duv, dvv ;

	if( m_surface )
	{
		UpdateData(TRUE) ;
		if( m_s < m_umin )
			m_s = m_umin ;
		else
		if( m_s > m_umax )
			m_s = m_umax ;
		if( m_t < m_vmin )
			m_t = m_vmin ;
		else
		if( m_t > m_vmax )
			m_t = m_vmax ;
		surfaceEvalDer2O(m_surface,
						 m_s,
						 m_t,
						 p,
						 du,
						 dv,
						 duu,
						 duv,
						 dvv) ;
		mathVProduct(du, dv, normal) ;
		if( mathUniVec(normal, MIN_DBL) == ERSUCSS )
		{
			curEval(du, 
				    dv, 
					duu, 
					duv, 
					dvv,
					normal, 
					dir1,
					dir2,
					&m_k1,
					&m_k2) ;
			if( mathUniVec(du, MIN_DBL) == ERSUCSS )
				m_a1 = mathGetAngleUnit(dir1, du)*180/PI1 ;
			if( mathUniVec(dv, MIN_DBL) == ERSUCSS )
				m_a2 = mathGetAngleUnit(dir1, dv)*180/PI1 ;
		}
		UpdateData(FALSE) ;
	}
}