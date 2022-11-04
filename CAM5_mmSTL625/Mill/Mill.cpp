/*	2010/3/29 nt
 *	implementation file of class CMill
 */
#include "StdAfx.h"
#include "CP.h"
#include "TRS.h"
#include "Tool.h"
#include "Machine.h"
#include "Path.h"
#include "Step.h"
#include "Mill.h"

///////////////////////////////////////////////////////////////
//	CMill
/*	2010/3/29 nt
 *	CMill
 *	nt modify 2010/9/25
 */
CMill::CMill()
{
	m_name = "数控工艺" ;
	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	mathInitRFrame(&m_lf) ;
	m_pMachine = NULL ;
	cpInit(&m_cp) ;
	m_pTool = NULL ;
	m_material = -1 ;
	m_partName = "" ;
	m_workpiece.SetSize(512, 512) ;
	m_part.SetSize(512, 512) ;
}

// nt modify 2010/9/25
CMill::~CMill()
{
	int i, n = m_steps.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		if( m_steps[i] )
			delete m_steps[i] ;
	}
	m_steps.RemoveAll() ;
	m_workpiece.RemoveAll() ;
	m_part.RemoveAll() ;
	if( m_pMachine )
		delete m_pMachine ;
	if( m_pTool )
		delete m_pTool ;
}
//--------------------------------------------------------------
void CMill::GetName(CStr& name)
{
	name = m_name ;
}

void CMill::SetName(CStr& name)
{
	m_name = name ;
}

// nt add 2010/9/26
int CMill::GetNumOfGeometry(int gmt)
{
	if( gmt = gmtWorkpiece )
		return m_workpiece.GetSize() ;
	else
		return m_part.GetSize() ;
}

// nt add 2010/9/26
int CMill::GetGeometryNo(int gmt, int index)
{
	if( gmt = gmtWorkpiece )
		return m_workpiece[index] ;
	else
		return m_part[index] ;
}

// nt add 2010/9/26
int CMill::AddGeometry(int gmt, int geometryNo)
{
	if( gmt = gmtWorkpiece )
	{
		m_workpiece.Add(geometryNo) ;
	}
	else
	{
		m_part.Add(geometryNo) ;
	}
	return 1 ;
}

// nt add 2010/9/26
int CMill::DeleteGeometry(int gmt, int geometryNo)
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
}

// nt add 2010/9/26
void CMill::ClearModel(int gmt)
{
	if( gmt = gmtWorkpiece )
	{
		m_workpiece.RemoveAll() ;
		return ;
	}
	else
	{
		m_part.RemoveAll() ;
		return ;
	}
}

// 取加工坐标系
int CMill::GetMCS(RFRAME* pLF)
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
int CMill::SetMCS(RFRAME* pLF)
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
void CMill::GetCP(CP* pCP)
{
	if( pCP )
		*pCP = m_cp ;
}

// nt add 2010/9/26
void CMill::SetCP(CP* pCP)
{
	if( pCP )
		m_cp = *pCP ;
}

int CMill::GetPartName(CStr& partName)
{
	partName = m_partName ;
	return 1 ;
}

int CMill::SetPartName(CStr& partName)
{
	m_partName = partName ;
	return 1 ;
}

CTool* CMill::GetTool()
{
	return m_pTool ;
}

void CMill::SetTool(CTool* pTool)
{
	if( !pTool )
		return ;
	if( m_pTool )
		delete m_pTool ;
	m_pTool = pTool ;
}

CMachine* CMill::GetMachine()
{
	return m_pMachine ;
}

void CMill::SetMachine(CMachine* pMachine)
{
	if( !pMachine )
		return ;
	if( m_pMachine )
		delete m_pMachine ;
	m_pMachine = pMachine ;
}

int CMill::GetNumOfStep()
{
	return m_steps.GetSize() ;
}

CStep* CMill::GetStep(int index)
{
	return (CStep*)(m_steps[index]) ;
}

