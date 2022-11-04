/*	2003/11/6 nt
 *	implementation file of class CPCell
 */
#include "StdAfx.h"
#include "DNAVI.h"
#include "MPoint.h"
#include "PCell.h"

////////////////////////////////////////////////////////////////
//	patch
/*	2003/11/6 nt
 *	patch
 */
CPCell::CPCell()
{
	m_mpoints = NULL ;
	m_color = RGB(0, 0, 0) ;
	m_m = 0 ;
	m_n = 0 ;
	m_type = 0 ;
}

CPCell::CPCell(int type)
{
	m_type = type ;
}

CPCell::CPCell(int n, double (*ps)[3])
{
	int i ;

	m_m = n ;
	m_n = 1 ;
	m_mpoints = new CMPoint[n] ;

	if( !m_mpoints )
		return ;
	for( i = 0 ; i < n ; i++ )
	{
		m_mpoints[i].m_p[0] = ps[i][0] ;
		m_mpoints[i].m_p[1] = ps[i][1] ;
		m_mpoints[i].m_p[2] = ps[i][2] ;
		m_mpoints[i].m_next = NULL ;
		m_mpoints[i].m_link = NULL ;
		m_mpoints[i].m_attrib = 1 ;
	}

	CalBox() ;

	return ;
}

CPCell::~CPCell()
{
	if( m_mpoints )
		delete []m_mpoints ;
}
//--------------------------------------------------------------
int	CPCell::GetNumOfMPoint()
{
	return m_m*m_n ;
}

CMPoint* CPCell::GetMPoint(int i)
{
	return &(m_mpoints[i]) ;
}

CMPoint* CPCell::GetMPointArray()
{
	return m_mpoints ;
}

int	CPCell::Load(CString& fileName)
{
	int i, k, n ;
	double x, y ; // nt add 2010/4/22
	FILE* fp = NULL ;
	char s[512] ;

	_tfopen_s(&fp, fileName.GetBuffer(0), _T("r")) ;
	if( !fp )
		return 0 ;
	fgets(s, 512, fp) ;
	fgets(s, 512, fp) ;
	fgets(s, 512, fp) ;
	fgets(s, 512, fp) ;
	fgets(s, 512, fp) ;
	fscanf_s(fp, "%d %d %d %d", &n, &m_m, &m_n, &k) ;
	if( n < 1 )
	{
		fclose(fp) ;
		return 0 ;
	}
	m_mpoints = new CMPoint[n] ;
	if( !m_mpoints )
	{
		m_m = 0 ;
		m_n = 0 ;
		fclose(fp) ;
		return 0 ;
	}
	for( i = 0 ; i < n ; i++ )
	{
		fscanf_s(fp, 
			   "%lf %lf %lf %d %lf %lf",
			   &(m_mpoints[i].m_p[0]),
			   &(m_mpoints[i].m_p[1]),
			   &(m_mpoints[i].m_p[2]),
			   &(m_mpoints[i].m_attrib),
			   &x,
			   &y) ;
		//m_mpoints[i].m_p[0] *= 0.001 ;
		//m_mpoints[i].m_p[1] *= 0.001 ;
		//m_mpoints[i].m_p[2] *= 0.001 ;
		m_mpoints[i].m_next = NULL ;
		m_mpoints[i].m_link = NULL ;
	}
	fclose(fp) ;

	CalBox() ;

	return 1 ;
}

