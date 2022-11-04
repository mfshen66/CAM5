/*	2009/5/9 nt
 *	the haed file of basic tools
 */
#ifndef _Tools_h_
#define _Tools_h_

////////////////////////////////////////////////////////////////
/*	2002/11/30 nt
 *	paper size
 */
enum EPaperSize
{
	psA4 = 0,
	psA4_ = 1,
	psA3 = 2,
	psA3_ = 3,
	psA2 = 4,
	psA2_ = 5,
	psA1 = 6,
	psA1_ = 7,
	psA0 = 8,
	psA0_ = 9,
	psA = 10,
	psA_ = 11,
	psB = 12,
	psC = 13,
	psD = 14,
	psE = 15,
	psUnknown = 100
} ;

void psGetWH(int ps, double& w, double& h) ;
//--------------------------------------------------------------
int ExtractPathName(CStr& full, CStr& pathName) ;
int ExtractPath(CStr& full, CStr& path) ; // nt add 2008/6/21
int ExtractExtName(CStr& full, CStr& extName) ;
int ExtractFileName(CStr& full, CStr& fileName) ;
//--------------------------------------------------------------
int fltToStr43(double d, char buf[9]) ;
int timeToStr(int t, CStr& str) ;
//==============================================================
int efpGet(char* efp, int size) ;
CStr cfFillFullPathName(char* fileName) ; // nt add 2020/5/12
//==============================================================
int licenseChk() ;
//==============================================================
FILE* cfOpen(char* fn, char* mode) ;
//==============================================================
/*
 *	double
 */
double trimValue(double value, int pre) ;
//--------------------------------------------------------------
// nt add 2009/10/21 : 从字符串中取数值
int strToNums(char* str, int& i) ;
int strToNums(char* str, double& d) ;
int strToNums(char* str, int& i1, int& i2) ;
int strToNums(char* str, double& d1, double& d2) ;
int strToNums(char* str, int& i, double& d) ;
int strToNums(char* str, double& d, int& i) ;
int strToNums(char* str, double& d1, double& d2, double& d3) ;
int strToNums(char* str, double& d1, double& d2, int& i) ;
int strToNums(CStr& str, int& i) ;
int strToNums(CStr& str, double& d) ;
int strToNums(CStr& str, int& i1, int& i2) ;
int strToNums(CStr& str, double& d1, double& d2) ;
int strToNums(CStr& str, int& i, double& d) ;
int strToNums(CStr& str, double& d, int& i) ;
int strToNums(CStr& str, double& d1, double& d2, double& d3) ;
int strToNums(CStr& str, double& d1, double& d2, int& i) ;
//==============================================================
void toM(PNT3D p) ;
void toM2(PNT2D p) ;
//==============================================================
/*double mathOProduct2(VEC3D v, PNT3D b, PNT3D e) ;
int arcMake(ARC* arc, 
			PNT3D center, 
			VEC3D normal, 
			PNT3D p1, 
			PNT3D p2,
			PNT3D p) ;*/
//--------------------------------------------------------------
typedef struct _win2d WIN2D ;
struct _win2d
{
	int w ; // 左上角是(0,0), 右下角是(w, h)
	int h ;
	double min[2] ;
	double max[2] ;
} ;

void win2dToIJ(WIN2D* pW, double p[2], int& i, int& j) ;
void win2dToXY(WIN2D* pW, int i, int j, double p[2]) ;
//--------------------------------------------------------------
// nt add 2011/7/7 for debug only
void dump(char* fileName, char* str) ;
void dump(char* fileName, int n) ;
void dump(char* fileName, double d) ;
void dump(char* fileName, double p[3]) ;
void dump(char* fileName, void* p) ;
//--------------------------------------------------------------
int bscurSaveASCII2(BSCUR* curv, CString& name) ;
int bssurSaveASCII2(BSSUR* surf, CString& name) ;
////////////////////////////////////////////////////////////////

#endif
