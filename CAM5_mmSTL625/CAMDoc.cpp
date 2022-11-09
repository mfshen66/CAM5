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

// CAMDoc.cpp : CCAMDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CAM.h"
#endif
#include "MainFrm.h"
#include "Models.h"
#include "Mills.h" // nt add 2010/4/1
#include "SelSet.h"
#include "CAMDoc.h"
#include "CAMView.h"
#include <propkey.h>
#include "Command.h"
#include "DocCmd.h"
#include "Dialogs.h"
#include "time.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////
// CCAMDoc
IMPLEMENT_DYNCREATE(CCAMDoc, CDocument)

BEGIN_MESSAGE_MAP(CCAMDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CCAMDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CCAMDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CCAMDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVENURBS, &CCAMDoc::OnFileSaveNURBS)
	ON_COMMAND(ID_FILE_LOADGE, &CCAMDoc::OnFileLoadGE)
	ON_COMMAND(ID_FILE_SAVEFEATURES, OnFileSaveFeatures)
	ON_COMMAND(ID_FILE_LOADFEATURES, OnFileLoadFeatures)
	ON_COMMAND(ID_FILE_SAVESTL, OnFileSaveSTL)
	ON_COMMAND(ID_FILE_LOADIGES, OnFileLoadIGES)
	ON_COMMAND(ID_EDIT_DELETE, &CCAMDoc::OnEditDelete)
	ON_COMMAND(ID_EDIT_MOVE, &CCAMDoc::OnEditMove)
	ON_COMMAND(ID_EDIT_ROTATE, &CCAMDoc::OnEditRotate)
	ON_COMMAND(ID_EDIT_SCALE, &CCAMDoc::OnEditScale)
	ON_COMMAND(ID_EDIT_MIRROR, &CCAMDoc::OnEditMirror)
	ON_COMMAND(ID_EDIT_COLOR, &CCAMDoc::OnEditColor)
	ON_COMMAND(ID_EDIT_WIDTH, &CCAMDoc::OnEditWidth)
	ON_COMMAND(ID_EDIT_MOVE2ORI, &CCAMDoc::OnEditMove2Ori)
	ON_COMMAND(ID_INSERT_WPLANE, &CCAMDoc::OnInsertWPlane)
	ON_COMMAND(ID_INSERT_OFFSET, &CCAMDoc::OnInsertOffset)
	ON_COMMAND(ID_INSERT_NORMALPLANE, &CCAMDoc::OnInsertNormalPlane)
	ON_COMMAND(ID_INSERT_PLANE3P, &CCAMDoc::OnInsertPlane3P)
	ON_COMMAND(ID_INSERT_PLANE2P, &CCAMDoc::OnInsertPlane2P)
	ON_COMMAND(ID_INSERT_PLANEX, &CCAMDoc::OnInsertPlaneX)
	ON_COMMAND(ID_INSERT_PLANEY, &CCAMDoc::OnInsertPlaneY)
	ON_COMMAND(ID_INSERT_PLANEZ, &CCAMDoc::OnInsertPlaneZ)
	ON_COMMAND(ID_INSERT_TANPLN, &CCAMDoc::OnInsertTanPln)
	ON_COMMAND(ID_INSERT_PLANEMOVE, &CCAMDoc::OnInsertPlaneMove)
	ON_COMMAND(ID_INSERT_REFPT, &CCAMDoc::OnInsertRefPt)
	ON_COMMAND(ID_INSERT_INTP, &CCAMDoc::OnInsertInterp)
	ON_COMMAND(ID_INSERT_LIN, &CCAMDoc::OnInsertLin)
	ON_COMMAND(ID_INSERT_CIRCLE, &CCAMDoc::OnInsertCircle)
	ON_COMMAND(ID_INSERT_ELL, &CCAMDoc::OnInsertEll)
	ON_COMMAND(ID_INSERT_ARCRAA, &CCAMDoc::OnInsertArcRAA)
	ON_COMMAND(ID_INSERT_ARCLL, &CCAMDoc::OnInsertArcLL)
	ON_COMMAND(ID_INSERT_ARC, &CCAMDoc::OnInsertArc)
	ON_COMMAND(ID_INSERT_LINE2CIR, &CCAMDoc::OnInsertLine2Cir)
	ON_COMMAND(ID_INSERT_FILLETCIR, &CCAMDoc::OnInsertFilletCir)
	ON_COMMAND(ID_INSERT_FILLETCUR, &CCAMDoc::OnInsertFilletCur)
	ON_COMMAND(ID_INSERT_BC, &CCAMDoc::OnInsertBC)
	ON_COMMAND(ID_INSERT_CURVES, &CCAMDoc::OnInsertCurves)
	ON_COMMAND(ID_INSERT_UVCURVE, &CCAMDoc::OnInsertUVCurve)
	ON_COMMAND(ID_INSERT_SURBORDER, &CCAMDoc::OnInsertSurBorder)
	ON_COMMAND(ID_INSERT_EXTRUDE, &CCAMDoc::OnInsertExtrude)
	ON_COMMAND(ID_INSERT_REVOLVE, &CCAMDoc::OnInsertRevolve)
	ON_COMMAND(ID_INSERT_RULE, &CCAMDoc::OnInsertRule)
	ON_COMMAND(ID_INSERT_BNDSUR4, &CCAMDoc::OnInsertBndSur4)
	ON_COMMAND(ID_INSERT_LOFT, &CCAMDoc::OnInsertLoft)
	ON_COMMAND(ID_INSERT_SWEEP, &CCAMDoc::OnInsertSweep)
	ON_COMMAND(ID_INSERT_PIPE, &CCAMDoc::OnInsertPipe)
	ON_COMMAND(ID_INSERT_CONNECT, &CCAMDoc::OnInsertConnect)
	ON_COMMAND(ID_INSERT_FILL, &CCAMDoc::OnInsertFill)
	ON_COMMAND(ID_INSERT_FILESUR, &CCAMDoc::OnInsertFileSur)
	ON_COMMAND(ID_INSERT_CYL, &CCAMDoc::OnInsertCyl)
	ON_COMMAND(ID_INSERT_SPH, &CCAMDoc::OnInsertSph)
	ON_COMMAND(ID_INSERT_CONE, &CCAMDoc::OnInsertCone)
	ON_COMMAND(ID_INSERT_TORUS, &CCAMDoc::OnInsertTorus)
	ON_COMMAND(ID_INSERT_OFFSUR, &CCAMDoc::OnInsertOffSur)
	ON_COMMAND(ID_INSERT_INTCUR, &CCAMDoc::OnInsertIntCur)
	ON_COMMAND(ID_INSERT_ISOCUR, &CCAMDoc::OnInsertIsoCur)
	ON_COMMAND(ID_INSERT_PLANE, &CCAMDoc::OnInsertPlane)
	ON_COMMAND(ID_INSERT_PIC, &CCAMDoc::OnInsertPic)
	ON_COMMAND(ID_INSERT_PCLOUD, &CCAMDoc::OnInsertPCloud)
	ON_COMMAND(ID_INSERT_FITSUR, &CCAMDoc::OnInsertFitSur)
	ON_COMMAND(ID_INSERT_PTS, &CCAMDoc::OnInsertPts)
	ON_COMMAND(ID_INSERT_MPOINTS2CURVE, &CCAMDoc::OnInsertMPoints2Curve)
	ON_COMMAND(ID_INSERT_SURPTS, &CCAMDoc::OnInsertSurPts)
	ON_COMMAND(ID_INSERT_MINDIST, &CCAMDoc::OnInsertMinDist)
	ON_COMMAND(ID_INSERT_PATH5, &CCAMDoc::OnInsertPath5)
	ON_COMMAND(ID_TOOL_MODSURCPT, &CCAMDoc::OnToolModSurCPt)
	ON_COMMAND(ID_TOOL_MODSURIPT, &CCAMDoc::OnToolModSurIPt)
	ON_COMMAND(ID_TOOL_BREAK, &CCAMDoc::OnToolBreak)
	ON_COMMAND(ID_TOOL_TRIMCUR, &CCAMDoc::OnToolTrimCur)
	ON_COMMAND(ID_TOOL_TONURBS, &CCAMDoc::OnToolToNURBS)
	ON_COMMAND(ID_TOOL_BCURG1, &CCAMDoc::OnToolBCurG1)
	ON_COMMAND(ID_TOOL_BCURG2, &CCAMDoc::OnToolBCurG2)
	ON_COMMAND(ID_TOOL_CURVEEXTEND, &CCAMDoc::OnToolCurveExtend)
	ON_COMMAND(ID_TOOL_SMTHCUR, &CCAMDoc::OnToolSmthCur)
	ON_COMMAND(ID_TOOL_MORPH, &CCAMDoc::OnToolMorph)
	ON_COMMAND(ID_TOOL_TANLIN2, &CCAMDoc::OnToolTanLin2)
	ON_COMMAND(ID_TOOL_OUTCURPTS, &CCAMDoc::OnToolCurPts)
	ON_COMMAND(ID_TOOL_CHKG0, &CCAMDoc::OnToolChkG0)
	ON_COMMAND(ID_TOOL_CHKG1, &CCAMDoc::OnToolChkG1)
	ON_COMMAND(ID_TOOL_BSURG1, &CCAMDoc::OnToolBSurG1)
	ON_COMMAND(ID_TOOL_BSURG2, &CCAMDoc::OnToolBSurG2)
	ON_COMMAND(ID_TOOL_CHKCURGX, &CCAMDoc::OnToolChkCurGx)
	ON_COMMAND(ID_TOOL_CHKSURGX, &CCAMDoc::OnToolChkSurGx)
	ON_COMMAND(ID_TOOL_MESHSUR, &CCAMDoc::OnToolMeshSur)
	ON_COMMAND(ID_TOOL_COMPPRJ, &CCAMDoc::OnToolCompPrj)
	ON_COMMAND(ID_TOOL_CUTSUR, &CCAMDoc::OnToolCutSur)
	ON_COMMAND(ID_TOOL_REVUV, &CCAMDoc::OnToolRevUV)
	ON_COMMAND(ID_TOOL_SURCLOSE, &CCAMDoc::OnToolSurClose)
	ON_COMMAND(ID_TOOL_SURSMTH, &CCAMDoc::OnToolSurSmth)
	ON_COMMAND(ID_TOOL_MESHREFINE, &CCAMDoc::OnToolMeshRefine)
	ON_COMMAND(ID_TOOL_TEST, &CCAMDoc::OnToolTest)
	ON_COMMAND(ID_ANALYSIS_CURVATURECOMB, &CCAMDoc::OnAnalysisCurvatureComb)
	ON_COMMAND(ID_ANALYSIS_GAUSS, &CCAMDoc::OnAnalysisCurvatureComb)
	ON_COMMAND(ID_ANALYSIS_DRAWUV, &CCAMDoc::OnAnalysisDrawUV)
	ON_COMMAND(ID_ANALYSIS_REFINE, &CCAMDoc::OnAnalysisRefine)
	ON_COMMAND(ID_ANALYSIS_CONTROLPTS, &CCAMDoc::OnAnalysisControlPts)
	ON_COMMAND(ID_ANALYSIS_METRIC, &CCAMDoc::OnAnalysisMetric)
	ON_COMMAND(ID_ANALYSIS_DIST, &CCAMDoc::OnAnalysisDist)
	ON_COMMAND(ID_ANALYSIS_DIST2, &CCAMDoc::OnAnalysisDist2)
	ON_COMMAND(ID_ANALYSIS_CCG2, &CCAMDoc::OnAnalysisCCG)
	ON_COMMAND(ID_ANALYSIS_CFG, &CCAMDoc::OnAnalysisCFG)
	ON_COMMAND(ID_NC_MODEL, &CCAMDoc::OnNCModel)
	ON_COMMAND(ID_NC_MCS, &CCAMDoc::OnNCMCS)
	ON_COMMAND(ID_NC_PARAMETERS, &CCAMDoc::OnNCParameters)
	ON_COMMAND(ID_NC_MACHINE, &CCAMDoc::OnNCMachine)
	ON_COMMAND(ID_NC_MATERIALLIB, &CCAMDoc::OnNCMaterialLib)
	ON_COMMAND(ID_NC_PDESIGN, &CCAMDoc::OnNCPDesign)
	ON_COMMAND(ID_NC_TOOLLIB, &CCAMDoc::OnNCToolLib)
	ON_COMMAND(ID_NC_PROGRAM, &CCAMDoc::OnNCProgram)
	ON_COMMAND(ID_NC_POSTPROCESS, &CCAMDoc::OnNCPostProcess)
	ON_COMMAND(ID_NC_EXPORTCLF, &CCAMDoc::OnNCExportCLF)
	ON_COMMAND(ID_NC_EXPORTDOC, &CCAMDoc::OnNCExportDoc)
	ON_COMMAND(ID_SIM_MOTION, &CCAMDoc::OnSimMotion)
	ON_COMMAND(ID_TEST, &CCAMDoc::OnTest)
	ON_COMMAND(ID_TOOL_CHKMESH, &CCAMDoc::OnToolChkMesh)
	ON_COMMAND(ID_TOOL_CHKBND, &CCAMDoc::OnToolChkBnd)
	ON_COMMAND(ID_TOOL_OPTION, &CCAMDoc::OnToolOption)
	ON_COMMAND(ID_TOOL_OPTTRI1, &CCAMDoc::OnToolOptTri1)
	ON_COMMAND(ID_TOOL_OPTTRI2, &CCAMDoc::OnToolOptTri2)
	END_MESSAGE_MAP()

//======================================================================
// CCAMDoc 构造/析构
CCAMDoc::CCAMDoc()
{
	// TODO: 在此添加一次性构造代码
	//m_cmdID = 0 ;
	m_mh = NULL ;
	m_pDlg = NULL ;
	m_ui = -1 ; // nt modify 2010/8/28
	m_mh = NULL ;
	m_nStep = 0 ;
	m_pCmd = NULL ;
	m_lastCmd = 0 ; // nt add 2009/12/18

	m_bDestroy = 0 ;
	m_lf.lfHeight = -12 ;
	m_lf.lfWidth	 = 0 ;
	m_lf.lfEscapement = 0 ;
	m_lf.lfOrientation= 0 ;
	m_lf.lfWeight = 300 ;
	m_lf.lfItalic = 0 ;
	m_lf.lfUnderline	= 0 ;
	m_lf.lfStrikeOut	= 0 ;
	m_lf.lfCharSet = ANSI_CHARSET ;
	m_lf.lfOutPrecision	= OUT_STROKE_PRECIS ;
	m_lf.lfClipPrecision = 0 ;
	m_lf.lfQuality = 0 ;
	m_lf.lfPitchAndFamily = 0 ;
	_tcscpy_s(m_lf.lfFaceName, _T("Arial")) ;
	m_pSelSet = new CSelSet(this) ;
	m_ses.SetSize(10000, 10000) ; // nt add 2010/4/23

	m_drawed = 0 ;
	m_nav_p[0] = 0. ;
	m_nav_p[1] = 0. ;
	m_nav_p[2] = 0. ;

	m_pPart = new CPart() ;
	m_pPart->AddRefPlaneOfXYZ() ;
	m_gridSize = 10. ;
	m_pWorkPlane = (CRefPlane*)m_pPart->GetFeature(0) ;

	m_pPV = NULL ;

	// nt add 2010/4/1------------------------------
	//m_pMill = NULL ;
	m_pMill = new CMill() ; // nt add 2010/9/26

	// nt add 2010/9/8
	m_hPart = 0 ;
	m_hMill = 0 ;
	CMTView* pMTView = (CMTView*)camGetMTView() ;
	CRect rectDummy;
	rectDummy.SetRectEmpty();
	const DWORD dwViewStyle = WS_CHILD |
							  WS_VISIBLE | 
							  TVS_HASLINES |
							  TVS_LINESATROOT | 
							  TVS_HASBUTTONS |
							  TVS_SHOWSELALWAYS ;
	m_tree.Create(dwViewStyle, rectDummy, pMTView, 4) ;
	m_images.Create(IDB_MT_VIEW, 16, 0, RGB(255, 0, 255)) ;
	m_tree.SetImageList(&m_images, TVSIL_NORMAL) ;
	RebuildModelTree() ; // nt add 2010/9/9
	m_tree.ShowWindow(FALSE) ;
	// end model tree

	return ;
}

CCAMDoc::~CCAMDoc()
{
	// nt add 2010/8/22
	FreeCmdDlg() ;

	m_bDestroy = 1 ;

	if( m_pCmd )
		delete m_pCmd ;
	if( m_pSelSet )
		delete m_pSelSet ;
	if( m_pPart )
		delete m_pPart ;
	if( m_pPV )
		RemovePV() ;

	// nt add 2009/12/5, nt modify 2010/4/23
	m_ses.RemoveAll() ;

	// nt add 2010/4/1
	if( m_pMill )
		delete m_pMill ;

	// nt add 2020/5/12 清楚CMTView中的CViewTree!!!!!!!!
	CMTView* pMTView = (CMTView*)camGetMTView() ;
	if( pMTView )
	{
		CViewTree* pViewTree = pMTView->GetViewTree() ;
		if( (&m_tree) == pViewTree )
		{
			pMTView->SetViewTree(NULL) ;
			pMTView->InvalidateRect(NULL) ;
		}
	}
	// end
}
//======================================================================
CCAMView* CCAMDoc::GetActiveView()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();
	CCAMView *pView = (CCAMView*)pChild->GetActiveView();

	return pView ;
}

BOOL CCAMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CCAMDoc 序列化
// nt add 2010/9/1 : in order to compatible with old version
void viSerializeReadIdle(CArchive& ar)
{
	int mode, w, h ;
	double d, e[3], c[3], u[3], n, f ;
	COLORREF c1, c2 ;
	ar >> mode
	   >> w
	   >> h
	   >> d
	   >> e[0] >> e[1] >> e[2]
	   >> c[0] >> c[1] >> c[2]
	   >> u[0] >> u[1] >> u[2]
	   >> n
	   >> f
	   >> c1
	   >> c2 ;
	
	return ;
}

void CCAMDoc::Serialize(CArchive& ar)
{
	int id, verNo ;
	void* pVI ;
	CCAMView* pView ;
	VINFO vinfo ; // nt add 2010/8/26
	
	if (ar.IsStoring())
	{
		verNo = camGetSysVerNo() ;
		ar << verNo ; // nt add 2010/9/1
		pView = GetActiveView() ;
		// TODO: add storing code here
		if( pView )
		{
			ar << (id = 1 );
			pVI = pView->GetVI() ;
			// nt modify 2010/8/26
			//viSerialize(pVI, ar) ;
			viSave(pVI, &vinfo) ;
			ar.Write(&vinfo, sizeof(VINFO)) ;
		}
		else
		{
			ar << (id = 0) ;
			return ;
		}

		// nt modify 2010/6/24
		id = 0 ;
		if( m_pPart )
			id += 1 ;
		if( m_pMill )
			id += 2 ;
		ar << id ;
		if( m_pPart )
			m_pPart->Serialize(ar) ;
		// nt add 2010/6/24
		if( m_pMill )
			m_pMill->Serialize(ar) ;
	}
	else
	{
		// TODO: add loading code here
		ar >> verNo ; // nt add 2010/9/1
		camSetDocVerNo(verNo) ; // nt add 2021/8/13
		if( verNo < 4 ) // old version
			id = verNo ; // old version only has view_id
		else
			ar >> id ; // new version has verNo and viwe_id
		if( id == 1 ) // has view info
		{
			if( verNo > 3 ) // new version
			{
				POSITION pos = GetFirstViewPosition() ;
				pView = (CCAMView*)GetNextView(pos) ;
				if( !pView )
					return ; // error
				pVI = pView->GetVI() ;
				ar.Read(&vinfo, sizeof(VINFO)) ;
				viLoad(pVI, &vinfo) ;
			}
			else
				viSerializeReadIdle(ar) ;
		}
		
		// nt modify 2010/6/24
		ar >> id ;
		if( id&1 )
		{
			if( m_pPart )
			{
				ClearCmd() ;
				if( m_pSelSet )
					m_pSelSet->Clear() ;
				m_pWorkPlane = NULL ;
				m_drawed = 0 ;
				delete m_pPart ;
			}
			m_pPart = new CPart() ;
			if( m_pPart )
				m_pPart->Serialize(ar) ;
		}
		if( id&2 )
		{
			if( m_pMill )
			{
				ClearCmd() ;
				if( m_pSelSet )
					m_pSelSet->Clear() ;
				m_drawed = 0 ;
				delete m_pMill ;
			}
			m_pMill = new CMill() ;
			if( m_pMill )
				m_pMill->Serialize(ar) ;
		}
		// nt add 2010/9/9
		RebuildModelTree() ;
	}
}
//======================================================================
#ifdef SHARED_HANDLERS

// 缩略图的支持
void CCAMDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CCAMDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CCAMDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS
//======================================================================
// CCAMDoc 诊断
#ifdef _DEBUG
void CCAMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCAMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
//======================================================================
// 有关cmd的操作
// 检查应该执行cmdID命令的第几步操作
int CCAMDoc::ChkCmd(MHANDLER mh)
{
	if( m_mh == NULL ) // no cmd
	{
		if( m_ui != -1 || // 无命令时，三个参数必须是初始状态
			m_pDlg != NULL ||
			m_nStep != 0 )
			return -1 ; // error
	}
	else
	{
		if( m_mh != mh ) // 当旧命令未完成时，禁止启动新命令
			return -1 ; // error
	}
	return m_nStep ;
}

// 启动cmdID命令（可以含一个对话框）
// before calling this function, need to confirm that :
// (1)m_mh=NULL,
// (2)m_pDlg=NULL
int CCAMDoc::OnCmd(MHANDLER mh, CDialogEx* pDlg, int ID)
{
	m_mh = mh ;
	FreeCmdDlg() ;
	m_pDlg = pDlg ;
	m_ui = -1 ; // nt modify 2010/8/28
	m_nStep = 0 ;
	if( pDlg )
	{
		CMainFrame* pMF = (CMainFrame*)AfxGetMainWnd() ;
		if( pMF == NULL )
			return 0 ;
		return pMF->CreateAttribWnd(pDlg, ID) ;
	}
	else
		return 1 ;
}

// 命令对话框结束交互，nt add 2010/8/22
// nt modify 2010/8/27
// nt modify 2010/8/28, old name : CmdDlgEnd()
// ui就是用户的交互动作:0=cancel,1=ok,2=apply
// nt modify 2011/6/12
int CCAMDoc::CmdDlgUI(int ui)
{
	// nt add 2011/6/12, 清除当前的交互命令
	if( ui == 0 || // cancel
		ui == 1 ) // ok
		ClearCmd() ;
	else // apply
	{
		m_nStep++ ;
		m_ui = ui ;
		if( m_mh )
			(this->*m_mh)() ;
	}

	return 1 ;
}

// initialize cmd state
int CCAMDoc::CmdEnd()
{
	m_mh = NULL ;
	FreeCmdDlg() ;
	m_nStep = 0 ;
	m_ui = -1 ; // nt modify 2010/8/28, old is 0
	return 1 ;
}

// 释放命令窗口及对话框, nt add 2010/8/22
int CCAMDoc::FreeCmdDlg()
{
	if( m_pDlg )
	{
		CMainFrame* pMF = (CMainFrame*)AfxGetMainWnd() ;
		if( pMF == NULL )
			return 0 ;
		pMF->CloseAttribWnd() ;
		m_pDlg->DestroyWindow() ;
		delete m_pDlg ;
		m_pDlg = NULL ;
	}
	return 1 ;
}
//======================================================================
// CCAMDoc 命令
//	CCAMDoc attributes
int CCAMDoc::IsDestroying()
{
	return m_bDestroy ;
}

void CCAMDoc::ClearCmd()
{
	// nt add 2010/8/27
	CmdEnd() ;
	// ----------------

	if( m_pCmd )
	{
		m_pCmd->Cancel() ;
		delete m_pCmd ;
		m_pCmd = NULL ;
	}
}

void CCAMDoc::ClearCmdAll()
{
	ClearCmd() ;

	POSITION pos = GetFirstViewPosition() ;
	while(pos != NULL)
	{
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( pView != NULL )
			pView->ClearCmd() ;
	}
}

