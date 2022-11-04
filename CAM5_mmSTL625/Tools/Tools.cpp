#include "StdAfx.h"

////////////////////////////////////////////////////////////////
//	paper size tool
void psGetWH(int ps, double& w, double& h) // unit is metre
{
	switch(ps)
	{
	case psA0:
		w = 1.189 ;
		h = 0.841 ;
		break ;
	case psA0_:
		w = 0.841 ;
		h = 1.189 ;
		break ;
	case psA1:
		w = 0.841 ;
		h = 0.594 ;
		break ;
	case psA1_:
		w = 0.594 ;
		h = 0.841 ;
		break ;
	case psA2:
		w = 0.594 ;
		h = 0.420 ;
		break ;
	case psA2_:
		w = 0.420 ;
		h = 0.594 ;
		break ;
	case psA3:
		w = 0.420 ;
		h = 0.297 ;
		break ;
	case psA3_:
		w = 0.297 ;
		h = 0.420 ;
		break ;
	case psA4:
		w = 0.297 ;
		h = 0.210 ;
		break ;
	case psA4_:
		w = 0.210 ;
		h = 0.297 ;
		break ;
	case psA:
		w = 0.2794 ;
		h = 0.2159 ;
		break ;
	case psA_:
		w = 0.2159 ;
		h = 0.2794 ;
		break ;
	case psB:
		w = 0.4318 ;
		h = 0.2794 ;
		break ;
	case psC:
		w = 0.5588 ;
		h = 0.4318 ;
		break ;
	case psD:
		w = 0.8636 ;
		h = 0.5588 ;
		break ;
	case psE:
		w = 1.1176 ;
		h = 0.8636 ;
		break ;
	default:
		break ;
	}

	return ;
}
//==============================================================
/*	2006/6/22 nt
 *	提取路径名称（不含扩展名）
 */
int ExtractPathName(CStr& full, CStr& pathName)
{
	int i, n ;

	n = full.GetLength() ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( full[i] == '.' )
		{
			pathName = full.Left(i) ;
			return 1 ;
		}
	}
	pathName = full ;

	return 0 ;
}

/*	2008/6/21 nt
 *	提取路径
 */
int ExtractPath(CStr& full, CStr& path)
{
	int i, n ;

	n = full.GetLength() ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( full[i] == '\\' )
		{
			path = full.Left(i) ;
			return 1 ;
		}
	}
	path = full ;

	return 0 ;
}

/*	2006/6/22 nt
 *	提取扩展名
 */
int ExtractExtName(CStr& full, CStr& extName)
{
	int i, n ;

	n = full.GetLength() ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( full[i] == '.' )
		{
			extName = full.Right(n-i) ;
			return 1 ;
		}
	}
	extName = full ;

	return 0 ;
}

/*	2008/3/29 nt
 *	提取文件名称（含扩展名）
 */
int ExtractFileName(CStr& full, CStr& fileName)
{
	int i, n = full.GetLength() ;
	for( i = n-1 ; i >= 0 ; i-- )
	{
		if( full[i] == '\\' )
		{
			fileName = full.Right(n-1-i) ;
			return 1 ;
		}
	}
	fileName = full ;
	return 1 ;
}
//==============================================================
/*	2005.5.13 nt
 *	对任何浮点数都取4.3形式，可以带一个负号，皆转化为8个字符, 例如-1234.567转化
 *	为[-1234.567], -234.56转化[ -234.560], 注意空格
 *	nt modify 2005/12/6 : 不能出现这样的结果: -0.000
 *	nt modify 2007/2/9 : 如果小于-999.999或大于999.999, 要做截断处理
 */
