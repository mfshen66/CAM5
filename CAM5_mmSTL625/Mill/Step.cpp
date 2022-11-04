/*	2010/9/25 nt
 *	implementation file of class CStep
 */
#include "StdAfx.h"
#include "CP.h"
#include "TRS.h"
#include "Tool.h"
#include "Path.h"
#include "Step.h"

///////////////////////////////////////////////////////////////
//	CStep
/*	2010/9/25 nt
 *	Step
 */
CStep::CStep()
{
	m_name = "工步" ;
	m_no = 0 ;
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	mathInitRFrame(&m_lf) ;
	m_pTool = NULL ;
	m_pPath = NULL ;
	m_workpiece.SetSize(512, 512) ;
	m_part.SetSize(512, 512) ;
	m_psurfaces.SetSize(512, 512) ;
	m_hItem = 0 ; // nt add 2010/11/29
	m_flags = 0 ; // nt add 2010/11/29
}

CStep::~CStep()
{
	m_workpiece.RemoveAll() ;
	m_part.RemoveAll() ;
	m_psurfaces.RemoveAll() ;
	if( m_pPath )
		delete m_pPath ;
	if( m_pTool )
		delete m_pTool ;
}
//--------------------------------------------------------------
/// nt add 2010/11/29
int CStep::GetType()
{
	return camStep ;
}

// nt add 2010/9/26
CStep* CStep::Copy()
{
	CStep* pCopy = new CStep() ;
	pCopy->SetName(m_name) ;
	pCopy->SetNo(m_no) ;
	pCopy->SetMCS(&m_lf) ;
	pCopy->SetBox(&m_box) ;
	int i, n = m_workpiece.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		pCopy->AddGeometry(gmtWorkpiece, m_workpiece[i]) ;
	}
	n = m_part.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		pCopy->AddGeometry(gmtPart, m_part[i]) ;
	}
	n = m_psurfaces.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		pCopy->AddGeometry(gmtPSurface, m_psurfaces[i]) ;
	}
	if( m_pTool )
	{
		CTool* pTool = m_pTool->Copy() ;
		pCopy->SetTool(pTool) ;
	}
	// CPath copy!!!
	// ...
	pCopy->SetCP(&m_cp) ;
	// nt add 2010/11/29
	pCopy->SetColor(m_color) ;

	return pCopy ;
}

void CStep::GetName(CStr& name)
{
	name = m_name ;
}

void CStep::SetName(CStr& name)
{
	m_name = name ;
}

void CStep::GetName(CString& name)
{
	name = m_name;
}

void CStep::SetName(CString& name)
{
	m_name = name;
}

// nt add 2010/9/26
int CStep::GetNumOfGeometry(int gmt)
{
	if( gmt = gmtWorkpiece )
		return m_workpiece.GetSize() ;
	else
	if( gmt == gmtPart )
		return m_part.GetSize() ;
	else
		return m_psurfaces.GetSize() ;
}

// nt add 2010/9/26
int CStep::GetGeometryNo(int gmt, int index)
{
	if( gmt = gmtWorkpiece )
		return m_workpiece[index] ;
	else
	if( gmt == gmtPart )
		return m_part[index] ;
	else
		return m_psurfaces[index] ;
}

// nt add 2010/9/26
int CStep::AddGeometry(int gmt, int geometryNo)
{
	if( gmt = gmtWorkpiece )
	{
		m_workpiece.Add(geometryNo) ;
	}
	else
	if( gmt == gmtPart )
	{
		m_part.Add(geometryNo) ;
	}
	else
	{
		m_psurfaces.Add(geometryNo) ;
	}
	return 1 ;
}

// nt add 2010/9/26
int CStep::DeleteGeometry(int gmt, int geometryNo)
{
	int i, n ;
	if( gmt = gmtWorkpiece )
	{
		n = m_workpiece.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			if( m_workpiece[i] == geometryNo )
			{
				m_workpiece.RemoveAt(i) ;
				return 1 ;
			}
		}
		return 0 ;
	}
	else
	if( gmt == gmtPart )
	{
		n = m_part.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			if( m_part[i] == geometryNo )
			{
				m_part.RemoveAt(i) ;
				return 1 ;
			}
		}
		return 0 ;
	}
	else
	{
		n = m_psurfaces.GetSize() ;
		for( i = 0 ; i < n ; i++ )
		{
			if( m_psurfaces[i] == geometryNo )
			{
				m_psurfaces.RemoveAt(i) ;
				return 1 ;
			}
		}
		return 0 ;
	}
}

// nt add 2010/9/26
void CStep::ClearModel(int gmt)
{
	if( gmt = gmtWorkpiece )
	{
		m_workpiece.RemoveAll() ;
		return ;
	}
	else
	if( gmt == gmtPart )
	{
		m_part.RemoveAll() ;
		return ;
	}
	else
	{
		m_psurfaces.RemoveAll() ;
		return ;
	}
}

int CStep::GetNo()
{
	return m_no ;
}

int CStep::SetNo(int no)
{
	m_no = no ;
	return 1 ;
}

// 取加工坐标系
int CStep::GetMCS(RFRAME* pLF)
{
	if( pLF )
	{
		*pLF = m_lf ;
		return 1 ;
	}
	else
		return 0 ;
}

