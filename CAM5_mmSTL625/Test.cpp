#include "stdafx.h"
#include "CAM.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "CAMDoc.h"
#include "Model\SND.h"
#include "Dialogs.h"

/*void CCAMDoc::OnTest()
{
	int i, n = GetSelected(60, 2), rt ;
	double gc[3] ;
	if( n == 2 )
	{
		FList fs[2] ;
		double ps[2][3] ;
		for( i = 0 ; i < 2 ; i++ )
		{
			fs[i] = (FList)GetObj(i) ;
			gc[0] = 1./3 ; //GetU(i) ;
			gc[1] = 1./3 ; //GetV(i) ;
			gc[2] = 1.-gc[0]-gc[1] ;
			fs[i]->GetPos(gc, ps[i]) ;
		}

		Pl* polyline = NULL ;
		if( m_pPart->m_pGM == NULL )
			return ;
		rt = m_pPart->m_pGM->CalGeodesicLine(fs[0], ps[0], fs[1], ps[1], 1.e-6, &polyline) ;
		if( polyline )
		{
			m_pPart->m_pGM->polylines[m_pPart->m_pGM->nPolyline] = polyline ;
			(m_pPart->m_pGM->nPolyline)++ ;
		}
	}

	return ;
}*/

/*void CCAMDoc::OnTest()
{
	int i, n = GetSelected(60, 1) ;
	double gc[3], t, d ;
	if( n == 1 )
	{
		FList fs[2] ;
		double ps[2][3] ;
		fs[0] = (FList)GetObj(0) ;
		gc[0] = 1./3 ; //GetU(i) ;
		gc[1] = 1./3 ; //GetV(i) ;
		gc[2] = 1.-gc[0]-gc[1] ;
		fs[0]->GetPos(gc, ps[0]) ;
		
		d = m_pPart->m_pGM->POLHead[1]->Snap(m_pPart->m_pGM, fs, ps, 1.e-6, i, t) ;
		AddPt(ps[0]) ;
		AddPt(ps[1]) ;
		AddPt(&fs[1]->VertexUsed[0]->Coord.x) ;
		AddPt(&fs[1]->VertexUsed[1]->Coord.x) ;
		AddPt(&fs[1]->VertexUsed[2]->Coord.x) ;
	}

	return ;
}*/

// PolyLines距离计算功能测试代码
//void CCAMDoc::OnTest()
//{
//	int i, k ;
//	GridModel* pGM = m_pPart->m_pGM ;
//	POList polys[2] ;
//	polys[0] = pGM->POLHead[11] ; // 11
//	polys[1] = pGM->POLHead[12] ; // 12
//	GDINFO* pGDI ;
//	pGM->CalGDInfo(polys, 1.e-3, &pGDI) ;
//	if( pGDI )
//	{
//		for( k = 0 ; k < 2 ; k++ )
//		{
//			for(i = 1 ; i <= pGDI->ns[k] ; i++ )
//			{
//				if( pGDI->pps[k][i].I > 0 &&
//					pGDI->pps[k][i].perp == 1 )
//					AddLin(pGDI->pps[k][i].ps[0], pGDI->pps[k][i].ps[1]) ;
//			}
//		}
//		delete pGDI ;
//		pGDI = NULL ;
//	}
//
//	return ;
//}

