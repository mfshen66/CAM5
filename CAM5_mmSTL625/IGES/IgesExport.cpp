#include "StdAfx.h"
#include "..\Models.h"
#include "..\API\IGES_API.h"
#include "IgesExport.h"

////////////////////////////////////////////////////////////////
// IGES Export
// nt 2012/10/13
CIgesExport::CIgesExport(int flags)
{
	m_flags = flags ; // 缺省只读裁剪曲面
	m_iges = NULL ;
	m_tol = 1.e-4 ;
}

CIgesExport::~CIgesExport()
{
	m_fs.RemoveAll() ;
	if( m_iges )
	{
		igesFree(m_iges) ;
		m_iges = NULL ;
	}
}
//--------------------------------------------------------------
void CIgesExport::AddFeature(CFeature* pF)
{
	m_fs.Add(pF) ;
}
//--------------------------------------------------------------
// nt add 2012/10/13
// 根据m_flags的设置按需导出IGES的几何元素
int CIgesExport::Export(CString& fileName)
{
	if( m_iges )
		igesFree(m_iges) ;
	m_iges = igesCreate() ;
	if( !m_iges )
		return 0 ; // error
	int i, n = m_fs.GetSize(), type ;
	PNT3D p ;
	CURVE* curve = NULL ;
	INTERVAL interval ;
	SURFACE* surface ;
	BOXUV boxuv ;
	CFeature* pF = NULL ;
	IENTITY* pE = NULL ;
	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_fs[i] ;
		if( pF )
		{
			type = pF->GetType() ;
			switch(type)
			{
			case ftRefPt:
				if( m_flags&1 )
				{
					((CRefPt*)pF)->GetP(p) ;
					pE = iPoint2Entity(p) ;
					igesAdd(m_iges, pE) ;
				}
				break ;
			case ftRefCur:
				if( m_flags&2 )
				{
					curve = ((CRefCur*)pF)->GetCurve() ;
					((CRefCur*)pF)->GetInterval(&interval) ;
					pE = iCurve2Entity(curve, &interval) ;
					igesAdd(m_iges, pE) ;
				}
				break ;
			case ftRefSur:
				if( m_flags&4 )
				{
					surface = ((CRefSur*)pF)->GetSurface() ;
					((CRefSur*)pF)->GetBoxUV(&boxuv) ;
					pE = iSurface2Entity(surface, &boxuv) ;
					igesAdd(m_iges, pE) ;
				}
				break ;
			default:
				break ;
			}
		}
	}
	int rt = igesExport(m_iges, fileName.GetBuffer(0)) ;
	igesFree(m_iges) ;
	m_iges = NULL ;

	return rt ;
}
//--------------------------------------------------------------
IENTITY* CIgesExport::iPoint2Entity(PNT3D p)
{
	IENTITY* pE ;

	pE = ientityCreate(116) ;
	pointGetP(pE, p) ;

	return pE ;
}

IENTITY* CIgesExport::iCurve2Entity(CURVE* curve, INTERVAL* interval)
{
	int type, i ;
	PNT3D p ;
	BSCUR* bscur ;
	IENTITY* pE ;

	type = entityGetType(curve) ;
	switch(type)
	{
	case etLine:
		pE = ientityCreate(110) ;
		curveEvalPos(curve, interval->t1, p) ;
		lineSetB(pE, p) ;
		curveEvalPos(curve, interval->t2, p) ;
		lineSetE(pE, p) ;
		break ;
	case etCircle:
		break ;
	case etBCurve:
		pE = ientityCreate(126) ;
		bscur = curveGetNurbs(curve) ;
		bscurCreate(pE, bscur->cnt_u, bscur->ord_u) ;
		for( i = 0 ; i < bscur->ord_u+bscur->cnt_u ; i++ )
			bscurSetKnot(pE, i, bscur->u_k[i]) ;
		for( i = 0 ; i < bscur->cnt_u ; i++ )
			bscurSetCP(pE, i, bscur->u_p[i]) ;
		break ;
	default:
		break ;
	}

	return pE ;
}

IENTITY* CIgesExport::iSurface2Entity(SURFACE* surface, BOXUV* boxuv)
{
	int type, i, j ;
	BSSUR* bssur ;
	IENTITY* pE ;

	type = entityGetType(surface) ;
	switch(type)
	{
	case etPlane:
		break ;
	case etBSurface:
		pE = ientityCreate(128) ;
		bssur = surfaceGetNurbs(surface) ;
		bssurCreate(pE, bssur->cnt_u, bssur->cnt_v, bssur->ord_u, bssur->ord_v) ;
		for( i = 0 ; i < bssur->ord_u+bssur->cnt_u ; i++ )
			bssurSetKnotU(pE, i, bssur->u_k[i]) ;
		for( i = 0 ; i < bssur->ord_v+bssur->cnt_v ; i++ )
			bssurSetKnotV(pE, i, bssur->v_k[i]) ;
		for( i = 0 ; i < bssur->cnt_u ; i++ )
		{
			for( j = 0 ; j < bssur->cnt_v ; j++ )
				bssurSetCP(pE, i, j, bssur->uv_p[i][j]) ;
		}
		break ;
	default:
		break ;
	}

	return pE ;
}

IENTITY* CIgesExport::iTFace2Entity(TFACE* pTFace)
{
	if( pTFace == NULL )
		return NULL ;
	return NULL ;
}
////////////////////////////////////////////////////////////////
