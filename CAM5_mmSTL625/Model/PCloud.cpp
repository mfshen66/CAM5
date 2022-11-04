/*	2003/11/6 nt
 *	implementation file of class CPCloud
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "MPoint.h"
#include "PCell.h"
#include "PCube.h"
#include "PCloud.h"

////////////////////////////////////////////////////////////////
//	point cloud
/*	2003/11/6 nt
 *	point cloud
 */
CPCloud::CPCloud()
{
	m_mtol = 0.1 ; //0.0001 ;
	m_mindis = 0.1 ; // 0.0001 ; // ???
	m_maxdis = 1. ; //0.001 ; // ???

	m_ns[0] = 0 ;
	m_ns[1] = 0 ;
	m_ns[2] = 0 ;
	m_w = 10. ; // 0.01 ; // ??? nt modify 2018/7/14
	m_ctol = MIN_DBL ;

	m_pcubes = NULL ;

	// nt add 2018/7/14
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;

	return ;
}

CPCloud::~CPCloud()
{
	int i, n ;

	for( n = m_pcells.GetSize(), i = 0 ; i < n ; i++ )
	{
		delete ((CPCell*)m_pcells[i]) ;
	}
	m_pcells.RemoveAll() ;

	if( m_pcubes )
		delete []m_pcubes ;
}
//--------------------------------------------------------------
// nt add 2018/7/14
int CPCloud::LoadPCD(CString& fileName)
{
	CPCell* pPCell = new CPCell() ;
	if( pPCell )
	{
		if( pPCell->LoadPCD(fileName) == 1 )
		{
			AddPCell(pPCell) ;
			return 1 ;
		}
		else
		{
			delete pPCell ;
			return 0 ;
		}
	}
	else
		return 0 ;
}

int	CPCloud::AddPCell(CPCell* pPCell)
{
	BOX3D oldBox = m_box ;

	m_pcells.Add(pPCell) ;
	
	CalBox() ;

	if( oldBox.min[0]-MIN_DBL < m_box.min[0] &&
		oldBox.max[0]+MIN_DBL > m_box.max[0] &&
		oldBox.min[1]-MIN_DBL < m_box.min[1] &&
		oldBox.max[1]+MIN_DBL > m_box.max[1] &&
		oldBox.min[2]-MIN_DBL < m_box.min[2] &&
		oldBox.max[2]+MIN_DBL > m_box.max[2] )
	{
		if( AddPCellToPCubes(pPCell) != 1 )
			return 0 ;
		return 1 ;
	}
	else
	{
		if( MakePCubes() != 1 )
			return 0 ;
		return 1 ;
	}
}

int	CPCloud::GetBox(BOX3D* box)
{
	*box = m_box ;

	return 1 ;
}

int	CPCloud::CalBox()
{
	int i, k, n ;
	BOX3D box ;

	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	for( n = m_pcells.GetSize(), i = 0 ; i < n ; i++ )
	{
		((CPCell*)m_pcells[i])->GetBox(&box) ;
		for( k = 0 ; k < 3 ; k++ )
		{
			if( m_box.min[k] > box.min[k] )
				m_box.min[k] = box.min[k] ;
			if( m_box.max[k] < box.max[k] )
				m_box.max[k] = box.max[k] ;
		}
	}

	return 1 ;
}

CPCube* CPCloud::GetPCube(int i, int j, int k)
{
	if( !m_pcubes )
		return NULL ;

	return &(m_pcubes[k*m_ns[0]*m_ns[1]+j*m_ns[0]+i]) ;
}

/*	2003/11/8 nt
 *	add one CPCell to CPCubes of CPCloud
 */
int CPCloud::AddPCellToPCubes(CPCell* pPCell)
{
	int i, k, m, indexs[3] ;
	CMPoint* pMPoint, *mpoints ;
	CPCube* pPCube ;

	if( !pPCell )
		return 0 ;

	m = pPCell->GetNumOfMPoint() ;
	mpoints = pPCell->GetMPointArray() ;
	for( i = 0 ; i < m ; i++ )
	{
		pMPoint = &(mpoints[i]) ;
		for( k = 0 ; k < 3 ; k++ )
			indexs[k] = (int)floor((pMPoint->m_p[k]-m_startp[k])/m_w) ;
		pPCube = GetPCube(indexs[0], indexs[1], indexs[2]) ;
		pMPoint->m_next = pPCube->m_mpoints ;
		pPCube->m_mpoints = pMPoint ;
	}

	return 1 ;
}

