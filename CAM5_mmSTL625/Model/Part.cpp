/*	2001/11/16 nt
 *	implementation file of class CPart
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "Feature.h"
#include "RefPlane.h"
#include "RefPt.h"
#include "RefCur.h"
#include "TSM.h"
#include "RefSur.h"
#include "Pic.h"
#include "Triangle.h" // nt add 2010/11/19
#include "Mark.h" // nt add 2011/4/20
#include "MPoint.h"
#include "PCell.h"
#include "PCube.h"
#include "PCloud.h"
#include "Model.h"
#include "Part.h"
#include "..\API\IGES_API.h" // nt add 2010/5/8
#include "..\IGES\IgesImport.h" // nt add 2010/5/19
#include "..\IGES\IgesExport.h" // nt add 2012/10/13

////////////////////////////////////////////////////////////////
//	part
/*	2001/11/16 nt
 *	part
 */
CPart::CPart()
{
	for( int i = 0 ; i < MAX_NUM_FEATURE ; i++ )
		m_ftCounts[i] = 0 ;
	m_pPCloud = NULL ; // nt add 2003/11/6
	m_curNo = -1 ; // nt add 2010/9/26

	m_pGM = NULL ;

	// jh add 2022/8/27 临时
	// m_pGM = new GridModel();
	// strcpy_s(m_pGM->ModelName, 100, "d:\\2022-6Data625\\cone.stl");
	// strcpy_s(m_pGM->ModelName, 100, "d:\\2022-6Data625\\sphere.stl");
	// strcpy_s(m_pGM->ModelName, 100, "d:\\2022-6Data625\\Plies Group.1_STLMeshData.stl");
	// m_pGM->stlDealInputFile();


	// nt add 2022/6/15 临时
	m_pGM = new GridModel() ;
	strcpy_s(m_pGM->ModelName, 100, "C:\\Git\\CAM625\\2022-6Data625\\Plies Group.1_STLMeshData.stl") ;
	m_pGM->stlDealInputFile() ;
	strcpy_s(m_pGM->PreDataFileName, 256, "C:\\Git\\CAM625\\2022-6Data625\\Ply.2_OriginPath_202106121632.txt") ;
	//strcpy_s(m_pGM->PreDataFileName, 256, "C:\\Git\\CAM625\\2022-6Data625\\Ply.2_OriginPath_202106121632.txt") ;
	//strcpy_s(m_pGM->PreDataFileName, 256, "C:\\Git\\CAM625\\2022-6Data625\\Ply.1_OriginPath_20200729090350.txt") ;
	
	m_pGM->GetInformationFromPath() ;

	// nt add 2022/7/8 临时 测试很多情况返回-1.的问题
	/*m_pGM = new GridModel() ;
	strcpy_s(m_pGM->ModelName, 100, "d:\\2022-6Data625\\返回太多-1\\Plies Group.1_STLMeshData.stl") ;
	m_pGM->stlDealInputFile() ;
	strcpy_s(m_pGM->PreDataFileName, 256, "d:\\2022-6Data625\\返回太多-1\\Ply.1_OriginPath_20210707091014.txt") ;
	m_pGM->GetInformationFromPath() ;*/

	// nt add 2022/7/8 临时 测试比实际大10的问题
	/*m_pGM = new GridModel() ;
	strcpy_s(m_pGM->ModelName, 100, "d:\\2022-6Data625\\距离比实际大10左右\\Plies Group.1_STLMeshData.stl") ;
	m_pGM->stlDealInputFile() ;
	strcpy_s(m_pGM->PreDataFileName, 256, "d:\\2022-6Data625\\距离比实际大10左右\\Ply.1_OriginPath_20210707104357.txt") ;
	m_pGM->GetInformationFromPath() ;*/

	// 轨迹线上的节点转化为CRefPt
	if( m_pGM )
	{
		int i, j, n, m ;
		CRefPt* pRefPt ;
		CRefCur* pRefCur ;
		POList OL, *OLs ;
		n = m_pGM->TrailNumDum ;

		// 生成节点
		//if( n > 0 )
		//{
		//	for( i = 1 ; i <= m_pGM->TrailNumDum ; i++ )
		//	//for( i = 2 ; i <= 2 ; i++ )
		//	{
		//		OL = m_pGM->POLHead[i] ;
		//		for( j = 1, m = 0 ; j <= OL->DNum ; j++ )
		//			m += (OL->ENum[j]-OL->SNum[j]+1) ;
		//		for( j = 1 ; j <= m ; j++ )
		//		{
		//			// add jh 2022/8/18
		//			//if(i == 11 || i == 20)
		//			//	pRefPt = new CRefPt(0, 10, &OL->PTrail[j].x);
		//			//else
		//			//	pRefPt = new CRefPt(0, &OL->PTrail[j].x) ;
		//			pRefPt = new CRefPt(0, &OL->PTrail[j].x);
		//			m_features.Add(pRefPt);
		//		}
		//	}
		//}
		OL = m_pGM->POLHead[42];
		for (j = 1, m = 0; j <= OL->DNum; j++)
			m += (OL->ENum[j] - OL->SNum[j] + 1);
		for (j = 1; j <= m; j++)
		{
			pRefPt = new CRefPt(0, &OL->PTrail[j].x);
			m_features.Add(pRefPt);
		}

		// 简单等距
		/*if( n > 0 )
		{
			OLs = (POList*)malloc(sizeof(POList)*(n*2+1)) ;
			for( i = 1 ; i <= n ; i++ )
			{
				OL = m_pGM->POLHead[i] ;
				OLs[i] = OL ;
				OLs[i+n] = OL->DirectOffset(100.) ;

				if( i <= 2 )
				{
					for( j = 1, m = 0 ; j <= OL->DNum ; j++ )
						m += (OL->ENum[j]-OL->SNum[j]+1) ;
					for( j = 1 ; j <= m ; j++ )
					{
						pRefCur = new CRefCur(&OL->PTrail[j].x, &OLs[i+n]->PTrail[j].x) ;
						m_features.Add(pRefCur) ;
					}
				}
			}
			m_pGM->TrailNumDum += n ;
			// free POLHead
			//free(m_pGM->POLHead) ;
			m_pGM->POLHead = OLs ;
		}*/
		//if( n > 0 ) // 仅生成一条等距线
		//{
		//	OLs = (POList*)malloc(sizeof(POList)*(n+2)) ;
		//	for( i = 1 ; i <= n ; i++ )
		//	{
		//		OL = m_pGM->POLHead[i] ;
		//		OLs[i] = OL ;
		//	}

		//	OL = OLs[2] ; // 针对这条线等距!!!!!!!!!!!!!!!!!
		//	OLs[n+1] = OL->DirectOffset(100.) ;
		//	for( j = 1, m = 0 ; j <= OL->DNum ; j++ )
		//		m += (OL->ENum[j]-OL->SNum[j]+1) ;
		//	for( j = 1 ; j <= m ; j++ )
		//	{
		//		pRefCur = new CRefCur(&OL->PTrail[j].x, &OLs[n+1]->PTrail[j].x) ;
		//		m_features.Add(pRefCur) ;
		//	}
		//	m_pGM->TrailNumDum += 1 ;
		//	// free POLHead
		//	free(m_pGM->POLHead) ;
		//	m_pGM->POLHead = OLs ;
		//}
	}
}

