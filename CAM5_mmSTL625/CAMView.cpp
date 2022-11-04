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

// CAMView.cpp : CCAMView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CAM.h"
#endif
#include "Models.h"
#include "Mills.h" // nt add 2010/4/1
#include "SelSet.h"
#include "ViewTree.h"
#include "CAMDoc.h"
#include "CAMView.h"
#include "Command.h"
#include "ViewCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////
// CCAMView
IMPLEMENT_DYNCREATE(CCAMView, CView)

BEGIN_MESSAGE_MAP(CCAMView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_VIEW_PAN, OnViewPan)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateViewPan)
	ON_COMMAND(ID_VIEW_ZOOMBYMOUSE, OnViewZoomByMouse)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMBYMOUSE, OnUpdateViewZoomByMouse)
	ON_COMMAND(ID_VIEW_ZOOMBYBOX, OnViewZoomByBox)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMBYBOX, OnUpdateViewZoomByBox)
	ON_COMMAND(ID_VIEW_ZOOMALL, OnViewZoomAll)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomOut)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ROTATE, OnViewRotate)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ROTATE, OnUpdateViewRotate)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_COMMAND(ID_VIEW_PARALLEL, OnViewParallel)
	ON_COMMAND(ID_VIEW_SELECT, OnViewSelect)
	ON_COMMAND(ID_VIEW_XY, OnViewXy)
	ON_COMMAND(ID_VIEW_YZ, OnViewYz)
	ON_COMMAND(ID_VIEW_ZX, OnViewZx)
	ON_COMMAND(ID_VIEW_ISO2, OnViewIso2)
	ON_COMMAND(ID_VIEW_PLANE, OnViewPlane)
	ON_COMMAND(ID_FILE_GCODE, OnFileGCode)
	ON_COMMAND(ID_VIEW_SIM, OnViewSim)
	ON_COMMAND(ID_VIEW_SIM5, OnViewSim5)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_DISPONOFF, OnViewDispOnOff)
	ON_COMMAND(ID_VIEW_TRI, OnViewTri)
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCAMView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()
//--------------------------------------------------------------
// CCAMView 构造/析构
CCAMView::CCAMView()
{
	// TODO: 在此处添加构造代码
	m_drawMode = dmNormal ;
	m_pCmd = NULL ;
	m_p[0] = m_p[1] = m_p[2] = 0.0 ; // nt add 2003/5/31
	m_glf = NULL ; // for testing only

	// nt add 2010/4/1
	m_dt1 = 50 ;
	m_t1 = 0 ;
	m_dt2 = 75 ;
	m_t2 = 0 ;
	m_pSim = NULL ;
	m_pSim5 = NULL ; // nt add 2010/6/2
	m_pTool = new CTool() ;
	//m_pTool->CreateFlatEnd(0.0025, 0.025) ;
	//m_pTool->CreateBallEnd(2.5, 25.) ;
	m_pTool->CreateTorus(10., 5., 5., 15., 50.) ;
	m_pGT = NULL ;
}

CCAMView::~CCAMView()
{
	if( m_pCmd )
		delete m_pCmd ;
	// nt add 2010/4/1
	if( m_pSim )
		delete m_pSim ;
	// nt add 2010/6/2
	if( m_pSim5 )
		delete m_pSim5 ;
	if( m_pTool )
		delete m_pTool ;
	if( m_pGT )
		gcodetpathFree(m_pGT) ;
}
//--------------------------------------------------------------
BOOL CCAMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	return CView::PreCreateWindow(cs);
}

int CCAMView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pVI = viCreate() ;
	viSetWnd(m_pVI, this) ;
	viSetInitZ(m_pVI, 10000000.) ; // 10000m, nt modify 2020/3/25 针对目前单位是mm
	viSetPixelSize(m_pVI, 0.5) ;
	RECT rect ;
	GetClientRect(&rect) ;
	int w = rect.right-rect.left,
		h = rect.bottom-rect.top ;
	viSetSize(m_pVI, w, h) ;
	SetTimer(1, 15, NULL) ;

	// nt add 2020/5/12
	camWake(m_pVI) ;
	m_glf = glfCreate(1, "宋体") ;
	CStr pathName = cfFillFullPathName("SLib.dat") ;
	glfLoadSyms(m_glf, pathName.GetBuffer(0)) ;
	viSetGLF(m_pVI, m_glf) ;
	// end
	camSleep(m_pVI) ;

	// nt add 2010/4/23
	CCAMDoc* pDoc = GetDocument() ;
	if( pDoc )
	{
		CSelSet* pSelSet = pDoc->GetSelSet() ;
		if( pSelSet )
			pSelSet->SetVI(m_pVI) ;
	}

	return 0 ;
}

