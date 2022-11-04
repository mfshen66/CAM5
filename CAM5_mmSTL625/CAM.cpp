// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// CAM.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CAM.h"
#include "Models.h"
#include "Mills.h" // nt add 2010/4/1
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CAMDoc.h"
#include "CAMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////
// CCAMApp
BEGIN_MESSAGE_MAP(CCAMApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CCAMApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()
//--------------------------------------------------------------
// CCAMApp 构造
CCAMApp::CCAMApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CAM.AppID.Version1.0"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pVI = NULL ; // nt add 2009/12/17
	m_pTLib = NULL ; // nt add 2010/9/24

	// nt add 2010/11/28
	optionInit(&m_option) ;
}

// nt add 2009/12/22
CCAMApp::~CCAMApp()
{
	// nt add 2010/11/28 : save options
	CStr pathName =	cfFillFullPathName("Options.Dat") ;
	optionSave(&m_option, pathName) ;
	// end
	rgbsFree() ;
	// nt add 2010/9/24
	if( m_pTLib )
	{
		CTLib* pTLib = (CTLib*)m_pTLib ;
		if( pTLib->IsModified() )
			pTLib->Save() ;
		delete pTLib ; // free tool lib
		m_pTLib = NULL ;
	}
}
//--------------------------------------------------------------
// 唯一的一个 CCAMApp 对象
CCAMApp theApp;

// CCAMApp 初始化

BOOL CCAMApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("BUAA数控加工编程应用程序V2010"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_CAMTYPE,
		RUNTIME_CLASS(CCAMDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CCAMView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	m_nCmdShow = SW_SHOWMAXIMIZED ; // nt add 2009/12/18
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// register draw functions which are used in GP.dll, nt add 2001/11/24
	if( regDrawPt(viDrawPoint) != ERSUCSS ||
		regDrawLin(viDrawLine) != ERSUCSS ||
		regDrawPoly(viDrawPolyline) != ERSUCSS ||
		regDrawTris(viDrawTris) != ERSUCSS ||
		regDrawQuas(viDrawQuas) != ERSUCSS )
		return FALSE ;

	// nt add 2009/12/22 : 参考
	CStr pathName = cfFillFullPathName("ACI.txt") ;
	rgbsCreate(pathName.GetBuffer(0)) ;

	// tool lib, nt add 2010/9/24
	pathName = cfFillFullPathName("TLib.txt") ;
	CTLib* pTLib = new CTLib() ;
	if( pTLib->Load(pathName) != 1 )
	{
		pTLib->CreateBasic() ;
		pTLib->SetModified() ;
	}
	m_pTLib = pTLib ;

	// nt add 2010/11/28 : options
	pathName = cfFillFullPathName("Options.Dat") ;
	optionLoad(&m_option, pathName) ;

	// nt add 2010/5/24, 单位默认为mm
	gpSetTol(1.0e-5) ;
	gpSetAng(1.0e-11) ;
	double pre[3] ;
	pre[0] = 1. ;
	pre[1] = 0.2 ;
	pre[2] = 10. ;
	gpSetPre(pre) ;
	double dsc[3] ;
	dsc[0] = 2.5 ;
	dsc[1] = 0.2 ;
	dsc[2] = 10. ;
	gpSetDsc(dsc) ;
	//gpSetFit(0.001) ;
	double tes[3] ;
	tes[0] = 1. ;
	tes[1] = 1.047197551196597 ;
	tes[2] = 0.001 ;
	gpSetTes(tes) ;
	// nt add 2021/7/26
	gpSetFit(0.01) ;

	return TRUE;
}

int CCAMApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}
//--------------------------------------------------------------
// CCAMApp 消息处理程序
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CCAMApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
//--------------------------------------------------------------
// CCAMApp 自定义加载/保存方法

void CCAMApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CCAMApp::LoadCustomState()
{
}

void CCAMApp::SaveCustomState()
{
}
//--------------------------------------------------------------
void CCAMApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
	TCHAR szFilter[] = _T("Part Files (*.Part)|*.part||") ;
	CFileDialog dlg(TRUE, _T("*.Part"), NULL, OFN_HIDEREADONLY, szFilter) ;
	if( dlg.DoModal() == IDOK )
	{
		CString ext = dlg.GetFileExt() ;
		CString pathName = dlg.GetPathName() ;
		if( ext == _T("Part") ||
			ext == _T("PART") ||
			ext == _T("part") )
			OpenDocumentFile(pathName) ;
	}
}
//--------------------------------------------------------------
int camGetSysVerNo()
{
	//return 5 ; // nt modify 2010/9/9
	return 6 ; // nt modify 2021/8/13
}

static int g_docVerNo ;

void camSetDocVerNo(int verNo)
{
	g_docVerNo = verNo ;
}

// 必须先调用camSetDocVerNo()进行设置
int camGetDocVerNo()
{
	return g_docVerNo ;
}

// 在不CView之间协调opengl资源
// nt add 2009/9/13
// nt rewrite 2011/7/7
int camWake(void* pVI)
{
	if( pVI )
	{
		if( theApp.m_pVI &&
			theApp.m_pVI != pVI ) // 需要切换当前激活的VI
			viSleep(theApp.m_pVI) ;
		HGLRC hGLRC = wglGetCurrentContext() ;
		if( hGLRC == NULL )
			viWake(pVI) ;
		theApp.m_pVI = pVI ;

		return 1 ;
	}
	else
		return 0 ;
}

// nt add 2009/9/13
int camSleep(void* pVI)
{
	if( pVI )
	{
		if( theApp.m_pVI == pVI ) // 当前激活的VI就是输入pVI
		{
			viSleep(theApp.m_pVI) ;
			theApp.m_pVI = NULL ;
		}
	}
	else
	{
		if( theApp.m_pVI ) // 当前激活的VI就是pVI
		{
			viSleep(theApp.m_pVI) ;
			theApp.m_pVI = NULL ;
		}	
	}

	return 1 ;
}
//--------------------------------------------------------------
// nt add 2010/8/22, common global functions
int cmdDlgUI(int ui)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd() ;
	CMDIChildWnd* pChildFrame = pMainFrame->MDIGetActive() ;
	CCAMDoc* pDoc = (CCAMDoc*)(pChildFrame->GetActiveDocument()) ;
	if( pDoc )
	{
		pDoc->CmdDlgUI(ui) ;
		return 1 ;
	}
	else
		return 0 ;
}
//--------------------------------------------------------------
void* GetTLib()
{
	return theApp.m_pTLib ;
}
//--------------------------------------------------------------
// nt add 2010/11/28
OPTION* camGetOption()
{
	return &(theApp.m_option) ;
}

void camSetOption(OPTION* option)
{
	if( option )
		theApp.m_option = *option ;

	return ;
}

// nt add 2020/5/12
void* camGetMTView()
{
	CMainFrame* pWndMain = (CMainFrame*)AfxGetMainWnd();
	if( pWndMain == NULL )
		return NULL ;
	return &(pWndMain->m_wndMT) ;
}
////////////////////////////////////////////////////////////////

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAboutDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}

void CAboutDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

CDocument* CCAMApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: 在此添加专用代码和/或调用基类
	CFileFind finder ; // nt add 2011/5/24, 避免因找不到这个文件而导致的宕机
	BOOL bWorking = finder.FindFile(lpszFileName) ;
	if( bWorking == FALSE )
		return NULL ;

	return CWinAppEx::OpenDocumentFile(lpszFileName);
}
//==============================================================
