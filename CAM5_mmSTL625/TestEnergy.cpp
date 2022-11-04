#include "stdafx.h"
#include "API\AL_API.h"
#include "CAM.h"
#include "MainFrm.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "CAMDoc.h"
#include <fstream>

//==============================================================
double curveEvalEnergy22(CURVE* curve, double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2 ;
	VEC3D v ;

	curveEvalPos(curve, t-dd, p0) ;
	curveEvalPos(curve, t, p1) ;
	curveEvalPos(curve, t+dd, p2) ;
	for( k = 0 ; k < 3 ; k++ )
		v[k] = (p0[k]-2*p1[k]+p2[k]) ; // /(dd*dd) ;

	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ;
}

double curveEvalEnergy33(CURVE* curve, double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2, p3 ;
	VEC3D v ;

	curveEvalPos(curve, t-1.5*dd, p0) ;
	curveEvalPos(curve, t-0.5*dd, p1) ;
	curveEvalPos(curve, t+0.5*dd, p2) ;
	curveEvalPos(curve, t+1.5*dd, p3) ;
	for( k = 0 ; k < 3 ; k++ )
		v[k] = (p3[k]-3*p2[k]+3*p1[k]-p0[k]) ; // /(dd*dd*dd) ;

	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ;
}

double curveEvalEnergy44(CURVE* curve, double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2, p3, p4 ;
	VEC3D v ;

	curveEvalPos(curve, t-2*dd, p0) ;
	curveEvalPos(curve, t-dd, p1) ;
	curveEvalPos(curve, t, p2) ;
	curveEvalPos(curve, t+dd, p3) ;
	curveEvalPos(curve, t+2*dd, p4) ;
	//dd *= dd ;
	//dd *= dd ;
	for( k = 0 ; k < 3 ; k++ )
		v[k] = (p4[k]-4*p3[k]+6*p2[k]-4*p1[k]+p0[k]) ; // /dd ;
	
	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ;
}

double curveEvalEnergy12(CURVE* curve, double t)
{
	PNT3D p ;
	VEC3D dt, dtt ;

	curveEvalDer2O(curve, t, p, dt, dtt) ;

	return dt[0]*dtt[0]+dt[1]*dtt[1]+dt[2]*dtt[2] ;
}

