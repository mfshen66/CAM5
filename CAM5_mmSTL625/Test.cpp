#include "stdafx.h"
#include "CAM.h"
#include "Models.h"
#include "Mills.h"
#include "SelSet.h"
#include "CAMDoc.h"
#include "Model\SND.h"
#include "Dialogs.h"

// 测地等距功能测试代码
#include <iostream>
using namespace std;

//#define FOR_NO_VERTEX_NORMAL "Debug only! For STL has no vertex normal!"
#define FOR_VERTEX_NORMAL    "Debug only! For STL has vertex normal!"

#include "MacForTest.h"

// 测地等距功能测试代码
#include <iostream>
using namespace std;
int calltimes = 3;
int TNum = 1;
void CCAMDoc::OnTest()
{
	GridModel* pGM = m_pPart->m_pGM;
	int n = pGM->TrailNumDum;
	POList polist1 = nullptr;

#ifdef FOR_NO_VERTEX_NORMAL
	polist1 = pGM->POLHead[42];
#endif // FOR_NO_VERTEX_NORMAL

#ifdef FOR_VERTEX_NORMAL
	PL* polyline = nullptr;
	FList f1 = pGM->stlGetFacetFromNum(6177), f2 = pGM->stlGetFacetFromNum(6177 + 10);
	PNT3D p1, p2;
	p1[0] = f1->VertexUsed[0]->Coord.x;
	p1[1] = f1->VertexUsed[0]->Coord.y;
	p1[2] = f1->VertexUsed[0]->Coord.z;
	p2[0] = f2->VertexUsed[0]->Coord.x;
	p2[1] = f2->VertexUsed[0]->Coord.y;
	p2[2] = f2->VertexUsed[0]->Coord.z;
	pGM->CalGeodesicLineOptNew(f1, p1, f2, p2, TOLLENGTH, &polyline, 3);
	polist1 = pGM->CreateMTIPathFromPL(polyline);
#endif // FOR_VERTEX_NORMAL

		POList polist2 = polist1->GeodesicOffsetFlexibleNew(5, pGM);
		//polist2->PolylineExtrapolate(pGM);
		POLYLINE_DISPLAY(polist1);
#ifdef FOR_VERTEX_NORMAL
		POLYLINE_DESTROY(polist1);
#endif // FOR_VERTEX_NORMAL

		POLYLINE_DISPLAY(polist2);
		POLYLINE_DESTROY(polist2);

	return;
}