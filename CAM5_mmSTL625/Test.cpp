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
void CCAMDoc::OnTest()
{
	//20221118版本
	GridModel* pGM = m_pPart->m_pGM;
	int n = pGM->TrailNumDum;

	//for (int j = 1; j <= n; j++)
	//{
		POList polist = pGM->POLHead[TNum++], offset = nullptr, offset_2 = nullptr;
		POList polist2 = polist->GeodesicOffsetFlexibleNew(/*calltimes++ * */10, pGM);
		if (polist2)
		{
			polist2->PolylineCheck();
			//polist2->TNum = ++(pGM->TrailNumDum);
			int n_polist2 = 0;
			for (int i = 1; i <= (polist2->DNum); i++)
				n_polist2 += (polist2->ENum[i] - polist2->SNum[i] + 1); // 计算需要等距的点数
			for (size_t i = polist2->SNum[1]; i < n_polist2; i++)
			{
				double dist = polist->CalGeodesicDistancePointToPl(pGM, polist2->FTrail[i], polist2->PTrail[i], 1);
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
	//}

	//POList polist2 = polist->GeodesicOffsetFlexibleNew(calltimes++ * 10, pGM);
	////polist2->PolylineCheck();
	//if (polist2)
	//{
	//	int n_polist2 = 0;
	//	for (int i = 1; i <= (polist2->DNum); i++)
	//		n_polist2 += (polist2->ENum[i] - polist2->SNum[i] + 1); // 计算需要等距的点数
	//	for (size_t i = polist2->SNum[1]; i < n_polist2; i++)
	//	{
	//		double dist = polist->CalGeodesicDistancePointToPl(pGM, polist2->FTrail[i], polist2->PTrail[i], 1);
	//		PNT3D p, q;
	//		memcpy(p, &polist2->PTrail[i], sizeof(PNT3D));
	//		memcpy(q, &polist2->PTrail[i + 1], sizeof(PNT3D));
	//		AddLin(p, q);
	//	}
	//	Redraw();
	//	polist2->Destroy();
	//	free(polist2);
	//	polist2 = nullptr;
	//}

	// 绘制三角形
	/*
	FList f1, f2;
	f1 = pGM->stlGetFacetFromNum(6116);
	f2 = pGM->stlGetFacetFromNum(5792);
	for (size_t i = 0; i < 3; i++)
	{
		PNT3D p, q;
		memcpy(p, &f1->VertexUsed[i]->Coord, sizeof(PNT3D));
		memcpy(q, &f1->VertexUsed[(i + 1) % 3]->Coord, sizeof(PNT3D));
		AddLin(p, q);
		memcpy(p, &f2->VertexUsed[i]->Coord, sizeof(PNT3D));
		memcpy(q, &f2->VertexUsed[(i + 1) % 3]->Coord, sizeof(PNT3D));
		AddLin(p, q);
	}
	*/

	//OutPutChordalHeight(chordal_height, offset->SNum[1], offset->ENum[1]);

	//delete[] chordal_height;
	//chordal_height = nullptr;

	return;
}