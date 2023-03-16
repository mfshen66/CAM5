/*	2009/11/25 nt
 *	implementation file of line and circle(arc) correction
 */
#include "StdAfx.h"

///////////////////////////////////////////////////////////////
/*	1992/4/1 nt
 *	return the distance from p1 to p2
 */
double math2dDist(PNT2D p1, PNT2D p2)
{
	return sqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])) ;
}

/*	2001/9/9 nt
 *	middle point
 */
void math2dMidPnt(PNT2D p1, PNT2D p2, PNT2D mid_p)
{
	mid_p[0] = 0.5*(p1[0]+p2[0]) ;
	mid_p[1] = 0.5*(p1[1]+p2[1]) ;

	return ;
}

/*	1992 nt
 *	mix two point
 */
void math2dMixPnt(PNT2D p1, PNT2D p2, double w1, double w2, PNT2D mix_p)
{
	mix_p[0] = w1*p1[0]+w2*p2[0] ;
	mix_p[1] = w1*p1[1]+w2*p2[1] ;

	return ;
}

/*  1997/12/13 nt
 *  旋转2D点
 */
void math2dRotPnt(PNT2D pivot, double angle, PNT2D p, PNT2D rot_p)
{
	double cosRotateAngle, sinRotateAngle ;
	VEC2D e1 ;

	e1[0] = p[0]-pivot[0] ;
	e1[1] = p[1]-pivot[1] ;
	cosRotateAngle = cos(angle) ;
	sinRotateAngle = sin(angle) ;

	rot_p[0] = pivot[0]+(cosRotateAngle*e1[0]-sinRotateAngle*e1[1]) ;
	rot_p[1] = pivot[1]+(cosRotateAngle*e1[1]+sinRotateAngle*e1[0]) ;

	return ;
}

/*	1992 nt
 *	mirror a point
 */
void math2dMirPnt(PNT2D pivot, VEC2D dir, PNT2D p, PNT2D mir_p)
{
	double d ;
	PNT2D mid_p ;

	d = dir[0]*(p[0]-pivot[0])+dir[1]*(p[1]-pivot[1]) ;
	mid_p[0] = pivot[0]+d*dir[0] ;
	mid_p[1] = pivot[1]+d*dir[1] ;
	mir_p[0] = 2.0*mid_p[0]-p[0] ;
	mir_p[1] = 2.0*mid_p[1]-p[1] ;

	return ;
}

/*	1992/4/1 nt
 *	unify the vector 2d
 */
int math2dUniVec(VEC2D v, double min_len)
{
	double len ;

	len = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( len < min_len )
	{
		v[0] = 0.0 ;
		v[1] = 0.0 ;
		return ERUNSUC ;
	}
	else
	{
		v[0] /= len ;
		v[1] /= len ;
		return ERSUCSS ;
	}
}

/*	1992/4/1 nt
 *	return the length of a vector 2d
 */
double math2dVecLen(VEC2D v)
{
	return sqrt(v[0]*v[0]+v[1]*v[1]) ;
}

/*	1992/4/1 nt
 *	return the dot product of two vector 2d
 */
double math2dOProduct(VEC2D vec1, VEC2D vec2)
{
	return vec1[0]*vec2[0]+vec1[1]*vec2[1] ;
}

/*	1992/4/1 nt
 *	return the dot product of two vector 2d
 */
double math2dOProduct2(VEC2D v, PNT2D p1, PNT2D p2)
{
	return v[0]*(p2[0]-p2[0])+v[1]*(p2[1]-p1[1]) ;
}

/*	2002/5/13 nt
 *	reverse dir
 */
void math2dRevVec(VEC2D v)
{
	v[0] = -v[0] ;
	v[1] = -v[1] ;
}

/*	1992/4/1 nt
 *	rotate a vector 2d
 */
void math2dRotVec(double angle, VEC2D v, VEC2D rot_v)
{
	double localX, localY ;
	VEC2D temp ;

	localX = cos(angle) ;
	localY = sin(angle) ;
	temp[0] = localX*v[0]-localY*v[1] ;
	temp[1] = localX*v[1]+localY*v[0] ;
	rot_v[0] = temp[0] ;
	rot_v[1] = temp[1] ;

	return ;
}

/*  1997/2/4 nt
 *  计算两个2D向量的夹角: 0.0 <= angle <= PI1
 */
double math2dGetAngle(VEC2D v1, VEC2D v2, double min_len)
{
	double d1, d2 ;

	d1 = sqrt(v1[0]*v1[0]+v1[1]*v1[1]) ;
	d2 = sqrt(v2[0]*v2[0]+v2[1]*v2[1]) ;
	if( d1 < min_len ||
		d2 < min_len )
		return -1.0 ;
	else
		return acos((v1[0]*v2[0]+v1[1]*v2[1])/(d1*d2)) ;
}

/*  1997/2/5 nt
 *  计算两个2D单位向量的夹角: 0.0 <= angle <= PI1
 */
double math2dGetAngleUnit(VEC2D v1, VEC2D v2)
{
	return acos(v1[0]*v2[0]+v1[1]*v2[1]) ;
}

/*  1997/2/14 nt
 *  2D单位向量与X轴正向按逆时针方向夹角:0.0 <= angle <= PI_2
 *  注意返回0.0和PI_2意义是一样的
 */
double math2dGetAngleX(VEC2D v) // ??? reference to Hatch.dll
{
	double angle ;

	angle = atan2(v[1], v[0]) ;
	if( angle < 0.0 )
		angle += PI2 ;

	return angle ;
}

/*	2002/3/7 nt
 *	get a vector which is defined by begin point and end point
 */
void math2dGetVec(PNT2D p1, PNT2D p2, VEC2D v)
{
	v[0] = p2[0]-p1[0] ;
	v[1] = p2[1]-p1[1] ;

	return ;
}