// nt add 2009/12/17
int CCAMDoc::HasCmd()
{
	if( m_pCmd )
		return 1 ;
	else
		return 0 ;
}

int CCAMDoc::GetLastCmd()
{
	return m_lastCmd ;
}

int CCAMDoc::SetLastCmd(int cmd)
{
	m_lastCmd = cmd ;
	return 1 ;
}

double CCAMDoc::GetGridSize()
{
	return m_gridSize ;
}

void CCAMDoc::SetGridSize(double gridSize)
{
	m_gridSize = gridSize ;
}

CRefPlane* CCAMDoc::GetWorkPlane()
{
	return m_pWorkPlane ;
}

CPart* CCAMDoc::GetPart() // nt 2000/7/18
{
	return m_pPart ;
}

CMill* CCAMDoc::GetMill() // nt 2010/6/2
{
	return m_pMill ;
}

// nt add 2009/5/9
int CCAMDoc::GetBox(double min[3], double max[3])
{
	min[0] = -0.5 ;
	min[1] = -0.5 ;
	min[2] = -0.5 ;
	max[0] = 0.5 ;
	max[1] = 0.5 ;
	max[2] = 0.5 ;
	if( m_pPart )
	{
		BOX3D box ;
		m_pPart->GetBox(&box) ;
		memcpy(min, box.min, sizeof(PNT3D)) ;
		memcpy(max, box.max, sizeof(PNT3D)) ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2009/12/17
int CCAMDoc::AddFeature(CFeature* pF)
{
	if( pF &&
		m_pPart )
	{
		// nt add 2010/11/28
		OPTION* option = camGetOption() ;
		// nt add 2010/10/17
		int type = pF->GetType() ;
		if( type == ftRefSur )
			pF->SetColor(option->materialIndex+256) ;
		else
		if( type == ftRefCur || // nt add 2011/7/4
			type == ftRefPt )
			pF->SetColor(option->colorIndex) ;
		// end
		m_pPart->Add(pF) ;
		// nt add 2010/9/9
		CString name ;
		pF->GetName(name) ;
		int iImage = 0 ;
		if( type == ftRefPlane )
			iImage = 51 ;
		else
		if( type == ftRefPt )
			iImage = 67 ;
		else
		if( type == ftRefCur )
			iImage = 33 ;
		else
		if( type == ftRefSur )
			iImage = 30 ;
		HTREEITEM hItem = m_tree.InsertItem(name, iImage, iImage+1, m_hPart);
		m_tree.SetItemData(hItem, (DWORD_PTR)pF) ;
		m_tree.Invalidate(TRUE) ;
		pF->SetItem(hItem) ;
		// end
		// add to select set. nt add 2001/11/28
		if( m_pSelSet )
		{
			m_pSelSet->AddSelected(pF) ;
			glFlush() ; // nt add 2010/8/29
		}
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/1
// 确保pF不在CSelSet中!!!
int CCAMDoc::DelFeature(CFeature* pF)
{
	if( pF &&
		m_pPart )
	{
		// nt add 2010/9/9
		HTREEITEM hItem = pF->GetItem() ;
		if( hItem )
			m_tree.DeleteItem(hItem) ;
		// end
		m_pPart->Delete(pF) ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/11/29
int CCAMDoc::AddStep(CStep* pStep)
{
	if( pStep &&
		m_pMill )
	{
		OPTION* option = camGetOption() ;
		pStep->SetColor(option->colorIndex) ;
		m_pMill->Add(pStep) ;
		CString name ;
		pStep->GetName(name) ;
		int iImage = 73 ;
		HTREEITEM hItem = m_tree.InsertItem(name, iImage, iImage+1, m_hMill);
		m_tree.SetItemData(hItem, (DWORD_PTR)pStep) ;
		m_tree.Invalidate(TRUE) ;
		pStep->SetItem(hItem) ;
		if( m_pSelSet )
		{
			m_pSelSet->AddSelected(pStep) ;
			glFlush() ;
		}
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/11/29
// 确保pStep不在CSelSet中!!!
int CCAMDoc::DelStep(CStep* pStep)
{
	if( pStep &&
		m_pMill )
	{
		HTREEITEM hItem = pStep->GetItem() ;
		if( hItem )
			m_tree.DeleteItem(hItem) ;
		m_pMill->Delete(pStep) ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/3/24, not add to model tree
int CCAMDoc::AddLin(PNT3D p, PNT3D q)
{
	VEC3D dir ;
	mathGetVec(p, q, dir) ;
	double d = mathDist(p, q) ; // length
	if( d > MIN_DBL &&
		m_pPart )
	{
		dir[0] /= d ;
		dir[1] /= d ;
		dir[2] /= d ;
		CURVE* line = lineCreate(p, dir) ; // line
		INTERVAL interval ;
		interval.t1 = 0. ;
		interval.t2 = d ;
		CRefCur* pRefCur ;
		pRefCur = new CRefCur(0, line, &interval) ;
		//m_pPart->Add(pRefCur) ;
		AddFeature(pRefCur) ; // nt add 2010/12/27
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/12/27
int CCAMDoc::AddPt(PNT3D p)
{
	if( m_pPart )
	{
		CRefPt* pRefPt = new CRefPt(0, p) ;
		AddFeature(pRefPt) ;
		return 1 ;
	}
	else
		return 0 ;
}
//==============================================================
CSelSet* CCAMDoc::GetSelSet() // nt 2000/7/18
{
	return m_pSelSet ;
}

// nt add 2011/8/22
void CCAMDoc::ClearSelSet()
{
	if( m_pSelSet )
		m_pSelSet->Clear() ;
}

// nt add 2009/12/5
int CCAMDoc::GetNumOfSelected()
{
	if( m_pSelSet )
		return m_pSelSet->GetNumOfSelected() ;
	else
		return 0 ;
}

// nt add 2009/12/5
int CCAMDoc::GetSelected(int type, int max)
{
	int n = 0 ;
	if( m_pSelSet )
	{
		// nt modify 2010/4/23
		m_ses.RemoveAll() ;
		n = m_pSelSet->GetSelected(type, max, m_ses) ;
	}
	return n ;
}

// nt add 2009/12/5
// 调用该函数前必须先调用GetSelected()!!!
void* CCAMDoc::GetObj(int i)
{
	int n = m_ses.GetSize() ;
	if( 0 <= i &&
		i < n )
		return m_ses[i] ; // nt modify 2010/4/23
	else
		return NULL ;
}

// nt add 2009/12/5
int CCAMDoc::GetType(int i)
{
	int n = m_ses.GetSize() ;
	if( 0 <= i &&
		i < n )
		return ((CVO*)(m_ses[i]))->GetType() ;
	else
		return -1 ;
}

// nt add 2009/12/5
// nt modify 2010/4/23
double CCAMDoc::GetU(int i)
{
	int n = m_ses.GetSize() ;
	if( 0 <= i &&
		i < n )
	{
		CVO* pVO = (CVO*)m_ses[i] ;
		if( pVO )
		{
			int type = pVO->GetType() ;
			if( type == ftRefCur )
				return ((CRefCur*)pVO)->GetU() ;
			else
			if( type == ftRefSur )
				return ((CRefSur*)pVO)->GetU() ;
		}
	}

	return MAX_DBL+1. ;
}

// nt modify 2010/4/23
double CCAMDoc::GetV(int i)
{
	int n = m_ses.GetSize() ;
	if( 0 <= i &&
		i < n )
	{
		CVO* pVO = (CVO*)m_ses[i] ;
		if( pVO )
		{
			int type = pVO->GetType() ;
			if( type == ftRefSur )
				return ((CRefSur*)pVO)->GetV() ;
		}
	}

	return MAX_DBL+1. ;
}

// nt add 2021/6/24 geometry picked position
int CCAMDoc::GetGPP(PNT3D gpp)
{
	int i, n, type ;
	double u, v ;
	CURVE* curve = NULL ;
	SURFACE* surface = NULL ;

	if( m_pSelSet )
	{
		n = m_pSelSet->GetNumOfSelected() ;
		for( i = 0 ; i < n ; i++ )
		{
			CVO* pVO = m_pSelSet->GetSelected(i) ;
			if( pVO->GetType() < 80 )
			{
				CFeature* pF = (CFeature*)pVO ;
				type = pF->GetType() ;
				if( type == ftRefPt )
				{
					((CRefPt*)pF)->GetP(gpp) ;
					return 1 ;
				}
				else
				if( type == ftRefCur )
				{
					u = ((CRefCur*)pF)->GetU() ;
					curve = ((CRefCur*)pF)->GetCurve() ;
					curveEvalPos(curve, u, gpp) ;
					return 1 ;
				}
				else
				if( type == ftRefSur )
				{
					u = ((CRefSur*)pF)->GetU() ;
					v = ((CRefSur*)pF)->GetV() ;
					surface = ((CRefSur*)pF)->GetSurface() ;
					surfaceEvalPos(surface, u, v, gpp) ;
					return 1 ;
				}
			}
		}
	}

	return 0 ;
}
//==============================================================
void CCAMDoc::Draw(void* pVI, int drawMode) // nt 2000/7/12
{
	glEnable(GL_DEPTH_TEST);


	if( m_pPart )
		m_pPart->Draw(pVI, drawMode) ;

	// nt add 2010/6/2
	if( m_pMill )
		m_pMill->Draw(pVI, drawMode) ;

	if( m_pWorkPlane )
	{
		m_pWorkPlane->DrawGrid(pVI, RGB(128, 128, 128), m_gridSize) ;
		glColor3ub(0, 255, 0) ; // nt add 2010/11/27
		m_pWorkPlane->DrawXYMark(pVI) ; // nt add 2010/11/27
	}

	if( m_pSelSet )
		m_pSelSet->Draw() ;

	if( m_pPV )
		m_pPV->Draw(pVI, drawMode) ;
	
	glDisable(GL_DEPTH_TEST);
}

void CCAMDoc::Redraw()
{
	POSITION pos = GetFirstViewPosition() ;
	while(pos != NULL)
	{
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( pView != NULL )
			pView->Invalidate(FALSE) ;
	}
}
//==============================================================
// nt add 2010/9/9
// 可能是激活也可能是inactive
void CCAMDoc::OnActivate()
{
	CMTView* pMTView = (CMTView*)camGetMTView() ;
	if( pMTView )
		pMTView->OnActivate() ;
}

// nt add 2010/9/9
CViewTree* CCAMDoc::GetTree()
{
	return &m_tree ;
}

// nt add 2010/9/9
int CCAMDoc::RebuildModelTree()
{
	m_tree.DeleteAllItems() ;
	m_hPart = m_tree.InsertItem(_T("零件模型"), 36, 37) ;
	m_tree.SetItemData(m_hPart, 0) ;
	m_hMill = m_tree.InsertItem(_T("加工模型"), 70, 71) ;
	m_tree.SetItemData(m_hMill, 0) ;

	// rebuild CPart
	if( m_pPart )
	{
		int i, n = m_pPart->GetNumOfFeature() ;
		HTREEITEM hItem ;
		CString name ;
		CFeature* pF ;
		for( i = 0 ; i < n ; i++ )
		{
			pF = m_pPart->GetFeature(i) ;
			if( pF )
			{
				pF->GetName(name) ;
				int iImage = 0, type = pF->GetType() ;
				if( type == ftRefPlane )
					iImage = 51 ;
				else
				if( type == ftRefPt )
					iImage = 67 ;
				else
				if( type == ftRefCur )
					iImage = 33 ;
				else
				if( type == ftRefSur )
					iImage = 30 ;
				hItem = m_tree.InsertItem(name, iImage, iImage+1, m_hPart) ;
				m_tree.SetItemData(hItem, (DWORD_PTR)pF) ;
				pF->SetItem(hItem) ;
			}
		}
	}

	// rebuild CMill
	if( m_pMill )
	{ // following codes are added by nt 2010/11/29
		int i, n = m_pMill->GetNumOfStep() ;
		HTREEITEM hItem ;
		CString name ;
		CStep* pStep ;
		for( i = 0 ; i < n ; i++ )
		{
			pStep = m_pMill->GetStep(i) ;
			if( pStep )
			{
				int iImage = 73 ;
				pStep->GetName(name) ;
				hItem = m_tree.InsertItem(name, iImage, iImage+1, m_hMill) ;
				m_tree.SetItemData(hItem, (DWORD_PTR)pStep) ;
				pStep->SetItem(hItem) ;
			}
		}
	}

	m_tree.Invalidate(TRUE) ;

	return 1 ;
}
//==============================================================
void CCAMDoc::AddPV(CPV* pPV)
{
	if( m_pPV )
		delete m_pPV ;
	m_pPV = pPV ;

	return ;
}

void CCAMDoc::RemovePV()
{
	if( m_pPV )
	{
		delete m_pPV ;
		m_pPV = NULL ;
	}
}
//==============================================================
// CCAMDoc navigate
// 注意：这部分导航就是针对3D curve的导航，将来草图的导航有
// 草图自己实现，与这部分代码无关。nt add 2009/5/9
int CCAMDoc::Navigate(DNAVI* dn)
{
	DrawNavP(0) ;

	if( !dn )
		return 0 ;

	dn->feat = NULL ;

	// 利用CRefPt和CRefCur进行导航
	if( m_pPart )
	{
		if( m_pPart->Navigate(dn) == 0 )
			return 0 ;
		if( dn->feat )
			goto navigated ;
	}

	// 利用当前工作平面进行导航
	if( m_pWorkPlane )
	{
		if( m_pWorkPlane->Navigate(dn, m_gridSize) == 0 )
			return 0 ;
		if( dn->feat )
			goto navigated ;
	}
		
	return 1 ;
navigated:
	memcpy(m_nav_p, dn->nav_p, sizeof(PNT3D)) ;
	DrawNavP(1) ;

	return 1 ;
}

// nt add 2010/3/26
int CCAMDoc::NavigateKeyPt(DNAVI* dn)
{
	DrawNavP(0) ;

	if( !dn )
		return 0 ;

	dn->feat = NULL ;

	// 利用CRefPt和CRefCur进行导航
	if( m_pPart )
	{
		if( m_pPart->NavigateKeyPt(dn) == 0 )
			return 0 ;
		if( dn->feat )
		{
			memcpy(m_nav_p, dn->nav_p, sizeof(PNT3D)) ;
			DrawNavP(1) ;
		}
	}

	return 1 ;
}

void CCAMDoc::DrawNavP(int mode)
{
	if( mode == m_drawed )
		return ;

	POSITION pos = GetFirstViewPosition() ;
	while(pos != NULL)
	{
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( pView != NULL )
		{
			void* pVI = pView->GetVI() ;
			if( viIsWake(pVI) )
			{
				glColor3ub(255, 0, 0) ;
				glPointSize(3.5f) ;
				glEnable(GL_COLOR_LOGIC_OP) ;
				glLogicOp(GL_XOR) ;
				glBegin(GL_POINTS) ;
					glVertex3dv(m_nav_p) ;
				glEnd() ;
				glDisable(GL_COLOR_LOGIC_OP) ;
				glFlush() ;
			}
		}
	}

	m_drawed = mode ;

	return ;
}
//==============================================================
// CCAMDoc mouse
void CCAMDoc::OnMouseMove(int nFlags, double p[3], double v[3], void* pVI) // nt 2000/7/18
{
	if( m_pCmd )
		m_pCmd->OnMouseMove(nFlags, p, v) ;
	else
	if( m_pSelSet )
		m_pSelSet->OnMouseMove(nFlags, p, v, pVI) ;

	// nt add 2009/5/9
	if( m_pWorkPlane )
	{
		PNT2D prj_p ;
		m_pWorkPlane->Project(p, v, prj_p) ;
		camPromptPt(prj_p) ;
	}

	return ;
}

void CCAMDoc::OnLButtonDown(int nFlags, double p[3], double v[3], void* pVI) // nt 2000/7/18
{
	if( m_pCmd )
		m_pCmd->OnLButtonDown(nFlags, p, v) ;
	else
	if( m_pSelSet )
		m_pSelSet->OnLButtonDown(nFlags, p, v, pVI) ;
}

void CCAMDoc::OnLButtonUp(int nFlags, double p[3], double v[3], void* pVI) // nt 2000/7/18
{
	if( m_pCmd )
		m_pCmd->OnLButtonUp(nFlags, p, v) ;
	else // nt add 2010/4/23
	if( m_pSelSet )
	{
		m_pSelSet->OnLButtonUp(nFlags, p, v, pVI) ;
		// nt add 2010/11/25
		if( m_pDlg )
		{
			if( m_mh == &CCAMDoc::OnEditMirror )
			{
				if( GetSelected(ftRefPlane, 1) > 0 )
				{
					CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
					RFRAME lf ;
					pRefPlane->GetLF(&lf) ;
					((CMirrorDlg*)m_pDlg)->OnSelectedMirrorPlane(&lf) ;
				}
			}
			else
			if( m_mh == &CCAMDoc::OnInsertIsoCur ) // nt add 2010/11/27
			{
				if( GetSelected(ftRefSur, 1) > 0 )
				{
					CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
					BOXUV boxuv ;
					pRefSur->GetBoxUV(&boxuv) ;
					((CIsoCurDlg*)m_pDlg)->SetBoxUV(boxuv.min[0],
												    boxuv.max[0],
												    boxuv.min[1],
												    boxuv.max[1]) ;
					((CIsoCurDlg*)m_pDlg)->OnSelectedSur(pRefSur) ;
					((CIsoCurDlg*)m_pDlg)->UpdateData(FALSE) ;
				}
			}
		}
	}
}

void CCAMDoc::OnRButtonDown(int nFlags, double p[3], double v[3], void* pVI) // nt 2000/7/18
{
	if( m_pCmd )
		m_pCmd->OnRButtonDown(nFlags, p, v) ;
}

void CCAMDoc::OnRButtonUp(int nFlags, double p[3], double v[3], void* pVI) // nt 2000/7/18
{
}

// nt add 2009/10/21
int CCAMDoc::OnReturn()
{
	if( m_pCmd )
		m_pCmd->OnReturn() ;
	else
	{
		int lastCmd = GetLastCmd() ;
		if( lastCmd != -1 )
		{
			SetLastCmd(-1) ;
			POSITION pos = GetFirstViewPosition() ;
			while(pos != NULL)
			{
				CCAMView* pView = (CCAMView*)GetNextView(pos) ;
				if( pView != NULL )
					pView->SendMessage(WM_COMMAND, lastCmd) ;
			}
		}
	}
	return 1 ;
}
//==============================================================
// CCAMDoc commands
// nt add 2010/5/8
// nt modify 2010/10
void CCAMDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	CString szFilter ;
	szFilter += _T("Part Files (*.Part)|*.Part|") ;
	szFilter += _T("IGES Files(*.igs)|*.igs|") ;
	szFilter += _T("OBJ Files(*.obj)|*.obj|") ;
	szFilter += _T("OBJ Files(*.LPS)|*.LPS|");
	CFileDialog dlg(TRUE, _T("*.Part"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		CString ext = dlg.GetFileExt() ;
		CString pathName = dlg.GetPathName() ;
		if( ext == _T("Part") ||
			ext == _T("part") ||
			ext == _T("PART") )
		{
			CFile file ;
			if( file.Open(pathName, CFile::modeRead) )
			{
				CArchive ar(&file, CArchive::load) ;
				Serialize(ar) ;
			}
			else
				AfxMessageBox(_T("打开文件错误!")) ;
		}
		else
		if( ext == _T("igs") ||
			ext == _T("IGS") )
		{
			if( m_pPart )
				if( m_pPart->ImportIGES(pathName, 5/*7*/) != 1 )
					AfxMessageBox(_T("数据交换错误!")) ;
		}
		else
		if( ext == _T("obj") || // nt add 2010/10
			ext == _T("OBJ") )
		{
			if( m_pSelSet )
				m_pSelSet->Clear() ;
			if( m_pPart )
				if( m_pPart->ImportObj(pathName) != 1 )
					camPromptStr("数据交换错误!") ;
		}
		else
		if (ext == _T("lps") || // nt add 2021/5/1
			ext == _T("LPS"))
		{
			int i, j, nloop, *nplps ;
			PNT3D p, q, p0;
			FILE* fp = NULL;
			CStr str;
			str = pathName;
			fopen_s(&fp, str.GetBuffer(0), "r+t");
			fscanf_s(fp, "%d\n", &nloop);
			nplps = new int[nloop];
			for (i = 0; i < nloop; i++)
				fscanf_s(fp, "%d\n", nplps+i);
			p[2] = q[2] = 0.;
			for (i = 0; i < nloop; i++)
			{
				fscanf_s(fp, "%lf %lf\n", p, p+1);
				memcpy(p0, p, sizeof(PNT2D));
				for (j = 1; j < nplps[i]; j++)
				{
					fscanf_s(fp, "%lf %lf\n", q, q+1);
					AddLin(p, q);
					memcpy(p, q, sizeof(PNT2D));
				}
				AddLin(p, p0);
			}
			fclose(fp);
		}
		Redraw() ;
	}
}

void CCAMDoc::OnFileSave() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Part Files (*.Part)|*.Part||");
	CFileDialog dlg(FALSE, _T(".Part"), GetTitle(), OFN_HIDEREADONLY, szFilter) ;
	CString pathName, ext ;
	CFileFind find ;
	int ret ;

	if( GetPathName() == _T("") )
	{
again:	if( dlg.DoModal() == IDOK )
		{
			pathName = dlg.GetPathName() ;
			if( find.FindFile((LPCTSTR)pathName) )
			{
				ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL) ;
				if( ret == IDCANCEL )
					return;
				else
				if( ret == IDNO )
					goto again ;
			}
			ext = dlg.GetFileExt() ;
			if( ext == _T("Part") )
			{
				if( OnSaveDocument(pathName) )
					SetPathName(pathName) ;
			}
		}
	}
	else
		OnSaveDocument(GetPathName()) ;	
}

void CCAMDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CString szFilter ;
	szFilter += "Part Files (*.Part)|*.Part|" ;
	szFilter += "DXF Files (*.dxf)|*.dxf|" ;
	szFilter += "IGES Files(*.igs)|*.igs|" ;
	szFilter += "VRML Files(*.wrl)|*.wrl|" ;
	szFilter += "3ds Files(*.3ds)|*.3ds|" ;
	szFilter += "obj Files(*.obj)|*.obj|" ;
	szFilter += "Point Files(*.Point)|*.Point||" ;
	CFileDialog dlg(FALSE, 
		            _T(".Part"),
					GetTitle(),
					OFN_HIDEREADONLY,
					szFilter.GetBuffer(0)) ;
	CString pathName, ext ;
	CFileFind find ;
	int ret ;

	// the code below have some bug I will improve it
	// now I will continue my process
again:
	if( dlg.DoModal() == IDOK )
	{
		CString title = dlg.GetFileName() ;
		if( title.Find('.') != -1 )
		{
			CString ext1 = dlg.GetFileExt() ;
			if( (ext1 != _T("Part")) &&
				(ext1 != _T("dxf")) &&
				(ext1 != _T("igs")) &&
				(ext1 != _T("wrl")) &&
				(ext1 != _T("3ds")) &&
				(ext1 != _T("obj")) &&
				(ext1 != _T("Point")) && // nt add 2015/5/30
				(ext1 != _T("point")) )
			{
				AfxMessageBox(_T("文件名包含非法字符"), MB_OK|MB_ICONSTOP) ;
				goto again ;
			}
		}
		pathName = dlg.GetPathName() ;
		if( find.FindFile((LPCTSTR)pathName) )
		{
			ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL) ;
			if( ret == IDCANCEL )
				return ;
			else
			if( ret == IDNO )
				goto again ;
		}
		ext = dlg.GetFileExt() ;
		if( ext == _T("Part") )
		{
			if( OnSaveDocument(pathName) )
				SetPathName(pathName) ;
		}
		else 
		if( ext == _T("dxf") )
		{
			CWaitCursor wait ;
			if( m_pPart )
				m_pPart->DxfOut(pathName) ;
		}
		else 
		if( ext == _T("igs") )
		{
			CWaitCursor wait ;
			if (m_pPart)
				m_pPart->IGESOut(pathName) ;
		}
		else
		if( ext == _T("wrl") )
		{
			TCHAR szFilter[] = _T("Bitmap位图(*.bmp)|*.bmp||") ;
			CFileDialog bmpdlg(TRUE, _T("*.bmp"), _T("bitmap"), OFN_HIDEREADONLY, szFilter) ;
			if( dlg.DoModal() == IDOK )
			{
				CString bitsName = bmpdlg.GetPathName() ;
				CWaitCursor wait ;
				if( m_pPart )
					 m_pPart->VRMLOut(pathName, bitsName) ;
			}
		}
		else 
		if( ext == _T("3ds") )
		{
			CWaitCursor wait ;
			if( m_pPart )
				m_pPart->Out3DS(pathName) ;
		}
		else
		if( ext == _T("obj") )
		{
			CWaitCursor wait ;
			if( m_pPart )
				m_pPart->ObjOut(pathName) ;
		}
		else
		if( ext == _T("Point") ||
			ext == _T("point") ) // nt add 2015/5/30
		{
			if( m_pPart )
			{
				//if( m_pPart->ExportPts(pathName) != 1 )
					//camPromptStr("数据交换错误!") ;
				int i, n = GetSelected(ftRefPt, 1000000) ;
				COA es ;
				for( i = 0 ; i < n ; i++ )
				{
					CRefPt* pRefPt = (CRefPt*)GetObj(i) ;
					es.Add(pRefPt) ;
				}
				if( m_pPart->ExportPts2(pathName, es) != 1 )
					camPromptStr("数据交换错误!") ;
				es.RemoveAll() ;
			}
		}
	}
}

/*	2006/8/13 nt
 *	保存单个NURBS, 文件格式为SRF
 */
void CCAMDoc::OnFileSaveNURBS() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("NURBS Files (*.SRF)|*.Srf||") ;
	CFileDialog dlg(FALSE, _T(".Srf"), GetTitle(), OFN_HIDEREADONLY, szFilter) ;
	CString pathName, ext ;
	CFileFind find ;
	int ret ;

again:
	if( dlg.DoModal() == IDOK )
	{
		CString title = dlg.GetFileName() ;
		pathName = dlg.GetPathName() ;
		if( find.FindFile((LPCTSTR)pathName) )
		{
			ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL) ;
			if( ret == IDCANCEL )
				return ;
			else
			if( ret == IDNO )
				goto again ;
		}
	
		if( !m_pSelSet ||
			!m_pPart )
			return ;
		int n = GetSelected(ftRefCur, 1) ; // get mult curves
		if( n > 0 )
		{
			int exact ;
			BSCUR* bc ;
			CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			CURVE* curve = pRefCur->GetCurve() ;
			if( entityGetType(curve) == etBCurve )
			{
				bc = curveGetNurbs(curve) ;
				bscurSaveASCII2(bc, pathName) ;
			}
			else // nt modify 2009/4/26, 目的是可以保存非样条曲线
			{
				curveToNurbs(curve, &interval, &bc, &exact) ;
				bscurSaveASCII2(bc, pathName) ;
				bscurFree(bc) ;
			}
		}

		n = GetSelected(ftRefSur, 1) ; // get mult surfaces
		if( n > 0 )
		{
			int exact ;
			BSSUR* bs ;
			CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
			BOXUV boxuv ;
			pRefSur->GetBoxUV(&boxuv) ;
			SURFACE* surface = pRefSur->GetSurface() ;
			if( entityGetType(surface) == etBSurface )
			{
				bs = surfaceGetNurbs(surface) ;
				bssurSaveASCII2(bs, pathName) ;
			}
			else
			{
				surfaceToNurbs(surface, &boxuv, &bs, &exact) ;
				bssurSaveASCII2(bs, pathName) ;
				bssurFree(bs) ;
			}
		}
	}
}

void CCAMDoc::OnFileLoadGE() 
{
	// TODO: Add your command handler code here
	if( m_pPart )
	{
		TCHAR szFilter[] = _T("图元 Files (*.GE)|*.ge|(*.TXT)|*.txt||") ;
		CFileDialog dlg(TRUE, _T(".GE"), GetTitle(), OFN_HIDEREADONLY, szFilter) ;
		if( dlg.DoModal() == IDOK )
		{
			if( m_pPart->LoadGE(dlg.GetPathName()) )
			{
			}
			else
				AfxMessageBox(_T("输入图元失败")) ;
		}
	}
}

/*	2010/10/17
 *	保存若干特征
 */
void CCAMDoc::OnFileSaveFeatures() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Feature Files (*.Feature)|*.Feature||") ;
	CFileDialog dlg(FALSE, _T(".Feature"), GetTitle(), OFN_HIDEREADONLY, szFilter) ;
	CString pathName, ext ;
	CFileFind find ;
	int ret ;

again:
	if( dlg.DoModal() == IDOK )
	{
		CString title = dlg.GetFileName() ;
		pathName = dlg.GetPathName() ;
		if( find.FindFile((LPCTSTR)pathName) )
		{
			ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL) ;
			if( ret == IDCANCEL )
				return ;
			else
			if( ret == IDNO )
				goto again ;
		}
	
		if( m_pPart )
		{
			int i, n = GetSelected(-1, 10000) ; // get all the features
			CFeature* pF ;
			COA fs ;
			if( n > 0 )
			{
				for( i = 0 ; i < n ; i++ )
				{
					if( GetType(i) < 50 )
					{
						pF = (CFeature*)GetObj(i) ;
						fs.Add(pF) ;
					}
				}
				CFile file ;
				if( file.Open(pathName, CFile::modeWrite|CFile::modeCreate) )
				{
					CArchive ar(&file, CArchive::store) ;
					m_pPart->SerializeFs(ar, fs) ;
					ar.Close() ;
					file.Close() ;
				}
			}
		}
	}
}

/*	2010/10/17
 *	打开若干特征
 */
void CCAMDoc::OnFileLoadFeatures() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Feature Files (*.Feature)|*.Feature||") ;
	CFileDialog dlg(TRUE, _T(".Feature"), GetTitle(), OFN_HIDEREADONLY, szFilter) ;
	CString pathName ;

	if( dlg.DoModal() == IDOK )
	{
		pathName = dlg.GetPathName() ;
		CFile file ;
		if( file.Open(pathName, CFile::modeRead) )
		{
			CArchive ar(&file, CArchive::load) ;
			COA fs ;
			m_pPart->SerializeFs(ar, fs) ;
			int i, n = fs.GetSize() ;
			for( i = 0 ; i < n ; i++ )
			{
				CFeature* pF = (CFeature*)fs[i] ;
				AddFeature(pF) ;
			}
			ar.Close() ;
			file.Close() ;
		}
	}
}

// nt add 2021/6/24 JH code
void CCAMDoc::OnFileSaveSTL() 
{
	// 检测选择集
	int n = GetSelected(ftRefSur, 100000);
	if (n < 1)
	{
		camPromptStr("先拾取要保存的曲面...");
		return;
	}

	// 选择文件路径
	CString szFilter =  _T("STL Files (*.stl)|*.stl|");
	CFileDialog dlg(FALSE, _T("*.stl"), NULL, OFN_HIDEREADONLY, szFilter);
	CString pathName;
	CFileFind find;
	int flag = 0; // 1 选择成功，0 选择失败
	while(dlg.DoModal() == IDOK)
	{
		CString title = dlg.GetFileName();
		if (title.Find('.') != -1)
		{
			CString ext1 = dlg.GetFileExt();
			if ((ext1 != _T("STL")) &&
				(ext1 != _T("stl")))
			{
				AfxMessageBox(_T("文件名包含非法字符"), MB_OK | MB_ICONSTOP);
				continue;
			}
		}
		pathName = dlg.GetPathName();
		if (find.FindFile((LPCTSTR)pathName))
		{
			int ret = AfxMessageBox(_T("是否覆盖原文件?"), MB_YESNOCANCEL);
			if (ret == IDNO)
				continue;
			if (ret == IDCANCEL)
				return;
		}
		flag = 1;
		break;
	}
	if (0 == flag)
		return; // 路径选择失败

	// 打开文件
	FILE* fp = NULL;
	if (_tfopen_s(&fp, pathName.GetBuffer(0), _T("w")) != 0)
	{
		AfxMessageBox(_T("打开文件错误!"));
		return;
	}
	if (!fp)
		return;

	// 保存数据
	int i, j, k;
	fprintf(fp, "solid SELECTED_SURFACES\n");
	for (i = 0; i < n; i++)
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i);
		FACET* facet = pRefSur->GetFacet();
		FNODE* fnode = facet->nodes;
		// facet转换为STL...
		PNT3D p0, p1, p2;
		VEC3D norm, v0, v1;
		for (j = 0; j < facet->ntria; j++) 
		{
			for (k = 0; k < 3; k++) 
			{
				p0[k] = fnode[facet->trias[j][0]].p[k];
				p1[k] = fnode[facet->trias[j][1]].p[k];
				p2[k] = fnode[facet->trias[j][2]].p[k];
				v0[k] = p1[k] - p0[k];
				v1[k] = p2[k] - p0[k];
			}
			mathVProduct(v0, v1, norm);
			mathUniVec(norm, MIN_LEN);
			fprintf(fp, "   facet normal %.6e %.6e %.6e\n",
				norm[0], norm[1], norm[2]);
			fprintf(fp, "      outer loop\n");
			fprintf(fp, "         vertex %.6e %.6e %.6e\n",
				p0[0], p0[1], p0[2]);
			fprintf(fp, "         vertex %.6e %.6e %.6e\n",
				p1[0], p1[1], p1[2]);
			fprintf(fp, "         vertex %.6e %.6e %.6e\n",
				p2[0], p2[1], p2[2]);
			fprintf(fp, "      endloop\n");
			fprintf(fp, "   endfacet\n");
		}
		// 保存四边面片
		for (int i = 0; i < facet->nquad; i++)
		{
			for (int setoff = 0; setoff < 2; setoff++) 
			{
				for (k = 0; k < 3; k++)
				{
					p0[k] = fnode[facet->quads[j][0 + setoff]].p[k];
					p1[k] = fnode[facet->quads[j][1 + setoff]].p[k];
					p2[k] = fnode[facet->quads[j][2 + setoff]].p[k];
					v0[k] = p1[k] - p0[k];
					v1[k] = p2[k] - p0[k];
				}
				mathVProduct(v0, v1, norm);
				mathUniVec(norm, MIN_LEN);
				fprintf(fp, "   facet normal %.6e %.6e %.6e\n",
					norm[0], norm[1], norm[2]);
				fprintf(fp, "      outer loop\n");
				fprintf(fp, "         vertex %.6e %.6e %.6e\n",
					p0[0], p0[1], p0[2]);
				fprintf(fp, "         vertex %.6e %.6e %.6e\n",
					p1[0], p1[1], p1[2]);
				fprintf(fp, "         vertex %.6e %.6e %.6e\n",
					p2[0], p2[1], p2[2]);
				fprintf(fp, "      endloop\n");
				fprintf(fp, "   endfacet\n");
			}
		}
	}
	fprintf(fp, "endsolid");

	// 关闭文件
	camPromptStr("保存文件成功.");
	fclose(fp);

	return ;
}

void CCAMDoc::OnFileLoadIGES()
{
	// TODO: Add your command handler code here
	CString szFilter ;
	szFilter += _T("IGES Files(*.igs)|*.igs|") ;
	CFileDialog dlg(TRUE, _T("*.igs"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		CString ext = dlg.GetFileExt() ;
		CString pathName = dlg.GetPathName() ;
		if( ext == _T("igs") ||
			ext == _T("IGS") )
		{
			if( m_pPart )
				if( m_pPart->ImportIGES3(pathName, 5/*7*/) != 1 )
					AfxMessageBox(_T("数据交换错误!")) ;
		}
		Redraw() ;
	}
}
//--------------------------------------------------------------
/*	2001/11/26 nt
 *	delete features
 *	nt modify 2010/5/22 : add Clear()
 *	nt modify 2010/11/29 : add camStep ...
 */
void CCAMDoc::OnEditDelete() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet )
		return ;
	camPromptStr("拾取一个或多个特征") ;
	int n = GetSelected(-1, 100000), type ; // get all entities
	if( n > 0 )
	{
		m_pSelSet->Clear() ; // nt add 2010/5/22
		for( int i = 0 ; i < n ; i++ )
		{
			type = GetType(i) ; // nt add 2010/11/29
			if( type < 50 &&
				m_pPart )
			{
				CFeature* pF = (CFeature*)GetObj(i) ;
				if( pF == m_pWorkPlane )
					m_pWorkPlane = NULL ;
				CStr name ;
				pF->GetName(name) ;
				if( name != "XY平面" &&
					name != "YZ平面" &&
					name != "ZX平面" )
				{
					// delete an item, nt add 2010/9/9
					HTREEITEM hItem = pF->GetItem() ;
					m_tree.DeleteItem(hItem) ;
					m_tree.Invalidate(TRUE) ;
					m_pPart->Delete(pF) ;
					delete pF ;
				}
			}
			else
			if( type == MPOINT )
			{
				CMPoint* pMPoint = (CMPoint*)GetObj(i) ;
				pMPoint->m_attrib = -1 ;
			}
			else
			if( type == camStep && // nt add 2010/11/29
				m_pMill )
			{
				CStep* pStep = (CStep*)GetObj(i) ;
				HTREEITEM hItem = pStep->GetItem() ;
				m_tree.DeleteItem(hItem) ;
				m_tree.Invalidate(TRUE) ;
				m_pMill->Delete(pStep) ;
				delete pStep ;
			}
		}
		Redraw() ;
	}

	return ;
}