void CCAMView::OnDestroy() 
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if( m_pVI )
	{
		camSleep(m_pVI) ; // nt add 2009/9/14
		if( m_glf )
			glfFree(m_glf) ;
		viFree(m_pVI) ;
	}

	KillTimer(1) ;

	return ;
}

void CCAMView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if( nType == SIZE_MINIMIZED ) // nt add 2001/10/16
        return ;

	// TODO: Add your message handler code here
	viOnSize(m_pVI, cx, cy) ;

	return ;
}

BOOL CCAMView::OnQueryNewPalette()
{
	BOOL nRet ;

	if( m_pVI )
	{
		viOnQueryNewPalette(m_pVI, &nRet) ;
		InvalidateRect(NULL, FALSE);
		return nRet;
	}

	return CView::OnQueryNewPalette();
}

void CCAMView::OnPaletteChanged(CWnd* pFocusWnd)
{
	if( pFocusWnd != this )
	{
		viOnPaletteChanged(m_pVI) ;
	}

	CView::OnPaletteChanged(pFocusWnd);
}

// nt add 2010/9/9
void CCAMView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类
	CCAMDoc* pDoc = GetDocument() ;
	if( pDoc )
		pDoc->OnActivate() ; // activate or inactivate
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//--------------------------------------------------------------
// CCAMView 绘制
// nt modify 2009/12/17
// viClearEx(m_pVI) can replace viClearBlack(m_pVI)
// viClear(m_pVI) can replace viClearBlack(m_pVI)
void CCAMView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此处为本机数据添加绘制代码
	CCAMDoc* pDoc = GetDocument();
	if( pDoc &&
		m_pVI )
	{
		// begin draw, make current glrc
		camWake(m_pVI) ; // nt add 2009/9/13
		glDrawBuffer(GL_BACK) ;
		viSetGL(m_pVI) ;
		viClearWhite(m_pVI) ; // nt add 2011/3/24

		// draw model
		pDoc->Draw(m_pVI, m_drawMode) ;
		if( m_drawMode == dmNormal )
			viDrawWorldFrame(m_pVI, RGB(255, 0, 0)) ;

		// nt add 2010/4/1
		if( m_pGT )
			gcodetpathDraw(m_pGT, m_pVI, dmNormal) ;
		// nt add 2010/4/1
		if( m_pSim )
			m_pSim->Draw(m_pVI) ;
		// nt add 2010/6/2
		if( m_pSim5 )
			m_pSim5->Draw(m_pVI) ;

		// end draw
		glFlush() ;
		SwapBuffers(wglGetCurrentDC()) ;
		glDrawBuffer(GL_FRONT) ;
	}

	return ;
}
//--------------------------------------------------------------
//	CCAMView attributes
int CCAMView::GetDrawMode()
{
	return m_drawMode ;
}

void* CCAMView::GetVI()
{
	return m_pVI ;
}

void CCAMView::ClearCmd()
{
	if( m_pCmd )
	{
		m_pCmd->Cancel() ;
		delete m_pCmd ;
		m_pCmd = NULL ;
	}
}
//--------------------------------------------------------------
// CCAMView 打印
void CCAMView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CCAMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCAMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCAMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}
//--------------------------------------------------------------
//void CCAMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}
// nt modify 2011/8/15 控制何时显示右键菜单
void CCAMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//#ifndef SHARED_HANDLERS
	CCAMDoc* pDoc = GetDocument() ;
	if( pDoc &&
		pDoc->HasCmd() == 0 )
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, 
													  point.x, 
													  point.y, 
													  this,
													  TRUE);
//#endif
}
//--------------------------------------------------------------
// CCAMView 诊断
#ifdef _DEBUG
void CCAMView::AssertValid() const
{
	CView::AssertValid();
}