CPart::~CPart()
{
	if( m_pPCloud ) // nt add 2003/11/6
		delete m_pPCloud ;

	for( int i = 0, n = m_features.GetSize() ; i < n ; i++ )
	{
		if( m_features[i] )
			delete m_features[i] ;
	}

	// nt add 2022/6/15 临时
	//if( m_pGM )...
}
//--------------------------------------------------------------
/*	2002/11/28 nt
 *	get initial feature name
 */
int CPart::GetInitFeatureName(int ft, CStr& ftName)
{
	switch(ft)
	{
	case ftRefPlane:
		ftName = "RefPlane" ;
		break ;
	case ftRefPt:
		ftName = "RefPt" ;
		break ;
	case ftRefCur:
		ftName = "RefCur" ;
		break ;
	case ftRefSur:
		ftName = "RefSur" ;
		break ;
	case ftPic:
		ftName = "Pic" ;
		break ;
	case ftTriangle: // nt add 2011/4/20
		ftName = "Triangle" ;
		break ;
	case ftMark: // nt add 2011/4/20
		ftName = "Mark" ;
		break ;
	default:
		return 0 ;
	}
	CStr tail ;
	tail.Format("%d", m_ftCounts[ft]) ;
	m_ftCounts[ft]++ ;
	ftName += tail ;

	return 1 ;
}

// nt add 2010/9/26
CFeature* CPart::GetFeatureByNo(int no)
{
	if( no >= 0 &&
		no <= m_curNo )
	{
		int i, n = GetNumOfFeature() ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)m_features[i] ;
			if( pF->GetNo() == no )
				return pF ;
		}
	}
	return NULL ;
}
//--------------------------------------------------------------
/*	2003/11/6 nt
 *	load points
 */
int CPart::LoadPts(CString& fileName)
{
	if( !m_pPCloud )
		m_pPCloud = new CPCloud() ;
	CPCell* pPCell = new CPCell() ;
	if( pPCell->Load(fileName) )
	{
		m_pPCloud->AddPCell(pPCell) ;
		return 1 ;
	}
	else
	{
		delete pPCell ;
		return 0 ;
	}
}

/*	2003/12/3 nt
 *	get pcloud
 */
CPCloud* CPart::GetPCloud()
{
	return m_pPCloud ;
}

/*	2001/11/28 nt
 *	create xy, yz and zx refplane
 */