double curveEvalEnergy23(CURVE* curve, double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2, p3 ;
	VEC3D dtt, dttt ;

	curveEvalPos(curve, t-dd, p0) ;
	curveEvalPos(curve, t, p1) ;
	curveEvalPos(curve, t+dd, p2) ;
	for( k = 0 ; k < 3 ; k++ )
		dtt[k] = (p0[k]-2*p1[k]+p2[k]) ; // /(dd*dd) ;

	curveEvalPos(curve, t-1.5*dd, p0) ;
	curveEvalPos(curve, t-0.5*dd, p1) ;
	curveEvalPos(curve, t+0.5*dd, p2) ;
	curveEvalPos(curve, t+1.5*dd, p3) ;
	for( k = 0 ; k < 3 ; k++ )
		dttt[k] = (p3[k]-3*p2[k]+3*p1[k]-p0[k]) ; // /(dd*dd*dd) ;

	return dtt[0]*dttt[0]+dtt[1]*dttt[1]+dtt[2]*dttt[2] ;
}
//--------------------------------------------------------------
// 1=|Pu|, 
// 2=|Puu|, 
// 3=|Puuu|, 
// 4=C, 
// 5=dC/du, C表示曲率
// 6=(duu,duuu), (,)表示矢量的点积, nt add 2012/9/20
// 7=|(duu,duuu)|, (,)表示矢量的点积, nt add 2013/7/7
/*void CCAMDoc::OnTest() 
{
	// TODO: Add your command handler code here
	if( !m_pSelSet ||
		!m_pPart )
		return ;
	camPromptStr("拾取一条或多条曲线...") ;
	int i, j, n = GetSelected(ftRefCur, 10000) ;
	if( n > 0 )
	{
		for( i = 0 ; i < n ; i++ )
		{
			CRefCur* pRefCur = (CRefCur*)GetObj(i) ;
			CURVE* curve = pRefCur->GetCurve() ;
			INTERVAL interval ;
			pRefCur->GetInterval(&interval) ;
			double d, c, t, max, s, aL ;
			PNT3D p, q ;
			VEC3D T, N, B ;
			CMark* pMark = NULL ;
			CStr name ;
			curveCalArcLen(curve, &interval, 0.001, &aL) ;
			if( entityGetType(curve) == etBCurve )
			{
				CMark* pMark = new CMark() ;
				max = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					d = curveEvalEnergy(curve, t, 2, 1e-5) ;
					if( max < d )
						max = d ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
				}
				if( max > MIN_DBL )
					pMark->Scale2(0.1*aL/max) ;
				name = "2阶导数模长" ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;

				pMark = new CMark() ;
				max = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					d = curveEvalEnergy(curve, t, 3, 1e-5) ;
					if( max < d )
						max = d ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
				}
				if( max > MIN_DBL )
					pMark->Scale2(0.1*aL/max) ;
				name = "3阶导数模长" ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;

				pMark = new CMark() ;
				max = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					d = curveEvalEnergy(curve, t, 4, 1e-4) ;
					if( max < d )
						max = d ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
				}
				if( max > MIN_DBL )
					pMark->Scale2(0.1*aL/max) ;
				name = "曲率" ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;

				pMark = new CMark() ;
				max = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					d = curveEvalEnergy(curve, t, 5, 1e-4) ;
					if( max < d )
						max = d ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
				}
				if( max > MIN_DBL )
					pMark->Scale2(0.1*aL/max) ;
				name = "曲率导数" ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;

				pMark = new CMark() ;
				max = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					d = curveEvalEnergy12(curve, t) ;
					if( max < fabs(d) )
						max = fabs(d) ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
				}
				if( max > MIN_DBL )
					pMark->Scale2(0.1*aL/max) ;
				name = "1、2阶导数点积" ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;

				pMark = new CMark() ;
				max = 0. ;
				s = 0. ;
				for( j = 0 ; j <= 1000 ; j++ )
				{
					t = interval.t1+(interval.t2-interval.t1)*j/1000. ; ;
					curveEvalCur(curve, t, p, T, N, B, &c) ;
					//d = 1.e6*curveEvalEnergy44(curve, t, 0.01) ;
					d = 1.e6*curveEvalEnergy33(curve, t, 0.01) ;
					//d = 1e20*curveEvalEnergy23(curve, t, 0.00001) ;
					//d = 1e15*curveEvalEnergy23(curve, t, 0.0001) ;
					//d = 1e11*curveEvalEnergy23(curve, t, 0.001) ;
					//d = 1e5*curveEvalEnergy23(curve, t, 0.01) ;
					//d = 1e4*curveEvalEnergy23(curve, t, 0.025) ;
					//d = 1e3*curveEvalEnergy23(curve, t, 0.05) ;
					//d = 1e2*curveEvalEnergy23(curve, t, 0.1) ;
					if( max < fabs(d) )
						max = fabs(d) ;
					mathEvalLin(p, N, d, q) ;
					pMark->AddLin(p, q) ;
					s += fabs(d) ;
				}
				//if( max > MIN_DBL )
				//	pMark->Scale2(0.1*aL/max) ;
				name.Format("2、3阶导数点积,总和=%lf", s) ;
				pMark->SetName(name) ;
				AddFeature(pMark) ;
				pMark = NULL ;
			}
		}
		Redraw() ;
	}

	return ;
}*/

