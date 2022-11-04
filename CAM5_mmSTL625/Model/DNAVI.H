/*	2001/11/24 nt
 *	the head file of dynamic navigation information structure
 *	���3D���ߵĵ���
 */
#ifndef _DNAVI_h_
#define _DNAVI_h_

///////////////////////////////////////////////////////////////
//	dynamic navigation
//  nt modify 2009/5/9 : Ŀ���Ǽ�3D�����ṹ, ���ٿ���������ͼԪ֮�䵼��,
//  ���ӵĵ������ڲ�ͼ�ĵ�����ʵ�֡����Ժ�������ݽṹ�ͺ������ˡ�
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
	double		pos[3] ; // mouse point, ����ֵ��������������ϵ
	double		dir[3] ; // view dir, ����ֵ��������������ϵ
	double		r ; // navigate radius

	int			type ; // navigation type
	int			featType ; // feature type
	void*		feat ; // feature which determined the navigation, such as CRefCur and CRefPt
	double		nav_p[3] ; // navigated point, ����ֵ��������������ϵ
} ;

/*	2001/11/24 nt
 *	DNAVI.cpp
 */
int dnaviInit(DNAVI* dn, double pos[3], double dir[3], double r) ;
///////////////////////////////////////////////////////////////

#endif