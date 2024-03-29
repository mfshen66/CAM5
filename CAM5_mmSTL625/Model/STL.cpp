#include "StdAfx.h"
#include "STL.h"
#include "STLOpt.h"
#include <vector>

using namespace std;
//==============================================================
/*	1997/4/22 nt
 *	在2D平面上判断原点是否在一个三角形内
 *	return IDIN: in ; IDOUT: not in; IDDEGE, degenerate
 */
int mathChkOriInTri2D(double p1[2],
					  double p2[2],
					  double p3[2],
					  double tol)
{
	int ks[3], i = 0 ;
	double xs[6] ; 

	if( (p1[0] >  tol && p2[0] >  tol && p3[0] >  tol) ||
		(p1[0] < -tol && p2[0] < -tol && p3[0] < -tol) ||
		(p1[1] >  tol && p2[1] >  tol && p3[1] >  tol) ||
		(p1[1] < -tol && p2[1] < -tol && p3[1] < -tol) )
		return IDOUT ;

	ks[0] = fabs(p1[1]) < tol ? 0 : p1[1] > 0.0 ? 1 : -1 ;
	ks[1] = fabs(p2[1]) < tol ? 0 : p2[1] > 0.0 ? 1 : -1 ;
	ks[2] = fabs(p3[1]) < tol ? 0 : p3[1] > 0.0 ? 1 : -1 ;
	if( ks[0] == 0 ) xs[i++] = p1[0] ;
	if( ks[1] == 0 ) xs[i++] = p2[0] ;
	if( ks[2] == 0 ) xs[i++] = p3[0] ;
	if( i >= 3 )
		return IDDEGE ;
	if( ks[0] * ks[1] < 0 )
		xs[i++] = p1[0]+p1[1]*(p2[0]-p1[0])/(p1[1]-p2[1]) ;
	if( ks[1] * ks[2] < 0 )
		xs[i++] = p2[0]+p2[1]*(p3[0]-p2[0])/(p2[1]-p3[1]) ;
	if( ks[2] * ks[0] < 0 )
		xs[i++] = p3[0]+p3[1]*(p1[0]-p3[0])/(p3[1]-p1[1]) ;
	
	if( i == 2 )
	{
		if( xs[0]*xs[1] < 0.0 ||
			fabs(xs[0]) < tol ||
			fabs(xs[1]) < tol )
			return IDIN ;
	}

	return IDOUT ;
}

/*	1997/5/5 nt
 *	计算一个点相对于2D三角形的重心坐标
 */
int mathCalTri2DGCP(PNT2D p1,
					PNT2D p2,
					PNT2D p3,
					PNT2D p,
					PNT3D gc)
{
	double a11, a12, a21, a22, delta ;

	a11 = p1[0]-p3[0] ;
	a12 = p2[0]-p3[0] ;
	a21 = p1[1]-p3[1] ;
	a22 = p2[1]-p3[1] ;
	if( fabs(delta = a11*a22-a12*a21) < 1e-20 )
		return ERUNSUC ;
	gc[0] = ((p3[1]-p[1])*a12-(p3[0]-p[0])*a22)/delta ;
	gc[1] = ((p3[0]-p[0])*a21-(p3[1]-p[1])*a11)/delta ;
	gc[2] = 1.0-gc[0]-gc[1] ;

	return ERSUCSS ;
}

/*	2016/8 nt
 *  将p投影到三角形上，再按二维情况计算重心坐标
 */
int mathCalTriGCPX(PNT3D p1, PNT3D p2, PNT3D p3, VEC3D normal, PNT3D p, PNT3D gc)
{
	PNT2D q1, q2, q3, q ;
	PNT3D O ;
	VEC3D v, X, Y ;

	O[0] = (p1[0] + p2[0] + p3[0]) / 3;
	O[1] = (p1[1] + p2[1] + p3[1]) / 3;
	O[2] = (p1[2] + p2[2] + p3[2]) / 3;
	mathGetXYFromZ(normal, X, Y);

	mathGetVec(O, p1, v);
	q1[0] = mathOProduct(X, v); // to local coordinate frame lf
	q1[1] = mathOProduct(Y, v);

	mathGetVec(O, p2, v);
	q2[0] = mathOProduct(X, v); // to local coordinate frame lf
	q2[1] = mathOProduct(Y, v);

	mathGetVec(O, p3, v);
	q3[0] = mathOProduct(X, v); // to local coordinate frame lf
	q3[1] = mathOProduct(Y, v);

	mathGetVec(O, p, v);
	q[0] = mathOProduct(X, v); // to local coordinate frame lf
	q[1] = mathOProduct(Y, v);

	mathCalTri2DGCP(q1, q2, q3, q, gc);

	return ERSUCSS;
}

/*  1997/2/8 nt
 *	计算点与线段的最近距离和与该点最近的线段上点的参数,参数在0.0,1.0之间
 */
double mathGetPntSegmNDT(PNT3D p, PNT3D begin, PNT3D end, double* pt)
{
    VEC3D v1, v2 ;
    double A, B, C ;

    *pt = 0.0 ;
    v1[0] = end[0]-begin[0] ;
	v1[1] = end[1]-begin[1] ;
	v1[2] = end[2]-begin[2] ;
    v2[0] = begin[0]-p[0] ;
	v2[1] = begin[1]-p[1] ; 
	v2[2] = begin[2]-p[2] ;
    A = v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2] ;
    B = v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] ;
    C = v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2] ;
    if( A > 1e-20 )
    {
        pt[0] = - B/A ;
        if( pt[0] > 1.0 )
        {
			C += (A+2.0*B) ; 
			pt[0] = 1.0 ; 
		}
        else
        if( pt[0] > 0.0 )
			C += B*pt[0] ;
        else
            pt[0] = 0.0 ;
    }

    return sqrt(C) ;
}

// 线段：from begin to end
// *perp = 1 : 最近点与p的连线垂直与线段; *perp = 0, 表示不垂直
// 是否垂直的判断依据：垂足是否在线段上，容差为tol
double mathGetPntSegmNDTPerp(PNT3D p, PNT3D begin, PNT3D end, double tol, double* pt, int* perp)
{
    VEC3D v1, v2 ;
    double A, B, C ;

    *pt = 0. ;
	*perp = 1 ;
    v1[0] = end[0]-begin[0] ;
	v1[1] = end[1]-begin[1] ;
	v1[2] = end[2]-begin[2] ;
    v2[0] = begin[0]-p[0] ;
	v2[1] = begin[1]-p[1] ; 
	v2[2] = begin[2]-p[2] ;
    A = v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2] ;
    B = v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] ;
    C = v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2] ;
    if( A > 1e-20 )
    {
        pt[0] = - B/A ;
        if( pt[0] > 1. )
        {
			C += (A+2.*B) ;
			if( (pt[0]-1.)*sqrt(A) > tol )
				*perp = 0 ;
			pt[0] = 1.0 ;
		}
        else
        if( pt[0] >= 0. )
			C += B*pt[0] ;
        else // pt[0] < 0.
		{
			if( -pt[0]*sqrt(A) > tol )
				*perp = 0 ;
            pt[0] = 0. ;
		}
    }

    return sqrt(C) ;
}

int mathChkPntSegmX(PNT3D p, PNT3D begin, PNT3D end, double tol)
{
    VEC3D v1, v2 ;
    double A, B, C, t ;

    if( mathMIN(begin[0], end[0]) > p[0] + tol || //chk box intersection
        mathMAX(begin[0], end[0]) + tol < p[0] ||
        mathMIN(begin[1], end[1]) > p[1] + tol ||
        mathMAX(begin[1], end[1]) + tol < p[1] ||
        mathMIN(begin[2], end[2]) > p[2] + tol ||
        mathMAX(begin[2], end[2]) + tol < p[2] )
        return IDOUT ;

    v1[0] = end[0]-begin[0] ;
	v1[1] = end[1]-begin[1] ;
	v1[2] = end[2]-begin[2] ;
    v2[0] = begin[0]-p[0] ;
	v2[1] = begin[1]-p[1] ;
	v2[2] = begin[2]-p[2] ;
    A = v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2] ;
    B = v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2] ;
    C = v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2] ;
    if( A > 1e-20 )
    {
        t = -B/A ;
        if( t > 1.0 )
			C += (A+2.0*B) ;
        else
        if( t > 0.0 )
			C += B*t ;
    }

	C = sqrt(fabs(C)) ;
    if( C < tol )
		return IDIN ;
	else
		return IDOUT ;
}

int mathGetTriNormal(PNT3D p1, PNT3D p2, PNT3D p3, VEC3D normal)
{
    int k ;
    VEC3D v1, v2 ;

    for( k = 0 ; k < 3 ; k++ )
    {
        v1[k] = p2[k]-p1[k] ;
        v2[k] = p3[k]-p2[k] ;
    }
    mathVProduct(v1, v2, normal) ;
    if( mathUniVec(normal, 1e-50) != ERSUCSS )
        return ERUNSUC ;

    return ERSUCSS ;
}
//==============================================================
Pog::Pog(int Type, void* Vef, double P[3])
{
	type = Type ;
	vef = Vef ;
	memcpy(p, P, sizeof(double[3])) ;
}

Pog::~Pog()
{

}
//==============================================================
Pl::Pl()
{
	arcLen = 0. ;
}

Pl::~Pl()
{
	int i, n = pogs.GetSize() ;

	for( i = 0 ; i < n ; i++ )
		delete pogs[i] ;
	pogs.RemoveAll() ;

	return ;
}

int Pl::CalArcLen()
{
	int i, n = pogs.GetSize() ;

	if( n < 2 )
		return 0 ; // error
	arcLen = 0. ;

	for( i = 1 ; i < n ; i++ )
		arcLen += mathDist(((POG*)pogs[i-1])->p, ((POG*)pogs[i])->p) ;
	
	return 1 ;
}

int Pl::GetNumPOGType(int type)
{
	int i, n = pogs.GetSize(), num = 0;
	for (i = 0; i < n; i++)
	{
		if (((POG*)pogs[i])->type == type)
			num++;
	}

	return num;
}

FList GetFaceOfTwoEdge(EList e1, EList e2)
{
	FRelated r1, r2 ;

	for( r1 = e1->FaceUsed ; r1 ; r1 = r1->FNext )
	{
		for( r2 = e2->FaceUsed ; r2 ; r2 = r2->FNext )
			if( r1->Face == r2->Face )
				return r1->Face ;
	}

	return NULL ;
}

int Pl::Opt(double tol, double eps)
{
	int i, n = pogs.GetSize(), dL ;
	double* A, *B, v[3], d, *p1, *p2, q1[3], q2[3], h1, h2, h, q[3], oldAL ;
	POG* pog ;
	EList e ;

	CalArcLen() ;
	for( dL = 0 ; dL < 1000 ; dL++ )
	{
		oldAL = arcLen ;
		for( i = 1 ; i < n-1 ; i++ ) // for inner node
		{
			pog = (POG*)pogs[i] ;
			if( ((POG*)pogs[i-1])->type == 1 && // on edge
				((POG*)pogs[i+1])->type == 1 && // on edge
				GetFaceOfTwoEdge((EList)(((POG*)pogs[i-1])->vef), 
								 (EList)(((POG*)pogs[i+1])->vef)) )
			{
				pogs.RemoveAt(i) ;
				delete pog ;
				n-- ;
				i-- ;
			}
			else
			if( pog->type == 1 )
			{
				e = (EList)pog->vef ;
				A = &e->VertexUsed[0]->Coord.x ;
				B = &e->VertexUsed[1]->Coord.x ;
				mathGetVec(A, B, v) ; // v = B-A
				d = mathVecLen(v) ;
				if( d > tol )
				{
					v[0] /= d ;
					v[1] /= d ;
					v[2] /= d ;
					p1 = ((POG*)pogs[i-1])->p ;
					p2 = ((POG*)pogs[i+1])->p ;
					mathPrjPntLin(p1, A, v, q1) ;
					mathPrjPntLin(p2, A, v, q2) ;
					h1 = mathDist(p1, q1) ;
					h2 = mathDist(p2, q2) ;
					if( (h = h1+h2) < 1e-10 )
						mathMidPnt(q1, q2, q) ;
					else
						mathMixPnt(q1, q2, h2/h, h1/h, q) ;
					h = (q[0]-A[0])*v[0]+(q[1]-A[1])*v[1]+(q[2]-A[2])*v[2] ;
					if( h < tol ) // A
					{
						memcpy(pog->p, A, sizeof(double[3])) ;
						pog->vef = e->VertexUsed[0] ;
						pog->type = 0 ;
					}
					else
					if( h > d-tol ) // B
					{
						memcpy(pog->p, B, sizeof(double[3])) ;
						pog->vef = e->VertexUsed[1] ;
						pog->type = 0 ;
					}
					else // on AB
						memcpy(pog->p, q, sizeof(double[3])) ;
				}
			}
		}
		CalArcLen() ;
		if( fabs(arcLen-oldAL) < eps )
			break ;
	}

	return 1 ;
}


double rand01()
{
	return (double)rand() / RAND_MAX;
}
//--------------------------------------------------------------
// structure for passing user-supplied data to the objective function and its Jacobian
typedef struct _lmd LMD;
#define ONFACE 0
#define ONEDGE 1
#define ONVERT 2
//
//				[  ] [ E] [  ] [ E] [ E] [  ] [  ]
// idxPnt2Para	[-1] [ 0] [-1] [ 1] [ 2] [-1] [-1]
// idxPara2Pnt	     [ 1]      [ 3] [ 4]
struct _lmd
{
	int numPnt;			// num of point
	int m;				// num of param need to opt
	int n;				// num of function (= numPnt -1)
	int* type;			// type of POG,					length = numPnt
	double(*ps)[3];		// point array which to opt,	length = numPnt
	int* idxPnt2Para;	//								length = numPnt
	double(*pEdge)[6];	// [V00 V01 V02 V10 V11 V12],	length = m
	int* idxPara2Pnt;	// 								length = m
};

// Create lmd from POGs
LMD* lmdCreate(COA* pogs)
{
	if (!pogs)
		return NULL;
	// malloc
	LMD* lmd = (LMD*)malloc(sizeof(LMD));
	if (lmd == NULL)
		return NULL;
	memset(lmd, 0, sizeof(LMD));

	// init
	int numPnt = pogs->GetSize();
	if (numPnt > 0) {
		lmd->numPnt = numPnt;
		lmd->type = (int*)malloc(numPnt * sizeof(int));
		lmd->ps = (double(*)[3])malloc(numPnt * sizeof(double[3]));
		lmd->idxPnt2Para = (int*)malloc(numPnt * sizeof(int));
		lmd->m = 0;
		for (int i = 0; i < numPnt; i++)
			if (((POG*)(*pogs)[i])->type == ONEDGE)
				lmd->m++;
		if (lmd->m) {
			lmd->pEdge = (double(*)[6])malloc(lmd->m * sizeof(double[6]));
			lmd->idxPara2Pnt = (int*)malloc(lmd->m * sizeof(int));
		}
		// set points
		POG* pog;
		EList e;
		double* A = NULL, * B = NULL;
		int idxEdgePOG = 0;
		for (int i = 0; i < numPnt; i++)
		{
			pog = (POG*)(*pogs)[i];
			lmd->type[i] = pog->type;						// type
			memcpy(lmd->ps[i], pog->p, 3 * sizeof(double));	// ps
			lmd->idxPnt2Para[i] = -1;
			if (pog->type == ONEDGE) {
				lmd->idxPnt2Para[i] = idxEdgePOG;
				e = (EList)pog->vef;
				A = &e->VertexUsed[0]->Coord.x;
				B = &e->VertexUsed[1]->Coord.x;
				memcpy(lmd->pEdge[idxEdgePOG], A, 3 * sizeof(double));
				memcpy(lmd->pEdge[idxEdgePOG] + 3, B, 3 * sizeof(double));
				lmd->idxPara2Pnt[idxEdgePOG] = i;
				idxEdgePOG++;
			}
		}
		// cal n
		lmd->n = numPnt - 1;
	}

	return lmd;
}
// Free LMD
void lmdFree(LMD* lmd)
{
	if (lmd)
	{
		if (lmd->type) delete[] lmd->type;
		if (lmd->ps) delete[] lmd->ps;
		if (lmd->pEdge) delete[] lmd->pEdge;
		if (lmd->idxPara2Pnt) delete[] lmd->idxPara2Pnt;
		delete lmd;
		lmd = NULL;
	}

	return;
}

// Init p based LMD
void lmdInitP(LMD* lmd, double* p)
{
	if (!lmd || !p)
		return;

	VEC3D vAB, vAP;
	double* A = NULL, * B = NULL;
	for (int i = 0; i < lmd->m; i++)
	{
		A = lmd->pEdge[i];
		B = lmd->pEdge[i] + 3;
		mathGetVec(A, B, vAB);
		mathGetVec(A, lmd->ps[lmd->idxPara2Pnt[i]], vAP);
		p[i] = sqrt(mathOProduct(vAP, vAP)) / sqrt(mathOProduct(vAB, vAB));
		//p[i] = rand01();
	}
	return;
}

// Update edge pnt depend on vector P
void lmdUpdatePnt(LMD* lmd, double*p)
{
	if (lmd) {
		for (int i = 0; i < lmd->m; i++)
			// mathVecAdd(1 - lmd->p[i], lmd->pEdge[i], lmd->p[i], lmd->pEdge[i] + 3, lmd->ps[lmd->idxPara2Pnt[i]]);
			mathVecAdd(1 - p[i], lmd->pEdge[i], p[i], lmd->pEdge[i] + 3, lmd->ps[lmd->idxPara2Pnt[i]]);
	}
}

// Cal sum-of-squares
double lmdCalSquareSum(LMD* lmd)
{
	double dist = 0.0;
	if (!lmd)
		return dist;
	// lmdUpdatePnt(lmd);
	for (int i = 0; i < lmd->numPnt - 1; i++) {
		dist += mathDist(lmd->ps[i], lmd->ps[i + 1]);
	}

	return dist;
}

void func(double* p, double* x, int m, int n, void* data)
{
	// set t
	register int i;
	LMD* lmd = (LMD*)data;
	lmdUpdatePnt(lmd, p);
	// double newGDAL = lmdCalSquareSum(lmd);
	for (i = 0; i < lmd->n; i++)
		x[i] = sqrt(mathDist(lmd->ps[i], lmd->ps[i + 1]));
		// x[i] = mathDist(lmd->ps[i], lmd->ps[i + 1]);

	return;
}

void df(double* p, double* jac, int m, int n, void* data)
{
	register int i, j;
	LMD* lmd = (LMD*)data;
	lmdUpdatePnt(lmd, p);
	double jaco = 0.0, norm2;
	PNT3D pnt;
	VEC3D vec;
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			// Cal i-st segment from i-st & (i+1)-st point
			jaco = 0.0;
			norm2 = 0.0;
			if (lmd->idxPnt2Para[i] == j) {
				memcpy(pnt, lmd->ps[i + 1], 3 * sizeof(double));		// pnt = P
				mathGetVec(pnt, lmd->ps[i], pnt);						// pnt = V_1 + x_j*(V_2-V_1) - P
				mathGetVec(lmd->pEdge[j], lmd->pEdge[j] + 3, vec);		// vec = V_2-V_1
				// mathGetVec(lmd->ps[i], lmd->ps[i] + 3, vec);			// vec = V_2-V_1
				norm2 = sqrt(mathOProduct(pnt, pnt));
				jaco = mathOProduct(pnt, vec) / norm2 / sqrt(norm2) / 2.0;
				// jaco = mathOProduct(pnt, vec) / norm2 / 2.0;
			}
			if (lmd->idxPnt2Para[i+1] == j) {
				memcpy(pnt, lmd->ps[i], 3 * sizeof(double));			// pnt = P
				mathGetVec(pnt, lmd->ps[i + 1], pnt);					// pnt = V_1 + x_j*(V_2-V_1) - P
				mathGetVec(lmd->pEdge[j], lmd->pEdge[j] + 3, vec);		// vec = V_2-V_1
				// mathGetVec(lmd->ps[i + 1], lmd->ps[i + 1] + 3, vec);	// vec = V_2-V_1
				norm2 = sqrt(mathOProduct(pnt, pnt));
				jaco = mathOProduct(pnt, vec) / norm2 / sqrt(norm2) / 2.0;
				// jaco = mathOProduct(pnt, vec) / norm2 / 2.0;
			}
			jac[i * m + j] = jaco;
		}
	}

	return;
}

int Pl::OptLM(double tol, int iMax)
{
	// Old arcLen
	CalArcLen();
	double oldAL = arcLen;

	LMD* lmd = lmdCreate(&pogs);
	int m = lmd->m;
	int n = lmd->n;
	double* p = (double*)malloc(lmd->m * sizeof(double));
	double* lb = (double*)malloc(lmd->m * sizeof(double));
	double* ub = (double*)malloc(lmd->m * sizeof(double));
	double* x = (double*)malloc(lmd->n * sizeof(double));
	double* opts = (double*)malloc(5 * sizeof(double));
	double* info = (double*)malloc(10 * sizeof(double));

	// Init P
	lmdInitP(lmd, p);
	// Init lb&ub
	for (int i = 0; i < m; i++) {
		lb[i] = 0.0; ub[i] = 1.0;
	}
	// Init X
	for (int i = 0; i < n; i++)
		x[i] = 0.0;

	// set optimization control parameters
	opts[0] = LM_INIT_MU;	// 1.e-3
	opts[1] = 1e-20;
	opts[2] = tol; // eps;
	opts[3] = 1e-20;
	opts[4] = -LM_DIFF_DELTA;

	// clock_t start = clock();

	// invoke the optimization function
	// int rt = dlevmar_bc_dif(func, p, x, m, n, lb, ub, NULL, iMax, opts, info, NULL, NULL, lmd);
	int rt = dlevmar_bc_der(func, df, p, x, m, n, lb, ub, NULL, iMax, opts, info, NULL, NULL, lmd);
	// int rt = dlevmar_der(func, df, p, x, m, n, iMax, opts, info, NULL, NULL, lmd);
	
	// clock_t end = clock();
	// double programTimes = ((double)end - start) / CLOCKS_PER_SEC;
	lmdUpdatePnt(lmd, p);
	double newGDAL = lmdCalSquareSum(lmd);

	// Update POG
	POG* pog = NULL;
	for (int i = 0; i < pogs.GetSize(); i++) {
		pog = (POG*)pogs[i];
		memcpy(pog->p, lmd->ps[i], 3 * sizeof(double));
	}

	lmdFree(lmd);
	free(opts);
	free(info);
	free(p);
	free(x);
	free(lb);
	free(ub);

	return 1;
}

void Pl::ReplaceBegin(double p[3])
{
	int n = pogs.GetSize() ;

	if( n < 2 )
		return ;
	POG* pog = (POG*)pogs[0] ;
	if( pog )
		memcpy(pog->p, p, sizeof(double[3])) ;

	return ;
}

void Pl::ReplaceEnd(double p[3])
{
	int n = pogs.GetSize() ;

	if( n < 2 )
		return ;
	POG* pog = (POG*)pogs[n-1] ;
	if( pog )
		memcpy(pog->p, p, sizeof(double[3])) ;

	return ;
}
//==============================================================
Gdinfo::Gdinfo()
{
	ns[0] = 0 ;
	ns[1] = 0 ;
	pps[0] = NULL ;
	pps[1] = NULL ;
	min = 1e50 ;
	max = -1e50 ;
	i = 0 ;
	I = 0 ;
}

Gdinfo::~Gdinfo()
{
	delete []pps[0] ;
	delete []pps[1] ;
}
//==============================================================
void MTIPathOriginList::Draw()
{
	int i, n = 0 ;

	for( i = 1 ; i <= DNum ; i++ )
		n += (ENum[i]-SNum[i]+1) ;

	glBegin(GL_LINE_STRIP) ;
	for( i = 1 ; i <= n ; i++ )
		glVertex3dv(&PTrail[i].x) ;
	glEnd() ;

	return ;
}

void MTIPathOriginList::Initialize()
{
	DNum = 1;
	TNum = 1;

	SNum = (int *)malloc(sizeof(int) * 2);
	ENum = (int *)malloc(sizeof(int) * 2);

	SNum[0] = -1;
	SNum[1] = 1;
	ENum[0] = -1;
	ENum[1] = 0;
	PTrail = (STLPNT3D*)malloc(sizeof(STLPNT3D));
	memset(PTrail, 0, sizeof(STLPNT3D));
	PNTrail = (STLVECTOR*)malloc(sizeof(STLVECTOR));
	memset(PNTrail, 0, sizeof(STLVECTOR));
	FTrail = (FList*)malloc(sizeof(FList));
	memset(FTrail, 0, sizeof(FList));

	PONext = nullptr;
}

void MTIPathOriginList::Modify(vector<STLPNT3D> iPTrails, vector<STLVECTOR> iPNTrails, vector<FList> iFTrails, POList iPONext)
{
	DNum = 1;
	TNum = 1;

	SNum = (int *)malloc(sizeof(int) * 2);
	ENum = (int *)malloc(sizeof(int) * 2);

	SNum[0] = 0;
	ENum[0] = 0;
	
	int n = iPTrails.size();

	SNum[1] = 1;
	ENum[1] = n;

	PTrail = (STLPNT3D*)malloc(sizeof(STLPNT3D)*(n + 1));
	memset(PTrail, 0, sizeof(STLPNT3D)*(n + 1));

	PNTrail = (STLVECTOR*)malloc(sizeof(STLVECTOR)*(n + 1));
	memset(PNTrail, 0, sizeof(STLVECTOR)*(n + 1));

	FTrail = (FList*)malloc(sizeof(FList)*(n + 1));
	memset(FTrail, 0, sizeof(FList)*(n + 1));

	for (size_t i = 0; i < n; i++)
	{
		PTrail[i + 1] = iPTrails[i];
		PNTrail[i + 1] = iPNTrails[i];
		FTrail[i + 1] = iFTrails[i];
	}

	PONext = iPONext;
}

// nt add 2022/7/10
POList MTIPathOriginList::Copy()
{
	int i, n = 0 ;
	POList copy ;

	copy = (POList)malloc(sizeof(struct MTIPathOriginList)) ;
	if( copy == NULL )
		return NULL ;
	for( i = 1 ; i <= DNum ; i++ )
		n += (ENum[i]-SNum[i]+1) ;
	copy->PTrail = (STLPNT3D*)malloc(sizeof(STLPNT3D)*(n+1)) ;
	memcpy(copy->PTrail, PTrail, sizeof(STLPNT3D)*(n+1)) ;
	copy->PNTrail = (STLPNT3D*)malloc(sizeof(STLPNT3D)*(n+1)) ;
	memcpy(copy->PNTrail, PNTrail, sizeof(STLPNT3D)*(n+1)) ;
	copy->FTrail = (FList*)malloc(sizeof(FList)*(n+1)) ;
	memcpy(copy->FTrail, FTrail, sizeof(FList)*(n+1)) ;
	copy->TNum = TNum ;
	copy->DNum = DNum ;
	copy->SNum = (int*)malloc(sizeof(int)*(DNum+1)) ;
	memcpy(copy->SNum, SNum, sizeof(int)*(DNum+1)) ;
	copy->ENum = (int*)malloc(sizeof(int)*(DNum+1)) ;
	memcpy(copy->ENum, ENum, sizeof(int)*(DNum+1)) ;
	copy->PONext = NULL ;

	return copy ;
}

// nt add 2022/7/10
POList MTIPathOriginList::DirectOffset(double d)
{
	int i, n = 0 ;
	POList offset ;

	offset = Copy() ;
	for( i = 1 ; i <= DNum ; i++ )
		n += (ENum[i]-SNum[i]+1) ;
	for( i = 1 ; i <= n ; i++ )
	{
		mathEvalLin(&PTrail[i].x, &PNTrail[i].x, d, &offset->PTrail[i].x) ;
	}

	return offset ;
}