// nt add 2009/5/27
// rewrite 20108/27 nt
// nt modify 2011/6/12 consider selected local coordinate system
void CCAMDoc::OnEditMove() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnEditMove) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("对选中的特征进行平移...") ;
		OnCmd(&CCAMDoc::OnEditMove, new CMoveDlg(this), IDD_MOVEDLG) ;
		return ;
	}

	CMoveDlg* pDlg ;
	pDlg = (CMoveDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	CRefPlane* pWorkPlane = m_pWorkPlane ; // nt add 2011/6/12
	int i, n = GetSelected(ftRefPlane, 1) ; // nt add 2011/6/12
	if( n > 0 ) // nt add 2011/6/12
		pWorkPlane = (CRefPlane*)GetObj(0) ;
	n = GetSelected(-1, 1000) ; // get CFeatures
	if( n < 1 )
		camPromptStr("拾取若干特征!") ;
	else
	{
		VEC3D v, temp ;
		v[0] = pDlg->m_x ;
		v[1] = pDlg->m_y ;
		v[2] = pDlg->m_z ;
		if( pDlg->m_local &&
			pWorkPlane )
		{
			RFRAME lf ;
			pWorkPlane->GetLF(&lf) ;
			temp[0] = lf.X[0]*v[0]+lf.Y[0]*v[1]+lf.Z[0]*v[2] ;
			temp[1] = lf.X[1]*v[0]+lf.Y[1]*v[1]+lf.Z[1]*v[2] ;
			temp[2] = lf.X[2]*v[0]+lf.Y[2]*v[1]+lf.Z[2]*v[2] ;
			memcpy(v, temp, sizeof(VEC3D)) ;
		}
		BOOL copy = pDlg->m_copy ;
		COA fs ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			fs.Add(pF) ;
		}
		if( copy )
		{
			COA copied_fs ;
			m_pPart->Copy(fs, copied_fs) ;
			m_pPart->Move(copied_fs, v) ;
		}
		else
			m_pPart->Move(fs, v) ;
		Redraw() ;
	}
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// nt add 2009/5/27
// nt rewrite 2010/8/28
// nt modify 2011/6/12 consider selected local coordinate system
void CCAMDoc::OnEditRotate() 
{
	int nStep = ChkCmd(&CCAMDoc::OnEditRotate) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("对选中的特征进行旋转...") ;
		OnCmd(&CCAMDoc::OnEditRotate, new CRotDlg(), IDD_ROTDLG) ;
		return ;
	}
	
	CRotDlg* pDlg ;
	pDlg = (CRotDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	CRefPlane* pWorkPlane = m_pWorkPlane ; // nt add 2011/6/12
	int i, n = GetSelected(ftRefPlane, 1) ; // nt add 2011/6/12
	if( n > 0 ) // nt add 2011/6/12
		pWorkPlane = (CRefPlane*)GetObj(0) ;
	n = GetSelected(-1, 1000) ; // get CFeatures
	if( n < 1 )
		camPromptStr("拾取若干特征!") ;
	else
	{
		RFRAME lf ;
		if( pDlg->m_local &&
			pWorkPlane )
			pWorkPlane->GetLF(&lf) ;
		else
			mathInitRFrame(&lf) ;
		BOOL copy = pDlg->m_copy ;
		double angle = pDlg->m_angle/180*PI1 ;
		VEC3D axis ; // nt add 2011/6/12
		if( pDlg->m_axis == 0 ) // nt add 2011/6/12
			memcpy(axis, lf.X, sizeof(VEC3D)) ;
		else
		if( pDlg->m_axis == 1 ) // nt add 2011/6/12
			memcpy(axis, lf.Y, sizeof(VEC3D)) ;
		else
			memcpy(axis, lf.Z, sizeof(VEC3D)) ;
		COA fs ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			fs.Add(pF) ;
		}
		if( copy )
		{
			COA copied_fs ;
			m_pPart->Copy(fs, copied_fs) ;
			m_pPart->Rotate(copied_fs, lf.O, axis, angle) ;
		}
		else
			m_pPart->Rotate(fs, lf.O, axis, angle) ;
		Redraw() ;
	}
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// nt add 2010/9/11
void CCAMDoc::OnEditScale() 
{
	int nStep = ChkCmd(&CCAMDoc::OnEditScale) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("对选中的特征进行比例变换...") ;
		CString name = _T("比例") ;
		OnCmd(&CCAMDoc::OnEditScale, new CLenDlg(name, 1.), IDD_LENGTH) ;
		return ;
	}
	
	CLenDlg* pDlg ;
	pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	double scale = pDlg->m_len ;
	if( fabs(scale-1.) > MIN_DBL )
	{
		int n = GetSelected(-1, 10000) ; // get CFeatures
		for( int i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			if( pF )
				pF->Scale(scale) ;
		}
		Redraw() ;
	}
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// nt add 2010/11/25
void CCAMDoc::OnEditMirror() 
{
	int nStep = ChkCmd(&CCAMDoc::OnEditMirror) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("对选中的特征进行镜像变换...") ;
		CMirrorDlg* pDlg = new CMirrorDlg() ;
		if( GetSelected(ftRefPlane, 1) > 0 )
		{
			CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
			RFRAME lf ;
			pRefPlane->GetLF(&lf) ;
			pDlg->OnSelectedMirrorPlane(&lf) ;
		}
		OnCmd(&CCAMDoc::OnEditMirror, pDlg, IDD_MIRRORDLG) ;
		return ;
	}
	
	CMirrorDlg* pDlg ;
	pDlg = (CMirrorDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	if( pDlg->m_mirrorPlane )
	{
		int n = GetSelected(-1, 10000) ; // get CFeatures
		if( n > 0 )
		{
			COA fs ;
			BOOL copy = TRUE ;
			for( int i = 0 ; i < n ; i++ )
			{
				CFeature* pF = (CFeature*)GetObj(i) ;
				fs.Add(pF) ;
			}
			if( copy )
			{
				COA copied_fs ;
				m_pPart->Copy(fs, copied_fs) ;
				m_pPart->Mirror(copied_fs, pDlg->m_lf.O, pDlg->m_lf.Z) ;
			}
			else
				m_pPart->Mirror(fs, pDlg->m_lf.O, pDlg->m_lf.Z) ;
			Redraw() ;
		}
	}
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// nt add 2010/11/26
void CCAMDoc::OnEditColor() 
{
	OPTION* option = camGetOption() ;
	int nStep = ChkCmd(&CCAMDoc::OnEditColor) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("编辑当前颜色...") ;
		CColorDlg* pDlg = new CColorDlg() ;
		pDlg->m_colorIndex = option->colorIndex ;
		pDlg->m_materialIndex = option->materialIndex ;
		OnCmd(&CCAMDoc::OnEditColor, pDlg, IDD_COLORDLG) ;
		return ;
	}
	
	CColorDlg* pDlg ;
	pDlg = (CColorDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	option->colorIndex = pDlg->m_colorIndex ;
	option->materialIndex = pDlg->m_materialIndex ;
	int i, n = GetSelected(-1, 10000), type ;
	for( i = 0 ; i < n ; i++ )
	{
		type = GetType(i) ;
		if( type < 50 )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			if( pF )
			{
				if( type == ftRefSur )
					pF->SetColor(option->materialIndex+256) ;
				else
					pF->SetColor(option->colorIndex) ;
			}
		}
	}
	if( n > 0 )
		Redraw() ;
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// nt add 2010/11/27
void CCAMDoc::OnEditWidth() 
{
	OPTION* option = camGetOption() ;
	int nStep = ChkCmd(&CCAMDoc::OnEditWidth) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("编辑当前线宽...") ;
		CWidthDlg * pDlg = new CWidthDlg() ;
		pDlg->SetWidth(option->width) ;
		OnCmd(&CCAMDoc::OnEditWidth, pDlg, IDD_WIDTHDLG) ;
		return ;
	}
	
	CWidthDlg* pDlg ;
	pDlg = (CWidthDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	if( option )
	{
		option->width = pDlg->GetWidth() ;
		int i, n = GetSelected(-1, 10000), type ;
		for( i = 0 ; i < n ; i++ )
		{
			type = GetType(i) ;
			if( type == ftRefCur )
			{
				CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
				pRefCur->SetWidth(option->width) ;
			}
		}
		if( n > 0 )
			Redraw() ;
	}
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

void CCAMDoc::OnEditMove2Ori() 
{
	int i, n = GetSelected(-1, 100000) ; // get CFeatures
	VEC3D v ;
	BOX3D box, all ;
	COA fs ;

	if( n < 1 )
		camPromptStr("拾取若干特征!") ;
	else
	{
		mathClnBox(&all) ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			pF->GetBox(&box) ;
			mathCalBoxUnion(&box, &all, &all) ;
			fs.Add(pF) ;
		}
		v[0] = -0.5*(all.min[0]+all.max[0]) ;
		v[1] = -0.5*(all.min[1]+all.max[1]) ;
		v[2] = -0.5*(all.min[2]+all.max[2]) ;
		
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)GetObj(i) ;
			pF->Move(v) ;
		}

		Redraw() ;
	}

	return ;
}
//==============================================================
/*	2001/11/20 nt
 *	insert a current work plane
 */
void CCAMDoc::OnInsertWPlane() 
{
	// TODO: Add your command handler code here
	int n = GetSelected(ftRefPlane, 1) ; // get refplane
	if( n < 1 )
		m_pWorkPlane = NULL ;
	else
		m_pWorkPlane = (CRefPlane*)GetObj(0) ;
	Redraw() ;

	return ;
}

/*	2001/11/24 nt
 *	insert a refplane which is an offset of another refplane
 *	nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertOffset() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertOffset) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("对选中的基准面进行等距...") ;
		OnCmd(&CCAMDoc::OnInsertOffset, new CLenDlg(), IDD_LENGTH) ;
		return ;
	}

	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1) ; // get refplane
	if( n > 0 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		lf.O[0] += lf.Z[0]*pDlg->m_len ; // translate local frame along z
		lf.O[1] += lf.Z[1]*pDlg->m_len ;
		lf.O[2] += lf.Z[2]*pDlg->m_len ;
		pRefPlane = new CRefPlane(rpOffset, &lf) ;
		AddFeature(pRefPlane) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;
	
	return ;
}

/*	2001/11/24 nt
 *	insert a normal plane of a curve to model part as a refplane
 */
void CCAMDoc::OnInsertNormalPlane() 
{
	// TODO: Add your command handler code here
	camPromptStr("首先拾取一条曲线") ;
	int n = GetSelected(ftRefCur, 1) ; // get one curve
	if( n == 1 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		double u = GetU(0) ;

		RFRAME lf ;
		lf.scale = 1.0 ;
		double c ;
		if( curveEvalCur(curve, u, lf.O, lf.Z, lf.X, lf.Y, &c) == ERSUCSS )
		{
			CRefPlane* pRefPlane = new CRefPlane(rpNormal, &lf) ;
			AddFeature(pRefPlane) ;
		}
	}

	return ;	
}

/*	2001/11/26 nt
 *	create a refplane by three points on curve
 *	nt modify 2010/11/26
 */
