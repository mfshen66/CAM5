/*	2001/11/24 nt
 *	the head file of dynamic navigation information structure
 *	针对3D曲线的导航
 */
#ifndef _DNAVI_h_
#define _DNAVI_h_

///////////////////////////////////////////////////////////////
//	dynamic navigation
//  nt modify 2009/5/9 : 目的是简化3D导航结构, 不再考虑在两个图元之间导航,
//  复杂的导航放在草图的导航中实现。简化以后，这个数据结构就很清晰了。
/*  2001/11/24 nt
 *  navigation type
 *	nt modify 2009/5/9
 */
enum EDNaviType
{
	dnGrid = 0,
	dnOn = 1, // on curve
	dnMiddle = 2, // middle point
	dnCenter = 3, // center point
	dnEnd = 4, // end point
	dnRefPt = 5// free point
} ;

/*  2001/11/24 nt
 *  dynamic navigation information
 *	nt modify 2009/5/9
 */
typedef struct _dnavi DNAVI ;
struct _dnavi
{
	double		pos[3] ; // mouse point, 坐标值定义在世界坐标系
	double		dir[3] ; // view dir, 坐标值定义在世界坐标系
	double		r ; // navigate radius

	int			type ; // navigation type
	int			featType ; // feature type
	void*		feat ; // feature which determined the navigation, such as CRefCur and CRefPt
	double		nav_p[3] ; // navigated point, 坐标值定义在世界坐标系
} ;

/*	2001/11/24 nt
 *	DNAVI.cpp
 */
int dnaviInit(DNAVI* dn, double pos[3], double dir[3], double r) ;
///////////////////////////////////////////////////////////////

#endif