// 非柔性滚子工作时的测地等距(不分段)
// smf add 2022/7/27
POList MTIPathOriginList::GeodesicOffsetNonFlexible(
	double iDistance, // iDistance可为负值，符号代表等距方向
	GridModel* iModel,	// 输入轨迹所依附模型, 用以获取法矢
	double* oChordalHeight, // 每一点处的弓高
	int iMaxChordalHeight) // 弓高最大值
{
	int i, n = 0;
	POList offset;

	int dir = (iDistance > 0) ? 1 : -1; // 确保等距方向保持一致，dir=1 或 dir=-1.
	iDistance = fabs(iDistance);

	double distance_square = iDistance * iDistance; // 用距离的平方进行比较和计算

	BOOLEAN out_of_model = true;
	int segm_count = 0;

	BOOLEAN last_segm_self_int = false, current_segm_self_int = false;
	double last_segm_self_int_len = 0., current_segm_self_int_len = 0.;

	offset = Copy(); // 先复制一条当前曲线

	for (i = 1; i <= DNum; i++)
		n += (ENum[i] - SNum[i] + 1); // 计算需要等距的点数
	FRelated related_face = (FRelated)malloc(sizeof(FaceRelated));
	related_face->Face = nullptr;
	related_face->FNext = nullptr;
	for (i = 1; i <= n; i++)
	{
		// 当前点是否在三角形内
		if (!IsPointInTriangle(PTrail[i], FTrail[i]))
		{
			offset->Destroy();
			return nullptr;
		}

		int num_related_face = 0;

		STLVECTOR off_dir, tan_dir;
		CalOffsetDir(i, dir, n, off_dir, tan_dir);

		GetRelatedFaceOfPTrail(i, related_face, num_related_face);

		STLPNT3D point_current, point_old; // 三角网格上的点
		memcpy(&point_current, &PTrail[i], sizeof(STLPNT3D));
		STLPNT3D project_point, project_point_old; // 三角网格上的点在原始曲线切平面上的投影
		memcpy(&project_point, &point_current, sizeof(STLPNT3D));

		STLVECTOR vec_ori_to_cur;
		memcpy(&vec_ori_to_cur, &off_dir, sizeof(STLVECTOR));
		double d = 0., delta_d = 0,
			d_prj = 0., delta_d_prj = 0,
			chordal_height = 0., chordal_height_tmp = 0;
		int it = 0;

		while (d_prj < iDistance && it < 1000) // 设置最大迭代次数，避免在模型边界处陷入死循环
		{
			it++; // 迭代次数加一
			int num_related_face_tmp = num_related_face;
			delta_d = 0.;
			delta_d_prj = 0.;
			for (int j = 0; j < num_related_face_tmp; j++)
			{
				memcpy(&point_old, &point_current, sizeof(STLPNT3D));
				memcpy(&project_point_old, &project_point, sizeof(STLPNT3D));
				if (FindNextPoint(related_face, num_related_face,
					point_current, tan_dir, vec_ori_to_cur))
				{
					mathPrjPntPln(point_current, PTrail[i], PNTrail[i], project_point);

					delta_d_prj = mathDist(project_point, project_point_old);
					d_prj += delta_d_prj;

					delta_d = mathDist(point_current, point_old);
					d += delta_d;

					if (d_prj < iDistance)
					{
						chordal_height_tmp = mathDist(point_current, project_point);
						chordal_height = max(chordal_height_tmp, chordal_height);
					}

					vec_ori_to_cur = vectorNormalize(point_current - PTrail[i]);
					break;
				}
			}
			if (delta_d_prj < EPS12)
				break;
		}

		if (d_prj >= iDistance)
		{
			if (out_of_model) // 从外到内
			{
				segm_count++;
				if (segm_count == 1)
				{
					offset->SNum[1] = i;
				}
				out_of_model = false;
			}
			double ratio = (d_prj - iDistance) / delta_d_prj;
			if (!(fabs(ratio) <= 1e-2))
				// 若等距点离三角形的边或顶点不很近，按比例调整；
			{
				STLVECTOR v = point_current - point_old, v_prj = project_point - project_point_old;
				point_current = point_current - ratio * v; // 更新当前点
				project_point = project_point - ratio * v_prj; // 更新当前点在切平面上的投影
			}
			// 否则，边上或顶点上的点为等距点，即不需要调整

			chordal_height_tmp = mathDist(project_point, point_current); // 弓高
			chordal_height = max(chordal_height, chordal_height_tmp); // 取弓高的最大值
			if (chordal_height <= iMaxChordalHeight) 
			{
				oChordalHeight[i] = chordal_height;
			}
			else // 弓高超过设置的阈值
			{
				ERROR_CHORDAL_HEIGHT
				segm_count++;
				offset->ENum[1] = i - 1;
				break;
			}

			// 自相交处理
			// 2022/11/21 smf add
			if (i > offset->SNum[1] + 1)
			{
				STLVECTOR dir_last = vectorNormalize(offset->PTrail[i - 1] - offset->PTrail[i - 2]); // 前一条线段的方向
				STLVECTOR dir_current = vectorNormalize(point_current - offset->PTrail[i - 1]); // 当前线段的方向
				if (dir_last * dir_current < 0)
				{
					// 当前段自相交，当前段的终点处缩短
					current_segm_self_int = true;
					current_segm_self_int_len = mathDist(point_current, offset->PTrail[i - 1]);
					mathShortenOrLengthenSegmByDist(offset->PTrail[i - 2], offset->PTrail[i - 1], SEGMENT::End, -current_segm_self_int_len);

				}
				else
				{
					current_segm_self_int = false;
					current_segm_self_int_len = 0.;
				}
				int num_pnts;
				STLPNT3D int_pnts[2];
				if (mathIntSegmCyl(offset->PTrail[i - 1], point_current,
					PTrail[i - 2], PTrail[i - 1],
					iDistance, EPS06, num_pnts, int_pnts)) // 有交点
				{
					if (num_pnts == 1)
					{
						memcpy(&point_current, &int_pnts[0], sizeof(STLPNT3D));
					}
					else if (num_pnts == 2)
					{
						memcpy(&point_current, &int_pnts[1], sizeof(STLPNT3D));
					}
				}
			}
			if (last_segm_self_int) // 若上一段自相交，当前段的起点处缩短
			{
				mathShortenOrLengthenSegmByDist(offset->PTrail[i - 1], point_current, SEGMENT::Begin, -last_segm_self_int_len);
			}

			// 用圆柱解决自相交问题，效果不好
			/*
			if (i > offset->SNum[1] + 1)
			{
				int num_pnts;
				STLPNT3D int_pnts[2];
				if (mathIntSegmCyl(offset->PTrail[i - 1], point_current,
					PTrail[i - 2], PTrail[i - 1],
					iDistance, EPS06, num_pnts, int_pnts)) // 有交点
				{
					if (num_pnts == 1)
					{
						memcpy(&point_current, &int_pnts[0], sizeof(STLPNT3D));
					}
					else if (num_pnts == 2)
					{
						memcpy(&point_current, &int_pnts[1], sizeof(STLPNT3D));
					}
				}
			}
			*/

			// 更新关键点坐标
			memcpy(&(offset->PTrail[i]), &point_current, sizeof(STLPNT3D));

			// 更新关键点所在三角形
			offset->FTrail[i] = related_face->Face;

			// 计算当前点point_current处法矢，更新等距线上的法矢
			STLVECTOR normal_point_current = iModel->CalAllVertexNormalVector(point_current, TOLLENGTH);
			memcpy(&(offset->PNTrail[i]), &normal_point_current, sizeof(STLPNT3D));
		}
		else
		{
			if (!out_of_model) // 从内到外
			{
				out_of_model = true;
				segm_count++;
				offset->ENum[1] = i - 1;
				break;
			}
		}
	}
	if (related_face)
	{
		related_face->Face = nullptr;
		related_face->FNext = nullptr;
		related_face = nullptr;
		free(related_face);
	}

	return offset;
}

BOOL MTIPathOriginList::FindNextPoint(
	FRelated& ioFace,
	int& ioFaceNum,
	STLPNT3D& ioPointOnPlane,
	STLVECTOR iNormalOfPlane,
	STLVECTOR iLastDir)
{
	FRelated current_face;
	current_face = ioFace;
	STLPNT3D pnts_it[3];
	int index_edge[3]; // 交边的编号
	int num_edge;
	STLPNT3D point_old; // 前一个点
	memcpy(&point_old, &ioPointOnPlane, sizeof(STLPNT3D));

	for (size_t i = 0; i < ioFaceNum && current_face; i++)
	{
		int res = mathPlnIntTri(
			current_face->Face, ioPointOnPlane, iNormalOfPlane,
			TOLLENGTH, TOLANGLE, pnts_it, index_edge, num_edge);

		for (size_t j = 0; j < res; j++)
		{
			int index_ver = -1; // 顶点的编号

			if (!mathIsCoincidentPoint(point_old, pnts_it[j])) // 若交点与上一个点不是重合点
			{
				if ((vectorNormalize(pnts_it[j] - point_old)) * iLastDir > INVERSQRT2)
				{
					if (IsPointAVertex(pnts_it[j], current_face->Face, index_ver)) // 若交点是当前三角形的顶点
					{
						memcpy(&ioPointOnPlane, &pnts_it[j], sizeof(STLPNT3D));
						ioFace->Face = current_face->Face->VertexUsed[i]->FaceUsed->Face;
						ioFace->FNext = current_face->Face->VertexUsed[i]->FaceUsed->FNext;
						ioFaceNum = current_face->Face->VertexUsed[i]->FaceNum;
						return true;
					}
					else // 交点不是三角形的顶点
					{
						for (size_t k = 0; k < num_edge; k++)
						{
							if (index_edge[k] != -1)
							{
								if (IsPointOnEdge(pnts_it[j], current_face->Face->EdgeUsed[index_edge[k]]))
								{
									memcpy(&ioPointOnPlane, &pnts_it[j], sizeof(STLPNT3D));
									ioFace->Face = current_face->Face->EdgeUsed[index_edge[k]]->FaceUsed->Face;
									ioFace->FNext = current_face->Face->EdgeUsed[index_edge[k]]->FaceUsed->FNext;
									ioFaceNum = current_face->Face->EdgeUsed[index_edge[k]]->FaceNum;
									return true;
								}
							}
							else
								return false;
						}
					}
				} // 防止向相反方向找交点
			}
		}
		current_face = current_face->FNext;
	}

	return false;
}

BOOL MTIPathOriginList::FindNextTri(STLPNT3D iBegin, STLPNT3D iEnd, FList &ioNextTri)
{
	FList iCurrentTri = ioNextTri;
	STLVECTOR dir = iEnd - iBegin, prj_dir = {0};
	dir = vectorNormalize(dir);
	STLPNT3D prj_p = {0};
	mathPrjPntPln(iEnd, ioNextTri->VertexUsed[0]->Coord, iCurrentTri->NormalVector, prj_p); // 终点投影到三角形上
	mathPrjVecPln(dir, iCurrentTri->NormalVector, prj_dir); // 方向也投影到三角形上
	prj_dir = vectorNormalize(dir);

	if (IsPointInTriangle(prj_p, iCurrentTri)) // 投影点是否在三角形内
	{
		return true;
	} // 点在三角形内
	else
	{
		for (size_t i = 0; i < 3; i++) // 判断prj_dir是否穿过三角形顶点
		{
			STLVECTOR v = prj_p - iCurrentTri->VertexUsed[i]->Coord;
			v = vectorNormalize(v);
			if (fabs(v * dir - 1) <= TOLANGLE) // 同向，即穿过三角形顶点[i]
			{
				FRelated pF = iCurrentTri->VertexUsed[i]->FaceUsed;
				for (size_t j = 0; j < iCurrentTri->VertexUsed[i]->FaceNum && pF->Face; j++)
				{
					if (pF->Face->FaceNO != iCurrentTri->FaceNO)
					{
						int k;
						for (k = 0; k < 3; k++) 
						{
							if (mathIsCoincidentPoint(iCurrentTri->VertexUsed[i]->Coord, pF->Face->VertexUsed[k]->Coord))
								break;
						}
						if (k == 3)
						{
							return false;
						}
						for (size_t m = 1; m <= 2; m++)
						{
							STLVECTOR v1, v2;
							v1 = pF->Face->VertexUsed[k]->Coord - pF->Face->VertexUsed[(k + 1) % 3]->Coord;
							v1 = vectorNormalize(v1);
							v2 = pF->Face->VertexUsed[k]->Coord - pF->Face->VertexUsed[(k + 2) % 3]->Coord;
							v2 = vectorNormalize(v2);
							if (mathIsVectorDuringTwoVectors(v1, v2, prj_dir)) // prj_dir在v1和v2之间
							{
								for (size_t kk = 0; kk < pF->Face->EdgeUsed[(k + 1) % 3]->FaceNum; kk++)
								{
									FRelated f = pF->Face->EdgeUsed[(k + 1) % 3]->FaceUsed;
									if (pF->Face->FaceNO != f->Face->FaceNO)
									{
										ioNextTri = f->Face;
										return true;
									}
									f = f->FNext;
								}
								return true;
							}
						}
					}
					pF = pF->FNext;
				}
			}
		}

		// 一般情况
		for (size_t i = 0; i < 3; i++)
		{
			STLVECTOR v1, v2;
			//v1 = prj_p - iCurrentTri->EdgeUsed[i]->VertexUsed[0]->Coord;
			v1 = iCurrentTri->EdgeUsed[i]->VertexUsed[0]->Coord - iBegin;
			v1 = vectorNormalize(v1);
			//v2 = prj_p - iCurrentTri->EdgeUsed[i]->VertexUsed[1]->Coord;
			v2 = iCurrentTri->EdgeUsed[i]->VertexUsed[1]->Coord - iBegin;
			v2 = vectorNormalize(v2);
			if (mathIsVectorDuringTwoVectors(v1, v2, prj_dir)) // 若prj_dir穿过了EdgeUsed[i]
			{
				FRelated pF = iCurrentTri->EdgeUsed[i]->FaceUsed;
				for (size_t j = 0; j < iCurrentTri->EdgeUsed[i]->FaceNum && pF->Face; j++)
				{
					if (pF->Face->FaceNO != iCurrentTri->FaceNO)
					{
						ioNextTri = pF->Face;
						return true;
					}
					pF = pF->FNext;
				} 
			}
		}
	} // 点不在三角形内
	return false;
}

// 没有求交点坐标，找的方向需要确定
BOOL MTIPathOriginList::FindNextTri2(STLPNT3D iBegin, STLPNT3D iEnd, FList &ioNextTri)
{
	FList iCurrentTri = ioNextTri;
	STLVECTOR dir = iEnd - iBegin, prj_dir = { 0 };
	dir = vectorNormalize(dir);
	STLPNT3D prj_p = { 0 };
	mathPrjPntPln(iEnd, ioNextTri->VertexUsed[0]->Coord, iCurrentTri->NormalVector, prj_p); // 终点投影到三角形上
	mathPrjVecPln(dir, iCurrentTri->NormalVector, prj_dir); // 方向也投影到三角形上
	prj_dir = vectorNormalize(dir);

	for (size_t i = 0; i < 3; i++) // 判断prj_dir是否穿过三角形顶点
	{
		STLVECTOR v = prj_p - iCurrentTri->VertexUsed[i]->Coord;
		v = vectorNormalize(v);
		if (fabs(v * dir - 1) <= TOLANGLE) // 同向，即穿过三角形顶点[i]
		{
			FRelated pF = iCurrentTri->VertexUsed[i]->FaceUsed;
			for (size_t j = 0; j < iCurrentTri->VertexUsed[i]->FaceNum && pF->Face; j++)
			{
				if (pF->Face->FaceNO != iCurrentTri->FaceNO)
				{
					int k;
					for (k = 0; k < 3; k++)
					{
						if (mathIsCoincidentPoint(iCurrentTri->VertexUsed[i]->Coord, pF->Face->VertexUsed[k]->Coord))
							break;
					}
					if (k == 3)
					{
						return false;
					}
					for (size_t m = 1; m <= 2; m++)
					{
						STLVECTOR v1, v2;
						v1 = pF->Face->VertexUsed[k]->Coord - pF->Face->VertexUsed[(k + 1) % 3]->Coord;
						v1 = vectorNormalize(v1);
						v2 = pF->Face->VertexUsed[k]->Coord - pF->Face->VertexUsed[(k + 2) % 3]->Coord;
						v2 = vectorNormalize(v2);
						if (mathIsVectorDuringTwoVectors(v1, v2, prj_dir)) // prj_dir在v1和v2之间
						{
							for (size_t kk = 0; kk < pF->Face->EdgeUsed[(k + 1) % 3]->FaceNum; kk++)
							{
								FRelated f = pF->Face->EdgeUsed[(k + 1) % 3]->FaceUsed;
								if (pF->Face->FaceNO != f->Face->FaceNO)
								{
									ioNextTri = f->Face;
									return true;
								}
								f = f->FNext;
							}
							return true;
						}
					}
				}
				pF = pF->FNext;
			}
		}
	}

	// 一般情况
	for (size_t i = 0; i < 3; i++)
	{
		STLVECTOR v1, v2;
		//v1 = prj_p - iCurrentTri->EdgeUsed[i]->VertexUsed[0]->Coord;
		v1 = iCurrentTri->EdgeUsed[i]->VertexUsed[0]->Coord - iBegin;
		v1 = vectorNormalize(v1);
		//v2 = prj_p - iCurrentTri->EdgeUsed[i]->VertexUsed[1]->Coord;
		v2 = iCurrentTri->EdgeUsed[i]->VertexUsed[1]->Coord - iBegin;
		v2 = vectorNormalize(v2);
		if (mathIsVectorDuringTwoVectors(v1, v2, prj_dir)) // 若prj_dir穿过了EdgeUsed[i]
		{
			FRelated pF = iCurrentTri->EdgeUsed[i]->FaceUsed;
			for (size_t j = 0; j < iCurrentTri->EdgeUsed[i]->FaceNum && pF->Face; j++)
			{
				if (pF->Face->FaceNO != iCurrentTri->FaceNO)
				{
					ioNextTri = pF->Face;
					return true;
				}
				pF = pF->FNext;
			}
		}
	}

	return false;
}

// 点iPoint是否是三角形iFace的顶点
BOOL MTIPathOriginList::IsPointAVertex(STLPNT3D iPoint, FaceList* iFace, int &oIndex)
{
	for (int i = 0; i < 3; i++)
	{
		if (mathSquareDist(iPoint, iFace->VertexUsed[i]->Coord) <= TOLSQUARELENGTH)
		{
			oIndex = i;
			return true;
		}
	}
	oIndex = -1;
	return false;
}

BOOL MTIPathOriginList::IsPointOnEdge(STLPNT3D iPoint, EList iEdge)
{
	STLVECTOR v;
	STLPNT3D p;
	double dis = mathDist(iPoint, iEdge->VertexUsed[0]->Coord);
	v = iEdge->VertexUsed[1]->Coord - iEdge->VertexUsed[0]->Coord;
	v = vectorNormalize(v);
	p = iEdge->VertexUsed[0]->Coord + dis * v;

	if (mathIsCoincidentPoint(iPoint, p))
		return true;
	else
		return false;
}

// smf add 2022/12/13
// 判断点是否在三角形内
BOOL MTIPathOriginList::IsPointInTriangle(STLPNT3D iPoint, FList iTri)
{
	BOOL rc = IDOUT;

	PNT3D p, pviot;
	VEC3D normal;
	memcpy(p, &iPoint, sizeof(STLPNT3D));
	memcpy(pviot, &(iTri->VertexUsed[0]->Coord), sizeof(STLPNT3D));
	memcpy(normal, &(iTri->NormalVector), sizeof(STLVECTOR));
	double d = 0.;
	if (( d = mathDistPntPln(p, pviot, normal)) >= TOLLENGTH) // 点与三角形不共面
	{
		return IDOUT;
	}
	else
	{
		STLVECTOR ap, bp, cp, ab, bc, ca;
		ap = iPoint - iTri->VertexUsed[0]->Coord;
		ap = vectorNormalize(ap);
		bp = iPoint - iTri->VertexUsed[1]->Coord;
		bp = vectorNormalize(bp);
		cp = iPoint - iTri->VertexUsed[2]->Coord;
		cp = vectorNormalize(cp);
		ab = iTri->VertexUsed[1]->Coord - iTri->VertexUsed[0]->Coord;
		ab = vectorNormalize(ab);
		bc = iTri->VertexUsed[2]->Coord - iTri->VertexUsed[1]->Coord;
		bc = vectorNormalize(bc);
		ca = iTri->VertexUsed[0]->Coord - iTri->VertexUsed[2]->Coord;
		ca = vectorNormalize(ca);
		
		double mixed_product1 = vectorNormalize(ab ^ ap) * iTri->NormalVector;
		double mixed_product2 = vectorNormalize(bc ^ bp) * iTri->NormalVector;
		double mixed_product3 = vectorNormalize(ca ^ cp) * iTri->NormalVector;

		if ((fabs(mixed_product1) <= TOLANGLE ||
			fabs(mixed_product2) <= TOLANGLE ||
			fabs(mixed_product3) <= TOLANGLE )||
			mixed_product1 * mixed_product2 >= 0 &&
		    mixed_product2 * mixed_product3 >= 0 )
		{
			return IDIN;
		}
		else
		{
			return IDOUT;
		}
	}
}

bool MTIPathOriginList::IsSegmInTriangle(STLPNT3D iBegin, STLPNT3D iEnd, FList iTri)
{
	if (IsPointInTriangle(iBegin, iTri) &&
		IsPointInTriangle(iEnd, iTri))
	{
		return true;
	}
	return false;
}

void MTIPathOriginList::PolylineCheck()
{
	STLVECTOR dir_old, dir_new;
	for (size_t i = SNum[1] + 1; i < ENum[1]; i++)
	{
		//删除过短的线
		if (mathSquareDist(PTrail[i - 1], PTrail[i]) <= 1)
		{
			DeleteOnePTrail(i);
			i--;
			continue;
		}
		dir_old = vectorNormalize(PTrail[i] - PTrail[i - 1]);
		dir_new = vectorNormalize(PTrail[i + 1] - PTrail[i]);
		if (dir_old * dir_new < TOLANGLE)
		{
			DeleteOnePTrail(i + 1);
			i--;
		}
	}
}

void MTIPathOriginList::UpdateNormalAndFace(GridModel *iModel, double iTol)
{
	int s_num = SNum[1], e_num = ENum[1];
	int n = e_num - s_num + 1;
	if (n >= 1)
	{
		for (size_t i = s_num; i < e_num; i++)
		{
			PNTrail[i] = iModel->CalAllVertexNormalVector(PTrail[i], iTol);
			FTrail[i] = iModel->FindClosetFace(PTrail[i], iTol);
		}
	}
	else
		return;
}

BOOL MTIPathOriginList::DeleteOnePTrail(int iIndex)
{
	//int n = ENum[1] - SNum[1] + 1;
	int n_new = ENum[1] - SNum[1];

	int size_of_pnt = sizeof(STLPNT3D);
	int size_of_vec = sizeof(STLPNT3D);
	int size_of_flist = sizeof(FList);

	int num_before_i = iIndex - SNum[1] + 1;
	int num_after_i = ENum[1] - iIndex;

	if (n_new > 0)
	{
		STLPNT3D* ptrail_new = (STLPNT3D*)malloc(size_of_pnt*(n_new + 1));
		STLVECTOR* pntrail_new = (STLVECTOR*)malloc(size_of_vec*(n_new + 1));
		FList* ftrail_new = (FList*)malloc(size_of_flist*(n_new + 1));

		memcpy(ptrail_new, PTrail, size_of_pnt * num_before_i);
		memcpy(pntrail_new, PNTrail, size_of_vec * num_before_i);
		memcpy(ftrail_new, FTrail, size_of_flist * num_before_i);

		memcpy(&ptrail_new[iIndex], &PTrail[iIndex + 1], size_of_pnt * num_after_i);
		memcpy(&pntrail_new[iIndex], &PNTrail[iIndex + 1], size_of_vec * num_after_i);
		memcpy(&ftrail_new[iIndex], &FTrail[iIndex + 1], size_of_flist * num_after_i);

		ENum[1]--;

		STLPNT3D* ptrail = PTrail;
		STLVECTOR* pntrail = PNTrail;
		FList* ftrail = FTrail;

		PTrail = ptrail_new;
		PNTrail = pntrail_new;
		FTrail = ftrail_new;

		if (ptrail)
		{
			free(ptrail);
			ptrail = nullptr;
		}
		if (pntrail)
		{
			free(pntrail);
			pntrail = nullptr;
		}
		if (ftrail)
		{
			free(ftrail);
			ftrail = nullptr;
		}

		return true;
	}

	return false;
}

void MTIPathOriginList::PolylineExtrapolate(GridModel *iModel)
{
	int s_num = SNum[1], e_num = ENum[1];
	// 1. 判断首（末）点是否在边界，若都在边界，直接返回；否则执行步骤2
	// 首点
	STLPNT3D p_current, p_next = { 0, 0, 0 }, p_last;
	FList f_current, f_next = nullptr;

	p_current = PTrail[s_num];
	f_current = FTrail[s_num];
	p_last = PTrail[s_num + 1];
	while (!GridModel::IsPointOnBoundary(p_current, f_current))
	{
		// 2. 首点延长
		STLVECTOR dir = vectorNormalize(p_current - p_last);
		if (FindNextTri2(p_last, p_current, f_current))
		{
			STLVECTOR normal_current = iModel->CalAllVertexNormalVector(p_current, TOLANGLE);
			AddFirstPTrail(p_current, normal_current, f_current);
		}
		//Extrapolate(p_current, dir, f_current, p_next, f_next);
		//if (f_next)
		//{
		//	STLVECTOR p_n_trail = iModel->CalAllVertexNormalVector(p_next, TOLANGLE);
		//	AddFirstPTrail(p_next, p_n_trail, f_next);
		//	f_current = f_next;
		//	p_current = p_next;
		//}
	}
	// 末点
	p_current = PTrail[e_num];
	f_current = FTrail[e_num];
	while (!GridModel::IsPointOnBoundary(p_current, f_current))
	{
		// 2. 末点延长
		STLVECTOR dir = vectorNormalize(PTrail[e_num] - PTrail[e_num - 1]);
		Extrapolate(p_current, dir, f_current, p_next, f_next);
		if (!f_next)
		{
			STLVECTOR p_n_trail = iModel->CalAllVertexNormalVector(p_next, TOLANGLE);
			PushOnePTrail(p_next, p_n_trail, f_next);
			f_current = f_next;
			p_current = p_next;
		}
	}

	return ;
}