void CCAMDoc::OnInsertPlane3P() 
{
	// TODO: Add your command handler code here
	return ;
}

/*	2001/11/26 nt
 *	create a refplane by two points on curve
 */
void CCAMDoc::OnInsertPlane2P() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取两条曲线") ;
	int i, n = GetSelected(ftRefCur, 2) ; // get two curve
	if( n == 2 )
	{
		CURVE* curves[2] ;
		PNT3D p1, p2 ;
		VEC3D tvec1, tvec2 ;
		double us[2] ;
		RFRAME lf ;
		for( i = 0 ; i < 2 ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			curves[i] = pRefCur->GetCurve() ;
			us[i] = GetU(i) ;
		}

		if( curveEvalTangent(curves[0], us[0], p1, tvec1) == ERSUCSS &&
			curveEvalTangent(curves[1], us[1], p2, tvec2) == ERSUCSS &&
			mathDist(p1, p2) > 1.0e-5 )
		{
			lf.O[0] = 0.5*(p1[0]+p2[0]) ;
			lf.O[1] = 0.5*(p1[1]+p2[1]) ;
			lf.O[2] = 0.5*(p1[2]+p2[2]) ;
			mathGetVec(p1, p2, lf.X) ;
			mathUniVec(lf.X, MIN_DBL) ;
			if( mathOProduct(tvec1, tvec2) < 0.0 )
			{
				tvec2[0] = -tvec2[0] ;
				tvec2[1] = -tvec2[1] ;
				tvec2[2] = -tvec2[2] ;
			}
			lf.Z[0] = 0.5*(tvec1[0]+tvec2[0]) ;
			lf.Z[1] = 0.5*(tvec1[1]+tvec2[1]) ;
			lf.Z[2] = 0.5*(tvec1[2]+tvec2[2]) ;
			mathVProduct(lf.Z, lf.X, lf.Y) ;
			mathUniVec(lf.Y, MIN_DBL) ;
			mathVProduct(lf.X, lf.Y, lf.Z) ;
			mathUniVec(lf.Z, MIN_DBL) ;
			mathUniVec(lf.Z, MIN_DBL) ;
			lf.scale = 1.0 ;
			CRefPlane* pRefPlane = new CRefPlane(0, &lf) ;
			AddFeature(pRefPlane) ;
		}
	}

	return ;
}

/*	2001/11/26 nt
 *	create a refplane by rotating a existing refplane around the x axis
 *  nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertPlaneX() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPlaneX) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取一个基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertPlaneX, new CAngDlg(), IDD_ANGLE) ;
		return ;
	}

	int n = GetSelected(ftRefPlane, 1) ; // one refplane
	if( n == 1 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		CAngDlg* pDlg = (CAngDlg*)m_pDlg ;
		if( !pDlg )
			return ;
		mathRotVec(lf.X, lf.O, pDlg->m_ang/180.0*PI1, lf.Y, lf.Y) ;
		mathRotVec(lf.X, lf.O, pDlg->m_ang/180.0*PI1, lf.Z, lf.Z) ;
		pRefPlane = new CRefPlane(0, &lf) ;
		AddFeature(pRefPlane) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2001/11/26 nt
 *	create a refplane by rotating a existing refplane around the y axis
 *  nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertPlaneY() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPlaneY) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取一个基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertPlaneY, new CAngDlg(), IDD_ANGLE) ;
		return ;
	}

	int n = GetSelected(ftRefPlane, 1) ; // one refplane
	if( n == 1 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		CAngDlg* pDlg = (CAngDlg*)m_pDlg ;
		if( !pDlg )
			return ;
		mathRotVec(lf.Y, lf.O, pDlg->m_ang/180.0*PI1, lf.X, lf.X) ;
		mathRotVec(lf.Y, lf.O, pDlg->m_ang/180.0*PI1, lf.Z, lf.Z) ;
		pRefPlane = new CRefPlane(0, &lf) ;
		AddFeature(pRefPlane) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;
	
	return ;	
}

/*	2001/11/26 nt
 *	create a refplane by rotating a existing refplane around the z axis
 *  nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertPlaneZ() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPlaneZ) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取一个基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertPlaneZ, new CAngDlg(), IDD_ANGLE) ;
		return ;
	}

	int n = GetSelected(ftRefPlane, 1) ; // one refplane
	if( n == 1 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		CAngDlg* pDlg = (CAngDlg*)m_pDlg ;
		if( !pDlg )
			return ;
		mathRotVec(lf.Z, lf.O, pDlg->m_ang/180.0*PI1, lf.Y, lf.Y) ;
		mathRotVec(lf.Z, lf.O, pDlg->m_ang/180.0*PI1, lf.X, lf.X) ;
		pRefPlane = new CRefPlane(0, &lf) ;
		AddFeature(pRefPlane) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}

/*	2003/5/9 nt
 *	insert surface tangent plane
 *  nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertTanPln() 
{
	// TODO: Add your command handler code here
	camPromptStr("首先拾取一张曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // get one surface
	if( n == 1 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		double u = GetU(0) ;
		double v = GetV(0) ;

		// nt add 2010/11/3
		n = GetSelected(ftRefPt, 1) ;
		if( n > 0 )
		{
			CRefPt* pRefPt = (CRefPt*)GetObj(0) ;
			PNT3D p, q ;
			pRefPt->GetP(p) ;
			u = MAX_DBL ;
			v = MAX_DBL ;
			surfaceSnap(surface, p, q, &u, &v) ;
		} 

		RFRAME lf ;
		lf.scale = 1.0 ;
		if( surfaceEvalNormal(surface, u, v, lf.O, lf.Z) == ERSUCSS )
		{
			mathGetXYFromZ(lf.Z, lf.X, lf.Y) ;
			CRefPlane* pRefPlane = new CRefPlane(rpNormal, &lf) ;
			AddFeature(pRefPlane) ;
		}
	}

	return ;	
}

/*	2002/5/29 nt
 *	move the origin of work plane
 *  nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertPlaneMove() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPlaneMove) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取一个基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertPlaneMove, new CVecDlg(), IDD_VECTOR) ;
		return ;
	}

	CVecDlg* pDlg = (CVecDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1) ; // one refplane
	if( n == 1 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		lf.O[0] += (lf.X[0]*pDlg->m_x+lf.Y[0]*pDlg->m_y+lf.Z[0]*pDlg->m_z) ;
		lf.O[1] += (lf.X[1]*pDlg->m_x+lf.Y[1]*pDlg->m_y+lf.Z[1]*pDlg->m_z) ;
		lf.O[2] += (lf.X[2]*pDlg->m_x+lf.Y[2]*pDlg->m_y+lf.Z[2]*pDlg->m_z) ;
		pRefPlane = new CRefPlane(0, &lf) ;
		AddFeature(pRefPlane) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}
//==============================================================
/*	2006/8/1 nt
 *	insert refpt
 *	nt rewrite 2010/8/29
 *	nt rewrite 2010/11/26
 */
void CCAMDoc::OnInsertRefPt() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertRefPt) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		ClearCmdAll() ;
		camPromptStr("输入参考点...") ;
		OnCmd(&CCAMDoc::OnInsertRefPt, new CRefPtDlg(), IDD_REFPTDLG) ;
		return ;
	}

	CRefPtDlg* pDlg = (CRefPtDlg*)m_pDlg ;
	if( !pDlg )
		return ;
	int i, n ;
	if( pDlg->m_mode == 1 ) // 输入坐标点
	{
		RFRAME lf ;
		if( m_pWorkPlane )
			m_pWorkPlane->GetLF(&lf) ;
		else
			mathInitRFrame(&lf) ;
		PNT3D p ;
		for( i = 0 ; i < pDlg->m_n ; i++ )
		{
			mathTransLocalPnt3D(&lf, pDlg->m_ps[i], p) ;
			CRefPt* pPt = new CRefPt(0, p) ;
			AddFeature(pPt) ;
		}
	}
	else
	if( pDlg->m_mode == 2 && // 等分点
		pDlg->m_nSegment > 0 )
	{
		n = GetSelected(ftRefCur, 10000) ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			if( pRefCur )
			{
				CURVE* curve = pRefCur->GetCurve() ;
				INTERVAL interval ;
				pRefCur->GetInterval(&interval) ;
				for( int k = 0 ; k <= pDlg->m_nSegment ; k++ )
				{
					double t = interval.t1+k*(interval.t2-interval.t1)/pDlg->m_nSegment ;
					PNT3D p ;
					curveEvalPos(curve, t, p) ;
					CRefPt* pPt = new CRefPt(0, p) ;
					AddFeature(pPt) ;
				}
			}
		}
	}
	else
	if( pDlg->m_mode == 3 ) // 圆心
	{
		n = GetSelected(ftRefCur, 10000) ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			if( pRefCur )
			{
				CURVE* curve = pRefCur->GetCurve() ;
				int type = entityGetType(curve) ;
				if( type == etCircle ||
					type == etEllipse )
				{
					PNT3D p1, p2, center ;
					curveEvalPos(curve, 0., p1) ;
					curveEvalPos(curve, PI1, p2) ;
					mathMidPnt(p1, p2, center) ;
					CRefPt* pPt = new CRefPt(0, center) ;
					AddFeature(pPt) ;
				}
			}
		}
	}
	else
	if( pDlg->m_mode == 4 ) // 端点
	{
		n = GetSelected(ftRefCur, 10000) ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			if( pRefCur )
			{
				PNT3D b, e ;
				pRefCur->GetBegin(b) ;
				pRefCur->GetEnd(e) ;
				CRefPt* pPt = new CRefPt(0, b) ;
				AddFeature(pPt) ;
				if( mathDist(b, e) > MIN_LEN )
				{
					pPt = new CRefPt(0, e) ;
					AddFeature(pPt) ;
				}
			}
		}
	}
	else
	if( pDlg->m_mode == 5 ) // 中点
	{
		n = GetSelected(ftRefCur, 10000) ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			if( pRefCur )
			{
				CURVE* curve = pRefCur->GetCurve() ;
				INTERVAL interval ;
				pRefCur->GetInterval(&interval) ;
				PNT3D midp ;
				curveEvalPos(curve, 0.5*(interval.t1+interval.t2), midp) ;
				CRefPt* pPt = new CRefPt(0, midp) ;
				AddFeature(pPt) ;
			}
		}
	}
	else
	if( pDlg->m_mode == 6 ) // 交互开始
	{ // start interactively draw CRefPt
		POSITION pos = GetFirstViewPosition() ;
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( !pView )
			return ;
		CDrawPt* pDP = new CDrawPt(this, pView) ;
		if( !pDP )
			return ;
		m_pCmd = pDP ;
	}
	else
	if( pDlg->m_mode == 7 ) // 交互结束
	{
		if( m_pCmd )
		{
			delete m_pCmd ;
			m_pCmd = NULL ;
		}
	}

	if( m_ui == 1 )
	{
		if( m_pCmd ) // nt add 2010/11/26
		{
			delete m_pCmd ;
			m_pCmd = NULL ;
		}
		CmdEnd() ;
	}
	SetLastCmd(ID_INSERT_REFPT) ;

	return ;
}
/*-----------------------------------------------------------*/
/*	2001/11/22 nt
 *	insert b-spline curves that interpolating given points
 */
void CCAMDoc::OnInsertInterp() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;

	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( !pView )
		return ;
	CDynSpln* pDynSpln = new CDynSpln(this, pView) ;
	if( !pDynSpln )
		return ;
	void* pVI = pView->GetVI() ;
	double d = viGetScale(pVI) ;
	if( pDynSpln->Create(10.0*d, 0.5*d, 256) != 1 ||
		pDynSpln->Start() != 1 )
	{
		delete pDynSpln ;
		return ;
	}
	pDynSpln->SetColor(RGB(0, 255, 0)) ;
	m_pCmd = pDynSpln ;
	SetLastCmd(ID_INSERT_INTP) ;

	return ;
}

void CCAMDoc::OnInsertLin() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;

	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( !pView )
		return ;
	CDrawLin* pDL = new CDrawLin(this, pView) ;
	if( !pDL )
		return ;
	m_pCmd = pDL ;
	SetLastCmd(ID_INSERT_LIN) ;

	return ;
}

// nt rewrite 2010/6/29
void CCAMDoc::OnInsertCircle() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertCircle) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		ClearCmdAll() ;
		camPromptStr("首先拾取基准平面...") ;
		// start interactively draw CRefPt--------------------------
		POSITION pos = GetFirstViewPosition() ;
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( !pView )
			return ;
		CDrawCir* pDrawCir = new CDrawCir(this, pView) ;
		if( !pDrawCir )
			return ;
		m_pCmd = pDrawCir ;
		// end------------------------------------------------------

		CString name = _T("半径") ;
		OnCmd(&CCAMDoc::OnInsertCircle, new CLenDlg(name, 25.), IDD_LENGTH) ;
		return ;
	}

	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int i, n = GetSelected(ftRefPlane, 1000) ;
	if( n > 0 )
	{
		for( i = 0 ; i < n ; i++ )
		{
			CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
			RFRAME lf ;
			pRefPlane->GetLF(&lf) ;
			CURVE* circle = circleCreate(&lf, pDlg->m_len) ;
			if( !circle )
				return ;
			CRefCur* pRefCur = new CRefCur(0, circle, NULL) ;
			AddFeature(pRefCur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt rewrite 2010/8/29
void CCAMDoc::OnInsertEll() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertEll) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertEll, new CEllDlg(50., 25.), IDD_ELLDLG) ;
		return ;
	}

	CEllDlg* pDlg = (CEllDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1000) ;
	if( n > 0 )
	{
		double r1, r2 ;
		r1 = mathMAX(pDlg->m_r1, pDlg->m_r2) ;
		r2 = mathMIN(pDlg->m_r1, pDlg->m_r2) ;
		for( int i = 0 ; i < n ; i++ )
		{
			CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
			RFRAME lf ;
			pRefPlane->GetLF(&lf) ;
			CURVE* ellipse = ellipseCreate(&lf, r1, r2) ;
			if( !ellipse )
				return ;
			CRefCur* pRefCur = new CRefCur(0, ellipse, NULL) ;
			AddFeature(pRefCur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2009/5/7 : 给定半径、起始角和终止角绘制圆弧
// nt rewrite 2010/8/29
void CCAMDoc::OnInsertArcRAA() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertArcRAA) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertArcRAA, new CRAADlg(), IDD_RAADLG) ;
		return ;
	}

	CRAADlg* pDlg = (CRAADlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1) ; // get refplane
	if( n == 1 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		CURVE* circle = circleCreate(&lf, pDlg->m_r) ;
		if( circle )
		{
			INTERVAL interval ;
			interval.t1 = pDlg->m_a1/180*PI1 ;
			interval.t2 = pDlg->m_a2/180*PI1 ;
			CRefCur*  pRefCur = new CRefCur(0, circle, &interval) ;
			AddFeature(pRefCur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2009/5/7 : 给两条首尾相连的线段绘制圆弧
// nt rewrite 2010/8/29
void CCAMDoc::OnInsertArcLL() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertArcLL) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取两条首尾相连的线段...") ;
		CString name = _T("半径") ;
		OnCmd(&CCAMDoc::OnInsertArcLL, new CLenDlg(name, 10.), IDD_LENGTH) ;
		return ;
	}

	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefCur, 2) ; // get CRefCurs
	if( n == 2 )
	{
		CRefCur* pRefCur1 = (CRefCur*)GetObj(0) ;
		CURVE* curve1 = pRefCur1->GetCurve() ;
		INTERVAL interval1 ;
		pRefCur1->GetInterval(&interval1) ;
		CRefCur* pRefCur2 = (CRefCur*)GetObj(1) ;
		CURVE* curve2 = pRefCur2->GetCurve() ;
		INTERVAL interval2 ;
		pRefCur2->GetInterval(&interval2) ;

		PNT3D p1, p2, q1, q2 ;
		VEC3D tvec1, tvec2 ;
		curveEvalPos(curve1, interval1.t1, p1) ;
		curveEvalPos(curve1, interval1.t2, p2) ;
		curveEvalPos(curve2, interval2.t1, q1) ;
		curveEvalPos(curve2, interval2.t2, q2) ;
		if( mathDist(p1, q1) < MIN_LEN )
		{
			curveEvalTangent(curve1, interval1.t1, p1, tvec1) ; // 起点
			curveEvalTangent(curve2, interval2.t1, q1, tvec2) ; // 起点
		}
		else
		if( mathDist(p1, q2) < MIN_LEN )
		{
			curveEvalTangent(curve1, interval1.t1, p1, tvec1) ; // 起点
			curveEvalTangent(curve2, interval2.t2, q1, tvec2) ; // 终点
			tvec2[0] = -tvec2[0] ;
			tvec2[1] = -tvec2[1] ;
			tvec2[2] = -tvec2[2] ;
		}
		else
		if( mathDist(p2, q1) < MIN_LEN )
		{
			curveEvalTangent(curve1, interval1.t2, p1, tvec1) ; // 终点
			tvec1[0] = -tvec1[0] ;
			tvec1[1] = -tvec1[1] ;
			tvec1[2] = -tvec1[2] ;
			curveEvalTangent(curve2, interval2.t1, q1, tvec2) ; // 起点
		}
		else
		if( mathDist(p2, q2) < MIN_LEN )
		{
			curveEvalTangent(curve1, interval1.t2, p1, tvec1) ; // 终点
			tvec1[0] = -tvec1[0] ;
			tvec1[1] = -tvec1[1] ;
			tvec1[2] = -tvec1[2] ;
			curveEvalTangent(curve2, interval2.t2, q1, tvec2) ; // 终点
			tvec2[0] = -tvec2[0] ;
			tvec2[1] = -tvec2[1] ;
			tvec2[2] = -tvec2[2] ;
		}
		else
		{
			camPromptStr("错误:端点不重合!") ;
			return ;
		}
		RFRAME lf ;
		lf.scale = 1. ;
		lf.O[0] = 0.5*(p1[0]+q1[0]) ;
		lf.O[1] = 0.5*(p1[1]+q1[1]) ;
		lf.O[2] = 0.5*(p1[2]+q1[2]) ;
		double a = mathGetAngleUnit(tvec1, tvec2) ;
		if( a < 1000.*MIN_ANG ||
			a > PI1-1000.*MIN_ANG )
		{
			camPromptStr("错误:端点切向量重合!") ;
			return ;
		}
		mathVProduct(tvec1, tvec2, lf.Z) ;
		mathUniVec(lf.Z, MIN_DBL) ;
		lf.X[0] = -(tvec1[0]+tvec2[0]) ;
		lf.X[1] = -(tvec1[1]+tvec2[1]) ;
		lf.X[2] = -(tvec1[2]+tvec2[2]) ;
		mathUniVec(lf.X, MIN_DBL) ;
		mathVProduct(lf.Z, lf.X, lf.Y) ;
		
		double d = pDlg->m_len/sin(0.5*a) ;
		lf.O[0] -= lf.X[0]*d ;
		lf.O[1] -= lf.X[1]*d ;
		lf.O[2] -= lf.X[2]*d ;
		CURVE* circle = circleCreate(&lf, pDlg->m_len) ;
		if( circle )
		{
			INTERVAL interval ;
			interval.t1 = 0.5*(a-PI1) ;
			interval.t2 = 0.5*(PI1-a) ;
			CRefCur* pRefCur = new CRefCur(0, circle, &interval) ;
			AddFeature(pRefCur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2009/5/27 : Arc
void CCAMDoc::OnInsertArc() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;

	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( !pView )
		return ;
	CDrawArc* pArc = new CDrawArc(this, pView) ;
	if( !pArc )
		return ;
	void* pVI = pView->GetVI() ;
	if( !pVI )
	{
		delete pArc ;
		return ;
	}
	m_pCmd = pArc ;
	SetLastCmd(ID_INSERT_ARC) ;

	return ;
}

// nt add 2009/5/30 : 以线段中点为圆心、其长度为直径作圆
// nt modify 2010/8/29
void CCAMDoc::OnInsertLine2Cir() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet ||
		!m_pPart )
		return ;
	camPromptStr("拾取一条或多条线段[一个参考平面]...") ;
	COA ses1, ses2 ;
	int n1 = m_pSelSet->GetSelected(ftRefCur, 1000, ses1) ; // CRefCur
	int n2 = m_pSelSet->GetSelected(ftRefPlane, 1, ses2) ; // CRefPlane
	if( n1 > 0 )
	{
		int i ;
		RFRAME lf ;
		double u, r ;
		INTERVAL interval ;
		CRefPlane* pRefPlane ;
		CRefCur* pRefCur ;
		CURVE* curve ;

		// get unit Z direction
		if( n2 > 0 )
		{
			pRefPlane = (CRefPlane*)ses2[0] ;
			pRefPlane->GetLF(&lf) ;
		}
		else
		{
			if( n1 < 2 )
				return ;
			pRefCur = (CRefCur*)ses1[n1-1] ;
			u = pRefCur->GetU() ;
			curve = pRefCur->GetCurve() ;
			curveEvalTangent(curve, u, lf.O, lf.Z) ;
			n1-- ;
		}

		lf.scale = 1. ;
		for( i = 0 ; i < n1 ; i++ )
		{
			pRefCur = (CRefCur*)ses1[i] ;
			pRefCur->GetInterval(&interval) ;
			curve = pRefCur->GetCurve() ;
			if( entityGetType(curve) == etLine )
			{
				curveEvalTangent(curve, 
								 0.5*(interval.t1+interval.t2),
								 lf.O,
								 lf.X) ;
				mathPrjVecPln(lf.Z, lf.X, lf.Z) ;
				if( mathUniVec(lf.Z, MIN_LEN) == ERSUCSS )
				{
					mathVProduct(lf.Z, lf.X, lf.Y) ;
					r = 0.5*fabs(interval.t2-interval.t1) ;
					curve = circleCreate(&lf, r) ;
					pRefCur = new CRefCur(0, curve, NULL) ;
					AddFeature(pRefCur) ;
				}
			}
		}
	}

	return ;
}
//--------------------------------------------------------------
// nt add 2010/1/23
// nt rewrite 2010/8/29
void CCAMDoc::OnInsertFilletCir()
{
	return ;
}

// nt add 2010/1/23
// nt add 2010/3/25
// 与给定直线相切但不过给定原始曲线的端点
// nt modify 2010/9/2 : 增加处理几种情况
void CCAMDoc::OnInsertFilletCur()
{
	return ;
}
//--------------------------------------------------------------
// nt add 2009/5/7 : Bezier curve
void CCAMDoc::OnInsertBC() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;
	camPromptStr("用鼠标依次输入Bezier曲线的控制顶点...") ;
	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( !pView )
		return ;
	CDrawBC* pBC = new CDrawBC(this, pView) ;
	if( !pBC )
		return ;
	m_pCmd = pBC ;

	return ;
}
//--------------------------------------------------------------
/*	2002/5/31 nt
 *	read and make a group of curves
 */
void CCAMDoc::OnInsertCurves() 
{
	// TODO: Add your command handler code here
}
//--------------------------------------------------------------
/*	2010/1/20 nt
 *	define curve on surface
 *	沿pRefPlane的Z轴方向投影
 */
void CCAMDoc::OnInsertUVCurve() 
{
	// TODO: Add your command handler code here
	return ;
}
//--------------------------------------------------------------
// nt add 2010/1/20
// 曲面的4条边界
void CCAMDoc::OnInsertSurBorder() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取一张或多张参考曲面...") ;
	int i, n = GetSelected(ftRefSur, 1000) ; // get all reference surfaces
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		int uvflags[4] = {0, 0, 1, 1} ;
		double uvs[4] ;
		uvs[0] = boxuv.min[0] ;
		uvs[1] = boxuv.max[0] ;
		uvs[2] = boxuv.min[1] ;
		uvs[3] = boxuv.max[1] ;
		CURVE* curve ;
		INTERVAL interval ;
		PNT3D dege_p ;
		for( int k = 0 ; k < 4 ; k++ )
		{
			if( surfaceGetIsoCurve(surface, 
									uvflags[k],
									uvs[k],
									&curve, 
									&interval,
									dege_p) == ERSUCSS )
			{
				if( k < 2 )
				{
					interval.t1 = mathMAX(interval.t1, boxuv.min[1]) ;
					interval.t2 = mathMIN(interval.t2, boxuv.max[1]) ;
				}
				else
				{
					interval.t1 = mathMAX(interval.t1, boxuv.min[0]) ;
					interval.t2 = mathMIN(interval.t2, boxuv.max[0]) ;
				}
				if( interval.t2-interval.t1 > MIN_DBL )
				{
					CRefCur* pRefCur = new CRefCur(0, 
												   curve,
												   &interval) ;
					AddFeature(pRefCur) ;
				}
			}
		}
	}

	return ;
}
//--------------------------------------------------------------
// nt add 2010/9/15 : 改进为可以交互拾取曲面再求交
void CCAMDoc::OnInsertIntCur() 
{
	// TODO: Add your command handler code here
	return ;	
}

