// ScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "ScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog


CScaleDlg::CScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScaleDlg)
	m_scale = 0.001 ;
	//}}AFX_DATA_INIT
}


void CScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScaleDlg)
	DDX_Text(pDX, IDC_EDIT1, m_scale);
	DDV_MinMaxDouble(pDX, m_scale, -1000., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CScaleDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg message handlers

void CScaleDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_scale -= pNMUpDown->iDelta*0.1 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}