void CCAMView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCAMDoc* CCAMView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCAMDoc)));
	return (CCAMDoc*)m_pDocument;
}
#endif //_DEBUG
//--------------------------------------------------------------
// CCAMView 消息处理程序
void CCAMView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	camWake(m_pVI) ; // nt add 2009/9/13
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		memcpy(m_p, p, sizeof(PNT3D)) ; // nt add 2003/5/31
		if( m_pCmd )
			m_pCmd->OnLButtonDown(nFlags, p, v) ;
		else
		{
			CCAMDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnLButtonDown(nFlags, p, v, m_pVI) ;
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CCAMView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	camWake(m_pVI) ; // nt add 2009/9/13
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		m_p[0] = m_p[1] = m_p[2] = 0.0 ; // nt add 2003/5/31
		if( m_pCmd )
			m_pCmd->OnLButtonUp(nFlags, p, v) ;
		else
		{
			CCAMDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnLButtonUp(nFlags, p, v, m_pVI) ;
		}
	}

	CView::OnLButtonUp(nFlags, point);
}

void CCAMView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	camWake(m_pVI) ; // nt add 2009/9/13
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		if( m_pCmd )
			m_pCmd->OnMouseMove(nFlags, p, v) ;
		else
		{
			CCAMDoc* pDoc = GetDocument() ;
			if( pDoc )
				pDoc->OnMouseMove(nFlags, p, v, m_pVI) ;
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CCAMView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		CCAMDoc* pDoc = GetDocument() ;
		if( pDoc )
			pDoc->OnRButtonDown(nFlags, p, v, m_pVI) ;
	}

	CView::OnRButtonDown(nFlags, point);
}

void CCAMView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint temp = point ;
	ClientToScreen(&temp); // ???
	OnContextMenu(this, temp); // ???

	if( m_pVI )
	{
		double p[3] ;
		double v[3] ;
		viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;
		CCAMDoc* pDoc = GetDocument() ;
		if( pDoc )
			pDoc->OnRButtonUp(nFlags, p, v, m_pVI) ;
	}
	
	CView::OnRButtonUp(nFlags, point);
}
//--------------------------------------------------------------
// nt add 2010/9/14
void CCAMView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	double p[3] ;
	double v[3] ;
	viGetWorldPointViewD(m_pVI, point.x, point.y, p, v) ;

	if( nFlags&MK_SHIFT ||
		nFlags&MK_RBUTTON )
	{
		if( m_pCmd == NULL ||
			m_pCmd->GetID() != vctPan )
			OnViewPan() ;
	}
	else
	if( nFlags&MK_CONTROL )
	{
		if( m_pCmd == NULL ||
			m_pCmd->GetID() != vctZoom )
			OnViewZoomByMouse() ;
	}
	else
	{
		if( m_pCmd == NULL ||
			m_pCmd->GetID() != vctRotate )
			OnViewRotate() ;
	}
	if( m_pCmd )
		m_pCmd->OnLButtonDown(nFlags, p, v) ;

	CView::OnMButtonDown(nFlags, point);
}

// nt add 2010/9/14
void CCAMView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ClearCmd() ; // nt add 2010/9/15

	CView::OnMButtonUp(nFlags, point);
}