// nt add 2018/7/14
int	CPCell::LoadPCD(CString& fileName)
{
	int i, j, k, n = 0 ;
	double x, y, z ;
	FILE* fp = NULL ;

	// release m_mpoints
	m_m = 0 ;
	m_n = 0 ;
	if( m_mpoints )
		delete []m_mpoints ;
	m_mpoints = NULL ;

	// n = how many points
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("r")) ;
	if( !fp )
		return 0 ;
	while(1)
	{
		if( fscanf_s(fp, "%lf %lf %lf %d %d %d", &x, &y, &z, &i, &j, &k) != 6 )
			break ;
		n++ ;
	}
	fclose(fp) ;

	// read all points
	_tfopen_s(&fp, fileName.GetBuffer(0), _T("r")) ;
	if( !fp )
		return 0 ;
	m_m = 1 ;
	m_n = n ;
	m_mpoints = new CMPoint[n] ;
	if( m_mpoints == NULL )
	{
		m_m = 0 ;
		m_n = 0 ;
		fclose(fp) ;
		return 0 ;
	}
	n = 0 ;
	while(1)
	{
		if( fscanf_s(fp, "%lf %lf %lf %d %d %d", &x, &y, &z, &i, &j, &k) != 6 )
			break ;
		m_mpoints[n].m_next = NULL ;
		m_mpoints[n].m_link = NULL ;
		m_mpoints[n].m_p[0] = x ;
		m_mpoints[n].m_p[1] = y ;
		m_mpoints[n].m_p[2] = z ;
		m_mpoints[n].m_normal[0] = 0. ;
		m_mpoints[n].m_normal[1] = 0. ;
		m_mpoints[n].m_normal[2] = 0. ;
		m_mpoints[n].m_k1 = 0. ;
		m_mpoints[n].m_k2 = 0. ;
		m_mpoints[n].m_v[0] = 0. ;
		m_mpoints[n].m_v[1] = 0. ;
		m_mpoints[n].m_v[2] = 0. ;
		m_mpoints[n].m_attrib = 0 ;
		n++ ;
	}
	fclose(fp) ;

	CalBox() ;

	return 1 ;
}

int	CPCell::CalBox()
{
	int i, k, n ;

	m_box.min[0] = MAX_DBL ;
	m_box.min[1] = MAX_DBL ;
	m_box.min[2] = MAX_DBL ;
	m_box.max[0] = -MAX_DBL ;
	m_box.max[1] = -MAX_DBL ;
	m_box.max[2] = -MAX_DBL ;
	for( n = m_m*m_n, i = 0 ; i < n ; i++ )
	{
		for( k = 0 ; k < 3 ; k++ )
		{
			if( m_box.min[k] > m_mpoints[i].m_p[k] )
				m_box.min[k] = m_mpoints[i].m_p[k] ;
			else
			if( m_box.max[k] < m_mpoints[i].m_p[k] )
				m_box.max[k] = m_mpoints[i].m_p[k] ;
		}
	}

	return 1 ;
}

int CPCell::GetBox(BOX3D* box)
{
	*box = m_box ;

	return 1 ;
}

int	CPCell::Draw(void* pVI, int drawMode)
{
	int i, n ;

	if( !pVI )
		return 0 ;

	switch(drawMode)
	{
	case dmNormal:
		glColor3ub(0, 0, 0) ;
		break ;
	case dmPrompt:
		glColor3ub(255, 0, 0) ;
		break ;
	case dmSelect:
		glColor3ub(0, 255, 0) ;
		break ;
	case dmActive:
		glColor3ub(255, 0, 255) ;
		break ;
	case dmEraze:
		glColor3ub(255, 255, 255) ;
		break ;
	default:
		return 0 ;
	}

	glPointSize(2.f) ;
	glBegin(GL_POINTS) ;
		for( n = m_m*m_n, i = 0 ; i < n ; i++ )
		{
			if( m_mpoints[i].m_attrib != -1 )
				glVertex3dv(m_mpoints[i].m_p) ;
		}
	glEnd() ;

	return 1 ;
}
//--------------------------------------------------------------
int CPCell::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
	}
	else
	{
	}

	return 1 ;
}
//--------------------------------------------------------------
/*	2003/11/8 nt
 *	dxf out points
 */