/*	2009/9/22 nt
 *	v1按逆时针方向转到v2的角度，0.<=a<2PI
 *	v1和v2都是单位向量
 */
double math2dGetRAngleACW(VEC2D v1, VEC2D v2)
{
	double a1 = math2dGetAngleX(v1) ;
	double a2 = math2dGetAngleX(v2) ;
	if( a2 >= a1 )
		return a2-a1 ;
	else
		return a2+PI2-a1 ;
}

int math2dGetVecUnit(PNT2D p1, PNT2D p2, double tol, VEC2D v)
{
	double d ;
	v[0] = p2[0]-p1[0] ;
	v[1] = p2[1]-p1[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d > tol )
	{
		v[0] /= d ;
		v[1] /= d ;
		return ERSUCSS ;
	}
	else
		return ERUNSUC ;
}

/*  1995/1 nt
 *  计算2D线段上的一点
 */
void math2dEvalLin(PNT2D begin, VEC2D dir, double u, PNT2D p)
{
	p[0] = begin[0]+dir[0]*u ;
	p[1] = begin[1]+dir[1]*u ;

	return ;
}

/*	1999/7/3 nt
 *	distance from point to line
 *	if point on left of the line, then return positive distance,
 *	otherwise negative
 */
double math2dDistPntLin(PNT2D p, PNT2D begin, VEC2D dir)
{ // the left direction of line is (-dir[1], dir[0])
	return -dir[1]*(p[0]-begin[0])+dir[0]*(p[1]-begin[1]) ;
}

/*	1999/7/3 nt
 *	project point to line
 */
void math2dPrjPntLin(PNT2D p, PNT2D begin, VEC2D dir, PNT2D prj_p)
{
	double d ;

	d = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1] ;
	prj_p[0] = begin[0]+d*dir[0] ;
	prj_p[1] = begin[1]+d*dir[1] ;

	return ;
}

void math2dPrjPntLinEx(PNT2D p, PNT2D begin, PNT2D end, PNT2D prj_p)
{
	double d ;
	VEC2D dir ;

	dir[0] = end[0]-begin[0] ;
	dir[1] = end[1]-begin[1] ;
	//prj_p[0] = begin[0] ; // nt modify 2019/12/14
	//prj_p[1] = begin[1] ; // nt modify 2019/12/14
	if( math2dUniVec(dir, MIN_DBL) == ERSUCSS )
	{
		d = (p[0]-begin[0])*dir[0]+(p[1]-begin[1])*dir[1] ;
		prj_p[0] = begin[0]+d*dir[0] ;
		prj_p[1] = begin[1]+d*dir[1] ;
	}

	return ;
}

/*  1997/2/4 nt
*  判断点在有向2D直线上(IDON),左侧(IDLEFT),还是右厕(IDRIGHT)
*/
int math2dChkPntLin(PNT2D p, PNT2D begin, PNT2D end, double tol)
{
	double d ;
	VEC2D v ;

	v[0] = end[0]-begin[0] ;
	v[1] = end[1]-begin[1] ;
	if( (d = sqrt(v[0]*v[0]+v[1]*v[1])) < MIN_DBL )
		return IDDEGE ;
	d = (v[1]*(begin[0]-p[0])+v[0]*(p[1]-begin[1]))/d ;
	if( fabs(d) < tol )
		return IDON ;

	return d > 0.0 ? IDLEFT : IDRIGHT ;
}

/*  1995/1 nt
 *  计算两条2D直线的交点
 *	返回IDPARAL,平行;IDINT,交于一点;IDOVERL,重合
 */
int math2dIntLin(PNT2D begin1,
				 VEC2D dir1,
				 PNT2D begin2,
				 VEC2D dir2,
				 double tol,
				 double ang,
				 PNT2D intpt)
{
	double d1, d2, d ;

	d1 = begin1[0]*dir1[1]-begin1[1]*dir1[0] ;
	d2 = begin2[0]*dir2[1]-begin2[1]*dir2[0] ;
	d = dir1[0]*dir2[1]-dir1[1]*dir2[0] ;
	if( fabs(d) < ang ) // here apply: sin(x) = x, when x is very small
	{
		d = begin2[0]*dir1[1]-begin2[1]*dir1[0]-d1 ;
		return fabs(d) < tol ? IDOVERL : IDPARAL ; // overlap, parallel
	}
	else
	{
		intpt[0] = (d2*dir1[0]-d1*dir2[0])/d ;
		intpt[1] = (d2*dir1[1]-d1*dir2[1])/d ;
		return IDINT ; // intersect
	}
}

/*	1999/7/3 nt
 *	计算线段的包围盒
 */
void math2dGetBoxSegm(PNT2D p1, PNT2D p2, PNT2D min, PNT2D max)
{
	if( p1[0] < p2[0] )
	{
		min[0] = p1[0] ;
		max[0] = p2[0] ;
	}
	else
	{
		min[0] = p2[0] ;
		max[0] = p1[0] ;
	}

	if( p1[1] < p2[1] )
	{
		min[1] = p1[1] ;
		max[1] = p2[1] ;
	}
	else
	{
		min[1] = p2[1] ;
		max[1] = p1[1] ;
	}

	return ;
}

/*	1997 nt
 *	check if a point is on the line segment
 *	返回IDIN，IDOUT
 */