// 两点间测地线计算功能测试代码
//void CCAMDoc::OnTest()
//{
//	GridModel* pGM = m_pPart->m_pGM;
//	// Get Face
//	FList f1 = pGM->stlGetFacetFromNum(800); // 10 & 80
//	FList f2 = pGM->stlGetFacetFromNum(10);  
//	PNT3D p1, p2;
//
//	double t00 = 0.5, t01 = 0.5, t02 = 0.5;
//	double t10 = 0.5, t11 = 0.5, t12 = 0.5;
//	p1[0] = ((1 - t02) * (1 - t00) + t02 * (1 - t01)) * f1->VertexUsed[0]->Coord.x + ((1 - t02) * t00) * f1->VertexUsed[1]->Coord.x + (t01 * t02) * f1->VertexUsed[2]->Coord.x;
//	p1[1] = ((1 - t02) * (1 - t00) + t02 * (1 - t01)) * f1->VertexUsed[0]->Coord.y + ((1 - t02) * t00) * f1->VertexUsed[1]->Coord.y + (t01 * t02) * f1->VertexUsed[2]->Coord.y;
//	p1[2] = ((1 - t02) * (1 - t00) + t02 * (1 - t01)) * f1->VertexUsed[0]->Coord.z + ((1 - t02) * t00) * f1->VertexUsed[1]->Coord.z + (t01 * t02) * f1->VertexUsed[2]->Coord.z;
//
//	p2[0] = ((1 - t12) * (1 - t10) + t12 * (1 - t11)) * f2->VertexUsed[0]->Coord.x + ((1 - t12) * t10) * f2->VertexUsed[1]->Coord.x + (t11 * t12) * f2->VertexUsed[2]->Coord.x;
//	p2[1] = ((1 - t12) * (1 - t10) + t12 * (1 - t11)) * f2->VertexUsed[0]->Coord.y + ((1 - t12) * t10) * f2->VertexUsed[1]->Coord.y + (t11 * t12) * f2->VertexUsed[2]->Coord.y;
//	p2[2] = ((1 - t12) * (1 - t10) + t12 * (1 - t11)) * f2->VertexUsed[0]->Coord.z + ((1 - t12) * t10) * f2->VertexUsed[1]->Coord.z + (t11 * t12) * f2->VertexUsed[2]->Coord.z;
//
//	PL* geod = NULL;
//	double tol = 1e-6;
//	double arcLen[4];	// 4 methods arc
//						// 0 - no opt
//						// 1 - normal opt
//						// 2 - LM opt
//						// 3 - normal opt & LM opt
//	double calTime[4];
//	for (int k = 0; k < 4; k++) {
//		clock_t start = clock();
//
//		// pGM->CalGeodesicLine(f1, p1, f2, p2, tol, &geod, k);
//		pGM->CalGeodesicLineOpt(f1, p1, f2, p2, tol, &geod, k); // add by jh, 2022/10/17
//		
//		clock_t end = clock();
//		calTime[k] = ((double)end - start) / CLOCKS_PER_SEC;
//		if (geod) {
//			geod->CalArcLen();
//			arcLen[k] = geod->arcLen;
//			for (int i = 0; i < geod->pogs.GetSize() - 1; i++) {
//				AddLin(((POG*)geod->pogs[i])->p, ((POG*)geod->pogs[i + 1])->p);
//				CRefPt* pRefPt = new CRefPt(0, 5*k, ((POG*)geod->pogs[i])->p);
//				m_pPart->m_features.Add(pRefPt);
//			}
//
//			delete geod;
//		}
//	}
//	Redraw();
//
//	return;
//}

