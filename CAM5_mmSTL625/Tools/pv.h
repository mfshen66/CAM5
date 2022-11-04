/*	2002/4/7 nt
 *	the head file of preview info
 */
#ifndef _PV_h_
#define _PV_h_

///////////////////////////////////////////////////////////////
//	preview info
/*  2002/4/7 nt
 *  preview info
 */
class CPV : public CObj
{
private:
	VPCARRIER* m_plines ;
	VPCARRIER* m_facets ;
	//VPCARRIER* m_tmeshs ;

	// nt add 2015/7/16 grid for surface
	int m_nu ;
	int m_nv ;
	double (*m_ps)[3] ;

public:
	CPV() ;
	~CPV() ;	

	void AddPLine(STRPT* strpt) ;
	void AddFacet(FACET* facet) ;
	//void AddTMesh(TMESH2D* tmesh) ;
	int CreateGrid(int nu, int nv) ; // nt add 2015/7/16
	void AddGridPt(int i, PNT3D p) ; // nt add 2015/7/16
	void Draw(void* pVI, int drawMode) ; // nt modify 2003/11/20
} ;
///////////////////////////////////////////////////////////////

#endif