int math2dChkPntSegm(PNT2D p, PNT2D begin, PNT2D end, double tol)
{
	VEC2D v1, v2 ;
	double A, B, C, t ;

	if( mathMIN(begin[0], end[0]) > p[0] + tol || // chk box intersect
		mathMAX(begin[0], end[0]) + tol < p[0] ||
		mathMIN(begin[1], end[1]) > p[1] + tol ||
		mathMAX(begin[1], end[1]) + tol < p[1] )
		return IDOUT ;

	v1[0] = end[0]-begin[0] ;
	v1[1] = end[1]-begin[1] ;
	v2[0] = begin[0]-p[0] ;
	v2[1] = begin[1]-p[1] ;
	A = v1[0]*v1[0]+v1[1]*v1[1] ;
	B = v1[0]*v2[0]+v1[1]*v2[1] ;
	C = v2[0]*v2[0]+v2[1]*v2[1] ;
	if( A > 1.e-20 )
	{
		t = - B/A ;
		if( t > 1.0 )
			C += (A+2.0*B) ;
		else
			if( t > 0.0 )
				C += B*t ;
	}

	if( sqrt(C) < tol )
		return IDIN ;
	else
		return IDOUT ;
}

/*	1995/3/18 nt
 *	BOX2D
 */
BOX2D* math2dGetBox()
{
	BOX2D* box ;

	box = (BOX2D*)malloc(sizeof(BOX2D)) ;
	if( !box )
		return NULL ;

	box->min[0] =  MAX_DBL ;
	box->min[1] =  MAX_DBL ;
	box->max[0] = -MAX_DBL ;
	box->max[1] = -MAX_DBL ;

	return box ;
}

/*
 *	enlarge a box by a point
 */
void math2dEnlargeBox(PNT2D p, BOX2D* box)
{
	int k ;

	for( k = 0 ; k < 2 ; k++ )
	{
		if( box->max[k] < p[k] )
			box->max[k] = p[k] ;
		if( box->min[k] > p[k] )
			box->min[k] = p[k] ;
	}
}

/*
 *	initialize a box
 */
void math2dIniBox(PNT2D min, PNT2D max, BOX2D* box)
{
	int k ;

	for( k = 0 ; k < 2 ; k++ )
	{
		box->min[k] = min[k] ;
		box->max[k] = max[k] ;
	}
}

/*
 *	initialize a box
 */
void math2dClnBox(BOX2D* box)
{
	box->min[0] =  MAX_DBL ;
	box->min[1] =  MAX_DBL ;
	box->max[0] = -MAX_DBL ;
	box->max[1] = -MAX_DBL ;
}

/*
 *	check if tow box intersect
 *	return IDNOINT: no intersection, IDINT: have intersect part
 */
int math2dChkBoxInt(BOX2D* box1, BOX2D* box2, double tol)
{
	if( box1->min[0] > box2->max[0]+tol ||
		box1->max[0] < box2->min[0]-tol ||
		box1->min[1] > box2->max[1]+tol ||
		box1->max[1] < box2->min[1]-tol )
		return IDNOINT ;

	return IDINT ;
}

/*
 *	check if a point is in the box : return IDIN: in, IDOUT: out
 */
int math2dChkBoxPnt(BOX2D* box, PNT2D p, double tol)
{
	if( box->min[0] > p[0]+tol ||
		box->max[0]+tol < p[0] ||
		box->min[1] > p[1]+tol ||
		box->max[1]+tol < p[1] )
		return IDOUT ;

	return IDIN ;
}

/*
 *	calculate the box of union
 */
void math2dCalBoxUnion(BOX2D* box1, BOX2D* box2, BOX2D* box)
{
	box->min[0] = mathMIN(box1->min[0], box2->min[0]) ;
	box->min[1] = mathMIN(box1->min[1], box2->min[1]) ;
	box->max[0] = mathMAX(box1->max[0], box2->max[0]) ;
	box->max[1] = mathMAX(box1->max[1], box2->max[1]) ;
}

/*  1995/8/27 nt
 *  transform local_point 2D which is defined in local_frame to
 *	world_point 2D 
 */
void math2dTransLocalPnt(RFRM2D* local_frame,
						 PNT2D local_point,
						 PNT2D world_point)
{
	int k ;
	PNT2D p ;

	for( k = 0 ; k < 2 ; k++ )
		p[k] = local_point[0]*local_frame->X[k] +
		local_point[1]*local_frame->Y[k] +
		local_frame->O[k] ;
	world_point[0] = p[0] ;
	world_point[1] = p[1] ;

	return ;
}

/*  1995/8/27 nt
 *  transform world_point 2D to local_point 3D which is defined in
 *	local_frame
 */
void math2dTransWorldPnt(RFRM2D* local_frame,
						 PNT2D world_point,
						 PNT2D local_point)
{
	VEC2D trans_point ;

	trans_point[0] = world_point[0]-local_frame->O[0] ;
	trans_point[1] = world_point[1]-local_frame->O[1] ;

	local_point[0] = trans_point[0]*local_frame->X[0] +
		trans_point[1]*local_frame->X[1] ;
	local_point[1] = trans_point[0]*local_frame->Y[0] +
		trans_point[1]*local_frame->Y[1] ;

	return ;
}

//	vector
/*  1995/8/27 nt
 *  transform local_vector 3D which is defined in local_frame to
 *	world vector 3D
 */
void math2dTransLocalVec(RFRM2D* local_frame,
						 VEC2D local_vector,
						 VEC2D world_vector)
{
	VEC2D v ;

	v[0] = local_vector[0]*local_frame->X[0]+local_vector[1]*local_frame->Y[0] ;
	v[1] = local_vector[0]*local_frame->X[1]+local_vector[1]*local_frame->Y[1] ;
	world_vector[0] = v[0] ;
	world_vector[1] = v[1] ;

	return ;
}

/*  1995/8/27 nt
 *  transform world_vector 3D to local_vector 3D which is defined
 *	in local_frame
 */
void math2dTransWorldVec(RFRM2D* local_frame,
						 VEC2D world_vector,
						 VEC2D local_vector)
{
	VEC2D v ;

	v[0] = mathOProduct2D(local_frame->X, world_vector) ;
	v[1] = mathOProduct2D(local_frame->Y, world_vector) ;
	local_vector[0] = v[0] ;
	local_vector[1] = v[1] ;

	return ;
}