// iSupport, iIndexOfVert1 和 iIndexOfVert2 均为 iPoint2 相关的参数
void MTIPathOriginList::Extrapolate(STLPNT3D iPoint, STLVECTOR iDirection, FList iSupport, STLPNT3D &oPoint, FList &oFNext)
{
	for (size_t i = 0; i < 3; i++)
	{
		VList v_list = iSupport->VertexUsed[i];
		FRelated f_related = v_list->FaceUsed;
		while (f_related)
		{
			// 线段与三角形求交
			STLVECTOR prj_dir;
			mathPrjVecPln(iDirection, f_related->Face->NormalVector, prj_dir);
			for (size_t j = 0; j < 3; j++)
			{
				// 线段和线段求交

				STLVECTOR v1, v2;
				v1 = 100 * iDirection;
				STLPNT3D p1 = iPoint, p2 = p1 + v1;
				STLPNT3D q1 = f_related->Face->VertexUsed[j]->Coord, q2 = f_related->Face->VertexUsed[(j + 1) % 3]->Coord;

				STLPNT3D oPoint;
				if (mathSegmIntSegmInFace(p1, p2, q1, q2, TOLLENGTH, oPoint) > 0)
				{
					if (mathDist(iPoint, oPoint) <= TOLLENGTH)
					{
						oFNext = f_related->Face;
						return;
					}
				}
				/*v2 = q2 - q1;
				STLVECTOR v = p2 - p1;

				double t1, t2;
				double det_xy = v1.x * v2.y - v1.y * v2.x;
				double det_yz = v1.y * v2.z - v1.z * v2.y;
				double det_xz = v1.x * v2.z - v1.z * v2.x;

				if (det_xy > TOLLENGTH * TOLLENGTH || 
					det_xy < -TOLLENGTH * TOLLENGTH)
				{
					double det_xy_1 = v.x * v2.y - v.y * v2.x;
					double det_xy_2 = v1.x * v.y - v1.y * v.x;
					t1 = det_xy_1 / det_xy;
					t2 = det_xy_2 / det_xy;
					if ((t1 >= 0 && t1 <= 1) && (t2 >= 0 && t2 <= 1) &&
						mathSquareDist(p1 + t1 * v1, q1 + t2 * v2) < TOLLENGTH * TOLLENGTH)
					{
						oPoint = p1 + t1 * v1;
						oFNext = f_related->Face;
						return;
					}
				}

				if (det_yz > TOLLENGTH * TOLLENGTH ||
					det_yz < -TOLLENGTH * TOLLENGTH)
				{
					double det_yz_1 = v.y * v2.z - v.z * v2.y;
					double det_yz_2 = v1.y * v.z - v1.z * v.y;
					t1 = det_yz_1 / det_yz;
					t2 = det_yz_2 / det_yz;
					if ((t1 >= 0 && t1 <= 1) && (t2 >= 0 && t2 <= 1) &&
						mathSquareDist(p1 + t1 * v1, q1 + t2 * v2) < TOLLENGTH * TOLLENGTH)
					{
						oPoint = p1 + t1 * v1;
						oFNext = f_related->Face;
						return;
					}
				}

				if (det_xz > TOLLENGTH * TOLLENGTH ||
					det_xz < -TOLLENGTH * TOLLENGTH)
				{
					double det_xz_1 = v.x * v2.z - v.z * v2.x;
					double det_xz_2 = v1.x * v.z - v1.z * v.x;
					t1 = det_xz_1 / det_xz;
					t2 = det_xz_2 / det_xz;
					if ((t1 >= 0 && t1 <= 1) && (t2 >= 0 && t2 <= 1) &&
						mathSquareDist(p1 + t1 * v1, q1 + t2 * v2) < TOLLENGTH * TOLLENGTH)
					{
						oPoint = p1 + t1 * v1;
						oFNext = f_related->Face;
						return;
					}
				}*/

			}
			f_related = f_related->FNext;
		}
	}
	oFNext = nullptr;
}

// 柔性滚子工作时的测地等距(不分段)
// smf add 2022/9/25
POList MTIPathOriginList::GeodesicOffsetFlexible(
	double iDistance, // iDistance可为负值，符号代表等距方向
	GridModel* iModel)  // 输入轨迹所依附模型, 用以获取法矢
{
	int i, n = 0;
	POList offset;

	int dir = (iDistance > 0) ? 1 : -1; // 确保等距方向保持一致，dir=1 或 dir=-1.
	iDistance = fabs(iDistance);

	double distance_square = iDistance * iDistance; // 用距离的平方进行比较和计算
	
	BOOLEAN out_of_model = true;
	int segm_count = 0;

	BOOLEAN last_segm_self_int = false, current_segm_self_int = false;
	double last_segm_self_int_len = 0., current_segm_self_int_len = 0.;

	offset = Copy(); // 先复制一条当前曲线

	for (i = 1; i <= DNum; i++)
		n += (ENum[i] - SNum[i] + 1); // 计算需要等距的点数
	FRelated related_face = (FRelated)malloc(sizeof(FaceRelated));
	related_face->Face = nullptr;
	related_face->FNext = nullptr;
	for (i = 1; i <= n; i++)
	{
		// 当前点是否在三角形内
		if (!IsPointInTriangle(PTrail[i], FTrail[i]))
		{
			offset->Destroy();
			return nullptr;
		}

		int num_related_face = 0;
		
		STLVECTOR off_dir, tan_dir;
		CalOffsetDir(i, dir, n, off_dir, tan_dir);

		GetRelatedFaceOfPTrail(i, related_face, num_related_face);

		STLPNT3D point_current, point_old; // 三角网格上的点
		memcpy(&point_current, &PTrail[i], sizeof(STLPNT3D));
		STLPNT3D project_point, project_point_old; // 三角网格上的点在原始曲线切平面上的投影
		memcpy(&project_point, &point_current, sizeof(STLPNT3D));

		STLVECTOR dir_old;
		memcpy(&dir_old, &off_dir, sizeof(STLVECTOR));
		double d = 0., delta_d = 0,
			d_prj = 0., delta_d_prj = 0;
		int it = 0;
		while (d < iDistance && it < 1000) // 设置最大迭代次数，避免在模型边界处陷入死循环
		{
			delta_d = 0;
			delta_d_prj = 0;
			it++; // 迭代次数加一
			int num_related_face_tmp = num_related_face;
			for (int j = 0; j < num_related_face_tmp; j++)
			{
				memcpy(&point_old, &point_current, sizeof(STLPNT3D));
				memcpy(&project_point_old, &project_point, sizeof(STLPNT3D));
				if (FindNextPoint(related_face, num_related_face,
					point_current, tan_dir, dir_old))
				{
					mathPrjPntPln(point_current, PTrail[i], PNTrail[i], project_point);

					delta_d_prj = mathDist(project_point, project_point_old);
					d_prj += delta_d_prj;

					delta_d = mathDist(point_current, point_old);
					d += delta_d;

					dir_old = vectorNormalize(point_current - point_old);
					break;
				}
			}
			if (delta_d < EPS12)
				break;
		}
		if (d >= iDistance)
		{
			if (out_of_model) // 从外到内
			{
				segm_count++;
				if (segm_count == 1)
				{
					offset->SNum[1] = i;
				}
				out_of_model = false;
			}
			double ratio = (d - iDistance) / delta_d;
			if (!(fabs(ratio) <= 1e-2))
				// 若等距点离三角形的边或顶点不很近，按比例调整；
			{
				STLVECTOR v = point_current - point_old, v_prj = project_point - project_point_old;
				point_current = point_current - ratio * v; // 更新当前点
				project_point = project_point - ratio * v_prj; // 更新当前点在切平面上的投影
			}
			// 否则，边上或顶点上的点为等距点，即不需要调整

			// 自相交处理
			// 2022/11/21 smf add
			if (i > offset->SNum[1] + 1)
			{
				STLVECTOR dir_last = vectorNormalize(offset->PTrail[i - 1] - offset->PTrail[i - 2]); // 前一条线段的方向
				STLVECTOR dir_current = vectorNormalize(point_current - offset->PTrail[i - 1]); // 当前线段的方向
				if (dir_last * dir_current < 0)
				{
					// 当前段自相交，当前段的终点处缩短
					current_segm_self_int = true;
					current_segm_self_int_len = mathDist(point_current, offset->PTrail[i - 1]);
					mathShortenOrLengthenSegmByDist(offset->PTrail[i - 2], offset->PTrail[i - 1], SEGMENT::End, -current_segm_self_int_len);
					
				}
				else
				{
					current_segm_self_int = false;
					current_segm_self_int_len = 0.;
				}
				int num_pnts;
				STLPNT3D int_pnts[2];
				if (mathIntSegmCyl(offset->PTrail[i - 1], point_current,
					PTrail[i - 2], PTrail[i - 1],
					iDistance, EPS06, num_pnts, int_pnts)) // 有交点
				{
					if (num_pnts == 1)
					{
						memcpy(&point_current, &int_pnts[0], sizeof(STLPNT3D));
					}
					else if (num_pnts == 2)
					{
						memcpy(&point_current, &int_pnts[1], sizeof(STLPNT3D));
					}
				}
			}


			if (last_segm_self_int) // 若上一段自相交，当前段的起点处缩短
			{
				mathShortenOrLengthenSegmByDist(offset->PTrail[i - 1], point_current, SEGMENT::Begin, -last_segm_self_int_len);
			}

			// 更新关键点坐标
			memcpy(&(offset->PTrail[i]), &point_current, sizeof(STLPNT3D));

			// 更新关键点所在三角形
			offset->FTrail[i] = related_face->Face;

			// 计算当前点point_current处法矢，更新等距线上的法矢
			STLVECTOR normal_point_current = iModel->CalAllVertexNormalVector(point_current, TOLLENGTH);
			memcpy(&(offset->PNTrail[i]), &normal_point_current, sizeof(STLPNT3D));

			last_segm_self_int = current_segm_self_int;
			last_segm_self_int_len = current_segm_self_int_len;
		}
		else
		{
			if (!out_of_model) // 从内到外
			{
				out_of_model = true;
				segm_count++;
				offset->ENum[1] = i - 1;
				break;
			}
		}
	}
	if (related_face)
	{
		related_face->Face = nullptr;
		related_face->FNext = nullptr;
		related_face = nullptr;
		free(related_face);
	}

	return offset;
}

double CalVariance(double * iArray, int iSize)
{
	int variance = 0.;
	int average = 0.;

	if (iSize > 2)
	{
		for (size_t i = 0; i < iSize; i++)
		{
			average += iArray[i];
		}
		average = average * 1. / iSize;

		for (size_t i = 0; i < 3; i++)
		{
			variance += (average - iArray[i])*(average - iArray[i]);
		}
		variance = variance * 0.5;
	}
	else
	{
		variance = 0.;
	}
	return variance;
}

POList MTIPathOriginList::GeodesicOffsetFlexibleNew(double iDistance, GridModel * iModel)
{
	vector<FList> f_arr;
	f_arr.clear();

	int n = 0;
	POList offset;

	int dir = (iDistance > 0) ? 1 : -1; // 确保等距方向保持一致，dir=1 或 dir=-1.
	iDistance = fabs(iDistance);

	double distance_square = iDistance * iDistance; // 用距离的平方进行比较和计算

	STLVECTOR off_dir, tan_dir;
	BOOLEAN out_of_model = true;
	int segm_count = 0;

	offset = Copy(); // 先复制一条当前曲线

	for (int i = 1; i <= DNum; i++)
		n += (ENum[i] - SNum[i] + 1); // 计算需要等距的点数
	FRelated related_face = (FRelated)malloc(sizeof(FaceRelated));
	related_face->Face = nullptr;
	related_face->FNext = nullptr;
	for (int i = 1; i <= n; i++)
	{
		// 当前点是否在三角形内
		if (!IsPointInTriangle(PTrail[i], FTrail[i]))
		{
			offset->Destroy();
			offset = nullptr;
			return offset;
		}

		int num_related_face = 0;

		// 计算测地线方向及测地等距方向
		CalOffsetDir(i, dir, n, off_dir, tan_dir);

		// 获取当前关键点处的邻接面及个数
		GetRelatedFaceOfPTrail(i, related_face, num_related_face);

		STLPNT3D point_current, point_old; // 三角网格上的点
		memcpy(&point_current, &PTrail[i], sizeof(STLPNT3D));
		STLPNT3D project_point, project_point_old; // 三角网格上的点在原始曲线切平面上的投影
		memcpy(&project_point, &point_current, sizeof(STLPNT3D));

		STLVECTOR dir_old;
		memcpy(&dir_old, &off_dir, sizeof(STLVECTOR));
		double d = 0., delta_d = 0,
			d_prj = 0., delta_d_prj = 0;
		int it = 0;
		while (d < iDistance && it < 1000) // 设置最大迭代次数，避免在模型边界处陷入死循环
		{
			delta_d = 0;
			delta_d_prj = 0;
			it++; // 迭代次数加一
			int num_related_face_tmp = num_related_face;
			for (int j = 0; j < num_related_face_tmp; j++)
			{
				memcpy(&point_old, &point_current, sizeof(STLPNT3D));
				memcpy(&project_point_old, &project_point, sizeof(STLPNT3D));
				if (FindNextPoint(related_face, num_related_face,
					point_current, tan_dir, dir_old))
				{
					mathPrjPntPln(point_current, PTrail[i], PNTrail[i], project_point);

					delta_d_prj = mathDist(project_point, project_point_old);
					d_prj += delta_d_prj;

					delta_d = mathDist(point_current, point_old);
					d += delta_d;

					dir_old = vectorNormalize(point_current - point_old);
					break;
				}
			}
			if (delta_d < EPS12)
				break;
		}

		// 获取目标等距线上不重复的三角形链
		FList f_current = related_face->Face;
		bool add_this_f = true;
		int f_size = f_arr.size();
		if (f_size == 0)
		{
			f_arr.push_back(f_current);
		}
		else
		{
			for (size_t j = 0; j < f_size; j++)
			{
				if (f_current == f_arr[j] || f_current->IsAdj(f_arr[j]))
				{
					add_this_f = false;
				}
			}
			if (add_this_f)
			{
				f_arr.push_back(f_current);
			}
		}
	}
	//offset->Destroy();
	//offset = nullptr;
	free(related_face);
	related_face = nullptr;
	
	vector<STLPNT3D> p_vec; // 点
	vector<STLVECTOR> n_vec;// 法矢
	vector<FList> f_vec;	// 面
	p_vec.clear();
	n_vec.clear();
	f_vec.clear();

	//bool added_ptrail = false;
	// 对获取到的三角形链进行插值
	int f_size = f_arr.size();
	STLVECTOR dir_begin = vectorNormalize(PTrail[SNum[1] + 1] - PTrail[SNum[1]]);
	for (size_t i = 0; i < f_size; i++)
	{
		FList cur_f = f_arr[i];
		STLPNT3D p_arr[3];
		double dist[3] = { 0. };
		for (size_t j = 0; j < 3; j++)
		{
			p_arr[j] = cur_f->VertexUsed[j]->Coord;
			dist[j] = CalGeodesicDistancePointToPl(iModel, cur_f, p_arr[j], dir); // 要有正有负！！！
		}

		if (CalVariance(dist, 3) > 4000)
		{
			continue;
		}

		STLPNT3D target_pnts[3] = { 0. };
		int index[3] = { -1, -1, -1 };
		int res = mathCalPointsByTriangleVertexInfo(p_arr[0], p_arr[1], p_arr[2], dist, iDistance, TOLLENGTH, target_pnts, index);
		// Todo: target_pnts待排序
		for (size_t k = 0; k < res; k++)
		{
			STLVECTOR normal_point_current = iModel->CalAllVertexNormalVector(target_pnts[k], TOLLENGTH);
			if (p_vec.empty() && res == 2)
			{
				STLVECTOR temp_dir = vectorNormalize(target_pnts[1] - target_pnts[0]);
				if (temp_dir * dir_begin < TOLANGLE)
				{
					STLPNT3D temp_pnt = target_pnts[0];
					target_pnts[0] = target_pnts[1];
					target_pnts[1] = temp_pnt;
				}
			}
			//if (!added_ptrail)
			//{
			//	result_pl->AddFirstPTrail(target_pnts[k], normal_point_current, cur_f);
			//	added_ptrail = true;
			//}
			//else
			//{
				//int index_insert = result_pl->ENum[1];
				//TODO:<待增加新的算法，在最后插入一个点>
				//result_pl->AddFirstPTrail(target_pnts[k], normal_point_current, cur_f);
				double distance = CalGeodesicDistancePointToPl(iModel, cur_f, target_pnts[k], dir);
				//result_pl->PushOnePtrail(target_pnts[k], normal_point_current, cur_f);
			if (!iModel->IsPointOnBoundary(target_pnts[k], cur_f))
			{
				p_vec.push_back(target_pnts[k]);
				n_vec.push_back(normal_point_current);
				f_vec.push_back(cur_f);
			}
			//}
		}
	}
	f_arr.clear();

	POList result_pl = (POList)malloc(sizeof(struct MTIPathOriginList));
	if (p_vec.size() > 0)
	{
		result_pl->Modify(p_vec, n_vec, f_vec);
	}
	else
	{
		result_pl->Destroy();
		free(result_pl);
		result_pl = nullptr;
	}
	p_vec.clear();
	n_vec.clear();
	f_vec.clear();

	return result_pl;
}

POList MTIPathOriginList::GeodesicOffsetPreprocessing(/*STLVECTOR iOffsetDir, */GridModel* iModel)
{
	POList new_pl;
	new_pl = Copy();
	for (size_t i = 1; i <= DNum; i++)
	{
		int tmp = new_pl->ENum[i] - new_pl->SNum[i] + 1;
		for (size_t j = 1; j < tmp; j++)
		{
			int current_index = new_pl->SNum[i] + j;
			STLVECTOR v = new_pl->PTrail[current_index] - new_pl->PTrail[current_index - 1];
			double l = v.norm();
			if (l > delta_l)
			{
				STLPNT3D new_p;
				if (l <= 2 * delta_l)
				{
					new_p = mathMidPoint(new_pl->PTrail[current_index - 1], new_pl->PTrail[current_index]);
				}
				else
				{
					STLVECTOR dir = 1. / l * v;
					new_p = new_pl->PTrail[current_index - 1] + delta_l * dir;
				}
				FList new_f = iModel->FindClosetFace(new_p, TOLLENGTH);
				STLPNT3D prj_p = { 0 };
				mathPrjPntPln(new_p, new_f->VertexUsed[0]->Coord, new_f->NormalVector, prj_p);
				//STLPNT3D old_p = new_pl->PTrail[current_index - 1];
				//FList new_f = new_pl->FTrail[current_index - 1];
				//STLPNT3D prj_p = {0};
				//mathPrjPntPln(new_p, new_f->VertexUsed[0]->Coord, new_f->NormalVector, prj_p);
				////mathPrjPntPln(old_p, new_f->VertexUsed[0]->Coord, new_f->NormalVector, prj_p); // 投影
				//while (!IsPointInTriangle(prj_p, new_f)) // 如果新加的点不在当前三角形内部，则寻找下一个三角形
				//{
				//	//STLVECTOR prj_dir = {0};
				//	//STLVECTOR new_normal = new_f->NormalVector;
				//	//mathPrjVecPln(dir, new_normal, prj_dir); // 单位投影向量
				//	FindNextTri(old_p, new_p, new_f);
				//	mathPrjPntPln(new_p, new_f->VertexUsed[0]->Coord, new_f->NormalVector, prj_p);
				//}
				STLVECTOR new_normal = iModel->CalAllVertexNormalVector(prj_p, TOLLENGTH);
				new_pl->AddOnePTrail(prj_p, new_normal, new_f, j);
				j--;
			}
		}
	}
	return new_pl;
}


int MTIPathOriginList::AddOnePTrail(STLPNT3D & iPTrail, STLVECTOR & iPNTrail, FList iFTrail, int iIndex)
{
	int i, n = 0;

	STLPNT3D* p_ptrail_new = nullptr;
	STLVECTOR* p_pntrail_new = nullptr;
	FList* p_ftrail_new = nullptr;

	BOOL added = false;

	if (iIndex < 1 || iIndex > ENum[DNum])
		return 0; // 插入的位置有错

	for (i = 1; i <= DNum; i++)
	{
		if (!added) // 若已插入
		{
			if (iIndex >= SNum[i] &&
				iIndex <= ENum[i]) // 找到iIndex所在的那一段
			{
				p_ptrail_new = (STLPNT3D*)malloc(sizeof(STLPNT3D)*(ENum[DNum] + 1 + 1)); // 添加之前需要ENum[DNum]+1块内存
				memset(p_ptrail_new, 0, sizeof(STLPNT3D)*(ENum[DNum] + 1 + 1));

				p_pntrail_new = (STLVECTOR*)malloc(sizeof(STLVECTOR)*(ENum[DNum] + 1 + 1));
				memset(p_pntrail_new, 0, sizeof(STLVECTOR)*(ENum[DNum] + 1 + 1));

				p_ftrail_new = (FList*)malloc(sizeof(FList)*(ENum[DNum] + 1 + 1));
				memset(p_ftrail_new, 0, sizeof(FList)*(ENum[DNum] + 1 + 1));

				// 插入点
				memcpy(p_ptrail_new, PTrail, sizeof(STLPNT3D)*(iIndex)); // iIndex之前的点
				memcpy(&(p_ptrail_new[iIndex]), &iPTrail, sizeof(STLPNT3D));
				memcpy(&(p_ptrail_new[iIndex + 1]), &(PTrail[iIndex]), sizeof(STLPNT3D)*(ENum[DNum] - iIndex + 1));  // iIndex及之后的点

				// 插入法矢
				memcpy(p_pntrail_new, PNTrail, sizeof(STLVECTOR)*(iIndex)); // iIndex之前的法矢
				memcpy(&(p_pntrail_new[iIndex]), &iPNTrail, sizeof(STLVECTOR));
				memcpy(&(p_pntrail_new[iIndex + 1]), &(PNTrail[iIndex]), sizeof(STLVECTOR)*(ENum[DNum] - iIndex + 1));  // iIndex及之后的法矢

				// 插入面
				memcpy(p_ftrail_new, FTrail, sizeof(FList)*(iIndex)); // iIndex之前的面
				memcpy(&(p_ftrail_new[iIndex]), &iFTrail, sizeof(FList));
				memcpy(&(p_ftrail_new[iIndex + 1]), &(FTrail[iIndex]), sizeof(FList)*(ENum[DNum] - iIndex + 1));  // iIndex及之后的面

				added = true; // 已经插入点
				ENum[i]++;
			}
		}
		else
		{
			SNum[i]++;
			ENum[i]++;
		}
	}

	STLPNT3D* p_ptrail = PTrail;
	STLVECTOR* p_pntrail = PNTrail;
	FList* p_ftrail = FTrail;
	PTrail = p_ptrail_new;
	PNTrail = p_pntrail_new;
	FTrail = p_ftrail_new;
	if (p_ptrail)
	{
		free(p_ptrail);
		p_ptrail = nullptr;
	}
	if (p_pntrail)
	{
		free(p_pntrail);
		p_pntrail = nullptr;
	}
	if (p_ftrail)
	{
		free(p_ftrail);
		p_ftrail = nullptr;
	}

	if (p_ptrail_new && p_pntrail && p_ftrail_new)
		return 1;

	return 0;
}

void MTIPathOriginList::AddFirstPTrail(STLPNT3D & iPTrail, STLVECTOR & iPNTrail, FList iFTrail)
{
	DNum = 1;
	TNum = 1;

	PTrail = (STLPNT3D*)malloc(2 * sizeof(STLPNT3D));
	PTrail[1] = iPTrail;
	PNTrail = (STLVECTOR*)malloc(2 * sizeof(STLVECTOR));
	PNTrail[1] = iPNTrail;
	FTrail = (FList*)malloc(2 * sizeof(FList));
	FTrail[1] = iFTrail;

	SNum = (int*)malloc(sizeof(int)*(1 + 1));
	SNum[1] = 1;
	ENum = (int*)malloc(sizeof(int)*(1 + 1));
	ENum[1] = 1;

	PONext = nullptr;
}

void MTIPathOriginList::PushOnePTrail(STLPNT3D & iPTrail, STLVECTOR & iPNTrail, FList iFTrail)
{
	if (DNum == 1 && TNum == 1)
	{
		int n = ENum[1] - SNum[1] + 1;
		ENum[1]++;
		STLPNT3D *ptrail_new = (STLPNT3D*)malloc(sizeof(STLPNT3D) * (n + 1 + 1));
		memcpy(ptrail_new, PTrail, sizeof(STLPNT3D) * (n + 1));
		ptrail_new[ENum[1]] = iPTrail;
		STLVECTOR *pntrail_new = (STLVECTOR*)malloc(sizeof(STLVECTOR) * (n + 1 + 1));
		memcpy(pntrail_new, PNTrail, sizeof(STLVECTOR) * (n + 1));
		pntrail_new[ENum[1]] = iPNTrail;
		FList *ftrail_new = (FList*)malloc(sizeof(FList) * (n + 1 + 1));
		memcpy(ftrail_new, FTrail, sizeof(FList) * (n + 1));
		ftrail_new[ENum[1]] = iFTrail;

		STLPNT3D *ptrail = PTrail;
		STLVECTOR *pntrail = PNTrail;
		FList *ftrail = FTrail;

		PTrail = ptrail_new;
		PNTrail = pntrail_new;
		FTrail = ftrail_new;

		if (ptrail)
		{
			free(ptrail);
			ptrail = nullptr;
		}
		if (pntrail)
		{
			free(pntrail);
			pntrail = nullptr;
		}
		if (ftrail)
		{
			free(ftrail);
			ftrail = nullptr;
		}
	}
	return;
}

void MTIPathOriginList::CalOffsetDir(int iIndex, int iDir, int iNum, STLVECTOR &oOffsetDir, STLVECTOR &oTanDir)
{
	// 测地方向
	STLVECTOR vec_tangent1, vec_tangent2;

	if (iIndex == 1)
		oTanDir = PTrail[iIndex + 1] - PTrail[iIndex];
	else if (iIndex == iNum)
		oTanDir = PTrail[iIndex] - PTrail[iIndex - 1];
	else
	{
		vec_tangent1 = PTrail[iIndex] - PTrail[iIndex - 1];
		vectorNormalize(vec_tangent1);
		vec_tangent2 = PTrail[iIndex + 1] - PTrail[iIndex];
		vectorNormalize(vec_tangent2);
		oTanDir = 0.5 * (vec_tangent1 + vec_tangent2);
	}
	oTanDir = vectorNormalize(oTanDir);
	// 关键点法矢方向
	STLVECTOR vec_normal = PNTrail[iIndex];

	oOffsetDir = oTanDir ^ vec_normal;
	oOffsetDir = vectorNormalize(oOffsetDir);
	if (iDir == -1)
		oOffsetDir = -oOffsetDir;
}

void MTIPathOriginList::GetRelatedFaceOfPTrail(int iIndex, FRelated oRelatedF, int &oRFNum)
{
	int index = -1;
	int ii;
	for (ii = 0; ii < 3; ii++)
	{
		if (IsPointAVertex(PTrail[iIndex], FTrail[iIndex], index)) // 点是顶点
		{
			oRelatedF->Face = FTrail[iIndex]->VertexUsed[index]->FaceUsed->Face;
			oRelatedF->FNext = FTrail[iIndex]->VertexUsed[index]->FaceUsed->FNext;
			oRFNum = FTrail[iIndex]->VertexUsed[index]->FaceNum;
			break;
		}
	}
	if (ii == 3)
	{
		for (ii = 0; ii < 3; ii++) // 点在边上
		{
			if (IsPointOnEdge(PTrail[iIndex], FTrail[iIndex]->EdgeUsed[ii]))
			{
				oRelatedF->Face = FTrail[iIndex]->EdgeUsed[ii]->FaceUsed->Face;
				oRelatedF->FNext = FTrail[iIndex]->EdgeUsed[ii]->FaceUsed->FNext;
				oRFNum = FTrail[iIndex]->EdgeUsed[ii]->FaceNum;
				break;
			}
		}
	}
	if (!oRFNum) // 点在三角形内部
	{
		oRelatedF->Face = FTrail[iIndex];
		oRelatedF->FNext = nullptr;
		oRFNum = 1;
	}
}

// 点到多段线的测地距离（需要传入正方向）
double MTIPathOriginList::CalGeodesicDistancePointToPl(GridModel *iModel, FList iFace, STLPNT3D iPoint, int iDir)
{
	FList f_arr[2] = { nullptr, nullptr };
	f_arr[0] = iFace;
	double ps[2][3] = { 0. };
	memcpy(ps[0], &iPoint, sizeof(STLVECTOR));
	int I = 0;
	double t = 0.;
	int prep = 0;
	double d = Snap(iModel, f_arr, ps, TOLLENGTH, I, t, prep);

	// 获取等距方向
	STLVECTOR postive_dir, tan_dir;
	int n = 0;
	for (int i = 1; i <= DNum; i++)
		n += (ENum[i] - SNum[i] + 1); // 计算需要等距的点数
	CalOffsetDir(I, iDir, n, postive_dir, tan_dir);

	STLPNT3D p;
	memcpy(&p, ps[1], sizeof(STLPNT3D)); // 当前多段线上的点
	STLVECTOR v = iPoint - p;
	if (v * postive_dir < 0) // 如果与等距方向相反，则为负距离
	{
		d *= -1;
	}
	return d;
}

