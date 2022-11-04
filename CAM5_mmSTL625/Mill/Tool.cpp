/*	2010/3/31 nt
 *	implementation file of class CTool
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"

///////////////////////////////////////////////////////////////
//	CTool
/*	2010/3/31 nt
 *	Tool
 */
CTool::CTool()
{
	m_type = toolTypeUD ;
	m_subType = toolSubTypeUD ;
	m_ver = 0 ;
	for( int i = 0 ; i < TOOL_MAXNUMPARAM ; i++ )
	{
		m_ds[i] = 0. ;
		m_rs[i] = 0. ;
		m_as[i] = 0. ;
	}
	m_rOffset = 0. ;
	m_roRegister = 0 ;
	m_zOffset = 0. ;
	m_zoRegister = 0 ;
	m_nBlade = 0 ;
	m_material = 0 ;
	m_name = "" ;
	m_no = 0 ;

	m_box.min[0] = 0. ;
	m_box.min[1] = 0. ;
	m_box.min[2] = 0. ;
	m_box.max[0] = 0. ;
	m_box.max[1] = 0. ;
	m_box.max[2] = 0. ;
	mathInitRFrame(&m_lf) ;
	m_dTol = 0.0001 ;
	m_dAng = 15*PI1/180 ;
	m_n = 0 ;
	for( int i = 0 ; i < TOOL_MAXNUMTRS ; i++ )
		m_trss[i] = NULL ;
	m_color = RGB(100, 100, 100) ;

	m_hItem = 0 ;
}

CTool::~CTool()
{
	for( int i = 0 ; i < m_n ; i++ )
	{
		if( m_trss[i] )
		{
			trsFree(m_trss[i]) ;
		}
	}
}
//--------------------------------------------------------------
//    z
//    |____
//    |   |
//    |   | h
//    |___|___x
//      r
int CTool::CreateFlat(double r, double h)
{
	PNT3D p ;
	VEC3D dir ;
	CURVE* line ;
	INTERVAL interval ;

	// paramters
	m_type = toolTypeMill ;
	m_subType = toolSubTypeFlat ;
	m_name = "平底刀" ;
	m_ds[0] = h ;
	m_rs[0] = r ;

	// make profile 1
	p[0] = 0. ;
	p[1] = 0. ;
	p[2] = 0. ;
	dir[0] = 1. ;
	dir[1] = 0. ;
	dir[2] = 0. ;
	interval.t1 = 0. ;
	interval.t2 = r ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 2
	p[0] = r ;
	p[1] = 0. ;
	p[2] = 0. ;
	dir[0] = 0 ;
	dir[1] = 0. ;
	dir[2] = 1. ;
	interval.t1 = 0. ;
	interval.t2 = h ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 3
	p[0] = r ;
	p[1] = 0. ;
	p[2] = h ;
	dir[0] = -1. ;
	dir[1] = 0. ;
	dir[2] = 0. ;
	interval.t1 = 0. ;
	interval.t2 = r ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// initialize
	if( CalBox() != 1 ||
		Disc() != 1 )
		return 0 ;
	else
		return 1 ;
}

int CTool::CreateBall(double r, double h)
{
	PNT3D p ;
	VEC3D dir ;
	RFRAME lf ;
	CURVE* line, *circle ;
	INTERVAL interval ;

	if( h < r+1.e-3 )
		return 0 ;

	// parameters
	m_type = toolTypeMill ;
	m_subType = toolSubTypeBall ;
	m_name = "球头刀" ;
	m_ds[0] = h ;
	m_rs[0] = r ;

	// make profile 1
	lf.O[0] = 0. ;
	lf.O[1] = 0. ;
	lf.O[2] = r ; // old used 0., 2010/6/28
	lf.X[0] = 0. ;
	lf.X[1] = 0. ;
	lf.X[2] = -1. ;
	lf.Y[0] = 1. ;
	lf.Y[1] = 0. ;
	lf.Y[2] = 0. ;
	lf.Z[0] = 0. ;
	lf.Z[1] = -1. ;
	lf.Z[2] = 0. ;
	lf.scale = 1. ;
	interval.t1 = 0. ;
	interval.t2 = PI1/2 ;
	circle = circleCreate(&lf, r) ;
	AddProfile(circle, &interval) ;
	entityFree(circle) ;

	// make profile 2
	p[0] = r ;
	p[1] = 0. ;
	p[2] = r ;
	dir[0] = 0 ;
	dir[1] = 0. ;
	dir[2] = 1. ;
	interval.t1 = 0. ;
	interval.t2 = h-r ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 3
	p[0] = r ;
	p[1] = 0. ;
	p[2] = h ;
	dir[0] = -1. ;
	dir[1] = 0. ;
	dir[2] = 0. ;
	interval.t1 = 0. ;
	interval.t2 = r ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// initialize
	if( CalBox() != 1 ||
		Disc() != 1 )
		return 0 ;
	else
		return 1 ;
}