// nt add 2010/9/14
BOOL CCAMView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pVI )
	{
		if( zDelta < 0 )
			viZoom(m_pVI, 1/0.9) ;
		else
			viZoom(m_pVI, 0.9) ;
		Invalidate(FALSE) ;
	}
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//--------------------------------------------------------------
//	CCAMView keyboard
void CCAMView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	double d ;
	RFRAME lf ;
	char c = '\0' ; // nt add 2009/10/21
	CStr str ; // nt add 2009/10/21
	int ctrl = 0, shift = 0, n ;
	VEC3D v ; // nt add 2009/10/21
	CCAMDoc* pDoc = GetDocument() ;

	// nt add 2009/10/21
	if( GetKeyState(VK_LSHIFT) < 0 ||
		GetKeyState(VK_RSHIFT) < 0 )
		shift = 1 ;
	// nt modify 2010/9/14
	/*if( shift &&
		pDoc->HasCmd() == 0 )
	{
		SendMessage(WM_COMMAND, ID_VIEW_PAN) ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}*/

	// nt add 2009/10/21
	if( GetKeyState(VK_CONTROL) < 0 )
		ctrl = 1 ; // pressed
	if( ctrl && // +
		nChar == 187 )
	{
		SendMessage(WM_COMMAND, ID_VIEW_ZOOMIN) ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}
	else
	if( ctrl && // -
		nChar == 189 )
	{
		SendMessage(WM_COMMAND, ID_VIEW_ZOOMOUT) ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}

	// nt modify 2010/9/14
	// nt add 2009/12/19
	/*if( GetKeyState(VK_TAB) < 0 &&
		pDoc->HasCmd() == 0 )
	{
		SendMessage(WM_COMMAND, ID_VIEW_ROTATE) ;
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}*/

	v[0] = 0. ; // nt add 2009/10/21
	v[1] = 0. ;
	v[2] = 0. ;
	switch(nChar)
	{
	//case 81: // nt add 2003/7/8 : q->quit
	case VK_ESCAPE:
		if( m_pCmd ) // nt add 2010/8/28, 先退出出CViewCmd...
			ClearCmd() ;
		else
			pDoc->ClearCmdAll() ;
		if( m_pSim ) // nt add 2010/7/2
		{
			if( m_pSim->IsPaused() )
				m_pSim->Continue() ;
			else
				m_pSim->Pause() ;
		}
		if( m_pSim5 ) // nt add 2010/7/2
		{
			if( m_pSim5->IsPaused() )
				m_pSim5->Continue() ;
			else
				m_pSim5->Pause() ;
		}
		break ;
	case VK_DELETE:
		SendMessage(WM_COMMAND, ID_EDIT_DELETE) ;
		break ;
	case VK_SPACE: // nt add 2009/11/12
	case VK_RETURN: // nt add 2002/12/7
		pDoc->OnReturn() ; // nt add 2009/10/21
		break ;
	case VK_F2:
		break ;
	case VK_F3:
		break ;
	case VK_F4:
		break ;
	case VK_F9:
		break ;
	case VK_F11:
		break ;
	case VK_F12:
		break ;
	case VK_UP:
		lf.scale = 1.0 ;
		viGetCenterLF(m_pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		d = 5.0*viGetScale(m_pVI) ;
		v[0] = d*lf.Y[0] ;
		v[1] = d*lf.Y[1] ;
		v[2] = d*lf.Y[2] ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_DOWN:
		lf.scale = 1.0 ;
		viGetCenterLF(m_pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		d = -5.0*viGetScale(m_pVI) ;
		v[0] = d*lf.Y[0] ;
		v[1] = d*lf.Y[1] ;
		v[2] = d*lf.Y[2] ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_RIGHT:
		lf.scale = 1.0 ;
		viGetCenterLF(m_pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		d = 5.0*viGetScale(m_pVI) ;
		v[0] = d*lf.X[0] ;
		v[1] = d*lf.X[1] ;
		v[2] = d*lf.X[2] ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	case VK_LEFT:
		lf.scale = 1.0 ;
		viGetCenterLF(m_pVI, lf.X, lf.Y, lf.Z, lf.O) ;
		d = -5.0*viGetScale(m_pVI) ;
		v[0] = d*lf.X[0] ;
		v[1] = d*lf.X[1] ;
		v[2] = d*lf.X[2] ;
		viPan(m_pVI, v) ;
		Invalidate(FALSE) ;
		break ;
	//case VK_SPACE: // nt add 2009/10/21
	//	c = ' ' ;
	//	break ;
	case VK_BACK: // nt add 2009/10/21
		camPromptGet(str) ;
		n = str.GetLength() ;
		if( n > 0 )
			str = str.Left(n-1) ;
		camPromptStr(str) ;
		break ;
	case 191: //case VK_DIVIDE:
		if( shift )
			c = '?' ;
		else
			c = '/' ;
		break ;
	case 190://case VK_DECIMAL: // nt add 2009/10/21
		if( shift )
			c = '>' ;
		else
			c = '.' ;
		break ;
	case 188://case VK_SEPARATOR: // nt add 2009/10/21
		if( shift )
			c = '<' ;
		else
			c = ',' ;
		break ;
	case 189: // nt add 2009/10/21
		if( shift )
			c = '_' ;
		else
			c = '-' ;
		break ;
	case 187: // nt add 2009/10/21
		if( shift )
			c = '+' ;
		else
			c = '=' ;
		break ;
	case 186:
		if( shift )
			c = ':' ;
		else
			c = ';' ;
		break ;
	case 220:
		if( shift )
			c = '|' ;
		else
			c = '\\' ;
		break ;
	case 222:
		if( shift )
			c = '"' ;
		else
			c = '\'' ;
		break ;
	case 219:
		if( shift )
			c = '{' ;
		else
			c = '[' ;
		break ;
	case 221:
		if( shift )
			c = '}' ;
		else
			c = ']' ;
		break ;
	default: // nt add 2009/10/21
		if( shift )
		{
			if( nChar == 49 )
				c = '!' ;
			else
			if( nChar == 50 )
				c = '@' ;
			else
			if( nChar == 51 )
				c = '#' ;
			else
			if( nChar == 52 )
				c = '$' ;
			else
			if( nChar == 53 )
				c = '%' ;
			else
			if( nChar == 54 )
				c = '^' ;
			else
			if( nChar == 55 )
				c = '&' ;
			else
			if( nChar == 56 )
				c = '*' ;
			else
			if( nChar == 57 )
				c = '(' ;
			else
			if( nChar == 48 )
				c = ')' ;
		}
		else
		{
			if( (48 <= nChar && // 0123456789
				nChar <= 57) ||
				(65 <= nChar && // ABC...Z
				nChar <= 90) )
				c = nChar ;
			else
			if( VK_NUMPAD0 <= nChar &&
				nChar <= VK_NUMPAD9 )
				c = '0'+nChar-VK_NUMPAD0 ;
		}
	}
	if( c != '\0' )
		camPromptAttach(c) ;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
//==============================================================
//	CCAMView commands
void CCAMView::OnViewPan() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCAMDoc* pDoc = GetDocument() ;
		//pDoc->ClearCmdAll() ;
		ClearCmd() ; // nt add 2010/8/28
		m_pCmd = new CPan(pDoc, this) ;
	}
}

void CCAMView::OnUpdateViewPan(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( m_pCmd &&
		m_pCmd->GetID() == vctPan )
		pCmdUI->SetCheck(1) ;
	else
		pCmdUI->SetCheck(0) ;

	return ;	
}

void CCAMView::OnViewRotate() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCAMDoc* pDoc = GetDocument() ;
		//pDoc->ClearCmdAll() ;
		ClearCmd() ; // nt add 2010/8/28
		m_pCmd = new CRotate(pDoc, this);
	}
}