void MTIPathOriginList::Destroy()
{
	if (PTrail)
	{
		free(PTrail);
		PTrail = nullptr;
	}
	if (PNTrail)
	{
		free(PNTrail);
		PNTrail = nullptr;
	}
	if (FTrail)
	{
		free(FTrail);
		FTrail = nullptr;
	}
	if (SNum)
	{
		free(SNum);
		SNum = nullptr;
	}
	if (ENum)
	{
		free(ENum);
		ENum = nullptr;
	}
}

//==============================================================
VList VertexList::SchMinVert(double p[3], double& min)
{
	VList v, V = NULL ;
	FList f ;
	FRelated r ;

	for( r = FaceUsed ; r ; r = r->FNext )
	{
		f = r->Face ;
		if( f )
		{
			v = f->SchMinVert(p, min) ;
			if( v )
				V = v ;
		}
	}

	return V ;
}
//==============================================================
FaceList::FaceList()
{
}

FaceList::~FaceList()
{
}

int FaceList::GetType()
{
	return 60 ;
}

int FaceList::GetNumOfCV(FList f)
{
	int i, j, n = 0 ;

	for( i = 0 ; i < 3 ; i++ )
	{
		for( j = 0 ; j < 3 ; j++ )
		{
			if( VertexUsed[i] == f->VertexUsed[j] )
			{
				n++ ;
				break ;
			}
		}
	}

	return n ;
}

int FaceList::IsAdj(FList f)
{
	return GetNumOfCV(f) == 2 ? 1 : 0 ;
}

int FaceList::IsAdjVertex(FList f)
{
	return GetNumOfCV(f) == 1 ? 1 : 0;
}

void FaceList::GetVertPos(int k, double p[3])
{
	memcpy(p, &VertexUsed[k%3]->Coord.x, sizeof(double[3])) ;

	return ;
}

void FaceList::GetPos(double gc[3], double p[3])
{
	p[0] = gc[0]*VertexUsed[0]->Coord.x+
		   gc[1]*VertexUsed[1]->Coord.x+
		   gc[2]*VertexUsed[2]->Coord.x ;
	p[1] = gc[0]*VertexUsed[0]->Coord.y+
		   gc[1]*VertexUsed[1]->Coord.y+
		   gc[2]*VertexUsed[2]->Coord.y ;
	p[2] = gc[0]*VertexUsed[0]->Coord.z+
		   gc[1]*VertexUsed[1]->Coord.z+
		   gc[2]*VertexUsed[2]->Coord.z ;

	return ;
}

FList FaceList::GetAdj(int i, int j)
{
	int k ;
	FRelated r ;
	EList e ;

	for( k = 0 ; k < 3 ; k++ )
	{
		e = EdgeUsed[k] ;
		if( (e->VertexUsed[0] == VertexUsed[i] &&
			 e->VertexUsed[1] == VertexUsed[j]) ||
			(e->VertexUsed[0] == VertexUsed[j] &&
			 e->VertexUsed[1] == VertexUsed[i]) )
		{
			for( r = e->FaceUsed ; r ; r = r->FNext )
			{
				if( r->Face != this )
					return r->Face ;
			}
		}
	}

	return NULL ;
}

EList FaceList::GetEdge(int i, int j)
{
	int k ;
	EList e ;

	for( k = 0 ; k < 3 ; k++ )
	{
		e = EdgeUsed[k] ;
		if( (e->VertexUsed[0] == VertexUsed[i%3] &&
			 e->VertexUsed[1] == VertexUsed[j%3]) ||
			(e->VertexUsed[0] == VertexUsed[j%3] &&
			 e->VertexUsed[1] == VertexUsed[i%3]) )
			return e ;
	}

	return NULL ; // error
}

VList FaceList::SchMinVert(double p[3], double& min)
{
	int k, I = -1 ;
	double d ;

	for( k = 0 ; k < 3 ; k++ )
	{
		if( VertexUsed[k]->flag == 0 ) // unused only
		{
			d = mathDist(&VertexUsed[k]->Coord.x, p) ;
			if( min > d )
			{
				min = d ;
				I = k ;
			}
		}
	}

	return I<0?NULL:VertexUsed[I] ;
}

void FaceList::GetBarycenter(double center[3])
{
	center[0] = (VertexUsed[0]->Coord.x+VertexUsed[1]->Coord.x+VertexUsed[2]->Coord.x)/3 ;
	center[1] = (VertexUsed[0]->Coord.y+VertexUsed[1]->Coord.y+VertexUsed[2]->Coord.y)/3 ;
	center[2] = (VertexUsed[0]->Coord.z+VertexUsed[1]->Coord.z+VertexUsed[2]->Coord.z)/3 ;

	return ;
}

void FaceList::Prj(double p[3], double prj_p[3])
{
	int k ;
	double gc[3], s = 0. ;

	mathCalTriGCPX(&VertexUsed[0]->Coord.x, 
				   &VertexUsed[1]->Coord.x,
				   &VertexUsed[2]->Coord.x,
				   &NormalVector.x,
				   p,
				   gc) ;
	for( k = 0 ; k < 3 ; k++ )
	{
		if( gc[k] < 0. )
			gc[k] = 0. ;
		s += gc[k] ;
	}
	if( s < 1e-50 )
	{
		memcpy(prj_p, p, sizeof(double[3])) ;
		return ; // error
	}
	for( k = 0 ; k < 3 ; k++ )
		gc[k] /= s ;
	
	prj_p[0] = gc[0]*VertexUsed[0]->Coord.x+
			   gc[1]*VertexUsed[1]->Coord.x+
			   gc[2]*VertexUsed[2]->Coord.x ;
	prj_p[1] = gc[0]*VertexUsed[0]->Coord.y+
			   gc[1]*VertexUsed[1]->Coord.y+
			   gc[2]*VertexUsed[2]->Coord.y ;
	prj_p[2] = gc[0]*VertexUsed[0]->Coord.z+
			   gc[1]*VertexUsed[1]->Coord.z+
			   gc[2]*VertexUsed[2]->Coord.z ;

	return ;
}

// 输入：
//		pivot:平面上一点
//		normal:平面法矢
//		p:三角形内或其边上一点
//		dir:单位方向，指向测地线的终点
//		tol:点重合容差
// 输出：
//		q是三角形边上一点或三角形顶点
//		i==j表示q就是顶点i
//		i!=j表示q在边上，该边的顶点为i和j
// 功能:
//		平面切三角形得到三角形边上一点q，保证q-p与dir同向
// return 0=not found q, 1=found q
int FaceList::PlaneCut(double pivot[3], 
					   double normal[3], 
					   double p[3], 
					   double dir[3], 
					   double tol, 
					   double q[3], 
					   int& i, 
					   int& j)
{
	int k, m;
	double* vs[3], hs[3], h, d1, d2, ps[3][3], Q[3], max = 0.0; // tol

	i = j = -1;

	for (k = 0; k < 3; k++)
	{
		vs[k] = &VertexUsed[k]->Coord.x;
		hs[k] = mathDistPntPlnSide(vs[k], pivot, normal); // original use p
	}

	for (k = 0; k < 3; k++)
	{
		m = k < 2 ? k + 1 : 0;
		if (fabs(hs[k]) < tol &&
			fabs(hs[m]) < tol)
		{
			d1 = mathOProduct2(dir, p, vs[k]);
			d2 = mathOProduct2(dir, p, vs[m]);
			if (d1 >= d2 && d1 > 0.) // nt modify 2022/7/8 old tol
			{
				memcpy(q, vs[k], sizeof(double[3]));
				i = j = k;
				return 1;
			}
			if (d2 >= d1 && d2 > 0.) // nt modify 2022/7/8 old tol
			{
				memcpy(q, vs[m], sizeof(double[3]));
				i = j = m;
				return 1;
			}
			return 0;
		}
	}

	for (k = 0; k < 3; k++)
		mathPrjPntPln(vs[k], pivot, normal, ps[k]); // original use p
	for (k = 0; k < 3; k++)
	{
		m = k < 2 ? k + 1 : 0;
		if (hs[k] * hs[m] < 0.)
		{
			h = hs[k] - hs[m];
			mathMixPnt(ps[k], ps[m], -hs[m] / h, hs[k] / h, Q);
			h = mathOProduct2(dir, p, Q);
			if (max <= h)
			{
				// 原来必须要求交点与dir同向, 即初始 max = 0
				memcpy(q, Q, sizeof(double[3]));
				i = k;
				j = m;
				max = h;
			}
		}
	}

	for (k = 0; k < 3; k++)
	{
		if (fabs(hs[k]) < tol)
		{
			h = mathOProduct2(dir, p, vs[k]);
			if (max <= h)
			{
				memcpy(q, vs[k], sizeof(double[3]));
				i = j = k;
				return 1;
			}
		}
	}

	return i < 0 ? 0 : 1;
}

// 返回：
//		-1=退化
//		0=not in triangle
//		1=in interior
//		2=on edge
//		3=coincident with vertex
int FaceList::IsIn(double p[3], double tol)
{
	double *A, *B, *C, q[3], d, AB[3], AC[3], v[3], center[3], s, t ;

	GetBarycenter(center) ;
	mathPrjPntPln(p, center, &NormalVector.x, q) ;
	d = mathDist(p, q) ;
	if( d > tol )
		return 0 ;

	A = &VertexUsed[0]->Coord.x ;
	B = &VertexUsed[1]->Coord.x ;
	C = &VertexUsed[2]->Coord.x ;
	if( mathDist(p, A) < tol || // p is coincident with one vertex
		mathDist(p, B) < tol ||
		mathDist(p, C) < tol )
		return 3 ;

	if( mathChkPntSegmX(p, A, B, tol) == IDIN || // p is on one edge
		mathChkPntSegmX(p, B, C, tol) == IDIN ||
		mathChkPntSegmX(p, C, A, tol) == IDIN )
		return 2 ;

	mathGetVec(A, B, AB) ;
	mathGetVec(A, C, AC) ;
	mathVProduct(AB, &NormalVector.x, v) ; // v perpendicular to AB
	d = mathOProduct(AC, v) ;
	if( fabs(d) < 1e-50 )
		return -1 ; // degenerate
	t = mathOProduct2(v, A, p)/d ;
	if( t < 0. || t > 1. )
		return 0 ;
	mathVProduct(AC, &NormalVector.x, v) ; // v perpendicular to AC
	d = mathOProduct(AB, v) ;
	if( fabs(d) < 1e-50 )
		return -1 ; // degenerate
	s = mathOProduct2(v, A, p)/d ;
	if( s < 0. || s > 1. )
		return 0 ;
	if( s+t < 1. )
		return 1 ;
	else
		return 0 ;
}

int FaceList::IsIn2(double p[3], double tol, double prj_p[3])
{
	double center[3], q[3] ;

	GetBarycenter(center) ;
	mathPrjPntPln(p, center, &NormalVector.x, q) ;
	if( prj_p )
		memcpy(prj_p, q, sizeof(double[3])) ;
	
	return IsIn(q, tol) ;
}

int FaceList::Draw(void* pVI, int drawMode)
{
	return 1 ;
}

int FaceList::Draw2(void* pVI, int drawMode)
{
	MATERIAL material ;
	if( drawMode == dmNormal )
		materialGet2(&material, 9) ;
	else
	if( drawMode == dmPrompt )
		materialGet2(&material, 1) ;
	else
	if( drawMode == dmSelect )
		materialGet2(&material, 41) ;
	materialSet(&material) ;

	viLighting(pVI, 1) ;
	viAddDefaultLight(pVI) ;	
	glBegin(GL_TRIANGLES) ;
		glNormal3dv(&NormalVector.x) ;
		glVertex3dv(&VertexUsed[0]->Coord.x) ;
		glNormal3dv(&NormalVector.x) ;
		glVertex3dv(&VertexUsed[1]->Coord.x) ;
		glNormal3dv(&NormalVector.x) ;
		glVertex3dv(&VertexUsed[2]->Coord.x) ;
	glEnd() ;
	viLighting(pVI, 0) ;
	
	return 1 ;
}
//==============================================================
GridModel::GridModel()
{
	VRoot = NULL ;
	EHead = NULL ;
	FHead = NULL ;
	VerNum = 0 ;
	EdgeNum = 0 ;
	FaceNum = 0 ;
	POLHead = NULL ;//排序后截断前路径//不可分段
	stl_xmin = 1e100 ;
	stl_ymin = 1e100 ;
	stl_zmin = 1e100 ;
	stl_xmax = -1e100 ;
	stl_ymax = -1e100 ;
	stl_zmax = -1e100 ;
	TraverseNum = 0 ;
	MeshResulttag  = 0 ;
	// nt add 2022/6/17
	nPolyline = 0 ;
}

void GridModel::InitFlagAll()
{
	EList e ;

	for( e = EHead ; e ; e = e->ENext )
	{
		if( e->VertexUsed[0] )
			e->VertexUsed[0]->flag = 0 ;
		if( e->VertexUsed[1] )
			e->VertexUsed[1]->flag = 0 ;
	}

	return ;
}

void GridModel::Draw(void* pVI, int drawMode)
{
	MATERIAL material ;
	if( drawMode == dmNormal )
		materialGet2(&material, 9) ;
	else
	if( drawMode == dmPrompt )
		materialGet2(&material, 1) ;
	else
	if( drawMode == dmSelect )
		materialGet2(&material, 41) ;
	materialSet(&material) ;

	FList f ;
	glBegin(GL_TRIANGLES) ;
	for( f = FHead ; f ; f = f->FNext )
	{
		glNormal3dv(&f->NormalVector.x) ;
		glVertex3dv(&f->VertexUsed[0]->Coord.x) ;
		glNormal3dv(&f->NormalVector.x) ;
		glVertex3dv(&f->VertexUsed[1]->Coord.x) ;
		glNormal3dv(&f->NormalVector.x) ;
		glVertex3dv(&f->VertexUsed[2]->Coord.x) ;
	}
	glEnd() ;
	// 绘制三角面片的边界 added by jh
	//glLineWidth(3.f);
	//for (f = FHead; f; f = f->FNext)
	//{
	//	glBegin(GL_LINE_LOOP);
	//	glVertex3dv(&f->VertexUsed[0]->Coord.x);
	//	glVertex3dv(&f->VertexUsed[1]->Coord.x);
	//	glVertex3dv(&f->VertexUsed[2]->Coord.x);
	//	glEnd();
	//}
	//glColor3f(1.0f, 1.0f, 1.0f);


	int i, j, n ;
	POG* pog ;
	glLineWidth(2.f) ;
	for( i = 0 ; i < nPolyline ; i++ )
	{
		n = polylines[i]->pogs.GetSize() ;
		glBegin(GL_LINE_STRIP) ;
			for( j = 0 ; j < n ; j++ )
			{
				pog = (POG*)polylines[i]->pogs[j] ;
				glVertex3dv(pog->p) ;
			}
		glEnd() ;
	}

	for( i = 1 ; i <= TrailNumDum ; i++ )
	{
		POLHead[i]->Draw() ;
	}

	return ;
}

FList GridModel::Pick(PNT3D pos, 
					  VEC3D dir,
					  double r, 
					  PNT3D p, // 输入输出参数
					  double* pz, // 输入输出参数
					  PNT3D gc)
{
	int k ;
	double z ;
	PNT2D ps[3], ori={0.,0.} ;
	PNT3D q ;
	VEC3D X, Y, v ;
	BOX3D box ;
	FList f, picked = NULL ;

	box.min[0] = stl_xmin ;
	box.min[1] = stl_ymin ;
	box.min[2] = stl_zmin ;
	box.max[0] = stl_xmax ;
	box.max[1] = stl_ymax ;
	box.max[2] = stl_zmax ;


	if( mathChkBoxLinInt(&box, pos, dir, r) == IDINT )
	{
		mathGetXYFromZ(dir, X, Y) ;
		for( f = FHead ; f ; f = f->FNext )
		{
			for( k = 0 ; k < 3 ; k++ )
			{
				v[0] = f->VertexUsed[k]->Coord.x-pos[0] ;
				v[1] = f->VertexUsed[k]->Coord.y-pos[1] ;
				v[2] = f->VertexUsed[k]->Coord.z-pos[2] ;
				ps[k][0] = v[0]*X[0]+v[1]*X[1]+v[2]*X[2] ;
				ps[k][1] = v[0]*Y[0]+v[1]*Y[1]+v[2]*Y[2] ;
			}
			if( mathChkOriInTri2D(ps[0], ps[1], ps[2], r) == IDIN )
			{
				mathCalTri2DGCP(ps[0], ps[1], ps[2], ori, gc) ;
				q[0] = f->VertexUsed[0]->Coord.x*gc[0]+
					   f->VertexUsed[1]->Coord.x*gc[1]+
					   f->VertexUsed[2]->Coord.x*gc[2] ;
				q[1] = f->VertexUsed[0]->Coord.y*gc[0]+
					   f->VertexUsed[1]->Coord.y*gc[1]+
					   f->VertexUsed[2]->Coord.y*gc[2] ;
				q[2] = f->VertexUsed[0]->Coord.z*gc[0]+
					   f->VertexUsed[1]->Coord.z*gc[1]+
					   f->VertexUsed[2]->Coord.z*gc[2] ;
				v[0] = q[0]-pos[0] ;
				v[1] = q[1]-pos[1] ;
				v[2] = q[2]-pos[2] ;
				z = v[0]*dir[0]+v[1]*dir[1]+v[2]*dir[2] ;
				if( *pz > z )
				{
					memcpy(p, q, sizeof(PNT3D)) ;
					picked = f ;
					*pz = z ;
				}
			}
		}
	}

	return picked ;
}

int GridModel::CalGeodesicLineOne(FList f, 
							      double p1[3], 
							      double p2[3], 
							      PL** polyline)
{
	*polyline = new Pl() ;
	(*polyline)->arcLen = mathDist(p1, p2) ;
	(*polyline)->pogs.Add(new Pog(2, f, p1)) ;
	(*polyline)->pogs.Add(new Pog(2, f, p2)) ;

	return 1 ; // success
}

int GridModel::CalGeodesicLineAdj(FList f1,
	double p1[3],
	FList f2,
	double p2[3],
	double tol,
	PL** polyline)
{
	int i, j, n = 0, is[2], type = 0;
	double A[3], B[3], v[3], d, q[3], q1[3], q2[3], h1, h2, h;
	void* vef = NULL;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (f1->VertexUsed[i] == f2->VertexUsed[j])
			{
				is[n] = i; // 标记 f1 与 f2 相邻的两点序号, 在 f1 内进行标记
				n++;
				break;
			}
		}
		if (n == 2)
			break;
	}
	if (n != 2)
		return 0; // 错误, 没有找到相邻的两点

	f1->GetVertPos(is[0], A);
	f1->GetVertPos(is[1], B);
	mathGetVec(A, B, v); // v = B-A
	d = mathVecLen(v);
	if (d < tol)
	{
		// 如果共边长度 < tol, 则认为通过AB中点
		// vef = f1->VertexUsed[is[0]] ; // A
		// mathMidPnt(A, B, q) ;
		// type = 2 ;	// face 2, edge 1, vertex 0
		vef = f1->GetEdge(is[0], is[1]);
		mathMidPnt(A, B, q);
		type = 1;
	}
	else
	{
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
		mathPrjPntLin(p1, A, v, q1);
		mathPrjPntLin(p2, A, v, q2);
		h1 = mathDist(p1, q1);
		h2 = mathDist(p2, q2);
		if ((h = h1 + h2) < 1e-10)
			mathMidPnt(q1, q2, q);
		else
			// q = q1*h2/h + q2*h1/h ;
			mathMixPnt(q1, q2, h2 / h, h1 / h, q);

		h = (q[0] - A[0]) * v[0] + (q[1] - A[1]) * v[1] + (q[2] - A[2]) * v[2];
		if (h > tol)
		{
			// q 点在 向量 AB 的正向
			if (h > d - tol) {
				// q 点超出了 B 点
				type = 0;
				vef = f1->VertexUsed[is[1]]; // B
				memcpy(q, B, sizeof(double[3]));
			}
			else // on AB
			{
				type = 1;
				vef = f1->GetEdge(is[0], is[1]);
			}
		}
		else {
			// q 点在 向量 AB 的反向, 超出了 A
			type = 0;
			vef = f1->VertexUsed[is[0]]; // A
			memcpy(q, A, sizeof(double[3]));
		}
	}

	*polyline = new Pl();
	(*polyline)->arcLen = mathDist(p1, q) + mathDist(q, p2);
	(*polyline)->pogs.Add(new Pog(2, f1, p1));
	(*polyline)->pogs.Add(new Pog(type, vef, q));
	(*polyline)->pogs.Add(new Pog(2, f2, p2));

	return 1; // success
}

// nt add 2022/7/9
// 找被射线穿过的邻面
FList FaceList::GetAdjRayPass(double p[3], double dir[3], double tol)
{
	int k, n ;
	double v1[3], v2[3], q[3], d ;
	VList v ;
	EList e ;
	FList F ;
	FRelated r ;

	n = IsIn(p, tol) ;
	if( n == 0 )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			e = EdgeUsed[k] ;
			for( r = e->FaceUsed ; r ; r = r->FNext )
			{
				F = r->Face ;
				if( F != this )
				{
					F->Prj(p, q) ;
					if( F->IsIn(q, tol) > 0 )
						return F ;
				}
			}
		}
	}
	else
	if( n == 2 ) // p on edge
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			e = EdgeUsed[k] ;
			for( r = e->FaceUsed ; r ; r = r->FNext )
			{
				F = r->Face ;
				if( F != this &&
					F->IsIn(p, tol) > 0 )
					return F ;
			}
		}
	}
	else
	if( n == 3 ) // p on vertex
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			v = VertexUsed[k] ;
			if( mathDist(p, &v->Coord.x) < tol )
			{
				for( r = v->FaceUsed ; r ; r = r->FNext )
				{
					F = r->Face ;
					if( F != this )
					{
						if( F->VertexUsed[0] == v )
						{
							mathGetVec(p, &F->VertexUsed[1]->Coord.x, v1) ;
							mathGetVec(p, &F->VertexUsed[2]->Coord.x, v2) ;
						}
						else
						if( F->VertexUsed[1] == v )
						{
							mathGetVec(p, &F->VertexUsed[0]->Coord.x, v1) ;
							mathGetVec(p, &F->VertexUsed[2]->Coord.x, v2) ;
						}
						else
						{
							mathGetVec(p, &F->VertexUsed[0]->Coord.x, v1) ;
							mathGetVec(p, &F->VertexUsed[1]->Coord.x, v2) ;
						}
						mathVProduct(v1, dir, v1) ;
						mathVProduct(v2, dir, v2) ;
						d = mathOProduct(v1, v2) ;
						if( d <= 0. )
							return F ;
					}
				}
			}
		}
	}
	else
		return this ; // p在三角形内或外(error)

	return NULL ;
}

// Check if all member of vec is val
static int checkAllEqualVal(double* vec, int n, double val)
{
	int ret = 1;
	for (int i = 0; i < n; i++) {
		if (vec[i] != val) {
			ret = 0;
			return ret;
		}
	}
	return ret;
}
// modified by jh
// 计算两点间的测地线
int GridModel::CalGeodesicLine(FList f1,
	double p1[3],
	FList f2,
	double p2[3],
	double tol,
	PL** polyline,
	int opt)
{
	int i, j, dL;
	double p[3], q[3], dir[3], vec[3], pivot[3], normal[3], h1, h2, q1[3], q2[3];
	FRelated r;
	VList v, v1, v2;
	EList e;
	FList f, F, oldf;

	*polyline = NULL;
	if (f1 == f2)
		return CalGeodesicLineOne(f1, p1, p2, polyline);	// 两个点在同一张面上
	else
		if (f1->IsAdj(f2))
			return CalGeodesicLineAdj(f1, p1, f2, p2, tol, polyline);	// 两张面相邻

	f1->Prj(p1, q1); // nt add 2022/7/9
	f2->Prj(p2, q2); // nt add 2022/7/9

	mathMidPnt(&f1->NormalVector.x, &f2->NormalVector.x, vec); // 平均法矢
	if (mathGetVecUnit(q1, q2, dir) != ERSUCSS || // direction from p1 to p2
		mathUniVec(vec, 1e-10) != ERSUCSS)
		return 0;
	mathVProduct(vec, dir, normal);
	if (mathUniVec(normal, 1e-10) != ERSUCSS)
		return 0;

	*polyline = new Pl();
	(*polyline)->pogs.Add(new Pog(2, f1, q1)); // add start point

	f = f1;
	mathMidPnt(q1, q2, pivot);
	memcpy(p, q1, sizeof(double[3]));
	// nt add 2022/7/9 当f不适合做初始跟踪时，选择其合适的邻面
	if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1)
		f = f->GetAdjRayPass(p, dir, tol);
	if (f == NULL)
		goto ExceExit; // added by jh, 2022-11-21, should goto ExceExit
	// end

	for (dL = 0; dL < FaceNum; dL++)
	{
		if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1)
			goto ExceExit;
		if (i != j) // q on edge
		{
			e = f->GetEdge(i, j);
			if (e == NULL)
				goto ExceExit;
			(*polyline)->pogs.Add(new Pog(1, e, q));
			f = f->GetAdj(i, j);
		}
		else // q on vertex
		{
			v = f->VertexUsed[i];
			if (v == NULL)
				goto ExceExit;
			// modified by jh, 2022-11-21
			// 检查 f2 是否与该顶点临接
			int flag = 0;
			for (r = v->FaceUsed; r; r = r->FNext)
			{
				F = r->Face;
				if (F == f2) {
					if (mathDist(q, q2) < tol) {
						// 如果 q == q2, Add q2
						(*polyline)->pogs.Add(new Pog(2, f2, q2));
						flag = 1;
						break;
					}
					else {
						// 如果 q != q2, Add q, then add q2
						(*polyline)->pogs.Add(new Pog(0, v, q));
						(*polyline)->pogs.Add(new Pog(2, f2, q2));
						flag = 1;
						break;
					}
				}
			}
			if (flag)
				break;

			(*polyline)->pogs.Add(new Pog(0, v, q));
			oldf = f;
			for (r = v->FaceUsed; r; r = r->FNext)
			{
				F = r->Face;
				if (F != oldf)
				{
					if (F->VertexUsed[0] == v)
					{
						v1 = F->VertexUsed[1];
						v2 = F->VertexUsed[2];
					}
					else
						if (F->VertexUsed[1] == v)
						{
							v1 = F->VertexUsed[0];
							v2 = F->VertexUsed[2];
						}
						else
						{
							v1 = F->VertexUsed[0];
							v2 = F->VertexUsed[1];
						}
					h1 = mathDistPntPlnSide(&(v1->Coord.x), pivot, normal);
					h2 = mathDistPntPlnSide(&(v2->Coord.x), pivot, normal);
					if (h1*h2 <= 0.)
					{
						f = F;
						break;
					}
				}
			}
			if (f == oldf) // not found F, not update f
				goto ExceExit;
		}
		if (mathDist(q, p) < tol) {
			goto ExceExit;
		}
		memcpy(p, q, sizeof(double[3]));

		if (f == NULL)
			goto ExceExit;
		if (f == f2)
			break;
	}
	if (dL >= FaceNum)
		goto ExceExit;
	(*polyline)->pogs.Add(new Pog(2, f2, p2)); // add end point

	int iMax = 999;

	if (opt == 1)
		(*polyline)->Opt(tol, tol);
	if (opt == 2)
	{
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}
	if (opt == 3)
	{
		(*polyline)->Opt(tol, tol);
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}


	return 1;
