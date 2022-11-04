#include "stdafx.h"
#include "Option.h"

////////////////////////////////////////////////////////////////
int optionInit(OPTION* option)
{
	if( option )
	{
		option->fontName = "宋体" ;
		option->fontH = 9*0.35 ; // 小五号, unit=mm
		option->bkColor = 0 ; // black
		option->unit = 0 ; // 0=mm, 1=m, 2=inch
		option->gridSize = 10. ; // 网格边长, unit=mm
		option->refPlaneHalfSize = 25. ; // unit=mm
		option->colorIndex = 255 ; // 颜色索引, white
		option->materialIndex = 0 ; // 材质索引
		option->width = 0 ; // 线宽, 单位是0.01mm
		option->ltName = "" ; // 线型名称
		option->tol = 1.e-5 ; // unit=mm
		option->ang = 1.e-11 ; // unit=弧度
		option->curDTol = 0.1 ; // 曲线离散, unit=mm
		option->curDAng = 0.15 ; // 曲线离散, unit=弧度
		option->curDChd = 10. ; // 曲线离散, unit=mm
		option->surDTol = 0.5 ; // 曲面剖分, unit=mm
		option->surDAng = 0.15 ; // 曲面剖分, unit=弧度
		option->surDChd = 20. ; // 曲面剖分, unit=mm
		option->fitTol = 0.01 ; // unit=mm
		option->extL = 10. ;
		option->extR = 10. ;
		option->extA = PI1*0.5 ;
		option->attMin = -0.01 ;
		option->attMax = 0.01 ;
		option->nZPlane = 40 ;
		option->pSize = 2.5 ; // 点的大小, unit=mm
		option->pSize2 = 5 ; // 点的大小, unit=pixel
		option->r = 25. ; // 画圆
		option->r1 = 50. ; // 画椭圆
		option->r2 = 25. ; // 画椭圆
		option->planeW = 10. ; // unit=mm
		option->cylH = 50. ; // unit=mm
		option->cylR = 25. ; // unit=mm
		option->coneH = 50. ; // unit=mm
		option->coneR = 25. ; // unit=mm
		option->sphR = 10. ; // unit=mm
		option->torusR1 = 50. ; // unit=mm
		option->torusR2 = 25. ; // unit=mm
		option->dimArrowType = 0 ; // 尺寸箭头类型
		option->dimH = 9*0.35 ; // 小五号, unit=mm
		option->dimColor = 255 ; // white
		option->dimFontName = "宋体" ;
		option->dimLinearPre[0] = 5 ; // 线性尺寸precision, 显示小数点前后各几位
		option->dimLinearPre[1] = 3 ;
		option->dimAngularPre[0] = 3 ; // 角度尺寸precision, 显示小数点前后各几位
		option->dimAngularPre[1] = 4 ;
		option->hTName = "" ; // 剖面线名称
		option->hAng = PI1*0.25 ;
		option->hW1 = 2.5 ;
		option->hW2 = 0.25 ;
		option->hTol = 0.5 ;
		option->symH = 9*0.35 ; // 符号高度, 小五号, unit=mm
		option->txtFontName = "宋体" ;
		option->txtH = 9*0.35 ; // 小五号, unit=mm
		option->txtBkColor = 5 ;
		option->txtColor = 255 ; // white
		// nt add 2021/6/15
		option->dTol = 0.1 ; // nt add 2021/6/14
		option->minEdge = 1. ; // nt add 2021/6/14
		option->maxEdge = 100. ; // nt add 2021/6/14
		option->minAngle = 2. ; // nt add 2021/6/14
		option->minArea = 1. ; // nt add 2021/6/14
		option->bTol = 0.001 ; // nt add 2021/6/14
		// end
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
int optionSerialize(OPTION* option, CArchive& ar)
{
	if( !option )
		return 0 ;

	if( ar.IsStoring() )
	{
		ar << option->fontName
		   << option->fontH
		   << option->bkColor
		   << option->unit
		   << option->gridSize
		   << option->refPlaneHalfSize
		   << option->colorIndex
		   << option->materialIndex
		   << option->width
		   << option->ltName
		   << option->tol
		   << option->ang
		   << option->curDTol
		   << option->curDAng
		   << option->curDChd
		   << option->surDTol
		   << option->surDAng
		   << option->surDChd
		   << option->fitTol
		   << option->extL
		   << option->extR
		   << option->extA
		   << option->attMin
		   << option->attMax
		   << option->nZPlane
		   << option->pSize
		   << option->pSize2
		   << option->r
		   << option->r1
		   << option->r2
		   << option->planeW
		   << option->cylH
		   << option->cylR
		   << option->coneH
		   << option->coneR
		   << option->sphR
		   << option->torusR1
		   << option->torusR2
		   << option->dimArrowType
		   << option->dimH
		   << option->dimColor
		   << option->dimFontName
		   << option->dimLinearPre[0]
		   << option->dimLinearPre[1]
		   << option->dimAngularPre[0]
		   << option->dimAngularPre[1]
		   << option->hTName
		   << option->hAng
		   << option->hW1
		   << option->hW2
		   << option->hTol
		   << option->symH
		   << option->txtFontName
		   << option->txtH
		   << option->txtBkColor
		   << option->txtColor
		   << option->dTol
		   << option->minEdge
		   << option->maxEdge
		   << option->minAngle
		   << option->minArea
		   << option->bTol ;
	}
	else
	{
		ar >> option->fontName
		   >> option->fontH
		   >> option->bkColor
		   >> option->unit
		   >> option->gridSize
		   >> option->refPlaneHalfSize
		   >> option->colorIndex
		   >> option->materialIndex
		   >> option->width
		   >> option->ltName
		   >> option->tol
		   >> option->ang
		   >> option->curDTol
		   >> option->curDAng
		   >> option->curDChd
		   >> option->surDTol
		   >> option->surDAng
		   >> option->surDChd
		   >> option->fitTol
		   >> option->extL
		   >> option->extR
		   >> option->extA
		   >> option->attMin
		   >> option->attMax
		   >> option->nZPlane
		   >> option->pSize
		   >> option->pSize2
		   >> option->r
		   >> option->r1
		   >> option->r2
		   >> option->planeW
		   >> option->cylH
		   >> option->cylR
		   >> option->coneH
		   >> option->coneR
		   >> option->sphR
		   >> option->torusR1
		   >> option->torusR2
		   >> option->dimArrowType
		   >> option->dimH
		   >> option->dimColor
		   >> option->dimFontName
		   >> option->dimLinearPre[0]
		   >> option->dimLinearPre[1]
		   >> option->dimAngularPre[0]
		   >> option->dimAngularPre[1]
		   >> option->hTName
		   >> option->hAng
		   >> option->hW1
		   >> option->hW2
		   >> option->hTol
		   >> option->symH
		   >> option->txtFontName
		   >> option->txtH
		   >> option->txtBkColor
		   >> option->txtColor
		   >> option->dTol
		   >> option->minEdge
		   >> option->maxEdge
		   >> option->minAngle
		   >> option->minArea
		   >> option->bTol ;
	}

	return 1 ;
}

int optionLoad(OPTION* option, CStr& pathName)
{
	CFile file ;
	CString temp;
	temp = pathName;
	if( file.Open(temp, CFile::modeRead) )
	{
		CArchive ar(&file, CArchive::load) ;
		optionSerialize(option, ar) ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}

int optionSave(OPTION* option, CStr& pathName)
{
	CFile file ;
	CString temp;
	temp = pathName;
	if( file.Open(temp, CFile::modeWrite|CFile::modeCreate) )
	{
		CArchive ar(&file, CArchive::store) ;
		optionSerialize(option, ar) ;
		ar.Close() ;
		file.Close() ;
		return 1 ;
	}
	else
		return 0 ;
}
////////////////////////////////////////////////////////////////