int fltToStr43(double d, char buf[9])
{
	int i, k, n ;
	char tmp[16] ;

	// nt add 2007/2/9
	if( d > 999.999f )
		d = 999.999f ;
	else
	if( d < -999.999f )
		d = -999.999f ;

	for( k = 0 ; k < 9 ; k++ ) // initial value
		buf[k] = ' ' ;
	sprintf_s(tmp, "%4.3lf", d) ; // to character
	n = (int)strlen(tmp) ;
	if( n > 9 ) // check valid, max length like -9999.999
		return 0 ;
	for( k = n-1, i = 8 ; k >= 0 ; k--, i-- )
	{
		buf[i] = tmp[k] ;
	}
	// nt add 2005/12/6
	if( tmp[0] == '-' &&
		fabs(d) < 0.001 )
		buf[9-n] = ' ' ;

	return 1 ;
}

// nt add 2005/5/25
int timeToStr(int t, CStr& str)
{
	int h, m, s ;

	h = t/3600 ;
	m = (t%3600)/60 ;
	s = t%60 ;
	if( h > 99 )
		h = 99 ;
	str.Format("%02d:%02d:%02d", h, m, s) ;

	return 1 ;
}
//==============================================================
// nt add 2007/5/11
static int glSize ;
static char glEFP[256] ;
int efpGet(char* efp, int size)
{
	if( efp &&
		size > 0 )
	{
		if( glSize < 1 )
		{
			HMODULE hModule = GetModuleHandle(NULL) ;
			GetModuleFileNameA(hModule, glEFP, 256) ;
			for( int i = (int)strlen(glEFP)-1, n = 0 ; i >= 0 && n < 2 ; i-- )
			{
				if( glEFP[i] == '\\' )
					n++ ;
				glEFP[i] = '\0' ;
			}
			glSize = (int)strlen(glEFP) ;
		}
		memcpy(efp, glEFP, sizeof(char)*(size>glSize?size:glSize)) ;
		return 1 ;
	}
	else
		return 0 ;
}

CStr cfFillFullPathName(char* fileName)
{
	char efp[256] ;
	efpGet(efp, 256) ;
	CStr pathName = efp ;
	pathName += "\\Libs\\" ;
	pathName += fileName ;

	return pathName ;
}
//==============================================================
int licenseChk()
{
	int i, n ;
	char buf[256] ;
	//GetMachineKey(256, buf) ;
	n = (int)strlen(buf) ;
	//Encrypt(n, buf) ;
	char efp[128] ;
	efpGet(efp, 128) ;
	strcat_s(efp, "\\License\\License.txt") ;
	FILE* fp = NULL ;
	fopen_s(&fp, efp, "r+t") ;
	if( !fp )
		return FALSE ;
	char code[256] ;
	fscanf_s(fp, "%s", code, 256) ;
	fclose(fp) ;
	for( i = 0 ; i < n ; i++ )
	{
		if( buf[i] != code[i] )
			return FALSE ;
	}
	return TRUE ;
}
//--------------------------------------------------------------
/*	2008/3/1 nt
 *	open file in director of config
 */
FILE* cfOpen(char* fn, char* mode)
{
	char pathName[128] ;
	efpGet(pathName, 128) ;
	strcat_s(pathName, "\\Config\\") ;
	strcat_s(pathName, fn) ;
	FILE* fp = NULL ;
	fopen_s(&fp, pathName, mode) ;

	return fp ;
}
//==============================================================
//	将实数按有效位数裁剪
double trimValue(double value, int pre)
{
	int i ;
	double d = value ;

	for( i = 0 ; i < pre ; i++ )
		d = d*10 ;
	if( d > 0 )
		d = (int)(d+0.5) ;
	else
		d = (int)(d-0.5) ;

	for( i = 0 ; i < pre ; i++ )
		d = d/10. ;

	return d ;
}
//==============================================================
// nt add 2009/10/21
int strToNums(char* str, int& i)
{
	int k, n = (int)strlen(str) ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( sscanf_s(str+k, "%d", &i) == 1 )
				return 1 ;
			else
				return 0 ;
		}
	}
	return 0 ;
}

int strToNums(CStr& str, int& i)
{
	return strToNums(str.GetBuffer(0), i) ;
}

