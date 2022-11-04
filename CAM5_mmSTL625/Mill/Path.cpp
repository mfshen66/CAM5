/*	2010/3/30 nt
 *	implementation file of class CPath
 */
#include "StdAfx.h"
#include "Path.h"

///////////////////////////////////////////////////////////////
//	CPath
/*	2010/3/30 nt
 *	CPath
 */
CPath::CPath()
{
	m_name = "" ;
	m_opType = opTypeUndefined ;
	m_opMethod = opMethodUndefined ;
	m_box.min[0] = 0. ;
	m_box.min[1] = 0. ;
	m_box.min[2] = 0. ;
	m_box.max[0] = 0. ;
	m_box.max[1] = 0. ;
	m_box.max[2] = 0. ;
	m_no = 0 ;
	m_npool = 0 ;
	m_pools = NULL ;
	m_n = 0 ;
	m_m = 0 ;
	//m_color = 255 ; // nt modify 2010/11/29
}

CPath::~CPath()
{
	if( m_pools )
	{
		for( int i = 0 ; i < m_npool ; i++ )
		{
			if( m_pools[i] )
				delete [](m_pools[i]) ;
		}
		delete []m_pools ;
	}
}
//--------------------------------------------------------------
int CPath::Create(int npool)
{
	if( m_pools )
	{
		for( int i = 0 ; i < m_npool ; i++ )
		{
			if( m_pools[i] )
				delete [](m_pools[i]) ;
		}
		delete []m_pools ;
	}
	m_npool = 0 ;
	m_pools = NULL ;
	m_n = 0 ;
	m_m = 0 ;

	if( npool > 0 )
	{
		m_npool = npool ;
		m_pools = new PPOOL[npool] ;
		if( !m_pools )
			return 0 ; // error
		for( int i = 0 ; i < npool ; i++ )
			m_pools[i] = NULL ;
		m_pools[0] = new PNODE[PATH_NUMPNODE] ;
		if( !m_pools[0] )
			return 0 ;
		m_n = 1 ;
		m_m = 0 ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int CPath::GetName(CStr& name)
{
	name = m_name ;
	return 1 ;
}

int CPath::SetName(CStr& name)
{
	m_name = name ;
	return 1 ;
}

//int CPath::GetType()
//{
//	return 1000 ; // ???!!!
//}

int CPath::GetOpType()
{
	return m_opType ;
}

void CPath::SetOpType(int opType)
{
	m_opType = opType ;
}

int CPath::GetOpMethod()
{
	return m_opMethod ;
}

void CPath::SetOpMethod(int opMethod)
{
	m_opMethod = opMethod ;
}

int CPath::GetNo()
{
	return m_no ;
}

//int CPath::GetColor()
//{
//	return m_color ;
//}

//void CPath::SetColor(int color)
//{
//	m_color = color ;
//}

int CPath::CalBox()
{
	int i, j, k, m ;
	for( k = 0 ; k < 3 ; k++ )
	{
		m_box.min[k] = MAX_DBL ;
		m_box.max[k] = -MAX_DBL ;
	}
	for( i = 0 ; i < m_n ; i++ )
	{
		if( i < m_n-1 )
			m = PATH_NUMPNODE ;
		else
			m = m_m ;
		for( j = 0 ; j < m ; j++ )
		{
			for( k = 0 ; k < 3 ; k++ )
			{
				if( m_box.min[k] > m_pools[i][j].pos[k] )
					m_box.min[k] = m_pools[i][j].pos[k] ;
				else
				if( m_box.max[k] < m_pools[i][j].pos[k] )
					m_box.max[k] = m_pools[i][j].pos[k] ;
			}
		}
	}

	return 1 ;
}

int CPath::GetBox(BOX3D* box)
{
	if( box )
	{
		*box = m_box ;
		return 1 ;
	}
	else
		return 0 ;
}

int CPath::GetNumOfPNode()
{
	if( m_n > 0 )
		return (m_n-1)*PATH_NUMPNODE+m_m ;
	else
		return 0 ;
}

int CPath::GetPNode(int i, PNODE* pnode)
{
	int I, J ;
	I = i/PATH_NUMPNODE ;
	J = i%PATH_NUMPNODE ;
	if( I < m_n-1 )
	{
		*pnode = m_pools[I][J] ;
		return 1 ;
	}
	else
	if( I < m_n ) // I == m_n-1
	{
		if( J < m_m )
		{
			*pnode = m_pools[I][J] ;
			return 1 ;
		}
		else
			return 0 ;
	}
	else
		return 0 ;
}

int CPath::AddLinearMotion(LMOTION* lmotion)
{
	if( m_m >= PATH_NUMPNODE )
	{
		if( m_n >= m_npool )
			return 2 ; // error : full!!!
		m_pools[m_n] = new PNODE[PATH_NUMPNODE] ;
		if( !m_pools[m_n] )
			return 0 ; // error
		m_n++ ;
		m_m = 0 ;
	}
	m_no++ ;
	m_pools[m_n-1][m_m].no = m_no ;
	memcpy(m_pools[m_n-1][m_m].pos, lmotion->position, sizeof(PNT3D)) ;
	memcpy(m_pools[m_n-1][m_m].axis, lmotion->tool_axis, sizeof(VEC3D)) ;
	m_pools[m_n-1][m_m].fr = lmotion->feed_value ;
	m_pools[m_n-1][m_m].type = lmotion->type ;
	m_m++ ;
	return 1 ;
}

// nt add 2010/6/2
int CPath::EndAddMotion()
{
	CalBox() ;
	return 1 ;
}
//--------------------------------------------------------------
int CPath::Draw(void* pVI, int drawMode)
{
	int i, j, m ;

	if( !pVI )
		return 0 ;
	/*if( drawMode == dmNormal )
	{
		glColor3ub(GetRValue(m_color), // color
				   GetGValue(m_color),
			       GetBValue(m_color)) ;
	}
	else
	if( drawMode == dmPrompt )
		glColor3ub(255, 0, 0) ;
	else
	if( drawMode == dmSelect )
		glColor3ub(0, 255, 0) ;
	else
		glColor3ub(50, 50, 100) ;*/
	glLineWidth(1.f) ;
	glBegin(GL_LINE_STRIP) ;
		for( i = 0 ; i < m_n ; i++ )
		{
			if( i < m_n-1 )
				m = PATH_NUMPNODE ;
			else
				m = m_m ;
			for( j = 0 ; j < m ; j++ )
			{
				glVertex3dv(m_pools[i][j].pos) ;
			}
		}
	glEnd() ;
	return 1 ;
}

//int CPath::Draw2(void* pVI, int drawMode)
//{
//	Draw(pVI, drawMode) ;
//	return 1 ;
//}

int CPath::Pick(PINFO* pi)
{
	if( mathChkBoxLinInt(&m_box, 
						 pi->pos, 
						 pi->dir, 
						 pi->r) == IDNOINT )
		return 2 ;

	int m = GetNumOfPNode() ;
	if( m > 2 )
	{
		PNT3D b, e ;
		PNODE pnode ;
		GetPNode(0, &pnode) ;
		memcpy(b, pnode.pos, sizeof(PNT3D)) ;
		mathPrjPntPln(b, pi->pos, pi->dir, b) ;
		for( int i = 1 ; i < m ; i++ )
		{
			GetPNode(i, &pnode) ;
			memcpy(e, pnode.pos, sizeof(PNT3D)) ;
			mathPrjPntPln(e, pi->pos, pi->dir, e) ;
			if( mathChkPntSegm(pi->pos, b, e, pi->r) == IDIN )
			{
				pi->ot = 1000 ; // ???!!!
				pi->obj = this ;
				pi->z = 0. ;
				pi->u = 0. ;
				pi->v = 0. ;
				return 1 ;
			}
			memcpy(b, e, sizeof(PNT3D)) ;
		}
	}

	return 2 ;
}

// nt modify 2010/9/26 : add ver
// nt modify 2010/11/29
int CPath::Serialize(CArchive& ar)
{
	int i, m, ver = 0 ;

	if( ar.IsStoring() )
	{
		ar << ver
		   << m_name
		   << m_opType
		   << m_opMethod
		   << m_no
		   << m_npool
		   << m_n
		   << m_m ;
		   //<< m_color ;
		ar.Write(&m_box, sizeof(BOX3D)) ;
		for( i = 0 ; i < m_n ; i++ )
		{
			if( i < m_n-1 )
				m = PATH_NUMPNODE ;
			else
				m = m_m ;
			ar.Write(m_pools[i], sizeof(PNODE)*m) ;
		}
	}
	else
	{
		ar >> ver
		   >> m_name
		   >> m_opType
		   >> m_opMethod
		   >> m_no
		   >> m_npool
		   >> m_n
		   >> m_m ;
		   //>> m_color ;
		ar.Read(&m_box, sizeof(BOX3D)) ;
		if( m_npool > 0 )
		{
			m_pools = new PPOOL[m_npool] ;
			if( !m_pools )
				return 0 ; // error
			for( i = 0 ; i < m_npool ; i++ )
				m_pools[i] = NULL ;
			for( i = 0 ; i < m_n ; i++ )
			{
				m_pools[i] = new PNODE[PATH_NUMPNODE] ;
				if( !m_pools[i] )
					return 0 ;
				if( i < m_n-1 )
					m = PATH_NUMPNODE ;
				else
					m = m_m ;
				ar.Read(m_pools[i], sizeof(PNODE)*m) ;
			}
		}
		else
			m_pools = NULL ;
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2011/11/30
// save as a UG CL file
int CPath::ToCL(CString& filePath)
{
	int i, j, m ;
	FILE* fp = NULL ;

	_tfopen_s(&fp, filePath.GetBuffer(0), _T("w+t")) ;
	if( fp )
	{
		for( i = 0 ; i < m_n ; i++ )
		{
			if( i < m_n-1 )
				m = PATH_NUMPNODE ;
			else
				m = m_m ;
			for( j = 0 ; j < m ; j++ )
			{
				fprintf(fp, 
					    "GOTO/%5.4lf,%5.4lf,%5.4lf,%1.8lf,%1.8lf,%1.8lf",
						m_pools[i][j].pos[0],
						m_pools[i][j].pos[1],
						m_pools[i][j].pos[2],
						m_pools[i][j].axis[0],
						m_pools[i][j].axis[1],
						m_pools[i][j].axis[2]) ;
			}
		}
		fclose(fp) ;
		return 1 ;
	}
	else
		return 0 ;
}
///////////////////////////////////////////////////////////////