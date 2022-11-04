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

// CAMDoc.h : CCAMDoc 类的接口
//
#pragma once

#include "ViewTree.h" // nt add 2010/9/9

////////////////////////////////////////////////////////////////
class CCommand ;
class CSelSet ;
class CPart ;
class CMill ;
class CPV ;
class CCAMView ;
//--------------------------------------------------------------
class CCAMDoc ;
typedef void (__thiscall CCAMDoc::*MHANDLER)() ; // message handler
//--------------------------------------------------------------
class CCAMDoc : public CDocument
{
private:
	// flag to be set 1 when destroying this CDocument
	int m_bDestroy ; // 0, 1 : nt add 2002/5/21
	// font
	LOGFONT m_lf ; // current document font
	// command
	CCommand* m_pCmd ; // current command
	int m_lastCmd ;
	// select set
	CSelSet* m_pSelSet ;
	COA m_ses ; // nt add 2009/12/5, 目的是简化拾取代码的复杂度
	// navigate
	int m_drawed ; // 0,1:not drawed, has drawed the navigated point
	PNT3D m_nav_p ; // navigated point
	// for preview
	CPV* m_pPV ; // nt add 2002/5/21
	// workplane
	double m_gridSize ; // work plane grid size
	CRefPlane* m_pWorkPlane ; // current workplane

	// model
	CPart* m_pPart ; // part model
	// CAM
	CMill* m_pMill ; // nt add 2010/4/1
	// model tree
	CViewTree m_tree ; // model tree, nt add 2010/9/8
	CImageList m_images ; // nt add 2010/9/8
	HTREEITEM m_hPart ; // 对应m_pPart, nt add 2010/9/8
	HTREEITEM m_hMill ; // 对应m_pMill, nt add 2010/9/8

	// current color index and material index, nt add 2010/10/17
	//int m_colorIndex ; // from 0 to 255
	//int m_materialIndex ; // from 256 to 256+41
	// 以上两项转移到了theApp.m_option, nt 2010/11/28

public:
	CCAMView* GetActiveView() ; // nt add 2003/2/15
	int IsDestroying() ; // nt add 2002/5/21

	void AddPV(CPV* pPV) ; // nt add 2002/5/21
	void RemovePV() ;

	void GetLogFont(LOGFONT* pLF) ;
	double GetGridSize() ;
	void SetGridSize(double gridSize) ;
	CRefPlane* GetWorkPlane() ;
	CPart* GetPart() ;
	CMill* GetMill() ; // nt add 2010/6/2
	int GetBox(double min[3], double max[3]) ; // nt add 2009/5/9
	int AddFeature(CFeature* pF) ; // nt add 2009/12/17
	int DelFeature(CFeature* pF) ; // nt add 2010/9/1
	int AddStep(CStep* pStep) ; // nt add 2010/11/29
	int DelStep(CStep* pStep) ; // nt add 2010/11/29
	int AddLin(PNT3D p, PNT3D q) ; // nt add 2010/3/24
	int AddPt(PNT3D p) ; // nt add 2010/12/27
	CSelSet* GetSelSet() ;
	void ClearSelSet() ; // nt add 2011/8/22
	int GetNumOfSelected() ; // nt add 2009/12/5
	int GetSelected(int type, int max) ; // nt add 2009/12/5
	void* GetObj(int i) ; // nt add 2009/12/5
	int GetType(int i) ; // nt add 2009/12/5
	double GetU(int i) ; // nt add 2009/12/5
	double GetV(int i) ; // nt add 2009/12/18
	int GetGPP(PNT3D gpp) ; // nt add 2021/6/24 geometry picked position
	
	void Draw(void* pVI, int drawMode) ;
	void Redraw() ;
	void OnActivate() ; // nt add 2010/9/9
	CViewTree* GetTree() ; // nt add 2010/9/9
	int RebuildModelTree() ; // nt add 2010/9/9
	
	void ClearCmd() ;
	void ClearCmdAll() ;
	int HasCmd() ; // nt add 2009/12/17
	int GetLastCmd() ;
	int SetLastCmd(int cmd) ;