int CPCell::DxfOut(FILE* fp)
{
	int i, n ;

	// dxf out entities
	n = m_m*m_n ;
	for( i = 0 ; i < n ; i++ )
	{
		if( m_mpoints[i].m_attrib == -1 )
			continue ;

		fprintf(fp, "%3d\n", 0) ;
		fprintf(fp, "%s\n",  "POINT") ;
    
		fprintf(fp, "%3d\n", 10) ;
		fprintf(fp, "%lf\n", 1000.0*m_mpoints[i].m_p[0]) ;
		fprintf(fp, "%3d\n", 20) ;
		fprintf(fp, "%lf\n", 1000.0*m_mpoints[i].m_p[1]) ;
		fprintf(fp, "%3d\n", 30) ;
		fprintf(fp, "%lf\n", 1000.0*m_mpoints[i].m_p[2]) ;
	}

	return 1 ;
}
//--------------------------------------------------------------
int CPCell::IGESOutIndex(FILE* fp)
{
	int i, n ;
	// iges out entities Now it is point
	char s[9] = "01000200" ;
	char s1[9] = "00000200" ;
	
	n = m_m*m_n ;
	for( i = 1 ; i <= n ; i++ )
	{
		if( m_mpoints[i].m_attrib == -1 )
		{
			fprintf(fp, "%8d%8d%8d%8d%8d%24d%s%c%7d\n", 116, i, 0, 0, 0, 0, s1, 'D', 2*i-1) ;
			fprintf(fp, "%8d%8d%8d%8d%8d%24d%8d%c%7d\n", 116, 0, 1, 1, 0, 0, 0, 'D', 2*i) ;
		}
		else
		{
			fprintf(fp, "%8d%8d%8d%8d%8d%24d%s%c%7d\n", 116, i, 0, 0, 0, 0, s, 'D', 2*i-1) ;
			fprintf(fp, "%8d%8d%8d%8d%8d%24d%8d%c%7d\n", 116, 0, 1, 1, 0, 0, 0, 'D', 2*i) ;
		}
	}
	fprintf(fp, "%8d%8d%8d%8d%8d%24d%s%c%7d\n", 110, n+1, 1, 1, 0, 0, s, 'D', 2*n+1) ;
	fprintf(fp, "%8d%8d%8d%8d%8d%24d%8d%c%7d\n", 110, 0, 1, 1, 0, 0, 0, 'D', 2*n+2) ;

	return 1 ;	
}

int CPCell::IGESOutPara(FILE* fp)
{
	int i, n ;
	
	n = m_m*m_n;
	for( i = 1 ; i <= n ; i++ )
	{
		fprintf(fp, "%d,%12lf,%12lf,%12lf;%29d", 
				116, 
				1000.0*m_mpoints[i].m_p[0],
				1000.0*m_mpoints[i].m_p[1],
			    1000.0*m_mpoints[i].m_p[2],
				2*i-1) ;
		fprintf(fp, "%c%7d\n", 'P', i) ;
	}
	fprintf(fp, "%d,%d,%d,%d,%d,%d,%d;%56d", 110, 0, 0, 0, 0, 0, 0, 2*n+1) ;
	fprintf(fp, "%c%7d\n", 'P', n+1) ;

	return 1 ;
}
//--------------------------------------------------------------
int CPCell::VRMLOut(FILE* fp)
{	
	int i, n, row = 0 ;
	
	n = m_n*m_m ;
    for( i = 0 ; i < n ; i++, row++ )
	{	
		if( m_mpoints[i].m_attrib == -1 )
			continue ;
		if( m_mpoints[i].m_attrib != 1 )
			break ;
		if( row < 3 )
		{
			fprintf(fp, "%lf %lf %lf,", 
					1000*m_mpoints[i].m_p[0], 
					1000*m_mpoints[i].m_p[1],
					1000*m_mpoints[i].m_p[2]) ;
        }
		else
		{
			fprintf(fp, "%lf %lf %lf,\n",
					1000*m_mpoints[i].m_p[0],
					1000*m_mpoints[i].m_p[1],
					1000*m_mpoints[i].m_p[2]) ;
			row = 0 ;
		}
	}
	fprintf(fp, "%lf %lf %lf\n", 0.0000, 0.0000, 0.00000) ;

	return 1 ;
}