void CCAMView::OnUpdateViewRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( m_pCmd &&
		m_pCmd->GetID() == vctRotate )
		pCmdUI->SetCheck(1) ;
	else
		pCmdUI->SetCheck(0) ;

	return ;	
}

void CCAMView::OnViewZoomByMouse() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCAMDoc* pDoc = GetDocument() ;
		//pDoc->ClearCmdAll() ;
		ClearCmd() ; // nt add 2010/8/28
		m_pCmd = new CZoom(pDoc, this) ;
	}
}

void CCAMView::OnUpdateViewZoomByMouse(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( m_pCmd &&
		m_pCmd->GetID() == vctZoom )
		pCmdUI->SetCheck(1) ;
	else
		pCmdUI->SetCheck(0) ;

	return ;	
}

void CCAMView::OnViewZoomByBox() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCAMDoc* pDoc = GetDocument() ;
		//pDoc->ClearCmdAll() ;
		ClearCmd() ; // nt add 2010/8/28
		m_pCmd = new CZoomBox(pDoc, this) ;
	}
}

void CCAMView::OnUpdateViewZoomByBox(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( m_pCmd &&
		m_pCmd->GetID() == vctZoomBox )
		pCmdUI->SetCheck(1) ;
	else
		pCmdUI->SetCheck(0) ;

	return ;	
}

