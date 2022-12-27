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
void CCAMDoc::OnTest()
{
	//20221118版本
	GridModel* pGM = m_pPart->m_pGM;
	POList polist = pGM->POLHead[42], offset = nullptr, offset_2 = nullptr;

	const double max_chordal_height = 20; // 设置弓高阈值
	POList polist1 = polist->GeodesicOffsetPreprocessing(pGM);
	int n_polist1 = 0;
	for (int i = 1; i <= (polist1->DNum); i++)
		n_polist1 += (polist1->ENum[i] - polist1->SNum[i] + 1); // 计算需要等距的点数
	for (size_t i = polist1->SNum[1]; i < n_polist1; i++)
	{
		PNT3D p, q;
		memcpy(p, &polist1->PTrail[i], sizeof(PNT3D));
		memcpy(q, &polist1->PTrail[i + 1], sizeof(PNT3D));
		AddLin(p, q);
	}
	//int n = 0;
	//for (int i = 1; i <= (polist->DNum); i++)
	//	n += (polist->ENum[i] - polist->SNum[i] + 1); // 计算需要等距的点数

	//double* chordal_height = new double[n + 1]; // 每个点的弓高
	//memset(chordal_height, 0, (n + 1) * sizeof(double));

	//double offsetDistance = -calltimes * 10.0;
	//offset = polist->GeodesicOffsetFlexible(offsetDistance, pGM);
	//++calltimes;

	//int n_offset = 0;
	//for (int i = 1; i <= (offset->DNum); i++)
	//	n_offset += (offset->ENum[i] - offset->SNum[i] + 1); // 计算需要等距的点数
	//for (size_t i = offset->SNum[1]; i < n_offset; i++)
	//{
	//	PNT3D p, q;
	//	memcpy(p, &offset->PTrail[i], sizeof(PNT3D));
	//	memcpy(q, &offset->PTrail[i + 1], sizeof(PNT3D));
	//	AddLin(p, q);
	//}

	//for (size_t i = offset->SNum[1]; i < n_offset; i++)
	//{
	//	if (i > offset->SNum[1] + 1)
	//	{
	//		STLVECTOR dir_last = vectorNormalize(offset->PTrail[i - 1] - offset->PTrail[i - 2]); // 前一条线段的方向
	//		STLVECTOR dir_current = vectorNormalize(offset->PTrail[i] - offset->PTrail[i - 1]); // 当前线段的方向
	//		if (dir_last * dir_current < 0)
	//		{
	//			ERROR_SELF_INT;
	//		}
	//	}
	//}
	Redraw();
	polist1->Destroy();
	
	//OutPutChordalHeight(chordal_height, offset->SNum[1], offset->ENum[1]);

	//delete[] chordal_height;
	//chordal_height = nullptr;

	return;
}