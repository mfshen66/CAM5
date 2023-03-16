#pragma once

///////////////////////////////////////////////////////////////
// 相切校正算法接口
double math2dDist(PNT2D p1, PNT2D p2) ;
void math2dMidPnt(PNT2D p1, PNT2D p2, PNT2D mid_p) ;
int math2dGetVecUnit(PNT2D p1, PNT2D p2, double tol, VEC2D v) ; // nt add 2009/11/26
double math2dGetAngleUnit(VEC2D v1, VEC2D v2) ;
void math2dRotVec(double angle, VEC2D v, VEC2D rot_v) ;
double math2dDistPntLin(PNT2D p, PNT2D begin, VEC2D dir) ;
void math2dPrjPntLin(PNT2D p, PNT2D begin, VEC2D dir, PNT2D prj_p) ;
void math2dEvalLin(PNT2D begin, VEC2D dir, double u, PNT2D p) ;
int math2dIntLin(PNT2D begin1, VEC2D dir1, PNT2D begin2, VEC2D dir2, double tol, double ang, PNT2D intpt) ;
int math2dCalCRAAEx1(PNT2D p0, PNT2D p1, PNT2D p2, double tol, double ang, PNT2D center, double* pr, double* pa0, double* pa2) ;
//-------------------------------------------------------------
int pcCorLin(PNT2D p, PNT2D center, double r, double tol, PNT2D q) ;
int ccCorLin(PNT2D center1, double r1, PNT2D center2, double r2, double tol, PNT2D q1, PNT2D q2) ;
int llInt2D(PNT2D b1, VEC2D dir1, PNT2D b2, VEC2D dir2, double tol, double ang, PNT2D p) ;
int lcInt2D(PNT2D b, VEC2D dir, PNT2D c, double r, double tol, PNT2D p1, PNT2D p2) ;
int ccInt2D(PNT2D c1, double r1, PNT2D c2, double r2, double tol, PNT2D p1, PNT2D p2) ;
int triCalCR2D(PNT2D p1, PNT2D p2, PNT2D p3, double* pr) ;
int toCir2D(PNT2D c, double r, PNT2D p) ;
int ccGetTP(PNT2D c1, double r1, PNT2D c2, double r2, PNT2D tp) ;
int llCorArc(PNT2D b1, VEC2D dir1, PNT2D b2, VEC2D dir2, double tol, PNT2D p1, PNT2D p2, PNT2D p3, PNT2D cc, double* pr) ;
int lcCorArc(PNT2D b, VEC2D dir, PNT2D c, double R, double tol, PNT2D p1, PNT2D p2, PNT2D p3, PNT2D cc, double* pr) ;
int ccCorArc(PNT2D c1, double r1, PNT2D c2, double r2, double tol, PNT2D p1, PNT2D p2, PNT2D p3, PNT2D cc, double* pr) ;
//-------------------------------------------------------------
// smf add 2023/02/21
// 两共面的线段求交
int mathSegmIntSegmInFace(PNT3D iBegin1, PNT3D iEnd1, PNT3D iBegin2, PNT3D iEnd2, double iTol, PNT3D oIntPoint);

double mathDet22(double a11, double a12, double a21, double a22); // A的行列式，|A|
///////////////////////////////////////////////////////////////