// nt add 2009/5/9
void CCAMView::OnViewZoomAll() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		CCAMDoc* pDoc = GetDocument() ;
		//pDoc->ClearCmd() ;
		double min[3], max[3], cp[3] ;
		pDoc->GetBox(min, max) ;
		cp[0] = 0.5*(min[0]+max[0]) ;
		cp[1] = 0.5*(min[1]+max[1]) ;
		cp[2] = 0.5*(min[2]+max[2]) ;
		double X[3], Y[3], Z[3], O[3] ;
		viGetCenterLF(m_pVI, X, Y, Z, O) ;
		viPrjPntPln(cp, O, Z, cp) ;
		double r = 0.5*viDist(min, max) ;
		min[0] = cp[0]-r*(X[0]+Y[0]) ;
		min[1] = cp[1]-r*(X[1]+Y[1]) ;
		min[2] = cp[2]-r*(X[2]+Y[2]) ;
		max[0] = cp[0]+r*(X[0]+Y[0]) ;
		max[1] = cp[1]+r*(X[1]+Y[1]) ;
		max[2] = cp[2]+r*(X[2]+Y[2]) ;
		viZoomByBox(m_pVI, min, max) ;
		Invalidate(TRUE) ;
	}
}

// nt add 2009/5/9
void CCAMView::OnViewZoomOut() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viZoom(m_pVI, 1/1.1) ;
		Invalidate(TRUE) ;
	}
}

// nt add 2009/5/9
void CCAMView::OnViewZoomIn() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viZoom(m_pVI, 1.1) ;
		Invalidate(TRUE) ;
	}
}
/*-----------------------------------------------------------*/
void CCAMView::OnViewPerspective() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetProjectMode(m_pVI, 1) ;
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewParallel() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetProjectMode(m_pVI, 0) ;
		InvalidateRect(NULL) ;
	}
}
/*-----------------------------------------------------------*/
void CCAMView::OnViewSelect() 
{
	// TODO: Add your command handler code here
	CCAMDoc* pDoc = GetDocument() ;
	pDoc->ClearCmdAll() ;
}
/*-----------------------------------------------------------*/
void CCAMView::OnViewXy() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetViewport(m_pVI, vpXY) ;
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewYz() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetViewport(m_pVI, vpYZ) ;
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewZx() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetViewport(m_pVI, vpZX) ;
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewIso2() 
{
	// TODO: Add your command handler code here
	if( m_pVI )
	{
		viSetViewport(m_pVI, vpI2) ;
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewPlane() 
{
	// TODO: Add your command handler code here
	if( !m_pVI )
		return ;

	CCAMDoc* pDoc = GetDocument() ;
	int n = pDoc->GetSelected(ftRefPlane, 1) ;
	if( n > 0 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)(pDoc->GetObj(0)) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		viSetCenterLF(m_pVI, lf.X, lf.Y, lf.O) ;
		InvalidateRect(NULL) ;
	}
	else
		camPromptStr("首先拾取一个基准平面") ;

	return ;
}
//--------------------------------------------------------------
// NC simullation
// nt add 2010/4/1
void CCAMView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pSim &&
		m_pSim->IsPaused() == 0 ) // nt add 2010/7/2
	{
		if( m_pSim->GetState() == 1 )
		{
			UINT ct = GetTickCount() ;
			if( ct >= m_t1+m_dt1 )
			{
				m_pSim->Update(ct) ;
				m_t1 = ct ;
			}
			if( ct >= m_t2+m_dt2 )
			{
				OnDraw(NULL) ;
				m_t2 = ct ;
			}
		}
	}
	// nt add 2010/6/2
	if( m_pSim5 &&
		m_pSim5->IsPaused() == 0 ) // nt add 2010/7/2
	{
		if( m_pSim5->GetState() == 1 )
		{
			UINT ct = GetTickCount() ;
			if( ct >= m_t1+m_dt1 )
			{
				m_pSim5->Update(ct) ;
				m_t1 = ct ;
			}
			if( ct >= m_t2+m_dt2 )
			{
				OnDraw(NULL) ;
				m_t2 = ct ;
			}
		}
	}
	
	CView::OnTimer(nIDEvent);
}

