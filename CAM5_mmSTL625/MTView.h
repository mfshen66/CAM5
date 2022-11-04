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

#pragma once

#include "ViewTree.h"

class CMTViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CMTView : public CDockablePane
{
// 构造
public:
	CMTView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// 特性
protected:

	CViewTree m_wndTree;
	CImageList m_images;
	CViewTree* m_pViewTree ; // 不负责释放该CViewTree
	CMTViewToolBar m_wndToolBar;
	void* m_pObj ; // 弹出右键菜单时激活的obj, nt add 2010/11/29

protected:
	void FillMTView();

// 实现
public:
	virtual ~CMTView();
	void OnActivate() ; // nt add 2010/9/8
	CViewTree* GetViewTree() ; // nt add 2020/5/12
	void SetViewTree(CViewTree* pViewTree) ; // nt add 2020/5/12

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnEditDelete();
	afx_msg void OnEditColor();
	afx_msg void OnEditWidth();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	CView* GetView() ; // nt add 2010/11/29

	DECLARE_MESSAGE_MAP()
};

