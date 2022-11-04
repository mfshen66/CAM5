/*	2010/3/30 nt
 *	implementation file of structure TRS
 */
#include "StdAfx.h"
#include "TRS.h"

///////////////////////////////////////////////////////////////
//	TRS
/*	2010/3/30 nt
 *	TRS
 */
TRS* trsCreate(CURVE* curve, INTERVAL* interval)
{
	TRS* trs = new TRS() ;
	if( curve )
		trs->curve = geometryCopy(curve) ;
	else
		trs->curve = NULL ;
	if( interval )
		trs->interval = *interval ;
	else
	{
		trs->interval.t1 = 0. ;
		trs->interval.t2 = 1. ;
	}
	trs->strpt = NULL ;
	trs->facet = NULL ;
	trs->n = 0 ;
	return trs ;
}

int trsFree(TRS* trs)
{
	if( trs )
	{
		if( trs->curve )
			entityFree(trs->curve) ;
		if( trs->strpt )
			strptFree(trs->strpt) ;
		if( trs->facet )
			facetFree(trs->facet) ;
		delete trs ;
		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}
//--------------------------------------------------------------
int trsCalBox(TRS* trs)
{
	if( trs &&
		trs->curve )
	{
		if( curveCalBox(trs->curve, 
						&(trs->interval), 
						&(trs->box)) != ERSUCSS )
			return ERUNSUC ;
		trs->box.min[1] = trs->box.min[0] ; // !!!
		trs->box.max[1] = trs->box.max[0] ; // !!!
		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}

int trsGetBox(TRS* trs, BOX3D* box)
{
	if( trs &&
		box )
	{
		*box = trs->box ;
		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}

// n>=3
int trsDisc(TRS* trs, double dTol, double dAng, int n)
{
	int i, j, nu, nv, m ;
	PNT3D o = {0., 0., 0.} ;
	VEC3D Y = {0., 1., 0.}, Z = {0., 0., 1.}, normal ;
	STRPT* strpt ;
	FACET* facet ;

	if( trs &&
		trs->curve &&
		n >= 3 )
	{
		if( curveDisc(trs->curve, 
					  &(trs->interval),
					  dTol, 
					  dAng,
					  1000.,
					  STRPT_PT|STRPT_VEC|STRPT_PARAM,
					  &strpt) != ERSUCSS )
			return ERUNSUC ;
		if( strpt )
		{
			for( i = 0 ; i < strpt->np ; i++ )
			{
				strpt->ps[i][1] = 0. ; // !!!应该就是0.
				mathVProduct(strpt->vs[i], Y, normal) ;
				mathUniVec(normal, MIN_DBL) ;
				normal[0] = -normal[0] ;
				normal[1] = -normal[1] ;
				normal[2] = -normal[2] ;
				memcpy(strpt->vs[i], normal, sizeof(VEC3D)) ;
			}

			trs->n = n ;
			nu = n ;
			nv = strpt->np-1 ;
			facet = facetCreate((nu+1)*(nv+1), nu*nv*2, 0) ;
			if( facet == NULL )
			{
				strptFree(strpt) ;
				return ERMEM ;
			}
			for( m = 0, i = 0 ; i <= nu ; i++ )
			{
				for( j = 0 ; j <= nv ; j++, m++ )
				{
					facet->nodes[m].uv[0] = i*PI2/nu ;
					facet->nodes[m].uv[1] = strpt->ts[j] ;
					facet->nodes[m].attrib = 0. ;
					mathRotPnt(Z, 
							   o,
							   facet->nodes[m].uv[0],
							   strpt->ps[j],
							   facet->nodes[m].p) ;
					mathRotVec(Z, 
							   o,
							   facet->nodes[m].uv[0],
							   strpt->vs[j],
							   facet->nodes[m].normal) ;
					facet->nodes[m].attrib = 0. ;
				}
			} // m = i*(nv+1)+j
			for( m = 0, i = 0 ; i < nu ; i++ )
			{
				for( j = 0 ; j < nv ; j++, m += 2 )
				{
					facet->trias[m][0] = i*(nv+1)+j ;
					facet->trias[m][1] = (i+1)*(nv+1)+j ;
					facet->trias[m][2] = (i+1)*(nv+1)+(j+1) ;

					facet->trias[m+1][0] = i*(nv+1)+j ;
					facet->trias[m+1][1] = (i+1)*(nv+1)+(j+1) ;
					facet->trias[m+1][2] = i*(nv+1)+(j+1) ;
				}
			}
			trs->strpt = strpt ;
			trs->facet = facet ;

			return ERSUCSS ;
		}
		else
			return ERUNSUC ;
	}
	else
		return ERUNSUC ;
}
//--------------------------------------------------------------
int trsDraw(TRS* trs, void* pVI, int drawMode, int color)
{
	if( trs )
	{
		FACET* facet = trs->facet ;
		glVertexPointer(3, GL_DOUBLE, sizeof(FNODE), facet->nodes[0].p) ;
		glNormalPointer(GL_DOUBLE, sizeof(FNODE), facet->nodes[0].normal) ;
		glDrawElements(GL_TRIANGLES, facet->ntria*3, GL_UNSIGNED_INT, facet->trias) ;

		//facetDraw(facet, IDSHADE, color) ;

		/*glBegin(GL_TRIANGLES) ;
		for( int i = 0 ; i < facet->ntria ; i++ )
		{
			glNormal3dv(facet->nodes[facet->trias[i][0]].normal) ;
			glVertex3dv(facet->nodes[facet->trias[i][0]].p) ;
			glNormal3dv(facet->nodes[facet->trias[i][1]].normal) ;
			glVertex3dv(facet->nodes[facet->trias[i][1]].p) ;
			glNormal3dv(facet->nodes[facet->trias[i][2]].normal) ;
			glVertex3dv(facet->nodes[facet->trias[i][2]].p) ;
		}
		glEnd() ;*/

		/*
		glBegin(GL_LINES) ;
		for( int i = 0 ; i < facet->ntria ; i++ )
		{
			glVertex3dv(facet->nodes[facet->trias[i][0]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][1]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][1]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][2]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][2]].p) ;
			glVertex3dv(facet->nodes[facet->trias[i][0]].p) ;
		}
		glEnd() ;*/

		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}

int trsPick(TRS* trs, PINFO* pi)
{
	int pick ;

	if( trs &&
		pi )
	{
		facetPick(trs->facet, 
				  pi->pos,
				  pi->dir,
				  pi->r,
				  &(pi->z),
				  &(pi->u),
				  &(pi->v),
				  &pick) ;
		if( pick == 1 )
		{
			pi->ot = 100 ; // ???
			pi->obj = trs ;
			return 1 ;
		}
		else
			return 2 ;
	}
	else
		return 0 ;
}

int trsSerialize(TRS* trs, CArchive& ar)
{
	int type ;
	double r ;
	PNT3D p, p1, p2 ;
	VEC3D tvec ;
	RFRAME lf ;

	if( ar.IsStoring() )
	{
		ar << trs->interval.t1
		   << trs->interval.t2
		   << trs->n ;
		ar.Write(&(trs->box), sizeof(BOX3D)) ;
		type = entityGetType(trs->curve) ;
		ar << type ;
		if( type == etLine )
		{
			curveEvalTangent(trs->curve, 0., p, tvec) ;
			ar << p[0]
			   << p[1]
			   << p[2]
			   << tvec[0]
			   << tvec[1]
			   << tvec[2] ;
		}
		else
		if( type == etCircle )
		{
			curveEvalPos(trs->curve, 0., p1) ;
			curveEvalPos(trs->curve, 0.5*PI1, p) ;
			curveEvalPos(trs->curve, PI1, p2) ;
			mathMidPnt(p1, p2, lf.O) ;
			r = 0.5*mathDist(p1, p2) ;
			if( r < MIN_RAD )
				return 0 ;
			lf.X[0] = (p1[0]-lf.O[0])/r ;
			lf.X[1] = (p1[1]-lf.O[1])/r ;
			lf.X[2] = (p1[2]-lf.O[2])/r ;
			lf.Y[0] = (p[0]-lf.O[0])/r ;
			lf.Y[1] = (p[1]-lf.O[1])/r ;
			lf.Y[2] = (p[2]-lf.O[2])/r ;
			ar << r
			   << lf.O[0]
			   << lf.O[1]
			   << lf.O[2]
			   << lf.X[0]
			   << lf.X[1]
			   << lf.X[2]
			   << lf.Y[0]
			   << lf.Y[1]
			   << lf.Y[2] ;
		}
		else
			return 0 ;
	}
	else
	{
		ar >> trs->interval.t1
		   >> trs->interval.t2
		   >> trs->n ;
		ar.Read(&(trs->box), sizeof(BOX3D)) ;
		ar >> type ;
		if( type == etLine )
		{
			ar >> p[0]
			   >> p[1]
			   >> p[2]
			   >> tvec[0]
			   >> tvec[1]
			   >> tvec[2] ;
			trs->curve = lineCreate(p, tvec) ;
		}
		else
		if( type == etCircle )
		{
			ar >> r
			   >> lf.O[0]
			   >> lf.O[1]
			   >> lf.O[2]
			   >> lf.X[0]
			   >> lf.X[1]
			   >> lf.X[2]
			   >> lf.Y[0]
			   >> lf.Y[1]
			   >> lf.Y[2] ;
			mathVProduct(lf.X, lf.Y, lf.Z) ;
			lf.scale = 1. ;
			trs->curve = circleCreate(&lf, r) ;
		}
		else
			return 0 ;
	}

	return 1 ;
}
///////////////////////////////////////////////////////////////