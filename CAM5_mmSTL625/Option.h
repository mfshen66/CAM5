#pragma once

////////////////////////////////////////////////////////////////
//	system options
//	nt add 2010/11/28
//1磅≈0.35毫米
//1英寸＝72磅
//小五号=9磅=3.163毫米
/*	字号与磅数的换算关系  
铅字印刷的时候，字体大小是使用字号作为单位的。
然而到了电脑排版横行的时代，字体的大小按照国际标准，
用“磅”作为单位了。不过由于两种标准不统一，所以没
有直接换算的公式。 

下面是字号与磅数换算表：其中打括号的字号表示以前很
少用的字号。磅值相对毫米的换算则可以使用
“磅数÷2.845=毫米数”的公式来进行换算，比如72磅的
大字约为25.3mm左右。同样的，也可以反过来使用测量出
的字大小乘于2.845来推算出磅数。或者按照1磅大约是
0.35mm进行换算。 

1英寸=2.54厘米=25.4毫米。 
编号   字号         磅数 
1     （大特号）    89 
2     （特号）      54  
3      初号         42 
4      小初号       36 
5     （大一号）    31.5       
6      一号         28    
7     （小一号）    24 
8      二号         21 
9      小二号       18 
10     三号         15.75 
11    （小三号）    15 
12     四号         14    
13     小四号       12 
14     五号         10.5 
15     小五号       9        
16     六号         7.875 
17     小六号       6.875 
18     七号         5.25 
19     八号         4.5 
*/
//--------------------------------------------------------------
typedef struct _option OPTION ;
struct _option
{
	CStr fontName ;
	double fontH ;
	int bkColor ;
	int unit ; // 0=mm, 1=m, 2=inch
	double gridSize ; // 网格边长
	double refPlaneHalfSize ;
	UINT simUpdateT ; // 更新CSim的时间间隔, unit=0.001second
	UINT simDispT ; // 重新绘制的时间间隔, unit=0.001second
	int colorIndex ; // 颜色索引
	int materialIndex ; // 材质索引
	int width ; // 线宽, 单位是0.01mm
	CStr ltName ; // 线型名称
	double tol ;
	double ang ;
	double curDTol ; // 曲线离散
	double curDAng ; // 曲线离散
	double curDChd ; // 曲线离散
	double surDTol ; // 曲面剖分
	double surDAng ; // 曲面剖分
	double surDChd ; // 曲面剖分
	double fitTol ; // 拟合容差, unit=mm
	double extL ; // 延伸长度, unit=mm
	double extR ; // 延伸半径, unit=mm
	double extA ; // 延伸圆心角, unit=弧度
	double attMin ; // 属性最小值
	double attMax ; // 属性最大值
	int nZPlane ; // 属性平面
	double pSize ; // 点的大小, unit=m
	int pSize2 ; // in pixel
	double r ; // 画圆
	double r1 ; // 画椭圆
	double r2 ; // 画椭圆
	double planeW ;
	double cylH ;
	double cylR ;
	double coneH ;
	double coneR ;
	double sphR ;
	double torusR1 ;
	double torusR2 ;
	int dimArrowType ; // 尺寸箭头类型
	double dimH ;
	int dimColor ;
	CStr dimFontName ;
	int dimLinearPre[2] ; // 线性尺寸precision, 显示小数点前后各几位
	int dimAngularPre[2] ; // 角度尺寸precision, 显示小数点前后各几位
	CStr hTName ; // 剖面线名称
	double hAng ; // 角度
	double hW1 ; // 剖面线间距1
	double hW2 ; // 剖面线间距2
	double hTol ; // 剖面线误差
	double symH ; // 符号高度
	CStr txtFontName ;
	double txtH ;
	int txtBkColor ;
	int txtColor ;
	// 625项目参数，nt add 2021/6/15
	double dTol ; // 弦高差 0.1mm
	double minEdge ; // 三角形最短边长，网格顶点最小距离，缺省值1.0mm
	double maxEdge ; // 三角片所在边最大长度，缺省值100.0mm
	double minAngle ; // 三角形最小角，单位=度
	double minArea ; // 三角片最小面积，缺省值1.0mm^2
	double bTol ; // 曲面片连接性判断，缺省值0.001mm，边界容差
	// 导航相关选项还未加...
} ;

/*
 *	Option.cpp
 */
int optionInit(OPTION* option) ;
int optionSerialize(OPTION* option, CArchive& ar) ;
int optionLoad(OPTION* option, CStr& pathName) ;
int optionSave(OPTION* option, CStr& pathName) ;
////////////////////////////////////////////////////////////////