/*	2003/11/6 nt
 *	make all CPCubes
 */
int	CPCloud::MakePCubes()
{
	int i, k, n ;
	CPCell* pPCell ;

	if( m_pcubes )
		delete []m_pcubes ;
	m_pcubes = NULL ;

	memcpy(m_startp, m_box.min, sizeof(PNT3D)) ;
	for( k = 0 ; k < 3 ; k++ )
		m_ns[k] = (int)(ceil((m_box.max[k]-m_box.min[k]+m_mtol)/m_w)+0.5) ;
	m_pcubes = new CPCube[m_ns[0]*m_ns[1]*m_ns[2]] ;

	for( n = m_pcells.GetSize(), i = 0 ; i < n ; i++ )
	{
		pPCell = (CPCell*)(m_pcells[i]) ;
		if( AddPCellToPCubes(pPCell) != 1 )
			return 0 ;
	}
	
	return 1 ;
}

CMPoint* CPCloud::GetNearestPt(PNT3D p, double r)
{
	int i, j, k, indexs[3] ;
	double d, min = MAX_DBL ;
	CPCube* pPCube ;
	CMPoint* pMin = NULL, *pMPoint ;

	for( k = 0 ; k < 3 ; k++ )
		indexs[k] = (int)floor((p[k]-m_startp[k])/m_w) ;

	for( i = -1 ; i < 2 ; i++ )
	{
		if( indexs[0]+i < 0 || indexs[0]+i >= m_ns[0] )
			continue ;
		for( j = -1 ; j < 2 ; j++ )
		{
			if( indexs[1]+j < 0 || indexs[1]+j >= m_ns[1] )
				continue ;
			for( k = -1 ; k < 2 ; k++ )
			{
				if( indexs[2]+k < 0 || indexs[2]+k >= m_ns[2] )
					continue ;
				pPCube = GetPCube(indexs[0]+i, indexs[1]+j, indexs[2]+k) ;
				for( pMPoint = pPCube->m_mpoints ; pMPoint ; pMPoint = pMPoint->m_next )
				{
					d = mathDist(p, pMPoint->m_p) ;
					if( min > d && d < r )
					{
						min = d ;
						pMin = pMPoint ;
					}
				}
			}
		}
	}

	return pMin ;
}
//--------------------------------------------------------------
int	CPCloud::Pick(PINFO* pi)
{
	int i, j, k ;
	double d, z ;
	BOX3D box ;
	CPCube* pPCube ;
	CMPoint* pMPoint ;

	for( i = 0 ; i < m_ns[0] ; i++ )
	{
		for( j = 0 ; j < m_ns[1] ; j++ )
		{
			for( k = 0 ; k < m_ns[2] ; k++ )
			{
				box.min[0] = m_startp[0]+i*m_w ;
				box.min[1] = m_startp[1]+j*m_w ;
				box.min[2] = m_startp[2]+k*m_w ;
				box.max[0] = box.min[0]+m_w ;
				box.max[1] = box.min[1]+m_w ;
				box.max[2] = box.min[2]+m_w ;
				if( mathChkBoxLinInt(&box, pi->pos, pi->dir, pi->r) == IDINT )
				{
					pPCube = GetPCube(i, j, k) ;
					if( !pPCube )
						return 0 ; // error
					for( pMPoint = pPCube->m_mpoints ; pMPoint ; pMPoint = pMPoint->m_next )
					{
						if( pMPoint->m_attrib == -1 ) // nt add 2003/11/8
							continue ;
						d = mathDistPntLin(pMPoint->m_p, pi->pos, pi->dir) ;
						if( d < pi->r )
						{
							z = (pMPoint->m_p[0]-pi->pos[0])*pi->dir[0]+
								(pMPoint->m_p[1]-pi->pos[1])*pi->dir[1]+
								(pMPoint->m_p[2]-pi->pos[2])*pi->dir[2] ;
							if( pi->obj == NULL || pi->z > z )
							{
								pi->ot = MPOINT ;
								pi->obj = pMPoint ;
								pi->z = z ;
								pi->att = 2 ; // nt add 2011/7/6
							}
						}
					}
				}
			}
		}
	}

	if( pi->obj ) // picked
		return 1 ;
	else
		return 2 ;
}

