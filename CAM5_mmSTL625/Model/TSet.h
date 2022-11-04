/*	2004/6/19 nt
 *	head file of TSet definitions
 */
#ifndef _TSet_h_
#define _TSet_h_

///////////////////////////////////////////////////////////////
/*	2003/11/7 nt
 *	triangle set
 */
class CTSet
{
private:
	COA	m_tcells ;
	BOX3D m_box ;
public:
	CTSet() ;
	~CTSet() ;
} ;
////////////////////////////////////////////////////////////////

#endif