// nt modify 2010/9/9
int CPart::AddRefPlaneOfXYZ()
{
	CRefPlane* pPlane ;
	CStr name ; // nt add 2010/9/9
	RFRAME lf ;
	
	lf.scale = 1.0 ;

	// xy plane
	lf.O[0] = lf.O[1] = lf.O[2] = 0.0 ;
	lf.X[0] = 1.0 ;
	lf.X[1] = 0.0 ;
	lf.X[2] = 0.0 ;
	lf.Y[0] = 0.0 ;
	lf.Y[1] = 1.0 ;
	lf.Y[2] = 0.0 ;
	lf.Z[0] = 0.0 ;
	lf.Z[1] = 0.0 ;
	lf.Z[2] = 1.0 ;
	pPlane = new CRefPlane(rpXY, &lf) ;
	name = "XY平面" ;
	pPlane->SetName(name) ;
	Add(pPlane) ;

	// yz plane
	lf.X[0] = 0.0 ;
	lf.X[1] = 1.0 ;
	lf.X[2] = 0.0 ;
	lf.Y[0] = 0.0 ;
	lf.Y[1] = 0.0 ;
	lf.Y[2] = 1.0 ;
	lf.Z[0] = 1.0 ;
	lf.Z[1] = 0.0 ;
	lf.Z[2] = 0.0 ;
	pPlane = new CRefPlane(rpYZ, &lf) ;
	name = "YZ平面" ;
	pPlane->SetName(name) ;
	Add(pPlane) ;

	// zx plane
	lf.X[0] = 0.0 ;
	lf.X[1] = 0.0 ;
	lf.X[2] = 1.0 ;
	lf.Y[0] = 1.0 ;
	lf.Y[1] = 0.0 ;
	lf.Y[2] = 0.0 ;
	lf.Z[0] = 0.0 ;
	lf.Z[1] = 1.0 ;
	lf.Z[2] = 0.0 ;
	pPlane = new CRefPlane(rpZX, &lf) ;
	name = "ZX平面" ;
	pPlane->SetName(name) ;
	Add(pPlane) ;

	return 1 ;
}

int CPart::GetNumOfFeature()
{
	return m_features.GetSize() ;
}

CFeature* CPart::GetFeature(int index)
{
	return (CFeature*)(m_features[index]) ;
}

// nt modify 2010/9/9
// nt modify 2010/9/26
int	CPart::Add(CFeature* pF)
{
	int k ;
	BOX3D box ;

	// 对于未命名的CFeature, 赋给一个缺省的名字, 2010/9/9
	CStr name ;
	pF->GetName(name) ;
	if( name == "" )
	{
		GetInitFeatureName(pF->GetType(), name) ;
		pF->SetName(name) ;
	}

	// nt add 2010/9/26
	m_curNo++ ;
	pF->SetNo(m_curNo) ;

	m_features.Add(pF) ;
	pF->GetBox(&box) ;
	for( k = 0 ; k < 3 ; k++ )
	{
		m_box.min[k] = mathMIN(box.min[k], m_box.min[k]) ;
		m_box.max[k] = mathMAX(box.max[k], m_box.max[k]) ;
	}

	return 1 ;
}

int	CPart::Delete(CFeature* pF)
{
	CFeature* pFeat ;

	for( int i = 0, n = m_features.GetSize() ; i < n ; i++ )
	{
		pFeat = (CFeature*)m_features[i] ;
		if( !pFeat )
			return 0 ;
		if( pFeat == pF )
		{
			m_features.RemoveAt(i) ;
			n-- ;
			i-- ;
		}
	}

	return 1 ;
}

int	CPart::Edit(CFeature* pF)
{
	if( !pF )
		return 0 ;

	return 1 ;
}

int	CPart::Disc(double tol, double ang)
{
	if( tol < 1.0e-5 ||
		ang < 1.0e-2 )
		return 0 ;

	return 1 ;
}

// nt modify 2009/5/9
int CPart::Navigate(DNAVI* dn)
{
	int i, n ;
	CFeature* pF ;

	if( !dn )
		return 0 ;

	dn->feat = NULL ;

	// for CRefCur and CRefPt
	n = m_features.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( pF )
		{
			if( pF->GetType() == ftRefPt ) // 使用参考点
				((CRefPt*)pF)->Navigate(dn) ;
			else
			if( pF->GetType() == ftRefCur ) // 使用参考曲线
				((CRefCur*)pF)->Navigate(dn) ;
		}
	}

	// for ...

	return 1 ;
}

// nt add 2010/3/26
int CPart::NavigateKeyPt(DNAVI* dn)
{
	int i, n ;
	CFeature* pF ;

	if( !dn )
		return 0 ;

	dn->feat = NULL ;

	// for CRefCur and CRefPt
	n = m_features.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( pF )
		{
			if( pF->GetType() == ftRefPt ) // 使用参考点
				((CRefPt*)pF)->Navigate(dn) ;
			else
			if( pF->GetType() == ftRefCur ) // 使用参考曲线
				((CRefCur*)pF)->NavigateKeyPt(dn) ;
		}
	}

	return 1 ;
}

int CPart::CalBox()
{
	int k ;
	BOX3D box ;
	CFeature* pFeat ;

	m_box.min[0] = m_box.min[1] = m_box.min[2] = MAX_DBL ;
	m_box.max[0] = m_box.max[1] = m_box.max[2] = -MAX_DBL ;
	for( int i = 0, n = m_features.GetSize() ; i < n ; i++ )
	{
		pFeat = (CFeature*)m_features[i] ;
		if( !pFeat )
			return 0 ;
		pFeat->GetBox(&box) ;
		for( k = 0 ; k < 3 ; k++ )
		{
			m_box.min[k] = mathMIN(box.min[k], m_box.min[k]) ;
			m_box.max[k] = mathMAX(box.max[k], m_box.max[k]) ;
		}
	}

	return 1 ;
}

