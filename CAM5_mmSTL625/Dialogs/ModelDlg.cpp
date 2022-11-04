// ModelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h"
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "CPDlg.h"
#include "ModelDlg.h"
#include "afxdialogex.h"


// CModelDlg 对话框

IMPLEMENT_DYNAMIC(CModelDlg, CDialogEx)

CModelDlg::CModelDlg(CCAMDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelDlg::IDD, pParent)
{
	m_pDoc = pDoc ;
	m_gm = 0 ;
}

CModelDlg::~CModelDlg()
{
}

void CModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CModelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CModelDlg::OnClickedMfcbutton1)
	ON_BN_CLICKED(IDC_MFCBUTTON2, &CModelDlg::OnClickedMfcbutton2)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CModelDlg::OnClickedMfcbutton3)
	ON_BN_CLICKED(IDC_MFCBUTTON4, &CModelDlg::OnClickedMfcbutton4)
	ON_COMMAND(IDC_RADIO1, &CModelDlg::OnRadio1)
	ON_COMMAND(IDC_RADIO2, &CModelDlg::OnRadio2)
END_MESSAGE_MAP()


// CModelDlg 消息处理程序
BOOL CModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* pB ;
	if( m_gm == 0 )
		pB = (CButton*)GetDlgItem(IDC_RADIO1) ; // CStep workpiece
	else
		pB = (CButton*)GetDlgItem(IDC_RADIO2) ; // CStep part
	pB->SetCheck(1) ;

	SetBackgroundColor(RGB(200, 200, 255)) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// 增加几何
void CModelDlg::OnClickedMfcbutton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( 	m_pDoc )
	{
		CPart* pPart = m_pDoc->GetPart() ;
		CMill* pMill = m_pDoc->GetMill() ;
		if( pPart &&
			pMill )
		{
			int i, n = m_pDoc->GetSelected(ftRefSur, 10000) ;
			for( i = 0 ; i < n ; i++ )
			{
				CFeature* pF = (CFeature*)m_pDoc->GetObj(i) ;
				if( pF )
				{
					int no = pF->GetNo() ;
					pMill->AddGeometry(m_gm, no) ;
				}
			}
		}
	}
}

// 减少几何
void CModelDlg::OnClickedMfcbutton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if( 	m_pDoc )
	{
		CPart* pPart = m_pDoc->GetPart() ;
		CMill* pMill = m_pDoc->GetMill() ;
		if( pPart &&
			pMill )
		{
			int i, n = m_pDoc->GetSelected(ftRefSur, 10000) ;
			for( i = 0 ; i < n ; i++ )
			{
				CFeature* pF = (CFeature*)m_pDoc->GetObj(i) ;
				if( pF )
				{
					int no = pF->GetNo() ;
					pMill->DeleteGeometry(m_gm, no) ;
				}
			}
		}
	}
}

// 显示
void CModelDlg::OnClickedMfcbutton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if( 	m_pDoc )
	{
		CPart* pPart = m_pDoc->GetPart() ;
		CSelSet* pSelSet = m_pDoc->GetSelSet() ;
		CMill* pMill = m_pDoc->GetMill() ;
		if( pPart &&
			pSelSet &&
			pMill )
		{
			int i, n = pMill->GetNumOfGeometry(m_gm) ;
			for( i = 0 ; i < n ; i++ )
			{
				int no = pMill->GetGeometryNo(m_gm, i) ;
				CFeature* pF = pPart->GetFeatureByNo(no) ;
				if( pF )
					pSelSet->AddSelected(pF) ;
			}
		}
	}
}

// 清除几何模型
void CModelDlg::OnClickedMfcbutton4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( m_pDoc )
	{
		CMill* pMill = m_pDoc->GetMill() ;
		if( pMill )
			pMill->ClearModel(m_gm) ;
		CSelSet* pSelSet = m_pDoc->GetSelSet() ;
		if( pSelSet )
			pSelSet->Clear() ;
	}
}

void CModelDlg::OnRadio1()
{
	// TODO: 在此添加命令处理程序代码
	m_gm = 0 ;
}

void CModelDlg::OnRadio2()
{
	// TODO: 在此添加命令处理程序代码
	m_gm = 1 ;
}

void CModelDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
	cmdDlgUI(1) ;
}

void CModelDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}