// nt add 2010/9/26
int CMill::GetMaxStepNo()
{
	int i, n = m_steps.GetSize(), max = -1 ;
	for( i = 0 ; i < n ; i++ )
	{
		CStep* pStep = (CStep*)m_steps[i] ;
		if( pStep )
		{
			int no = pStep->GetNo() ;
			if( max < no )
				max = no ;
		}
	}
	return max ;
}

int	CMill::Add(CStep* pStep)
{
	int k ;
	BOX3D box ;

	m_steps.Add(pStep) ;
	pStep->GetBox(&box) ;
	for( k = 0 ; k < 3 ; k++ )
	{
		m_box.min[k] = mathMIN(box.min[k], m_box.min[k]) ;
		m_box.max[k] = mathMAX(box.max[k], m_box.max[k]) ;
	}

	return 1 ;
}

int	CMill::Delete(CStep* pStep)
{
	CStep* pS ;

	for( int i = 0, n = m_steps.GetSize() ; i < n ; i++ )
	{
		pS = (CStep*)m_steps[i] ;
		if( !pS )
			return 0 ;
		if( pStep == pS )
		{
			m_steps.RemoveAt(i) ;
			return 1 ;
		}
	}

	return 0 ;
}

int CMill::CalBox()
{
	int k ;
	BOX3D box ;
	CStep* pStep ;

	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	for( int i = 0, n = m_steps.GetSize() ; i < n ; i++ )
	{
		pStep = (CStep*)m_steps[i] ;
		if( !pStep )
			return 0 ;
		pStep->GetBox(&box) ;
		for( k = 0 ; k < 3 ; k++ )
		{
			m_box.min[k] = mathMIN(box.min[k], m_box.min[k]) ;
			m_box.max[k] = mathMAX(box.max[k], m_box.max[k]) ;
		}
	}

	return 1 ;
}

int CMill::GetBox(BOX3D* box)
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
int CMill::Draw(void* pVI, int drawMode)
{
	int i, n = m_steps.GetSize() ;
	for( i = 0 ; i < n ; i++ )
	{
		CStep* pStep = (CStep*)m_steps[i] ;
		if( !pStep )
			return 0 ;
		pStep->Draw(pVI, drawMode) ;
	}

	return 1 ;
}

int	CMill::Pick(PINFO* pi)
{
	int i, n = m_steps.GetSize() ;
	if( !pi )
		return 0 ;
	for( i = 0 ; i < n ; i++ )
	{
		CStep* pStep = (CStep*)m_steps[i] ;
		if( !pStep )
			return 0 ;
		pStep->Pick(pi) ;
		if( pi->obj )
			return 1 ;
	}

	return 2 ;
}

// 1:picked;2:pick none;0:error
int	CMill::PickByBox(RFRAME* pLF, BOX2D* pBox, double r, COA& es)
{
	return 2 ;
}

int CMill::Serialize(CArchive& ar)
{
	int i, n, flag, ver = 0 ;

	if( ar.IsStoring() )
	{
		ar << ver 
		   << m_name 
		   << m_partName
		   << m_material ;

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
		if( m_pMachine )
			flag = 1 ;
		else
			flag = 0 ;
		ar << flag ;
		if( flag )
			m_pMachine->Serialize(ar) ;

		n = m_steps.GetSize() ;
		ar << n ;
		for( i = 0 ; i < n ; i++ )
		{
			CStep* pStep = (CStep*)m_steps[i] ;
			pStep->Serialize(ar) ;
		}
	}
	else
	{
		ar >> ver 
		   >> m_name 
		   >> m_partName
		   >> m_material ;

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
			CMachine* pMachine = new CMachine(5) ;
			pMachine->Serialize(ar) ;
			SetMachine(pMachine) ;
		}

		ar >> n ;
		for( i = 0 ; i < n ; i++ )
		{
			CStep* pStep = new CStep() ;
			pStep->Serialize(ar) ;
			m_steps.Add(pStep) ;
		}
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////