// nt add 2009/10/21
int strToNums(char* str, double& d)
{
	int k = 0, n = (int)strlen(str) ;
	d = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			str[k] == '.' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( sscanf_s(str+k, "%lf", &d) == 1 )
				return 1 ;
			else
				return 0 ;
		}
	}
	return 0 ;
}

int strToNums(CStr& str, double& d)
{
	return strToNums(str.GetBuffer(0), d) ;
}

// nt add 2009/10/21
int strToNums(char* str, int& i1, int& i2)
{
	int k, n = (int)strlen(str), m = 0 ;
	i1 = 0 ;
	i2 = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m < 1 )
			{
				if( sscanf_s(str+k, "%d", &i1) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%d", &i2) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}

	return m ;
}

int strToNums(CStr& str, int& i1, int& i2)
{
	return strToNums(str.GetBuffer(0), i1, i2) ;
}

// nt add 2009/10/21
int strToNums(char* str, double& d1, double& d2)
{
	int k, n = (int)strlen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			str[k] == '.' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m < 1 )
			{
				if( sscanf_s(str+k, "%lf", &d1) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%lf", &d2) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CStr& str, double& d1, double& d2)
{
	return strToNums(str.GetBuffer(0), d1, d2) ;
}

// nt add 2009/10/21
int strToNums(char* str, int& i, double& d)
{
	int k, n = (int)strlen(str), m = 0 ;
	i = 0 ;
	d = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			(str[k] == '.' &&
			m == 1) ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m < 1 )
			{
				if( sscanf_s(str+k, "%d", &i) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%lf", &d) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CStr& str, int& i, double& d)
{
	return strToNums(str.GetBuffer(0), i, d) ;
}

// nt add 2009/10/21
int strToNums(char* str, double& d, int& i)
{
	int k, n = (int)strlen(str), m = 0 ;
	d = 0. ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			(str[k] == '.' &&
			m == 0) ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m < 1 )
			{
				if( sscanf_s(str+k, "%lf", &d) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%d", &i) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 2 )
				return 2 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CStr& str, double& d, int& i)
{
	return strToNums(str.GetBuffer(0), d, i) ;
}

// nt add 2009/10/21
int strToNums(char* str, 
			  double& d1, 
			  double& d2, 
			  double& d3)
{
	int k, n = (int)strlen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	d3 = 0. ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			str[k] == '.' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m == 0 )
			{
				if( sscanf_s(str+k, "%lf", &d1) != 1 )
					return 0 ;
			}
			else
			if( m == 1 )
			{
				if( sscanf_s(str+k, "%lf", &d2) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%lf", &d3) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 3 )
				return 3 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CStr& str, 
			  double& d1, 
			  double& d2, 
			  double& d3)
{
	return strToNums(str.GetBuffer(0), d1, d2, d3) ;
}


// nt add 2009/10/21
int strToNums(char* str, 
			  double& d1, 
			  double& d2, 
			  int& i)
{
	int k, n = (int)strlen(str), m = 0 ;
	d1 = 0. ;
	d2 = 0. ;
	i = 0 ;
	for( k = 0 ; k < n ; k++ )
	{
		if( str[k] == '-' ||
			str[k] == '.' ||
			(str[k] >= '0' &&
			str[k] <= '9') )
		{
			if( m == 0 )
			{
				if( sscanf_s(str+k, "%lf", &d1) != 1 )
					return 0 ;
			}
			else
			if( m == 1 )
			{
				if( sscanf_s(str+k, "%lf", &d2) != 1 )
					return 0 ;
			}
			else
			{
				if( sscanf_s(str+k, "%d", &i) != 1 )
					return 0 ;
			}
			m++ ;
			if( m == 3 )
				return 3 ;
			for( k = k+1 ; k < n ; k++ )
			{
				if( str[k] == ',' ||
					str[k] == ' ' )
					break ;
			}
		}
	}
	return m ;
}

int strToNums(CStr& str, 
			  double& d1, 
			  double& d2, 
			  int& i)
{
	return strToNums(str.GetBuffer(0), d1, d2, i) ;
}
//==============================================================
void toM(PNT3D p)
{
	p[0] *= 0.001 ;
	p[1] *= 0.001 ;
	p[2] *= 0.001 ;
}

void toM2(PNT2D p)
{
	p[0] *= 0.001 ;
	p[1] *= 0.001 ;
}
//==============================================================
// nt add 2010/1/21
// 两点相减再做内积
/*double mathOProduct2(VEC3D v, PNT3D b, PNT3D e)
{
	return v[0]*(e[0]-b[0])+
		   v[1]*(e[1]-b[1])+
		   v[2]*(e[2]-b[2]) ;
}*/

// nt add 2010/1/21
// center和normal决定了圆弧所在的平面以及圆弧的法矢
// p1和p2是圆弧的端点，
// p决定要那一部分的圆弧
// center是圆心
/*int arcMake(ARC* arc, 
			PNT3D center, 
			VEC3D normal, 
			PNT3D p1, 
			PNT3D p2,
			PNT3D p)
{
	VEC3D v1, v2, v ;
	double r1, r2, a1, a2 ;
	arc->lf.scale = 1. ;
	memcpy(arc->lf.O, center, sizeof(PNT3D)) ;
	memcpy(arc->lf.Z, normal, sizeof(VEC3D)) ;
	v1[0] = p1[0]-center[0] ;
	v1[1] = p1[1]-center[1] ;
	v1[2] = p1[2]-center[2] ;
	r1 = sqrt(v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]) ;
	v2[0] = p2[0]-center[0] ;
	v2[1] = p2[1]-center[1] ;
	v2[2] = p2[2]-center[2] ;
	r2 = sqrt(v2[0]*v2[0]+v2[1]*v2[1]+v2[2]*v2[2]) ;
	if( fabs(r1-r2) > MIN_LEN ||
		r1 < MIN_RAD ||
		r2 < MIN_RAD )
		return 0 ;
	v1[0] /= r1 ;
	v1[1] /= r1 ;
	v1[2] /= r1 ;
	v2[0] /= r2 ;
	v2[1] /= r2 ;
	v2[2] /= r2 ;
	v[0] = p[0]-center[0] ;
	v[1] = p[1]-center[1] ;
	v[2] = p[2]-center[2] ;
	mathUniVec(v, MIN_DBL) ;
	a1 = mathGetRAngleXZ(v1, normal, v2) ;
	a2 = mathGetRAngleXZ(v1, normal, v) ;
	if( a1 > a2 )
	{
		memcpy(arc->lf.X, v1, sizeof(VEC3D)) ;
		arc->a = a1 ;
	}
	else
	{
		memcpy(arc->lf.X, v2, sizeof(VEC3D)) ;
		arc->a = PI2-a1 ;
	}
	mathVProduct(arc->lf.Z, arc->lf.X, arc->lf.Y) ;
	mathUniVec(arc->lf.Y, MIN_DBL) ;
	mathVProduct(arc->lf.X, arc->lf.Y, arc->lf.Z) ;
	mathUniVec(arc->lf.Z, MIN_DBL) ;

	return 1 ;
}*/
//--------------------------------------------------------------
// nt add 2010/9/7
void win2dToIJ(WIN2D* pW, double p[2], int& i, int& j)
{
	i = (int)(pW->w*(p[0]-pW->min[0])/(pW->max[0]-pW->min[0])) ;
	j = (int)(pW->h*(pW->max[1]-p[1])/(pW->max[1]-pW->min[1])) ;
}

void win2dToXY(WIN2D* pW, int i, int j, double p[2])
{
	p[0] = pW->min[0]+i*(pW->max[0]-pW->min[0])/pW->w ;
	p[1] = pW->max[1]-j*(pW->max[1]-pW->min[1])/pW->w ;

}
//--------------------------------------------------------------
// nt add 2011/7/7
void dump(char* fileName, char* str)
{
	FILE *fp = NULL ;
	fopen_s(&fp, fileName, "a+t") ;
	if( !fp )
		return ;
	fprintf(fp, "%s\n", str) ;
	fclose(fp) ;
}

void dump(char* fileName, int n)
{
	FILE *fp = NULL ;
	fopen_s(&fp, fileName, "a+t") ;
	if( !fp )
		return ;
	fprintf(fp, "%d\n", n) ;
	fclose(fp) ;
}

void dump(char* fileName, double d)
{
	FILE *fp = NULL ;
	fopen_s(&fp, fileName, "a+t") ;
	if( !fp )
		return ;
	fprintf(fp, "%lf\n", d) ;
	fclose(fp) ;
}

void dump(char* fileName, double p[3])
{
	FILE *fp = NULL ;
	fopen_s(&fp, fileName, "a+t") ;
	if( !fp )
		return ;
	fprintf(fp, "(%lf %lf %lf)\n", p[0], p[1], p[2]) ;
	fclose(fp) ;
}

void dump(char* fileName, void* p)
{
	int i = *((int*)(&p)) ;
	FILE *fp = NULL ;
	fopen_s(&fp, fileName, "a+t") ;
	if( !fp )
		return ;
	fprintf(fp, "%d\n", i) ;
	fclose(fp) ;
}
//--------------------------------------------------------------
/*	2006/8/13 nt
*	save single NURBS curve
*/
int bscurSaveASCII2(BSCUR* curv, CString& name)
{
	int i, n;
	FILE* fp = NULL;

	if (!curv)
		return ERINPUT;

	_tfopen_s(&fp, name.GetBuffer(0), _T("w+t"));
	if (!fp)
		return ERUNSUC;
	fprintf_s(fp,
		"%d %d\n",
		curv->ord_u,
		curv->cnt_u);
	for (n = curv->ord_u + curv->cnt_u, i = 0; i < n; i++)
		fprintf_s(fp, "%5.15f\n", curv->u_k[i]);
	for (i = 0; i < curv->cnt_u; i++)
		fprintf_s(fp,
			"%5.15f %5.15f %5.15f %5.15f\n",
			curv->u_p[i][0],
			curv->u_p[i][1],
			curv->u_p[i][2],
			curv->u_p[i][3]);
	fprintf_s(fp, "\n");
	fclose(fp);

	return ERSUCSS;
}

/*	2006/8/13 nt
*	save single NURBS surface
*/
int bssurSaveASCII2(BSSUR* surf, CString& name)
{
	int i, j, n;
	FILE* fp = NULL;

	if (!surf)
		return ERINPUT;

	_tfopen_s(&fp, name.GetBuffer(0), _T("w+t"));
	if (!fp)
		return ERUNSUC;
	fprintf_s(fp,
		"%d %d %d %d\n",
		surf->ord_u,
		surf->cnt_u,
		surf->ord_v,
		surf->cnt_v);
	for (n = surf->ord_u + surf->cnt_u, i = 0; i < n; i++)
		fprintf_s(fp, "%5.15f\n", surf->u_k[i]);
	for (n = surf->ord_v + surf->cnt_v, j = 0; j < n; j++)
		fprintf_s(fp, "%5.15f\n", surf->v_k[j]);
	for (i = 0; i < surf->cnt_u; i++)
	{
		for (j = 0; j < surf->cnt_v; j++)
			fprintf_s(fp,
				"%5.15f %5.15f %5.15f %5.15f\n",
				surf->uv_p[i][j][0],
				surf->uv_p[i][j][1],
				surf->uv_p[i][j][2],
				surf->uv_p[i][j][3]);
	}
	fprintf(fp, "\n");
	fclose(fp);

	return ERSUCSS;
}
////////////////////////////////////////////////////////////////