ExceExit:
	if (*polyline)
		delete *polyline;
	*polyline = NULL;

	return 0; // error
}


// theta - The rotation angle of the awerage normal vector, [-pi/2, pi/2)
int GridModel::CalGeodesicLineOptTheta(FList f1, double p1[3], FList f2, double p2[3], double theta, double tol, PL** polyline)
{
	int i, j, dL;
	double p[3], q[3], dir[3], vec[3], pivot[3], normal[3], h1, h2, q1[3], q2[3];
	FRelated r;
	VList v, v1, v2;
	EList e;
	FList f, F, oldf;

	*polyline = NULL;
	if (f1 == f2)
		return CalGeodesicLineOne(f1, p1, p2, polyline);
	else
		if (f1->IsAdj(f2))
			return CalGeodesicLineAdj(f1, p1, f2, p2, tol, polyline);

	f1->Prj(p1, q1); // nt add 2022/7/9
	f2->Prj(p2, q2); // nt add 2022/7/9

	mathMidPnt(&f1->NormalVector.x, &f2->NormalVector.x, vec); // 平均法矢
	if (mathGetVecUnit(q1, q2, dir) != ERSUCSS || // direction from p1 to p2
		mathUniVec(vec, 1e-10) != ERSUCSS)
		return 0;
	mathVProduct(vec, dir, normal);
	if (mathUniVec(normal, 1e-10) != ERSUCSS)
		return 0;

	mathMidPnt(q1, q2, pivot);
	// adjust normal
	mathRotVec(dir, pivot, theta, normal, normal);

	*polyline = new Pl();
	(*polyline)->pogs.Add(new Pog(2, f1, p1)); // add start point

	f = f1;
	memcpy(p, q1, sizeof(double[3]));
	// nt add 2022/7/9 当f不适合做初始跟踪时，选择其合适的邻面
	if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1)
		f = f->GetAdjRayPass(p, dir, tol);
	if (f == NULL)
		return 0;
	// end

	for (dL = 0; dL < FaceNum; dL++)
	{
		if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1)
			goto ExceExit;
		if (i != j) // q on edge
		{
			e = f->GetEdge(i, j);
			if (e == NULL)
				goto ExceExit;
			(*polyline)->pogs.Add(new Pog(1, e, q));
			f = f->GetAdj(i, j);
		}
		else // q on vertex
		{
			v = f->VertexUsed[i];
			if (v == NULL)
				goto ExceExit;
			(*polyline)->pogs.Add(new Pog(0, v, q));
			oldf = f;
			for (r = v->FaceUsed; r; r = r->FNext)
			{
				F = r->Face;
				if (F != oldf)
				{
					if (F->VertexUsed[0] == v)
					{
						v1 = F->VertexUsed[1];
						v2 = F->VertexUsed[2];
					}
					else
						if (F->VertexUsed[1] == v)
						{
							v1 = F->VertexUsed[0];
							v2 = F->VertexUsed[2];
						}
						else
						{
							v1 = F->VertexUsed[0];
							v2 = F->VertexUsed[1];
						}
					h1 = mathDistPntPlnSide(&(v1->Coord.x), pivot, normal);
					h2 = mathDistPntPlnSide(&(v2->Coord.x), pivot, normal);
					if (h1 * h2 <= 0.)
					{
						f = F;
						break;
					}
				}
			}
			if (f == oldf) // not found F, not update f
				goto ExceExit;
		}
		memcpy(p, q, sizeof(double[3]));

		if (f == NULL)
			goto ExceExit;
		if (f == f2)
			break;
	}
	if (dL >= FaceNum)
		goto ExceExit;
	(*polyline)->pogs.Add(new Pog(2, f2, p2)); // add end point

	return 1;
ExceExit:
	if (*polyline)
		delete* polyline;
	*polyline = NULL;

	return 0; // error
}

#define PI 3.14159265358979
int GridModel::CalGeodesicLineOpt(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline, int opt)
{
	// STEP 1 Search for unimodal intervals
	int N = 10;								// N > 1
	double lenStop = PI * 1e-2;
	double* theta = new double[N + 2];		// len = N + 2
	double* f_theta = new double[N + 2];	// len = N + 2
	double* f_theta_f1tof2 = new double[N + 2];	// len = N + 2
	double* f_theta_f2tof1 = new double[N + 2];	// len = N + 2
	double f_theta_minf1tof2 = MAX_DBL, f_theta_minf2tof1 = MAX_DBL;
	for (int i = 0; i < N + 2; i++) {
		theta[i] = -PI / 2 + i * (PI) / (N + 1);
		if (i == N + 1) {
			f_theta_f1tof2[i] = f_theta_f1tof2[0];		// f(-PI/2)=f(PI/2)
			f_theta_f2tof1[i] = f_theta_f2tof1[0];		// f(-PI/2)=f(PI/2)
			continue;
		}
		// f1 to f2
		if (!CalGeodesicLineOptTheta(f1, p1, f2, p2, theta[i], tol, polyline) || !*polyline) {
			// goto ExceExit;
			f_theta_f1tof2[i] = MAX_DBL;
		}
		else {
			(*polyline)->CalArcLen();
			f_theta_f1tof2[i] = (*polyline)->arcLen;
			if (f_theta_f1tof2[i] < f_theta_minf1tof2)
				f_theta_minf1tof2 = f_theta_f1tof2[i];
			delete* polyline;
		}
		// f2 to f1
		if (!CalGeodesicLineOptTheta(f2, p2, f1, p1, theta[i], tol, polyline) || !*polyline) {
			// goto ExceExit;
			f_theta_f2tof1[i] = MAX_DBL;
		}
		else {
			(*polyline)->CalArcLen();
			f_theta_f2tof1[i] = (*polyline)->arcLen;
			if (f_theta_f2tof1[i] < f_theta_minf2tof1)
				f_theta_minf2tof1 = f_theta_f2tof1[i];
			delete* polyline;
		}
	}
	// Select f1tof2 or f2tof1
	// Create new variable
	FList f1_new = NULL, f2_new = NULL;
	double p1_new[3], p2_new[3];

	double* f_theta_min = NULL;
	if (f_theta_minf1tof2 < f_theta_minf2tof1) {
		f1_new = f1;
		f2_new = f2;
		for (int k = 0; k < 3; k++) {
			p1_new[k] = p1[k];
			p2_new[k] = p2[k];
		}
		f_theta_min = f_theta_f1tof2;
	}
	else {
		f1_new = f2;
		f2_new = f1;
		for (int k = 0; k < 3; k++) {
			p1_new[k] = p2[k];
			p2_new[k] = p1[k];
		}
		f_theta_min = f_theta_f2tof1;
	}
	for (int i = 0; i < N + 2; i++)
		f_theta[i] = f_theta_min[i];
	f_theta_min = NULL;
	delete[] f_theta_f1tof2;
	delete[] f_theta_f2tof1;
	// Iter
	double len = theta[N + 1] - theta[0];
	while (len > lenStop) {
		// find min
		int idx = 1;
		double minf_theta = f_theta[idx];
		for (int i = 2; i < N + 1; i++) {
			if (f_theta[i] < minf_theta) {
				idx = i;
				minf_theta = f_theta[i];
			}
		}
		// Update theta
		theta[0] = theta[idx - 1]; f_theta[0] = f_theta[idx - 1];
		theta[N + 1] = theta[idx + 1]; f_theta[N + 1] = f_theta[idx + 1];
		len = theta[N + 1] - theta[0];
		for (int i = 1; i < N + 1; i++) {
			theta[i] = theta[0] + i * (len) / (N + 1);
			if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, theta[i], tol, polyline) || !*polyline) {
				// goto ExceExit;
				f_theta[i] = MAX_DBL;
				continue;
			}
			(*polyline)->CalArcLen();
			f_theta[i] = (*polyline)->arcLen;
			delete* polyline;
		}
	}
	// find interval
	int idx = 1;
	double minf_theta = f_theta[idx];
	for (int i = 2; i < N + 1; i++) {
		if (f_theta[i] < minf_theta) {
			idx = i;
			minf_theta = f_theta[i];
		}
	}
	double a0 = theta[idx - 1], b0 = theta[idx + 1];	// initial interval of the golden section
	len = b0 - a0;
	double f_a0 = f_theta[idx - 1], f_b0 = f_theta[idx + 1];

	if (theta)
		delete[] theta;
	if (f_theta)
		delete[] f_theta;

	// STEP 2 golden section
	lenStop = PI * 1e-4;
	double rho = (3 - sqrt(5)) / 2;
	double a1 = a0 + len * rho, b1 = b0 - len * rho;
	// Cal f_a1
	if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, a1, tol, polyline) || !*polyline)
		goto ExceExit;
	(*polyline)->CalArcLen();
	double f_a1 = (*polyline)->arcLen;
	delete* polyline;
	// Cal f_b1
	if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, b1, tol, polyline) || !*polyline)
		goto ExceExit;
	(*polyline)->CalArcLen();
	double f_b1 = (*polyline)->arcLen;
	delete* polyline;
	// Start Iter
	while (len > lenStop) {
		if (f_a1 < f_b1) {
			b0 = b1; f_b0 = f_b1;
			b1 = a1; f_b1 = f_a1;
			len = b0 - a0;
			a1 = a0 + len * rho;
			if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, a1, tol, polyline) || !*polyline)
				goto ExceExit;
			(*polyline)->CalArcLen();
			f_a1 = (*polyline)->arcLen;
			delete* polyline;
		}
		else {
			a0 = a1; f_a0 = f_a1;
			a1 = b1; f_a1 = f_b1;
			len = b0 - a0;
			b1 = b0 - len * rho;
			if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, b1, tol, polyline) || !*polyline)
				goto ExceExit;
			(*polyline)->CalArcLen();
			f_b1 = (*polyline)->arcLen;
			delete* polyline;
		}
	}

	if (!CalGeodesicLineOptTheta(f1_new, p1_new, f2_new, p2_new, (a0 + b0) / 2, tol, polyline) || !*polyline)
		goto ExceExit;

	int iMax = 999;
	if (opt == 1)
		(*polyline)->Opt(tol, tol);
	if (opt == 2)
	{
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}
	if (opt == 3)
	{
		(*polyline)->Opt(tol, tol);
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}

	return 1;

ExceExit:
	if (*polyline)
		delete* polyline;
	*polyline = NULL;

	return 0; // error
}

// jh add 2022/11/24
// 计算两点间的测地线, 将平均法矢确定的曲面法矢旋转 theta, 加入双向 dir 计算及 dir 的单步更新
int GridModel::CalGeodesicLineThetaNew(FList f1,
	double p1[3],
	FList f2,
	double p2[3],
	double theta,
	double tol,
	PL ** polyline,
	int opt)
{
	calFuncNum++;
	int i, j, idx, dL = 0, iMax, fail, flag;
	double p[3], q[3], dir[3], vec[3], pivot[3], normal[3], h1, h2, q1[3], q2[3];
	FRelated r;
	VList v, v1, v2;
	EList e;
	FList f, F, oldf;
	PL* polyline0 = NULL, *polyline1 = NULL;
	*polyline = NULL;

	if (f1 == f2)
		return CalGeodesicLineOne(f1, p1, p2, polyline);	// 两个点在同一张面上
	else {
		if (f1->IsAdj(f2))
			return CalGeodesicLineAdj(f1, p1, f2, p2, tol, polyline);	// 两张面边相邻
		if (f1->IsAdjVertex(f2)) {
			// 两张面点相邻
			f1->Prj(p2, q1);
			if (mathDist(p2, q1) < tol)
				return CalGeodesicLineOne(f1, p1, q1, polyline);	// 两个点在同一张面上
			f2->Prj(p1, q2);
			if (mathDist(p1, q2) < tol)
				return CalGeodesicLineOne(f2, p2, q2, polyline);	// 两个点在同一张面上
		}
	}
	f1->Prj(p1, q1); // nt add 2022/7/9
	f2->Prj(p2, q2); // nt add 2022/7/9

	mathMidPnt(&f1->NormalVector.x, &f2->NormalVector.x, vec); // 平均法矢
	if (mathGetVecUnit(q1, q2, dir) != ERSUCSS || // direction from p1 to p2
		mathUniVec(vec, 1e-10) != ERSUCSS)
		return 0;
	mathVProduct(vec, dir, normal);
	if (mathUniVec(normal, 1e-10) != ERSUCSS)
		return 0;
	mathMidPnt(q1, q2, pivot);
	// adjust normal
	mathRotVec(dir, pivot, theta, normal, normal);
	// 开始计算
	for (idx = 0; idx < 2; idx++) {
		fail = 0; // fail flag
		mathGetVecUnit(q1, q2, dir);

		*polyline = new Pl();
		(*polyline)->pogs.Add(new Pog(2, f1, q1)); // add start point
		f = f1;
		memcpy(p, q1, sizeof(double[3]));
		if (idx)
			mathVecAdd(-1, dir, 0, dir, dir); // reverse dir
		if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1)
			f = f->GetAdjRayPass(p, dir, tol);
		if (f == NULL) {
			fail = 1;
		}
		else {
			for (dL = 0; dL < FaceNum; dL++)
			{
				if (dL == 118)
					dL = dL;
				if (f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1) {
					fail = 1;
					break;
				}
				if (i != j) // q on edge
				{
					e = f->GetEdge(i, j);
					if (e == NULL) {
						fail = 1;
						break;
					}
					(*polyline)->pogs.Add(new Pog(1, e, q));
					f = f->GetAdj(i, j);
				}
				else // q on vertex
				{
					v = f->VertexUsed[i];
					if (v == NULL) {
						fail = 1;
						break;
					}
					// modified by jh, 2022-11-21
					// 检查 f2 是否与该顶点临接
					flag = 0;
					for (r = v->FaceUsed; r; r = r->FNext)
					{
						F = r->Face;
						if (F == f2) {
							if (mathDist(q, q2) < tol) {
								// 如果 q == q2, Add q2
								(*polyline)->pogs.Add(new Pog(2, f2, q2));
								flag = 1;
								break;
							}
							else {
								// 如果 q != q2, Add q, then add q2
								(*polyline)->pogs.Add(new Pog(0, v, q));
								(*polyline)->pogs.Add(new Pog(2, f2, q2));
								flag = 1;
								break;
							}
						}
					}
					if (flag)
						break;

					(*polyline)->pogs.Add(new Pog(0, v, q));
					oldf = f;
					for (r = v->FaceUsed; r; r = r->FNext)
					{
						F = r->Face;
						if (F != oldf)
						{
							if (F->VertexUsed[0] == v)
							{
								v1 = F->VertexUsed[1];
								v2 = F->VertexUsed[2];
							}
							else
								if (F->VertexUsed[1] == v)
								{
									v1 = F->VertexUsed[0];
									v2 = F->VertexUsed[2];
								}
								else
								{
									v1 = F->VertexUsed[0];
									v2 = F->VertexUsed[1];
								}
							h1 = mathDistPntPlnSide(&(v1->Coord.x), pivot, normal);
							h2 = mathDistPntPlnSide(&(v2->Coord.x), pivot, normal);
							if (h1 * h2 <= 0.)
							{
								f = F;
								break;
							}
						}
					}
					if (f == oldf) {
						// not found F, not update f
						fail = 1;
						break;
					}
				}
				// Update dir
				if (mathDist(p, q) > tol) {
					mathGetVecUnit(p, q, dir);
				}
				memcpy(p, q, sizeof(double[3]));

				if (f == NULL) {
					fail = 1;
					break;
				}
				if (f == f2)
					break;
			}
		}
		if (dL >= FaceNum || fail)
		{
			if (*polyline)
				delete *polyline;
			*polyline = NULL;
			continue;
		}
		(*polyline)->pogs.Add(new Pog(2, f2, p2)); // add end point

		if (idx) {
			polyline1 = *polyline;
			*polyline = NULL;
		}
		else {
			polyline0 = *polyline;
			*polyline = NULL;
		}
	}
	// 比较两个方向的切割结果
	if (polyline0 && polyline1) {
		// 计算出两条 polyine
		polyline0->CalArcLen();
		polyline1->CalArcLen();
		if (polyline0->arcLen <= polyline1->arcLen)
		{
			*polyline = polyline0;
			delete polyline1;
		}
		else {
			*polyline = polyline1;
			delete polyline0;
		}
	}
	else if (polyline0)
		*polyline = polyline0;
	else if (polyline1)
		*polyline = polyline1;
	else {
		return 0;
	}

	iMax = 999;

	if (opt == 1)
		(*polyline)->Opt(tol, tol);
	if (opt == 2)
	{
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}
	if (opt == 3)
	{
		(*polyline)->Opt(tol, tol);
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}


	return 1;
}

int GridModel::calFuncNum = 0;

// jh add 2022/11/24
// 计算两点间的测地线, 对切割平面的方向做进一步优化
int GridModel::CalGeodesicLineOptNew(FList f1,
	double p1[3],
	FList f2,
	double p2[3],
	double tol,
	PL ** polyline,
	int opt)
{
#define PI 3.14159265358979
#define SEARCHNUM 6
#define INSERTEDNUM 10

	calFuncNum = 0;
	// STEP 1 Search for unimodal intervals
	int iSearchMax = SEARCHNUM;				// Max search number
	int N = INSERTEDNUM;					// N > 1, Inserted node number
											// 第一次插入节点, 数目尽可能多, 增加命中
	int idx;
	double lenStop = PI * 1e-6;
	double* theta = NULL;					// len = N + 2
	double* f_theta = NULL;					// len = N + 2

	int it = 0;
	for (it = 0; it < iSearchMax; it++) {
		theta = new double[N + 2];			// len = N + 2
		f_theta = new double[N + 2];		// len = N + 2
		for (int i = 0; i < N + 2; i++) {
			theta[i] = -PI / 2 + i * (PI) / (N + 1);
			if (i == N + 1) {
				f_theta[i] = f_theta[0];
				continue;
			}
			// f1 to f2
			if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, theta[i], tol, polyline, 0) || !*polyline) {
				// goto ExceExit;
				f_theta[i] = MAX_DBL;
			}
			else {
				(*polyline)->CalArcLen();
				f_theta[i] = (*polyline)->arcLen;
				delete* polyline;
				*polyline = NULL;
			}
		}
		// Check
		if (checkAllEqualVal(f_theta, N + 2, MAX_DBL)) {
			// Need to reduce step size
			delete[] theta; theta = NULL;
			delete[] f_theta; f_theta = NULL;
			N *= 2;
		}
		else
			break;
	}
	if (iSearchMax == it) {
		// 搜索 iSearchMax 次, 没有成功
		goto ExceExit;
	}

	// 迭代, 找到 f_theta 对应的最小的 theta, 在其相邻区间内插值, 细化区间
	// 注意处理首末为最小 f_theta 的情况
	double len = theta[N + 1] - theta[0];
	double min_theta = 0.0, minf_theta = 0.0;
	while (len > PI * 0.01) {
		// find min
		idx = 0;
		minf_theta = f_theta[idx];
		for (int i = 1; i < N + 2; i++) {
			if (f_theta[i] < minf_theta) {
				idx = i;
				min_theta = theta[idx];
				minf_theta = f_theta[i];
			}
		}
		// Update theta
		if (idx == 0) {
			theta[0] = theta[0]; f_theta[0] = f_theta[0];
			theta[N + 1] = theta[1]; f_theta[N + 1] = f_theta[1];
		}
		else if (idx == N + 1) {
			theta[0] = theta[N]; f_theta[0] = f_theta[N];
			theta[N + 1] = theta[N + 1]; f_theta[N + 1] = f_theta[N + 1];
		}
		else {
			theta[0] = theta[idx - 1]; f_theta[0] = f_theta[idx - 1];
			theta[N + 1] = theta[idx + 1]; f_theta[N + 1] = f_theta[idx + 1];
		}
		len = theta[N + 1] - theta[0];
		for (int i = 1; i < N + 1; i++) {
			theta[i] = theta[0] + i * (len) / (N + 1);
			if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, theta[i], tol, polyline, 0) || !*polyline) {
				// goto ExceExit;
				f_theta[i] = MAX_DBL;
				continue;
			}
			(*polyline)->CalArcLen();
			f_theta[i] = (*polyline)->arcLen;
			delete* polyline;
		}
		// 检查更新后的角度是否均能求出结果
		if (checkAllEqualVal(f_theta, N + 2, MAX_DBL))
		{
			// 所有的角度均求不出结果, 则取更新前的 min_theta ,进行求解
			if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
				goto ExceExit;
			}
			return 1;
		}
	}
	// find interval
	idx = 0;
	minf_theta = f_theta[idx];
	for (int i = 1; i < N + 2; i++) {
		if (f_theta[i] < minf_theta) {
			idx = i;
			minf_theta = f_theta[i];
		}
	}
	min_theta = theta[idx];	// 更新 min_theta

	if (idx == 0 || idx == N + 1) {
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		if (theta)
			delete[] theta;
		if (f_theta)
			delete[] f_theta;
		return 1;
	}

	double a0 = theta[idx - 1], b0 = theta[idx + 1];	// initial interval of the golden section
	len = b0 - a0;
	double f_a0 = f_theta[idx - 1], f_b0 = f_theta[idx + 1];
	if (f_a0 == MAX_DBL ||
		f_b0 == MAX_DBL ||
		fabs(f_a0 - minf_theta) / fabs(minf_theta) > 1.0 ||
		fabs(f_b0 - minf_theta) / fabs(minf_theta) > 1.0) {
		// interval endpoint abnormal
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		if (theta)
			delete[] theta;
		if (f_theta)
			delete[] f_theta;
		return 1;
	}

	if (theta) {
		delete[] theta;
		theta = NULL;
	}
	if (f_theta) {
		delete[] f_theta;
		f_theta = NULL;
	}

	// STEP 2 golden section
	lenStop = PI * 1e-4;
	double rho = (3 - sqrt(5)) / 2;
	double a1 = a0 + len * rho, b1 = b0 - len * rho;
	// Cal f_a1
	if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, a1, tol, polyline, 0) || !*polyline) {
		// Fail, then try min_theta
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		return 1;
	}
	(*polyline)->CalArcLen();
	double f_a1 = (*polyline)->arcLen;
	delete* polyline;
	// Cal f_b1
	if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, b1, tol, polyline, 0) || !*polyline) {
		// Fail, then try min_theta
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		return 1;
	}
	(*polyline)->CalArcLen();
	double f_b1 = (*polyline)->arcLen;
	delete* polyline;
	// modified by jh, 2023-1-16
	if ((fabs(f_a1 - minf_theta) / fabs(minf_theta) > 1.0) ||
		(fabs(f_b1 - minf_theta) / fabs(minf_theta) > 1.0))
	{
		// Fail, then try min_theta
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		return 1;
	}

	// Start Iter
	while (len > lenStop ||
		!(fabs(f_a0 - f_a1) < tol && fabs(f_b0 - f_b1) < tol)) {
		if (f_a1 < f_b1) {
			b0 = b1; f_b0 = f_b1;
			b1 = a1; f_b1 = f_a1;
			len = b0 - a0;
			a1 = a0 + len * rho;
			if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, a1, tol, polyline, 0) || !*polyline) {
				// Fail, then try min_theta
				if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
					goto ExceExit;
				}
				return 1;
			}
			(*polyline)->CalArcLen();
			f_a1 = (*polyline)->arcLen;
			delete* polyline;
			if (fabs(f_a1 - minf_theta) / fabs(minf_theta) > 1.0)
			{
				// Fail, then try min_theta
				if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
					goto ExceExit;
				}
				return 1;
			}
		}
		else {
			a0 = a1; f_a0 = f_a1;
			a1 = b1; f_a1 = f_b1;
			len = b0 - a0;
			b1 = b0 - len * rho;
			if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, b1, tol, polyline, 0) || !*polyline) {
				// Fail, then try min_theta
				if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
					goto ExceExit;
				}
				return 1;
			}
			(*polyline)->CalArcLen();
			f_b1 = (*polyline)->arcLen;
			delete* polyline;
			if (fabs(f_b1 - minf_theta) / fabs(minf_theta) > 1.0)
			{
				// Fail, then try min_theta
				if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
					goto ExceExit;
				}
				return 1;
			}
		}
	}
	// modified by jh， 2023-1-16
	// 最终结果以 a0 进行计算, (a0+b0)/2 可能会是一个异常的结果
	if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, a0, tol, polyline, 0) || !*polyline) {
		// Fail, then try min_theta
		if (!CalGeodesicLineThetaNew(f1, p1, f2, p2, min_theta, tol, polyline, opt) || !*polyline) {
			goto ExceExit;
		}
		return 1;
	}


	int iMax = 999;
	if (opt == 1)
		(*polyline)->Opt(tol, tol);
	if (opt == 2)
	{
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}
	if (opt == 3)
	{
		(*polyline)->Opt(tol, tol);
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}

	return 1;

ExceExit:
	if (theta)
		delete[] theta;
	if (f_theta)
		delete[] f_theta;
	if (*polyline)
		delete* polyline;
	theta = NULL;
	f_theta = NULL;
	*polyline = NULL;

	return 0; // error
}

// added by jh, 2022/11/24
// 计算两点间的测地线, 末端点在 p21 与 p22 之间游动
int GridModel::CalGeodesicLineFloatTailNew(FList f1,
	double p1[3],
	FList f2,
	double p2[3],
	double p21[3],
	double p22[3],
	double tol,
	PL ** polyline,
	int opt)
{
	// added by jh, 2023-01-05
	// 对 共面, 边相邻, 点相邻 等特殊情况做处理
	double q1[3], q2[3];
	f1->Prj(p2, q1);
	f2->Prj(p1, q2);
	if (f1 == f2)
		CalGeodesicLineOne(f2, p1, p2, polyline);				// 两个点在同一张面上
	else if (f1->IsAdj(f2))
		CalGeodesicLineAdj(f1, p1, f2, p2, tol, polyline);		// 两张面边相邻
	else if (f1->IsAdjVertex(f2) && mathDist(p2, q1) < tol)
		CalGeodesicLineOne(f1, p1, q1, polyline);			// 两张面点相邻
	else if (f1->IsAdjVertex(f2) && mathDist(p1, q2) < tol)
		CalGeodesicLineOne(f2, q2, p2, polyline);			// 两张面点相邻
	else if (!CalGeodesicLineOptNew(f1, p1, f2, p2, tol, polyline, opt)) {
		if (*polyline)
			delete *polyline;
		*polyline = NULL;
		return 0;
	}
	int size = (*polyline)->pogs.GetSize();
	delete (*polyline)->pogs[size - 1];
	(*polyline)->pogs.RemoveAt(size - 1);

	// add end point
	VList vTemp0 = new VertexList;
	VList vTemp1 = new VertexList;
	EList eTemp = new EdgeList;
	memcpy(&(vTemp0->Coord.x), p21, sizeof(double[3]));
	memcpy(&(vTemp1->Coord.x), p22, sizeof(double[3]));
	eTemp->VertexUsed[0] = vTemp0;
	eTemp->VertexUsed[1] = vTemp1;
	eTemp->FaceUsed = NULL;
	(*polyline)->pogs.Add(new Pog(1, eTemp, p2));

	int iMax = 999;

	if (opt == 1)
		(*polyline)->Opt(tol, tol);
	if (opt == 2)
	{
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}
	if (opt == 3)
	{
		(*polyline)->Opt(tol, tol);
		(*polyline)->OptLM(tol, iMax);	// optimize using LM
	}

	delete vTemp0;
	delete vTemp1;
	delete eTemp;

	return 1;
}