// 设置加工坐标系
int CStep::SetMCS(RFRAME* pLF)
{
	if( pLF )
	{
		m_lf = *pLF ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/26
void CStep::GetCP(CP* pCP)
{
	if( pCP )
		*pCP = m_cp ;
}

// nt add 2010/9/26
void CStep::SetCP(CP* pCP)
{
	if( pCP )
		m_cp = *pCP ;
}

CTool* CStep::GetTool()
{
	return m_pTool ;
}

void CStep::SetTool(CTool* pTool)
{
	if( !pTool )
		return ;
	if( m_pTool )
		delete m_pTool ;
	m_pTool = pTool ;
}

CPath* CStep::GetPath()
{
	return m_pPath ;
}

void CStep::SetPath(CPath* pPath)
{
	if( !pPath )
		return ;
	if( m_pPath )
		delete m_pPath ;
	m_pPath = pPath ;
	CalBox() ;
}

// nt add 2010/11/29
int CStep::GetColor()
{
	return m_color ;
}

// nt add 2010/11/29
void CStep::SetColor(int color)
{
	m_color = color ;
}

// nt add 2010/11/29
HTREEITEM CStep::GetItem()
{
	return m_hItem ;
}

// nt add 2010/11/29
void CStep::SetItem(HTREEITEM hItem)
{
	m_hItem = hItem ;
}

int CStep::CalBox()
{
	int k ;
	BOX3D box ;

	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;

	if( m_pPath )
	{
		m_pPath->GetBox(&box) ;
		for( k = 0 ; k < 3 ; k++ )
		{
			m_box.min[k] = mathMIN(box.min[k], m_box.min[k]) ;
			m_box.max[k] = mathMAX(box.max[k], m_box.max[k]) ;
		}
	}

	return 1 ;
}

int CStep::GetBox(BOX3D* box)
{
	if( box )
	{
		*box = m_box ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2010/9/26
void CStep::SetBox(BOX3D* box)
{
	if( box )
		m_box = *box ;
}
//--------------------------------------------------------------
int CStep::Draw(void* pVI, int drawMode)
{
	if( !m_pPath )
		return 0 ;

	// nt add 2010/11/29
	if( drawMode == dmNormal )
	{
		unsigned char rgb[3] = {0, 0, 0} ;
		rgbsGet(m_color, rgb) ;
		glColor3ub(rgb[0], rgb[1], rgb[2]) ;
	}
	else
	if( drawMode == dmPrompt )
		glColor3ub(255, 0, 0) ;
	else
	if( drawMode == dmSelect )
		glColor3ub(0, 255, 0) ;
	else
		glColor3ub(50, 50, 100) ;
	// end

	m_pPath->Draw(pVI, drawMode) ;

	return 1 ;
}

// nt add 2010/11/29
int CStep::Draw2(void* pVI, int drawMode)
{
	if( !m_pPath )
		return 0 ;
	Draw(pVI, drawMode) ;

	return 1 ;
}

int	CStep::Pick(PINFO* pi)
{
	if( !pi )
		return 0 ;
	if( !m_pPath )
		return 0 ;
	m_pPath->Pick(pi) ;
	if( pi->obj )
	{
		pi->ot = camStep ; // nt add 2010/11/30
		pi->obj = this ; // nt add 2010/11/30
		return 1 ;
	}

	return 2 ;
}

// 1:picked;2:pick none;0:error
int	CStep::PickByBox(RFRAME* pLF, BOX2D* pBox, double r, COA& es)
{
	return 2 ;
}
//--------------------------------------------------------------
// nt modify 2010/11/29
int CStep::Serialize(CArchive& ar)
{
	int flag, ver = 0, i, n, no ;

	if( ar.IsStoring() )
	{
		ar << ver
		   << m_name
		   << m_no
		   << m_color ; // nt add 2010/11/29

		ar.Write(&m_box, sizeof(BOX3D)) ;
		ar.Write(&m_lf, sizeof(RFRAME)) ;
		ar.Write(&m_cp, sizeof(CP)) ;
		if( m_pTool )
			flag = 1 ;
		else
			flag = 0 ;
		ar << flag ;
		if( flag )
			m_pTool->Serialize(ar) ;
		if( m_pPath )
			flag = 1 ;
		else
			flag = 0 ;
		ar << flag ;
		if( flag )
			m_pPath->Serialize(ar) ;

		n = m_workpiece.GetSize() ;
		ar << n ;
		for( i = 0 ; i < n ; i++ )
		{
			no = m_workpiece[i] ;
			ar << no ;
		}
		n = m_part.GetSize() ;
		ar << n ;
		for( i = 0 ; i < n ; i++ )
		{
			no = m_part[i] ;
			ar << no ;
		}
		n = m_psurfaces.GetSize() ;
		ar << n ;
		for( i = 0 ; i < n ; i++ )
		{
			no = m_psurfaces[i] ;
			ar << no ;
		}
	}
	else
	{
		ar >> ver
		   >> m_name
		   >> m_no
		   >> m_color ; // nt add 2010/11/29

		ar.Read(&m_box, sizeof(BOX3D)) ;
		ar.Read(&m_lf, sizeof(RFRAME)) ;
		ar.Read(&m_cp, sizeof(CP)) ;
		ar >> flag ;
		if( flag == 1 )
		{
			CTool* pTool = new CTool() ;
			pTool->Serialize(ar) ;
			SetTool(pTool) ;
		}
		ar >> flag ;
		if( flag == 1 )
		{
			CPath* pPath = new CPath() ;
			pPath->Serialize(ar) ;
			SetPath(pPath) ;
		}

		ar >> n ;
		for( i = 0 ; i < n ; i++ )
		{
			ar >> no ;
			m_workpiece.Add(no) ;
		}
		ar >> n ;
		for( i = 0 ; i < n ; i++ )
		{
			ar >> no ;
			m_part.Add(no) ;
		}
		ar >> n ;
		for( i = 0 ; i < n ; i++ )
		{
			ar >> no ;
			m_psurfaces.Add(no) ;
		}
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////
