/*	1999/7/2 nt
 *	the haed file of class CObj
 */
#ifndef _Obj_h_
#define _Obj_h_

#ifdef __cplusplus
////////////////////////////////////////////////////////////////
//	error
int stGetError() ;
void stSetError(int error) ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//	1999/7/2 nt
//	Abstract Class of CObj and it's COL and COA
/*	1999/7/2 nt
 *	class of abstract object
 */
class CObj
{
private:
public:
	CObj() ;
	virtual ~CObj() ;
} ;

/*	1999/7/2 nt
 *	class of object list item
 */
class CLI
{
public:
	CLI* m_prev ;
	CLI* m_next ;
	CObj* m_pObj ;
public:
	CLI(CObj* pObj) ;
	~CLI() ;

	CLI* GetPrev() ;
	void SetPrev(CLI* prev) ;
	CLI* GetNext() ;
	void SetNext(CLI* next) ;
	CObj* GetObj() ;
	void SetObj(CObj* pObj) ;
} ;

// pointer of CLI
#define POSI void*

/*	1999/7/2 nt
 *	class of object list
 */
class COL
{
private:
	int	m_size ;
	CLI* m_head ;
	CLI* m_tail ;
public:
	COL() ;
	~COL() ;

	int	GetSize() ;
	int	IsEmpty() ;
	void RemoveAll() ;

	CObj* GetHead() ;
	POSI GetHeadPosi() ;
	POSI AddHead(CObj* pObj) ;
	CObj* RemoveHead() ;

	CObj* GetTail() ;
	POSI GetTailPosi() ;
	POSI AddTail(CObj* pObj) ;
	CObj* RemoveTail() ;

	CObj* GetNext(POSI& pos) ;
	CObj* GetPrev(POSI& pos) ;
	CObj* GetAt(POSI pos) ;
	void SetAt(POSI pos, CObj* pObj) ;
	void RemoveAt(POSI pos) ;

	POSI InsertBefore(POSI pos, CObj* pObj) ;
	POSI InsertAfter(POSI pos, CObj* pObj) ;

	POSI Find(CObj* pObj, POSI startAfter = NULL) ;
	POSI FindIndex(int index) ; // index is from 0 to m_size - 1
} ;

/*	1999/8/6 nt
 *	class of object array
 */
class COA
{
private:
	int	m_grow ; // growing size when COA reallocate m_objs
	int	m_nObj ; // the array size of m_objs, m_nObj >= 0
	CObj** m_objs ; // object array storing CObj*, may be NULL
	int	m_size ; // number of objects stored, m_size<=m_nObj
public:
	COA() ; // create
	COA(int nGrow, int nSize) ; // create and set grow and memory size
	~COA() ; // delete

	int	GetSize() ; // number of objects currently stored
	int	IsEmpty() ; // return 1: empty; return 0: not empty
	void SetSize(int nSize, int nGrow) ; // set memory size and grow size
	void RemoveAll() ; // remove all objects currently stored
	void FreeExtra() ; // free extra memory

	CObj* GetAt(int index) ; // index must from 0 to size-1
	void SetAt(int index, CObj* pObj) ; // index must from 0 to size-1
	int	Add(CObj* pObj) ;
	int	Append(COA* pOA) ; // append array, return first added element index
	void Copy(COA* pOA) ; // overwrite old elements
	void InsertAt(int index, CObj* pObj) ; // index must from 0 to size-1
	void InsertAt(int index, COA* pOA) ; // index must from 0 to size-1
	void RemoveAt(int index) ; // index must from 0 to size-1
	void RemoveAt2(int index) ; // index must from 0 to size-1, nt add 2010/4/18
	CObj* operator[](int index) ; // index must from 0 to size-1
} ;

/*	1999/8/13 nt
 *	class of int array
 */
class CIA
{
private:
	int	m_grow ; // growing size when CIA reallocate m_ints
	int	m_nInt ; // the array size of m_ints, m_nInt >= 0
	int* m_ints ; // int array storing int, may be NULL
	int	m_size ; // number of ints stored, m_size<=m_nInt
public:
	CIA() ; // create
	CIA(int nGrow, int nSize) ; // create and set grow and memory size
	~CIA() ; // delete

	int	GetSize() ; // number of objects currently stored
	int	IsEmpty() ; // return 1: empty; return 0: not empty
	void SetSize(int nSize, int nGrow) ; // set memory size and grow size
	void RemoveAll() ; // remove all objects currently stored
	void FreeExtra() ; // free extra memory

