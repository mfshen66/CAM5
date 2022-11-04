#include "stdafx.h"
#include "DlgPane.h"

////////////////////////////////////////////////////////////////
// CDlgPane : a contaner of CDialogEx
// nt 2010/8/21
CDlgPane::CDlgPane(CDialogEx* pDlg, UINT ID)
{
	m_pDlg = pDlg ;
	m_ID = ID ;
}

CDlgPane::~CDlgPane()
{
}
//--------------------------------------------------------------
CDialogEx* CDlgPane::GetDlg()
{
	return m_pDlg ;
}

UINT CDlgPane::GetID()
{
	return m_ID ;
}
//--------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDlgPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()
//--------------------------------------------------------------
// CWorkspaceBar 消息处理程序
int CDlgPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CDockablePane::OnCreate(lpCreateStruct) == -1 )
		return -1 ;

	// create m_pDlg
	if( m_pDlg )
	{
		if( !m_pDlg->Create(m_ID, this) )
			return -1 ; // 未能创建
		m_pDlg->ShowWindow(SW_SHOW) ;
		AdjustLayout() ;
	}

	return 0 ;
}

void CDlgPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy) ;
	AdjustLayout() ;
}

void CDlgPane::AdjustLayout()
{
	if( GetSafeHwnd() == NULL )
		return ;

	CRect rectClient ;
	GetClientRect(rectClient) ;

	m_pDlg->SetWindowPos(NULL, 
						 rectClient.left+1,
						 rectClient.top+1,
						 rectClient.Width()-2,
						 rectClient.Height()-2,
						 SWP_NOACTIVATE|SWP_NOZORDER) ;
}

void CDlgPane::OnPaint()
{
	CPaintDC dc(this) ; // 用于绘制的设备上下文

	CRect rect ;
	m_pDlg->GetWindowRect(rect) ;
	ScreenToClient(rect) ;

	rect.InflateRect(1, 1) ;
	dc.Draw3dRect(rect, 
				  ::GetSysColor(COLOR_3DSHADOW), 
				  ::GetSysColor(COLOR_3DSHADOW)) ;
}
////////////////////////////////////////////////////////////////

