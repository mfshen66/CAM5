// PicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "PicDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPicDlg dialog
static CString prevName ;

CPicDlg::CPicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicDlg)
	m_min = 0;
	m_max = 255;
	m_transparent = 0 ;
	m_tdeg = 255 ;
	m_fileName = prevName ;
	m_dpi = 300;
	//}}AFX_DATA_INIT
}


void CPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicDlg)
	DDX_Text(pDX, IDC_EDIT1, m_min);
	DDV_MinMaxInt(pDX, m_min, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_max);
	DDV_MinMaxInt(pDX, m_max, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_fileName);
	DDX_Text(pDX, IDC_EDIT4, m_dpi);
	DDV_MinMaxInt(pDX, m_dpi, 10, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPicDlg, CDialogEx)
	//{{AFX_MSG_MAP(CPicDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, OnDeltaposSpin3)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicDlg message handlers
void CPicDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_min -= pNMUpDown->iDelta*1 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CPicDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_max -= pNMUpDown->iDelta*1 ;
    UpdateData(FALSE) ;
	
	*pResult = 0;
}

void CPicDlg::OnOK() 
{
	// TODO: Add extra validation here
	prevName = m_fileName ;
	CSliderCtrl* pSliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	m_tdeg = pSliderCtrl->GetPos() ;
	
	CDialog::OnOK();
	cmdDlgUI(1) ;
}

void CPicDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilter[] = _T("BMP Files (*.bmp)|*.BMP|JPEG Files (*.jpg)|*.JPG|GIF Files (*.gif)|*.GIF|JPEG Files (*.jpeg)|*.JPEG||") ;
	CFileDialog dlg(TRUE, _T("*.bmp"), NULL, OFN_HIDEREADONLY, szFilter) ;

	if( dlg.DoModal() == IDOK )
	{
		m_fileName = dlg.GetPathName() ;
		UpdateData(FALSE) ;
	}

	return ;
}

void CPicDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_transparent = 1 ;
	m_tdeg = 255 ;
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1) ;
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0) ;
	CSliderCtrl* pSliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	pSliderCtrl->SetRange(0, 255) ;
	pSliderCtrl->SetPos(255) ; // nt add 2004/6/10
	pSliderCtrl->ShowWindow(TRUE) ;
}

void CPicDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE) ;
	m_dpi -= pNMUpDown->iDelta*50 ;
    UpdateData(FALSE) ;

	*pResult = 0;
}

void CPicDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_transparent = 0 ;
	m_tdeg = 255 ;
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0) ;
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1) ;
	CSliderCtrl* pSliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	pSliderCtrl->ShowWindow(FALSE) ;
}

BOOL CPicDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_transparent )
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1) ;
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0) ;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0) ;
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1) ;
	}

	CSliderCtrl* pSliderCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1) ;
	pSliderCtrl->ShowWindow(FALSE) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPicDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CPicDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}