// 测地等距功能测试代码
#include <iostream>
using namespace std;
void CCAMDoc::OnTest()
{
	//PNT3D iPntOfSegment1 = { -1, -1, -1 }, iPntOfSegment2 = { 1, 1, 1 },  // 线段的两个端点
	//	iPntOnPlane = { 0, 0, 0 };
	//VEC3D iNormPlane = {0, 0, 1};  // 平面的法矢及平面上一点
	//double iTolLength = TOLLENGTH, iTolAngle = TOLANGLE;  // 长度容差及角度容差
	//PNT3D oPntsOfIntersection[2] = { 1, 1, 1 };
	//int res = mathSegmentIntPln(iPntOfSegment1, iPntOfSegment2, iPntOnPlane, iNormPlane, iTolLength, iTolAngle, oPntsOfIntersection);

	//PNT3D iPntOfTri1 = { 1, 1, 1 }, iPntOfTri2 = { 0, 1, 0 }, iPntOfTri3 = { 1, 0, 1 }, // 三角形的三个顶点
	//	iPntOnPlane = { 0, 0, 0 };
	//VEC3D iNormPlane = { 0, 0, 1 };  // 平面的法矢及平面上一点
	//double iTolLength = TOLLENGTH, iTolAngle = TOLANGLE; // 长度容差及角度容差
	//PNT3D oPntsOfIntersection[3] = { 1 };

	//int res = mathPlnIntTri(
	//	iPntOfTri1, iPntOfTri2, iPntOfTri3, // 三角形的三个顶点
	//	iPntOnPlane, iNormPlane,  // 平面的法矢及平面上一点
	//	iTolLength, iTolAngle,  // 长度容差及角度容差
	//	oPntsOfIntersection); // 交点数组的指针

	//STLPNT3D iSTLPntOfTri1 = { 1, 1, 1 }, iSTLPntOfTri2 = { 0, 1, 0 }, iSTLPntOfTri3 = { 1, 0, 1 }, // 三角形的三个顶点
	//	iSTLPntOnPlane = { 0, 0, 0 };
	//STLVECTOR iSTLNormPlane = { 0, 0, 1 };  // 平面的法矢及平面上一点
	//double iTolLength = TOLLENGTH, iTolAngle = TOLANGLE; // 长度容差及角度容差
	//STLPNT3D oSTLPntsOfIntersection[3];
	//int res = mathPlnIntTri(
	//	iSTLPntOfTri1, iSTLPntOfTri2, iSTLPntOfTri3, // 三角形的三个顶点
	//	iSTLPntOnPlane, iSTLNormPlane,  // 平面的法矢及平面上一点
	//	iTolLength, iTolAngle,  // 长度容差及角度容差
	//	oSTLPntsOfIntersection); // 交点数组的指针

	GridModel* pGM = m_pPart->m_pGM;

	POList polist = pGM->POLHead[2], offset = nullptr, offset_2 = nullptr;

	int n = 0;
	for (int i = 1; i <= (polist->DNum); i++)
		n += (polist->ENum[i] - polist->SNum[i] + 1); // 计算需要等距的点数
	double* chordal_height = new double[n + 1]; // 每个点的弓高

	offset = polist->GeodesicOffsetNonFlexible(-25, pGM, chordal_height);

	offset_2 = polist->GeodesicOffsetFlexible(25, pGM, chordal_height);

	int n_offset = 0, n_offset_2 = 0;
	for (int i = 1; i <= (offset->DNum); i++)
		n_offset += (offset->ENum[i] - offset->SNum[i] + 1); // 计算需要等距的点数
	for (int i = 1; i <= (offset_2->DNum); i++)
		n_offset_2 += (offset_2->ENum[i] - offset_2->SNum[i] + 1); // 计算需要等距的点数
	for (size_t i = offset->SNum[1]; i < n_offset; i++)
	{
		PNT3D p, q;
		memcpy(p, &offset->PTrail[i], sizeof(PNT3D));
		memcpy(q, &offset->PTrail[i + 1], sizeof(PNT3D));
		AddLin(p, q);
	}
	for (size_t i = offset_2->SNum[1]; i < n_offset_2; i++)
	{
		PNT3D p, q;
		memcpy(p, &offset_2->PTrail[i], sizeof(PNT3D));
		memcpy(q, &offset_2->PTrail[i + 1], sizeof(PNT3D));
		AddLin(p, q);
	}
	Redraw();

	// 输出弓高数据至C:\\Test\\OutPutChordalHeight.txt。
	OutPutChordalHeight(chordal_height, offset->SNum[1], offset->ENum[1]);

	/*PNT3D p = { 0, 0, 0 }, begin = { 0, 0, -1 }, end = { 0, 0, 1 }, p1 = { 1, 1, 0 }, p2 = { -1, -1, 0 };
	VEC3D v = { 0, 0, 1 };
	double r = 1, tol = TOLLENGTH;
	int pn = 0;
	PNT3D p_int1, p_int2;
	mathIntSegmCyl(begin, end, p, v, r, tol, &pn, p1, p2, p_int1, p_int2);*/
	delete[] chordal_height;
	chordal_height = nullptr;

	return;
}