// nt add 2009/5/27
int CPart::Copy(COA& fs, COA& copied_fs)
{
	int i, n = fs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CFeature* pF = (CFeature*)fs[i] ;
		if( pF )
		{
			CFeature* pCopied = pF->Copy() ;
			copied_fs.Add(pCopied) ;
			Add(pCopied) ;
		}
	}

	return 1 ;
}

// nt add 2009/5/27
int CPart::Move(COA& fs, VEC3D v)
{
	int i, n = fs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CFeature* pF = (CFeature*)fs[i] ;
		if( pF )
		{
			pF->Move(v) ;
		}
	}
	CalBox() ;

	return 1 ;
}

// nt add 2009/5/27
int CPart::Rotate(COA& fs, PNT3D pivot, VEC3D axis, double angle) 
{
	int i, n = fs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CFeature* pF = (CFeature*)fs[i] ;
		if( pF )
		{
			pF->Rotate(pivot, axis, angle) ;
		}
	}
	CalBox() ;

	return 1 ;
}

// nt add 2010/11/19
int CPart::Mirror(COA& fs, PNT3D pivot, VEC3D normal)
{
	int i, n = fs.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CFeature* pF = (CFeature*)fs[i] ;
		if( pF )
		{
			pF->Mirror(pivot, normal) ;
		}
	}
	CalBox() ;

	return 1 ;
}
//--------------------------------------------------------------
int intervalIsInPeX(INTERVAL* interval, 
					double periodic, 
					double t, 
					double tol,
					double* pt)
{
	int k1, k2 ;

	*pt = t ;
	if( periodic < MIN_DBL )
	{
		if( interval->t1-tol < t &&
			t < interval->t2+tol )
			return ID_YES ;
		else
			return ID_NO ;
	}
	else
	{
		k1 = (int)ceil((interval->t1-t-tol)/periodic-MIN_DBL) ;
		k2 = (int)floor((interval->t2-t+tol)/periodic+MIN_DBL) ;
		if( k1 <= k2 )
		{
			*pt = t+k1*periodic ;
			return ID_YES ;
		}
		else
			return ID_NO ;
	}
}

/*	2009/5/7 nt
 *	在p点处对CPart内的曲线进行裁剪
 *	代码来源于CAD2D中的CPart类
 *	nt modify 2010/10/20 see CAD2D Part->Trim()
 */
int CPart::Trim(PNT2D p, COA& trimmed, COA& created)
{
	return 1 ;
}
//--------------------------------------------------------------
// nt add 2008/11/14
// nt modify 2008/11/14
// nt modify 2010/10/22
// nt modify 2011/7/3
// nt modify 2011/7/13
// nt modify 2012/10/12 add flags
int CPart::Draw(void* pVI, int drawMode)
{
	glEnable(GL_DEPTH_TEST);

	// 绘制曲面类图元
	int i, n, type, flags ;
	CFeature* pF ;

	n = m_features.GetSize() ;
	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;
	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( pF )
		{
			type = pF->GetType() ;
			flags = pF->GetFlags() ;
			if( flags&FEATURE_VISIBLE )
			{
				if( type == ftRefSur ||
					type == ftRefPlane ||
					type == ftPic ||
					type == ftTriangle )
					pF->Draw(pVI, drawMode) ;
			}
		}
	}
	viLighting(pVI, 0) ;

	glDisable(GL_DEPTH_TEST);
	// nt add 2022/6/15 临时
	glEnable(GL_DEPTH_TEST);
	if( m_pGM )
	{
		viLighting(pVI, 1) ;
		viAddDefaultLight(pVI) ;
		m_pGM->Draw(pVI, drawMode) ;
		viLighting(pVI, 0) ;
	}
	glDisable(GL_DEPTH_TEST);
	// end

	glEnable(GL_DEPTH_TEST) ;
	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( pF )
		{
			type = pF->GetType() ;
			flags = pF->GetFlags() ;
			if( flags&FEATURE_VISIBLE )
			{
				if( type == ftRefCur ||
					type == ftRefPt ||
					type == ftMark )
					pF->Draw(pVI, drawMode) ;
				else
				if( type == ftRefSur ) // nt add 2021/6/25
					((CRefSur*)pF)->DrawBorder(pVI, drawMode) ;
			}
		}
	}
	glDisable(GL_DEPTH_TEST) ;

	// 绘制点云
	if( m_pPCloud ) // nt add 2003/11/6
	{
		glEnable(GL_DEPTH_TEST) ;
		m_pPCloud->Draw(pVI, drawMode) ;
		glDisable(GL_DEPTH_TEST) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt modify 2010/9/20 : add IsPicked()
int	CPart::Pick(PINFO* pi)
{
	int i, n = m_features.GetSize() ;
	CFeature* pF ;

	if( !pi )
		return 0 ;

	if( pi->type == MPOINT ) // nt add 2003/11/7 CMPoint
	{
		if( m_pPCloud )
			return m_pPCloud->Pick(pi) ;
		else
			return 2 ;
	}

	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( !pF )
			return 0 ;
		if( pF->GetType() == pi->type &&
			pF->IsPicked() == 0 )
		{
			pF->Pick(pi) ;
			/*if( (pi->ot == ftRefCur || // nt modify 2003/7/22 : old use pi->type
				pi->ot == ftRefPt) &&
				pi->obj )*/
			if( pi->att > 0 &&
				pi->obj )
				return 1 ;
		}
	}

	// nt add 2022/6/15
	if( m_pGM &&
		pi->type == STLTRI )
	{
		PNT3D q, gc ;
		FList f = m_pGM->Pick(pi->pos, pi->dir, pi->r, q, &pi->z, gc) ;
		if( f &&
			f->IsPicked() == 0 )
		{
			pi->obj = f ;
			pi->ot = STLTRI ;
			pi->u = gc[0] ;
			pi->v = gc[1] ;
		}
	}

	return 2 ; // nt modify 2003/7/22 : old return 1
}