double mathClampPeriodic(double b, double e, double t)
{
	double periodic ;

	periodic = e-b ;
	if( periodic < MIN_DBL )
		return 0.5*(b+e) ;

	if( t < b ||
		t > e )
		return t-floor((t-b)/periodic)*periodic ;
	else
		return t ;
}

int intervalCover(double t1, 
				  double t2, 
				  double pe,
				  double tol,
				  int flag, 
				  INTERVAL* interval)
{
	double u1, u2, min, max ;

	if( pe > MIN_DBL ) // periodic
	{
		u1 = mathClampPeriodic(0.0, pe, t1) ; // clamp t1 in[0.0, pe]
		u2 = mathClampPeriodic(0.0, pe, t2) ; // clamp t2 in[0.0, pe]
		if( fabs(u1-u2) < tol ) // not handle this case
			return ERUNSUC ;
		if( u1 < u2 )
		{
			min = u2-pe ;
			max = u2 ;
		}
		else
		{
			min = u2 ;
			max = u2+pe ;
		}
		if( flag > 0 )
		{
			interval->t1 = u1 ;
			interval->t2 = max ;
		}
		else
		if( flag < 0 )
		{
			interval->t1 = min ;
			interval->t2 = u1 ;
		}
		else
		{
			interval->t1 = u1 < 0.5*(min+max) ? min : u1 ;
			interval->t2 = u1 < 0.5*(min+max) ? u1 : max ;
		}
	}
	else // open
	{
		interval->t1 = mathMIN(t1, t2) ;
		interval->t2 = mathMAX(t1, t2) ;
	}

	return ERSUCSS ;
}

/*  2002/8/30 nt
 *	obtain a arc which is passing through P0, P1 and P2
 *	nt modify 2003/1/7
 */
int math2dCalCRAAEx1(PNT2D p0,
					 PNT2D p1,
					 PNT2D p2,
					 double tol,
					 double ang,
					 PNT2D center,
					 double* pr,
					 double* pa0,
					 double* pa2)
{
	int id ;
	double r0, r1, r2 ;
	PNT2D mid_p0, mid_p2 ;
	VEC2D dir0, dir2 ;
	INTERVAL interval ;

	if( !p0 || 
		!p1 || 
		!p2 || 
		!center ||
		!pr )
		return ERUNSUC ;

	dir0[0] = p0[1]-p1[1] ; // perpendicular to p0p1
	dir0[1] = p1[0]-p0[0] ;
	dir2[0] = p1[1]-p2[1] ; // perpendicular to p1p2
	dir2[1] = p2[0]-p1[0] ;
	math2dMidPnt(p0, p1, mid_p0) ;
	math2dMidPnt(p1, p2, mid_p2) ;
	if( math2dUniVec(dir0, MIN_DBL) != ERSUCSS ||
		math2dUniVec(dir2, MIN_DBL) != ERSUCSS ||
		math2dIntLin(mid_p0,
					 dir0,
					 mid_p2,
					 dir2,
					 tol,
					 ang,
					 center) != IDINT )
		return ERDEGE ;
	r0 = math2dDist(p0, center) ;
	r1 = math2dDist(p1, center) ;
	r2 = math2dDist(p2, center) ;
	*pr = (r0+r1+r2)/3 ;
	if( (*pr) < tol ||
		fabs(r0-(*pr)) > tol ||
		fabs(r1-(*pr)) > tol ||
		fabs(r2-(*pr)) > tol )
		return ERUNSUC ;

	if( !pa0 || 
		!pa2 )
		return ERSUCSS ;
	dir0[0] = (p0[0]-center[0])/r0 ;
	dir0[1] = (p0[1]-center[1])/r0 ;
	dir2[0] = (p2[0]-center[0])/r2 ;
	dir2[1] = (p2[1]-center[1])/r2 ;
	*pa0 = math2dGetAngleX(dir0) ;
	*pa2 = math2dGetAngleX(dir2) ;
	id = math2dChkPntLin(p1, p0, p2, tol) ;
	if( id == IDON )
		return ERUNSUC ;
	if( id == IDLEFT )
		id = -1 ;
	else
		id = 1 ;
	if( intervalCover(*pa0, *pa2, PI2, tol*(*pr), id, &interval) != ERSUCSS )
		return ERUNSUC ;
	*pa0 = interval.t1 ;
	*pa2 = interval.t2 ;

	return ERSUCSS ;
}
//=============================================================
//	修正线段或圆弧使与圆相切 
// line L=(p, q)
// circle C=(center, r)
// 修正q, 使L与圆相切
// return 0:error, 1:corrected, 2:not correct
// 不寻求最优（意义不大）
int pcCorLin(PNT2D p, 
			 PNT2D center, 
			 double r, 
			 double tol, 
			 PNT2D q)
{
	int k ;
	double a, d ;
	VEC2D v, dir ;

	v[0] = center[0]-p[0] ;
	v[1] = center[1]-p[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d < r+MIN_LEN )
		return 2 ;
	v[0] /= d ;
	v[1] /= d ;
	a = asin(r/d) ;
	for( k = -1 ; k < 2 ; k += 2 )
	{
		math2dRotVec(k*a, v, dir) ;
		d = math2dDistPntLin(q, p, dir) ;
		if( fabs(d) < tol )
		{
			math2dPrjPntLin(center, p, dir, q) ;
			return 1 ;
		}
	}

	return 2 ;
}

