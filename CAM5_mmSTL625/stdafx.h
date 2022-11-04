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

// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
////////////////////////////////////////////////////////////////
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif
#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
//--------------------------------------------------------------
#define CStr CStringA // nt add 2016/6/3
#include "stdio.h"
#include "malloc.h"
#include "math.h"
#include "gl\gl.h"
#include "API\VI_API.h"
#include "API\GP_API.h"
#include "API\GCT_API.h"
#include "Obj\Obj.h"
#include "Tools\Tools.h" // nt add 2009/5/9
#include "Tools\PV.h" // nt add 2009/5/9
#include "Tools\ReadWrite.h" // nt add 2010/4/22
#include "Tools\Math2D.h" // nt add 2020/4/11
#include "Option.h" // nt add 2010/11/28

//--------------------------------------------------------------
int camGetSysVerNo() ; // nt add 2003/6/23
int camWake(void* pVI) ; // nt add 2009/9/12
int camSleep(void* pVI) ; // nt add 2009/9/12
//--------------------------------------------------------------
void camPromptUpdate() ; // nt add 2009/10/21
void camPromptAttach(char c) ; // nt add 2009/10/21
void camPromptAttach(CStr& str) ; // nt add 2009/10/21
void camPromptGet(CStr& str) ; // nt add 2009/10/21
void camPromptClear() ; // nt add 2009/10/21
void camPromptStr(CStr& str) ; // in MainFrm.cpp
void camPromptStr(char* text) ; // in MainFrm.cpp
void camPromptPt(double p[2]) ; // in MainFrm.cpp
////////////////////////////////////////////////////////////////