	int Navigate(DNAVI* dn) ;
	int NavigateKeyPt(DNAVI* dn) ; // nt add 2010/3/26
	void DrawNavP(int mode) ; // mode = 0(eraze) or 1(draw)

	void OnMouseMove(int nFlags, double p[3], double v[3], void* pVI) ; // nt 2000/7/18
	void OnLButtonDown(int nFlags, double p[3], double v[3], void* pVI) ; // nt 2000/7/18
	void OnLButtonUp(int nFlags, double p[3], double v[3], void* pVI) ; // nt 2000/7/18
	void OnRButtonDown(int nFlags, double p[3], double v[3], void* pVI) ; // nt 2000/7/18
	void OnRButtonUp(int nFlags, double p[3], double v[3], void* pVI) ; // nt 2000/7/18
	int OnReturn() ; // nt add 2009/10/21
	int AddPtWithColor(PNT3D p,int color) ;
	void Compare(CURVE* curve, BSCUR* nurbs, INTERVAL* interval, int nSample) ;
	void Compare(SURFACE* surface, BSSUR* nurbs, BOXUV* boxuv, int nSample) ;

	// smf add 2022/10/09
	// 输出弓高数据至C:\\Test\\OutPutChordalHeight.txt。
	void OutPutChordalHeight(double* iChordalHeight, int iNum);

protected: // 仅从序列化创建
	CCAMDoc();
	DECLARE_DYNCREATE(CCAMDoc)

// 特性
public:
	//UINT m_cmdID ; // 当前的命令ID
	MHANDLER m_mh ; // message handler=CCAMDoc的成员函数,default value=NULL
	CDialogEx* m_pDlg ; // 当前命令使用的对话框
	int m_nStep ; // 交互状态的第几步, default value = 0
	int m_ui ; // 当前用户交互状态:-1=idle,0=cancel,1=ok,2=apply, nt modify 2010/8/28

// 操作
public:
	int ChkCmd(MHANDLER mh) ; // 检查命令状态，nt add 2010/8/22
	int OnCmd(MHANDLER mh, CDialogEx* pDlg, int ID) ; // 启动命令及命令对话框，nt add 2010/8/22
	int CmdDlgUI(int ui) ; // 命令对话框用户交互，nt add 2010/8/22
	int CmdEnd() ; // 命令结束，nt add 2010/8/22
	int FreeCmdDlg() ; // 释放命令窗口及对话框, nt add 2010/8/22

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CCAMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
public:
	void SetSearchContent(const CString& value);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileSaveNURBS();
	afx_msg void OnFileLoadGE();
	afx_msg void OnFileSaveFeatures();
	afx_msg void OnFileLoadFeatures();
	afx_msg void OnFileSaveSTL();
	afx_msg void OnFileLoadIGES();
	afx_msg void OnEditDelete();
	afx_msg void OnEditMove();
	afx_msg void OnEditRotate();
	afx_msg void OnEditScale();
	afx_msg void OnEditMirror();
	afx_msg void OnEditColor();
	afx_msg void OnEditWidth();
	afx_msg void OnEditMove2Ori();
	afx_msg void OnInsertWPlane();
	afx_msg void OnInsertOffset();
	afx_msg void OnInsertNormalPlane();
	afx_msg void OnInsertPlane3P();
	afx_msg void OnInsertPlane2P();
	afx_msg void OnInsertPlaneX();
	afx_msg void OnInsertPlaneY();
	afx_msg void OnInsertPlaneZ();
	afx_msg void OnInsertTanPln();
	afx_msg void OnInsertPlaneMove();
	afx_msg void OnInsertRefPt();
	afx_msg void OnInsertInterp();
	afx_msg void OnInsertLin();
	afx_msg void OnInsertCircle();
	afx_msg void OnInsertEll();
	afx_msg void OnInsertArcRAA();
	afx_msg void OnInsertArcLL();
	afx_msg void OnInsertArc();
	afx_msg void OnInsertLine2Cir();
	afx_msg void OnInsertFilletCir();
	afx_msg void OnInsertFilletCur();
	afx_msg void OnInsertBC();
	afx_msg void OnInsertCurves();
	afx_msg void OnInsertUVCurve();
	afx_msg void OnInsertSurBorder();
	afx_msg void OnInsertIntCur();
	afx_msg void OnInsertIsoCur();
	afx_msg void OnInsertPlane();
	afx_msg void OnInsertExtrude();
	afx_msg void OnInsertRevolve();
	afx_msg void OnInsertRule();
	afx_msg void OnInsertBndSur4();
	afx_msg void OnInsertLoft();
	afx_msg void OnInsertSweep();
	afx_msg void OnInsertPipe();
	afx_msg void OnInsertConnect();
	void InsertConnect1() ;
	void InsertConnect2() ;
	afx_msg void OnInsertFill();
	afx_msg void OnInsertFileSur();
	afx_msg void OnInsertCyl();
	afx_msg void OnInsertSph();
	afx_msg void OnInsertCone();
	afx_msg void OnInsertTorus();
	afx_msg void OnInsertOffSur();
	afx_msg void OnInsertPts();
	afx_msg void OnInsertMPoints2Curve();
	afx_msg void OnInsertSurPts();
	afx_msg void OnInsertPic();
	afx_msg void OnInsertPCloud();
	afx_msg void OnInsertFitSur();
	afx_msg void OnInsertPath5();
	afx_msg void OnInsertMinDist();
	afx_msg void OnToolBreak();
	afx_msg void OnToolTrimCur();
	afx_msg void OnToolToNURBS();
	afx_msg void OnToolBCurG1();
	afx_msg void OnToolBCurG2();
	afx_msg void OnToolCurveExtend();
	afx_msg void OnToolSmthCur();
	afx_msg void OnToolMorph();
	afx_msg void OnToolTanLin2();
	afx_msg void OnToolCurPts();
	afx_msg void OnToolChkCurGx() ;
	afx_msg void OnToolChkG0();
	afx_msg void OnToolChkG1();
	afx_msg void OnToolBSurG1();
	afx_msg void OnToolBSurG2();
	afx_msg void OnToolChkSurGx();
	afx_msg void OnToolMeshSur();
	afx_msg void OnToolCompPrj();
	afx_msg void OnToolTest();
	afx_msg void OnToolModSurCPt();
	afx_msg void OnToolModSurIPt();
	afx_msg void OnToolCutSur();
	afx_msg void OnToolRevUV();
	afx_msg void OnToolSurClose();
	afx_msg void OnToolSurSmth();
	afx_msg void OnToolMeshRefine();
	afx_msg void OnAnalysisCurvatureComb();
	afx_msg void OnAnalysisDrawUV();
	afx_msg void OnAnalysisRefine();
	afx_msg void OnAnalysisControlPts();
	afx_msg void OnAnalysisMetric();
	afx_msg void OnAnalysisDist();
	afx_msg void OnAnalysisDist2();
	afx_msg void OnAnalysisCCG();
	afx_msg void OnAnalysisCFG();
	afx_msg void OnNCModel();
	afx_msg void OnNCMCS();
	afx_msg void OnNCParameters();
	afx_msg void OnNCMachine();
	afx_msg void OnNCMaterialLib();
	afx_msg void OnNCPDesign();
	afx_msg void OnNCToolLib();
	afx_msg void OnNCProgram();
	afx_msg void OnNCPostProcess();
	afx_msg void OnNCExportCLF();
	afx_msg void OnNCExportDoc();
	afx_msg void OnSimMotion();
	afx_msg void OnTest();
	afx_msg void OnToolChkMesh(); // nt add 2021/6/13
	afx_msg void OnToolChkBnd(); // nt add 2021/6/13
	afx_msg void OnToolOption(); // nt add 2021/6/16
	afx_msg void OnToolOptTri1(); // nt add 2021/8/13
	afx_msg void OnToolOptTri2(); // nt add 2021/8/13
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
#endif // SHARED_HANDLERS
};
////////////////////////////////////////////////////////////////