	int	GetAt(int index) ; // index must from 0 to size-1
	void SetAt(int index, int num) ; // index must from 0 to size-1
	int	Add(int num) ;
	int	Append(CIA* pIA) ; // append array, return first added element index
	void Copy(CIA* pIA) ; // overwrite old elements
	void InsertAt(int index, int num) ; // index must from 0 to size-1
	void InsertAt(int index, CIA* pIA) ; // index must from 0 to size-1
	void RemoveAt(int index) ; // index must from 0 to size-1
	int	operator[](int index) ; // index must from 0 to size-1
} ;

/*	1999/8/23 nt
 *	class of double array
 */
class CDA
{
private:
	int	m_grow ; // growing size when CIA reallocate m_ints
	int	m_nDbl ; // the array size of m_dbls, m_nDbl >= 0
	double* m_dbls ; // double array storing int, may be NULL
	int	m_size ; // number of ints stored, m_size<=m_nInt
public:
	CDA() ; // create
	CDA(int nGrow, int nSize) ; // create and set grow and memory size
	~CDA() ; // delete

	int	GetSize() ; // number of objects currently stored
	int	IsEmpty() ; // return 1: empty; return 0: not empty
	void SetSize(int nSize, int nGrow) ; // set memory size and grow size
	void RemoveAll() ; // remove all objects currently stored
	void FreeExtra() ; // free extra memory

	double GetAt(int index) ; // index must from 0 to size-1
	void SetAt(int index, double dbl) ; // index must from 0 to size-1
	int	Add(double dbl) ;
	int	Append(CDA* pDA) ; // append array, return first added element index
	void Copy(CDA* pDA) ; // overwrite old elements
	void InsertAt(int index, double dbl) ; // index must from 0 to size-1
	void InsertAt(int index, CDA* pDA) ; // index must from 0 to size-1
	void RemoveAt(int index) ; // index must from 0 to size-1
	double operator[](int index) ; // index must from 0 to size-1
} ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//	CAr
/*	2000/7/15 nt
 *	CAr
 */
class CAr
{
private:
	int m_mode; // 0 : for read ; 1 : for write
	TCHAR m_name[512]; // full path and name
	FILE* m_fp; // file pointer
public:
	CAr(); // nt add 2010/5/3
	CAr(int mode, TCHAR* name);
	~CAr();

	int Open(int mode, TCHAR* name); // nt add 2010/5/3
	int Close(); // nt add 2010/5/3
	int Seek(long offset, int where); // see fseek(), nt add 2010/5/3
	int	IsStoring(); // 0 : for read ; 1 : for write
	TCHAR* GetName();
	FILE* GetFP();

	CAr& operator >> (int& i);
	CAr& operator >> (float& f);
	CAr& operator >> (double& d);
	int	Read(void* buffer, int size);

	CAr& operator << (int i);
	CAr& operator << (float f);
	CAr& operator << (double d);
	int	Write(void* buffer, int size);
};
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//	void pointer carrier
class CCarrier : public CObj
{
private:
	void* m_vp ;
public:
	CCarrier() ;
	~CCarrier() ;

	void* GetVP() ;
	void SetVP(void* vp) ;
} ;
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Visual Object, nt add 2010/4/18
#define VO_PICKED 0x1 // picked
//#define VO_ 0x2
//#define VO_ 0x4
//#define VO_ 0x8
//#define VO_ 0x10
//#define VO_ 0x20
//#define VO_ 0x40
//#define VO_ 0x80
//#define VO_ 0x100
//#define VO_ 0x200
//#define VO_ 0x400
//#define VO_ 0x800
//#define VO_ 0x1000
//#define VO_ 0x2000
//#define VO_ 0x4000
//#define VO_ 0x8000
//#define VO_ 0x10000
//#define VO_ 0x20000
//#define VO_ 0x40000
//#define VO_ 0x80000
//#define VO_ 0x100000

class CVO : public CObj
{
public:
	int m_flags ;
public:
	CVO() ;
	~CVO() ;

	int IsPicked() ; // return 1=picked, 0=not picked
	int SetPick(int pick) ;
	virtual int GetType() = 0 ;
	virtual int Draw(void* pVI, int drawMode) = 0 ;
	virtual int Draw2(void* pVI, int drawMode) = 0 ;
} ;
////////////////////////////////////////////////////////////////

#endif

#endif