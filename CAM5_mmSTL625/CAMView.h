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

// CAMView.h : CCAMView 类的接口
//

#pragma once

class CCommand ;
class CCAMView : public CView
{
protected: // 仅从序列化创建
	CCAMView();
	DECLARE_DYNCREATE(CCAMView)

// 特性
public:
	CCAMDoc* GetDocument() const;

	int m_drawMode ; // nt 2000/5/13
	CCommand* m_pCmd ; // nt 2000/5/14
	void* m_pVI ; // nt 2000/5/13
	void* m_glf ; // nt add 2009/7/24, for testing
	PNT3D m_p ; // previous lbutton down position, nt add 2003/5/31
	// for tool motion simulation, nt add 2010/4/1
	UINT m_dt1 ; // 更新CSim的时间间隔
	UINT m_t1 ;
	UINT m_dt2 ; // 重新绘制的时间间隔
	UINT m_t2 ;
	CSim* m_pSim ;
	CSim5* m_pSim5 ; // nt add 2010/6/2
	CTool* m_pTool ; // 临时
	GCODETPATH* m_pGT ; // 临时

// 操作
public:
	int GetDrawMode() ; // nt 2000/5/13
	void* GetVI() ; // nt 2000/5/13
	void ClearCmd() ; // nt 2000/10/7

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

// 实现
public:
	virtual ~CCAMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewPan();
	afx_msg void OnUpdateViewPan(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomByMouse();
	afx_msg void OnUpdateViewZoomByMouse(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomByBox();
	afx_msg void OnUpdateViewZoomByBox(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomAll();
	afx_msg void OnViewZoomOut();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewRotate();
	afx_msg void OnUpdateViewRotate(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnViewParallel();
	afx_msg void OnViewSelect();
	afx_msg void OnViewXy();
	afx_msg void OnViewYz();
	afx_msg void OnViewZx();
	afx_msg void OnViewIso2();
	afx_msg void OnViewPlane();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFileGCode();
	afx_msg void OnViewSim();
	afx_msg void OnViewSim5();
	afx_msg void OnViewStop();
	afx_msg void OnViewDispOnOff();
	afx_msg void OnViewTri();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CAMView.cpp 中的调试版本
inline CCAMDoc* CCAMView::GetDocument() const
   { return reinterpret_cast<CCAMDoc*>(m_pDocument); }
#endif