// nt add 2009/11/24
// line L=(q1, q2)
// circle C1=(center1, r1)
// circle C2=(center2, r2)
// 只要q1,q2在C1和C2的切线附近
// 就把q1,q2赋值成切点
// 不寻求最优（意义不大）
// return 0:error, 1:corrected, 2:not correct
int ccCorLin(PNT2D center1,
			 double r1,
			 PNT2D center2,
			 double r2,
			 double tol,
			 PNT2D q1,
			 PNT2D q2)
{
	int k ;
	double d, d1, d2, w, a ;
	VEC2D v, dir, normal ;
	PNT2D p ;

	v[0] = center2[0]-center1[0] ;
	v[1] = center2[1]-center1[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d < MIN_LEN )
		return 2 ;
	v[0] /= d ;
	v[1] /= d ;

	w = r1/(r1+r2) ;
	if( r1+r2+MIN_LEN < d )
	{
		p[0] = center1[0]*(1-w)+center2[0]*w ;
		p[1] = center1[1]*(1-w)+center2[1]*w ;
		d1 = d*w ;
		d2 = d*(1-w) ;
		if( d1 > r1+MIN_LEN &&
			d2 > r2+MIN_LEN )
		{
			a = asin(r2/d2) ;
			for( k = -1 ; k < 2 ; k += 2 )
			{
				math2dRotVec(k*a, v, dir) ;
				d1 = math2dDistPntLin(q1, p, dir) ;
				d2 = math2dDistPntLin(q2, p, dir) ;
				if( fabs(d1) < tol &&
					fabs(d2) < tol )
				{
					math2dPrjPntLin(center1, p, dir, q1) ;
					math2dPrjPntLin(center2, p, dir, q2) ;
					return 1 ;
				}
			}
		}
	}

	if( fabs(r2-r1)+MIN_LEN < d )
	{
		a = asin(fabs(r2-r1)/d) ;
		if( r1 > r2 )
		{
			v[0] = -v[0] ;
			v[1] = -v[1] ;
		}
		for( k = -1 ; k < 2 ; k += 2 )
		{
			math2dRotVec(k*a, v, dir) ;	
			math2dRotVec(k*(a+PI1_2), v, normal) ;
			math2dEvalLin(center1, normal, r1, p) ;
			d1 = math2dDistPntLin(q1, p, dir) ;
			d2 = math2dDistPntLin(q2, p, dir) ;
			if( fabs(d1) < tol &&
				fabs(d2) < tol )
			{
				memcpy(q1, p, sizeof(PNT2D)) ;
				math2dEvalLin(center2, normal, r2, q2) ;
				return 1 ;
			}
		}
	}

	return 2 ;
}
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// 2009-11-25 nt
/*  2009/11/26 nt
 *  计算两条2D直线的交点
 *	返回0=平行
 *      1=交于一点
 *		2=重合
 */
int llInt2D(PNT2D b1,
		    VEC2D dir1,
		    PNT2D b2,
		    VEC2D dir2,
		    double tol,
		    double ang,
		    PNT2D p)
{
   	double d1, d2, t1, t2 ;
	PNT2D p1, p2 ;

	d1 = -(b1[0]-b2[0])*dir2[1]+(b1[1]-b2[1])*dir2[0] ;
	d2 = d1-dir1[0]*dir2[1]+dir1[1]*dir2[0] ;
	if( fabs(d2-d1) < ang ) // parallel
	{
		t1 = 0.5*((b2[0]-b1[0])*dir1[0]+(b2[1]-b1[1])*dir1[1]) ;
		t2 = 0.5*((b1[0]-b2[0])*dir2[0]+(b1[1]-b2[1])*dir2[1]) ;
		p1[0] = b1[0]+dir1[0]*t1 ;
		p1[1] = b1[1]+dir1[1]*t1 ;
		p2[0] = b2[0]+dir2[0]*t2 ;
		p2[1] = b2[1]+dir2[1]*t2 ;
		if( math2dDist(p1, p2) < tol )
		{
			p[0] = 0.5*(p1[0]+p2[0]) ;
			p[1] = 0.5*(p1[1]+p2[1]) ;
			return 2 ;
		}
		else
			return 0 ;
	}
	else
	{
		t1 = d1/(d1-d2) ;
		p[0] = b1[0]+dir1[0]*t1 ;
		p[1] = b1[1]+dir1[1]*t1 ;
		return 1 ;
	}
}

// return value:
// -1=error
// 0=no intersection
// 1=tangent contact
// 2=intersect
int lcInt2D(PNT2D b,
		    VEC2D dir,
		    PNT2D c,
		    double r,
		    double tol,
		    PNT2D p1,
		    PNT2D p2)
{
	double d ;
	PNT2D p ;

	math2dPrjPntLin(c, b, dir, p) ;
	d = math2dDist(p, c) ;
	if( d > r+tol )
		return 0 ;
	else
	if( d > r-tol )
	{
		memcpy(p1, p, sizeof(PNT2D)) ;
		memcpy(p2, p, sizeof(PNT2D)) ;
		return 1 ;
	}
	else
	{
		d = sqrt(fabs(r*r-d*d)) ;
		math2dEvalLin(p, dir, -d, p1) ;
		math2dEvalLin(p, dir, d, p2) ;
		return 2 ;
	}
}