// 1:picked;2:pick none;0:error
// 注意：pLB定义在pLF的XY平面上的包围盒
int	CPCloud::PickByBox(RFRAME* pLF, BOX2D* pLB, double r, COA& mps)
{
	int i, j, k ;
	PNT3D p ; // local p in pLF
	CPCube* pPCube ;
	CMPoint* pMPoint ;
	BOX3D box1, box2 ;

	for( i = 0 ; i < m_ns[0] ; i++ )
	{
		for( j = 0 ; j < m_ns[1] ; j++ )
		{
			for( k = 0 ; k < m_ns[2] ; k++ )
			{
				pPCube = GetPCube(i, j, k) ;
				if( !pPCube )
					return 0 ; // error
				box1.min[0] = m_startp[0]+i*m_w ;
				box1.min[1] = m_startp[1]+j*m_w ;
				box1.min[2] = m_startp[2]+k*m_w ;
				box1.max[0] = box1.min[0]+m_w ;
				box1.max[1] = box1.min[1]+m_w ;
				box1.max[2] = box1.min[2]+m_w ;
				mathTransWorldBox(pLF, &box1, &box2) ; // to local
				if( pLB->min[0]-r < box2.min[0] && // 完全包含情况
					box2.max[0] < pLB->max[0]+r &&
					pLB->min[1]-r < box2.min[1] &&
					box2.max[1] < pLB->max[1]+r )
				{
					for( pMPoint = pPCube->m_mpoints ; pMPoint ; pMPoint = pMPoint->m_next )
						mps.Add(pMPoint) ;
				}
				else
				{
					for( pMPoint = pPCube->m_mpoints ; pMPoint ; pMPoint = pMPoint->m_next )
					{
						if( pMPoint->m_attrib == -1 ) // nt add 2003/11/8
							continue ;
						mathTransWorldPnt3D(pLF, pMPoint->m_p, p) ; // to local
						if( pLB->min[0]-r < p[0] && // 完全包含情况
							p[0] < pLB->max[0]+r &&
							pLB->min[1]-r < p[1] &&
							p[1] < pLB->max[1]+r )
							mps.Add(pMPoint) ;
					}
				}
			}
		}
	}

	if( mps.GetSize() > 0 )
		return 1 ;
	else
		return 2 ;
}

// nt add 2010/4/22
int CPCloud::PickedAll(COA& mps)
{
	int i, k, n, m ;
	CPCell* pPCell ;

	for( n = m_pcells.GetSize(), i = 0 ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		if( pPCell )
		{
			for( m = pPCell->m_m*pPCell->m_n, k = 0 ; k < m ; k++ )
			{
				if( pPCell->m_mpoints[i].m_attrib != -1 )
					mps.Add(&(pPCell->m_mpoints[k])) ;
			}
		}
	}

	return 1 ;
}
//--------------------------------------------------------------
int	CPCloud::Draw(void* pVI, int drawMode)
{
	int i, n ;

	for( n = m_pcells.GetSize(), i = 0 ; i < n ; i++ )
	{
		((CPCell*)m_pcells[i])->Draw(pVI, drawMode) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int	CPCloud::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
	}
	else
	{
	}

	return 1 ;
}
//--------------------------------------------------------------
/*	2003/11/8 nt
 *	dxf out points
 */
