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

// 测地等距功能测试代码
#include <iostream>
using namespace std;
int calltimes = 1;
int TNum = 1;

#define FOR_NO_VERTEX_NORMAL "Debug only! For STL has no vertex normal!"
//#define FOR_VERTEX_NORMAL    "Debug only! For STL has vertex normal!"

#ifdef FOR_NO_VERTEX_NORMAL
#undef FOR_VERTEX_NORMAL
#endif // !FOR_NO_VERTEX_NORMAL

#ifdef FOR_VERTEX_NORMAL
#undef FOR_NO_VERTEX_NORMAL
#endif // FOR_VERTEX_NORMAL

void CCAMDoc::OnTest()
{
	GridModel* pGM = m_pPart->m_pGM;
	int n = pGM->TrailNumDum;
	POList polist1 = nullptr, offset = nullptr, offset_2 = nullptr;

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

		//POList polist1 = polist2->GeodesicOffsetPreprocessing(pGM);
		POList polist2 = polist1->GeodesicOffsetFlexibleNew(calltimes++ * 10, pGM);
		if (polist1)
		{
			polist1->PolylineCheck();
			//polist2->TNum = ++(pGM->TrailNumDum);
			int n_polist1 = 0;
			for (int i = 1; i <= (polist1->DNum); i++)
				n_polist1 += (polist1->ENum[i] - polist1->SNum[i] + 1); // 计算需要等距的点数
			for (size_t i = polist1->SNum[1]; i < n_polist1; i++)
			{
				//double dist = polist->CalGeodesicDistancePointToPl(pGM, polist2->FTrail[i], polist2->PTrail[i], 1);
				PNT3D p, q;
				memcpy(p, &polist1->PTrail[i], sizeof(PNT3D));
				memcpy(q, &polist1->PTrail[i + 1], sizeof(PNT3D));
				AddLin(p, q);
			}
			Redraw();
			polist1->Destroy();
			free(polist1);
			polist1 = nullptr;
		}

		if (polist2)
		{
			polist2->PolylineCheck();
			//polist2->TNum = ++(pGM->TrailNumDum);
			int n_polist2 = 0;
			for (int i = 1; i <= (polist2->DNum); i++)
				n_polist2 += (polist2->ENum[i] - polist2->SNum[i] + 1); // 计算需要等距的点数
			for (size_t i = polist2->SNum[1]; i < n_polist2; i++)
			{
				//double dist = polist->CalGeodesicDistancePointToPl(pGM, polist2->FTrail[i], polist2->PTrail[i], 1);
				PNT3D p, q;
				memcpy(p, &polist2->PTrail[i], sizeof(PNT3D));
				memcpy(q, &polist2->PTrail[i + 1], sizeof(PNT3D));
				AddLin(p, q);
			}
			Redraw();
			polist2->Destroy();
			free(polist2);
			polist2 = nullptr;
		}

	return;
}