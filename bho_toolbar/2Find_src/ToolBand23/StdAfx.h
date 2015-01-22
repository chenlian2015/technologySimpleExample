// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__7A0235CE_3B39_431C_A7BE_DB3F244EB597__INCLUDED_)
#define AFX_STDAFX_H__7A0235CE_3B39_431C_A7BE_DB3F244EB597__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#define _CRTDBG_MAP_ALLOC 

#define WINVER 0x0500
#define _WIN32_IE 0x0501

#pragma warning(disable: 4251) //'' : class '' needs to have dll-interface to be used by clients of class ''
#pragma warning(disable: 4275)  //non dll-interface class
#pragma warning(disable: 4786) //identifier was truncated to '255' characters in the debug information

#include <atlbase.h>
#include <comdef.h>

#include <winuser.h>

#import "msxml.dll"  named_guids

#define _TOOLBAR_SAMPLE  1
#define BLUE_ON_OVER_TEXT 1
#define DONT_CHECK_VERSION_FILE 1
#define SELECT_SEARCH_FROM_MENU 1
#define DONT_CHECK_LOCAL_FILES 1
#define USE_TICKER 1
#define APPLY_BACKGROUND 1
#define ADD_CONTEXT_ITEM 1


//#define USE_RESOURCE_FILES 1


extern CComModule _Module;
#include <atlcom.h>

#include <atlapp.h>
#include <atlwin.h>

#include <atlctrls.h>
#include <AtlCtrlx.h>
#include <atlmisc.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7A0235CE_3B39_431C_A7BE_DB3F244EB597__INCLUDED)