// dxf out header
void dxfOutHeader(FILE* fp, double w, double h)
{
    // 头段标记
    fprintf(fp, "%3d\n",  0) ;
    fprintf(fp, "%s\n",  "SECTION") ;
    fprintf(fp, "%3d\n",  2) ;
    fprintf(fp, "%s\n",  "HEADER") ;

    // 图形的边界限制
    fprintf(fp, "%3d\n",  9) ;
    fprintf(fp, "%s\n",  "$LIMMIN") ;
    fprintf(fp, "%3d\n",  10) ;
    fprintf(fp, "%lf\n",  w) ;
    fprintf(fp, "%3d\n",  20) ;
    fprintf(fp, "%lf\n",  h) ;

    fprintf(fp, "%3d\n",  9) ;
    fprintf(fp, "%s\n",  "$LIMMAX") ;
    fprintf(fp, "%3d\n",  10) ;
    fprintf(fp, "%lf\n",  w) ;
    fprintf(fp, "%3d\n",  20) ;
    fprintf(fp, "%lf\n",  h) ;

    // 头段结束
    fprintf(fp, "%3d\n",  0) ;
    fprintf(fp, "%s\n",  "ENDSEC") ;

	return ;
}

// dxf begin out entities
void dxfOutBeginEntities(FILE* fp)
{
    // 实体表头
    fprintf(fp, "%3d\n", 0) ;
    fprintf(fp, "%s\n",  "SECTION") ;
    fprintf(fp, "%3d\n", 2) ;
    fprintf(fp, "%s\n",  "ENTITIES") ;

	return ;
}

// dxf end out entities
void dxfOutEndEntities(FILE* fp)
{
    // 实体段结束
    fprintf(fp, "%3d\n", 0) ;
    fprintf(fp, "%s\n",  "ENDSEC") ;

}
// dxf out end
void dxfOutEnd(FILE* fp)
{
    fprintf(fp, "%3d\n", 0) ;
    fprintf(fp, "%s\n",  "EOF") ;

	return ;
}

int CPCloud::DxfOut(FILE* fp)
{
	int i, n ;
	CPCell* pPCell ;

	dxfOutHeader(fp, 300., 200.0) ; // ??? w, h
	// dxf out entities
	dxfOutBeginEntities(fp) ;
	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->DxfOut(fp) ;
	}
	dxfOutEndEntities(fp) ;
	dxfOutEnd(fp) ;

	return 1 ;
}
//--------------------------------------------------------------
void igesOutHeader(FILE* fp)
{
	char s0[] = "CAM5 1.0" ;
	char s1[] = "1H,,1H;,1H3,18Hfile's Dirctionary,41Hthis is genrated by copyerCAM V1.0 G      1" ;
	char s2[] = "points,11HVersion 5.3,32,308,15,308,15,1H3,1.,2,2HMM,50,0.125,13H040329.G      2" ;
	char s3[] = "150706,1E-008,499990.,4Huser,,11,0,;                                    G      3" ;

	fprintf(fp, "%s", s0) ;
	fprintf(fp, "%65c%7d\n", 'S', 1) ;
	fprintf(fp, "%s\n", s1) ;
	fprintf(fp, "%s\n", s2) ;
	fprintf(fp, "%s\n", s3) ;

	return ;
}

void igesOutEnd(FILE*fp, int n)
{
	fprintf(fp, "%c%7d%c%7d%c%7d%c%7d", 'S', 1, 'G', 3, 'D', 2*n+2, 'P', n+1) ;
	fprintf(fp, "%41c%7d\n", 'T', 1) ;

	return ;
}

int CPCloud::IGESOut(FILE* fp)
{
	int i, n, Num ;
	CPCell* pPCell ;
	Num = 0 ;

	igesOutHeader(fp) ;
	// iges loop to output the cloud
	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->IGESOutIndex(fp) ;
	
	}
	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ ) 
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->IGESOutPara(fp) ;
		Num += ((((CPCell*)m_pcells[i] )->m_m)*(((CPCell*)m_pcells[i])->m_n)) ;
	}
	igesOutEnd(fp, Num) ;

	return 1 ;
}
//--------------------------------------------------------------
void vrmlOutHeader(FILE* fp)
{
	char s[] = "#VRML V2.0 utf8" ;
	fprintf(fp, "%s\n", s) ;

	return ;
}

