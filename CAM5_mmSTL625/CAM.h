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

// CAM.h : CAM 应用程序的主头文件
//
#pragma once

////////////////////////////////////////////////////////////////
// nt 关闭了下面三行代码，如果打开可能导致VC2010死机
//#ifndef __AFXWIN_H__
//	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
//#endif
#include "resource.h"       // 主符号
// CCAMApp:
// 有关此类的实现，请参阅 CAM.cpp
//
class CCAMApp : public CWinAppEx
{
public:
	void* m_pVI ; // nt add 2009/12/17
	void* m_pTLib ; // nt add 2010/9/24

	// system option, nt add 2010/11/28
	OPTION m_option ;

public:
	CCAMApp();
	~CCAMApp(); // nt add 2009/12/22

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern CCAMApp theApp;
//--------------------------------------------------------------
// nt add 2009/12/16
int camGetSysVerNo() ;
void camSetDocVerNo(int verNo) ; // nt add 2021/8/13
int camGetDocVerNo() ; // nt add 2021/8/13
int camWake(void* pVI) ;
int camSleep(void* pVI) ;
OPTION* camGetOption() ; // nt add 2010/11/28
void camSetOption(OPTION* option) ; // nt add 2021/6/16
void* camGetMTView() ; // nt add 2020/5/12
//--------------------------------------------------------------
// nt add 2010/8/22 common global functions
int cmdDlgUI(int ui) ;
//--------------------------------------------------------------
void* GetTLib() ; // nt add 2010/9/24
////////////////////////////////////////////////////////////////