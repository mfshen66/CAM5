/*	2010/3/31 nt
 *	implementation file of class CSim
 */
#include "StdAfx.h"
#include "TRS.h"
#include "Tool.h"
#include "Sim.h"

///////////////////////////////////////////////////////////////
//	CSim
/*	2010/3/31 nt
 *	Simulation
 */
CSim::CSim()
{
	m_state = 0 ;
	m_pTool = NULL ;
	mathInitRFrame(&m_old) ;
	m_pGT = NULL ;
	m_n = 0 ;
	m_minsp = 500. ;
	m_maxsp = 15000. ;
	m_pause = 0 ; // nt add 2010/7/2

	m_t = 0 ;
	m_i = -1 ;
	m_speed = 0. ;
	mathInitRFrame(&m_lf1) ;
	mathInitRFrame(&m_lf2) ;
	m_d = 0. ;
	mathInitRFrame(&m_lf) ;
	m_bi = -1 ;
	m_cb[0] = '\0' ;
	m_rs = 0 ;
}

CSim::~CSim()
{
	if( m_pTool )
		m_pTool->SetLF(&m_old) ;
}
//--------------------------------------------------------------
// nt add 2010/7/2
int CSim::IsPaused()
{
	return m_pause ;
}

// nt add 2010/7/2
int CSim::Pause()
{
	m_pause = 1 ;
	return 1 ;
}

// nt add 2010/7/2
int CSim::Continue()
{
	m_t = GetTickCount() ;
	m_pause = 0 ;
	return 1 ;
}

int CSim::GetState()
{
	return m_state ;
}

int CSim::SetGT(GCODETPATH* pGT)
{
	m_pGT = pGT ;
	return 1 ;
}

CTool* CSim::GetTool()
{
	return m_pTool ;
}

int CSim::SetTool(CTool* pTool)
{
	m_pTool = pTool ;
	return 1 ;
}

int CSim::SetMinSpeed(double minsp)
{
	m_minsp = minsp ;
	return 1 ;
}

int CSim::SetMaxSpeed(double maxsp)
{
	m_maxsp = maxsp ;
	return 1 ;
}

int CSim::GetBI()
{
	return m_bi ;
}

int CSim::GetCurBlk(CString& cb)
{
	cb = m_cb ;
	return 1 ;
}

int CSim::GetCurPos(PNT3D cp)
{
	memcpy(cp, m_lf.O, sizeof(PNT3D)) ;
	return 1 ;
}

int CSim::GetRS()
{
	return m_rs ;
}

// 根据i更新m_lf1,m_lf2,m_lf等
int CSim::UpdateTPN(int i)
{
	if( m_pGT &&
		i >= 0 &&
		i < m_n-1 )
	{
		TNODE tnode ;
		gcodetpathGetTNode(m_pGT, i, &tnode) ;
		m_lf1.O[0] = (double)tnode.p[0] ;
		m_lf1.O[1] = (double)tnode.p[1] ;
		m_lf1.O[2] = (double)tnode.p[2] ;
		memcpy(m_lf.O, m_lf1.O, sizeof(PNT3D)) ;
		m_speed = tnode.F ; // mm/min
		if( m_speed < m_minsp )
			m_speed = m_minsp ;
		else
		if( m_speed > m_maxsp )
			m_speed = m_maxsp ;
		if( m_bi != tnode.bi ) // m_bi没有变不更新
		{
			m_bi = tnode.bi ;
			m_cb[0] = '\0' ;
			gcodetpathGetCode(m_pGT, m_bi, m_cb) ;
		}
		m_rs = tnode.rs ;
		gcodetpathGetTNode(m_pGT, i+1, &tnode) ;
		m_lf2.O[0] = (double)tnode.p[0] ;
		m_lf2.O[1] = (double)tnode.p[1] ;
		m_lf2.O[2] = (double)tnode.p[2] ;
		m_d = mathDist(m_lf1.O, m_lf2.O) ;
		m_i = i ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
// 做一些初始化
int CSim::Start(UINT ct)
{
	if( m_pTool )
	{
		if( m_pGT ) // m_pGT仅用于三坐标G代码仿真
		{
			m_pTool->GetLF(&m_old) ;
			m_n = gcodetpathGetNumOfTNode(m_pGT) ;
			if( m_n < 2 )
				return 0 ;
			int i ;
			m_i = -1 ;
			m_bi = -1 ;
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
	else
		return 0 ;
}

int CSim::Update(UINT ct)
{
	if( m_state != 1 ||
		m_pTool == NULL ||
		m_d < MIN_DBL )
		return 0 ;
	int dt = (int)(ct-m_t) ; // unit=second/1000
	if( dt <= 0 )
		return 1 ;
	m_t = ct ;
	double d = 0.001*dt/60.*m_speed ; // 需要走的路径长度
	double s = mathDist(m_lf.O, m_lf2.O) ; // 还剩的路径长度
	if( d < s ) // 还在当前段内走
	{
		m_lf.O[0] += d*(m_lf2.O[0]-m_lf1.O[0])/m_d ;
		m_lf.O[1] += d*(m_lf2.O[1]-m_lf1.O[1])/m_d ;
		m_lf.O[2] += d*(m_lf2.O[2]-m_lf1.O[2])/m_d ;
	}
	else // 超出当前段
	{
		d -= s ; // 需要走的路径长度，unit=m
		int i ;
		TNODE tnode ;
		for( i = m_i+1 ; i < m_n-1 ; i++ )
		{
			memcpy(m_lf1.O, m_lf2.O, sizeof(PNT3D)) ;
			gcodetpathGetTNode(m_pGT, i+1, &tnode) ;
			m_lf2.O[0] = (double)tnode.p[0] ;
			m_lf2.O[1] = (double)tnode.p[1] ;
			m_lf2.O[2] = (double)tnode.p[2] ;
			m_d = mathDist(m_lf1.O, m_lf2.O) ;
			if( d < m_d ) // ok
			{
				UpdateTPN(i) ;
				memcpy(m_lf.O, m_lf1.O, sizeof(PNT3D)) ;
				m_lf.O[0] += d*(m_lf2.O[0]-m_lf1.O[0])/m_d ;
				m_lf.O[1] += d*(m_lf2.O[1]-m_lf1.O[1])/m_d ;
				m_lf.O[2] += d*(m_lf2.O[2]-m_lf1.O[2])/m_d ;
				break ;
			}
			else
				d -= m_d ;
		}
		if( i >= m_n-1 ) // finish
		{
			UpdateTPN(m_n-2) ;
			memcpy(m_lf.O, m_lf2.O, sizeof(PNT3D)) ;
			m_state = 2 ;
		}
	}
	m_pTool->SetLF(&m_lf) ;

	return 1 ;
}
//--------------------------------------------------------------
int CSim::Draw(void* pVI)
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