void vrmlOutViewPort(FILE* fp)
{
	char s1[] = "DEF view1 Viewpoint{\n" ;
	char s2[] = "position 0 0 500 \n" ;
	char s3[] = "description \"view1\"\n}\n" ;
	fprintf(fp, "%s%s%s", s1, s2, s3) ;
	
	return ;
}

void printSign1(FILE* fp)
{
	fprintf(fp, "%c\n", '{') ;

	return ;
}

void printSign2(FILE* fp)
{
	fprintf(fp, "%c\n", '}') ;

	return ;
}

void printSign3(FILE* fp)
{
	fprintf(fp, "%c\n", '[') ;

	return ;
}

void printSign4(FILE* fp)
{
	fprintf(fp, "%c\n", ']') ;

	return ;
}

void printGroup(FILE* fp)
{
	char s[] = "Group" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printChildren(FILE* fp)
{
	char s[] = "children" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printShape(FILE* fp)
{
	char s[] = "Shape" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printTranslation(FILE* fp)
{
	char s[] = "translation 0 0 0" ;
	fprintf(fp, "%s\n", s) ;

	return ;
}

void printTransform(FILE* fp)
{
	char s[] = "DEF Pnts Transform" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printAppearance(FILE* fp)
{
	char s[] = "appearance Appearance" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printMaterial(FILE* fp)
{
	char s[] = "material Material" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printEmissiveColor(FILE* fp)
{
	char s[] = "emissiveColor 1 0 0 " ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printGeometry(FILE* fp)
{
	char s[] = "geometry " ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printPointSet(FILE* fp)
{
	char s[] = " PointSet" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printCoord(FILE* fp)
{
	char s[] = "coord Coordinate" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printPoint(FILE* fp)
{
	char s[] = "point" ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printColor1(FILE* fp)
{
	char s[] = "color " ;
	fprintf(fp, "%s", s) ;

	return ;
}

void printColor2(FILE* fp)
{
	char s[] = "Color " ;
	fprintf(fp, "%s", s) ;

	return ;
}

int CPCloud::VRMLOut(FILE* fp, CString& bitsName)
{
	int i, n ;
	CPCell* pPCell ;

	vrmlOutHeader(fp) ;
	vrmlOutViewPort(fp) ;
	printGroup(fp) ;
	printSign1(fp) ;
	printChildren(fp) ;
	printSign3(fp) ;
	printTransform(fp) ;
	printSign1(fp) ;
	printTranslation(fp) ;
	printChildren(fp) ;
	printSign3(fp) ;
	printShape(fp) ;
	printSign1(fp) ;
//	printAppearance(fp) ;
//	printSign1(fp) ;
//	printMaterial(fp) ;
//	printSign1(fp) ;
//	printEmissiveColor(fp) ;
//	printSign2(fp) ;
//	printSign2(fp) ;
	printGeometry(fp) ;
	printPointSet(fp) ;
	printSign1(fp) ;
	printCoord(fp) ;
	printSign1(fp) ;
	printPoint(fp) ;
	printSign3(fp) ;

	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->VRMLOut(fp) ;
	}
	printSign4(fp) ;
	printSign2(fp) ;
	printColor1(fp) ;
	printColor2(fp) ;
	printSign1(fp) ;
	printColor1(fp) ;
	printSign3(fp) ;
	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->VRMLColorOut(fp, bitsName) ;
	}
 	printSign4(fp) ;
	printSign2(fp) ;   
	printSign2(fp) ;
	printSign2(fp) ;
	printSign4(fp) ;
	printSign2(fp) ;
	printSign4(fp) ;
	printSign2(fp) ;

	return 1 ;
}
//--------------------------------------------------------------
int CPCloud::Out3DS(CString& fileName)
{
	int i, n ;

	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
	
		CPCell *pPCell = (CPCell*)m_pcells[i] ;
		pPCell->Out3DS(fileName) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int CPCloud::ObjOut(FILE* fp)
{
	int i, n ;
	CPCell* pPCell ;

	for( i = 0, n = m_pcells.GetSize() ; i < n ; i++ )
	{
		pPCell = (CPCell*)m_pcells[i] ;
		pPCell->ObjOut(fp) ;
	}

	return 1 ;
}
////////////////////////////////////////////////////////////////
