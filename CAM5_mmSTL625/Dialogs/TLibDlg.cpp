// TLibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\CAM.h"
#include "..\Models.h"
#include "..\Mills.h"
#include "..\SelSet.h"
#include "..\CAMDoc.h"
#include "TLibDlg.h"
#include "afxdialogex.h"
#include "FlatToolDlg.h"
#include "BallToolDlg.h"
#include "BarrelToolDlg.h"

// CTLibDlg 对话框

IMPLEMENT_DYNAMIC(CTLibDlg, CDialogEx)

CTLibDlg::CTLibDlg(CCAMDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialogEx(CTLibDlg::IDD, pParent)
{
	m_pDoc = pDoc ; // nt add 2010/9/27
	m_pTG = NULL ;
	m_pTSG = NULL ;
	m_pTool = NULL ;
}

CTLibDlg::~CTLibDlg()
{
}

void CTLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTLibDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTLibDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTLibDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTLibDlg::OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTLibDlg::OnClickedButton4)
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CTLibDlg::OnClickedMfcButton1)
END_MESSAGE_MAP()


// CTLibDlg 消息处理程序
// 添加
void CTLibDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = GetSelected(), hTSG, hT ;
		if( hItem )
		{
			if( m_pTG && // 选中了子组或刀具
				m_pTSG )
			{
				int subType = m_pTSG->GetSubType() ;
				hTSG = m_pTSG->GetItem() ;
				if( hTSG == 0 )
					return ;
				if( subType == toolSubTypeFlat )
				{
					CFlatToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
						CTool* pTool = new CTool() ;
						pTool->CreateFlat(5., 10.) ;
						// 设置pTool的其他参数...
						m_pTSG->Add(pTool) ;
						CString name ;
						pTool->GetName(name) ;
						hT = pTree->InsertItem(name, 0, 0, hTSG) ;
						pTree->SetItemData(hT, (DWORD_PTR)pTool) ;
						pTool->SetItem(hT) ;
						pTLib->SetModified() ;
						pTree->Invalidate(TRUE) ;
						return ;
					}
				}
				else
				if( subType == toolSubTypeBall )
				{
					CBallToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
						CTool* pTool = new CTool() ;
						pTool->CreateBall(5., 10.) ;
						// 设置pTool的其他参数...
						m_pTSG->Add(pTool) ;
						CString name ;
						pTool->GetName(name) ;
						hT = pTree->InsertItem(name, 1, 1, hTSG) ;
						pTree->SetItemData(hT, (DWORD_PTR)pTool) ;
						pTool->SetItem(hT) ;
						pTLib->SetModified() ;
						pTree->Invalidate(TRUE) ;
						return ;
					}
				}
				else
				if( subType == toolSubTypeTorus )
				{
				}
				else
				if( subType == toolSubTypeBarrel )
				{
					CBarrelToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
					}
				}
			}
		}
	}
}

// 删除
void CTLibDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = GetSelected() ;
		if( hItem )
		{
			if( m_pTG )
			{
				if( AfxMessageBox(_T("确认删除?"), MB_YESNO) == IDYES )
				{
					if(	m_pTSG == NULL ) // 删除组CTGroup:m_pTG
					{
						pTLib->Remove(m_pTG) ;
						delete m_pTG ;
					}
					else
					if( m_pTool == NULL ) // 删除子组CTSubGroup:m_pTSG
					{
						m_pTG->Remove(m_pTSG) ;
						delete m_pTSG ;
					}
					else // 删除刀具CTool:m_pTool
					{
						m_pTSG->Remove(m_pTool) ;
						delete m_pTool ;
					}
					pTLib->SetModified() ;
					pTree->DeleteItem(hItem) ;
					pTree->Invalidate(TRUE) ;
				}
			}
		}
	}
}

// 拷贝
void CTLibDlg::OnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = GetSelected(), hTSG, hT ;
		if( hItem )
		{
			if( m_pTG && // 选中了刀具
				m_pTSG &&
				m_pTool )
			{
				hTSG = m_pTSG->GetItem() ;
				if( hTSG == 0 )
					return ;
				CTool* pTool = m_pTool->Copy() ;
				m_pTSG->Add(pTool) ;
				CString name ;
				pTool->GetName(name) ;
				hT = pTree->InsertItem(name, 0, 0, hTSG) ;
				pTree->SetItemData(hT, (DWORD_PTR)pTool) ;
				pTool->SetItem(hT) ;
				pTLib->SetModified() ;
				pTree->Invalidate(TRUE) ;
				return ;
			}
		}
	}
}

// 编辑
void CTLibDlg::OnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = GetSelected(), hTSG ;
		if( hItem )
		{
			if( m_pTG && // 选中了刀具
				m_pTSG &&
				m_pTool )
			{
				int subType = m_pTSG->GetSubType() ;
				hTSG = m_pTSG->GetItem() ;
				if( hTSG == 0 )
					return ;
				if( subType == toolSubTypeFlat )
				{
					CFlatToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
						// 设置m_pTool的其他参数...
						m_pTool->Recreate() ;
						pTLib->SetModified() ;
						return ;
					}
				}
				else
				if( subType == toolSubTypeBall )
				{
					CBallToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
						// 设置m_pTool的其他参数...
						m_pTool->Recreate() ;
						pTLib->SetModified() ;
						return ;
					}
				}
				else
				if( subType == toolSubTypeTorus )
				{
				}
				else
				if( subType == toolSubTypeBarrel )
				{
					CBarrelToolDlg dlg ;
					if( dlg.DoModal() == IDOK )
					{
						// 设置m_pTool的其他参数...
						m_pTool->Recreate() ;
						pTLib->SetModified() ;
						return ;
					}
				}
			}
		}
	}
}

