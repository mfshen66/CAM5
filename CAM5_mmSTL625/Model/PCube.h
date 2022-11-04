/*	2004/6/19 nt
 *	head file of class CPCube definitions
 */
#ifndef _PCube_h_
#define _PCube_h_

////////////////////////////////////////////////////////////////
/*	2003/11/6 nt
 *	point cube
 */
class CPCube
{
public:
	int	m_n ;
	CMPoint* m_mpoints ; // pointer list of CMPoint(here use CMPoint->next)
public:
	CPCube() ;
	~CPCube() ;
} ;
////////////////////////////////////////////////////////////////

#endif