// 1:picked;2:pick none;0:error
// 注意：pLB定义在pLF的XY平面上的包围盒
// nt modify 2010/10/17 : 不过滤CRefPlane
int	CPart::PickByBox(RFRAME* pLF, 
					 BOX2D* pLB, 
					 double r, 
					 COA& fs)
{
	int i, n = m_features.GetSize(), rt_value ;
	CFeature* pF ;
	BOX3D box1 ; // nt add 2010/4/16
	BOX3D box2 ; // nt add 2010/4/22

	for( i = 0 ; i < n ; i++ )
	{
		pF = (CFeature*)m_features[i] ;
		if( pF ) //&&
			//pF->GetType() != ftRefPlane ) // nt add 2010/4/9 过滤
		{
			pF->GetBox(&box1) ; // nt add 2010/4/16
			mathTransWorldBox(pLF, &box1, &box2) ;
			if( pLB->min[0]-r < box2.min[0] && // 完全包含情况
				box2.max[0] < pLB->max[0]+r &&
				pLB->min[1]-r < box2.min[1] &&
				box2.max[1] < pLB->max[1]+r )
				fs.Add(pF) ;
			else
			{
				rt_value = pF->PickByBox(pLF, pLB, r) ; // 实体本身
				if( rt_value == 0 ) // error
					return 0 ;
				if( rt_value == 1 ) // picked
					fs.Add(pF) ;
			}
		}
	}

	if( m_pPCloud )
	{
		m_pPCloud->GetBox(&box1) ; // nt add 2010/4/16
		mathTransWorldBox(pLF, &box1, &box2) ;
		if( pLB->min[0]-r < box2.min[0] && // 完全包含情况
			box2.max[0] < pLB->max[0]+r &&
			pLB->min[1]-r < box2.min[1] &&
			box2.max[1] < pLB->max[1]+r )
			m_pPCloud->PickedAll(fs) ;
		else
		{
			rt_value = m_pPCloud->PickByBox(pLF, pLB, r, fs) ; // 点
			if( rt_value == 0 ) // error
				return 0 ;
		}
	}

	if( fs.GetSize() > 0 )
		return 1 ;
	else
		return 2 ;
}
//--------------------------------------------------------------
// nt add 2010/10/17 : serialize some features
int CPart::SerializeFs(CArchive& ar, COA& fs)
{
	int i, n, type ;

	if( ar.IsStoring() )
	{
		n = fs.GetSize() ;
		ar << n ;
		for( i = 0 ; i < n ; i++ ) // for each feature in the fs
		{
			CFeature* pF = (CFeature*)fs[i] ;
			type = pF->GetType() ;
			ar << type ;
			pF->Serialize(ar) ;
		}
	}
	else
	{
		CRefPlane* pRefPlane ;
		CRefPt* pRefPt ;
		CRefCur* pRefCur ;
		CRefSur* pRefSur ;
		CPic* pPic ; // nt add 2004/9/13
		CTriangle* pTriangle ; // nt add 2011/4/20
		CMark* pMark ; // nt add 2011/4/20
		ar >> n ; // read number of features in the fs
		for( i = 0 ; i < n ; i++ ) // for each feature in the fs
		{
			ar >> type ;
			switch(type)
			{
			case ftRefPlane:
				pRefPlane = new CRefPlane() ; // new a feature of refplane
				if( pRefPlane->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pRefPlane) ;
				break ;
			case ftRefPt:
				pRefPt = new CRefPt() ; // new a feature of refpt
				if( pRefPt->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pRefPt) ;
				break ;
			case ftRefCur:
				pRefCur = new CRefCur() ; // new a feature of refcur
				if( pRefCur->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pRefCur) ;
				break ;
			case ftRefSur:
				pRefSur = new CRefSur() ; // new a feature of refsur
				if( pRefSur->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pRefSur) ;
				break ;
			case ftPic: // nt add 2004/9/13
				pPic = new CPic() ; // new a feature of CPic
				if( pPic->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pPic) ;
				break ;
			case ftTriangle: // nt add 2011/4/20
				pTriangle = new CTriangle() ;
				if( pTriangle->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pTriangle) ;
				break ;
			case ftMark: // nt add 2011/4/20
				pMark = new CMark() ;
				if( pMark->Serialize(ar) != 1 )
					return 0 ;
				fs.Add(pMark) ;
				break ;
			default:
				return 0 ;
			}
		}
	}

	return 1 ;
}

