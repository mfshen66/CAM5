/*	2004/6/19 nt
 *	head file of class CTCell definitions
 */
#ifndef _TCell_h_
#define _TCell_h_

////////////////////////////////////////////////////////////////
//	point cloud
/*	2003/11/7 nt
 *	triangle cell
 */
class CTCell : public CObj
{
private:
	int	m_color ;
	PNT3D* m_nodes ;
	int	(*m_tris)[3] ;
	BOX3D m_box ;
public:
	CTCell() ;
	~CTCell() ;
} ;
////////////////////////////////////////////////////////////////

#endif
