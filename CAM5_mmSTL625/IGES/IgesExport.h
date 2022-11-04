#ifndef _IgesExport_h_
#define _IgesExport_h_

////////////////////////////////////////////////////////////////
// 导出IGES
// nt 2012/10/13
class CIgesExport
{
private:
	int m_flags ; // 处理:&1=point, &2=curve, &4=裁剪曲面
	IGES* m_iges ; // 存储读入的所有IGES entity
	COA m_fs ;
	double m_tol ;

public:
	CIgesExport(int flags) ;
	~CIgesExport() ;

	void AddFeature(CFeature* pF) ;
	int Export(CString& fileName) ; // return 0=error, 1=ok
private:
	IENTITY* iPoint2Entity(PNT3D p) ;
	IENTITY* iCurve2Entity(CURVE* curve, INTERVAL* interval) ;
	IENTITY* iSurface2Entity(SURFACE* surface, BOXUV* boxuv) ;
	IENTITY* iTFace2Entity(TFACE* pTFace) ;
} ;
////////////////////////////////////////////////////////////////

#endif
