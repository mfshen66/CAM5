/*	2002/4/7 nt
 *	implementation file of preview info
 */
#include "StdAfx.h"
#include "..\Models.h"
#include "PV.h"

///////////////////////////////////////////////////////////////
//	CPV
/*	2002/4/7 nt
 *	CPV
 */
CPV::CPV()
{
	m_plines = NULL ;
	m_facets = NULL ;
	//m_tmeshs = NULL ;

	// nt add 2015/7/16
	m_nu = 0 ;
	m_nv = 0 ;
	m_ps = NULL ;
}

CPV::~CPV()
{
	VPCARRIER* vpc ;

	for( vpc = m_plines ; vpc != NULL ; vpc = vpc->next ) // free polylines
		strptFree((STRPT*)(vpc->pointer)) ;
	mathFreSDChns(m_plines) ;
	m_plines = NULL ;

	for( vpc = m_facets ; vpc != NULL ; vpc = vpc->next ) // free facets
		facetFree((FACET*)(vpc->pointer)) ;
	mathFreSDChns(m_facets) ;
	m_facets = NULL ;

	/*for( vpc = m_tmeshs ; vpc != NULL ; vpc = vpc->next ) // free tmeshs
		tmesh2dFree((TMESH2D*)(vpc->pointer)) ;
	mathFreSDChns(m_facets) ;
	m_tmeshs = NULL ;*/

	if( m_ps )
		delete []m_ps ;
	m_ps = NULL ;

	return ;
}

void CPV::AddPLine(STRPT* strpt)
{
	VPCARRIER* vpc ;

	if( strpt )
	{
		vpc = (VPCARRIER*)envAlloc(sizeof(VPCARRIER)) ;
		vpc->pointer = strpt ;
		vpc->next = m_plines ;
		m_plines = vpc ;
	}
}

void CPV::AddFacet(FACET* facet)
{
	VPCARRIER* vpc ;

	if( facet )
	{
		vpc = (VPCARRIER*)envAlloc(sizeof(VPCARRIER)) ;
		vpc->pointer = facet ;
		vpc->next = m_facets ;
		m_facets = vpc ;
	}
}

/*void CPV::AddTMesh(TMESH2D* tmesh)
{
	VPCARRIER* vpc ;

	if( tmesh )
	{
		vpc = (VPCARRIER*)envAlloc(sizeof(VPCARRIER)) ;
		vpc->pointer = tmesh ;
		vpc->next = m_tmeshs ;
		m_tmeshs = vpc ;
	}
}*/

int CPV::CreateGrid(int nu, int nv)
{
	if( m_ps )
		delete []m_ps ;
	m_ps = new double[nu*nv][3] ;
	if( !m_ps )
		return 0 ;
	m_nu = nu ;
	m_nv = nv ;

	return 1 ;
}

void CPV::AddGridPt(int i, PNT3D p)
{
	memcpy(m_ps[i], p, sizeof(PNT3D)) ;

	return ;
}

void CPV::Draw(void* pVI, int drawMode)
{
	int i, j ;
	VPCARRIER* vpc ;

	for( vpc = m_plines ; vpc != NULL ; vpc = vpc->next ) // draw polylines
		strptDraw((STRPT*)(vpc->pointer), RGB(0, 0, 0), 1.0) ;

	for( vpc = m_facets ; vpc != NULL ; vpc = vpc->next ) // draw facets
	{
		FACET* facet = (FACET*)(vpc->pointer) ;
		for( int i = 0 ; i < facet->ntria ; i++ )
		{
			glBegin(GL_LINE_LOOP) ;
			glVertex3dv(facet->nodes[facet->trias[i][0]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][1]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][2]].p) ;
			glEnd() ;
		}
		
	}

	/*for( vpc = m_tmeshs ; vpc != NULL ; vpc = vpc->next ) // draw tmeshs
	{
		TMESH2D* tmesh = (TMESH2D*)(vpc->pointer) ;
		for( MTRIA* mtria = tmesh->mtrias ; mtria ; mtria = mtria->next )
		{
			glBegin(GL_LINE_LOOP) ;
			glVertex2dv(mtria->mverts[0]->uv) ;
			glVertex2dv(mtria->mverts[1]->uv) ;
			glVertex2dv(mtria->mverts[2]->uv) ;
			glEnd() ;
		}
		glBegin(GL_LINES) ;
		for( MEDGE2D* e = tmesh->medges ; e ; e = e->next )
		{
			glVertex3d(e->mverts[0]->uv[0],
					   e->mverts[0]->uv[1],
					   e->mverts[0]->attrib) ;
			glVertex3d(e->mverts[1]->uv[0],
					   e->mverts[1]->uv[1],
					   e->mverts[1]->attrib) ;
		}
		glEnd() ;
	}*/

	// nt add 2015/7/16
	if( m_ps )
	{
		for( i = 0 ; i < m_nu ; i++ )
		{
			glBegin(GL_LINE_STRIP) ;
			for( j = 0 ; j < m_nv ; j++ )
			{
				glVertex3dv(m_ps[m_nv*i+j]) ;
			}
			glEnd() ;
		}

		for( j = 0 ; j < m_nv ; j++ )
		{
			glBegin(GL_LINE_STRIP) ;
			for( i = 0 ; i < m_nu ; i++ )
			{
				glVertex3dv(m_ps[m_nv*i+j]) ;
			}
			glEnd() ;
		}
	}

	return ;
}
///////////////////////////////////////////////////////////////