void CCAMView::OnFileGCode()
{
	LPTSTR pch ;
	CString szFilter ;
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrInitialDir = NULL ;
	szFilter += _T("G代码文件 (.NC)|*.nc|");
    szFilter += _T("所有文件 (*.*)|*.*|");
    pch = szFilter.GetBuffer(0);
    while ((pch = _tcschr(pch, '|')) != NULL)
		*pch++ = '\0';
    dlg.m_ofn.lpstrFilter = szFilter ;
    dlg.m_ofn.lpstrTitle = _T("G代码文件") ;
	dlg.m_ofn.nFilterIndex = 7 ;
	if( dlg.DoModal() == IDOK )
	{
		CString pathName = dlg.GetPathName() ;
		GCODETPATH* pGT = gcodetpathCreate() ;
		gcodetpathSetGCodeFileName(pGT, pathName.GetBuffer(0)) ;
		float cp[3] ;
		cp[0] = 0.f ;
		cp[1] = 0.f ;
		cp[2] = 0.f ;
		gcodetpathSetCutterPos(pGT, cp) ;
		gcodetpathSetDiscPrecision(pGT, 0.005f) ;
		if( gcodetpathMakeTPath(pGT) != 1 )
		{
			char eInfo[300] ;
			eInfo[0] = '\0' ;
			gcodetpathGetEInfo(pGT, eInfo) ;
			if( eInfo[0] != '\0' )
			{
				strcat_s(eInfo, "是发生错误处的代码!") ;
				camPromptStr(eInfo) ;
			} 
			gcodetpathFree(pGT) ;
			m_pGT = NULL ;
		}
		else
		{
			gcodetpathScale(pGT, 1.f) ;
			if( m_pGT )
				gcodetpathFree(m_pGT) ;
			m_pGT = pGT ;
		}
	}
}

void CCAMView::OnViewSim()
{
	if( m_pSim )
		delete m_pSim ;
	m_pSim = NULL ;
	if( m_pTool &&
		m_pGT )
	{
		m_pSim = new CSim() ;
		m_pSim->SetTool(m_pTool) ;
		m_pSim->SetGT(m_pGT) ;
		UINT ct = GetTickCount() ;
		m_t1 = ct ;
		m_t2 = ct ;
		m_pSim->Start(ct) ;
	}
}

void CCAMView::OnViewSim5()
{
	if( m_pSim5 )
		delete m_pSim5 ;
	m_pSim5 = NULL ;
	CStep* pStep = NULL ; // nt add 2010/11/29
	CPath* pPath = NULL ;
	CCAMDoc* pDoc = GetDocument() ;
	int n = pDoc->GetSelected(camStep, 1) ;
	if( n > 0 )
	{
		pStep = (CStep*)(pDoc->GetObj(0)) ;
		pPath = pStep->GetPath() ;
	}
	if( m_pTool &&
		pPath )
	{
		m_pSim5 = new CSim5() ;
		// nt add 2010/12/15
		RFRAME lf ;
		m_pTool->GetLF(&lf) ;
		lf.O[2] = 100. ;
		m_pTool->SetLF(&lf) ;
		// end
		m_pSim5->SetTool(m_pTool) ;
		m_pSim5->SetPath(pPath) ;
		UINT ct = GetTickCount() ;
		m_t1 = ct ;
		m_t2 = ct ;
		m_pSim5->Start(ct) ;
	}
}

void CCAMView::OnViewStop()
{
	if( m_pSim )
	{
		delete m_pSim ;
		m_pSim = NULL ;
	}
	// nt add 2010/6/2
	if( m_pSim5 )
	{
		delete m_pSim5 ;
		m_pSim5 = NULL ;
	}
}

void CCAMView::OnViewDispOnOff()
{
	CCAMDoc* pDoc = GetDocument() ;
	if( pDoc )
	{
		int i, n = pDoc->GetSelected(-1, 10000000), flags ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)pDoc->GetObj(i) ;
			if( pF )
			{
				flags = pF->GetFlags() ;
				if( flags&FEATURE_VISIBLE )
					pF->SetFlags(flags-FEATURE_VISIBLE) ;
				else
					pF->SetFlags(flags+FEATURE_VISIBLE) ;
			}
		}
		InvalidateRect(NULL) ;
	}
}

void CCAMView::OnViewTri()
{
	if( m_drawMode != dmWireframe )
		m_drawMode = dmWireframe ;
	else
		m_drawMode = dmShaded ;

	InvalidateRect(NULL) ;
}
////////////////////////////////////////////////////////////////