// nt add 2010/6/28
//           |---|---------
//           | r3|
//           |   |         h2
//           /   \---------
//			/     \
//         /   r1  \       h1
//         \_/---\_/ ______
//                r2
// 将刀位点向下移动r2, 2010/7/2
int CTool::CreateTorus(double r1,
					   double r2,
					   double r3,
					   double h1,
					   double h2)
{
	double d ;
	PNT3D p, q ;
	VEC3D dir ;
	RFRAME lf ;
	CURVE* line, *circle ;
	INTERVAL interval ;

	if( r1 < r2+1.e-3 )
		return 0 ;

	// parameters
	m_type = toolTypeMill ;
	m_subType = toolSubTypeTorus ;
	m_name = "环形刀" ;
	m_ds[0] = h1 ;
	m_ds[1] = h2 ;
	m_rs[0] = r1 ;
	m_rs[1] = r2 ;
	m_rs[2] = r3 ;

	// make profile 1
	p[0] = 0. ;
	p[1] = 0. ;
	p[2] = r2 ;
	dir[0] = 1. ;
	dir[1] = 0. ;
	dir[2] = 0. ;
	interval.t1 = 0. ;
	interval.t2 = r2-r1 ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 2
	lf.O[0] = r1 ;
	lf.O[1] = 0. ;
	lf.O[2] = r2 ;
	lf.X[0] = -1. ;
	lf.X[1] = 0. ;
	lf.X[2] = 0. ;
	lf.Y[0] = 0. ;
	lf.Y[1] = 0. ;
	lf.Y[2] = -1. ;
	lf.Z[0] = 0. ;
	lf.Z[1] = -1. ;
	lf.Z[2] = 0. ;
	lf.scale = 1. ;
	interval.t1 = 0. ;
	interval.t2 = PI1 ;
	circle = circleCreate(&lf, r2) ;
	AddProfile(circle, &interval) ;
	entityFree(circle) ;

	// make profile 3
	p[0] = r1+r2 ;
	p[1] = 0. ;
	p[2] = r2 ;
	q[0] = r3 ;
	q[1] = 0. ;
	q[2] = h1 ;
	mathGetVec(p, q, dir) ;
	d = mathVecLen(dir) ;
	if( d < MIN_LEN )
		return 0 ;
	dir[0] /= d ;
	dir[1] /= d ;
	dir[2] /= d ;
	interval.t1 = 0. ;
	interval.t2 = d ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 4
	p[0] = r3 ;
	p[1] = 0. ;
	p[2] = h1 ;
	dir[0] = 0. ;
	dir[1] = 0. ;
	dir[2] = 1. ;
	interval.t1 = 0. ;
	interval.t2 = h2 ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// make profile 5
	p[0] = r3 ;
	p[1] = 0. ;
	p[2] = h1+h2 ;
	dir[0] = -1. ;
	dir[1] = 0. ;
	dir[2] = 0. ;
	interval.t1 = 0. ;
	interval.t2 = r3 ;
	line = lineCreate(p, dir) ;
	AddProfile(line, &interval) ;
	entityFree(line) ;

	// initialize
	if( CalBox() != 1 ||
		Disc() != 1 )
		return 0 ;
	else
		return 1 ;
}

