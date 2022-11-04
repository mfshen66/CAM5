/*	2010/3/31 nt
 *	implementation file of class CSim5
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "Path.h"
#include "Sim5.h"

///////////////////////////////////////////////////////////////
//	CSim5
/*	2010/6/1 nt
 *	Simulation
 */
CSim5::CSim5()
{
	m_state = 0 ;
	m_pTool = NULL ;
	mathInitRFrame(&m_old) ;
	m_pPath = NULL ;
	m_n = 0 ;
	m_minsp = 500. ;
	m_maxsp = 15000. ;
	m_pause = 0 ;

	m_t = 0 ;
	m_i = -1 ;
	m_speed = 0. ;
	mathInitRFrame(&m_lf1) ;
	mathInitRFrame(&m_lf2) ;
	m_d = 0. ;
	mathInitRFrame(&m_lf) ;
}

CSim5::~CSim5()
{
	if( m_pTool )
		m_pTool->SetLF(&m_old) ;
}
//--------------------------------------------------------------
// nt add 2010/7/2
int CSim5::IsPaused()
{
	return m_pause ;
}

// nt add 2010/7/2
int CSim5::Pause()
{
	m_pause = 1 ;
	return 1 ;
}

// nt add 2010/7/2
int CSim5::Continue()
{
	m_t = GetTickCount() ;
	m_pause = 0 ;
	return 1 ;
}

int CSim5::GetState()
{
	return m_state ;
}

int CSim5::SetPath(CPath* pPath)
{
	m_pPath = pPath ;
	return 1 ;
}

CTool* CSim5::GetTool()
{
	return m_pTool ;
}

int CSim5::SetTool(CTool* pTool)
{
	m_pTool = pTool ;
	return 1 ;
}

int CSim5::SetMinSpeed(double minsp)
{
	m_minsp = minsp ;
	return 1 ;
}

int CSim5::SetMaxSpeed(double maxsp)
{
	m_maxsp = maxsp ;
	return 1 ;
}

int CSim5::GetCurPos(PNT3D cp)
{
	memcpy(cp, m_lf.O, sizeof(PNT3D)) ;
	return 1 ;
}

int CSim5::GetCurAxis(VEC3D axis)
{
	memcpy(axis, m_lf.Z, sizeof(VEC3D)) ;
	return 1 ;
}

// 根据i更新m_lf1,m_lf2,m_lf等
int CSim5::UpdateTPN(int i)
{
	if( m_pPath &&
		i >= 0 &&
		i < m_n )
	{
		m_lf1 = m_lf2 ; // move forwardly
		m_lf  = m_lf1 ;
		PNODE pnode ;
		m_pPath->GetPNode(i, &pnode) ;
		memcpy(m_lf2.O, pnode.pos, sizeof(PNT3D)) ;
		memcpy(m_lf2.Z, pnode.axis, sizeof(VEC3D)) ;
		mathGetXYFromZ(m_lf2.Z, m_lf2.X, m_lf2.Y) ;
		m_speed = pnode.fr ; // mm/min
		if( m_speed < m_minsp )
			m_speed = m_minsp ;
		else
		if( m_speed > m_maxsp )
			m_speed = m_maxsp ;
		m_d = mathDist(m_lf1.O, m_lf2.O) ;
		m_i = i ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// 做一些初始化
// 适用用于五坐标G代码仿真
int CSim5::Start(UINT ct)
{
	if( m_pTool &&
		m_pPath )
	{
		m_pTool->GetLF(&m_old) ;
		m_lf  = m_old ; // initial position
		m_lf1 = m_old ; // initial position
		m_lf2 = m_old ; // initial position
		m_n = m_pPath->GetNumOfPNode() ;
		if( m_n < 2 )
			return 0 ;
		int i ;
		m_i = -1 ;
		for( i = 0 ; i < m_n-1 ; i++ )
		{
			UpdateTPN(i) ; // 更新路径节点等
			if( m_d > MIN_DBL )
				break ;
		}
		if( i >= m_n-1 )
			return 0 ;
		m_t = ct ; // current time
		m_state = 1 ;
		return 1 ;
	}
	else
		return 0 ;
}

int CSim5::Update(UINT ct)
{
	if( m_state != 1 ||
		m_pTool == NULL ||
		m_pPath == NULL ||
		m_d < MIN_DBL )
		return 0 ;
	int dt = (int)(ct-m_t) ; // unit=second/1000
	if( dt <= 0 )
		return 1 ;
	m_t = ct ;
	double w, d = 0.001*dt/60.*m_speed ; // 需要走的路径长度
	double s = mathDist(m_lf.O, m_lf2.O) ; // 还剩的路径长度
	if( d < s ) // 还在当前段内走
	{
		m_lf.O[0] += d*(m_lf2.O[0]-m_lf1.O[0])/m_d ;
		m_lf.O[1] += d*(m_lf2.O[1]-m_lf1.O[1])/m_d ;
		m_lf.O[2] += d*(m_lf2.O[2]-m_lf1.O[2])/m_d ;
		w = (s-d)/m_d ;
		m_lf.Z[0] = w*m_lf1.Z[0]+(1.-w)*m_lf2.Z[0] ;
		m_lf.Z[1] = w*m_lf1.Z[1]+(1.-w)*m_lf2.Z[1] ;
		m_lf.Z[2] = w*m_lf1.Z[2]+(1.-w)*m_lf2.Z[2] ;
		mathUniVec(m_lf.Z, MIN_DBL) ;
		mathGetXYFromZ(m_lf.Z, m_lf.X, m_lf.Y) ;
	}
	else // 超出当前段
	{
		d -= s ; // 需要走的路径长度，unit=mm
		int i ;
		for( i = m_i+1 ; i < m_n ; i++ )
		{
			UpdateTPN(i) ;
			if( d < m_d ) // ok
			{
				m_lf.O[0] += d*(m_lf2.O[0]-m_lf1.O[0])/m_d ;
				m_lf.O[1] += d*(m_lf2.O[1]-m_lf1.O[1])/m_d ;
				m_lf.O[2] += d*(m_lf2.O[2]-m_lf1.O[2])/m_d ;
				w = (m_d-d)/m_d ;
				m_lf.Z[0] = w*m_lf1.Z[0]+(1.-w)*m_lf2.Z[0] ;
				m_lf.Z[1] = w*m_lf1.Z[1]+(1.-w)*m_lf2.Z[1] ;
				m_lf.Z[2] = w*m_lf1.Z[2]+(1.-w)*m_lf2.Z[2] ;
				mathUniVec(m_lf.Z, MIN_DBL) ;
				mathGetXYFromZ(m_lf.Z, m_lf.X, m_lf.Y) ;
				break ;
			}
			else
				d -= m_d ;
		}
		if( i >= m_n ) // finish
		{
			m_lf = m_lf2 ;
			m_state = 2 ;
		}
	}
	m_pTool->SetLF(&m_lf) ;

	return 1 ;
}
//--------------------------------------------------------------
int CSim5::Draw(void* pVI)
{
	if( m_pTool )
	{
		m_pTool->Draw(pVI, dmNormal) ;
		return 1 ;
	}
	else
		return 0 ;
}
///////////////////////////////////////////////////////////////