bool GridModel::IsPointOnBoundary(STLPNT3D iPoint, FList iSupport)
{
	//if (!MTIPathOriginList::IsPointInTriangle(iPoint, iSupport))
	//	return false; // 不在三角形内
	int index = -1;

	if (MTIPathOriginList::IsPointAVertex(iPoint, iSupport, index))
	{
		VList vertex = iSupport->VertexUsed[index];
		for (size_t i = 0; i < vertex->FaceNum; i++)
		{
			FList face = vertex->FaceUsed[i].Face;
			for (size_t j = 0; j < 3; j++)
			{
				EList edge = face->EdgeUsed[j];
				if (edge->FaceNum == 1 && MTIPathOriginList::IsPointOnEdge(iPoint, edge))
				{
					return true; // 是顶点，且该顶点所在的边没有邻面
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < 3; i++)
		{
			EList edge = iSupport->EdgeUsed[i];

			if (MTIPathOriginList::IsPointOnEdge(iPoint, edge))
			{
				if (edge->FaceNum == 1)
				{
					return true; // 在第i条边上，且该边没有邻面
				}
			}
		}
	}

	return false;
}

POList GridModel::CreateMTIPathFromPL(PL* iPolyLine)
{
	POList res = nullptr;
	if (iPolyLine)
	{
		int trail_num = iPolyLine->pogs.GetSize();
		res = (POList)malloc(sizeof(struct MTIPathOriginList));
		if (res == nullptr || trail_num < 2)
			return res;

		res->TNum = 1;
		res->DNum = 1;
		res->SNum = (int*)malloc(sizeof(int)*(2));
		res->ENum = (int*)malloc(sizeof(int)*(2));
		res->SNum[1] = 1;
		res->ENum[1] = trail_num;

		res->PTrail = (STLPNT3D*)malloc(sizeof(STLPNT3D)*(trail_num + 1));
		res->PNTrail = (STLVECTOR*)malloc(sizeof(STLVECTOR)*(trail_num + 1));
		res->FTrail = (FList*)malloc(sizeof(FList)*(trail_num + 1));

		res->PONext = nullptr;

		for (size_t i = 0; i < trail_num; i++)
		{
			memcpy(&res->PTrail[i + 1], ((POG*)iPolyLine->pogs[i])->p, sizeof(STLPNT3D));
			res->PNTrail[i + 1] = CalAllVertexNormalVector(res->PTrail[i + 1], TOLLENGTH);
			res->FTrail[i + 1] = FindClosetFace(res->PTrail[i + 1], TOLLENGTH);
		}
	}

	return res;
}

// p1在f1上，p2在f2上
// ptOnLine在p1,p2的连线上
// 将ptOnLine投影到网格的某个Face上，输出投影点, 返回该Face
FList GridModel::Prj(FList f1, 
					 double p1[3], 
					 FList f2, 
					 double p2[3], 
					 double tol, 
					 double ptOnLine[3],
				     double ptOnFace[3])
{
	int i, j, dL ;
	double p[3], q[3], dir[3], vec[3], pivot[3], normal[3], h1, h2 ;
	FRelated r ;
	VList v, v1, v2 ;
	EList e ;
	FList f, F, oldf ;

	if( f1->IsIn2(ptOnLine, tol, ptOnFace) > 0 )
		return f1 ;
	else
	if( f2->IsIn2(ptOnLine, tol, ptOnFace) > 0 )
		return f2 ;
	
	mathMidPnt(&f1->NormalVector.x, &f2->NormalVector.x, vec) ; // 平均法矢
	if( mathGetVecUnit(p1, p2, dir) != ERSUCSS || // direction from p1 to p2
		mathUniVec(vec, 1e-10) != ERSUCSS )
		return NULL ;
	mathVProduct(vec, dir, normal) ;
	if( mathUniVec(normal, 1e-10) != ERSUCSS )
		return NULL ;
	
	f = f1 ;
	mathMidPnt(p1, p2, pivot) ;
	memcpy(p, p1, sizeof(double[3])) ;
	for( dL = 0 ; dL < FaceNum ; dL++ )
	{
		if( f->PlaneCut(pivot, normal, p, dir, tol, q, i, j) != 1 )
			return NULL ;
		if( i != j ) // q on edge
		{
			e = f->GetEdge(i, j) ;
			if( e == NULL )
				return NULL ;
			f = f->GetAdj(i, j) ;
		}
		else // q on vertex
		{
			v = f->VertexUsed[i] ;
			if( v == NULL )
				return NULL ;
			oldf = f ;
			for( r = v->FaceUsed ; r ; r = r->FNext )
			{
				F = r->Face ;
				if( F != oldf )
				{
					if( F->VertexUsed[0] == v )
					{
						v1 = F->VertexUsed[1] ;
						v2 = F->VertexUsed[2] ;
					}
					else
					if( F->VertexUsed[1] == v )
					{
						v1 = F->VertexUsed[0] ;
						v2 = F->VertexUsed[2] ;
					}
					else
					{
						v1 = F->VertexUsed[0] ;
						v2 = F->VertexUsed[1] ;
					}
					h1 = mathDistPntPlnSide(&(v1->Coord.x), pivot, normal) ;
					h2 = mathDistPntPlnSide(&(v2->Coord.x), pivot, normal) ;
					if( h1*h2 <= 0. )
					{
						f = F ;
						break ;
					}
				}
			}
			if( f == oldf ) // not found F, not update f
				return NULL ;
		}
		memcpy(p, q, sizeof(double[3])) ;

		if( f == NULL )
			return NULL ;
		if( f == f2 )
			break ;
		if( f->IsIn2(ptOnLine, tol, ptOnFace) > 0 )
			return f ;
	}

	return NULL ;
}

int GridModel::CalGDInfo(POList polys[2], double tol, GDINFO** pGDI)
{
	int i, k, n ;
	GDINFO* gdi ;
	PP* pp ;

	*pGDI = NULL ;
	gdi = new GDINFO() ;
	gdi->min = 1e50 ;
	gdi->max = -1e50 ;
	gdi->i = 0 ;
	gdi->I = 0 ;

	for( k = 0 ; k < 2 ; k++ )
	{
		for( n = 0, i = 1 ; i <= polys[k]->DNum ; i++ )
			n += (polys[k]->ENum[i]-polys[k]->SNum[i]+1) ;
		gdi->ns[k] = n ;
		gdi->pps[k] = new PP[n+1] ;
		gdi->pps[k][0].I = -1 ; // set invalid
	
		for( i = 1 ; i <= n ; i++ )
		{
			if( i == 2 && k == 1 )
				i = i ;
			pp = &gdi->pps[k][i] ;
			pp->fs[0] = polys[k]->FTrail[i] ; // 面
			memcpy(pp->ps[0], &polys[k]->PTrail[i].x, sizeof(double[3])) ; // 点
			// nt modify 2022/7/4 add pp->perp
			pp->d = polys[1-k]->Snap(this, pp->fs, pp->ps, tol, pp->I, pp->t, pp->perp) ;
			if( pp->I > 0 &&
				pp->perp == 1 ) // nt add 2022/7/4 只用垂直的情况更新min,max!!!
			{
				if( gdi->min > pp->d )
				{
					gdi->min = pp->d ;
					gdi->i = k?-i:i ;
				}
				if( gdi->max < pp->d )
				{
					gdi->max = pp->d ;
					gdi->I = k?-i:i ;
				}
			}
		}
	}
	*pGDI = gdi ;

	return 1 ;
}
//==============================================================
/*	2022/6/18 nt
 *	功能：计算点到polyline的最近距离
 *	输入：
 *		pGM : GridModel
 *		fs[0] : Flist which ps[0] is on
 *		ps[0] : a point on the grid
 *		tol : 点重合容差
 *	输出:
 *		ps[1] : a point on this polyline
 *		fs[1] : FList which ps[1] is on
 *		I : index of PTrail
 *		t : [0.,1.), ps[1] = (1-t)*PTrail[i]+t*PTrail[i+1], i is the return value
 *		perp : 1=垂直,0=非垂直
 *	返回:
 *		最近距离
 *	nt modify 2022/7/9 use Prj()
 */
double MTIPathOriginList::Snap(GridModel* pGM,
	FList fs[2],
	double ps[2][3],
	double tol,
	int& I,
	double& t,
	int& perp)
{
	int i, n = 0, PERP;
	double min = 1e50, d, s, q[3];
	Pl* polyline = NULL;

	fs[1] = NULL;
	t = 0.;
	I = 0;
	perp = 0; // nt add 2022/7/4, 0 or 1

	// 点的下标从1到n
	for (i = 1; i <= DNum; i++)
		n += (ENum[i] - SNum[i] + 1); // i段实际点数=ENum[i]-SNum[i]+1

	if (n < 2)
		return -1.; // error

	for (i = 1; i < n; i++)
	{
		//d = mathGetPntSegmNDT(ps[0], &PTrail[i].x, &PTrail[i+1].x, &s) ;
		// nt modify 2022/7/4
		d = mathGetPntSegmNDTPerp(ps[0], &PTrail[i].x, &PTrail[i + 1].x, tol, &s, &PERP);
		//if( (PERP == perp && min > d) || // nt add 2022/7/4, 尽量保留垂直的结果
		//	PERP > perp )
		if (min > d)
		{
			// modified by jh, 2023/01/07 取消尽量保留垂直的约束
			mathMixPnt(&PTrail[i].x, &PTrail[i + 1].x, 1. - s, s, ps[1]);
			min = d;
			t = s;
			I = i;
			perp = PERP;
		}
	}
	if (I < 1) // I一定<n
		return -1.; // error

	// nt add 2022/7/9
	fs[1] = pGM->Prj(FTrail[I], &PTrail[I].x, FTrail[I + 1], &PTrail[I + 1].x, tol, ps[1], q);
	if (fs[1] == NULL)
		return -1.; // error=ps[1]投影不在网格面上
	if (min < tol)
		return min;

	if (pGM->CalGeodesicLineFloatTailNew(fs[0], ps[0], fs[1], q, &PTrail[I].x, &PTrail[I + 1].x, tol, &polyline, 3) < 1 ||
		polyline == NULL)
	{
		fs[1] = NULL;
		t = 0.;
		I = 0;
		return -1.; // error
	}
	// added by jh, Update param t of tail point
	int num = polyline->pogs.GetSize();
	POG* pog = (POG*)polyline->pogs[num - 1];
	VEC3D v1, v2;
	for (int k = 0; k < 3; k++) {
		v1[k] = *(&PTrail[i + 1].x + k) - *(&PTrail[i].x + k);
		v2[k] = pog->p[k] - *(&PTrail[i].x + k);
	}
	t = mathVecLen(v2) / mathVecLen(v1);
	t = t < 0.0 ? 0.0 : (t > 1.0 ? 1.0 : t);
	// added by jh, Update ps[1]
	memcpy(ps[1], pog->p, sizeof(double[3]));
	// polyline->ReplaceEnd(ps[1]) ; // nt add 2022/7/9
	polyline->CalArcLen();
	d = polyline->arcLen;
	// release polyline
	 delete polyline ;
	//pGM->polylines[pGM->nPolyline] = polyline; // debug only
	//(pGM->nPolyline)++; // debug only

	return d;
}
//==============================================================
BOOL GridModel::stlVectorNormal(STLVECTOR *v1)
{
	double alph =stlDistanceOneVector(*v1);
	if ( alph < 1.e-4) {
		return FALSE;
	}
	*v1 = stlTwoVectorDivide(*v1,alph);
	return TRUE;
}

FList GridModel::stlGetFacetFromNum(int N)
{
	FList f = FHead;
	for (int i = 1;i <= FaceNum; i ++)
	{
		if (f ->FaceNO == N)return f;
		else if(f->FNext != NULL)f = f->FNext;
	}
	return f;
}

BOOL GridModel::stlCreateSTLTriangle(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3)
{
	STLVECTOR v1, v2, nv;

	v1 = stlTwoVectorMinus(p2,p1);
	if ( !stlVectorNormal(&v1) ) {
		return FALSE;
	}

	v2 = stlTwoVectorMinus(p3,p2);
	if ( !stlVectorNormal(&v2) ) {
		return FALSE;
	}


	nv = stlTwoVectorProduct(v1,v2);
	if ( !stlVectorNormal(&nv) ) {
		STLVECTOR v3 = stlTwoVectorMinus(p1,p3);
		if ( !stlVectorNormal(&v3) ) {
			return FALSE;
		}
		nv = stlTwoVectorProduct(v3,v1);
		if ( !stlVectorNormal(&nv) ) {
			return FALSE;
		}
	}

	stlCreateFace(&nv, &p1, &p2, &p3,-1);

	return TRUE;
}

FList GridModel::stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3,int tNum)
{
	FList p, temp;

	if (fabs(p1->x-p2->x) < EPS06 && fabs(p1->y-p2->y) < EPS06 && fabs(p1->z-p2->z) < EPS06 ||
		fabs(p1->x-p3->x) < EPS06 && fabs(p1->y-p3->y) < EPS06 && fabs(p1->z-p3->z) < EPS06 ||
		fabs(p3->x-p2->x) < EPS06 && fabs(p3->y-p2->y) < EPS06 && fabs(p3->z-p2->z) < EPS06 ) {
		return NULL;
	}

	FaceNum++;
	p = stlFaceAlloc();
	//默认p1,p2,p3,nv呈右手法则
	/*p->NormalVector.x=nv->x;
	p->NormalVector.y=nv->y;
	p->NormalVector.z=nv->z;*/
	// nt add 2022/7/9
	mathGetTriNormal(&p1->x, &p2->x, &p3->x, &p->NormalVector.x) ; 
	if( mathOProduct(&nv->x, &p->NormalVector.x) < 0. )
	{
		p->NormalVector.x = -p->NormalVector.x ;
		p->NormalVector.y = -p->NormalVector.y ;
		p->NormalVector.z = -p->NormalVector.z ;
	}
	// end

	if(tNum == -1)p->FaceNO = FaceNum;
	else p->FaceNO = tNum;

	p->VertexUsed[0]=stlCreateVertex(&VRoot,p1);
	p->VertexUsed[1]=stlCreateVertex(&VRoot,p2);
	p->VertexUsed[2]=stlCreateVertex(&VRoot,p3);

	p->EdgeUsed[0]=stlCreateEdge(p->VertexUsed[0],p->VertexUsed[1]);
	p->EdgeUsed[1]=stlCreateEdge(p->VertexUsed[1],p->VertexUsed[2]);
	p->EdgeUsed[2]=stlCreateEdge(p->VertexUsed[2],p->VertexUsed[0]);
	
	// 处理 VERTEC 上面的相邻关系
	stlAddFace2Vertex(p,p->VertexUsed[0]);
	stlAddFace2Vertex(p,p->VertexUsed[1]);
	stlAddFace2Vertex(p,p->VertexUsed[2]);

	p->EdgeUsed[0]->FaceCircleTag = 0;
	p->EdgeUsed[1]->FaceCircleTag = 0;
	p->EdgeUsed[2]->FaceCircleTag = 0;

	// 处理 EDGE 上面的相邻关系
	stlAddFace2Edge(p,p->EdgeUsed[1]);
	stlAddFace2Edge(p,p->EdgeUsed[0]);
	stlAddFace2Edge(p,p->EdgeUsed[2]);

	if(FHead == NULL) {
		FHead = p;
	}
	else {
		temp = FHead;
		FHead = p;
		FHead->FNext = temp;
	}

	return p;
}

FList GridModel::stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, STLVECTOR *v1, STLVECTOR *v2, STLVECTOR *v3, int tNum)
{
	FList p, temp;

	if (fabs(p1->x - p2->x) < EPS06 && fabs(p1->y - p2->y) < EPS06 && fabs(p1->z - p2->z) < EPS06 ||
		fabs(p1->x - p3->x) < EPS06 && fabs(p1->y - p3->y) < EPS06 && fabs(p1->z - p3->z) < EPS06 ||
		fabs(p3->x - p2->x) < EPS06 && fabs(p3->y - p2->y) < EPS06 && fabs(p3->z - p2->z) < EPS06) {
		return NULL;
	}

	FaceNum++;
	p = stlFaceAlloc();
	//默认p1,p2,p3,nv呈右手法则
	/*p->NormalVector.x=nv->x;
	p->NormalVector.y=nv->y;
	p->NormalVector.z=nv->z;*/
	// nt add 2022/7/9
	mathGetTriNormal(&p1->x, &p2->x, &p3->x, &p->NormalVector.x);
	if (mathOProduct(&nv->x, &p->NormalVector.x) < 0.)
	{
		p->NormalVector.x = -p->NormalVector.x;
		p->NormalVector.y = -p->NormalVector.y;
		p->NormalVector.z = -p->NormalVector.z;
	}
	// end

	if (tNum == -1)p->FaceNO = FaceNum;
	else p->FaceNO = tNum;

	p->VertexUsed[0] = stlCreateVertex(&VRoot, p1, v1);
	p->VertexUsed[1] = stlCreateVertex(&VRoot, p2, v2);
	p->VertexUsed[2] = stlCreateVertex(&VRoot, p3, v3);

	p->EdgeUsed[0] = stlCreateEdge(p->VertexUsed[0], p->VertexUsed[1]);
	p->EdgeUsed[1] = stlCreateEdge(p->VertexUsed[1], p->VertexUsed[2]);
	p->EdgeUsed[2] = stlCreateEdge(p->VertexUsed[2], p->VertexUsed[0]);

	// 处理 VERTEC 上面的相邻关系
	stlAddFace2Vertex(p, p->VertexUsed[0]);
	stlAddFace2Vertex(p, p->VertexUsed[1]);
	stlAddFace2Vertex(p, p->VertexUsed[2]);

	p->EdgeUsed[0]->FaceCircleTag = 0;
	p->EdgeUsed[1]->FaceCircleTag = 0;
	p->EdgeUsed[2]->FaceCircleTag = 0;

	// 处理 EDGE 上面的相邻关系
	stlAddFace2Edge(p, p->EdgeUsed[1]);
	stlAddFace2Edge(p, p->EdgeUsed[0]);
	stlAddFace2Edge(p, p->EdgeUsed[2]);

	if (FHead == NULL) {
		FHead = p;
	}
	else {
		temp = FHead;
		FHead = p;
		FHead->FNext = temp;
	}

	return p;
}

FList GridModel::stlFaceAlloc()
{
	FList p;

	//p = (FList)malloc(sizeof(struct FaceList));
	p = new FaceList() ; // nt add 2022/6/15
	if (p==NULL) {
		exit(1);
	}
	p->FCircleTag = 0.0;
	p->FaceNO = -1;
	p->FNext = NULL;
	return p;
}

// nt modify 2022/6/15
VList GridModel::stlCreateVertex(VList *root, STLPNT3D *pt)
{
	STLPNT3D q ;

	q.x = /*0.001**/pt->x ;
	q.y = /*0.001**/pt->y ;
	q.z = /*0.001**/pt->z ;

	if(q.x < stl_xmin) 
		stl_xmin = (double)(q.x);
	if(q.x > stl_xmax) 
		stl_xmax = (double)(q.x);
	if(q.y < stl_ymin) 
		stl_ymin = (double)(q.y);
	if(q.y > stl_ymax) 
		stl_ymax = (double)(q.y);
	if(q.z < stl_zmin) 
		stl_zmin = (double)(q.z);
	if(q.z > stl_zmax) 
		stl_zmax = (double)(q.z);

	return stlSortTree(&q,root);
}

VList GridModel::stlCreateVertex(VList *root, STLPNT3D *pt, STLVECTOR *vt)
{
	STLPNT3D q;

	q.x = /*0.001**/pt->x;
	q.y = /*0.001**/pt->y;
	q.z = /*0.001**/pt->z;

	if (q.x < stl_xmin)
		stl_xmin = (double)(q.x);
	if (q.x > stl_xmax)
		stl_xmax = (double)(q.x);
	if (q.y < stl_ymin)
		stl_ymin = (double)(q.y);
	if (q.y > stl_ymax)
		stl_ymax = (double)(q.y);
	if (q.z < stl_zmin)
		stl_zmin = (double)(q.z);
	if (q.z > stl_zmax)
		stl_zmax = (double)(q.z);

	return stlSortTree(&q, vt, root);
}

