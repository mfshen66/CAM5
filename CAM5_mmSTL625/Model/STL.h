#ifndef _STL_h_
#define _STL_h_
#include <vector>

#define EPS06 1e-6
#define EPS12 1e-12
#define STLTRI 60

#define TOLLENGTH 1.e-3
#define TOLSQUARELENGTH 1.e-6
#define TOLANGLE 1.e-6

#define INVERSQRT2 0.7071067811865475244

typedef struct {
	double   x,y,z;
	double inline norm(){return sqrt(x * x + y * y + z * z);
	};
} STLVECTOR;

typedef struct {
	double   x,y;
} POINT2D;

typedef STLVECTOR  STLPNT3D;
typedef struct VertexList   *VList;
typedef struct EdgeList     *EList;
typedef struct FaceList     *FList;
typedef struct FaceRelated  *FRelated;
typedef struct MTIPathOriginList    *POList;
typedef struct GridModel GM ; // nt add 2022/6/15

// smf add 2022/09/07
STLVECTOR vectorNormalize(STLVECTOR& iVector);
STLVECTOR operator- (STLPNT3D& iEnd, STLPNT3D& iBegin);
STLVECTOR operator- (STLVECTOR& iVector); // ����ȡ��
STLVECTOR operator+ (STLVECTOR& iVector1, STLVECTOR& iVector2);
STLVECTOR operator* (double iScalar, STLVECTOR& iVector);
double operator*(STLVECTOR& iVector1, STLVECTOR& iVector2);
STLVECTOR operator^ (STLVECTOR& iVectorU, STLVECTOR& iVectorV);

struct VertexList {
	STLPNT3D     Coord;		
	STLVECTOR    NormalVector;
	VList        Rchild;
	VList        Lchild;
	FRelated     FaceUsed;
	int          FaceNum;
	int			 flag ; // nt add 2022/6/17

	VList SchMinVert(double p[3], double& min) ;
};

struct EdgeList {
	VList         VertexUsed[2];
	EList         ENext;
	FRelated      FaceUsed;
	int           FaceNum;
	double		  FCircleTag;
	int			  FaceCircleTag;
};

struct FaceList : public CVO
{
	STLVECTOR   NormalVector;
	VList       VertexUsed[3];
	EList       EdgeUsed[3];
	FList       FNext;
	int         FaceNO;
	double		  FCircleTag;
	FaceList() ;
	~FaceList() ;
	int GetType() ;
	void GetVertPos(int k, double p[3]) ;
	void GetPos(double gc[3], double p[3]) ;
	int GetNumOfCV(FList f) ; // common vertex
	int IsAdj(FList f) ;
	int IsAdjVertex(FList f);
	FList GetAdj(int i, int j) ;
	FList GetAdjRayPass(double p[3], double dir[3], double tol) ; // nt add 2022/7/9
	EList GetEdge(int i, int j) ;
	VList SchMinVert(double p[3], double& min) ;
	int PlaneCut(double pivot[3], double normal[3], double p[3], double dir[3], double tol, double q[3], int& i, int& j) ;
	int IsIn(double p[3], double tol) ;
	int IsIn2(double p[3], double tol, double prj_p[3]) ; // nt add 2022/7/9 p��ͶӰ��Face�ڣ����߽磩
	void Prj(double p[3], double prj_p[3]) ;
	void GetBarycenter(double center[3]) ;
	int Draw(void* pVI, int drawMode) ;
	int Draw2(void* pVI, int drawMode) ;
};

// �߶����߶���
int mathSegmIntSegm(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, STLPNT3D &oPoint1, STLPNT3D &oPoint2);

// �߶���ƽ����
int mathSegmentIntPln(PNT3D iPntOfSegment1, PNT3D iPntOfSegment2,  // �߶ε������˵�
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	PNT3D* oPntsOfIntersection); // ���������ָ��

// ƽ������������
int mathPlnIntTri(
	PNT3D iPntOfTri1, PNT3D iPntOfTri2, PNT3D iPntOfTri3, // �����ε���������
	PNT3D iPntOnPlane, VEC3D iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	PNT3D* oPntsOfIntersection, // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

int mathPlnIntTri(
	STLPNT3D iPntOfTri1, STLPNT3D iPntOfTri2, STLPNT3D iPntOfTri3, // �����ε���������
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	STLPNT3D* oPntsOfIntersection,  // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

int mathPlnIntTri(
	FList iTriangle, // ������
	STLPNT3D iPntOnPlane, STLVECTOR iNormPlane,  // ƽ��ķ�ʸ��ƽ����һ��
	double iTolLength, double iTolAngle,  // �����ݲ�Ƕ��ݲ�
	STLPNT3D* oPntsOfIntersection,  // ���������ָ��
	int* oIndexOfIntEdge, int& oNumOfIntEdge); // ��ƽ���ཻ�ıߵ���ż�����

// ������ƽ������
double mathSquareDist(PNT3D p1, PNT3D p2);
double mathSquareDist(STLPNT3D p1, STLPNT3D p2);

// 2022/12/01 smf add 
// ָ���߶εĶ˵�
enum SEGMENT
{
	Begin = 0,
	End = +1
};
// 2022/12/01 smf add 
// ���̻�����һ���߶Ρ�iDist��Ϊ��ֵ��iDist < 0 ���̣�iDist > 0 ���졣
// iBeginOrEnd: 0-��㣻1-�յ㡣
// ����ֵ��0-�߶ι��̣��������̣�1-�ɹ���
int mathShortenOrLengthenSegmByDist(STLPNT3D &ioBegin, STLPNT3D &ioEnd, int iBeginOrEnd, double iDist);

BOOL mathIsCoincidentPoint(STLPNT3D iPoint1, STLPNT3D iPoint2);
// �����ľ���
double mathDist(STLPNT3D p1, STLPNT3D p2);

// ��ͶӰ��������
void mathPrjPntPln(STLPNT3D iPoint, STLPNT3D iPointOfPlane, STLVECTOR iNormalOfPlane, STLPNT3D& oProjectPoint);
// ����ͶӰ��������
void mathPrjVecPln(STLVECTOR iVector, STLVECTOR iNormal, STLVECTOR &oPrjVec);

void mathTransWorldVec3DByXYZ(VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	VEC3D world_vector,
	VEC3D local_vector);

int mathIntLinCyl(PNT3D begin,
	VEC3D dir,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	double ang,
	PNT3D intpt1,
	PNT3D intpt2);

void mathTransLocalPnt3DByOXYZ(PNT3D origin,
	VEC3D e1,
	VEC3D e2,
	VEC3D e3,
	PNT3D local_point,
	PNT3D world_point);

int mathIntSegmCyl(PNT3D begin,
	PNT3D end,
	PNT3D pivot,
	VEC3D axis,
	double r,
	double tol,
	int* pn,
	PNT3D p1,
	PNT3D p2,
	double* pt1,
	double* pt2);

// smf add 2023/02/24
// �����߶εĽ������
int mathSegmIntSegmInFace(STLPNT3D iBegin1, STLPNT3D iEnd1, STLPNT3D iBegin2, STLPNT3D iEnd2, double iTol, STLPNT3D& oIntPoint);

bool mathIsPointInSegm(STLPNT3D iPoint, STLPNT3D iBegin, STLPNT3D iEnd, double iTol);

STLPNT3D mathMidPoint(STLPNT3D iPoint1, STLPNT3D iPoint2); // �е�

#define ERROR_SELF_INT  \
{	\
char s_i[10];			\
sprintf_s(s_i, "%d.", i);	\
char msg[30] = "�Ⱦ������ཻ! i = ";	\
strcat_s(msg, s_i);	\
CString str_msg(msg);	\
MessageBox(NULL, (LPCWSTR)str_msg, (LPCWSTR)L"����", MB_OK);	\
}

#define ERROR_CHORDAL_HEIGHT  \
{	\
char s_i[10];			\
sprintf_s(s_i, "%d.", i);	\
char msg[30] = "��������볬����ֵ! i = ";	\
strcat_s(msg, s_i);	\
CString str_msg(msg);	\
MessageBox(NULL, (LPCWSTR)str_msg, (LPCWSTR)L"����", MB_OK);	\
}

// smf add 2022/11/01
// �ж�Բ�����ϵĵ��Ƿ����ӳ�����:
// iCylBegin, iCylEnd: Բ��������ֹ��
// iRadius: Բ���뾶
BOOLEAN mathIsPointOnCylinderExtension(PNT3D iPoint, PNT3D iCylBegin, PNT3D iCylEnd, double iRadius);

// smf add 2022/11/01
// �߶���Բ������: 
// iSegmBegin, iSegmEnd: �߶ε������˵�
// iCylBegin, iCylEnd: Բ��������ֹ��
// iRadius: Բ���뾶
// oNumIntPnts: �������
// oIntPnts: ��������
BOOLEAN mathIntSegmCyl(
	STLVECTOR iSegmBegin, STLVECTOR iSegmEnd,	//�߶ε������˵�
	STLVECTOR iCylBegin, STLVECTOR iCylEnd,		// Բ�����ߵ���ֹ��
	double iRadius,								// Բ���뾶
	double iTol,								// �ݲ� 
	int &oNumIntPnts,							// �������
	STLVECTOR *oIntPnts);						// ��������

// smf add 2022/12/10
// ���������ζ������Ϣ���ҵ������α���ֵ���ڸ���ֵ��һ��
// ioIndex-��ֵ���������
// Ӧ��ֻ����0��1��2��
// 0-�������ڲ�û�е�ֵ�㣻
// 1-�����ε�һ������Ϊ��ֵ�㣬������ö�����Χ������������
// 2-�����α�����������ֵ��
int mathCalPointsByTriangleVertexInfo(
	STLPNT3D iVertex1, STLPNT3D iVertex2, STLPNT3D iVertex3, 
	double iInfo[3], // �����ζ��㴦����Ϣ��ֵ��
	double iRef, // �����Ĳο�ֵ
	double iTol, // �ݲ�
	STLPNT3D ioPoint[3], // ��ֵ��
	int* ioIndex); // ��ֵ���������

BOOL mathIsVectorDuringTwoVectors(STLVECTOR iVector1, STLVECTOR iVector2, STLVECTOR iVector);

const double INVSQRT2 = 0.70710678118654752440;
const double delta_l = 50.;

using namespace std;

struct MTIPathOriginList{//����ǰ·���ɷֶ�,�����·�����ɷֶ�
	int TNum;					//�������
	int DNum;					//�������
	STLPNT3D *PTrail;			//�ؼ���			POLYLINE
	STLPNT3D *PNTrail;			//�ؼ��㷨��
	FList *FTrail;				//�������Ƭ
	int *SNum;					//�ؼ�����Ŀ��
	int *ENum;					//�ؼ�����Ŀĩ
	POList PONext;

	void Initialize();
	void Modify(vector<STLPNT3D> iPTrails, vector<STLVECTOR> iPNTrails, vector<FList> iFTrails, POList iPONext = nullptr);
	POList Copy() ; // nt add 2022/7/10
	POList DirectOffset(double d) ; // nt add 2022/7/10

	// smf add 2022/7/27
	// �����Թ��ӹ���ʱ�Ĳ�صȾ�(���ֶ�)
	POList GeodesicOffsetNonFlexible(
		double iDistance, // iDistance��Ϊ��ֵ�����Ŵ���Ⱦ෽��
		GridModel* iModel,	// ����켣������ģ��, ���Ի�ȡ��ʸ
		double* oChordalHeight, // ÿһ�㴦�Ĺ���
		int iMaxChordalHeight); // �������ֵ

	// smf add 2022/9/25
	// ���Թ��ӹ���ʱ�Ĳ�صȾ�(���ֶ�)
	POList GeodesicOffsetFlexible(
		double iDistance, // iDistance��Ϊ��ֵ�����Ŵ���Ⱦ෽��
		GridModel* iModel	// ����켣������ģ��, ���Ի�ȡ��ʸ
		/*double* oChordalHeight,*/  // ÿһ�㴦�Ĺ���
		/*int iMaxChordalHeight*/);  // �������ֵ

	POList GeodesicOffsetFlexibleNew(
		double iDistance, // iDistance��Ϊ��ֵ�����Ŵ���Ⱦ෽��
		GridModel* iModel);	// ����켣������ģ��, ���Ի�ȡ��ʸ

	POList GeodesicOffsetPreprocessing(/*STLVECTOR iOffsetDir, */GridModel* iModel);

	void Draw() ;
	double Snap(GridModel* pGM, FList fs[2], double ps[2][3], double tol, int& I, double& t, int& perp) ;
	BOOL FindNextPoint(FRelated& ioFace, int& ioFaceNum, STLPNT3D& ioPointOnPlane, STLVECTOR iNormalOfPlane, STLVECTOR iLastDir);
	BOOL FindNextTri(STLPNT3D iBegin, STLPNT3D iEnd, FList &ioNextTri);
	BOOL FindNextTri2(STLPNT3D iBegin, STLPNT3D iEnd, FList &ioNextTri);
	static BOOL IsPointAVertex(STLPNT3D iPoint, FaceList* iFace, int &oIndex);
	static BOOL IsPointOnEdge(STLPNT3D iPoint, EList iEdge);
	// smf add 2022/12/12
	static BOOL IsPointInTriangle(STLPNT3D iPoint, FList iTri);
	static bool IsSegmInTriangle(STLPNT3D iBegin, STLPNT3D iEnd, FList iTri);

	// ����Ƿ����Խ�
	void PolylineCheck();
	BOOL DeleteOnePTrail(int iIndex);

	// ��ĩ������
	void PolylineExtrapolate(GridModel *iModel);
	void Extrapolate(STLPNT3D iPoint, STLVECTOR iDirection, FList iSupport, STLPNT3D &oPoint, FList &oFNext); // iDirectionӦΪ��λ����

	// smf add 2022/11/03
	int AddOnePTrail(STLPNT3D &iPTrail, STLVECTOR &iPNTrail, FList iFTrail, int iIndex); // �Ⱦ������һ���ؼ���
	void AddFirstPTrail(STLPNT3D &iPTrail, STLVECTOR &iPNTrail, FList iFTrail);
	void PushOnePTrail(STLPNT3D &iPTrail, STLVECTOR &iPNTrail, FList iFTrail); // �Ⱦ���������һ���ؼ���
	void CalOffsetDir(int iIndex, int iDir, int iNum, STLVECTOR &oOffsetDir, STLVECTOR &oTanDir); // ����Ⱦ෽��
	void GetRelatedFaceOfPTrail(int iIndex, FRelated oRelatedF, int &oRFNum);

	// �㵽����ߵĲ�ؾ���
	double CalGeodesicDistancePointToPl(GridModel *iModel, FList iFace, STLPNT3D iPoint, int iDir);
	void Destroy();

	void UpdateNormalAndFace(GridModel *iModel, double iTol = TOLLENGTH);
};

struct FaceRelated abstract {
	FList     Face;
	FRelated  FNext;
};

typedef struct Pog POG ;
struct Pog : public CObj // point on grid
{
	int type ;
	void* vef ;
	double p[3] ;

	Pog(int Type, void* Vef, double P[3]) ;
	~Pog() ;
} ;

typedef struct Pl PL ;
struct Pl
{
	COA pogs ;
	double arcLen ;

	Pl() ;
	~Pl() ;

	int CalArcLen() ;
	int GetNumPOGType(int type);	// add by jh, get number of type POG
	int Opt(double tol, double eps) ;
	int OptLM(double tol, int iMax) ;	// add by jh
	void ReplaceBegin(double p[3]) ;
	void ReplaceEnd(double p[3]) ;
} ;

typedef struct Pp PP ; // point pair
struct Pp
{
	FList fs[2] ;
	double ps[2][3] ;
	int I ; // λ������, I<1��ʾ���point pair����Ч�ģ�������
	double t ; // [0., 1.)
	double d ; // geodesic distance
	int perp ; // nt add 2022/7/4 1=��ֱ, 0=����ֱ
} ;

typedef struct Gdinfo GDINFO ; // ��ؾ�����Ϣ
struct Gdinfo
{
	int ns[2] ; // ns[0]Ϊpolyline1�ڵ������ns[1]Ϊpolyline2�ڵ����
	// pps[0][0],pps[1][0]δ��
	PP* pps[2] ; // pps[0]��Ԫ��ʵ�ʸ���Ϊns[0]+1, pps[1]��Ԫ��ʵ�ʸ���Ϊns[1]+1
	double min ;
	double max ;
	int i ; // i>0, ��С������Ϊpps[0][i], ����Ϊpps[1][|i|]
	int I ; // I>0, ��������Ϊpps[0][I], ����Ϊpps[1][|I|]

	Gdinfo() ;
	~Gdinfo() ;
} ;

struct GridModel {
	VList   VRoot;		// ����������ڵ�
	EList   EHead;		// ��������ͷ
	FList   FHead;		// ��������ͷ
	int  VerNum;		// ����
	int  EdgeNum;		// ����
	int  FaceNum;		// ����
	POList	*POLHead;	// �����ض�ǰ·��//���ɷֶ�
	double stl_xmin ;	// ��Χ�з�Χ, �ɶ�������
	double stl_ymin ;
	double stl_zmin ;
	double stl_xmax ;
	double stl_ymax ;
	double stl_zmax ;
	int TraverseNum ;
	int MeshResulttag ;
	char MeshResult[10][256] ;
	char PointName[100], ModelName[100];
	STLPNT3D PBreak;
	char PreDataFileName[256]; //�ݿ�,�����е�Path/Geometry/Trajectory
	double isag,istep,iangle,Plyangle,dval,gapValue;
	int TrailNumDum,FiberNum;
	int stlFaceTag;

	GridModel() ;
	BOOL stlCreateSTLTriangle(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3);
	BOOL stlVectorNormal(STLVECTOR *v1);

	// ���˹�ϵ����
	FList stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3,int tNum);
	FList stlCreateFace(STLVECTOR *nv, STLPNT3D *p1, STLPNT3D *p2, STLPNT3D *p3, STLVECTOR *v1, STLVECTOR *v2, STLVECTOR *v3, int tNum);
	FList stlFaceAlloc();
	VList stlCreateVertex(VList *root, STLPNT3D *pt);
	VList stlCreateVertex(VList *root, STLPNT3D *pt, STLVECTOR *vt);
	VList stlSortTree(STLPNT3D *pt, VList *t);
	VList stlSortTree(STLPNT3D *pt, STLVECTOR *vt, VList *t);
	VList stlVertexAlloc();
	EList stlCreateEdge(VList p1, VList p2);
	EList stlEdgeAlloc();
	void stlAddFace2Vertex(FList f, VList v);
	void stlAddFace2Edge(FList f, EList e);

	// 2016.09Add
	BOOL stlCompareTwo3DPoint(STLPNT3D p3d1, STLPNT3D p3d2, double epsdis);	// �������ƶȱȽ�
	int stlCompareTriangleVertex(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3, STLPNT3D q1, STLPNT3D q2, STLPNT3D q3); // ����Ƭ���, �����غϵ���

	STLPNT3D stlTwoVectorPlus(STLPNT3D p1,STLPNT3D p2);		// ��������� p1+p2
	STLPNT3D stlTwoVectorMinus(STLPNT3D p1,STLPNT3D p2);	// ��������� p1-p2
	STLPNT3D stlTwoVectorMultiply(STLPNT3D p1,double l1);	// �����ӳ� p1*l1
	STLPNT3D stlTwoVectorDivide(STLPNT3D p1,double l1);		// �������� p1/l1
	
	// add by wjq, ���������ķ�ʸ
	double triCalArea(STLPNT3D p1, STLPNT3D p2, STLPNT3D p3);
	double CalDistofPtAndFace(STLPNT3D p0, FList f);
	FList FindClosetFace(STLPNT3D p0, double tol);
	int Is_in_triangle(STLPNT3D p0, FList f, double tol);
	STLVECTOR CalAllVertexNormalVector(STLPNT3D p1, double tol);	// add by wjq, ���㷨ʸ

	//20170825 ADD DAT�ļ�����
	void stlDealInputFile();
	void stlReadFile(char *file);
	void stlReadSTLBinFile(char *file);
	void stlReadStlFile(char *file);
	void stlRead4Bytes(FILE *stlfile, char *c);
	void stlReadDatFile(char *file);
	
	void    GetInformationFromPath();	// ���̷�·����ȡ��Ϣ, ��Ҫ���TXT�ļ�
	POList	stlPOLHeadAlloc();			// �ض�ǰ·��
    FList stlGetFacetFromNum(int N);	// ��ȡ���Ϊ N ����Ƭ
	//void GetNormaVectorOnVertex(VList p) ;
	double stlDistanceTwoPoints(STLPNT3D p1, STLPNT3D p2) ;	// ��������䳤
	double  stlDistanceOneVector(STLPNT3D p1) ;				// ����������
	
	STLPNT3D stlDistanceVectorTwoPoint(STLPNT3D p1, STLPNT3D p2);	// �������������(��λ��), ����������, �򷵻� (0.0,0.0,0.0)
	STLPNT3D stlTwoVectorProduct(STLPNT3D v1, STLPNT3D v2);			// ����������
	STLPNT3D stlOPPNormalVector(STLPNT3D v) ;						// ���ص� v ����Ӧ���෴ʸ��
	STLPNT3D CreateMTIPoint();										// ����Ĭ�ϵ� (0.0,0.0,0.0)
	STLPNT3D CreateMTIPoint(double ix,double iy,double iz);			// �� xyz ����
	STLPNT3D CreateMTIPoint(double p1[]);							// �����鴴��
	STLPNT3D CreateMTIPoint(double p1[],int iNbStart);				// �������ĳλ�ô�����
	double GetDistanceVertexToCoreOfTrangle(STLPNT3D p, FList face) ;

	void InitFlagAll() ;											// �� EHead ����, ������ Vertex �� flag ֵΪ 0
	int CalGeodesicLineOne(FList f, double p1[3], double p2[3], PL** polyline) ;	// 
	int CalGeodesicLineAdj(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline) ;
	// jh modify
	// ���������Ĳ����, �����Ż�����
	// opt = 0 no opt, opt = 1 normal opt, opt = 2 LM opt, opt = 3 normal&LM opt
	int CalGeodesicLine(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline, int opt) ;
	// added by jh, 2022/09/30
	// ���������Ĳ����, ���и�ƽ��ķ�������һ���Ż�
	int CalGeodesicLineOpt(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline, int opt);
	// added by jh
	// ���������Ĳ����, ĩ�˵��� p21 �� p22 ֮���ζ�
	int CalGeodesicLineFloatTail(FList f1, double p1[3], FList f2, double p2[3], double p21[3], double p22[3], double tol, PL** polyline, int opt);

	// added by jh, 2022/11/24
	// ���������Ĳ����, �����Ż�����, ����˫�� dir ���㼰 dir �ĵ�������
	int CalGeodesicLineNew(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline, int opt);
	// jh add 2022/09/30
	// �� CalGeodesicLineOpt ����
	int CalGeodesicLineOptTheta(FList f1, double p1[3], FList f2, double p2[3], double theta, double tol, PL** polyline);
	FList Prj(FList f1, double p1[3], FList f2, double p2[3], double tol, double ptOnLine[3], double ptOnFace[3]) ; // nt add 2022/7/8	
	int CalGDInfo(POList polys[2], double tol, GDINFO** pGDI) ;
	FList Pick(PNT3D pos, VEC3D dir, double r, PNT3D p, double* pz, PNT3D gc) ;
	void Draw(void* pVI, int drawMode) ;
	// jh add 2022/11/24
	// ���������Ĳ����, ��ƽ����ʸȷ�������淨ʸ��ת theta, ����˫�� dir ���㼰 dir �ĵ�������
	int CalGeodesicLineThetaNew(FList f1, double p1[3], FList f2, double p2[3], double theta, double tol, PL** polyline, int opt);
	static int calFuncNum;
	// jh add 2022/11/24
	// ���������Ĳ����, ���и�ƽ��ķ�������һ���Ż�
	int CalGeodesicLineOptNew(FList f1, double p1[3], FList f2, double p2[3], double tol, PL** polyline, int opt);
	// added by jh, 2022/11/24
	// ���������Ĳ����, ĩ�˵��� p21 �� p22 ֮���ζ�
	int CalGeodesicLineFloatTailNew(FList f1, double p1[3], FList f2, double p2[3], double p21[3], double p22[3], double tol, PL** polyline, int opt);

	// smf add 2023/01/31
	static bool IsPointOnBoundary(STLPNT3D iPoint, FList iSupport);

	POList CreateMTIPathFromPL(PL* iPolyLine);

	int nPolyline ;
	Pl* polylines[100] ;
} ;


// 2022/10/10 smf add

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 20 /* �洢�ռ��ʼ������ */

typedef int Status;
typedef int SElemType; /* SElemType���͸���ʵ������������������Ϊint */


/* ��ջ�ṹ */
typedef struct StackNode
{
	SElemType data;
	struct StackNode* next;
}StackNode, * LinkStackPtr;


typedef struct
{
	LinkStackPtr top;
	int count;
}LinkStack;

#endif