void CCAMDoc::OnInsertMinDist()
{
	// TODO: 在此添加命令处理程序代码
}

// nt add 2010/11/27
void CCAMDoc::OnInsertIsoCur() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertIsoCur) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取一张曲面...") ;
		CIsoCurDlg* pDlg = new CIsoCurDlg() ; // create and initialize
		int n = GetSelected(ftRefSur, 1) ;
		if( n > 0 )
		{
			CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
			BOXUV boxuv ;
			pRefSur->GetBoxUV(&boxuv) ;
			pDlg->m_pRefSur = pRefSur ;
			pDlg->m_selectedSur = TRUE ;
			pDlg->SetBoxUV(boxuv.min[0], 
						   boxuv.max[0], 
						   boxuv.min[1], 
						   boxuv.max[1]) ;
		}
		OnCmd(&CCAMDoc::OnInsertIsoCur, pDlg, IDD_ISOCURDLG) ;
		return ;
	}

	CIsoCurDlg* pDlg = (CIsoCurDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	if( pDlg->m_selectedSur )
	{
		CRefSur* pRefSur = (CRefSur*)pDlg->m_pRefSur ;
		SURFACE* surface = pRefSur->GetSurface() ;
		if( pRefSur )
		{
			PNT3D dege_p ;
			INTERVAL interval ;
			CURVE* curve = NULL ;
			double uv = pDlg->m_uvFlag < 1 ? pDlg->m_u : pDlg->m_v ;
			if( surfaceGetIsoCurve(surface, 
								   pDlg->m_uvFlag,
								   uv,
								   &curve, 
								   &interval,
								   dege_p) == ERSUCSS &&
				curve )
			{
				if( pDlg->m_uvFlag < 1 )
				{
					interval.t1 = pDlg->m_vmin ;
					interval.t2 = pDlg->m_vmax ;
				}
				else
				{
					interval.t1 = pDlg->m_umin ;
					interval.t2 = pDlg->m_umax ;
				}
				CRefCur* pRefCur = new CRefCur(0, curve, &interval) ;
				AddFeature(pRefCur) ;
			}
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}
//==============================================================
// nt rewrite 2010/8/31
void CCAMDoc::OnInsertPlane() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPlane) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面，并输入边长...") ;
		CString name = _T("边长") ;
		OnCmd(&CCAMDoc::OnInsertPlane, new CLenDlg(name, 10.), IDD_LENGTH) ;
		return ;
	}
	
	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1000) ; // get refplane
	for( int i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		SURFACE* pPlane = planeCreate(&lf) ;
		if( pPlane )
		{
			BOXUV boxuv = {-0.5*pDlg->m_len, 
						   -0.5*pDlg->m_len, 
						   0.5*pDlg->m_len,
						   0.5*pDlg->m_len} ;
			CRefSur* pRefSur = new CRefSur(etPlane, pPlane, &boxuv) ;
			AddFeature(pRefSur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}

/*	2001/11/26 nt
 *	generate a rule b-spline surface
 *	nt modify 2010/8/31
 */
void CCAMDoc::OnInsertRule() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取两条曲线...") ;
	int n = GetSelected(ftRefCur, 2) ; // get two curve
	if( n == 2 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		BSCUR* bnd1 = pRefCur->GetNurbs() ;
		pRefCur = (CRefCur*)GetObj(1) ;
		BSCUR* bnd2 = pRefCur->GetNurbs() ;

		BSSUR* nurbs ;
		if( bnd1 && 
			bnd2 &&
			bssurCreRule(bnd1, bnd2, &nurbs) == ERSUCSS )
		{
			CRefSur* pRefSur = new CRefSur(0, nurbs) ;
			bssurFree(nurbs) ;
			AddFeature(pRefSur) ;
		}
		bscurFree(bnd1) ;
		bscurFree(bnd2) ;
	}

	return ;
}

/*	2001/11 nt
 *	create extrude surface
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnInsertExtrude() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertExtrude) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面及若干轮廓线...") ;
		CString name = _T("高度") ;
		OnCmd(&CCAMDoc::OnInsertExtrude, new CLenDlg(name, 10.), IDD_LENGTH) ;
		return ;
	}

	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1) ; // get one refplane
	if( n > 0 )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		n = GetSelected(ftRefCur, 1000) ; // get curves
		for( int i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			CURVE* profile = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			SURFACE* rulsur = rulsurCreate(profile, lf.Z) ;
			BOXUV boxuv ;
			boxuv.min[0] = interval.t1 ;
			boxuv.max[0] = interval.t2 ;
			boxuv.min[1] = mathMIN(0., pDlg->m_len) ;
			boxuv.max[1] = mathMAX(0., pDlg->m_len) ;
			CRefSur* pRefSur = new CRefSur(0, rulsur, &boxuv) ;
			AddFeature(pRefSur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}

/*	2001/11 nt
 *	create revolve surface
 *	nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertRevolve() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertRevolve) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取一条轴线、若干条母线...") ;
		OnCmd(&CCAMDoc::OnInsertRevolve, new CAngDlg(), IDD_ANGLE) ;
		return ;
	}

	CAngDlg* pDlg = (CAngDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefCur, 1000) ; // get curves
	if( n > 1 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		PNT3D pivot ;
		VEC3D axis ;
		curveEvalTangent(curve, 0.5, pivot, axis) ;
		for( int i = 1 ; i < n ; i++ )
		{
			pRefCur = (CRefCur*)GetObj(i) ;
			curve = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			SURFACE* revsur ;
			BOXUV boxuv ;
			if( entityGetType(curve) != etBCurve )
			{
				int exact ;
				BSCUR* nurbs ;
				curveToNurbs(curve, &interval, &nurbs, &exact) ;
				CURVE* profile = bcurveCreate(nurbs) ;
				bscurFree(nurbs) ;
				revsur = revsurCreate(profile, pivot, axis) ;
				entityFree(profile) ;
				boxuv.min[0] = 0. ; // ???
				boxuv.max[0] = 1. ; // ???
			}
			else
			{
				boxuv.min[0] = interval.t1 ;
				boxuv.max[0] = interval.t2 ;
				revsur = revsurCreate(curve, pivot, axis) ;
			}
			boxuv.min[1] = 0. ;
			boxuv.max[1] = pDlg->m_ang/180.0*PI1 ; // nt modify 2002/6/6
			CRefSur* pRefSur = new CRefSur(0, revsur, &boxuv) ;
			AddFeature(pRefSur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2001/11/24 nt
 *	generate 2, 3, or 4 boundaries b-spline surface
 */
void CCAMDoc::OnInsertBndSur4() 
{
	// TODO: Add your command handler code here
	camPromptStr("拾取2,3,或4条曲线作为曲面的边界...") ;
	int i, n = GetSelected(ftRefCur, 4) ; // get 2, 3 or 4 curves
	if( n == 2 ||
		n == 3 ||
		n == 4 )
	{
		BSCUR* bnds[4] = {NULL, NULL, NULL, NULL} ;
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			bnds[i] = pRefCur->GetNurbs() ;
		}
		BSSUR* nurbs ;
		if( bssurCreSkin(bnds[0], // create surf by boundaries
						 bnds[1], 
						 bnds[2],
						 bnds[3], 
						 MIN_LEN,
						 &nurbs) == ERSUCSS )
		{
			CRefSur* pRefSur = new CRefSur(0, nurbs) ;
			bssurFree(nurbs) ;
			AddFeature(pRefSur) ;
		}
		for( i = 0 ; i < n ; i++ )
			bscurFree(bnds[i]) ;
	}

	return ;
}
//--------------------------------------------------------------
/*	2001/11/26 nt
 *	generate loft surface
 */
void CCAMDoc::OnInsertLoft() 
{
	// TODO: Add your command handler code here
	return ;
}
/*-----------------------------------------------------------*/
/*	2001/11 nt
 *	sweep surface by three spine curve
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnInsertSweep() 
{
	// TODO: Add your command handler code here
	return ;	
}
/*-----------------------------------------------------------*/
/*	2001/11 nt
 *	create piping surface
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnInsertPipe() 
{
	// TODO: Add your command handler code here
	return ;	
}
//--------------------------------------------------------------
// nt rewrite 2010/8/31
void CCAMDoc::OnInsertConnect() 
{
	// TODO: Add your command handler code here
	return ;
}

void CCAMDoc::InsertConnect1()
{
	return ;	
}

// nt add 2010/1/27
// 可用于构造进排气边面
// 2010/3/18 nt rewrite
void CCAMDoc::InsertConnect2() 
{	
	return ;	
}
//--------------------------------------------------------------
// nt add 2006/7/28 N-side surface
void CCAMDoc::OnInsertFill() 
{
	// TODO: Add your command handler code here
	return ;	
}
/*-----------------------------------------------------------*/
/*	2001/11/26 nt
 *	load b-spline surface from .srf file
 */
BSSUR* loadBSSUR(FILE* fp)
{
    BSSUR* surf ;
    int ord_u, cnt_u, ord_v, cnt_v ;
    int i, j, n ;

    if( !fp )
		return NULL ;
    if( fscanf_s(fp, "%d %d %d %d", &ord_u, &cnt_u, &ord_v, &cnt_v) != 4 ||
		ord_u < 2 ||
		ord_u > 20 ||
		cnt_u < 2 ||
		cnt_u > 100000 ||
		ord_v < 2 ||
		ord_v > 20 ||
		cnt_v < 2 ||
		cnt_v > 100000 )
		return NULL ;
	surf = bssurCreate(ord_u, cnt_u, ord_v, cnt_v) ;
    if( !surf )
        return NULL ;
    for( n = ord_u+cnt_u, i = 0 ; i < n ; i++ )
	{
		if( fscanf_s(fp, "%lf", &surf->u_k[i]) != 1 )
			goto ExceExit ;
	}
    for( n = ord_v+cnt_v, j = 0 ; j < n ; j++ )
	{
        if( fscanf_s(fp, "%lf", &surf->v_k[j]) != 1 )
			goto ExceExit ;
	}

    for( i = 0 ; i < surf->cnt_u ; i++ )
	{
        for( j = 0 ; j < surf->cnt_v ; j++ )
		{
            if( fscanf_s(fp, "%lf %lf %lf %lf",
                         &surf->uv_p[i][j][0],
                         &surf->uv_p[i][j][1],
                         &surf->uv_p[i][j][2],
                         &surf->uv_p[i][j][3]) != 4 )
				goto ExceExit ;
		}
	}

    return surf ;
ExceExit:
	bssurFree(surf) ;

	return NULL ;
}

void CCAMDoc::OnInsertFileSur()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL) ;
	if( dlg.DoModal() == IDOK )
	{
		CString pathName = dlg.GetPathName() ;
		FILE* fp = NULL ;
		_tfopen_s(&fp, pathName.GetBuffer(0), _T("r")) ;
		BSSUR* nurbs = loadBSSUR(fp) ;
		fclose(fp) ;
		if( !nurbs )
		{
			camPromptStr("数据格式错误!") ;
			return ;
		}
		int i, j ;
		double s = 1.0 ;
		CScaleDlg dlg ;
		if( dlg.DoModal() == IDOK )
			s = dlg.m_scale ;
		if( fabs(s-1.0) > 1.0e-10 )
		{
			for( i = 0 ; i < nurbs->cnt_u ; i++ )
			{
				for( j = 0 ; j < nurbs->cnt_v ; j++ )
				{
					nurbs->uv_p[i][j][0] *= s ;
					nurbs->uv_p[i][j][1] *= s ;
					nurbs->uv_p[i][j][2] *= s ;
				}
			}
		}
		CRefSur* pRefSur = new CRefSur(0, nurbs) ;
		bssurFree(nurbs) ;
		AddFeature(pRefSur) ;
	}

	return ;
}
/*-----------------------------------------------------------*/
/*	2001/12/1 nt
 *	生成圆柱面
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnInsertCyl() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertCyl) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面、输入半径和高度...") ;
		OnCmd(&CCAMDoc::OnInsertCyl, new CRHDlg(), IDD_RHDLG) ;
		return ;
	}

	CRHDlg* pDlg = (CRHDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1000) ;
	for( int i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		SURFACE* cyl = cylinderCreate(&lf, pDlg->m_r) ;
		BOXUV boxuv ;
		boxuv.min[0] = 0.0 ;
		boxuv.max[0] = PI2 ;
		boxuv.min[1] = 0.0 ;
		boxuv.max[1] = pDlg->m_h ;
		CRefSur* pRefSur = new CRefSur(0, cyl, &boxuv) ;
		AddFeature(pRefSur) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}

/*	2001/12/1 nt
 *	生成球面
 *	nt rewrite 2010/8/31
 *	nt modify 2010/11/26
 */
void CCAMDoc::OnInsertSph() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertSph) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面、点、线段等，并输入半径...") ;
		CString name = _T("半径") ;
		OnCmd(&CCAMDoc::OnInsertSph, new CLenDlg(name, 10.), IDD_LENGTH) ;
		return ;
	}
	
	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int i, n = GetSelected(ftRefPlane, 10000) ;
	RFRAME lf ;
	mathInitRFrame(&lf) ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		pRefPlane->GetLF(&lf) ;
		SURFACE* sph = sphereCreate(&lf, pDlg->m_len) ;
		CRefSur* pRefSur = new CRefSur(0, sph, NULL) ;
		AddFeature(pRefSur) ;
	}
	n = GetSelected(ftRefPt, 10000) ;
	mathInitRFrame(&lf) ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefPt* pRefPt = (CRefPt*)GetObj(i) ;
		pRefPt->GetP(lf.O) ;
		SURFACE* sph = sphereCreate(&lf, pDlg->m_len) ;
		CRefSur* pRefSur = new CRefSur(0, sph, NULL) ;
		AddFeature(pRefSur) ;
	}
	n = GetSelected(ftRefCur, 10000) ;
	mathInitRFrame(&lf) ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		CURVE* curve = pRefCur->GetCurve() ;
		PNT3D p1, p2 ;
		double r ;
		SURFACE* sph ;
		CRefSur* pRefSur ;
		int type = entityGetType(curve) ;
		if( type == etLine )
		{
			curveEvalPos(curve, interval.t1, p1) ;
			curveEvalPos(curve, interval.t2, p2) ;
			mathMidPnt(p1, p2, lf.O) ;
			r = 0.5*mathDist(p1, p2) ;
			sph = sphereCreate(&lf, r) ;
			pRefSur = new CRefSur(0, sph, NULL) ;
			AddFeature(pRefSur) ;
		}
		else
		if( type == etCircle )
		{
			curveEvalPos(curve, 0., p1) ;
			curveEvalPos(curve, PI1, p2) ;
			mathMidPnt(p1, p2, lf.O) ;
			r = 0.5*mathDist(p1, p2) ;
			curveEvalPos(curve, 0.5*PI1, p2) ;
			mathGetVec(lf.O, p1, lf.X) ;
			mathGetVec(lf.O, p2, lf.Y) ;
			mathUniVec(lf.X, MIN_LEN) ;
			mathUniVec(lf.Y, MIN_LEN) ;
			mathVProduct(lf.X, lf.Y, lf.Z) ;
			mathUniVec(lf.Z, MIN_LEN) ;
			lf.scale = 1. ;
			sph = sphereCreate(&lf, r) ;
			pRefSur = new CRefSur(0, sph, NULL) ;
			AddFeature(pRefSur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2001/12/1 nt
 *	生成圆锥面
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnInsertCone() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertCone) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面、输入半径和高度...") ;
		OnCmd(&CCAMDoc::OnInsertCone, new CRHDlg(), IDD_RHDLG) ;
		return ;
	}

	CRHDlg* pDlg = (CRHDlg*)m_pDlg ;
	if( !pDlg ||
		pDlg->m_r < 0.001 ||
		pDlg->m_h < 0.001 )
		return ; // error
	int n = GetSelected(ftRefPlane, 1000) ;
	for( int i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		SURFACE* cone = coneCreate(&lf, pDlg->m_r/pDlg->m_h) ;
		BOXUV boxuv ;
		boxuv.min[0] = 0.0 ;
		boxuv.max[0] = PI2 ;
		boxuv.min[1] = 0.0 ;
		boxuv.max[1] = pDlg->m_h ;
		CRefSur* pRefSur = new CRefSur(0, cone, &boxuv) ;
		AddFeature(pRefSur) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2001/12/1 nt
 *	生成圆环面
 *	nt rewrite 2010/8/29
 */
void CCAMDoc::OnInsertTorus() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertTorus) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("首先拾取基准平面...") ;
		OnCmd(&CCAMDoc::OnInsertTorus, new CEllDlg(50., 25.), IDD_ELLDLG) ;
		return ;
	}

	CEllDlg* pDlg = (CEllDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	double r1, r2 ;
	r1 = mathMAX(pDlg->m_r1, pDlg->m_r2) ;
	r2 = mathMIN(pDlg->m_r1, pDlg->m_r2) ;
	int n = GetSelected(ftRefPlane, 1000) ;
	for( int i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		SURFACE* torus = torusCreate(&lf, r1, r2) ;
		if( torus )
		{
			CRefSur* pRefSur = new CRefSur(0, torus, NULL) ;
			AddFeature(pRefSur) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}
//--------------------------------------------------------------
/*	2002/6/7 nt
 *	create offset surface
 */
void CCAMDoc::OnInsertOffSur() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertOffSur) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取要等距的曲面、输入等距距离...") ;
		CString name = _T("距离") ;
		OnCmd(&CCAMDoc::OnInsertOffSur, new CLenDlg(name, 5.), IDD_LENGTH) ;
		return ;
	}
	
	CLenDlg* pDlg = (CLenDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefSur, 1000) ;
	for( int i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		SURFACE* offset = offsetCreate(surface, pDlg->m_len) ;
		pRefSur = new CRefSur(0, offset, &boxuv) ;
		AddFeature(pRefSur) ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;	
}
//==============================================================
void CCAMDoc::OnInsertPic() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnInsertPic) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取基准面、输入图像文件...") ;
		OnCmd(&CCAMDoc::OnInsertPic, new CPicDlg(), IDD_PICDLG) ;
		return ;
	}

	CPicDlg* pDlg = (CPicDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefPlane, 1) ; // get refplane
	for( int i = 0 ; i < n ; i++ )
	{
		CRefPlane* pRefPlane = (CRefPlane*)GetObj(i) ;
		RFRAME lf ;
		pRefPlane->GetLF(&lf) ;
		int ok ;
		CPic* pPic = new CPic(&lf, 
							  pDlg->m_fileName, 
							  pDlg->m_transparent, 
							  pDlg->m_tdeg,
							  pDlg->m_min, 
							  pDlg->m_max, 
							  ok) ;
		if( ok &&
			pPic )
			AddFeature(pPic) ;
		else
		{
			if( pPic )
				delete pPic ;
			camPromptStr("图像操作失败!") ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}
//==============================================================
void CCAMDoc::OnInsertPCloud() 
{
	// TODO: Add your command handler code here
	if(	!m_pPart )
		return ;

	TCHAR szFilter[] = _T("单片点云 Files (*.txt)|*.txt||") ;
	CFileDialog dlg(TRUE, _T("*.txt"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		CString ext = dlg.GetFileExt() ;
		CString pathName = dlg.GetPathName() ;
		if( ext == _T("txt") )
			m_pPart->LoadPts(pathName) ;
		Redraw() ;
	}

	return ;
}

void CCAMDoc::OnInsertFitSur() 
{
	// TODO: Add your command handler code here
	camPromptStr("4个测量点...") ;
	int n = GetSelected(MPOINT, 4) ; // get MPOINTs
	if( n > 3 )
	{
	}

	return ;	
}

void CCAMDoc::OnInsertPts() 
{
	// TODO: Add your command handler code here
	if( 	!m_pPart )
		return ;
	camPromptStr("拾取两个测量点...") ;
	int n = GetSelected(MPOINT, 2) ; // get two mpoints
	if( n == 2 )
	{
		CMPoint* pMPoint1 = (CMPoint*)GetObj(0) ;
		CMPoint* pMPoint2 = (CMPoint*)GetObj(1) ;
		int k, n = 2 ;
		double ps[100][3], w ;
		for( k = 1 ; k < n ; k++ )
		{
			w = (double)k/n ;
			ps[k-1][0] = (1.0-w)*pMPoint1->m_p[0]+w*pMPoint2->m_p[0] ;
			ps[k-1][1] = (1.0-w)*pMPoint1->m_p[1]+w*pMPoint2->m_p[1] ;
			ps[k-1][2] = (1.0-w)*pMPoint1->m_p[2]+w*pMPoint2->m_p[2] ;
		}
		CPCloud* pPCloud = m_pPart->m_pPCloud ;
		CPCell* pPCell = new CPCell(n-1, ps) ;
		pPCloud->AddPCell(pPCell) ;
		Redraw() ;
	}	

	return ;
}

void CCAMDoc::OnInsertMPoints2Curve()
{
	// TODO: Add your command handler code here
	return ;
}

void CCAMDoc::OnInsertSurPts() 
{
	// TODO: Add your command handler code here
	if( !m_pPart )
		return ; // error
	int n = GetSelected(ftRefSur, 1000) ;
	for( int I = 0 ; I < n ; I++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(I) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		int i, j, m = 20, n = 20, k = 0 ;
		double (*ps)[3] = new double[m*n][3] ;
		for( i = 0 ; i < m ; i++ )
		{
			for( j = 0 ; j < n ; j++ )
			{
				surfaceEvalPos(surface, 
							   (double)i/m,
							   (double)j/n,
							   ps[k]) ;
				k++ ;
			}
		}
		if( m_pPart->m_pPCloud == NULL )
			m_pPart->m_pPCloud = new CPCloud() ;
		CPCloud* pPCloud = m_pPart->m_pPCloud ;
		CPCell* pPCell = new CPCell(m*n, ps) ;
		pPCloud->AddPCell(pPCell) ;
		delete []ps ;
	}
	if( n > 0 )
		Redraw() ;

	return ;
}
//--------------------------------------------------------------
// CAM
// nt add 2010/6/1 : example code!!!
void CCAMDoc::OnInsertPath5() 
{
	if( !m_pMill )
		m_pMill = new CMill() ;

	// create CPath
	CPath* pPath = new CPath() ;
	pPath->Create(1000) ;
	//-----------------------------
	LMOTION lmotion ;
	lmotion.type = UF_PATH_MOTION_TYPE_CUT ; //motionTypeCUT ;
	lmotion.feed_unit = feedUnitPER_MINUTE ;
	lmotion.feed_value = 3000. ; // uint=mm/min
	lmotion.position[0] = 0. ;
	lmotion.position[1] = 0. ;
	lmotion.position[2] = 100. ;
	lmotion.tool_axis[0] = 0. ;
	lmotion.tool_axis[1] = 0. ;
	lmotion.tool_axis[2] = 1. ;
	pPath->AddLinearMotion(&lmotion) ;
	lmotion.position[0] = 100. ;
	lmotion.position[1] = 0. ;
	lmotion.position[2] = 0. ;
	lmotion.tool_axis[0] = 0. ;
	lmotion.tool_axis[1] = 0.6 ;
	lmotion.tool_axis[2] = 0.8 ;
	pPath->AddLinearMotion(&lmotion) ;
	lmotion.position[0] = 100. ;
	lmotion.position[1] = 100. ;
	lmotion.position[2] = 0. ;
	lmotion.tool_axis[0] = 0. ;
	lmotion.tool_axis[1] = 0. ;
	lmotion.tool_axis[2] = 1. ;
	pPath->AddLinearMotion(&lmotion) ;
	lmotion.position[0] = 0. ;
	lmotion.position[1] = 100. ;
	lmotion.position[2] = 0. ;
	lmotion.tool_axis[0] = 0. ;
	lmotion.tool_axis[1] = 0.8 ;
	lmotion.tool_axis[2] = 0.6 ;
	pPath->AddLinearMotion(&lmotion) ;
	lmotion.position[0] = 0. ;
	lmotion.position[1] = 0. ;
	lmotion.position[2] = 0. ;
	lmotion.tool_axis[0] = 0. ;
	lmotion.tool_axis[1] = 0. ;
	lmotion.tool_axis[2] = 1. ;
	pPath->AddLinearMotion(&lmotion) ;
	pPath->EndAddMotion() ;
	//------------------------------
	// add CPath to CMill
	CStep* pStep = new CStep() ;
	pStep->SetPath(pPath) ;
	//m_pMill->Add(pStep) ;
	AddStep(pStep) ; // nt add 2010/11/30
	Redraw() ;

	return ;
}
//==============================================================
/*	2006/8/19 nt
 *	break a curve by picking
 */
void CCAMDoc::OnToolBreak() 
{
	// TODO: Add your command handler code here
	camPromptStr("先拾取一条曲线...") ;
	int n = GetSelected(ftRefCur, 1) ; // get one curve
	if( n > 0 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		double u = GetU(0) ;
		PNT3D p ;
		curveEvalPos(curve, u, p) ;
		// nt add 2010/11/26
		n = GetSelected(ftRefPt, 1) ;
		double t ;
		if( n > 0 )
		{
			PNT3D q ;
			CRefPt* pRefPt = (CRefPt*)GetObj(0) ;
			pRefPt->GetP(q) ;
			t = u ;
			if( curveSnap2(curve, 
						   q,
						   1.e-5,
						   p,
						   &t) == ERSUCSS )
				u = t ;
		}
		else
		// nt add 2010/3/26, 通过导航修正p
		{
			POSITION pos = GetFirstViewPosition() ;
			CCAMView* pView = (CCAMView*)GetNextView(pos) ;
			if( !pView )
				return ;
			void* pVI = pView->GetVI() ;
			double r = 5.*viGetScale(pVI) ;
			if( r < 0.1 )
				r = 0.1 ;
			DNAVI dn ;
			dnaviInit(&dn, p, NULL, r) ;
			NavigateKeyPt(&dn) ;
			if( dn.feat )
			{
				t = u ;
				if( curveSnap2(curve, 
							   dn.nav_p,
							   1.e-5,
							   p,
							   &t) == ERSUCSS )
					u = t ;
			}
		}
		// end

		INTERVAL int2 ;
		int2 = interval ;
		int2.t2 = u ;
		pRefCur->ModInterval(&int2) ;
		BSCUR* nurbs ;
		interval.t1 = u ;
		int exact ;
		curveToNurbs(curve, &interval, &nurbs, &exact) ;
		pRefCur = new CRefCur(0, nurbs) ;
		AddFeature(pRefCur) ;
	}
	SetLastCmd(ID_TOOL_BREAK) ; // nt add 2010/8/30

	return ;	
}

// nt add 2009/5/7
void CCAMDoc::OnToolTrimCur() 
{
	// TODO: Add your command handler code here
	camPromptStr("点击要裁剪掉的曲线段...") ;
	ClearCmdAll() ;
	if( m_pSelSet ) // nt add 2010/3/24
		m_pSelSet->Clear() ;

	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( !pView )
		return ;
	CTrimCur* pTC = new CTrimCur(this, pView) ;
	if( !pTC )
		return ;
	void* pVI = pView->GetVI() ;
	if( !pVI )
	{
		delete pTC ;
		return ;
	}
	m_pCmd = pTC ;
	SetLastCmd(ID_TOOL_TRIMCUR) ;

	return ;
}

void CCAMDoc::Compare(CURVE* curve, BSCUR* nurbs, INTERVAL* interval, int nSample)
{
	int i ;
	double t, d, max = 0. ;
	PNT3D p, q ;

	for( i = 0 ; i < nSample ; i++ )
	{
		t = interval->t1+i*(interval->t2-interval->t1)/(nSample-1) ;
		curveEvalPos(curve, t, p) ;
		bscurEvalPos(nurbs, t, q) ;
		AddLin(p, q) ;
		d = mathDist(p, q) ;
		if( max < d )
			max = d ;
	}

	return ;
}

void CCAMDoc::Compare(SURFACE* surface, BSSUR* nurbs, BOXUV* boxuv, int nSample)
{
	int i, j ;
	double u, v, d, max1 = 0., max2 = 0.  ;
	PNT3D p, q ;

	for( i = 0 ; i < nSample ; i++ )
	{
		//u = boxuv->min[0]+i*(boxuv->max[0]-boxuv->min[0])/(nSample-1) ;
		for( j = 0 ; j < nSample ; j++ )
		{
			u = boxuv->min[0]+i*(boxuv->max[0]-boxuv->min[0])/(nSample-1) ;
			v = boxuv->min[1]+j*(boxuv->max[1]-boxuv->min[1])/(nSample-1) ;
			surfaceEvalPos(surface, u, v, p) ;
			bssurEvalPos(nurbs, u, v, q) ;
			//AddLin(p, q) ;
			d = mathDist(p, q) ;
			if( max1 < d )
				max1 = d ;
			bssurSnap(nurbs, p, 1.e-5, q, &u, &v, &d) ;
			if( max2 < d )
				max2 = d ;
		}
	}

	return ;
}

/*	2006/8/24 nt
 *	curve or surface to NURBS
 */
void CCAMDoc::OnToolToNURBS() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet ||
		!m_pPart )
		return ;
	camPromptStr("将曲线曲面转化为样条...") ;
	int i, exact, n = GetSelected(ftRefCur, 100000) ; // get curves
	if( n > 0 )
	{
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			CURVE* curve = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			if( entityGetType(curve) != etBCurve )
			{
				BSCUR* nurbs_cur ;
				curveToNurbs(curve, &interval, &nurbs_cur, &exact) ;
				//Compare(curve, nurbs_cur, &interval, 100) ; // nt add 2021/2/25 test only
				m_pPart->Delete(pRefCur) ;
				pRefCur = new CRefCur(0, nurbs_cur) ;
				AddFeature(pRefCur) ;
			}
		}
	}

	n = GetSelected(ftRefSur, 100000) ; // get surfaces
	if( n > 0 )
	{
		for( i = 0 ; i < n ; i++ )
		{
			CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
			SURFACE* surface = pRefSur->GetSurface() ;
			BOXUV boxuv ;
			pRefSur->GetBoxUV(&boxuv) ;
			if( entityGetType(surface) != etBSurface )
			{
				BSSUR* nurbs_sur ;
				surfaceToNurbs(surface, &boxuv, &nurbs_sur, &exact) ;
				//Compare(surface, nurbs_sur, &boxuv, 100) ; // nt add 2021/2/25 test only
				m_pPart->Delete(pRefSur) ;
				pRefSur = new CRefSur(0, nurbs_sur) ;
				AddFeature(pRefSur) ;
			}
		}
	}

	/*int exact2 ;
	RFRAME lf ;
	mathInitRFrame(&lf) ;
	BSSUR* nurbs = NULL ;
	BOXUV boxuv ;
	//SURFACE* surface = sphereCreate(&lf, 10.) ;
	//boxuv.min[0] = 0. ;
	//boxuv.min[1] = 0. ;
	//boxuv.max[0] = PI1_2 ;
	//boxuv.max[1] = PI1_2 ;
	//SURFACE* surface = cylinderCreate(&lf, 10.) ;
	//boxuv.min[0] = 0. ;
	//boxuv.min[1] = 0. ;
	//boxuv.max[0] = PI2 ;
	//boxuv.max[1] = 100. ;
	SURFACE* surface = torusCreate(&lf, 20., 4.) ;
	boxuv.min[0] = 0. ;
	boxuv.min[1] = 0. ;
	boxuv.max[0] = PI1_2 ;
	boxuv.max[1] = PI1_2 ;
	gpSetFit(0.01) ;
	surfaceToNurbs(surface, &boxuv, &nurbs, &exact2) ;
	double u, v, d, max = 0. ;
	PNT3D p, q ;
	for( i = 0 ; i < 1000 ; i++ )
	{
		u = mathRand01()*boxuv.max[0] ;
		v = mathRand01()*boxuv.max[1] ;
		surfaceEvalPos(surface, u, v, p) ;
		bssurSnap(nurbs, p, 1.e-3, q, &u, &v, &d) ;
		if( max < d )
			max = d ;
	}
	entityFree(surface) ;
	surface = NULL ;
	surface = bsurfaceCreate(nurbs) ;
	bssurFree(nurbs) ;
	nurbs = NULL ;
	CRefSur* pRefSur = new CRefSur(0, surface, &boxuv) ;
	AddFeature(pRefSur) ;*/

	return ;	
}