VList GridModel::stlSortTree(STLPNT3D *pt, VList *t)  //二叉排序树
{
	VList	 p,p1;  //p作为遍历节点使用,p1作为返回指针使用 
	int	   Bool;
	if(*t == NULL)  //树为空
	{
		*t = p1 = stlVertexAlloc();
		p1->Coord = *pt; 
		VerNum++;
	} 
	else   //树不为空
	{
		p = *t;  //p作为遍历节点使用
		Bool = 1; 
		do 
		{
			if (pt->z < p->Coord.z)   //如果需要插入的z值小于当前节点的z值,插入左子树
			{
				if(p->Lchild != NULL)  //左子树不空
				{
					p = p->Lchild;
				}
				else 
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt; 
					p->Lchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else if(pt->z > p->Coord.z) 
			{
				if(p->Rchild != NULL) 
				{
					p = p->Rchild;
				}
				else 
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt; 
					p->Rchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else 
			{
				if(pt->x == p->Coord.x && pt->y == p->Coord.y)  //完全相同
				{
					p1 = p;
					Bool = 0;
				}
				else   //插入左子树
				{
					if(p->Lchild != NULL) 
					{
						p = p->Lchild;
					}
					else 
					{
						p1 = stlVertexAlloc();
						p1->Coord = *pt; 
						p->Lchild = p1;
						VerNum++;
						Bool = 0;
					}
				}
			}
		}
		while(Bool);
	}
	return p1;
}

VList GridModel::stlSortTree(STLPNT3D *pt, STLVECTOR *vt, VList *t)
{
	VList	 p, p1;  //p作为遍历节点使用,p1作为返回指针使用 
	int	   Bool;
	if (*t == NULL)  //树为空
	{
		*t = p1 = stlVertexAlloc();
		p1->Coord = *pt;
		p1->NormalVector = *vt;
		VerNum++;
	}
	else   //树不为空
	{
		p = *t;  //p作为遍历节点使用
		Bool = 1;
		do
		{
			if (pt->z < p->Coord.z)   //如果需要插入的z值小于当前节点的z值,插入左子树
			{
				if (p->Lchild != NULL)  //左子树不空
				{
					p = p->Lchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Lchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else if (pt->z > p->Coord.z)
			{
				if (p->Rchild != NULL)
				{
					p = p->Rchild;
				}
				else
				{
					p1 = stlVertexAlloc();
					p1->Coord = *pt;
					p->Rchild = p1;
					VerNum++;
					Bool = 0;
				}
			}
			else
			{
				if (pt->x == p->Coord.x && pt->y == p->Coord.y)  //完全相同
				{
					p1 = p;
					Bool = 0;
				}
				else   //插入左子树
				{
					if (p->Lchild != NULL)
					{
						p = p->Lchild;
					}
					else
					{
						p1 = stlVertexAlloc();
						p1->Coord = *pt;
						p->Lchild = p1;
						VerNum++;
						Bool = 0;
					}
				}
			}
		} while (Bool);
	}
	return p1;
}

VList GridModel::stlVertexAlloc()
{
	VList p;
	p = (VList)malloc(sizeof(struct VertexList));
	if (p==NULL) 
	{
		exit(1);
	}
	p->Lchild = NULL;
	p->Rchild = NULL;
	p->FaceUsed = NULL;
	p->FaceNum = 0;
	return p;
}
EList GridModel::stlCreateEdge(VList p1, VList p2)
{
	EList   e, e_new;
	FRelated fr1, fr2;
	int    i, j, k;
	fr1 = p1->FaceUsed;
	fr2 = p2->FaceUsed;
	if(fr1 != NULL && fr2 != NULL) 
	{
		// 两个点均关联了一些面, 握手遍历
		for(i=0; i< p1->FaceNum; i++) 
		{
			fr2 = p2->FaceUsed;
			for(j=0; j<p2->FaceNum; j++) 
			{
				// 两个点关联到同一张面
				if(fr1->Face == fr2->Face) //如果两个点相关联的面相同
				{
					// 将该面的三个边取出来, 如果与 p1 p2 组成的边相同, 直接返回该边
					for(k=0; k<3; k++) 
					{
						e = fr1->Face->EdgeUsed[k];
						if (e->VertexUsed[0]==p2 && e->VertexUsed[1]==p1) 
							return e;
						else if (e->VertexUsed[0]==p1 && e->VertexUsed[1]==p2) 
							return e;
					}
				}
				fr2 = fr2->FNext;
			}
			fr1 = fr1->FNext;
		} 
	}
	e_new = stlEdgeAlloc();
	e_new->VertexUsed[0] = p1;
	e_new->VertexUsed[1] = p2;
	e = EHead;
	EHead = e_new;
	EHead->ENext = e;
	EdgeNum++;
	return EHead;
}

EList GridModel::stlEdgeAlloc()
{
	EList p;
	p = (EList)malloc(sizeof(struct EdgeList));
	if (p==NULL) 
	{
		exit(1);
	}
	p->ENext = NULL;
	p->FaceUsed = NULL;
	p->FaceNum = 0;
	p->FaceCircleTag = 0;
	p->FCircleTag = 0.0;
	return p;
}

void GridModel::stlAddFace2Vertex(FList f, VList v)
{
	FRelated fr, fnew;
	fnew = (FRelated)malloc(sizeof(struct FaceRelated));
	if (fnew==NULL) 
	{
		exit(1);
	}
	fnew->Face = f;
	if (v->FaceUsed == NULL) 
	{
		v->FaceUsed = fnew;
		fnew->FNext = NULL;
	}
	else 
	{
		fr = v->FaceUsed;
		v->FaceUsed = fnew;
		fnew->FNext = fr;
	}
	(v->FaceNum)++;
}

void GridModel::stlAddFace2Edge(FList f, EList e)
{
	FRelated fr,fnew;
	fnew = (FRelated)malloc(sizeof(struct FaceRelated));
	if (fnew==NULL) 
	{
		exit(1);
	}
	fnew->Face = f;
	if (e->FaceUsed == NULL) 
	{
		e->FaceUsed = fnew;
		fnew->FNext = NULL;
	}
	else 
	{
		fr = e->FaceUsed;
		e->FaceUsed = fnew;
		fnew->FNext = fr;
	}
	(e->FaceNum)++;
}

BOOL GridModel::stlCompareTwo3DPoint(STLPNT3D p3d1, STLPNT3D p3d2, double epsdis)
{
	BOOL equaled = FALSE;
	if (fabs(p3d1.x - p3d2.x) < epsdis)
	{
		if (fabs(p3d1.y - p3d2.y) < epsdis)
		{
			if (fabs(p3d1.z - p3d2.z) < epsdis)
			{
				equaled = TRUE;
			}
		}
	}
	return equaled;
}

double GridModel::stlDistanceTwoPoints(STLPNT3D p1, STLPNT3D p2)
{
	double dd = stlDistanceOneVector(stlTwoVectorMinus(p1,p2));
	return dd;
}

int GridModel::stlCompareTriangleVertex(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3, STLPNT3D q1, STLPNT3D q2, STLPNT3D q3)
{
	STLPNT3D  v1, v2, v3;
	v1 = stlTwoVectorMinus(p1,q1);
	v2 = stlTwoVectorMinus(p2,q1);
	v3 = stlTwoVectorMinus(p3,q1);
	int  flag = 0;
	if (stlDistanceOneVector(v1)< 0.1 ) {
		flag++;
		v2 = stlTwoVectorMinus(p2,q2);
		v3 = stlTwoVectorMinus(p3,q3);
		if ( stlDistanceOneVector(v2)< 0.1 ) {
			flag++;
			if ( stlDistanceOneVector(v3) < 0.1 ) {
				flag++;
			}
		}
		else {
			v2 = stlTwoVectorMinus(p3,q2);
			v3 = stlTwoVectorMinus(p2,q3);
			if ( stlDistanceOneVector(v2) < 0.1 ) {
				flag++;
				if ( stlDistanceOneVector(v3)< 0.1 ) {
					flag++;
				}
			}
		}
	}
	else if ( stlDistanceOneVector(v2) < 0.1 ) {
		flag++;
		v1 = stlTwoVectorMinus(p1,q2);
		v3 = stlTwoVectorMinus(p3,q3);
		if ( stlDistanceOneVector(v1)< 0.1 ) {
			flag++;
			if (stlDistanceOneVector(v3) < 0.1 ) {
				flag++;
			}
		}
		else {
			v1 = stlTwoVectorMinus(p3,q2);
			v3 = stlTwoVectorMinus(p1,q3);
			if (stlDistanceOneVector(v1) < 0.1 ) {
				flag++;
				if (stlDistanceOneVector(v3) < 0.1 ) {
					flag++;
				}
			}
		}
	}
	else if (stlDistanceOneVector(v3) < 0.1 ) {
		flag++;
		v1 = stlTwoVectorMinus(p1,q2);
		v2 = stlTwoVectorMinus(p2,q3);
		if ( stlDistanceOneVector(v2) < 0.1 ) {
			flag++;
			if (stlDistanceOneVector(v1) < 0.1 ) {
				flag++;
			}
		}
		else {
			v1 = stlTwoVectorMinus(p1,q3);
			v2 = stlTwoVectorMinus(p2,q2);
			if (stlDistanceOneVector(v2) < 0.1 ) {
				flag++;
				if ( stlDistanceOneVector(v1) < 0.1 ) {
					flag++;
				}
			}
		}
	}
	return flag;
}
STLPNT3D GridModel::stlTwoVectorPlus(STLPNT3D p1,STLPNT3D p2)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x + p2.x;	PTemp.y = p1.y + p2.y;	PTemp.z = p1.z + p2.z;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorMinus(STLPNT3D p1,STLPNT3D p2)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x - p2.x;	PTemp.y = p1.y - p2.y;	PTemp.z = p1.z - p2.z;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorMultiply(STLPNT3D p1,double l1)
{
	STLPNT3D PTemp = p1;
	PTemp.x = p1.x * l1;	PTemp.y = p1.y * l1;	PTemp.z = p1.z * l1;
	return PTemp;
}
STLPNT3D GridModel::stlTwoVectorDivide(STLPNT3D p1,double l1)
{
	STLPNT3D PTemp = PBreak;
	if(fabs( l1 ) > 1.e-9)
	{
		PTemp.x = p1.x / l1;	PTemp.y = p1.y / l1;	PTemp.z = p1.z / l1;
	}
	return PTemp;
}
//计算某向量长度
double  GridModel::stlDistanceOneVector(STLPNT3D p1)
{
	return (sqrt(fabs(p1.x*p1.x +p1.y*p1.y +p1.z*p1.z)));
}

POList GridModel::stlPOLHeadAlloc()
{
	POList T;
	T = (POList)malloc(sizeof(struct MTIPathOriginList)) ;
	if( T == NULL )
	{
		exit(1) ;
	}
	T->PTrail = NULL ;
	T->PNTrail = NULL ;
	T->FTrail = NULL ;
	T->TNum = 0 ;
	T->DNum = 1 ;
	T->SNum = NULL ;
	T->ENum = NULL ;
	T->PONext = NULL ;

	return T ;
}

void  GridModel::GetInformationFromPath()//从铺放路径获取信息			主要针对TXT文件
{
	FILE * ffpem;
	int NumTemp;
	int it = 0;
	double d ; // nt add 2022/6/18
	char str[300],*ret, *ip;
	if(fopen_s(&ffpem, PreDataFileName,"r") != 0) {
		printf("Error: Path file open failed\n");
		MessageBox(NULL, L"数据文件不存在,请检查！", L"提示", MB_OK);
		exit(1);
	}
label:
	ret = fgets(str,300,ffpem);
 	if (ret != NULL)
	{
		if (strstr(str,"iSag=")!= NULL)
		{
			ip = strstr(str,"iSag=")+5;
			sscanf_s(ip,"%le",&isag);
			ip = strstr(str,"iAngle=")+5;
			sscanf_s(ip,"%le",&iangle);
			ip = strstr(str,"iStep=")+6;
			sscanf_s(ip,"%le",&istep);
		}
		else if (strstr(str,"AngelStart=")!=NULL)
		{
			ip = strstr(str,"AngelStart=")+11;
			sscanf_s(ip,"%le",&Plyangle);
		}
		else if (strstr(str,"MeshDATAFile=")!= NULL)
		{
			ip = strstr(str,"_DATA")+5;
			//sprintf_s(FileResultName,DataFileFullPath);
			//strcat_s(FileResultName,ip);
			//stlReadStlFile(FileResultName);
		}
		else if (strstr(str,"Width=")!= NULL)
		{
			ip = strstr(str,"Width=")+6;
			sscanf_s(ip,"%le",&dval);
			ip = strstr(str,"Gap=")+4;
			sscanf_s(ip,"%le",&gapValue);
			ip = strstr(str,"TapNums=")+8;
			sscanf_s(ip,"%d",&FiberNum);
		}
		else if (strstr(str,"ModelName=")!= NULL)
		{
			ip = strstr(str,"ModelName=")+10;
			//sscanf_s(ip,"%s",&ModelName);
		}
		else if (strstr(str,"Element=")!= NULL)
		{
			ip = strstr(str,"Element=")+8;
			sscanf_s(ip,"%s",&PointName, 100);
		}
		else if (strstr(str,"TotalNum=")!= NULL)
		{
			ip = strstr(str,"TotalNum=")+9;
			sscanf_s(ip,"%d",&TrailNumDum);
		}
		else if (strstr(str,"NormalTag=")!= NULL)
		{
			ip = strstr(str,"NormalTag=")+10;
			sscanf_s(ip,"%d",&stlFaceTag);
		}
		else if (strstr(str,"Output Start")!= NULL)
		{
			POLHead = new POList[TrailNumDum+1];
			ret = fgets(str,300,ffpem);
label2:
			if (strstr(str,"tiaoshu=")!= NULL)
			{
				it++;
				ip = strstr(str,"tiaoshu=")+8;
				sscanf_s(ip,"%d",&NumTemp);
				POLHead[NumTemp] = stlPOLHeadAlloc();
				POLHead[NumTemp]->TNum = NumTemp;
				POLHead[NumTemp]->ENum = new int[2];
				POLHead[NumTemp]->SNum = new int[2];
				POLHead[NumTemp]->ENum[1] = 0;
				POLHead[NumTemp]->SNum[1] = 1;
				ip = strstr(str,"dianshu=")+8;
				int PointNumTemp;
				sscanf_s(ip,"%d",&PointNumTemp);
				POLHead[NumTemp]->PNTrail = new STLPNT3D[PointNumTemp+1];
				POLHead[NumTemp]->PTrail = new STLPNT3D[PointNumTemp+1];
				POLHead[NumTemp]->FTrail = new FList[PointNumTemp+1];
label3:
				ret = fgets(str,300,ffpem);
				if (strstr(str,"x=")!= NULL)
				{
					int FaceTemp;
					POLHead[NumTemp]->ENum[1]++;
					ip = strstr(str,"x=")+2;
					sscanf_s(ip,"%le",&d);
					POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]].x = /*0.001**/d ;
					ip = strstr(str,"y=")+2;
					sscanf_s(ip,"%le",&d);
					POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]].y = /*0.001**/d ;
					ip = strstr(str,"z=")+2;
					sscanf_s(ip,"%le",&d);
					POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]].z = /*0.001**/d ;

					ip = strstr(str,"Nx=")+3;
					sscanf_s(ip,"%le",&POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]].x);
					ip = strstr(str,"Ny=")+3;
					sscanf_s(ip,"%le",&POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]].y);
					ip = strstr(str,"Nz=")+3;
					sscanf_s(ip,"%le",&POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]].z);
					ip = strstr(str,"FaceNum=")+8;
					sscanf_s(ip,"%d",&FaceTemp);

					if (POLHead[NumTemp]->ENum[1] > 2 && stlDistanceTwoPoints(POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]],POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]-1]) < 1.e-6)//20210106
					{
						POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]-1] = POLHead[NumTemp]->PTrail[POLHead[NumTemp]->ENum[1]] ;
						POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]-1] = POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]] ;
						POLHead[NumTemp]->ENum[1]--;
					}
					//if(POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]].z < 0) 
					//	POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]] = stlOPPNormalVector(POLHead[NumTemp]->PNTrail[POLHead[NumTemp]->ENum[1]]);
					if(FHead != NULL)
					{
						POLHead[NumTemp]->FTrail[POLHead[NumTemp]->ENum[1]] = stlGetFacetFromNum(FaceTemp);

					}
					goto label3;
				}
				ret = fgets(str,300,ffpem);
				goto label2;
			}
			if(strstr(str,"Output End") != NULL) {
				TrailNumDum = it;
				fclose(ffpem);
				ffpem = NULL;
				return;
			}
			else {
				goto label2;
			}
		}
		if(strstr(str,"Output End") != NULL) {
			TrailNumDum = it;
			fclose(ffpem);
			ffpem = NULL;
			return;
		}
		else {
			goto label;
		}
	}
	if(strstr(str,"Output End") != NULL) {
		TrailNumDum = it;
		fclose(ffpem);
		ffpem = NULL;
		return;
	}
	else {
		goto label;
	}
}

double GridModel::triCalArea(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3)
{
	STLPNT3D V1, V2, nv;
	V1 = stlTwoVectorMinus(p2, p1);
	V2 = stlTwoVectorMinus(p3, p2);
	nv = stlTwoVectorProduct(V1, V2);
	double  l1 = stlDistanceOneVector(nv);
	return 0.5 * l1;
}

double GridModel::CalDistofPtAndFace(STLPNT3D p0, FList f)
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	x1 = f->VertexUsed[2]->Coord.x - f->VertexUsed[1]->Coord.x;
	x2 = f->VertexUsed[1]->Coord.x - f->VertexUsed[0]->Coord.x;
	y1 = f->VertexUsed[2]->Coord.y - f->VertexUsed[1]->Coord.y;
	y2 = f->VertexUsed[1]->Coord.y - f->VertexUsed[0]->Coord.y;
	z1 = f->VertexUsed[2]->Coord.z - f->VertexUsed[1]->Coord.z;
	z2 = f->VertexUsed[1]->Coord.z - f->VertexUsed[0]->Coord.z;
	STLPNT3D A = CreateMTIPoint(x1, y1, z1);
	STLPNT3D B = CreateMTIPoint(x2, y2, z2);
	STLPNT3D n = stlTwoVectorProduct(A, B);
	x3 = p0.x - f->VertexUsed[2]->Coord.x;
	y3 = p0.y - f->VertexUsed[2]->Coord.y;
	z3 = p0.z - f->VertexUsed[2]->Coord.z;
	STLPNT3D Q = CreateMTIPoint(x3, y3, z3);
	STLPNT3D n2 = vectorNormalize(n);
	double s = fabs(n2 * Q);
	return s;
}

FList GridModel::FindClosetFace(STLPNT3D p0, double tol)
{

	FList f = FHead;
	for (int i = 1; i <= FaceNum; i++)
	{
		if (CalDistofPtAndFace(p0, f) < tol&& Is_in_triangle(p0, f, tol) == 1)
			return f;
		else if (f->FNext != NULL)f = f->FNext;
	}
	return f;
}

//判断点的投影是不是在三角形内
int GridModel::Is_in_triangle(STLPNT3D p0, FList f, double tol)
{
	double s1 = CalDistofPtAndFace(p0, f);
	VList v1, v2, v3;
	v1 = f->VertexUsed[1];
	v2 = f->VertexUsed[2];
	v3 = f->VertexUsed[0];
	STLPNT3D q1, q2, q3;
	q1 = v1->Coord;
	q2 = v2->Coord;
	q3 = v3->Coord;
	double a0, a1, a2, a3;
	a0 = triCalArea(q1, q2, q3);
	a1 = triCalArea(p0, q2, q3);
	a2 = triCalArea(q1, p0, q3);
	a3 = triCalArea(q1, q2, p0);
	if (fabs(a0 - a1 - a2 - a3) < tol)return 1;
	else return 0;
}

double cal2STLPNT3D_DIST(STLPNT3D p1, STLPNT3D p2)
{
	double s = (p1.x - p2.x) * (p1.x - p2.x)
		+ (p1.y - p2.y) * (p1.y - p2.y)
		+ (p1.z - p2.z) * (p1.z - p2.z);
	return sqrt(fabs(s));
}

STLVECTOR GridModel::CalAllVertexNormalVector(STLPNT3D p0, double tol)
{
	FList f = FindClosetFace(p0, tol);
	STLVECTOR n = f->NormalVector;
	return n;
}

void GridModel::stlDealInputFile()
{
	int flag = 0;
	FILE *file;

	char buf[300];
	errno_t err;
	err = fopen_s(&file, ModelName, "rb");
	if (strstr(ModelName,"stl") != NULL)
	{
		flag = 1;
		fread(buf, 1, 255,file);
		char* c;
		for(int i = 0; i < 255; i ++)
		{
			c = strstr(buf + i,"facet normal");  //search substring "facet normal"
			if (c != NULL) 
			{
				flag = 2;   //find "facet normal", then flag =2,stl文件为字符文件
				break;
			}
		}
	}
	else if (strstr(ModelName,"dat") != NULL)
	{
		flag = 3;
	}
	fclose(file);
	if (flag != 0)
	{
		if(flag == 1)
		{
			stlReadSTLBinFile(ModelName); //二进制stl文件
		}
		else if(flag == 2)
		{
			stlReadStlFile(ModelName);   //字符stl文件
		} 
		else if(flag == 3)
		{
			stlReadDatFile(ModelName);		//有限元网格Dat文件
		}
	}
}
void GridModel::stlReadFile(char *filename)
{
	int flag = 0;
	FILE *file;

	char buf[300];
	errno_t err;
	err = fopen_s(&file, filename, "rb");
	int FileEx = 1;
	if(err != 0){
		MessageBox(NULL, L"文件不存在，请检查输入条件。", L"提示", MB_OK);
		FileEx = 0;
	}
	if (strstr(filename,"stl") != NULL)
	{
		flag = 1;
		fread(buf, 1, 255,file);
		char* c;
		for(int i = 0; i < 255; i ++)
		{
			c = strstr(buf + i,"facet normal");  //search substring "facet normal"
			if (c != NULL) 
			{
				flag = 2;   //find "facet normal", then flag =2,stl文件为字符文件
				break;
				file = NULL;
			}
		}
	}
	else if (strstr(filename,"dat") != NULL)
	{
		flag = 3;
	}
	fclose(file);
	file = NULL;

	if (flag != 0)
	{
		if(flag == 1)
		{
			stlReadSTLBinFile(filename); //二进制stl文件
		}
		else if(flag == 2)
		{
			stlReadStlFile(filename);   //字符stl文件
		} 
		else if(flag == 3)
		{
			stlReadDatFile(filename);		//有限元网格Dat文件
		}
		TraverseNum = 0;
		//GetNormaVectorOnVertex(VRoot);//获取顶点处法向 nt comment 2022/6/14
	}
}
/*void GridModel::GetNormaVectorOnVertex(VList p)
{
	if (p)
	{
		TraverseNum ++;
		FRelated ff = p->FaceUsed;
		int FNum = p->FaceNum;
		FList f = ff->Face;
		double *dTemp = new double[FNum+1]; 
		double dEnd = 0;
		STLPNT3D NormalV = CreateMTIPoint();
		STLPNT3D NormalV2 = CreateMTIPoint();
		for (int i =1; i <= FNum;i++)
		{
			dTemp[i] = GetDistanceVertexToCoreOfTrangle(p->Coord,ff ->Face);
			if(dTemp[i] >= 0)
			{
				NormalV2 =  stlTwoVectorDivide(ff ->Face->NormalVector,dTemp[i]);
				NormalV =stlTwoVectorPlus(NormalV,NormalV2);
				dEnd += 1.0 / dTemp[i];
			}
			if(ff->FNext != NULL)
				ff = ff ->FNext;
		}
		if(FNum == 1)
			dEnd = 1.0;
		p->NormalVector = stlTwoVectorMultiply(NormalV,dEnd);
		stlVectorNormal(&(p->NormalVector));
		if(dTemp != NULL)delete [] dTemp;
			dTemp = NULL;
		GetNormaVectorOnVertex(p->Lchild);
		GetNormaVectorOnVertex(p->Rchild);
	}
	else if(TraverseNum >= VerNum-1)
	{
		return ;
	}
}*/
void GridModel::stlReadSTLBinFile(char *filename)
{
	FILE *stlfile;

	STLVECTOR nv;
	STLPNT3D  p1,p2,p3;

	union 
	{
		long int i4;
		char   c[4];
		float   r;
	} buf;

	errno_t err;

	err = fopen_s(&stlfile, filename, "rb");
	if ( err != 0 ) {
		return;
	}

	fseek(stlfile,0L,SEEK_SET); //move point to the beginning
	if(stlfile == NULL) 
	{
		printf("Error: STL file open failed\n");
		exit(1);
	}

	for(int i=0; i<80; i++)  
	{
		char c = getc(stlfile); 
		printf("%c",c);
	}
	printf("\n");

	buf.c[0] = getc(stlfile); //get the total number of the facet
	buf.c[1] = getc(stlfile);
	buf.c[2] = getc(stlfile);
	buf.c[3] = getc(stlfile);
	int NumberTotal = buf.i4;  //total number of facet
	//NumberTotal = 400000;
	int step = (int)(NumberTotal/20);

	char str[133];
	strcpy_s(str,"正在打开模型, 请等待");

	for(int i=0; i<NumberTotal; i++) 
	{
		if (i%step == 0) 
		{
			strcat_s(str, ".");
		}

		stlRead4Bytes(stlfile,&buf.c[0]);  //get the vector of the facet    
		nv.x = buf.r; 
		stlRead4Bytes(stlfile,&buf.c[0]);    
		nv.y = buf.r; 
		stlRead4Bytes(stlfile,&buf.c[0]);    
		nv.z = buf.r; 

		stlRead4Bytes(stlfile,&buf.c[0]);  //get the points of the facet    
		p1.x = buf.r;
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p1.y = buf.r;
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p1.z = buf.r;

		stlRead4Bytes(stlfile,&buf.c[0]);    
		p2.x = buf.r;
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p2.y = buf.r;
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p2.z = buf.r;

		stlRead4Bytes(stlfile,&buf.c[0]);    
		p3.x = buf.r;
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p3.y = buf.r; 
		stlRead4Bytes(stlfile,&buf.c[0]);    
		p3.z = buf.r;

		char c = getc(stlfile); 
		c = getc(stlfile); 

		STLPNT3D vp1, vp2;
		vp1 = CreateMTIPoint();
		vp2 = nv;
		nv = stlTwoVectorMinus(vp2,vp1);
		stlVectorNormal(&nv);
		stlCreateFace(&nv,&p1,&p2,&p3,-1);   //生成一个三角片面,nv为法向量,p为三个顶点坐标

	}
	fclose(stlfile);
	stlfile = NULL;
}

void GridModel::stlRead4Bytes(FILE *stlfile, char *c)
{
	c[0] = getc(stlfile);
	c[1] = getc(stlfile);
	c[2] = getc(stlfile);
	c[3] = getc(stlfile);
}
void GridModel::stlReadStlFile(char *file)
{
	FILE		*stlfile;
	STLPNT3D  p1,p2,p3,nv;
	STLVECTOR v1 = { 0, 0, 0 }, v2 = { 0, 0, 0 }, v3 = {0, 0, 0};
	int			tttNum;
	char    str[256]; // nt modify 2022/7/8 old 80
	char    *ret, *ip;

	if(fopen_s(&stlfile, file,"r") != 0) {
		printf("Error: STL file open failed\n");
		exit(1);
	}
	MeshResulttag = 0;
label:
	ret = fgets(str,256,stlfile);
	if ( ret != NULL ) {
		if(strstr(str,"##") != NULL)
		{
			//ip = strstr(str,"%%");
			sscanf_s(str,"%s",&MeshResult[MeshResulttag], 256);
			MeshResulttag ++;
		}
		if ( strstr(str,"facet normal") != NULL ) {
			ip = strstr(str,"normal") + 6;
			sscanf_s(ip,"%le %le %le",&nv.x,&nv.y,&nv.z);
			if (strstr(str,"FaceNum = ") != NULL)
			{
				ip = strstr(str,"FaceNum = ") + 10;
				sscanf_s(ip,"%d",&tttNum);
			}
			else
				tttNum = -1;
			
			while(1)
			{
				ret = fgets(str,256,stlfile);
				if( strstr(str,"outer loop") != NULL ) // nt add 2022/7/8
					break ;
			}
			ret = fgets(str,256,stlfile);
			ip = strstr(str,"vertex") + 6;
			sscanf_s(ip,"%le %le %le",&p1.x,&p1.y,&p1.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v1.x, &v1.y, &v1.z);
			}

			ret = fgets(str,256,stlfile);
			ip = strstr(str,"vertex") + 6;
			sscanf_s(ip,"%le %le %le",&p2.x,&p2.y,&p2.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v2.x, &v2.y, &v2.z);
			}

			ret = fgets(str,256,stlfile);
			ip = strstr(str,"vertex") + 6;
			sscanf_s(ip,"%le %le %le",&p3.x,&p3.y,&p3.z);
			ip = strstr(str, "V_Normal");
			if (ip)
			{
				ip += 8;
				sscanf_s(ip, "%le %le %le", &v3.x, &v3.y, &v3.z);
			}

			stlCreateFace(&nv, &p1, &p2, &p3, &v1, &v2, &v3, tttNum);
			goto label;
		}
		else if(strstr(str,"endsolid") != NULL) {
			fclose(stlfile);
			stlfile = NULL;
			return;
		}
		else {
			goto label;
		}
	}
	stlfile = NULL;
}

void GridModel::stlReadDatFile(char *file)
{
	FILE		*datfile;
	STLPNT3D  p1,p2,p3,nv;
	char    str[80];
	char    *ret,*ip;
	int     temp[5],temp1;
	STLPNT3D *PTemp;
	int i = 1;

	//获取点数,确定点数组大小
	if(fopen_s(&datfile, file,"r") != 0) {
		printf("Error: dat file open failed\n");
		exit(1);
	}
label2:	
	ret = fgets(str,80,datfile);
	if ( ret != NULL ) {
		if ( strstr(str,"GRID*") != NULL ) {
			ret = fgets(str,80,datfile);
			ret = fgets(str,80,datfile);
			i ++;
			goto label2;
		}
		else if (strstr(str,"ELEMENTS") != NULL )
			fclose(datfile);
		else 
			goto label2;	
	}

	fclose(datfile);

	PTemp = new STLPNT3D[i];
	
	//重新打开文档,开始拓扑关系建立
	if(fopen_s(&datfile, file,"r") != 0) {
		printf("Error: dat file open failed\n");
		exit(1);
	}
	
	i = 1;
label1:
	ret = fgets(str,80,datfile);
	if ( ret != NULL ) {
		if ( strstr(str,"GRID*") != NULL ) {

			ip = strstr(str,"GRID*") + 5;
			sscanf_s(ip,"%d %le %le",&temp1,&p1.x,&p1.y);
			ret = fgets(str,80,datfile);
			ret = fgets(str,80,datfile);
 			sscanf_s(ip,"%d %le",&temp1,&p1.z);
			PTemp[i] = p1;

 			i ++;

			goto label1;

			}
			else if (strstr(str,"ELEMENTS") != NULL )
				goto labell;
			else
				goto label1;
		}	
	
labell:
	ret = fgets(str,80,datfile);
	if (ret != NULL)
	{
		if (strstr(str,"CTRIA3") != NULL)//从面开始拓扑入手
		{
			ip =strstr(str,"CTRIA3") + 6;
			sscanf_s(ip,"%d %d %d %d %d",&temp[0],&temp[1],&temp[2],&temp[3],&temp[4]);

			p1 = PTemp[temp[2]];
			p2 = PTemp[temp[3]];
			p3 = PTemp[temp[4]];

			STLPNT3D l1,l2;
			l1 = stlDistanceVectorTwoPoint(p1,p2);
			l2 = stlDistanceVectorTwoPoint(p1,p3);
			nv = stlTwoVectorProduct(l1,l2);
			stlVectorNormal(&nv);
			nv = stlOPPNormalVector(nv);
			stlCreateFace(&nv,&p1,&p2,&p3,-1); 
			goto labell;
		}
		else if(strstr(str,"ENDDATA") != NULL) {
			fclose(datfile);

			delete PTemp;
      PTemp = NULL;

			return;
		}
		else {
			goto labell;
		}
	}
}

//返回两点方向向量并单位化
STLPNT3D GridModel::stlDistanceVectorTwoPoint(STLPNT3D p1,STLPNT3D p2)
{
	STLPNT3D PTemp = stlTwoVectorMinus(p2,p1);
	if (!stlVectorNormal(&PTemp))
	{
		PTemp = CreateMTIPoint();
	}//return PBreak;
	return PTemp;
}
///////////////////////////////////////////////////////////////////////////////////////
// Function: 计算两向量叉积
// Input:		 L1  L2  两向量
// Return:  叉积
///////////////////////////////////////////////////////////////////////////////////////
STLPNT3D  GridModel::stlTwoVectorProduct(STLPNT3D L1,STLPNT3D L2)
{
	STLPNT3D P = PBreak;	// ?
	P.x = L1.y * L2.z - L1.z * L2.y;	
	P.y = L1.z * L2.x - L1.x * L2.z;		
	P.z = L1.x * L2.y - L1.y * L2.x;
	return P;
}
STLPNT3D GridModel::stlOPPNormalVector(STLPNT3D p1)
{
	p1 = stlTwoVectorMinus(CreateMTIPoint(), p1);
	return p1;
}
STLPNT3D GridModel::CreateMTIPoint()//创建默认点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(0.,0.,0.);
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double ix,double iy,double iz)//从xyz创点
{
	STLPNT3D pTemp;
	pTemp.x = ix;	pTemp.y = iy;	pTemp.z = iz;
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double p1[])//从数组创点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(p1[0],p1[1],p1[2]);
	return pTemp;
}
STLPNT3D GridModel::CreateMTIPoint(double p1[],int iNbStart)//从数组创点
{
	STLPNT3D pTemp;
	pTemp = CreateMTIPoint(p1[iNbStart+0],p1[iNbStart+1],p1[iNbStart+2]);
	return pTemp;
}

STLVECTOR vectorNormalize(STLVECTOR& iVector)
{
	STLVECTOR vector_normal = { 0. };
	double norm = iVector.x * iVector.x + iVector.y * iVector.y + iVector.z * iVector.z;
	if (norm <= TOLLENGTH)
		return vector_normal; // too short!
	if (fabs(norm - 1.) < TOLANGLE)
		return iVector; // 已是单位向量，不需要单位化
	norm = 1. / sqrt(norm);
	vector_normal.x = iVector.x * norm;
	vector_normal.y = iVector.y * norm;
	vector_normal.z = iVector.z * norm;

	return vector_normal;
}

STLVECTOR operator-(STLPNT3D& iEnd, STLPNT3D& iBegin)
{
	STLVECTOR v;
	v.x = iEnd.x - iBegin.x;
	v.y = iEnd.y - iBegin.y;
	v.z = iEnd.z - iBegin.z;
	return v;
}

STLVECTOR operator-(STLVECTOR& iVector)
{
	STLVECTOR oVector;
	oVector.x = -iVector.x;
	oVector.y = -iVector.y;
	oVector.z = -iVector.z;
	return oVector;
}

STLVECTOR operator+(STLVECTOR& iVector1, STLVECTOR& iVector2)
{
	STLVECTOR v;
	v.x = iVector1.x + iVector2.x;
	v.y = iVector1.y + iVector2.y;
	v.z = iVector1.z + iVector2.z;
	return v;
}


