/*	2004/6/19 nt
 *	head file of class CMPoint definitions
 */
#ifndef _MPoint_h_
#define _MPoint_h_

////////////////////////////////////////////////////////////////
//	point cloud
#define MPOINT 65 // nt modify 2010/8/24, old is 1000

/*	2003/11/6 nt
 *	measured point
 */
class CMPoint : public CVO // nt add 2010/4/22
{
public:
	CMPoint* m_next ;
	CMPoint* m_link ; // use only in selecting
	PNT3D m_p ;
	VEC3D m_normal ;
	double m_k1 ;
	double m_k2 ;
	VEC3D m_v ;
	int	m_attrib ;
public:
	CMPoint() ;
	CMPoint(PNT3D p) ;
	~CMPoint() ;

	int GetType() ; // nt add 2010/4/22
	int	Draw(void* pVI, int drawMode) ;
	int	Draw2(void* pVI, int drawMode) ; // nt add 2010/4/23
} ;
////////////////////////////////////////////////////////////////

#endif