/*	2001/11/28 nt
 *	modify curve G1
 *	nt rewrite 2006/9/5
 */
void CCAMDoc::OnToolBCurG1() 
{
	// TODO: Add your command handler code here
	return ;
}

/*	2001/11/28 nt
 *	modify curve G2
 *	nt rewrite 2010/8/30
 */
void CCAMDoc::OnToolBCurG2() 
{
	// TODO: Add your command handler code here
	return ;
}

/*	2009/4/25 nt
 *	检查曲线的几何连续性
 *	nt rewrite 2010/8/30
 */
void CCAMDoc::OnToolChkCurGx() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnToolChkCurGx) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("选择两条首尾衔接的曲线...") ;
		OnCmd(&CCAMDoc::OnToolChkCurGx, new CChkDlg(), IDD_CHECK) ;
		return ;
	}

	CChkDlg* pDlg = (CChkDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->m_strs.RemoveAll() ;
	int n = GetSelected(ftRefCur, 2) ; // get two curve
	if( n == 2 )
	{
		CRefCur* pRefCur1 = (CRefCur*)GetObj(0) ;
		CURVE* curve1 = pRefCur1->GetCurve() ;
		double u = pRefCur1->GetU() ;
		INTERVAL interval1 ;
		pRefCur1->GetInterval(&interval1) ;
		CRefCur* pRefCur2 = (CRefCur*)GetObj(1) ;
		CURVE* curve2 = pRefCur2->GetCurve() ;
		INTERVAL interval2 ;
		pRefCur2->GetInterval(&interval2) ;
		PNT3D pOnCur ;
		curveEvalPos(curve1, u, pOnCur) ;

		PNT3D b1, e1, b2, e2 ;
		curveEvalPos(curve1, interval1.t1, b1) ;
		curveEvalPos(curve1, interval1.t2, e1) ;
		curveEvalPos(curve2, interval2.t1, b2) ;
		curveEvalPos(curve2, interval2.t2, e2) ;
		
		double t1, t2, min = MAX_DBL ;
		if( mathDist(b1, b2) < MIN_LEN &&
			mathDist(b1, pOnCur) < min )
		{
			t1 = interval1.t1 ;
			t2 = interval2.t1 ;
			min = mathDist(b1, pOnCur) ;
		}
		if( mathDist(b1, e2) < MIN_LEN &&
			mathDist(b1, pOnCur) < min )
		{
			t1 = interval1.t1 ;
			t2 = interval2.t2 ;
			min = mathDist(b1, pOnCur) ;
		}
		if( mathDist(e1, b2) < MIN_LEN &&
			mathDist(e1, pOnCur) < min )
		{
			t1 = interval1.t2 ;
			t2 = interval2.t1 ;
			min = mathDist(e1, pOnCur) ;
		}
		if( mathDist(e1, e2) < MIN_LEN &&
			mathDist(e1, pOnCur) < min )
		{
			t1 = interval1.t2 ;
			t2 = interval2.t2 ;
			min = mathDist(e1, pOnCur) ;
		}

		CString str ;
		if( min > MAX_DBL*0.99 )
		{
			str = _T("曲线不首尾衔接!") ;
			pDlg->m_strs.Add(str) ;
		}
		else
		{
			VEC3D T1, T2, N, B ;
			double c1, c2, d, a, c ; // curvature
			curveEvalCur(curve1, t1, b1, T1, N, B, &c1) ;
			curveEvalCur(curve2, t2, b2, T2, N, B, &c2) ;
			d = mathDist(b1, b2) ;
			a = mathGetAngleUnit(T1, T2) ;
			if( a > 0.5*PI1 )
				a = PI1-a ;
			c = fabs(c1-c2) ;
			str.Format(_T("端点距离偏差 = %lf"), d) ;
			pDlg->m_strs.Add(str) ;
			str.Format(_T("切矢角度偏差(度) = %lf"), a*180/PI1) ;
			pDlg->m_strs.Add(str) ;
			str.Format(_T("曲率偏差 = %lf"), c) ;
			pDlg->m_strs.Add(str) ;
		}
	}
	pDlg->Flush() ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2002/6/18 nt
 *	smooth curve
 *	nt modify 2011/1/28
 *	nt modify 2013/6/19
 */
void CCAMDoc::OnToolSmthCur() 
{
	// TODO: Add your command handler code here
	return ;
}

// nt add 2011/8/11
void CCAMDoc::OnToolMorph()
{
	camPromptStr("先拾取两条线段及若干曲线...") ;
	int n = GetSelected(ftRefCur, 1000) ;
	if( n > 2 ) // 注意：下面未调整line1,line2的方向
	{
		double d1, d2, d ;
		PNT3D b1, e1, b2, e2, p ;
		RFRAME lf1, lf2 ; // lf1:from, lf2:to
		CRefCur* pRefCur ;
		pRefCur = (CRefCur*)GetObj(0) ; // line1
		pRefCur->GetBegin(b1) ;
		pRefCur->GetEnd(e1) ;
		mathMidPnt(b1, e1, lf1.O) ;
		d1 = mathDist(b1, e1) ;
		if( d1 < 0.001 )
			return ;
		lf1.X[0] = (e1[0]-b1[0])/d1 ;
		lf1.X[1] = (e1[1]-b1[1])/d1 ;
		lf1.X[2] = (e1[2]-b1[2])/d1 ;
		pRefCur = (CRefCur*)GetObj(1) ; // line2
		pRefCur->GetBegin(b2) ;
		pRefCur->GetEnd(e2) ;
		mathMidPnt(b2, e2, lf2.O) ;
		d2 = mathDist(b2, e2) ;
		if( d2 < 0.001 )
			return ;
		lf2.X[0] = (e2[0]-b2[0])/d2 ;
		lf2.X[1] = (e2[1]-b2[1])/d2 ;
		lf2.X[2] = (e2[2]-b2[2])/d2 ;
		mathPrjPntLin(lf2.O, lf1.O, lf1.X, p) ;
		d = mathDist(lf2.O, p) ;
		if( d < MIN_LEN )
			return ;
		lf1.Y[0] = (lf2.O[0]-p[0])/d ;
		lf1.Y[1] = (lf2.O[1]-p[1])/d ;
		lf1.Y[2] = (lf2.O[2]-p[2])/d ;
		mathVProduct(lf1.X, lf1.Y, lf1.Z) ;
		lf1.scale = 1. ;
		mathPrjPntLin(lf1.O, lf2.O, lf2.X, p) ;
		d = mathDist(lf1.O, p) ;
		if( d < MIN_LEN )
			return ;
		lf2.Y[0] = (p[0]-lf1.O[0])/d ;
		lf2.Y[1] = (p[1]-lf1.O[1])/d ;
		lf2.Y[2] = (p[2]-lf1.O[2])/d ;
		mathVProduct(lf2.X, lf2.Y, lf2.Z) ;
		lf2.scale = 1. ;

		for( int i = 2 ; i < n ; i++ )
		{
			pRefCur = (CRefCur*)GetObj(i) ;
			CURVE* curve = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			CURVE* copy = geometryCopy(curve) ;
			geometryToLocal(copy, &lf1) ;
			geometryScale(copy, d2/d1) ;
			geometryToWorld(copy, &lf2) ;
			pRefCur = new CRefCur(0, copy, &interval) ;
			AddFeature(pRefCur) ;
		}
	}
}

// 构造曲线的三角框架
void CCAMDoc::OnToolTanLin2()
{
	int i, n = GetSelected(ftRefCur, 10000) ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		CURVE* curve = pRefCur->GetCurve() ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		PNT3D p1, p2, p, q, midp ;
		VEC3D tvec1, tvec2 ;
		curveEvalTangent(curve, interval.t1, p1, tvec1) ;
		curveEvalTangent(curve, interval.t2, p2, tvec2) ;
		mathIntLin(p1, tvec1, p2, tvec2, 1.0e-5, 1.e-11, p, q) ;
		mathMidPnt(p, q, midp) ;
		AddLin(p1, midp) ;
		AddLin(p2, midp) ;
		AddLin(p1, p2) ;
	}

	return ;
}

/*	2006/5/21 nt
 *  out pts on curve
 *	nt rewrite 2010/8/31
 */