// return value:
// -2=error
// -1=tangent contact(内切)
// 0=no intersection
// 1=tangent contact
// 2=intersect
// 3=overlap
int ccInt2D(PNT2D c1, 
		    double r1,
		    PNT2D c2,
		    double r2,
		    double tol,
		    PNT2D p1,
		    PNT2D p2)
{
	double d, d2, h ;
	PNT2D p ;
	VEC2D v ;
	v[0] = c2[0]-c1[0] ;
	v[1] = c2[1]-c1[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d < tol )
	{
		if( fabs(r1+d-r2) < tol &&
			fabs(r1-d-r2) < tol )
			return 3 ;
		else
			return 0 ;
	}
	else
	{
		v[0] /= d ;
		v[1] /= d ;
		if( d > r1+r2+tol ) // 相离
			return 0 ;
		else
		if( r1+d+tol < r2 || // 内含
			r2+d+tol < r1 )
			return 0 ;
		else
		if( fabs(r1+r2-d) < tol ) // 外切
		{
			math2dEvalLin(c1, v, r1, p1) ;
			math2dEvalLin(c2, v, -r2, p2) ;
			return 1 ;
		}
		else
		if( fabs(r1+d-r2) < tol ) // 内切
		{
			math2dEvalLin(c1, v, -r1, p1) ;
			math2dEvalLin(c2, v, -r2, p2) ;
			return -1 ;
		}
		else
		if( fabs(r2+d-r1) < tol ) // 内切
		{
			math2dEvalLin(c1, v, r1, p1) ;
			math2dEvalLin(c2, v, r2, p2) ;
			return -1 ;
		}
		else // 相交
		{
			// d1+d2=d,               (1) 
			// d1*d1+h*h=r1*r1        (2)
			// d2*d2+h*h=r2*r2        (3)
			d2 = (d+(r2*r2-r1*r1)/d)/2 ;
			h = sqrt(fabs(r2*r2-d2*d2)) ;
			math2dEvalLin(c1, v, d-d2, p) ;
			p1[0] = p[0]-v[1]*h ;
			p1[1] = p[1]+v[0]*h ;
			p2[0] = p[0]+v[1]*h ;
			p2[1] = p[1]-v[0]*h ;
			return 2 ;
		}
	}
}
//--------------------------------------------------------------
// nt add 2009/11/26
// calculate radius of circumcircle
int triCalCR2D(PNT2D p1, PNT2D p2, PNT2D p3, double* pr)
{
	double d ;
	PNT2D b1, b2, p ;
	VEC2D v1, v2 ;
	*pr = 0. ;
	math2dMidPnt(p1, p3, b1) ;
	math2dMidPnt(p2, p3, b2) ;
	if( math2dGetVecUnit(p3, p1, MIN_LEN, v1) == ERSUCSS &&
		math2dGetVecUnit(p3, p2, MIN_LEN, v2) == ERSUCSS )
	{
		d = v1[0] ;
		v1[0] = -v1[1] ;
		v1[1] = d ;
		d = v2[0] ;
		v2[0] = -v2[1] ;
		v2[1] = d ;
		if( llInt2D(b1, 
					v1,
					b2,
					v2,
					MIN_LEN,
					MIN_ANG,
					p) == 1 )
		{
			*pr = math2dDist(p, p3) ;
			return 1 ;
		}
		else
			return 0 ;
	}
	else
		return 0 ;
}