int CPart::Serialize(CArchive& ar)
{
	int i, n, type ;

	CModel::Serialize(ar) ;

	if( ar.IsStoring() )
	{
		ar.Write(m_ftCounts, sizeof(int)*MAX_NUM_FEATURE) ; // nt add 2010/9/9

		ar << m_curNo ; // nt add 2010/9/26

		n = GetNumOfFeature() ; // get number of features in the part model
		ar << n ;
		for( i = 0 ; i < n ; i++ ) // for each feature in the part model
		{
			CFeature* pFeat = GetFeature(i) ;
			type = pFeat->GetType() ;
			ar << type ;
			pFeat->Serialize(ar) ;
		}
	}
	else
	{
		if( m_state >= 1024 )
			ar.Read(m_ftCounts, sizeof(int)*MAX_NUM_FEATURE) ; // nt add 2010/9/9

		if( m_state >= 2048 ) // nt add 2010/9/26
			ar >> m_curNo ;

		CRefPlane* pRefPlane ;
		CRefPt* pRefPt ;
		CRefCur* pRefCur ;
		CRefSur* pRefSur ;
		CPic* pPic ; // nt add 2004/9/13
		CTriangle* pTriangle ; // nt add 2010/11/14
		CMark* pMark ; // nt add 2011/4/20
		ar >> n ; // read number of features in the part model
		for( i = 0 ; i < n ; i++ ) // for each feature in the part model
		{
			ar >> type ;
			switch(type)
			{
			case ftRefPlane:
				pRefPlane = new CRefPlane() ; // new a feature of refplane
				if( pRefPlane->Serialize(ar) != 1 )
					return 0 ;
				Add(pRefPlane) ;
				break ;
			case ftRefPt:
				pRefPt = new CRefPt() ; // new a feature of refpt
				if( pRefPt->Serialize(ar) != 1 )
					return 0 ;
				Add(pRefPt) ;
				break ;
			case ftRefCur:
				pRefCur = new CRefCur() ; // new a feature of refcur
				if( pRefCur->Serialize(ar) != 1 )
					return 0 ;
				Add(pRefCur) ;
				break ;
			case ftRefSur:
				pRefSur = new CRefSur() ; // new a feature of refsur
				if( pRefSur->Serialize(ar) != 1 )
					return 0 ;
				Add(pRefSur) ;
				break ;
			case ftPic: // nt add 2004/9/13
				pPic = new CPic() ; // new a feature of CPic
				if( pPic->Serialize(ar) != 1 )
					return 0 ;
				Add(pPic) ;
				break ;
			case ftTriangle: // nt add 2010/11/14
				pTriangle = new CTriangle() ;
				if( pTriangle->Serialize(ar) != 1 )
					return 0 ;
				Add(pTriangle) ;
				break ;
			case ftMark: // nt add 2011/4/20
				pMark = new CMark() ;
				if( pMark->Serialize(ar) != 1 )
					return 0 ;
				Add(pMark) ;
				break ;
			default:
				return 0 ;
			}
		}
	}

	return 1 ;
}

/*	2003/11/8 nt
 *	dxf out measurement point only
 */
int CPart::DxfOut(CString& fileName)
{
	FILE* fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("w+t")) ;
	if( !fp )
		return 0 ;

	if( m_pPCloud )
		m_pPCloud->DxfOut(fp) ;

    fclose(fp) ;

	return 1 ;
}

// nt rewrite 2012/10/13
int CPart::IGESOut(CString& fileName)
{
	CIgesExport* pExport = new CIgesExport(7) ;
	int i, n = m_features.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CFeature* pF = (CFeature*)m_features[i] ;
		if( pF )
		{
			pExport->AddFeature(pF) ;
		}
	}
	pExport->Export(fileName) ;
	delete pExport ;
	
	return 1 ;
}

int CPart::VRMLOut(CString& fileName, CString& bitsName)
{
	FILE *fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("w+t")) ;
	if( !fp )
		return 0 ;

	if( m_pPCloud )
		m_pPCloud->VRMLOut(fp, bitsName) ;

	fclose(fp) ;

	return 1 ;
}

int CPart::Out3DS(CString& fileName)
{
	if( m_pPCloud )
		m_pPCloud->Out3DS(fileName) ;

	return 1 ;
}

