// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\CAM.h"
#include "WaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog
CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWaitDlg::IDD, pParent)
{
	m_prompt = "µÈ´ýÊ°È¡..." ;
}

CWaitDlg::CWaitDlg(CString& prompt)
	: CDialogEx(CWaitDlg::IDD, NULL)
{
	m_prompt = prompt ;
}

BEGIN_MESSAGE_MAP(CWaitDlg, CDialogEx)
	//{{AFX_MSG_MAP(CWaitDlg)
	ON_COMMAND(IDAPPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers
BOOL CWaitDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	// TODO: Add extra initialization here
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC) ;
	pStatic->SetWindowText(m_prompt) ;

	SetBackgroundColor(RGB(200, 200, 255)) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitDlg::OnOK()
{
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CWaitDlg::OnCancel()
{
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

void CWaitDlg::OnApply()
{
	UpdateData(TRUE) ;
	cmdDlgUI(2) ;
}