void CCAMDoc::OnTest() 
{
	int n = GetSelected(ftRefCur, 1) ; // two curves
	if( n == 1 )
	{
		CRefCur* pRefCur1 = (CRefCur*)GetObj(0) ;
		CURVE* curve1 = pRefCur1->GetCurve() ;
		INTERVAL interval1 ;
		pRefCur1->GetInterval(&interval1) ;
		//CRefCur* pRefCur2 = (CRefCur*)GetObj(1) ;
		//CURVE* curve2 = pRefCur2->GetCurve() ;
		//INTERVAL interval2 ;
		//pRefCur2->GetInterval(&interval2) ;

		VEC3D normal = {0., 0., -1.} ;
		CURVE *offcur1 ; //, *offcur2;
		offcur1 = offcurCreate(curve1, normal, 0.5) ;
		//offcur2 = offcurCreate(curve2, normal, 0.01) ;
		//INTPT* intpts = NULL, *intpt ;
		//curveInt(offcur1, &interval1, offcur2, &interval2, &intpts) ;

		//for( intpt = intpts ; intpt != NULL ; intpt = intpt->next )
		//{
		//	CRefPt* pRefPt = new CRefPt(0, intpt->p) ;
		//	AddFeature(pRefPt) ;
		//}

		pRefCur1 = new CRefCur(0, offcur1, &interval1) ;
		AddFeature(pRefCur1) ;

		//pRefCur2 = new CRefCur(0, offcur2, &interval2) ;
		//AddFeature(pRefCur2) ;
	}

	return ;
}
//==============================================================

