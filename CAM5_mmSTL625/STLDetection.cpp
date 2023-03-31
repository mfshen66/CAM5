#include "stdafx.h"

#include "STLDetection.h"

void FindMinInTreeDoubles(double iArray[3], double &oMinValue, int &oIndex)
{
	oMinValue = iArray[0];
	oIndex = 0;
	for (size_t i = 1; i < 3; i++)
	{
		if (iArray[i] < oMinValue)
		{
			oMinValue = iArray[i];
			oIndex = i;
		}
	}
}

void NormalDetection(GridModel * iGridModel, int &oFNum, double &oMaxTheta)
{
	FList f = iGridModel->FHead;
	oFNum = 0;
	oMaxTheta = 1;

	double tolSquareLength = 1.e-8;
	double tolAngle = 1.e-6;

	while (f)
	{
		double norm[3];
		norm[0] = mathSquareDist(f->VertexUsed[0]->Coord, f->VertexUsed[1]->Coord);
		norm[1] = mathSquareDist(f->VertexUsed[1]->Coord, f->VertexUsed[2]->Coord);
		norm[2] = mathSquareDist(f->VertexUsed[2]->Coord, f->VertexUsed[0]->Coord);
		int index;
		double value;
		FindMinInTreeDoubles(norm, value, index);

		STLVECTOR v1 = f->VertexUsed[index]->Coord - f->VertexUsed[(index + 2) % 3]->Coord;
		STLVECTOR v2 = f->VertexUsed[(index + 1) % 3]->Coord - f->VertexUsed[(index + 2) % 3]->Coord;

		STLVECTOR n = v1 ^ v2;

		n = vectorNormalize(n);
		
		STLVECTOR fNormal = vectorNormalize(f->NormalVector);

		double theta = n * fNormal;

		if (theta < oMaxTheta)
		{
			oMaxTheta = theta;
			oFNum = f->FaceNO;
		}

		f = f->FNext;
	} 
}

void FindMinLengthOfEdge(GridModel * iGridModel, int & oFNum, int & oIndex, double & oMinSuqreLength)
{
	EList e = iGridModel->EHead;
	oFNum = 0;
	oIndex = 0;
	oMinSuqreLength = 1000000.;

	while (e)
	{
		double length = mathSquareDist(e->VertexUsed[0]->Coord, e->VertexUsed[1]->Coord);
		
		if (length < oMinSuqreLength)
		{
			oMinSuqreLength = length;
			FList f = e->FaceUsed->Face;
			oFNum = f->FaceNO;
			for (size_t i = 0; i < 3; i++)
			{
				if (f->EdgeUsed[i] == e)
				{
					oIndex = i;
				}
			}
		}

		e = e->ENext;
	}
}
