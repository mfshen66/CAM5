/*	1999/8/20 nt
 *	head file of class CModel definition
 */
#ifndef _Model_h_
#define _Model_h_

#ifdef __cplusplus
///////////////////////////////////////////////////////////////
//	Model
/*	1999/8/18 nt
 *	EModelType
 */
enum EModelType
{
	mdPart = 0, // Part
	mdAssembly = 1, // Assembly
	mdDrawing = 2, // Drawing
} ;

/*	1999/8/20 nt
 *	EModelState
 */
enum EModelState // mutually exclusive
{
	msNormal, // normal, usually waiting command, 123
	msEdit, // edit feature, part mating or view, 123
	msUpdate, // updating, 123
	msSection, // viewing by an cross section, 123
	msExplode, // explode, 2
	msCheck, // display interference, 2
	msCommand, // tanslate or rotate component, 123
	msPaper, // in paper space, 3
	msView // in view space, 3
} ;

/*	1999/8/20 nt
 *	CModel
 */
class CModel : public CObj
{
protected:
	//CModelID	m_modelID ; // model ID
	//int		m_version ; // version No.
	//char		m_author[32] ; // name of author
	//int		m_priority ; // 优先级别
	//char		m_pathName[128] ; // path and name of this model
	//int		m_lastModifyTime ; // time elapsed since 1970/01/01/00/00, set it before save(in Serialize()), so it really meaning is the time when last save
	//int		m_itWhenLastSave ; // used to determine : need save or not, set it before save(in Serialize())
	//int		m_maxStep ; // max model operation step occured up to now, begin from 1
	BOX3D       m_box ; // model box

	int			m_state ; // model current状态,如正在编辑等, not save
	//int		m_waitUpdateFlag ; // no-zero:wait for update, not save
    //int       m_nRef ; // the current number of reference time, not save

	//COL		m_records ; // operation history, not save
public:
    CModel() ;
    virtual ~CModel() ;

	//CModelID*	GetModelID() ;
	//void		SetModelID(CModelID* pModelID) ;
	//int		GetVersion() ;
	//char*		GetAuthor() ;
	//void		SetAuthor(char author[32]) ;
	//char*		GetPathName() ;
    //void      SetPathName(char pathName[128]) ;
	//int		GetLastModifyTime() ;
	//void		SetLastModifyTime(int time) ;
	//int		NeedSave() ; // 1:need;0:not need
	//int		GetMaxStep() ; // get current max step
	//void		AddMaxStep() ; // m_step++
	int			GetBox(BOX3D* box) ;
	int			SetBox(BOX3D* box) ;
	//int		DrawBox(int drawMode) ;

	//int		GetModelState() ;
	//void		SetModelState(int state) ;
	//int		GetWaitUpdateFlag() ;
	//void		SetWaitUpdateFlag(int flag) ;
    //void      AddRef() ; // m_ref++
    //void      Release() ; // m_ref--

	//virtual int Rebuild() = 0 ; // call after Serialize() (loading),0,1:error,sucess

	//int		Undo() ; // 0:error;1:sucess
	//int		IsUndoable() ; // 1:can undo;0:can not undo

	//virtual int IsEmpty() = 0 ; // 1:empty,0:not empty
	//virtual int NeedUpdate() = 0 ; // 1:need update;0:not need
	//virtual int IsUndoable() = 0 ; // 1:can undo;0:can not undo
	virtual int Navigate(DNAVI* dn) = 0 ;
    virtual int CalBox() = 0 ; // 1:success;0:error
    virtual int Draw(void* pVI, int drawMode) = 0 ;
    virtual int Pick(PINFO* pi) = 0 ; // 1:picked;2:pick none;0:error
    virtual int Serialize(CArchive& ar) ; // // 1:success;0:error
} ;
///////////////////////////////////////////////////////////////
#endif

#endif