void CCAMDoc::OnToolCurPts() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnToolCurPts) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取若干条曲线...") ;
		OnCmd(&CCAMDoc::OnToolCurPts, new CCurPtsDlg(), IDD_CURPTSDLG) ;
		return ;
	}
	
	CCurPtsDlg* pDlg = (CCurPtsDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int n = GetSelected(ftRefCur, 1000) ; // get curves
	if( n > 0 )
	{
		int i, j, m = pDlg->m_n ;
		INTERVAL interval ;
		CRefCur* pRefCur ;
		CURVE* curve ;
		double u ;
		PNT3D p ;
		FILE* fp = NULL ;
		_tfopen_s(&fp, pDlg->m_outFile.GetBuffer(0), _T("w+t")) ;
		if( fp )
		{
			for( i = 0 ; i < n ; i++ )
			{
				fprintf_s(fp, "CURVE=%d\n", i) ;
				fprintf_s(fp, "NUMBER_OF_POINTS=%d\n", m) ;
				pRefCur = (CRefCur*)GetObj(i) ;
				curve = pRefCur->GetCurve() ;
				if( curve )
				{
					curveGetInterval(curve, &interval) ;
					for( j = 0 ; j < m ; j++ )
					{
						u = interval.t1+j*(interval.t2-interval.t1)/(m-1) ;
						curveEvalPos(curve, u, p) ;
						fprintf_s(fp, "%lf %lf %lf\n", p[0], p[1], p[2]) ;
					}
				}
				fprintf_s(fp, "END\n") ;
			}
			fclose(fp) ;
		}
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}
//--------------------------------------------------------------
// nt modify 2009/5/25 : 使可同时处理多条曲线
void CCAMDoc::OnToolCompPrj() 
{
	// TODO: Add your command handler code here
	return ;	
}
/*-----------------------------------------------------------*/
/*	2001/11/29 nt
 *	extend curve as line
 *	nt modify 2009/5/24 : 允许处理多条曲线
 *	nt rewrite 2010/8/30
 */
void CCAMDoc::OnToolCurveExtend() 
{
	// TODO: Add your command handler code here
	return ;
}
//==============================================================
/*	2001/11/29 nt
 *	b-spline surface G1 joining
 */
void CCAMDoc::OnToolBSurG1() 
{
	// TODO: Add your command handler code here
	return ;
}

void CCAMDoc::OnToolBSurG2() 
{
	// TODO: Add your command handler code here
	return ;
}
//--------------------------------------------------------------
// nt rewrite 2010/9/1
// 注意：只针对NURBS有完整边界的情况
void CCAMDoc::OnToolChkSurGx() 
{
	// TODO: Add your command handler code here
	return ;	
}
//--------------------------------------------------------------
/*	2006/5/21 nt
 *	检查曲线和曲面的最大位置误差
 *	nt modify 2010/3/17
 *	nt rewrite 2010/9/1
 *	nt modify 2010/9/1 允许处理多条曲线
 */
void CCAMDoc::OnToolChkG0() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnToolChkG0) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取若干曲线和一张曲面...") ;
		OnCmd(&CCAMDoc::OnToolChkG0, new CChkDlg(), IDD_CHECK) ;
		return ;
	}

	CChkDlg* pDlg = (CChkDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->m_strs.RemoveAll() ;
	int n = GetSelected(ftRefSur, 1) ; // one surface
	if( n == 1 )
	{
		double max = 0. ;
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		n = GetSelected(ftRefCur, 1000) ; // curves
		for( int i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			CURVE* curve = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			double e ;
			if( surfaceCalDevG0(surface, // 计算曲线到曲面的偏差
								curve,
								&interval,
								&e) == 1 &&
				max < e )
				max = e ;
		}
		TCHAR str[64] ;
		_stprintf_s(str, _T("位置误差=%lfmm"), max) ;
		pDlg->m_strs.Add(str) ;
		pDlg->Flush() ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

/*	2006/5/21 nt
 *	检查两曲面沿指定边界的normal vector的最大角度误差
 *	nt modify 2010/3/16 : u,v,s,t未赋初值
 */
void CCAMDoc::OnToolChkG1() 
{
	int nStep = ChkCmd(&CCAMDoc::OnToolChkG1) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取一条曲线和两张曲面...") ;
		OnCmd(&CCAMDoc::OnToolChkG1, new CChkDlg(), IDD_CHECK) ;
		return ;
	}

	CChkDlg* pDlg = (CChkDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->m_strs.RemoveAll() ;
	int m = GetSelected(ftRefCur, 1) ; // one curve
	if( m != 1 )
	{
		camPromptStr("拾取一条曲线和两张曲面!") ;
		return ;
	}
	CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
	CURVE* curve = pRefCur->GetCurve() ;
	INTERVAL interval ;
	pRefCur->GetInterval(&interval) ;
	int n = GetSelected(ftRefSur, 2) ; // two surface
	if( n != 2 )
	{
		camPromptStr("拾取一条曲线和两张曲面!") ;
		return ;
	}
	CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
	SURFACE* surface1 = pRefSur->GetSurface() ;
	pRefSur = (CRefSur*)GetObj(1) ;
	SURFACE* surface2 = pRefSur->GetSurface() ;
	double e ;
	if( surfaceCalDevG1(surface1,
						surface2,
						curve,
						&interval,
						&e) == 1 )
	{
		TCHAR str[64] ;
		_stprintf_s(str, _T("角度误差=%lf度"), e) ;
		pDlg->m_strs.Add(str) ;
		pDlg->Flush() ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}
//--------------------------------------------------------------
void CCAMDoc::OnToolMeshSur() 
{
	// TODO: Add your command handler code here
	return ;	
}
//--------------------------------------------------------------
void CCAMDoc::OnToolModSurCPt() 
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;
	camPromptStr("先拾取一张要修改的NURBS曲面...") ;
	int n = GetSelected(ftRefSur, 1) ; // get nurbs surface
	if( n == 1 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		POSITION pos = GetFirstViewPosition() ;
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( !pView )
			return ;
		CModSurCPt* pCmd = new CModSurCPt(this, pView) ;
		pCmd->Create(pRefSur) ;
		m_pCmd = pCmd ;
		pCmd->Draw() ; // nt add 2010/9/1
	}
	SetLastCmd(ID_TOOL_MODSURCPT) ;

	return ;
}

void CCAMDoc::OnToolModSurIPt()
{
	// TODO: Add your command handler code here
	ClearCmdAll() ;

	if( !m_pSelSet ||
		!m_pPart )
		return ;
	int n = GetSelected(ftRefSur, 1) ; // get nurbs surface
	if( n < 1 )
		AfxMessageBox(_T("拾取NURBS曲面")) ;
	else
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		POSITION pos = GetFirstViewPosition() ;
		CCAMView* pView = (CCAMView*)GetNextView(pos) ;
		if( !pView )
			return ;

		SURFACE* surface = pRefSur->GetSurface() ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		PNT3D p ;
		VEC3D normal ;
		surfaceEvalNormal(surface, 0.5, 0.5, p, normal) ;
		p[0] += normal[0]*10. ;
		p[1] += normal[1]*10. ;
		p[2] += normal[2]*10. ;

		//bssurAppx(nurbs, 1, &p, IDG0, 1., &d) ; // 更换GP后需要恢复2006.5.3
		pRefSur->Disc(1., 0.3) ;
		Redraw() ;
	}

	return ;
}
//==============================================================
// 2010/1/21
// 一种特殊的过渡曲面
/*void CCAMDoc::OnToolTest()
{
	// 满足要求
	// (1)curve1是surface1和surface2的交线，
	// (2)curve2在surface2上
	camPromptStr("拾取要过渡曲线、两张待过渡曲面及限制线") ;
	if( GetSelected(ftRefCur, 2) != 2 ) // get two curves
		return ;
	CRefCur* pRefCur = (CRefCur*)GetObj((0) ;
	CURVE* edge = pRefCur->GetCurve() ; // edge need to be rounded
	INTERVAL interval1 ;
	pRefCur->GetInterval(&interval1) ;
	pRefCur = (CRefCur*)GetObj((1) ;
	CURVE* line = pRefCur->GetCurve() ; // restriction line
	INTERVAL interval2 ;
	pRefCur->GetInterval(&interval2) ;
	//----------------------------------------------------------
	// flag1和flag2表示surface1和surface2的等距方向
	double u, v ;
	PNT3D p1, p2 ;
	VEC3D n1, n2 ;
	int flag1 = 1, flag2 = 1 ;
	CRefSur* pRefSur ;
	if( GetSelected(ftRefSur, 2) != 2 ) // get two surfaces
		return ;
	pRefSur = (CRefSur*)GetObj((0) ;
	SURFACE* surface1 = pRefSur->GetSurface() ; // surface1
	pRefSur = (CRefSur*)GetObj((1) ;
	SURFACE* surface2 = pRefSur->GetSurface() ; // surface2
	//----------------------------------------------------------
	u = GetU(0) ;
	v = GetV(0) ;
	surfaceEvalPos(surface1, u, v, p1) ; // surface1的拾取点
	u = -MAX_DBL ;
	v = -MAX_DBL ;
	if( surfaceSnap(surface2, p1, p2, &u, &v) != ERSUCSS )
		return ; // error
	surfaceEvalNormal(surface2, u, v, p2, n2) ;
	//if( n2[0]*(p1[0]-p2[0])+
	//	n2[1]*(p1[1]-p2[1])+
	//	n2[2]*(p1[2]-p2[2]) < 0. )
	if( mathOProduct2(n2, p2, p1) < 0. )
		flag2 = -1 ;
	//----------------------------------------------------------
	u = GetU(1) ;
	v = GetV(1) ;
	surfaceEvalPos(surface2, u, v, p2) ; // surface2的拾取点
	u = -MAX_DBL ;
	v = -MAX_DBL ;
	if( surfaceSnap(surface1, p2, p1, &u, &v) != ERSUCSS )
		return ; // error
	surfaceEvalNormal(surface1, u, v, p1, n1) ;
	//if( n1[0]*(p2[0]-p1[0])+
	//	n1[1]*(p2[1]-p1[1])+
	//	n1[2]*(p2[2]-p1[2]) < 0. )
	if( mathOProduct2(n1, p1, p2) < 0. )
		flag1 = -1 ;

	// 计算截面线-----------------------------------------------
	// 计算初始位置
	int n = 100 ; // edge分成n段
	PNT3D p, q ;
	VEC3D T, v1, v2 ;
	double r0, r1, r2, r, s, t, a, d, uv[2], st[2] ;
	r0 = 0.0075 ;
	r1 = 0.01 ;
	r2 = 0.0075 ;
	r = r0 ;
	t = interval1.t1 ;
	curveEvalTangent(edge, t, p, T) ;
	curveSnap(line, p, q, &s) ; // in order to estimate s
	surfaceSnap(surface1, p, p1, &u, &v) ;
	surfaceEvalNormal(surface1, u, v, p1, n1) ;
	mathVProduct(T, n1, v1) ;
	if( mathUniVec(v1, MIN_DBL) != ERSUCSS )
		return ; // error
	surfaceSnap(surface2, p, p2, &u, &v) ;
	surfaceEvalNormal(surface2, u, v, p2, n2) ;
	mathVProduct(T, n2, v2) ;
	if( mathUniVec(v2, MIN_DBL) != ERSUCSS )
		return ; // error
	if( mathOProduct(n1, v2)*flag1 < 0. ) // adjust v1 direction
	{
		v1[0] = -v1[0] ;
		v1[1] = -v1[1] ;
		v1[2] = -v1[2] ;
	}
	if( mathOProduct(n2, v1)*flag2 < 0. ) // adjust v2 direction
	{
		v2[0] = -v2[0] ;
		v2[1] = -v2[1] ;
		v2[2] = -v2[2] ;
	}
	a = mathGetAngleUnit(v1, v2) ;
	if( a < MIN_ANG ||
		a > PI1-MIN_ANG )
		return ; // error
	d = r0/tan(0.5*a) ;
	mathEvalLin(p, v1, d, q) ;
	surfaceSnap(surface1, q, p1, &(uv[0]), &(uv[1])) ;
	mathEvalLin(p, v2, d, q) ;
	surfaceSnap(surface2, q, p2, &(st[0]), &(st[1])) ;

	// 创建变半径函数
	BSCUR* vr = bscurCreate(4, 7) ;
	vr->u_k[4] = 0.5 ;
	vr->u_k[5] = 0.5 ;
	vr->u_k[6] = 0.5 ;
	for( int k = 0 ; k < 7 ; k++ )
	{
		vr->u_p[k][0] = k/6. ;
		vr->u_p[k][2] = 0. ;
		vr->u_p[k][3] = 1. ;
	}
	vr->u_p[0][1] = r0 ;
	vr->u_p[1][1] = r0 ;
	vr->u_p[2][1] = r1 ;
	vr->u_p[3][1] = r1 ;
	vr->u_p[4][1] = r1 ;
	vr->u_p[5][1] = r2 ;
	vr->u_p[6][1] = r2 ;

	// 计算各截面线---------------------------------------------
	double step = (interval1.t2-interval1.t1)/n, R, d1, d2 ;
	PNT3D center ; // arc center
	VEC3D normal, Q ;
	INTERVAL interval ;
	CURVE* circle ;
	ARC arc ;
	int rt, f ;
	u = -MAX_DBL ;
	v = -MAX_DBL ;
	for( int i = 0 ; i <= n ; i++, t += step )
	{
		curveEvalTangent(edge, t, p, T) ;
		bscurEvalPos(vr, 
					 (t-interval1.t1)/(interval1.t2-interval1.t1),
					 Q) ;
		r = Q[1] ;
		SURFACE* offset1 = offsetCreate(surface1, r*flag1) ;
		SURFACE* offset2 = offsetCreate(surface2, r*flag2) ;
		if( surfaceIttSurPln(offset1,
							 offset2,
							 p,
							 T, 
							 1.e-8,
							 center, // point on the center line
							 uv,
							 st,
							 n1,
							 n2) !=	IDCNVGN )
		{
			entityFree(offset1) ;
			entityFree(offset2) ;
			bscurFree(vr) ;
			return ; // error
		}
		surfaceEvalPos(surface1, uv[0], uv[1], p1) ;
		surfaceEvalPos(surface2, st[0], st[1], p2) ;
		// 判断p2是否超过限制线, 先用平面(p, T)截取限制线上一点q
		rt = curveIttPln(line, p, T, MIN_LEN, MIN_ANG, q, &s, &f) ;
		if( rt == IDCNVGN )
		{
			curveEvalTangent(line, s, q, v1) ;
			surfaceSnap(surface2, q, Q, &u, &v) ;
			surfaceEvalNormal(surface2, u, v, Q, normal) ;
			mathVProduct(v1, normal, v2) ;
			mathUniVec(v2, MIN_DBL) ;
			d1 = mathOProduct2(v2, q, p1) ;
			d2 = mathOProduct2(v2, q, p2) ;
			if( d1*d2 < 0. ) // 超出限制线
			{
				RFRAME lf ;
				mathInitRFrame(&lf) ;
				memcpy(lf.O, q, sizeof(PNT3D)) ;
				SURFACE* ball = sphereCreate(&lf, r) ;
				surfaceSnap(ball, center, Q, &(st[0]), &(st[1])) ;
				if( surfaceIttSurPln(offset1,
									 ball,
									 p,
									 T, 
									 1.e-8,
									 center, // on the center line
									 uv,
									 st,
									 n1,
									 n2) !=	IDCNVGN )
				{
					entityFree(ball) ;
					entityFree(offset1) ;
					entityFree(offset2) ;
					bscurFree(vr) ;
					return ; // error
				}
				surfaceSnap(surface2, q, Q, &(st[0]), &(st[1])) ;
				surfaceEvalPos(surface1, uv[0], uv[1], p1) ;
				memcpy(p2, q, sizeof(PNT3D)) ;
				entityFree(ball) ;
			}
		}
		if( mathMakeArc(&arc, center, T, p1, p2, p) == ERSUCSS )
		{
			R = 0.5*(mathDist(p1, center)+
					 mathDist(p2, center)) ;
			circle = circleCreate(&arc.lf, R) ;
			interval.t1 = 0. ;
			interval.t2 = arc.a ;
			pRefCur = new CRefCur(0, circle, &interval) ;
			m_pPart->Add(pRefCur) ;
		}
		entityFree(offset1) ;
		entityFree(offset2) ;
	}
	bscurFree(vr) ;
	Redraw() ;

	return ;
}*/
//--------------------------------------------------------------
// nt add 2010/3/12
// 曲面切割
void CCAMDoc::OnToolCutSur()
{
	//----------------------------------------------------------
	if( m_pPart == NULL )
		return ;
	camPromptStr("拾取一张要切割的曲面和一张或两张切割面") ;
	int n = GetSelected(ftRefSur, 3) ;
	if( n < 2 )
		return ;
	CRefSur* pRefSur ;
	pRefSur = (CRefSur*)GetObj(0) ;
	SURFACE* surface = pRefSur->GetSurface() ; // sur to be cut
	BOXUV boxuv ;
	pRefSur->GetBoxUV(&boxuv) ;
	BOX3D box ;
	pRefSur->GetBox(&box) ;
	SURFACE* cut1 = NULL, *cut2 = NULL ;
	BOXUV boxuv1, boxuv2 ;
	BOX3D box1, box2 ;
	if( n > 1 )
	{
		pRefSur = (CRefSur*)GetObj(1) ;
		cut1 = pRefSur->GetSurface() ; // get first cut surface
		pRefSur->GetBoxUV(&boxuv1) ;
		pRefSur->GetBox(&box1) ;
	}
	if( n > 2 )
	{
		pRefSur = (CRefSur*)GetObj(2) ;
		cut2 = pRefSur->GetSurface() ; // get second cut surface
		pRefSur->GetBoxUV(&boxuv2) ;
		pRefSur->GetBox(&box2) ;
	}

	//----------------------------------------------------------
	CUTSUR* cutsur = cutsurCreate(surface,
								  &boxuv,
								  &box,
								  cut1,
								  &boxuv1,
								  &box1,
								  cut2,
								  &boxuv2,
								  &box2,
								  1.e-3) ; //5.e-5) ;
	if( cutsurMake(cutsur) == ERSUCSS )
	{
		for( int k = 0 ; k < 3 ; k++ )
		{
			if( cutsur->bss[k] )
			{
				pRefSur = new CRefSur(0, cutsur->bss[k]) ;
				m_pPart->Add(pRefSur) ;
			}
		}
		Redraw() ;
	}
	else
		camPromptStr("计算失败!") ;
	cutsurFree(cutsur) ;

	return ;
}
//--------------------------------------------------------------
// 曲线曲面反向
// nt add 2010/5/23
// nt rewrite 2010/9/1
void CCAMDoc::OnToolRevUV()
{
	int nStep = ChkCmd(&CCAMDoc::OnToolRevUV) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取NURBS曲面...") ;
		OnCmd(&CCAMDoc::OnToolRevUV, new CRevDlg(), IDD_REVDLG) ;
		return ;
	}

	CRevDlg* pDlg = (CRevDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int i, n1 = 0, n2 = 0 ;
	if( pDlg->m_mode == 0 )
	{
		n1 = GetSelected(ftRefCur, 10000) ; // get CRefCurs
		for( i = 0 ; i < n1 ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			pRefCur->RevsDir() ;
		}
	}
	if( pDlg->m_mode == 0 ||
		pDlg->m_mode == 1 ||
		pDlg->m_mode == 2 )
	{
		n2 = GetSelected(ftRefSur, 10000) ; // get CRefSurs
		for( i = 0 ; i < n2 ; i++ )
		{
			CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
			pRefSur->RevsDir(pDlg->m_mode) ;
		}
	}
	if( n1+n2 > 0 )
		Redraw() ;
	if( m_ui == 1 )
		CmdEnd() ;
	
	return ;
}

// nt add 2010/11/26
void CCAMDoc::OnToolSurClose()
{
	int i, n = GetSelected(ftRefSur, 10000), flag = 0 ; // get CRefSur
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		if( entityGetType(surface) == etBSurface )
		{
			BSSUR* nurbs = surfaceGetNurbs(surface) ;
			if( nurbs )
			{
				int i, j, nu, nv ;
				PNT3D p ;
				nu = nurbs->cnt_u-1 ;
				nv = nurbs->cnt_v-1 ;
				if( mathDist(nurbs->uv_p[0][0], nurbs->uv_p[nu][0]) < 0.001 &&
					mathDist(nurbs->uv_p[0][nv], nurbs->uv_p[nu][nv]) < 0.001 )
				{
					for( j = 1 ; j < nv ; j++ )
					{
						mathMidPnt(nurbs->uv_p[0][j], nurbs->uv_p[nu][j], p) ;
						memcpy(nurbs->uv_p[0][j], p, sizeof(PNT3D)) ;
						memcpy(nurbs->uv_p[nu][j], p, sizeof(PNT3D)) ;
					}
					pRefSur->Disc(0.5, 0.15) ;
					pRefSur->CalBox() ;
					flag = 1 ;
				}
				else
				if( mathDist(nurbs->uv_p[0][0], nurbs->uv_p[0][nv]) < 0.001 &&
					mathDist(nurbs->uv_p[nu][0], nurbs->uv_p[nu][nv]) < 0.001 )
				{
					for( i = 1 ; i < nu ; i++ )
					{
						mathMidPnt(nurbs->uv_p[i][0], nurbs->uv_p[i][nv], p) ;
						memcpy(nurbs->uv_p[i][0], p, sizeof(PNT3D)) ;
						memcpy(nurbs->uv_p[i][nv], p, sizeof(PNT3D)) ;
					}
					pRefSur->Disc(0.5, 0.15) ;
					pRefSur->CalBox() ;
					flag = 1 ;
				}
			}
		}
	}

	if( flag < 1 )
	{
		camPromptStr("拾取NURBS曲面!") ;
		return ;
	}
	else
		Redraw() ;
	
	return ;
}

/*	2013/6/20 nt
 *	smooth surface
 */
void CCAMDoc::OnToolSurSmth() 
{
	// TODO: Add your command handler code here
	return ;
}
//======================================================================
/*	2001/11/24 nt
 *	display or undisplay curve curvature
 *	nt rewrite 2011/4/23 增加用CMark显示曲面曲率
 */
void CCAMDoc::OnAnalysisCurvatureComb() 
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnAnalysisCurvatureComb) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("显示选中的曲线、曲面的曲率梳...") ;
		OnCmd(&CCAMDoc::OnAnalysisCurvatureComb, new CCombDlg(), IDD_COMBDLG) ;
		return ;
	}

	CCombDlg* pDlg ;
	pDlg = (CCombDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	int i, atts, n1, n2 ;
	n1 = GetSelected(ftRefCur, 10000) ; // get curves
	for( i = 0 ; i < n1 ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		atts = pRefCur->GetAtts() ;
		if( atts&1 )
			pRefCur->SetCur(0) ;
		else
			pRefCur->SetCur(1) ;
	}
	n2 = GetSelected(ftRefSur, 10000) ; // get surfaces
	for( i = 0 ; i < n2 ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		CMark* pMark = NULL ;
		if( pDlg->m_comb )
		{
			pMark = pRefSur->CalKuKv(pDlg->m_nu, 
									 pDlg->m_nv,
									 pDlg->m_factor) ; // 生成CMark显示标志
		}
		else
		{
			ISODEF isodef ;
			isodef.attType = pDlg->m_attType ;
			isodef.factor = pDlg->m_factor ;
			isodef.nZPlane = pDlg->m_nZPlane ;
			if( fabs(pDlg->m_attMax-pDlg->m_attMin) > MIN_DBL )
			{
				isodef.attMin = pDlg->m_attMin ;
				isodef.attMax = pDlg->m_attMax ;
			}
			else
			{
				isodef.attMin = 0. ;
				isodef.attMax = 0. ;
			}
			isodef.attTol = 0. ;
			pMark = pRefSur->CalIso(&isodef) ; // 生成CMark显示标志
		}
		if( pMark )
			AddFeature(pMark) ;
	}
	if( n1+n2 > 0 )
		Redraw() ;
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}

// 开关绘制曲面的UV标志
void CCAMDoc::OnAnalysisDrawUV() 
{
	// TODO: Add your command handler code here
	int i ;
	camPromptStr("拾取药画参数方向的曲线、曲面...") ;
	int n1 = GetSelected(ftRefCur, 10000) ; // get curves
	for( i = 0 ; i < n1 ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		pRefCur->SwapDrawU() ;
	}

	int n2 = GetSelected(ftRefSur, 10000) ; // get surfaces
	for( i = 0 ; i < n2 ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		pRefSur->SwapDrawUV() ;
	}

	if( n1+n2 > 0 )
		Redraw() ;

	return ;
}

/*	2010/3/19 nt
 *	曲线曲面的加密显示
 */
void CCAMDoc::OnAnalysisRefine() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet ||
		!m_pPart )
		return ;
	camPromptStr("拾取要加密显示的曲线、曲面...") ;
	int i, n = GetSelected(ftRefCur, 1000) ; // get mult curves
	for( i = 0 ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		pRefCur->Disc(0.01, PI1/180) ;
	}

	n = GetSelected(ftRefSur, 1000) ; // get mult surfaces
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		pRefSur->Disc(0.01, PI1/180) ;
	}
	Redraw() ;

	return ;
}

// nt add 2006/8/7
// nt modify 2010/9/2
void CCAMDoc::OnAnalysisControlPts() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet ||
		!m_pPart )
		return ;
	camPromptStr("拾取一条或多条曲线或曲面...") ;
	COA ses1, ses2  ;
	int n1 = m_pSelSet->GetSelected(ftRefCur, 100, ses1) ; // get curves
	int n2 = m_pSelSet->GetSelected(ftRefSur, 100, ses2) ; // get surfaces
	if( n1 > 0 ||
		n2 > 0 )
	{
		int i, j, k ;
		PNT3D p, q ;
		CURVE* curve ;
		SURFACE* surface ;
		BSCUR* bsc ;
		BSSUR* bss ;
		CMark* pMark ;
		CPV* pPV = new CPV() ;
		for( i = 0 ; i < n1 ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)ses1[i] ;
			curve = pRefCur->GetCurve() ;
			if( entityGetType(curve) == etBCurve )
			{
				bsc = curveGetNurbs(curve) ;
				pMark = new CMark() ;
				p[0] = bsc->u_p[0][0]/bsc->u_p[0][3] ;
				p[1] = bsc->u_p[0][1]/bsc->u_p[0][3] ;
				p[2] = bsc->u_p[0][2]/bsc->u_p[0][3] ;
				for( j = 1 ; j < bsc->cnt_u ; j++ )
				{
					q[0] = bsc->u_p[j][0]/bsc->u_p[j][3] ;
					q[1] = bsc->u_p[j][1]/bsc->u_p[j][3] ;
					q[2] = bsc->u_p[j][2]/bsc->u_p[j][3] ;
					pMark->AddLin(p, q) ;
					memcpy(p, q, sizeof(double[3])) ;
				}
				AddFeature(pMark) ;
			}
		}

		for( i = 0 ; i < n2 ; i++ )
		{
			CRefSur* pRefSur = (CRefSur*)ses2[i] ;
			surface = pRefSur->GetSurface() ;
			if( entityGetType(surface) == etBSurface )
			{
				bss = surfaceGetNurbs(surface) ;
				pMark = new CMark() ;
				for( j = 0 ; j < bss->cnt_u ; j++ )
				{
					p[0] = bss->uv_p[j][0][0]/bss->uv_p[j][0][3] ;
					p[1] = bss->uv_p[j][0][1]/bss->uv_p[j][0][3] ;
					p[2] = bss->uv_p[j][0][2]/bss->uv_p[j][0][3] ;
					for( k = 0 ; k < bss->cnt_v ; k++ )
					{
						q[0] = bss->uv_p[j][k][0]/bss->uv_p[j][k][3] ;
						q[1] = bss->uv_p[j][k][1]/bss->uv_p[j][k][3] ;
						q[2] = bss->uv_p[j][k][2]/bss->uv_p[j][k][3] ;
						pMark->AddLin(p, q) ;
						memcpy(p, q, sizeof(double[3])) ;
					}
				}
				for( k = 0 ; k < bss->cnt_v ; k++ )
				{
					p[0] = bss->uv_p[0][k][0]/bss->uv_p[0][k][3] ;
					p[1] = bss->uv_p[0][k][1]/bss->uv_p[0][k][3] ;
					p[2] = bss->uv_p[0][k][2]/bss->uv_p[0][k][3] ;
					
					for( j = 0 ; j < bss->cnt_u ; j++ )
					{
						q[0] = bss->uv_p[j][k][0]/bss->uv_p[j][k][3] ;
						q[1] = bss->uv_p[j][k][1]/bss->uv_p[j][k][3] ;
						q[2] = bss->uv_p[j][k][2]/bss->uv_p[j][k][3] ;
						pMark->AddLin(p, q) ;
						memcpy(p, q, sizeof(double[3])) ;
					}
				}
				AddFeature(pMark) ;
			}
		}
		Redraw() ;
	}

	return ;
}