// nt add 2010/9/22
int CTool::Recreate()
{
	// nt add 2010/9/24
	for( int i = 0 ; i < m_n ; i++ )
	{
		if( m_trss[i] )
		{
			trsFree(m_trss[i]) ;
		}
	}
	m_n = 0 ;

	if( m_type == toolTypeMill )
	{
		if( m_subType == toolSubTypeFlat )
			return CreateFlat(m_rs[0], m_ds[0]) ;
		else
		if( m_subType == toolSubTypeBall )
			return CreateBall(m_rs[0], m_ds[0]) ;
		else
		if( m_subType == toolSubTypeTorus )
			return CreateTorus(m_rs[0], 
			                   m_rs[1], 
							   m_rs[2], 
							   m_ds[0], 
							   m_ds[1]) ;
		else
			return 0 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/22
CTool* CTool::Copy()
{
	CTool* pCopy = new CTool() ;
	pCopy->m_type = m_type ;
	pCopy->m_subType = m_subType ;
	pCopy->m_ver = m_ver ;
	pCopy->m_rOffset = m_rOffset ;
	pCopy->m_roRegister = m_roRegister ;
	pCopy->m_zOffset = m_zOffset ;
	pCopy->m_zoRegister = m_zoRegister ;
	pCopy->m_nBlade = m_nBlade ;
	pCopy->m_material = m_material ;
	pCopy->m_name = m_name ;
	pCopy->m_no = m_no ;
	pCopy->m_dTol = m_dTol ;
	pCopy->m_dAng = m_dAng ;
	pCopy->m_color = m_color ;
	pCopy->m_n = 0 ;
	for( int i = 0 ; i < TOOL_MAXNUMPARAM ; i++ )
	{
		pCopy->m_ds[i] = m_ds[i] ;
		pCopy->m_rs[i] = m_rs[i] ;
		pCopy->m_as[i] = m_as[i] ;
	}

	// recreate
	if( pCopy->Recreate() != 1 )
	{
		delete pCopy ;
		return NULL ;
	}
	else
		return pCopy ;
}
//--------------------------------------------------------------
int CTool::GetType()
{
	return m_type ;
}

int CTool::GetSubType()
{
	return m_subType ;
}

int CTool::GetNumOfTRS()
{
	return m_n ;
}

TRS* CTool::GetTRS(int index)
{
	if( index >= 0 &&
		index < m_n )
		return m_trss[index] ;
	else
		return NULL ;
}

int CTool::AddProfile(CURVE* curve, INTERVAL* interval)
{
	if( m_n < TOOL_MAXNUMTRS )
	{
		m_trss[m_n] = trsCreate(curve, interval) ;
		m_n++ ;
		return 1 ;
	}
	else
		return 0 ;
}

double CTool::GetDTol()
{
	return m_dTol ;
}

int CTool::SetDTol(double dTol)
{
	m_dTol = dTol ;
	return 1 ;
}

double CTool::GetDAng()
{
	return m_dAng ;
}

int CTool::SetDAng(double dAng)
{
	m_dAng = dAng ;
	return 1 ;
}

int CTool::GetLF(RFRAME* pLF)
{
	*pLF = m_lf ;
	return 1 ;
}

int CTool::SetLF(RFRAME* pLF)
{
	if( pLF )
	{
		m_lf = *pLF ;
		return 1 ;
	}
	else
		return 0 ;
}

int CTool::GetName(CStr& name)
{
	name = m_name ;
	return 1 ;
}

int CTool::SetName(CStr& name)
{
	m_name = name ;
	return 1 ;
}

int CTool::GetName(CString& name)
{
	name = m_name;
	return 1;
}

int CTool::SetName(CString& name)
{
	m_name = name;
	return 1;
}

int CTool::GetNo()
{
	return m_no ;
}


HTREEITEM CTool::GetItem()
{
	return m_hItem ;
}

void CTool::SetItem(HTREEITEM hItem)
{
	m_hItem = hItem ;
}

int CTool::SetNo(int no)
{
	m_no = no ;
	return 1 ;
}

int CTool::GetColor()
{
	return m_color ;
}

int CTool::SetColor(int color)
{
	m_color = color ;
	return 1 ;
}

int CTool::CalBox()
{
	if( m_n > 0 )
	{
		int k ;
		BOX3D box ;
		trsCalBox(m_trss[0]) ;
		trsGetBox(m_trss[0], &m_box) ;
		for( int i = 1 ; i < m_n ; i++ )
		{
			trsCalBox(m_trss[i]) ;
			trsGetBox(m_trss[i], &box) ;
			for( k = 0 ; k < 3 ; k++ )
			{
				if( m_box.min[k] > box.min[k] )
					m_box.min[k] = box.min[k] ;
				if( m_box.max[k] < box.max[k] )
					m_box.max[k] = box.max[k] ;
			}
		}
		mathTransLocalBox(&m_lf, &m_box, &m_box) ;
		return 1 ;
	}
	else
		return 0 ;
}

int CTool::GetBox(BOX3D* box)
{
	if( box )
	{
		*box = m_box ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// 已经先计算了包围盒
int CTool::Disc()
{
	int i, n ;
	double r = 0., d, a ;
	BOX3D box ;
	for( i = 0 ; i < m_n ; i++ )
	{
		trsGetBox(m_trss[i], &box) ;
		if( r < fabs(box.min[0]) )
			r = fabs(box.min[0]) ;
		if( r < fabs(box.max[0]) )
			r = fabs(box.max[0]) ;
	}
	if( r < MIN_RAD )
		return 0 ;
	d = (r-m_dTol)/r ;
	if( d < MIN_DBL ) // 防止d太小
		d = MIN_DBL ;
	a = 2*acos(d) ;
	if( a < MIN_DBL )
		a = MIN_DBL ;
	n = (int)ceil(PI2/a) ;
	if( n > 512 ) // ???
		n = 512 ;
	if( n < 6 )
		n = 6 ;

	for( i = 0 ; i < m_n ; i++ )
	{
		if( trsDisc(m_trss[i], m_dTol, m_dAng, n) != ERSUCSS )
			return 0 ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int CTool::Draw(void* pVI, int drawMode)
{
	int k ;
	double matrix[16] ;
	for( k = 0 ; k < 3 ; k++ )
	{
		matrix[k]    = m_lf.X[k] ;
		matrix[k+4]  = m_lf.Y[k] ;
		matrix[k+8]  = m_lf.Z[k] ;
		matrix[k+12] = m_lf.O[k] ;
		matrix[3+4*k]  = 0. ;
	}
	matrix[15] = 1. ;

	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;
	MATERIAL material ;
	materialGet2(&material, 4) ;
	materialSet(&material) ;
	glPushMatrix() ;
	glMultMatrixd(matrix) ;
		for( k = 0 ; k < m_n ; k++ )
		{
			trsDraw(m_trss[k], pVI, drawMode, 0) ;
		}
	glPopMatrix() ;
	viLighting(pVI, 0) ;

	return 1 ;
}

int CTool::Pick(PINFO* pi)
{
	if( pi )
	{
		PINFO pi2 ;
		pi2 = *pi ;
		mathTransWorldPnt3D(&m_lf, pi->pos, pi2.pos) ;
		mathTransWorldVec3D(&m_lf, pi->dir, pi2.dir) ;
		for( int i = 0 ; i < m_n ; i++ )
		{
			if( trsPick(m_trss[i], &pi2) == 1 )
			{
				pi->ot = pi2.ot ;
				pi->obj = pi2.obj ;
				pi->z = pi2.z ;
				pi->u = pi2.u ;
				pi->v = pi2.v ;
				return 1 ;
			}
		}
		return 2 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// nt add 2010/9/22
int CTool::Save(FILE* fp)
{
	if( fp )
	{
		fprintf_s(fp, "@%s\n", m_name.GetBuffer(0)) ;
		fprintf_s(fp, "%d ", m_no) ;
		fprintf_s(fp, "%d ", m_type) ;
		fprintf_s(fp, "%d ", m_subType) ;
		fprintf_s(fp, "%d ", m_ver) ;
		fprintf_s(fp, "%lf ", m_rOffset) ;
		fprintf_s(fp, "%d ", m_roRegister) ;
		fprintf_s(fp, "%lf ", m_zOffset) ;
		fprintf_s(fp, "%d ", m_zoRegister) ;
		fprintf_s(fp, "%d ", m_nBlade) ;
		fprintf_s(fp, "%d ", m_material) ;
		fprintf_s(fp, "%lf ", m_dTol) ;
		fprintf_s(fp, "%lf ", m_dAng) ;
		fprintf_s(fp, "%d\n", m_color) ;
		for( int i = 0 ; i < TOOL_MAXNUMPARAM ; i++ )
		{
			fprintf_s(fp, 
				      "%lf %lf %lf\n", 
				      m_ds[i], 
				      m_rs[i],
				      m_as[i]) ;
		}

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/22
int CTool::Load(FILE* fp)
{
	if( fp )
	{
		while(1)
		{
			char c = fgetc(fp) ;
			if( c == '@' ||
				c == EOF )
				break ;
		}
		char buf[64] ;
		fgets(buf, 64, fp) ;
		m_name = buf ;
		fscanf_s(fp, "%d ", &m_no) ;
		fscanf_s(fp, "%d ", &m_type) ;
		fscanf_s(fp, "%d ", &m_subType) ;
		fscanf_s(fp, "%d ", &m_ver) ;
		fscanf_s(fp, "%lf ", &m_rOffset) ;
		fscanf_s(fp, "%d ", &m_roRegister) ;
		fscanf_s(fp, "%lf ", &m_zOffset) ;
		fscanf_s(fp, "%d ", &m_zoRegister) ;
		fscanf_s(fp, "%d ", &m_nBlade) ;
		fscanf_s(fp, "%d ", &m_material) ;
		fscanf_s(fp, "%lf ", &m_dTol) ;
		fscanf_s(fp, "%lf ", &m_dAng) ;
		fscanf_s(fp, "%d\n", &m_color) ;
		for( int i = 0 ; i < TOOL_MAXNUMPARAM ; i++ )
		{
			fscanf_s(fp, 
				     "%lf %lf %lf\n", 
				     &(m_ds[i]), 
				     &(m_rs[i]),
				     &(m_as[i])) ;
		}

		if( Recreate() != 1 )
			return 0 ;

		return 1 ;
	}
	else
		return 0 ;
}

// nt modify 2010/9/22
int CTool::Serialize(CArchive& ar)
{
	int i ;

	if( ar.IsStoring() )
	{
		ar << m_type
		   << m_subType
		   << m_ver
		   << m_rOffset
		   << m_roRegister
		   << m_zOffset
		   << m_zoRegister
		   << m_nBlade
		   << m_material
		   << m_name
		   << m_no
		   << m_dTol
		   << m_dAng
		   << m_color
		   << m_n ;
		ar.Write(m_ds, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Write(m_rs, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Write(m_as, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Write(&m_box, sizeof(BOX3D)) ;
		ar.Write(&m_lf, sizeof(RFRAME)) ;
		for( i = 0 ; i < m_n ; i++ )
		{
			trsSerialize(m_trss[i], ar) ;
		}
	}
	else
	{
		ar >> m_type
		   >> m_subType
		   >> m_ver
		   >> m_rOffset
		   >> m_roRegister
		   >> m_zOffset
		   >> m_zoRegister
		   >> m_nBlade
		   >> m_material
		   >> m_name
		   >> m_no
		   >> m_dTol
		   >> m_dAng
		   >> m_color
		   >> m_n ;
		ar.Read(m_ds, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Read(m_rs, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Read(m_as, sizeof(double)*TOOL_MAXNUMPARAM) ;
		ar.Read(&m_box, sizeof(BOX3D)) ;
		ar.Read(&m_lf, sizeof(RFRAME)) ;
		for( i = 0 ; i < m_n ; i++ )
		{
			m_trss[i] = trsCreate(NULL, NULL) ;
			trsSerialize(m_trss[i], ar) ;
			trsDisc(m_trss[i], 
					m_dTol, m_dAng,
					m_trss[i]->n) ;
		}
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////