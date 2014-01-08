// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1CEDC015_97CC_47B2_BF99_2285C9E40854__INCLUDED_)
#define AFX_STDAFX_H__1CEDC015_97CC_47B2_BF99_2285C9E40854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4101)
#pragma warning(disable:4503)
#pragma warning(disable:4786)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT



/************************************************************************/
// 12306 include
#include <json/json.h>
#include "_12306/typedef.h"
#include "_12306/qmap.h"
#include "_12306/petcmd.h"
#include "_12306/function.h"
#include "_12306/LoginRequest.h"
#include "_12306/api/HandleCode.h"
#include "_12306/api/memory_conf.h"
#include "_12306/function.h"
#include "_12306/unicode_utf-8.h"

#pragma comment(lib, "ws2_32.lib")

extern CLoginRequest* creq;

extern string g_strapppath; // 应用程序路径
extern string strRCodePath; // 验证码
extern string strsignfile;	// 证书
extern string strerrorfile;
extern string strinifile;
extern string stralterapp;
extern const string strfixuserinfo; // 使用者
/************************************************************************/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1CEDC015_97CC_47B2_BF99_2285C9E40854__INCLUDED_)
