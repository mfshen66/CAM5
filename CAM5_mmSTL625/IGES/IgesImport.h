#ifndef _IgesImport_h_
#define _IgesImport_h_

////////////////////////////////////////////////////////////////
// 导入IGES
class CIgesImport
{
private:
	int m_flags ; // 处理:&1=point, &2=curve, &4=裁剪曲面
	IGES* m_iges ; // 存储读入的所有IGES entity
	int m_max ; // maximum value of deep, 为避免循环嵌套
	int m_deep ; // ientity2xxx()的嵌套层次, 为避免循环嵌套
	COA m_fs ; // 需要转成IGES或从IGES中转来的CFeature，可能含裁剪曲面CTS
	double m_tol ; // nt add 2010/5/20
public:
	COA m_errBnds ; // nr add 2021/3/6

public:
	CIgesImport(int flags) ;
	~CIgesImport() ;

	int GetMax() ;
	void SetMax(int max) ;
	int GetNumOfFeature() ;
	CFeature* GetFeature(int i) ;
	void ClearFeatures() ; // nt add 2010/5/19
	int Import(CString& fileName) ; // return 0=error, 1=ok
	int Import3(CString& fileName) ; // nt add 2021/8/4 专用

private:
	int ientity2LF(IENTITY* pE, RFRAME* pLF) ;
	int ientity2Point(IENTITY* pE, PNT3D p) ;
	CURVE* ientity2Curve(IENTITY* pE, INTERVAL* interval) ;
	SURFACE* ientity2Surface(IENTITY* pE, BOXUV* boxuv) ;
	TFACE* ientity2TFace(IENTITY* pE, int sp) ; // nt add 2010/5/18, loops in space
	int ientity2TLoop(IENTITY* pE, TLOOP* tloop, int sp) ; // nt add 2010/5/21
	//----------------------------------------------------------
	CURVE* ientity2Line(IENTITY* pE, INTERVAL* interval) ;
	CURVE* ientity2Arc(IENTITY* pE, INTERVAL* interval) ;
	CURVE* ientity2BSCur(IENTITY* pE, INTERVAL* interval) ;
	CURVE* ientity2Conic(IENTITY* pE, INTERVAL* interval) ;
	CURVE* ientity2OffCur(IENTITY* pE, INTERVAL* interval) ;
	SURFACE* ientity2Plane(IENTITY* pE, BOXUV* boxuv) ;
	SURFACE* ientity2Rule2(IENTITY* pE, BOXUV* boxuv) ;
	SURFACE* ientity2Rule(IENTITY* pE, BOXUV* boxuv) ;
	SURFACE* ientity2Revolve(IENTITY* pE, BOXUV* boxuv) ;
	SURFACE* ientity2BSSur(IENTITY* pE, BOXUV* boxuv) ;
	SURFACE* ientity2OffSur(IENTITY* pE, BOXUV* boxuv) ;
} ;
////////////////////////////////////////////////////////////////

#endif