int CPart::ObjOut(CString& fileName)
{
	FILE* fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("w+t")) ;
	if( !fp )
		return 0 ;

	if( m_pPCloud )
		m_pPCloud->ObjOut(fp) ;

    fclose(fp) ;

	return 1 ;
}
//--------------------------------------------------------------
BSCUR* bscurLoadASCII2(FILE* fp, double s)
{
    BSCUR* curv ;
    int i, n, ord_u, cnt_u ;

    if( !fp )
		return NULL ;
    if( fscanf_s(fp, "%d %d", &ord_u, &cnt_u) != 2 ||
		ord_u < 2 ||
		ord_u > 20 ||
		cnt_u < 2 ||
		cnt_u > 1000000 )
		return NULL ;
	curv = bscurCreate(ord_u, cnt_u) ;
    if( !curv )
        return NULL ;

    for( n = ord_u+cnt_u, i = 0 ; i < n ; i++ )
	{
        if( fscanf_s(fp, "%lf", &curv->u_k[i]) != 1 )
			goto ExceExit ;
	}
    for( i = 0 ; i < curv->cnt_u ; i++ )
	{
        if( fscanf_s(fp, "%lf %lf %lf %lf",
                       &curv->u_p[i][0],
                       &curv->u_p[i][1],
                       &curv->u_p[i][2],
                       &curv->u_p[i][3]) != 4 )
			goto ExceExit ;
		//curv->u_p[i][0] *= curv->u_p[i][3]  ;
		//curv->u_p[i][1] *= curv->u_p[i][3] ;
		//curv->u_p[i][2] *= curv->u_p[i][3] ;
		curv->u_p[i][0] *= s ;
		curv->u_p[i][1] *= s ;
		curv->u_p[i][2] *= s ;
	}

    return curv ;
ExceExit:
	bscurFree(curv) ;

	return NULL ;
}

BSSUR* bssurLoadASCII2(FILE* fp, double s)
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
		cnt_u > 1000000 ||
		ord_v < 2 ||
		ord_v > 20 ||
		cnt_v < 2 ||
		cnt_v > 1000000 )
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
			surf->uv_p[i][j][0] *= s ;
			surf->uv_p[i][j][1] *= s ;
			surf->uv_p[i][j][2] *= s ;
		}
	}

    return surf ;
ExceExit:
	bssurFree(surf) ;

	return NULL ;
}

// 2009/4/25 nt
// 读入基本图元
// 包括：
//     (1)NURBS曲线
//     (1)NURBS曲面
typedef double (*PSTR3)[3] ;
typedef double (**PNET)[3] ;

PNET pnetCreate(int nu, int nv)
{
	PNET pnet = new PSTR3[nu] ;
	for( int i = 0 ; i < nu ; i++ )
		pnet[i] = new double[nv][3] ;

	return pnet ;
}

void pnetFree(int nu, PNET pnet)
{
	if( pnet )
	{
		for( int i = 0 ; i < nu ; i++ )
			delete []pnet[i] ;
		delete pnet ;
	}

	return ;
}

int CPart::LoadGE(CString& fileName)
{
	FILE* fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("r+t")) ;
	if( fp )
	{
		char buf[80] ;
		int i, j, n, m, ord_u = 4, ord_v = 4 ;
		double s = 1. ;
		fgets(buf, 80, fp) ;
		if( strstr(buf, "PANDACAD-GE") )
		{
			for( int dl = 0 ; dl < 1000000 ; dl++ )
			{
				if( fgets(buf, 80, fp) > 0 )
				{
					if( strstr(buf, "BSCUR") )
					{
						BSCUR* bscur = bscurLoadASCII2(fp, s) ;
						if( bscur )
						{
							CRefCur* pRefCur = new CRefCur(0, bscur) ;
							Add(pRefCur) ;
						}
						else
						{
							fclose(fp) ;
							return 0 ;
						}
					}
					else
					if( strstr(buf, "BSSUR") )
					{
						BSSUR* bssur = bssurLoadASCII2(fp, s) ;
						if( bssur )
						{
							CRefSur* pRefSur = new CRefSur(0, bssur) ;
							Add(pRefSur) ;
						}
						else
						{
							fclose(fp) ;
							return 0 ;
						}
					}
					else
					if( strstr(buf, "PSTR") ) // nt add 2014/9/20
					{
						fscanf_s(fp, "%d", &n) ;
						double (*ps)[3] = NULL ;
						if( n > 0 )
						{
							ps = new double[n][3] ;
							for( i = 0 ; i < n ; i++ )
							{
								if( fscanf_s(fp, "%lf %lf %lf",
									         &(ps[i][0]), 
											 &(ps[i][1]), 
											 &(ps[i][2])) != 3 )
								{
									delete []ps ;
									return 0 ;
								}
								ps[i][0] *= s ;
								ps[i][1] *= s ;
								ps[i][2] *= s ;
							}
							if( ord_u == 0 )
							{
								for( i = 0 ; i < n ; i++ )
								{
									CRefPt* pRefPt = new CRefPt(0, ps[i]) ;
									Add(pRefPt) ;
								}
							}
							else
							{
								//BSCUR* nurbs ;
								//bscurIntpPs(n, ps, ord_u, &nurbs) ;
								//CRefCur* pRefCur = new CRefCur(0, nurbs) ;
								//Add(pRefCur) ;
							}
							delete []ps ;
							ps = NULL ;
						}
					}
					else
					if( strstr(buf, "PNET") ) // nt add 2014/9/20
					{
						fscanf_s(fp, "%d %d", &n, &m) ;
						double (**net)[3] = NULL ;
						if( n > 0 && m > 0 )
						{
							net = pnetCreate(n, m) ;
							for( i = 0 ; i < n ; i++ )
							{
								for( j = 0 ; j < m ; j++ )
								{
									if( fscanf_s(fp, "%lf %lf %lf",
												 &(net[i][j][0]), 
												 &(net[i][j][1]), 
												 &(net[i][j][2])) != 3 )
									{
										pnetFree(n, net) ;
										return 0 ;
									}
									net[i][j][0] *= s ;
									net[i][j][1] *= s ;
									net[i][j][2] *= s ;
								}
							}
							if( ord_u == 0 )
							{
								for( i = 0 ; i < n ; i++ )
								{
									for( j = 0 ; j < m ; j++ )
									{
										CRefPt* pRefPt = new CRefPt(0, net[i][j]) ;
										Add(pRefPt) ;
									}
								}
							}
							else
							{
								//BSSUR* nurbs ;
								//bssurCrePntArray(n, m, net, &nurbs) ;
								//CRefSur* pRefSur = new CRefSur(0, nurbs) ;
								//Add(pRefSur) ;
							}
							pnetFree(n, net) ;
							net = NULL ;
						}
					}
					else
					if( strstr(buf, "SCALE") )
					{
						fscanf_s(fp, "%lf", &s) ;
					}
					else
					if( strstr(buf, "ORD_U") ) // nt add 2014/9/20
					{
						fscanf_s(fp, "%d", &ord_u) ;
					}
					else
					if( strstr(buf, "ORD_V") ) // nt add 2014/9/20
					{
						fscanf_s(fp, "%d", &ord_v) ;
					}
				}
				else
					break ;
			}
			fclose(fp) ;
			return 1 ;
		}
		else
		{
			fclose(fp) ;
			return 0 ;
		}
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// nt add 2010/5/19
// flags&1 = read points
// flags&2 = read curves
// flags&4 = read trim surfaces
int CPart::ImportIGES(CString& fileName, int flags)
{
	CIgesImport* pIgesImport = new CIgesImport(flags) ;
	int rt = pIgesImport->Import(fileName) ;
	if( rt == 1 )
	{
		int i, n ;

		n = pIgesImport->GetNumOfFeature() ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = pIgesImport->GetFeature(i) ;
			Add(pF) ;
		}
		pIgesImport->ClearFeatures() ;

		// nt add 2021/3/6 debug only
		/*n = pIgesImport->m_errBnds.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)pIgesImport->m_errBnds[i] ;
			Add(pF) ;
		}
		pIgesImport->m_errBnds.RemoveAll() ;*/
	}
	delete pIgesImport ;
	return rt ;
}