// nt add 2006/9/1
void CCAMDoc::OnAnalysisMetric()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnAnalysisMetric) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取被测量的相关点、线、面特征...") ;
		OnCmd(&CCAMDoc::OnAnalysisMetric, new CChkDlg(), IDD_CHECK) ;
		return ;
	}

	CChkDlg* pDlg = (CChkDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->m_strs.RemoveAll() ;
	int i, n = 0, n1 = 0, n2 = 0 ;
	CURVE* curves[2] = {NULL, NULL} ;
	SURFACE* surface = NULL ; // nt add 2011/6/18
	BOXUV boxuv ; // nt add 2011/6/18
	INTERVAL intervals[2] ;
	double a, d, t ;
	PNT3D ps[2], p, q ;
	VEC3D v1, v2 ;
	TCHAR str[64] ;
	n = GetSelected(ftRefCur, 2) ; // get 1 or 2 curves
	for( i = 0 ; i < n ; i++ )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
		curves[i] = pRefCur->GetCurve() ;
		pRefCur->GetInterval(&(intervals[i])) ;
	}
	n1 = GetSelected(ftRefPt, 2) ; // get 1 or 2 points
	for( i = 0 ; i < n1 ; i++ )
	{
		CRefPt* pRefPt = (CRefPt*)GetObj(i) ;
		pRefPt->GetP(ps[i]) ;
	}
	n2 = GetSelected(ftRefSur, 1) ; // get 1 surface
	for( i = 0 ; i < n2 ; i++ ) // nt add 2011/6/18
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		surface = pRefSur->GetSurface() ;
		pRefSur->GetBoxUV(&boxuv) ;
	}
	if( n == 2 && // 计算直线的夹角
		entityGetType(curves[0]) == etLine &&
		entityGetType(curves[1]) == etLine )
	{
		curveEvalTangent(curves[0], 0., p, v1) ;
		curveEvalTangent(curves[1], 0., p, v2) ;
		a = mathGetAngleUnit(v1, v2) ;
		_stprintf_s(str, _T("直线夹角=%lf度"), 180*a/PI1) ;
		pDlg->m_strs.Add(str) ;
	}
	else
	if( n == 1 && // 计算点线距离
		n1 == 1 )
	{
		curveSnap(curves[0], ps[0], p, &t) ;
		d = mathDist(p, ps[0]) ;
		_stprintf_s(str, _T("点线距离=%lfmm"), d) ;
		pDlg->m_strs.Add(str) ;
	}
	else
	if( n1 == 2 ) // 计算点点距离
	{
		d = mathDist(ps[0], ps[1]) ;
		_stprintf_s(str, _T("两点距离=%lfmm"), d) ;
		pDlg->m_strs.Add(str) ;
	}
	else
	if( n > 0 )
	{
		if( entityGetType(curves[0]) == etLine ) // 计算线段长度(弧长)
		{
			curveEvalPos(curves[0], intervals[0].t1, p) ;
			curveEvalPos(curves[0], intervals[0].t2, q) ;
			d = mathDist(p, q) ;
			_stprintf_s(str, _T("线段长度=%lfmm"), d) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(curves[0]) == etCircle ) // 计算圆弧的半径等
		{
			curveEvalPos(curves[0], 0., p) ;
			curveEvalPos(curves[0], PI1, q) ;
			d = 0.5*mathDist(p, q) ;
			a = 180*fabs(intervals[0].t2-intervals[0].t1)/PI1 ;
			_stprintf_s(str, _T("半径=%lfmm, 圆心角=%lf度"), d, a) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		{
			curveCalArcLen(curves[0], &(intervals[0]), 0.001, &d) ;
			_stprintf_s(str, _T("弧长=%lfmm"), d) ;
			pDlg->m_strs.Add(str) ;
			if( entityGetType(curves[0]) == etBCurve ) // all energy
			{
			}
		}
	}
	else
	if( n2 > 0 )
	{
		double r1, r2 ;
		RFRAME lf ;
		if( entityGetType(surface) == etPlane )
		{
			surfaceGetConicInfo(surface, &lf, &r1, &r2) ;
			_stprintf_s(str, _T("平面法矢=(%lf,%lf,%lf)"), lf.Z[0], lf.Z[1], lf.Z[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("平面上点=(%lf,%lf,%lf)"), lf.O[0], lf.O[1], lf.O[2]) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(surface) == etCylinder )
		{
			surfaceGetConicInfo(surface, &lf, &r1, &r2) ;
			_stprintf_s(str, _T("圆柱轴线=(%lf,%lf,%lf)"), lf.Z[0], lf.Z[1], lf.Z[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("轴线上点=(%lf,%lf,%lf)"), lf.O[0], lf.O[1], lf.O[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("圆柱半径=%lfmm"), r1) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(surface) == etCone )
		{
			surfaceGetConicInfo(surface, &lf, &r1, &r2) ;
			_stprintf_s(str, _T("圆锥轴线=(%lf,%lf,%lf)"), lf.Z[0], lf.Z[1], lf.Z[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("顶点=(%lf,%lf,%lf)"), lf.O[0], lf.O[1], lf.O[2]) ;
			pDlg->m_strs.Add(str) ;
			double a = acos(r1) ;
			_stprintf_s(str, _T("圆锥半角=%lf度"), a) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(surface) == etSphere )
		{
			surfaceGetConicInfo(surface, &lf, &r1, &r2) ;
			_stprintf_s(str, _T("球心=(%lf,%lf,%lf)"), lf.O[0], lf.O[1], lf.O[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("半径=%lfmm"), r1) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(surface) == etTorus )
		{
			surfaceGetConicInfo(surface, &lf, &r1, &r2) ;
			_stprintf_s(str, _T("圆环轴线=(%lf,%lf,%lf)"), lf.Z[0], lf.Z[1], lf.Z[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("中心点=(%lf,%lf,%lf)"), lf.O[0], lf.O[1], lf.O[2]) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("半径1=%lfmm"), r1) ;
			pDlg->m_strs.Add(str) ;
			_stprintf_s(str, _T("半径2=%lfmm"), r2) ;
			pDlg->m_strs.Add(str) ;
		}
		else
		if( entityGetType(surface) == etBSurface )
		{
		}
	}
	pDlg->Flush() ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2006/9/1
// 计算点到曲面的最近距离
void CCAMDoc::OnAnalysisDist()
{
	// TODO: Add your command handler code here
	return ;
}

// nt add 2006/91/1
// 计算曲线到曲面的最近距离
void CCAMDoc::OnAnalysisDist2()
{
	// TODO: Add your command handler code here
	return ;
}

// nt add 2010/9/6 显示曲线的曲率图
// nt modify 增加显示曲面的各种曲率功能 2011/4/21
void CCAMDoc::OnAnalysisCCG()
{
	// TODO: Add your command handler code here
	// nt add 2011/4/21
	int n = GetSelected(ftRefSur, 1) ;
	if( n > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		CSCGDlg dlg ;
		dlg.Init(1024, surface, &boxuv) ;
		dlg.DoModal() ;
		return ; // 如果有曲面就只处理曲面，不处理曲线了
	}

	int nStep = ChkCmd(&CCAMDoc::OnAnalysisCCG) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取一条曲线...") ;
		OnCmd(&CCAMDoc::OnAnalysisCCG, new CCCGDlg(1000), IDD_CCGDLG) ;
		return ;
	}

	CCCGDlg* pDlg = (CCCGDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->Clear() ;
	n = GetSelected(ftRefCur, 1) ;
	if( n == 1 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		double t = 0., s = 0., c = 0., min = MAX_DBL, max = -MAX_DBL ;
		PNT3D p1, p2 ;
		VEC3D T, N1, N2, B ;
		int flag = 1 ;
		for( int i = 0 ; i <= 500 ; i++ )
		{
			t = interval.t1+i*(interval.t2-interval.t1)/500. ;
			curveEvalCur(curve, t, p2, T, N2, B, &c) ;
			if( i > 0 )
			{
				if( mathOProduct(N1, N2) < 0. )
					flag *= -1 ;
			}
			c *= flag ;
			if( min > c )
				min = c ;
			if( max < c )
				max = c ;
			pDlg->AddPt(s, c) ;
			if( i > 0 )
				s += mathDist(p1, p2) ;
			memcpy(p1, p2, sizeof(PNT3D)) ;
			memcpy(N1, N2, sizeof(VEC3D)) ;
		}
		pDlg->SetMax(max) ;
		pDlg->SetMin(min) ;
		pDlg->Update() ;
	}
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2013/4/5
// 显示曲线平滑性（即(duu, duuu)）的图
void CCAMDoc::OnAnalysisCFG()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnAnalysisCFG) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		camPromptStr("拾取一条曲线...") ;
		OnCmd(&CCAMDoc::OnAnalysisCFG, new CCFGDlg(1000), IDD_CFGDLG) ;
		return ;
	}

	CCFGDlg* pDlg = (CCFGDlg*)m_pDlg ;
	if( !pDlg )
		return ; // error
	pDlg->Clear() ;
	int nn = GetSelected(ftRefCur, 1) ;
	if( nn > 0 )
	{
		CRefCur* pRefCur = (CRefCur*)GetObj(0) ;
		CURVE* curve = pRefCur->GetCurve() ;
		INTERVAL interval ;
		pRefCur->GetInterval(&interval) ;
		double t = 0., s = 0., c = 0., min = MAX_DBL, max = -MAX_DBL ;
		PNT3D p1, p2 ;
		for( int i = 0 ; i <= 500 ; i++ )
		{
			t = interval.t1+i*(interval.t2-interval.t1)/500. ;
			c = 0. ; // curveEvalEnergy(curve, t, 6, 1.e-5) ; // (duu,duuu)
			curveEvalPos(curve, t, p1) ;
			if( min > c )
				min = c ;
			if( max < c )
				max = c ;
			pDlg->AddPt(s, c) ;
			if( i > 0 )
				s += mathDist(p1, p2) ;
			memcpy(p1, p2, sizeof(PNT3D)) ;
		}
		pDlg->SetMax(max) ;
		pDlg->SetMin(min) ;
		pDlg->Update() ;
	}

	int mm = 0 ;
	if( nn == 0 )
		mm = GetSelected(ftRefSur, 1) ;
	if( mm > 0 )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(0) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		BOXUV boxuv ;
		pRefSur->GetBoxUV(&boxuv) ;
		if( entityGetType(surface) != etBSurface )
			return ;
		BSSUR* nurbs = surfaceGetNurbs(surface) ;
		double u, v, e = 0. ;
		int i, j, n = 100, m = 100 ;
		for( i = 0 ; i <= n ; i++ )
		{
			u = boxuv.min[0]+i*(boxuv.max[0]-boxuv.min[0])/n ;
			for( j = 0 ; j <= m ; j++ )
			{
				v = boxuv.min[1]+i*(boxuv.max[1]-boxuv.min[1])/n ;
				// |(Suu,Suuu)|/||Su||+|(Svv,Svvv)|/||Sv||
				e += 0. ;
			}
		}
		e /= (101*101) ;
		pDlg->SetMax(e) ;
		pDlg->SetMin(e) ;
		pDlg->Update() ;
	}

	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}
//======================================================================
void CCAMDoc::OnToolTest()
{
	return ;
}
//==============================================================
// nt add 2010/9/27
void CCAMDoc::OnNCModel()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnNCModel) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		OnCmd(&CCAMDoc::OnNCModel, new CModelDlg(this), IDD_MODELDLG) ;
		return ;
	}

	CModelDlg* pDlg = (CModelDlg*)m_pDlg ;
	if( !pDlg )
		return ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2010/9/27
void CCAMDoc::OnNCMCS()
{
	// TODO: Add your command handler code here
	if( m_pMill )
	{
		int n = GetSelected(ftRefPlane, 1) ;
		if( n > 0 )
		{
			CRefPlane* pRefPlane = (CRefPlane*)GetObj(0) ;
			if( pRefPlane )
			{
				RFRAME lf ;
				pRefPlane->GetLF(&lf) ;
				m_pMill->SetMCS(&lf) ;
			}
		}
	}
}

// nt add 2010/9/27
void CCAMDoc::OnNCParameters()
{
	// TODO: Add your command handler code here
	if( m_pMill )
	{
		CCPDlg dlg ;
		if( dlg.DoModal() == IDOK )
		{
			// CP cp ;
			// ...
			// m_pMill->SetCP(&cp) ;
		}
	}
}

// nt add 2010/9/27
void CCAMDoc::OnNCMachine()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnNCMachine) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		OnCmd(&CCAMDoc::OnNCMachine, new CMachineDlg(), IDD_MACHINEDLG) ;
		return ;
	}

	CMachineDlg* pDlg = (CMachineDlg*)m_pDlg ;
	if( !pDlg )
		return ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2010/9/27
void CCAMDoc::OnNCMaterialLib()
{
	// TODO: Add your command handler code here
}

// nt add 2010/9/27
// nt modify 2010/11/29
void CCAMDoc::OnNCPDesign()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnNCPDesign) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive, nt modify 2010/11/29
	{
		CPDesignDlg* pDlg = new CPDesignDlg(this) ;
		int n = GetSelected(camStep, 1) ;
		if( n > 0 )
		{
			pDlg->m_pStep = (CStep*)GetObj(0) ;
			pDlg->m_pStep->GetName(pDlg->m_name) ;
		}
		OnCmd(&CCAMDoc::OnNCPDesign, pDlg, IDD_PDESIGNDLG) ;
		return ;
	}

	CPDesignDlg* pDlg = (CPDesignDlg*)m_pDlg ;
	if( !pDlg )
		return ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

void CCAMDoc::OnNCToolLib()
{
	// TODO: Add your command handler code here
	int nStep = ChkCmd(&CCAMDoc::OnNCToolLib) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		OnCmd(&CCAMDoc::OnNCToolLib, new CTLibDlg(this), IDD_TLIBDLG) ;
		return ;
	}

	CTLibDlg* pDlg = (CTLibDlg*)m_pDlg ;
	if( !pDlg )
		return ;
	if( m_ui == 1 )
		CmdEnd() ;

	return ;
}

// nt add 2010/9/27
void CCAMDoc::OnNCProgram()
{
	// TODO: Add your command handler code here
}

// nt add 2010/9/27
void CCAMDoc::OnNCPostProcess()
{
	// TODO: Add your command handler code here
}

// nt add 2010/9/27
void CCAMDoc::OnNCExportCLF()
{
	// TODO: Add your command handler code here
}

// nt add 2010/9/27
void CCAMDoc::OnNCExportDoc()
{
	// TODO: Add your command handler code here
}
//==============================================================
void CCAMDoc::OnSimMotion()
{
	// TODO: Add your command handler code here
	POSITION pos = GetFirstViewPosition() ;
	CCAMView* pView = (CCAMView*)GetNextView(pos) ;
	if( pView )
	{
		pView->SendMessage(WM_COMMAND, ID_VIEW_SIM5) ;
	}
}
//==============================================================
// [example]
// nt add 2010/8/22
// this function is a standard user interactive process!
/*void CCAMDoc::OnTest()
{
	int nStep = ChkCmd(&CCAMDoc::OnTest) ; // check cmd step
	if( nStep < 0 )
		return ; // error
	else
	if( nStep == 0 ) // start user interactive
	{
		OnCmd(&CCAMDoc::OnTest, new CXYZDlg(), IDD_XYZDLG) ;
		return ;
	}
	
	// finished user interactive
	// vist CXYZDlg...
	// implement some functions in CCAMDoc...
	if( m_ui == 1 ) // nt add 2010/8/28, allow to use "apply"
		CmdEnd() ; // end and initialize cmd

	return ;
}*/
//==============================================================
// nt add 2021/6/13
void CCAMDoc::OnToolChkMesh()
{
	// TODO: Add your command handler code here
	int i, j, k, I, J, K, n = GetSelected(ftRefSur, 100000), nErr = 0 ;
	double a, b, c, d, minEdge, maxEdge, minAngle, u, v, maxDev, dTol ;
	PNT3D p, q ;
	OPTION* option = camGetOption() ;

	if( n < 1 )
	{
		camPromptStr("先拾取要检测的曲面...") ;
		return ;
	}
	
	minEdge = MAX_DBL ;
	maxEdge = 0. ;
	minAngle = 180. ;
	maxDev = 0. ;
	dTol = option->dTol ;
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		SURFACE* surface = pRefSur->GetSurface() ;
		FACET* facet = pRefSur->GetFacet() ;
		if( surface == NULL ||
			facet == NULL )
			continue ;
		for( j = 0 ; j < facet->ntria ; j++ )
		{
			I = facet->trias[j][0] ;
			J = facet->trias[j][1] ;
			K = facet->trias[j][2] ;

			a = mathDist(facet->nodes[I].p, facet->nodes[J].p) ;
			b = mathDist(facet->nodes[J].p, facet->nodes[K].p) ;
			c = mathDist(facet->nodes[K].p, facet->nodes[I].p) ;
			if( a < c )
			{
				d = c ;
				c = a ;
				a = d ;
			}
			if( b < c )
			{
				d = c ;
				c = b ;
				b = d ;
			}
			if( minEdge > c )
				minEdge = c ;
			if( maxEdge < a )
				maxEdge = a ;
			if( maxEdge < b )
				maxEdge = b ;

			d = acos((a*a+b*b-c*c)/(2*a*b))/PI1*180 ;
			if( minAngle > d )
				minAngle = d ;

			u = (facet->nodes[I].uv[0]+
				 facet->nodes[J].uv[0]+
				 facet->nodes[K].uv[0])/3 ;
			v = (facet->nodes[I].uv[1]+
				 facet->nodes[J].uv[1]+
				 facet->nodes[K].uv[1])/3 ;
			for( k = 0 ; k < 3 ; k++ )
				p[k] = (facet->nodes[I].p[k]+
				        facet->nodes[J].p[k]+
				        facet->nodes[K].p[k])/3 ;
			//surfaceSnap(surface, p, q, &u, &v) ;
			surfaceEvalPos(surface, u, v, q) ;
			d = mathDist(p, q) ;
			if( d > dTol )
			{
				AddPt(p) ;
				nErr++ ;
			}
			if( maxDev < d )
				maxDev = d ;
		}
	}
	CStr str ;
	str.Format("共%d张曲面,最小边=%3.3lf,最大边=%3.3lf,最小角=%3.3lf°,弦高差=%3.3lf,超差点=%d",
			   n,
		       minEdge,
			   maxEdge,
			   minAngle,
			   maxDev,
			   nErr) ;
	camPromptStr(str) ;

	return ;
}

int curveIsOverlap2(CURVE* curve1,
					INTERVAL* interval1,
					CURVE* curve2,
					INTERVAL* interval2,
					double tol,
					int* yesno)
{
	int i, type1, type2 ;
	double d, s, t, ang = PI1/8 ;
	PNT3D b1, e1, b2, e2, p, q ;
	STRPT* disc = NULL ;

	*yesno = ID_NO ;
	type1 = entityGetType(curve1) ;
	type2 = entityGetType(curve2) ;
	curveEvalPos(curve1, interval1->t1, b1) ;
	curveEvalPos(curve1, interval1->t2, e1) ;
	curveEvalPos(curve2, interval2->t1, b2) ;
	curveEvalPos(curve2, interval2->t2, e2) ;
	if( (mathDist(b1, b2) < tol && 
		 mathDist(e1, e2) < tol) ||
		(mathDist(b1, e2) < tol && 
		 mathDist(e1, b2) < tol) )
	{
		if( type1 == etLine &&
			type2 == etLine )
		{
			*yesno = ID_YES ;
			return ERSUCSS ;
		}
		for( i = 1 ; i <= 3 ; i++ )
		{
			s = interval1->t1+0.25*(interval1->t2-interval1->t1)*i ;
			curveEvalPos(curve1, s, p) ;
			t = 1.e20 ;
			curveSnap(curve2, p, q, &t) ;
			d = mathDist(p, q) ;
			if( d > tol )
				return ERSUCSS ;
			s = interval2->t1+0.25*(interval2->t2-interval2->t1)*i ;
			curveEvalPos(curve2, s, p) ;
			t = 1.e20 ;
			curveSnap(curve1, p, q, &t) ;
			d = mathDist(p, q) ;
			if( d > tol )
				return ERSUCSS ;
		}
		if( type1 <= etEllipse && // 直线、圆、椭圆三种情况
			type2 <= etEllipse )
		{
			*yesno = ID_YES ;
			return ERSUCSS ;
		}

		curveDisc(curve1,
				  interval1,
				  10*tol,
				  ang,
				  1000.*tol,
				  STRPT_PT,
				  &disc) ;
		if( disc == NULL )
			return ERUNSUC ;
		t = 1.e20 ;
		for( i = 1 ; i < disc->np-1 ; i++ )
		{
			curveSnap(curve2, disc->ps[i], q, &t) ; // nearest point
			d = mathDist(disc->ps[i], q) ;
			if( d > tol )
			{
				strptFree(disc) ;
				return ERSUCSS ;
			}
		}
		strptFree(disc) ;
		disc = NULL ;

		curveDisc(curve2,
				  interval2,
				  10*tol,
				  ang,
				  1000.*tol,
				  STRPT_PT,
				  &disc) ;
		if( disc == NULL )
			return ERUNSUC ;
		t = 1.e20 ;
		for( i = 1 ; i < disc->np-1 ; i++ )
		{
			curveSnap(curve1, disc->ps[i], q, &t) ; // nearest point
			d = mathDist(disc->ps[i], q) ;
			if( d > tol )
			{
				strptFree(disc) ;
				return ERSUCSS ;
			}
		}
		strptFree(disc) ;
		disc = NULL ;

		*yesno = ID_YES ;
	}

	return ERSUCSS ;
}

void CCAMDoc::OnToolChkBnd()
{
	// TODO: Add your command handler code here
	int i, j, I, J, n, yesno, nMatched = 0 ;
	double dTol, bTol ;
	BOX3D box1, box2 ;
	BNDX* bnd1, *bnd2 ;
	CRefSur* pRefSur1, *pRefSur2 ;
	OPTION* option = NULL ;
	COA ss ;

	n = GetSelected(ftRefSur, 100000) ;
	if( n < 2 )
	{
		camPromptStr("先拾取至少两张曲面...") ;
		return ;
	}
	for( i = 0 ; i < n ; i++ )
	{
		pRefSur1 = (CRefSur*)GetObj(i) ;
		if( pRefSur1 &&
			pRefSur1->m_n > 0 )
		{
			pRefSur1->InitBndAdj() ;
			ss.Add(pRefSur1) ;
		}
	}

	option = camGetOption() ;
	dTol = option->dTol ;
	bTol = option->bTol ;

	n = ss.GetSize() ;
	for( I = 0 ; I < n ; I++ )
	{
		pRefSur1 = (CRefSur*)ss[I] ;
		for( i = 0 ; i < pRefSur1->m_n ; i++ )
		{
			bnd1 = &pRefSur1->m_bnds[i] ;
			box1 = bnd1->box ;
			for( J = I+1 ; J < n ; J++ )
			{
				pRefSur2 = (CRefSur*)ss[J] ;
				pRefSur2->GetBox(&box2) ;
				if( box2.min[0]-dTol < box1.min[0] &&
					box1.max[0] < box2.max[0]+dTol &&
					box2.min[1]-dTol < box1.min[1] &&
					box1.max[1] < box2.max[1]+dTol &&
					box2.min[2]-dTol < box1.min[2] &&
					box1.max[2] < box2.max[2]+dTol )
				{
					for( j = 0 ; j < pRefSur2->m_n ; j++ )
					{
						bnd2 = &pRefSur2->m_bnds[j] ;
						if( mathDist(bnd1->box.min, bnd2->box.min) < dTol &&
							mathDist(bnd1->box.max, bnd2->box.max) < dTol )
						{
							curveIsOverlap2(bnd1->curve,
											&bnd1->interval,
											bnd2->curve,
											&bnd2->interval,
											bTol,
											&yesno) ;
							if( yesno == ID_YES )
							{
								bnd1->nAdj++ ;
								bnd2->nAdj++ ;
								nMatched++ ;
							}
						}			
					}
				}
			}
		}
	}
	ss.RemoveAll() ;
	CStr str ;
	str.Format("共匹配%d条边", nMatched) ;
	camPromptStr(str) ;

	return ;
}

void CCAMDoc::OnToolOption()
{
	OPTION* pOpt = camGetOption() ;
	COptionDlg dlg ;

	dlg.m_dTol = pOpt->dTol ;
	dlg.m_minEdge = pOpt->minEdge ;
	dlg.m_maxEdge = pOpt->maxEdge ;
	dlg.m_minAngle = pOpt->minAngle ;
	dlg.m_minArea = pOpt->minArea ;
	dlg.m_bTol = pOpt->bTol ;
	if( dlg.DoModal() == IDOK )
	{
		pOpt->dTol = dlg.m_dTol ;
		pOpt->minEdge = dlg.m_minEdge ;
		pOpt->maxEdge = dlg.m_maxEdge ;
		pOpt->minAngle = dlg.m_minAngle ;
		pOpt->minArea = dlg.m_minArea ;
		pOpt->bTol = dlg.m_bTol ;
		camSetOption(pOpt) ;
	}

	return ;
}

void CCAMDoc::OnToolMeshRefine()
{
	// TODO: Add your command handler code here
	int i, n = GetSelected(ftRefSur, 100000) ;
	OPTION* option = camGetOption() ;

	if( n < 1 )
	{
		camPromptStr("先拾取需网格加密曲面...") ;
		return ;
	}
	
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		pRefSur->RefineFacet(option->dTol) ;
	}

	return ;
}

void CCAMDoc::OnToolOptTri1()
{
	// TODO: Add your command handler code here
	int i, n = GetSelected(ftRefSur, 100000) ;

	if( n < 1 )
	{
		camPromptStr("先拾取需优化的曲面...") ;
		return ;
	}
	
	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		pRefSur->OptTri(50) ;
	}

	return ;
}

void CCAMDoc::OnToolOptTri2()
{
	// TODO: Add your command handler code here
	int i, n = GetSelected(ftRefSur, 100000) ;

	if( n < 1 )
	{
		camPromptStr("先拾取需重新剖分的曲面...") ;
		return ;
	}

	for( i = 0 ; i < n ; i++ )
	{
		CRefSur* pRefSur = (CRefSur*)GetObj(i) ;
		pRefSur->ReDisc() ;
	}
	
	return ;
}

void CCAMDoc::OutPutChordalHeight(double* iChordalHeight, int iNumStart, int iNumEnd)
{
	FILE* fp = nullptr;
	fopen_s(&fp, "C:\\Test\\OutPutChordalHeight.txt", "w+");
	if (fp)
	{
		for (int i = iNumStart; i <= iNumEnd; i++)
		{
			fprintf(fp, "ChordalHeight[%d] = %.6f mm\n", i, iChordalHeight[i]);
		}

		fclose(fp);
	}

	fp = nullptr;
}
////////////////////////////////////////////////////////////////