STLVECTOR operator*(double iScalar, STLVECTOR& iVector)
{
	STLVECTOR v;
	v.x = iScalar * iVector.x;
	v.y = iScalar * iVector.y;
	v.z = iScalar * iVector.z;
	return v;
}

double operator*(STLVECTOR& iVector1, STLVECTOR& iVector2)
{
	double res = 0.;
	res = iVector1.x * iVector2.x + iVector1.y * iVector2.y + iVector1.z * iVector2.z;
	return res;
}

STLVECTOR operator^(STLVECTOR& iVectorU, STLVECTOR& iVectorV)
{
	VEC3D v1, v2, rtvec;
	memcpy(v1, &iVectorU, sizeof(PNT3D));
	memcpy(v2, &iVectorV, sizeof(PNT3D));

	mathVProduct(v1, v2, rtvec);

	STLVECTOR vector_w;
	memcpy(&vector_w, rtvec, sizeof(PNT3D));

	return vector_w;
}

// 线段与线段求交
int mathSegmIntSegm(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, STLPNT3D &oBegin, STLPNT3D &oEnd)
{
	PNT3D b1, e1, b2, e2, p1, p2;
	int sizeof_point = sizeof(PNT3D);

	memcpy(b1, &iBegin1, sizeof_point);
	memcpy(e1, &iEnd1, sizeof_point);
	memcpy(b2, &iBegin2, sizeof_point);
	memcpy(e2, &iEnd2, sizeof_point);

	int res = mathIntSegm2(b1, e1, b2, e2, TOLLENGTH, TOLANGLE, p1, p2);

	memcpy(&oBegin, p1, sizeof_point);
	memcpy(&oEnd, p2, sizeof_point);

	return res;
}

// 线段与平面求交
// smf add 2022/9/06
int mathSegmentIntPln(PNT3D iPntOfSegment1, PNT3D iPntOfSegment2,  // 线段的两个端点
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	PNT3D* oPntsOfIntersection) // 交点数组的指针
{
	int num_intersection_pnts = -1; // 初始化

	double distToPlane1 = mathDistPntPlnSide(iPntOfSegment1, iPntOnPlane, iNormPlane);
	double distToPlane2 = mathDistPntPlnSide(iPntOfSegment2, iPntOnPlane, iNormPlane);

	if (distToPlane1 * distToPlane2 > TOLSQUARELENGTH) // 线段在平面的一侧，无交
	{
		num_intersection_pnts = 0;
		return num_intersection_pnts;
	}

	if (fabs(distToPlane1) < TOLLENGTH &&
		fabs(distToPlane2) < TOLLENGTH) // 线段在平面上，两个交点
	{
		num_intersection_pnts = 2;
		memcpy(oPntsOfIntersection[0], iPntOfSegment1, sizeof(PNT3D));
		memcpy(oPntsOfIntersection[1], iPntOfSegment2, sizeof(PNT3D));
		return num_intersection_pnts;
	}

	PNT3D p;
	VEC3D v, v1;
	if (mathGetVecUnit(iPntOfSegment1, iPntOfSegment2, v1) == 0)
	{
		num_intersection_pnts = 0; // 两点十分靠近，0个交点？？？
		return num_intersection_pnts;
	}
	double acos_alpha = 0.;
	double dist = 0.;
	acos_alpha = mathOProduct(v1, iNormPlane);
	if (acos_alpha < 0) // 线段方向向量与平面法线夹角，并判断是否同向
	{
		v1[0] *= -1;
		v1[1] *= -1;
		v1[2] *= -1;
		memcpy(p, iPntOfSegment1, sizeof(PNT3D));
		dist = distToPlane1;
	}
	else
	{
		memcpy(p, iPntOfSegment2, sizeof(PNT3D));
		dist = distToPlane2;
	}
	memcpy(v, v1, sizeof(VEC3D));

	num_intersection_pnts = 1;

	acos_alpha = fabs(acos_alpha);
	double tmp = dist / acos_alpha;
	for (size_t i = 0; i < 3; i++)
	{
		oPntsOfIntersection[0][i] = p[i] - v[i] * tmp; // 计算线-面交点坐标
	}

	return num_intersection_pnts;
}

int mathPlnIntTri(
	PNT3D iPntOfTri1, PNT3D iPntOfTri2, PNT3D iPntOfTri3, // 三角形的三个顶点
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // 平面上一点及平面的法矢
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	PNT3D* oPntsOfIntersection, // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	for (size_t i = 0; i < 3; i++)
	{
		oIndexOfIntEdge[i] = -1;
	}
	oNumOfIntEdge = 0;
	int num_intersection_pnts = 0; // 初始化

	PNT3D iPntsOfTri[3];
	memcpy(iPntsOfTri[0], iPntOfTri1, sizeof(PNT3D));
	memcpy(iPntsOfTri[1], iPntOfTri2, sizeof(PNT3D));
	memcpy(iPntsOfTri[2], iPntOfTri3, sizeof(PNT3D));

	double distToPlane[3];
	for (size_t i = 0; i < 3; i++)
		distToPlane[i] = mathDistPntPlnSide(iPntsOfTri[i], iPntOnPlane, iNormPlane);

	// 判断三点是否都在平面上
	if (fabs(distToPlane[0]) <= iTolLength &&
		fabs(distToPlane[1]) <= iTolLength &&
		fabs(distToPlane[2]) <= iTolLength)
	{
		num_intersection_pnts = 3; // 三点都在平面上
		memcpy(oPntsOfIntersection[0], iPntOfTri1, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 0;
		memcpy(oPntsOfIntersection[1], iPntOfTri2, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 1;
		memcpy(oPntsOfIntersection[2], iPntOfTri3, sizeof(PNT3D));
		oIndexOfIntEdge[0] = 2;
		return num_intersection_pnts;
	}

	// 判断三点是否在平面同侧
	if ((distToPlane[0] < -iTolLength &&
		distToPlane[1] < -iTolLength &&
		distToPlane[2] < -iTolLength) ||
		(distToPlane[0] > iTolLength &&
			distToPlane[1] > iTolLength &&
			distToPlane[2] > iTolLength))
	{
		num_intersection_pnts = 0; // 三点在平面同侧，无交
		return num_intersection_pnts;
	}
	for (int i = 0; i < 3; i++)
	{
		VEC3D v;
		int num_int_pnts = 0;
		if (num_int_pnts = mathGetVecUnit(iPntsOfTri[i], iPntsOfTri[(i + 1) % 3], v) == 0) // 线段长度过短
		{
			continue;
		}

		PNT3D int_pnts[2] = {};
		// 线段(三角形的边)与平面求交
		num_int_pnts = mathSegmentIntPln(iPntsOfTri[i], iPntsOfTri[(i + 1) % 3],
			iPntOnPlane, iNormPlane,  // 平面上一点及平面的法矢
			iTolLength, iTolAngle,  // 长度容差及角度容差
			int_pnts); // 交点数组

//检查是否有重合的交点，去重
		for (size_t j = 0; j < num_int_pnts; j++)
		{
			// 只要有交，就保留边序号
			oIndexOfIntEdge[oNumOfIntEdge] = i;
			oNumOfIntEdge++;
			if (num_intersection_pnts == 0)
			{
				memcpy(oPntsOfIntersection[0], int_pnts[j], sizeof(PNT3D));
				num_intersection_pnts++;
			}
			else
			{
				int k = 0;
				for (k = 0; k < num_intersection_pnts; k++)
				{
					if (mathSquareDist(int_pnts[j], oPntsOfIntersection[k]) <= TOLSQUARELENGTH) // 是重合点
					{
						k = 0;
						break;
					}
				}
				if (k == num_intersection_pnts) // 不是重合点
				{
					// 保存不重合的交点
					memcpy(oPntsOfIntersection[num_intersection_pnts], int_pnts[j], sizeof(PNT3D));
					num_intersection_pnts++;
				}
			}
		}
	}
	return num_intersection_pnts;
}

int mathPlnIntTri(
	STLPNT3D iSTLPntOfTri1, STLPNT3D iSTLPntOfTri2, STLPNT3D iSTLPntOfTri3, // 三角形的三个顶点
	STLPNT3D iSTLPntOnPlane, STLVECTOR iSTLNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oSTLPntsOfIntersection, // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	PNT3D iPntOfTri1, iPntOfTri2, iPntOfTri3, // 三角形的三个顶点
		iPntOnPlane;
	VEC3D iNormPlane;  // 平面的法矢及平面上一点
	PNT3D oPntsOfIntersection[3];

	memcpy(iPntOfTri1, &iSTLPntOfTri1, sizeof(PNT3D));
	memcpy(iPntOfTri2, &iSTLPntOfTri2, sizeof(PNT3D));
	memcpy(iPntOfTri3, &iSTLPntOfTri3, sizeof(PNT3D));
	memcpy(iPntOnPlane, &iSTLPntOnPlane, sizeof(PNT3D));
	memcpy(iNormPlane, &iSTLNormPlane, sizeof(PNT3D));
	int res = mathPlnIntTri(
		iPntOfTri1, iPntOfTri2, iPntOfTri3, // 三角形的三个顶点
		iPntOnPlane, iNormPlane,  // 平面的法矢及平面上一点
		iTolLength, iTolAngle,  // 长度容差及角度容差
		oPntsOfIntersection,  // 交点数组的指针
		oIndexOfIntEdge, oNumOfIntEdge); // 与平面相交的边的序号
	memcpy(oSTLPntsOfIntersection, oPntsOfIntersection, 3 * sizeof(PNT3D));
	return res;
}

int mathPlnIntTri(
	FList iTriangle, // 三角形
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // 平面的法矢及平面上一点
	double iTolLength, double iTolAngle,  // 长度容差及角度容差
	STLPNT3D* oPntsOfIntersection,  // 交点数组的指针
	int* oIndexOfIntEdge, int& oNumOfIntEdge) // 与平面相交的边的序号及数量
{
	return mathPlnIntTri(
		iTriangle->VertexUsed[0]->Coord, iTriangle->VertexUsed[1]->Coord, iTriangle->VertexUsed[2]->Coord, // 三角形的三个顶点
		iPntOnPlane, iNormPlane,  // 平面的法矢及平面上一点
		iTolLength, iTolAngle,  // 长度容差及角度容差
		oPntsOfIntersection,  // 交点数组的指针
		oIndexOfIntEdge, oNumOfIntEdge);// 与平面相交的边的序号
}

double mathSquareDist(PNT3D p1, PNT3D p2)
{
	double squareDist = 0.;
	for (size_t i = 0; i < 3; i++)
	{
		double delta = p1[i] - p2[i];
		squareDist += delta * delta;
	}
	return squareDist;
}

double mathSquareDist(STLPNT3D p1, STLPNT3D p2)
{
	double squareDist = 0.;
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;
	double delta_z = p1.z - p2.z;
	squareDist = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
	return squareDist;
}

// 2022/12/01 smf add 
// 缩短或延伸一条线段。iDist可为负值，iDist < 0 缩短；iDist > 0 延伸。
// iBeginOrEnd: 0-起点；1-终点。
// 返回值：0-线段过短，不可缩短；1-成功。
int mathShortenOrLengthenSegmByDist(STLPNT3D &ioBegin, STLPNT3D &ioEnd, int iBeginOrEnd, double iDist)
{
	double length = mathDist(ioBegin, ioEnd);
	if (iDist < 0 && length < fabs(iDist))
	{
		return 0; // 线段过短。
	}

	STLVECTOR segm_uni = vectorNormalize(ioEnd - ioBegin);

	if (iBeginOrEnd) // 从终点
	{
		ioEnd = ioEnd + iDist * segm_uni;
	}
	else // 从起点
	{
		ioBegin = ioBegin - iDist * segm_uni;
	}

	return 1;
}

BOOL mathIsCoincidentPoint(STLPNT3D iPoint1, STLPNT3D iPoint2)
{
	double d = mathSquareDist(iPoint1, iPoint2);
	if (d <= TOLSQUARELENGTH)
		return true;
	else
		return false;
}

double mathDist(STLPNT3D p1, STLPNT3D p2)
{
	double dist = 0.;
	double delta_x = p1.x - p2.x;
	double delta_y = p1.y - p2.y;
	double delta_z = p1.z - p2.z;
	dist = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
	dist = sqrt(dist);
	return dist;
}

void mathPrjPntPln(STLPNT3D iPoint, STLPNT3D iPointOfPlane, STLVECTOR iNormalOfPlane, STLPNT3D& oProjectPoint)
{
	PNT3D p, pivot, prj_p;
	VEC3D normal;
	memcpy(p, &iPoint, 3 * sizeof(double));
	memcpy(pivot, &iPointOfPlane, 3 * sizeof(double));
	memcpy(normal, &iNormalOfPlane, 3 * sizeof(double));

	mathPrjPntPln(p, pivot, normal, prj_p);

	memcpy(&oProjectPoint, prj_p, 3 * sizeof(double));
}

void mathPrjVecPln(STLVECTOR iVector, STLVECTOR iNormal, STLVECTOR &oPrjVec)
{
	VEC3D v, normal, prj_v;

	memcpy(v, &iVector, sizeof(STLVECTOR));
	memcpy(normal, &iNormal, sizeof(STLVECTOR));

	mathPrjVecPln(v, normal, prj_v);

	memcpy(&oPrjVec, prj_v, sizeof(STLVECTOR));
	oPrjVec = vectorNormalize(oPrjVec);
}

/*  1997/12/6 nt
 *  直线与圆柱求交
 *	返回IDNOINT,无交点;返回IDTAN,切于一点;返回IDINT,交于两点;返回IDOVERL,重合
 *	nt modify 2003/2/18
 */
int mathIntLinCyl(PNT3D begin,
	VEC3D dir,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	double ang,
	PNT3D intpt1,
	PNT3D intpt2)
{
	int rt_value = IDTAN, k;
	double d, h, c;
	PNT3D localBegin;
	VEC3D e1, e2, localDir;

	mathGetXYFromZ(axis, e1, e2);
	mathTransWorldPnt3DByOXYZ(pivot, e1, e2, axis, begin, localBegin);
	mathTransWorldVec3DByXYZ(e1, e2, axis, dir, localDir);
	d = sqrt(localDir[0] * localDir[0] + localDir[1] * localDir[1]);
	if (d < ang) // paralell to cylinder axis
	{
		d = sqrt(localBegin[0] * localBegin[0] + localBegin[1] * localBegin[1]);
		if (r - tol < d && d < r + tol)
			return IDOVERL;
		else
			return IDNOINT;
	}
	else
	{
		h = fabs(localBegin[0] * localDir[1] - localBegin[1] * localDir[0]) / d;
		if (h > r + tol) // no intersection
			return IDNOINT;
		c = -(localBegin[0] * localDir[0] + localBegin[1] * localDir[1]) / d;
		intpt1[0] = localBegin[0] + localDir[0] * c / d; // origin project to line 2d
		intpt1[1] = localBegin[1] + localDir[1] * c / d; // nt add 2003/2/18 : d!!!
		intpt1[2] = localBegin[2];
		memcpy(intpt2, intpt1, sizeof(PNT3D));
		k = fabs(localDir[0]) > fabs(localDir[1]) ? 0 : 1;
		if (h < r - tol) // two intersection points,otherwise tangential contact
		{
			d = sqrt(r*r - h * h) / d;
			intpt1[0] -= (d*localDir[0]); // intersect point of circle/line 2d
			intpt1[1] -= (d*localDir[1]);
			intpt2[0] += (d*localDir[0]); // intersect point of circle/line 2d
			intpt2[1] += (d*localDir[1]);
			rt_value = IDINT;
		}
		intpt1[2] += (localDir[2] * (intpt1[k] - localBegin[k]) / localDir[k]);
		intpt2[2] += (localDir[2] * (intpt2[k] - localBegin[k]) / localDir[k]);
		mathTransLocalPnt3DByOXYZ(pivot, e1, e2, axis, intpt1, intpt1);
		mathTransLocalPnt3DByOXYZ(pivot, e1, e2, axis, intpt2, intpt2);
		return rt_value;
	}
}

/*  2002/3/8 nt
 *  transform world_vector 3D to local_vector 3D which is defined
 *	in local_frame
 */
void mathTransWorldVec3DByXYZ(VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	VEC3D world_vector,
	VEC3D local_vector)
{
	int k;
	VEC3D v;
	double local_invse[9];

	for (k = 0; k < 3; k++)
	{
		local_invse[3 * k] = e1[k];
		local_invse[3 * k + 1] = e2[k];
		local_invse[3 * k + 2] = e3[k];
	}
	for (k = 0; k < 3; k++)
		v[k] = world_vector[0] * local_invse[k] +
		world_vector[1] * local_invse[k + 3] +
		world_vector[2] * local_invse[k + 6];
	local_vector[0] = v[0];
	local_vector[1] = v[1];
	local_vector[2] = v[2];
}

/*  2002/3/7 nt
 *	transform local_point 3D to world_point 3D which is defined in
 *	local frame with origin,X,Y,Z as it's origin,X,Y,Z axis
 *	nt modify 2003/2/16
 */
void mathTransLocalPnt3DByOXYZ(PNT3D origin,
	VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	PNT3D local_point,
	PNT3D world_point)
{
	PNT3D p;

	p[0] = origin[0] + local_point[0] * e1[0] + local_point[1] * e2[0] + local_point[2] * e3[0];
	p[1] = origin[1] + local_point[0] * e1[1] + local_point[1] * e2[1] + local_point[2] * e3[1];
	p[2] = origin[2] + local_point[0] * e1[2] + local_point[1] * e2[2] + local_point[2] * e3[2];
	world_point[0] = p[0];
	world_point[1] = p[1];
	world_point[2] = p[2];

	return;
}

/*  2002/3/28 nt
 *  calculate the intersection of cylinder/segment, consider the overlaping case
 *	return IDINT, IDNOINT
 *	t1, t2 are in [0.0, 1.0]. if begin coincident with p1(p2), t1(t2) is 0.0, if
 *	end is conincident with p1(p2), t1(t2) is 1.0
 */
int mathIntSegmCyl(PNT3D begin,
	PNT3D end,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	int* pn,
	PNT3D p1,
	PNT3D p2,
	double* pt1,
	double* pt2)
{
	int rt_value;
	double d;
	VEC3D dir;

	// 1.
	*pn = 0;

	// 2.
	dir[0] = end[0] - begin[0];
	dir[1] = end[1] - begin[1];
	dir[2] = end[2] - begin[2];
	if ((d = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2])) < MIN_DBL)
	{
		mathMidPnt(begin, end, p1);
		if (fabs(mathDistPntLin(p1, pivot, axis) - r) < tol)
		{
			*pt1 = 0.5;
			*pn = 1;
			return IDINT;
		}
		return IDNOINT;
	}
	dir[0] /= d;
	dir[1] /= d;
	dir[2] /= d;

	// 3.
	rt_value = mathIntLinCyl(begin, dir, pivot, axis, r, tol, MIN_ANG, p1, p2);
	if (rt_value == IDINT || rt_value == IDTAN || rt_value == IDOVERL)
	{
		if ((d = mathGetPntSegmNDT(p1, begin, end, pt1)) < tol)
			*pn = 1;
	}
	if (rt_value == IDINT)
	{
		if ((d = mathGetPntSegmNDT(p2, begin, end, pt2)) < tol)
		{
			(*pn)++;
			if (*pn == 1)
			{
				memcpy(p1, p2, sizeof(PNT3D));
				*pt1 = *pt2;
			}
		}
	}

	return *pn > 0 ? IDINT : IDNOINT;
}

// smf add 2023/02/24
// 共面线段的交点计算
int mathSegmIntSegmInFace(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, double iTol, STLPNT3D& oIntPoint)
{
	STLVECTOR v1, v2, v_b1_b2;
	v1 = iEnd1 - iBegin1;
	v2 = iEnd2 - iBegin2;
	v_b1_b2 = iBegin2 - iBegin1;

	STLVECTOR unit_v1, unit_v2, unit_v_b1_b2;
	unit_v1 = vectorNormalize(v1);
	unit_v2 = vectorNormalize(v2);

	double theta1 = unit_v1 * unit_v2;
	if (fabs(theta1 - 1) < TOLANGLE ||
		fabs(theta1 + 1) < TOLANGLE)
	{
		// 平行或共线

		// 判断是否在线段上
		if (mathIsPointInSegm(iBegin1, iBegin2, iEnd2, iTol) && 
			mathIsPointInSegm(iEnd1, iBegin2, iEnd2, iTol))
		{
			// 两点都在线段内
			return 2;
		}
	}

	double det_xy, det_yz, det_xz;

	det_xy = mathDet22(v1.x, -v2.x, v1.y, -v2.y);
	det_yz = mathDet22(v1.y, -v2.y, v1.z, -v2.z);
	det_xz = mathDet22(v1.x, -v2.x, v1.z, -v2.z);

	double t1 = -1000, t2 = -1000;
	if (det_xy < -iTol ||
		det_xy > iTol)
	{
		double det_1 = mathDet22(v_b1_b2.x, -v2.x, v_b1_b2.y, -v2.y);
		t1 = det_1 / det_xy;
		double det_2 = mathDet22(v1.x, v_b1_b2.x, v1.y, v_b1_b2.y);
		t2 = det_2 / det_xy;
	}
	else
		if (det_yz < -iTol ||
			det_yz > iTol)
		{
			double det_1 = mathDet22(v_b1_b2.y, -v2.y, v_b1_b2.z, -v2.z);
			t1 = det_1 / det_yz;
			double det_2 = mathDet22(v1.y, v_b1_b2.y, v1.z, v_b1_b2.z);
			t2 = det_2 / det_yz;
		}
		else
			if (det_xz < -iTol ||
				det_xz > iTol)
			{
				double det_1 = mathDet22(v_b1_b2.x, -v2.x, v_b1_b2.z, -v2.z);
				t1 = det_1 / det_xz;
				double det_2 = mathDet22(v1.x, v_b1_b2.x, v1.z, v_b1_b2.z);
				t2 = det_2 / det_xz;
			}

	if (t1 >= 0. && t1 <= 1. &&
		t2 >= 0. && t2 <= 1.) // 交点在线段上
	{
		oIntPoint = iBegin1 + t1 * v1;
		return IDINT;
	}
	return IDNOINT;
}

bool mathIsPointInSegm(STLPNT3D iPoint, STLPNT3D iBegin, STLPNT3D iEnd, double iTol)
{
	double bp, pe, be;
	bp = mathDist(iBegin, iPoint);
	pe = mathDist(iPoint, iEnd);
	be = mathDist(iBegin, iEnd);
	if (fabs(bp + pe - be) <= iTol)
	{
		return true;
	}
	return false;
}

STLPNT3D mathMidPoint(STLPNT3D iPoint1, STLPNT3D iPoint2)
{
	STLPNT3D mid_point;
	mid_point.x = 0.5 * (iPoint1.x + iPoint2.x);
	mid_point.y = 0.5 * (iPoint1.y + iPoint2.y);
	mid_point.z = 0.5 * (iPoint1.z + iPoint2.z);
	return mid_point;
}

BOOLEAN mathIntSegmCyl(
	STLVECTOR iSegmBegin, STLVECTOR iSegmEnd,	//线段的两个端点
	STLVECTOR iCylBegin, STLVECTOR iCylEnd,		// 圆柱轴线的起止点
	double iRadius,								// 圆柱半径
	double iTol,								// 容差 
	int &oNumIntPnts,							// 交点个数
	STLPNT3D *oIntPnts)						// 交点坐标
{
	PNT3D segm_begin, segm_end, cyl_begin, cyl_end, p1, p2;
	memcpy(segm_begin, &iSegmBegin, sizeof(STLPNT3D));
	memcpy(segm_end, &iSegmEnd, sizeof(STLPNT3D));
	memcpy(cyl_begin, &iCylBegin, sizeof(STLPNT3D));
	memcpy(cyl_end, &iCylEnd, sizeof(STLPNT3D));
	VEC3D axis;
	mathGetVecUnit(cyl_begin, cyl_end, axis);
	double pt1, pt2;
	int pn;
	oNumIntPnts = 0;
	if (mathIntSegmCyl(segm_begin, segm_end, cyl_begin, axis, iRadius, iTol, &pn, p1, p2, &pt1, &pt2) == IDINT)
	{
		if (!mathIsPointOnCylinderExtension(p1, cyl_begin, cyl_end, iRadius))
		{
			memcpy(&oIntPnts[oNumIntPnts], p1, sizeof(STLPNT3D));
			oNumIntPnts++;
		}
		if (pn == 2)
		{
			if (!mathIsPointOnCylinderExtension(p2, cyl_begin, cyl_end, iRadius))
			{
				memcpy(&oIntPnts[oNumIntPnts], p2, sizeof(STLPNT3D));
				oNumIntPnts++;
			}
		}
	}
	return (oNumIntPnts > 0) ? 1 : 0;
}

BOOLEAN mathIsPointOnCylinderExtension(PNT3D iPoint, PNT3D iCylBegin, PNT3D iCylEnd, double iRadius)
{
	VEC3D v1, v2;
	mathGetVecUnit(iCylBegin, iPoint, v1);
	mathGetVecUnit(iCylEnd, iPoint, v2);

	if (mathOProduct(v1, v2) > 0)
		return false;
	else
		return true;
}


// smf add 2022/12/10
// 根据三角形顶点的信息，找到三角形边上值等于给定值的一点
// ioIndex-等值顶点的索引
// 应该只返回0，1或2：
// 0-三角形内部没有等值点；
// 1-三角形的一个顶点为等值点，需遍历该顶点周围的所有三角形
// 2-三角形边上有两个等值点
int mathCalPointsByTriangleVertexInfo(STLPNT3D iVertex1, STLPNT3D iVertex2, STLPNT3D iVertex3, double iInfo[3], double iRef, double iTol, STLPNT3D ioPoint[3], int* ioIndex)
{
	ioIndex[0] = -1;
	ioIndex[1] = -1;
	ioIndex[2] = -1;
	STLPNT3D iVertex[3];
	int t = sizeof(STLPNT3D);
	memcpy(iVertex, &iVertex1, t);
	memcpy(iVertex + 1, &iVertex2, t);
	memcpy(iVertex + 2, &iVertex3, t);

	bool equal[3] = { false, false, false };
	int rc = 0;

	for (size_t i = 0; i < 3; i++)
	{
		if (fabs(iInfo[i] - iRef) < iTol)
		{
			equal[i] = true;
			ioIndex[i] = i;
			memcpy(ioPoint + rc, &iVertex + i, t); // 顶点值为给定值
			rc++;
		}
		else
			equal[i] = false;
	}

	//如果只有0个或1个顶点是等值点
	if (rc == 1 || rc == 0)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (equal[i] || equal[(i + 1) % 3])
				continue;
			else
			{
				if ((iRef >= iInfo[i]) && (iRef <= iInfo[(i + 1) % 3]) ||
					(iRef >= iInfo[(i + 1) % 3]) && (iRef <= iInfo[i]))
				{
					double tmp = (iRef - iInfo[i]) / (iInfo[(i + 1) % 3] - iInfo[i]);
					STLVECTOR delta_v = iVertex[(i + 1) % 3] - iVertex[i];
					ioPoint[rc] = iVertex[i] + tmp * delta_v; // 线性插值
					ioIndex[rc] = i;
					rc++;
				}
			}
		}
	}

	return rc;
}

BOOL mathIsVectorDuringTwoVectors(STLVECTOR iVector1, STLVECTOR iVector2, STLVECTOR iVector)
{
	STLVECTOR v1 = vectorNormalize(iVector1 ^ iVector), v2 = vectorNormalize(iVector ^ iVector2);
	double theta1 = acos(iVector1 * iVector), theta2 = acos(iVector2 * iVector);
	if (v1 * v2 >= 0 && (theta1 + theta2 < PI + TOLANGLE))
		return true;
	else
		return false;
}