int CPart::ImportIGES3(CString& fileName, int flags)
{
	CIgesImport* pIgesImport = new CIgesImport(flags) ;
	int rt = pIgesImport->Import3(fileName) ;
	if( rt == 1 )
	{
		int i, n ;

		n = pIgesImport->GetNumOfFeature() ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = pIgesImport->GetFeature(i) ;
			Add(pF) ;
		}
		pIgesImport->ClearFeatures() ;

		// nt add 2021/3/6 debug only
		/*n = pIgesImport->m_errBnds.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			CFeature* pF = (CFeature*)pIgesImport->m_errBnds[i] ;
			Add(pF) ;
		}
		pIgesImport->m_errBnds.RemoveAll() ;*/
	}
	delete pIgesImport ;
	return rt ;
}
//--------------------------------------------------------------
// nt add 2010/10/21
// only read v... and f...
int CPart::ImportObj(CString& fileName)
{
	return 0 ;
}

// nt add 2015/5/30
int CPart::ExportPts(CString& fileName)
{
	FILE* fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("w+t")) ;
	if( fp )
	{
		int i, n = m_features.GetSize() ;
		CFeature* pF ;
		CRefPt* pRefPt ;
		PNT3D p ;
		for( i = 0 ; i < n ; i++ )
		{
			pF = (CFeature*)m_features[i] ;
			if( !pF )
				return 0 ;
			if( pF->GetType() == ftRefPt )
			{
				pRefPt = (CRefPt*)pF ;
				pRefPt->GetP(p) ;
				fprintf(fp, "%6.4lf %6.4lf %6.4lf\n", p[0], p[1], p[2]) ;
			}
		}
	}
	fclose(fp) ;

	return 1 ;
}

// nt add 2015/5/30
int CPart::ExportPts2(CString& fileName, COA& es)
{
	FILE* fp = NULL ;
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("w+t")) ;
	if( fp )
	{
		int i, n = es.GetSize() ;
		CFeature* pF ;
		CRefPt* pRefPt ;
		PNT3D p ;
		for( i = 0 ; i < n ; i++ )
		{
			pF = (CFeature*)es[i] ;
			if( !pF )
				return 0 ;
			if( pF->GetType() == ftRefPt )
			{
				pRefPt = (CRefPt*)pF ;
				pRefPt->GetP(p) ;
				fprintf(fp, "%6.4lf %6.4lf %6.4lf\n", 1.1*p[0], 1.09*p[1], 1.11*p[2]) ;
			}
		}
	}
	fclose(fp) ;

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2018/7/14
int CPart::LoadPCD(CString& fileName)
{
	if( m_pPCloud == NULL )
		m_pPCloud = new CPCloud() ;
	return m_pPCloud->LoadPCD(fileName) ;
}
////////////////////////////////////////////////////////////////
