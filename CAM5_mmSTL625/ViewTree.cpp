// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

#include "stdafx.h"
#include "CAM.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "ViewTree.h"
#include "CAMDoc.h"
#include "CAMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CViewTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hOld = pNMTreeView->itemOld.hItem ;
	HTREEITEM hNew = pNMTreeView->itemNew.hItem ;
	SHORT ctrlKey = GetKeyState(VK_CONTROL) ;
	// CTRL键按下, 保留上一选项的选中状态
	if( hOld && 
		(ctrlKey&0xF0) )
		SetItemState(hOld, TVIS_SELECTED, TVIS_SELECTED) ;
	if( hNew )
		SetItemState(hNew, TVIS_SELECTED, TVIS_SELECTED) ;
	else
		goto Exit ;

	// nt add 2010/9/9, 为与CSelSet保持同步...
	CMainFrame *pWndMain = (CMainFrame*)AfxGetMainWnd() ;
	CMDIChildWndEx* temp = (CMDIChildWndEx*)pWndMain->MDIGetActive() ;
	CChildFrame* pChild = (CChildFrame*)temp ;
    if( !pChild )
		goto Exit ;
    CCAMView* pView = (CCAMView*)pChild->GetActiveView() ;
    if( !pView )
		goto Exit ;
	CCAMDoc *pDoc = (CCAMDoc*)(pView->GetDocument()) ;
	if( !pDoc )
		goto Exit ;
	CViewTree* pT = pDoc->GetTree() ;
	CSelSet* pSelSet = pDoc->GetSelSet() ;
	if( pT == this &&
		pSelSet )
	{
		if( !(ctrlKey&0xF0) )
			pSelSet->Clear() ;
		CVO* pVO = (CVO*)GetItemData(hNew) ;
		pSelSet->AddSelected(pVO) ;
		glFlush() ;
	}

Exit:
	*pResult = 0;
}