// nt add 2010/9/27
// 选择当前刀具
void CTLibDlg::OnClickedMfcButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = GetSelected() ;
		if( hItem )
		{
			if( m_pTG && // 选中了刀具
				m_pTSG &&
				m_pTool )
			{
				if( m_pDoc )
				{
					CMill* pMill = m_pDoc->GetMill() ;
					if( pMill )
					{
						CTool* pTool = m_pTool->Copy() ;
						pMill->SetTool(pTool) ;
					}
				}
			}
		}
	}
}

BOOL CTLibDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_images.Create(IDB_TOOLLIB, 16, 0, RGB(255, 0, 255)) ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	CTLib* pTLib = (CTLib*)GetTLib() ;
	if( pTree &&
		pTLib )
	{
		pTree->SetImageList(&m_images, TVSIL_NORMAL) ;
		HTREEITEM hRoot = pTree->InsertItem(_T("刀具库"), 4, 4);
		pTree->SetItemData(hRoot, 0) ;
		pTree->SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

		CString name ;
		HTREEITEM hTG, hTSG, hT ;
		int i, n = pTLib->GetNumOfGroup() ;
		for( i = 0 ; i < n ; i++ )
		{
			CTGroup* pTG = pTLib->GetGroup(i) ;
			if( pTG )
			{
				pTG->GetName(name) ;
				hTG = pTree->InsertItem(name, 4, 4, hRoot);
				pTree->SetItemData(hTG, (DWORD_PTR)pTG) ;
				pTG->SetItem(hTG) ;
				int j, m = pTG->GetNumOfSubGroup() ;
				for( j = 0 ; j < m ; j++ )
				{
					CTSubGroup* pTSG = pTG->GetSubGroup(j) ;
					if( pTSG )
					{
						pTSG->GetName(name) ;
						hTSG = pTree->InsertItem(name, 4, 4, hTG) ;
						pTree->SetItemData(hTSG, (DWORD_PTR)pTSG) ;
						pTSG->SetItem(hTSG) ;
						int k, s = pTSG->GetNumOfTool() ;
						for( k = 0 ; k < s ; k++ )
						{
							CTool* pTool = pTSG->GetTool(k) ;
							if( pTool )
							{
								pTool->GetName(name) ;
								int index = 0, subType ;
								subType = pTool->GetSubType() ;
								if( subType == toolSubTypeFlat )
									index = 0 ;
								else
								if( subType == toolSubTypeBall )
									index = 1 ;
								else
								if( subType == toolSubTypeTorus )
									index = 2 ;
								else
								if( subType == toolSubTypeBarrel )
									index = 3 ;
								hT = pTree->InsertItem(name, index, index, hTSG) ;
								pTree->SetItemData(hT, (DWORD_PTR)pTool) ;
								pTool->SetItem(hT) ;
							}
						}
						pTree->Expand(hTSG, TVE_EXPAND) ;
					}
				}
				pTree->Expand(hTG, TVE_EXPAND) ;
			}
		}
		pTree->Expand(hRoot, TVE_EXPAND) ;
	}

	SetBackgroundColor(RGB(200, 200, 255)) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTLibDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
	cmdDlgUI(0) ;
}

HTREEITEM CTLibDlg::GetSelected()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pTG = NULL ;
	m_pTSG = NULL ;
	m_pTool = NULL ;
	CTLib* pTLib = (CTLib*)GetTLib() ;
	CTreeCtrl* pTree = (CTreeCtrl*)GetDlgItem(IDC_TREE1) ;
	if( pTLib &&
		pTree )
	{
		HTREEITEM hItem = pTree->GetSelectedItem() ;
		if( hItem )
		{
			int i, n = pTLib->GetNumOfGroup() ;
			for( i = 0 ; i < n ; i++ )
			{
				CTGroup* pTG = pTLib->GetGroup(i) ;
				if( pTG )
				{
					HTREEITEM hTG = pTG->GetItem() ;
					if( hTG == hItem )
					{
						m_pTG = pTG ;
						return hItem ;
					}
					int j, m = pTG->GetNumOfSubGroup() ;
					for( j = 0 ; j < m ; j++ )
					{
						CTSubGroup* pTSG = pTG->GetSubGroup(j) ;
						if( pTSG )
						{
							HTREEITEM hTSG = pTSG->GetItem() ;
							if( hTSG == hItem )
							{
								m_pTG = pTG ;
								m_pTSG = pTSG ;
								return hItem ;
							}
							int k, s = pTSG->GetNumOfTool() ;
							for( k = 0 ; k < s ; k++ )
							{
								CTool* pTool = pTSG->GetTool(k) ;
								if( pTool )
								{
									HTREEITEM hTool = pTool->GetItem() ;
									if( hTool == hItem )
									{
										m_pTG = pTG ;
										m_pTSG = pTSG ;
										m_pTool = pTool ;
										return hItem ;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0 ;
}