/*//==============================================================
// x = (10+t)*cos(t)
// y = (10+t)*sin(t)
void spiralEvalPos(double t, PNT3D p)
{
	p[0] = (10+t)*cos(t) ;
	p[1] = (10+t)*sin(t) ;
	p[2] = 0. ;
}

void spiralEvalDer1O(double t, PNT3D p, VEC3D dt)
{
	double c, s ;

	c = cos(t) ;
	s = sin(t) ;
	p[0] = (10+t)*c ;
	p[1] = (10+t)*s ;
	p[2] = 0. ;
	dt[0] = c-p[1] ;
	dt[1] = s+p[0] ;
	dt[2] = 0. ;
}

void spiralEvalDer2O(double t, PNT3D p, VEC3D dt, VEC3D dtt)
{
	double c, s ;

	c = cos(t) ;
	s = sin(t) ;
	p[0] = (10+t)*c ;
	p[1] = (10+t)*s ;
	p[2] = 0. ;
	dt[0] = c-p[1] ;
	dt[1] = s+p[0] ;
	dt[2] = 0. ;
	dtt[0] = -2*s-p[0] ;
	dtt[1] = 2*c-p[1] ;
	dtt[2] = 0. ;
}

int spiralEvalCur(double t,
				  PNT3D pos, 
				  VEC3D T, 
				  VEC3D N, 
				  VEC3D B,
				  double* pcur)
{
	double r, d ;

	if( !T ||
		!N ||
		!B ||
		!pcur )
		return ERUNSUC ;

	spiralEvalDer2O(t, pos, T, N) ;

	r = sqrt(T[0]*T[0]+T[1]*T[1]+T[2]*T[2]) ;
	if( r < 1.e-20 )
		return ERUNSUC ;
	mathVProduct(T, N, B) ;
	d = sqrt(B[0]*B[0]+B[1]*B[1]+B[2]*B[2]) ;
	*pcur = d/(r*r*r) ;
	T[0] /= r ;
	T[1] /= r ;
	T[2] /= r ;
	if( d < 1.e-20 )
	{
		mathGetXYFromZ(T, N, B) ;
		return ERSUCSS ;
	}
	B[0] /= d ;
	B[1] /= d ;
	B[2] /= d ;
	mathVProduct(B, T, N) ;
	mathUniVec(N, MIN_DBL) ;

	return ERSUCSS ;
}
//--------------------------------------------------------------
double spiralEvalEnergy22(double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2 ;
	VEC3D v ;

	spiralEvalPos(t-dd, p0) ;
	spiralEvalPos(t, p1) ;
	spiralEvalPos(t+dd, p2) ;
	for( k = 0 ; k < 3 ; k++ )
		v[k] = (p0[k]-2*p1[k]+p2[k]) ;

	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ;
}

double spiralEvalEnergy33(double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2, p3 ;
	VEC3D v ;

	spiralEvalPos(t-1.5*dd, p0) ;
	spiralEvalPos( t-0.5*dd, p1) ;
	spiralEvalPos(t+0.5*dd, p2) ;
	spiralEvalPos(t+1.5*dd, p3) ;
	for( k = 0 ; k < 3 ; k++ )
		v[k] = (p3[k]-3*p2[k]+3*p1[k]-p0[k]) ;

	//v[0] = -3*cos(t)+(10+t)*sin(t) ;
	//v[1] = -3*sin(t)-(10+t)*cos(t) ;
	//v[2] = 0. ;

	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ;
}

double spiralEvalEnergy23(double t, double dd)
{
	int k ;
	PNT3D p0, p1, p2, p3 ;
	VEC3D dtt, dttt ;

	spiralEvalPos(t-dd, p0) ;
	spiralEvalPos(t, p1) ;
	spiralEvalPos(t+dd, p2) ;
	for( k = 0 ; k < 3 ; k++ )
		dtt[k] = (p0[k]-2*p1[k]+p2[k]) ;

	spiralEvalPos(t-1.5*dd, p0) ;
	spiralEvalPos(t-0.5*dd, p1) ;
	spiralEvalPos(t+0.5*dd, p2) ;
	spiralEvalPos(t+1.5*dd, p3) ;
	for( k = 0 ; k < 3 ; k++ )
		dttt[k] = (p3[k]-3*p2[k]+3*p1[k]-p0[k]) ;

	return dtt[0]*dttt[0]+dtt[1]*dttt[1]+dtt[2]*dttt[2] ;
}
//--------------------------------------------------------------
void CCAMDoc::OnTest()
{
	int i ;
	CStr name ;
	double t1 = 0., t2 = 20., t, c, d, max, aL = 0. ;
	PNT3D p, q ;
	VEC3D T, N, B ;
	CMark* pMark = NULL ;
	
	pMark = new CMark() ;
	spiralEvalPos(t1, p) ;
	for( i = 1 ; i <= 1000 ; i++ )
	{
		t = t1+(t2-t1)*i/1000. ;
		spiralEvalPos(t, q) ;
		aL += mathDist(p, q) ;
		pMark->AddLin(p, q) ;
		memcpy(p, q, sizeof(PNT3D)) ;
	}
	name = "spiral" ;
	pMark->SetName(name) ;
	AddFeature(pMark) ;
	pMark = NULL ;

	pMark = new CMark() ;
	max = 0. ;
	for( i = 0 ; i <= 1000 ; i++ )
	{
		t = t1+(t2-t1)*i/1000. ;
		spiralEvalCur(t, p, T, N, B, &c) ;
		d = spiralEvalEnergy22(t, 1e-3) ;
		if( max < d )
			max = d ;
		mathEvalLin(p, N, d, q) ;
		pMark->AddLin(p, q) ;
	}
	if( max > MIN_DBL )
		pMark->Scale2(0.01*aL/max) ;
	name = "2阶导数模长" ;
	pMark->SetName(name) ;
	AddFeature(pMark) ;
	pMark = NULL ;

	pMark = new CMark() ;
	max = 0. ;
	for( i = 0 ; i <= 1000 ; i++ )
	{
		t = t1+(t2-t1)*i/1000. ;
		spiralEvalCur(t, p, T, N, B, &c) ;
		//d = 1e15*spiralEvalEnergy33(t, 1e-3) ;
		d = 1e10*spiralEvalEnergy33(t, 1e-2) ;
		if( max < d )
			max = d ;
		mathEvalLin(p, N, d, q) ;
		pMark->AddLin(p, q) ;
	}
	if( max > MIN_DBL )
		pMark->Scale2(0.01*aL/max) ;
	name = "3阶导数模长" ;
	pMark->SetName(name) ;
	AddFeature(pMark) ;
	pMark = NULL ;

	pMark = new CMark() ;
	max = 0. ;
	for( i = 0 ; i <= 1000 ; i++ )
	{
		t = t1+(t2-t1)*i/1000. ;
		spiralEvalCur(t, p, T, N, B, &c) ;
		//d = 1e15*spiralEvalEnergy33(t, 1e-3) ;
		d = 1e10*spiralEvalEnergy23(t, 1e-2) ;
		if( max < d )
			max = d ;
		mathEvalLin(p, N, d, q) ;
		pMark->AddLin(p, q) ;
	}
	if( max > MIN_DBL )
		pMark->Scale2(0.01*aL/max) ;
	name = "2、3阶导内积" ;
	pMark->SetName(name) ;
	AddFeature(pMark) ;
	pMark = NULL ;
}
//==============================================================*/