int CPCell::VRMLColorOut(FILE *fp, CString& bitsName)
{
	int n,row = 0;
	n = m_n * m_m ;
	CBitmap pBitmap ;
	CDC memDC ;
	CDC dc ;
	HDC hDC ;
	COLORREF colorRef ;
	BYTE red, blue, green ;
	int width, height ;
	CSize sizeMap ;
	BITMAP* pPBitmap = new BITMAP ;

	HBITMAP hBitmap = NULL ;
	hBitmap = (HBITMAP)LoadImage(NULL, bitsName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) ;
	pBitmap.Attach(hBitmap) ;
	// then load the bitmap into the memory
	memDC.CreateCompatibleDC(&dc) ;
	memDC.SelectObject(&pBitmap) ;
	pBitmap.GetBitmap(pPBitmap) ;
	width = pPBitmap ->bmWidth ;
	height = pPBitmap ->bmHeight ;
	double deltaRow = width/m_m ;
	double deltaLine = height/m_n ;
	hDC = memDC.GetSafeHdc() ;
	
	for( int i = 0 ; i < m_m ; i++ )
	{
		 for (int j = 0 ; j < m_n ; j++, row++ )
		 {
			if( m_mpoints[i*m_n+j].m_attrib == -1 )
				continue ;
			if (m_mpoints[i*m_n+j].m_attrib != 1 )
				break ;
			colorRef = ::GetPixel(hDC, (int)((j-m_box.min[0]*1000)*deltaRow), (int)(i*deltaLine)) ;
			red = GetRValue(colorRef) ;
			green = GetGValue(colorRef) ;
			blue = GetBValue(colorRef) ;
			if( row < 3)
			{
				fprintf(fp, "%lf %lf %lf,", ((double)red)/255, ((double)green)/255, ((double)blue)/255) ;
			}
			else
			{
				fprintf(fp, "%lf %lf %lf,\n", ((double)red)/255, (double(green))/255, ((double)blue)/255) ;
				row = 0 ;
			}
		 }
	 }
	 fprintf(fp, "%d %d %d", 0, 0, 0) ;

   	 return 1 ;
}
//--------------------------------------------------------------
int CPCell::Out3DS(CString& fileName)
{
	CFile file ;
	CString FilePath ;
	CMPoint point ;
	int i ;
	int  n = m_m*m_n ;
	float* pArrayx = new float[n] ;
	float* pArrayy = new float[n] ;
	float* pArrayz = new float[n] ;

	if( !file.Open(fileName.GetBuffer(0), CFile::modeReadWrite|CFile::typeBinary|CFile::modeCreate) )
	{
		TRACE("******************************************WRONG\n") ;
		return 0 ;
	}

	short chunk_main = 0x4d4d ;
	short chunk_version1 = 0x0002 ; // short
	long  chunk_unknow1 = 0x0003 ;
	short chunk_colorf = 0x0010 ; // 12
	short chunk_masterscale = 0x0100 ; // 4

	short chunk_mdata = 0x3d3d ;
	short chunk_version2 = 0x3d3e ; // 4
	long chunk_unknow2 = 0x0002 ; // 2
	short chunk_percent = 0x0030 ; // 2

	USHORT chunk_material = 0xafff ; // 0+sub
	USHORT chunk_matname = 0xa000 ; // varying
	CString str("AMOEBA_PATTERN") ;
	USHORT chunk_matdiffuse = 0xa020 ; // 0+sub
	USHORT chunk_shininessPer = 0xa040 ; //
	USHORT chunk_shininessStr = 0xa041 ; //
	USHORT chunk_specularcolor = 0xa030 ; // 0+sub
	USHORT chunk_object = 0x4000 ; // varying+sub
	CString str2("object") ;
	char c1 = '\0' ;
	short chunk_triobject = (short)0x4100 ; // 0+sub
	short chunk_vertice = (short)0x4110 ; //varying
	short chunk_logCoord = (short)0x4160 ; // 48
	short chunk_mapcoordlist = (short)0x4140 ; // varying
	short chunk_facedes = (short)0x4120 ; // varying+sub
	//short chunk_keyframer = 0xb000 ; // 0+sub
	//short chunk_keyheader = 0xb00a ;	
	//short chunk_framversion = 0x0005 ; //
	//short chunk_meshinfo = 0xb002 ; // 0+sub
	//short chunk_herPos = 0xb030 ; // 2
	//short chunk_objName = 0xb010 ; // varying
	//short chunk_pivotPoint = 0xb013 ; // 12
	//short chunk_posTrack = 0xb020 ;	// varying
	//short chunk_rotTrack = 0xb021 ;
	//short chunk_scaleTrack = 0xb022 ;
	CString strmatname("AMOEBA_PATTERN") ;
	CString strobject1("Null_Object") ;

	long num_vertice = m_m*m_n*12+8 ; // 4110
	//long num_verticelist = m_m*m_n*8 ; // 4140
	//long num_localcoord = 54 ; // 4160
	//long num_facedescription = (m_m-1)*(m_n-1) ; // 4120
	long num_material = 103 ;
	long num_matname = 21 ;
	long num_matdiffuse = 24 ;
	long num_colorf = 18 ;
	float f_color = 0.0f ;
	float f_colorr = 0.4f ;
	float f_colorg = 0.5f ;
	float f_colorb = 0.3f ;
	float f_scale = 4.0f ;
	float f_log1 = 1.0f ;
	float f_log2 = 0.0f ;
	float f_log3 = 1.0f ;
	int status = 7 ;
	int status0 = 0 ;

	long num_shininessper = 15 ;
	long num_percent = 8 ;
	int i_percent = 12 ;
	long num_shininessStr = 14 ;
	long num_specular = 24 ;
	long num_masterscale = 10 ;
	long num_object1 = 85 ;
	long num_triobject1 = 68 ;
	long num_vertice1 = 8 ;
	long num_logcoord = 54 ;
	long num_coordlist = 8*n+8 ;
	long num_facedes = (m_m-1)*(m_n-1)*8+8 ;
	short num_face = (short)((m_m-1)*(m_n-1)) ;

	long num_triobject = num_vertice+6+num_coordlist+num_logcoord+num_facedes ;
	long num_object = num_triobject+17 ;
	long num_mversion = 0x000a ;
	long num_mdata = num_object+num_object1+num_mversion+num_material+16 ;
	long num_version = 0x000a ;
	long num_main = num_mdata+16 ;

	file.Write(&chunk_main, 2) ;
	file.Write(&num_main, 4) ;
	file.Write(&chunk_version1, 2) ;
	file.Write(&num_version, 4) ;
	file.Write(&chunk_unknow1, 4) ;
	file.Write(&chunk_mdata, 2) ;
	file.Write(&num_mdata, 4) ;
	file.Write(&chunk_version2, 2) ;
	file.Write(&num_mversion, 4) ;
	file.Write(&chunk_unknow2, 4) ;
	file.Write(&chunk_material,2) ;
	file.Write(&num_material, 4) ;
	file.Write(&chunk_matname, 2) ;
	file.Write(&num_matname, 4) ;
	file.Write(strmatname, 14) ;
	file.Write(&c1, 1) ;
	file.Write(&chunk_matdiffuse, 2) ;
	file.Write(&num_matdiffuse, 4) ;
	file.Write(&chunk_colorf, 2) ;
	file.Write(&num_colorf, 4) ;
	file.Write(&f_color, 4) ;
	file.Write(&f_color, 4) ;
	file.Write(&f_color, 4) ;
	file.Write(&chunk_shininessPer, 2) ;
	file.Write(&num_shininessper, 4) ;
	file.Write(&chunk_percent, 2) ;
	file.Write(&num_percent, 4) ;
	file.Write(&i_percent, 2) ;
	file.Write(&chunk_shininessStr, 2) ;
	file.Write(&num_shininessStr, 4) ;
	file.Write(&chunk_percent, 2) ;
	file.Write(&num_percent, 4) ;
	file.Write(&i_percent, 2) ;
	file.Write(&chunk_specularcolor, 2) ;
	file.Write(&num_specular, 4) ;
	file.Write(&chunk_colorf, 2) ;
	file.Write(&num_colorf, 4) ;
	file.Write(&f_colorr, 4) ;
	file.Write(&f_colorg, 4) ;
	file.Write(&f_colorb, 4) ;
	file.Write(&chunk_masterscale, 2) ;
	file.Write(&num_masterscale, 4) ;
	file.Write(&f_scale, 4) ;
	file.Write(&chunk_object, 2) ;
	file.Write(&num_object1, 4) ;
	file.Write(strobject1, 10) ;
	file.Write(&c1,1) ;
	file.Write(&chunk_triobject, 2) ;
	file.Write(&num_triobject1, 4) ;
	file.Write(&chunk_vertice, 2) ;
	file.Write(&num_vertice1, 4) ;
	file.Write(&c1,1) ;
	file.Write(&c1,1) ;
	file.Write(&chunk_logCoord, 2) ;
	file.Write(&num_logcoord,4) ;
	file.Write(&f_log1, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log1, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log1, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	

	file.Write(&chunk_object, 2) ;
	file.Write(&num_object, 4) ;
	file.Write(str2, 6) ;
	file.Write(&c1, 1) ;
	file.Write(&chunk_triobject, 2) ;
	file.Write(&num_triobject, 4) ;
	file.Write(&chunk_vertice, 2) ;
	file.Write(&num_vertice, 4) ;
	file.Write(&n, 2) ;
	for( i = 0 ; i < n ; i++ )
	{	
		pArrayx[i] = (float)m_mpoints[i].m_p[0]*1000 ;
		pArrayy[i] = (float)m_mpoints[i].m_p[1]*1000 ;
		pArrayz[i] = (float)m_mpoints[i].m_p[2]*1000 ;
		file.Write(&(pArrayx[i]), 4) ;
		file.Write(&(pArrayy[i]), 4) ;
		file.Write(&(pArrayz[i]), 4) ;
	}
	TRACE("*********************************%d**********\n", sizeof(pArrayx[0])) ;
	file.Write(&chunk_mapcoordlist, 2) ;
	file.Write(&num_coordlist, 4) ;
	file.Write(&n, 2) ;

	float coordlist = 0.0 ;
	for(i = 0 ; i < n ; i++)
	{
		file.Write(&coordlist, 4) ;
		file.Write(&coordlist, 4) ;
	}

	file.Write(&chunk_logCoord, 2) ;
	file.Write(&num_logcoord, 4) ;
	file.Write(&f_log3, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log3, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log3, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&f_log2, 4) ;
	file.Write(&chunk_facedes, 2) ;
	file.Write(&num_facedes, 4) ;
	file.Write(&num_face, 2) ;
	for( i = 0 ; i < n-m_n ; i++ )
	{
		if( (m_mpoints[i].m_attrib == 1) &&
			(m_mpoints[i+1].m_attrib == 1) &&
			(m_mpoints[i+m_n].m_attrib ==1) )
		{
			file.Write(&i, 2) ;
			int j = i+1;
			file.Write(&j, 2) ;
			j = i+m_n;
			file.Write(&j, 2) ;
			file.Write(&(status), 2) ;
		}
		else
		{
			file.Write(&i, 2) ;
			int k = i+1;
			file.Write(&k, 2) ;
			k = i+m_n;
			file.Write(&k, 2) ;
			file.Write(&(status0), 2) ;
		}
	}

	file.Close() ;

	return 1 ;
}
//--------------------------------------------------------------
int CPCell::ObjOut(FILE* fp)
{
	int i, sum ;
	
	sum = m_m*m_n ;
	for( i = 0 ; i < sum ; i++ )
	{
		fprintf(fp, 
				"v %lf %lf %lf \n",
				m_mpoints[i].m_p[0]*100,
				m_mpoints[i].m_p[1]*100,
				m_mpoints[i].m_p[2]*100) ;
	}
	for( i = 0 ; i < sum-m_n ; i++ )
	{
		if( (i <= sum-m_n) &&
			(m_mpoints[i].m_attrib != -1) &&
			(m_mpoints[i+1].m_attrib != -1) &&
			(m_mpoints[i+m_n].m_attrib != -1) )
		{
			fprintf(fp, "f %d %d %d \n", i, i+1, i+m_n) ;
		}
	}
	for( i = sum ; i > m_n ; i-- )
	{
		if( (i >= m_n) &&
			(m_mpoints[i].m_attrib != -1) &&
			(m_mpoints[i-1].m_attrib != -1) &&
			(m_mpoints[i-m_n].m_attrib != -1) )
		{
			fprintf(fp, "f %d %d %d \n", i, i-1, i-m_n) ;
		}
	}
	
	return 1 ;
}
////////////////////////////////////////////////////////////////