// 在圆上找一点q离p最近，用这点q取代p,
// 注意要求p不在圆心处
int toCir2D(PNT2D c, double r, PNT2D p)
{
	double d ;
	VEC2D v ;
	v[0] = p[0]-c[0] ;
	v[1] = p[1]-c[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d > MIN_LEN )
	{
		v[0] /= d ;
		v[1] /= d ;
		math2dEvalLin(c, v, r, p) ;
		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2009/11/26
int ccGetTP(PNT2D c1, double r1, PNT2D c2, double r2, PNT2D tp)
{
	double d, d1, d2 ;
	PNT2D p1, p2 ;
	VEC2D v ;
	
	d = math2dDist(c1, c2) ;
	if( d < MIN_LEN )
		return 0 ;
	v[0] = (c2[0]-c1[0])/d ;
	v[1] = (c2[1]-c1[1])/d ;
	p1[0] = c1[0]+r1*v[0] ; // 可能切点之一
	p1[1] = c1[1]+r1*v[1] ;
	p2[0] = c1[0]-r1*v[0] ; // 可能切点之二
	p2[1] = c1[1]-r1*v[1] ;
	d1 = fabs(mathDist2D(p1, c2)-r2) ;
	d2 = fabs(mathDist2D(p2, c2)-r2) ;
	if( d1 < d2 )
	{
		memcpy(tp, p1, sizeof(PNT2D)) ;
		if( d1 < MIN_LEN )
			return 1 ;
	}
	else
	{
		memcpy(tp, p2, sizeof(PNT2D)) ;
		if( d2 < MIN_LEN )
			return 1 ;
	}

	return 2 ;
}
//--------------------------------------------------------------
// nt add 2009/11/26
// line1=(b1, dir1)
// line2=(b2, dir2)
// arc=(p1,p2,p3)
// 在误差范围内用line1和line2对arc进行校正，
// 使之与line1和line2相切
// nt modify 2010/9/14
// nt modify 2011/10/26 : add cc and pr
int llCorArc(PNT2D b1, 
			 VEC2D dir1,
			 PNT2D b2,
			 VEC2D dir2,
			 double tol,
			 PNT2D p1,
			 PNT2D p2,
			 PNT2D p3,
			 PNT2D cc,
			 double* pr)
{
	int k ;
	double a, d, r, d1, d2, d3 ;
	PNT2D c, o ;
	VEC2D v, v1, v2 ;

	// nt add 2010/9/14, 先处理特殊情况
	if( fabs(math2dDistPntLin(p1, b1, dir1)) < tol &&
		fabs(math2dDistPntLin(p3, b2, dir2)) < tol )
	{
		v1[0] = -dir1[1] ;
		v1[1] = dir1[0] ;
		v2[0] = -dir2[1] ;
		v2[1] = dir2[0] ;
		if( math2dIntLin(p1, 
						 v1, 
						 p3,
						 v2,
						 MIN_LEN,
						 MIN_ANG,
						 c) == IDINT )
		{
			d1 = math2dDist(p1, c) ;
			d2 = math2dDist(p2, c) ;
			d3 = math2dDist(p3, c) ;
			r = (d1+d2+d3)/3 ;
			if( fabs(d1-r) < tol &&
				fabs(d2-r) < tol &&
				fabs(d3-r) < tol )
			{
				memcpy(o, p2, sizeof(PNT2D)) ; // nt add 2011/10/25
				if( toCir2D(c, r, o) == 1 )
				{
					memcpy(p2, o, sizeof(PNT2D)) ;
					math2dPrjPntLin(c, b1, dir1, p1) ;
					math2dPrjPntLin(c, b2, dir2, p3) ;
					if( cc ) // nt add 2011/10/26
						memcpy(cc, c, sizeof(PNT2D)) ;
					if( pr ) // nt add 2011/10/26
						*pr = r ;
					return 1 ;
				}
			}
		}
	}
	if( fabs(math2dDistPntLin(p3, b1, dir1)) < tol &&
		fabs(math2dDistPntLin(p1, b2, dir2)) < tol )
	{
		v1[0] = -dir1[1] ;
		v1[1] = dir1[0] ;
		v2[0] = -dir2[1] ;
		v2[1] = dir2[0] ;
		if( math2dIntLin(p3, 
						 v1, 
						 p1,
						 v2,
						 MIN_LEN,
						 MIN_ANG,
						 c) == IDINT )
		{
			d1 = math2dDist(p1, c) ;
			d2 = math2dDist(p2, c) ;
			d3 = math2dDist(p3, c) ;
			r = (d1+d2+d3)/3 ;
			if( fabs(d1-r) < tol &&
				fabs(d2-r) < tol &&
				fabs(d3-r) < tol )
			{
				memcpy(o, p2, sizeof(PNT2D)) ; // nt add 2011/10/25
				if( toCir2D(c, r, o) == 1 )
				{
					memcpy(p2, o, sizeof(PNT2D)) ;
					math2dPrjPntLin(c, b1, dir1, p3) ;
					math2dPrjPntLin(c, b2, dir2, p1) ;
					if( cc ) // nt add 2011/10/26
						memcpy(cc, c, sizeof(PNT2D)) ;
					if( pr ) // nt add 2011/10/26
						*pr = r ;
					return 1 ;
				}
			}
		}
	}

	// 只要得到r
	if( triCalCR2D(p1, p2, p3, &r) != 1 ||
		math2dIntLin(b1, 
					 dir1,
					 b2,
					 dir2,
					 MIN_LEN, 
					 MIN_ANG,
					 o) != IDINT )
		return 2 ;

	a = math2dGetAngleUnit(dir1, dir2) ;
	if( a < MIN_ANG ||
		a > PI1-MIN_ANG )
		return 2 ;
	a *= 0.5 ;
	v[0] = dir1[0]+dir2[0] ;
	v[1] = dir1[1]+dir2[1] ;
	d = sqrt(v[0]*v[0]+v[1]*v[1]) ;
	if( d < MIN_DBL )
		return 2 ;
	v[0] /= d ;
	v[1] /= d ;
	
	for( k = 0 ; k < 4 ; k++ )
	{
		d = r/sin(a) ;
		math2dEvalLin(o, v, d, c) ;
		// 检查是否基本重合
		if( fabs(math2dDist(p1, c)-r) < tol &&
			fabs(math2dDist(p2, c)-r) < tol &&
			fabs(math2dDist(p3, c)-r) < tol )
		{
			math2dPrjPntLin(c, b1, dir1, p1) ;
			if( toCir2D(c, r, p2) != 1 )
				return 2 ;
			math2dPrjPntLin(c, b2, dir2, p3) ;
			if( cc ) // nt add 2011/10/26
				memcpy(cc, c, sizeof(PNT2D)) ;
			if( pr ) // nt add 2011/10/26
				*pr = r ;
			return 1 ;
		}
		a = PI1_2-a ;
		d = v[0] ; // 相当于逆时针转90度
		v[0] = -v[1] ;
		v[1] = d ;
	}

	return 2 ;
}

// nt add 2009/11/26
// nt modify 2011/10/26 : add cc and pr
int lcCorArc(PNT2D b, 
			 VEC2D dir,
			 PNT2D c,
			 double R,
			 double tol,
			 PNT2D p1,
			 PNT2D p2,
			 PNT2D p3,
			 PNT2D cc,
			 double* pr)
{
	int i, j, k, n = 0, rt ;
	double r, r1, r2, r3, rr ;
	PNT2D p, q1, q2, q3, cs[8] ;

	// 只要得到r
	if( triCalCR2D(p1, p2, p3, &r) != 1 )
		return 2 ;
	for( i = -1 ; i < 2 ; i += 2 )
	{
		p[0] = b[0]-dir[1]*r*i ; // 平移r或-r
		p[1] = b[1]+dir[0]*r*i ;
		for( j = -1 ; j < 2 ; j += 2 )
		{
			rt = lcInt2D(p, // 确定公切圆的圆心: c
					     dir,
					     c,
					     fabs(R+r*j),
					     MIN_LEN, 
					     cs[n],
					     cs[n+1]) ;
			if( rt == 1 )
				n++ ;
			else
			if( rt == 2 )
				n += 2 ;
		}
	}

	for( k = 0 ; k < n ; k++ )
	{
		// 检查是否基本重合
		r1 = math2dDist(p1, cs[k]) ;
		r2 = math2dDist(p2, cs[k]) ;
		r3 = math2dDist(p3, cs[k]) ;
		if( fabs(r1-r) < tol &&
			fabs(r2-r) < tol &&
			fabs(r3-r) < tol )
		{
			memcpy(q1, p1, sizeof(PNT2D)) ;
			memcpy(q2, p2, sizeof(PNT2D)) ;
			memcpy(q3, p3, sizeof(PNT2D)) ;
			math2dPrjPntLin(cs[k], b, dir, q1) ;
			if( toCir2D(cs[k], r, q2) != 1 ||
				ccGetTP(cs[k], r, c, R, q3) != 1 )
				return 0 ;
			r1 = math2dDist(q1, cs[k]) ;
			r2 = math2dDist(q2, cs[k]) ;
			r3 = math2dDist(q3, cs[k]) ;
			rr = math2dDist(c, q3) ;
			if( fabs(r1-r) < MIN_LEN &&
				fabs(r2-r) < MIN_LEN &&
				fabs(r3-r) < MIN_LEN &&
				fabs(rr-R) < MIN_LEN )
			{
				memcpy(p1, q1, sizeof(PNT2D)) ;
				memcpy(p2, q2, sizeof(PNT2D)) ;
				memcpy(p3, q3, sizeof(PNT2D)) ;
				if( cc ) // nt add 2011/10/26
					memcpy(cc, cs[k], sizeof(PNT2D)) ;
				if( pr ) // nt add 2011/10/26
					*pr = r ;
				return 1 ;
			}
		}
	}

	return 2 ;
}

// correct Arc=(p1, p2, p3)
// circle C1=(c1, r1)
// circle C2=(c2, r2)
// p1在C1上（允许有一定的误差）
// p2在C2上（允许有一定的误差）
// nt modify 2011/10/26 : add cc and pr
int ccCorArc(PNT2D c1, 
			 double r1,
			 PNT2D c2,
			 double r2,
			 double tol,
			 PNT2D p1,
			 PNT2D p2,
			 PNT2D p3,
			 PNT2D cc,
			 double* pr)
{
	int i, j, k, n, rt ;
	double r ;
	PNT2D cs[8] ;

	// 只要得到r, 注意下面得到的圆cs[k]的半径都是r
	if( triCalCR2D(p1, p2, p3, &r) != 1 )
		return 2 ;
	// 得到最多8个相切圆
	n = 0 ;
	for( i = -1 ; i < 2 ; i += 2 )
	{
		for( j = -1 ; j < 2 ; j += 2 )
		{
			rt = ccInt2D(c1, 
					     fabs(r1+i*r),
					     c2,
					     fabs(r2+j*r),
					     MIN_LEN,
					     cs[n],
					     cs[n+1]) ;
			if( rt == -1 ||
				rt == 1 )
				n++ ;
			else
			if( rt == 2 )
				n += 2 ;
		}
	}

	for( k = 0 ; k < n ; k++ )
	{
		if( fabs(math2dDist(p1, cs[k])-r) < tol &&
			//fabs(math2dDist(p2, cs[k])-r) < tol &&
			fabs(math2dDist(p3, cs[k])-r) < tol )
		{
			if( ccGetTP(c1, r1, cs[k], r, p1) != 1 ||
				toCir2D(cs[k], r, p2) != 1 ||
				ccGetTP(c2, r2, cs[k], r, p3) != 1 )
				return 2 ;
			if( cc ) // nt add 2011/10/26
				memcpy(cc, cs[k], sizeof(PNT2D)) ;
			if( pr ) // nt add 2011/10/26
				*pr = r ;
			return 1 ;
		}
	}

	return 2 ;
}

//--------------------------------------------------------------
// smf add 2023/02/21
// 共面线段的交点计算
int mathSegmIntSegmInFace(PNT3D iBegin1, PNT3D iEnd1, PNT3D iBegin2, PNT3D iEnd2, double iTol, PNT3D oIntPoint)
{
	VEC3D v1, v2, v_b1_b2;
	for (size_t i = 0; i < 3; i++)
	{
		v1[i] = iEnd1[i] - iBegin1[i];
		v2[i] = iEnd2[i] - iBegin2[i];
		v_b1_b2[i] = iBegin2[i] - iBegin1[i];
	}

	double det_xy, det_yz, det_xz;

	det_xy = mathDet22(v1[0], -v2[0], v1[1], -v2[1]);
	det_yz = mathDet22(v1[1], -v2[1], v1[2], -v2[2]);
	det_xz = mathDet22(v1[0], -v2[0], v1[2], -v2[2]);

	double t1 = -1000, t2 = -1000;
	if (det_xy < -iTol ||
		det_xy > iTol)
	{
		double det_1 = mathDet22(v_b1_b2[0], -v2[0], v_b1_b2[1], -v2[1]);
		t1 = det_1 / det_xy;
		double det_2 = mathDet22(v1[0], v_b1_b2[0], v1[1], v_b1_b2[1]);
		t2 = det_2 / det_xy;
	}
	else
		if (det_yz < -iTol ||
			det_yz > iTol)
		{
			double det_1 = mathDet22(v_b1_b2[1], -v2[1], v_b1_b2[2], -v2[2]);
			t1 = det_1 / det_yz;
			double det_2 = mathDet22(v1[1], v_b1_b2[1], v1[2], v_b1_b2[2]);
			t2 = det_2 / det_yz;
		}
		else
			if (det_xz < -iTol ||
				det_xz > iTol)
			{
				double det_1 = mathDet22(v_b1_b2[0], -v2[0], v_b1_b2[2], -v2[2]);
				t1 = det_1 / det_xz;
				double det_2 = mathDet22(v1[0], v_b1_b2[0], v1[2], v_b1_b2[2]);
				t2 = det_2 / det_xz;
			}

	if (t1 >= 0. && t1 <= 1. &&
		t2 >= 0. && t2 <= 1.) // 交点在线段上
	{
		for (int i = 0; i < 3; i++)
		{
			oIntPoint[i] = iBegin1[i] + t1 * v1[i];
		}
		return IDINT;
	}
	return IDNOINT;
}

// A的行列式，|A|
double mathDet22(double a11, double a12, double a21, double a22)
{
	double det = a11 * a22 - a12 * a21;
	return det;
}
///////////////////////////////////////////////////////////////

