// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__06928108_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
#define AFX_STDAFX_H__06928108_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable: 4100)	//	unreferenced formal parameter
#pragma warning(disable: 4183)	//	missing return type; assumed to be a member function returning 'int'
#pragma warning(disable: 4189)	//	local variable is initialized but not referenced
#pragma warning(disable: 4996)	//	'blah blah blah' was declared deprecated
#pragma warning(disable: 4706)	//	assignment within conditional expression
#pragma warning(disable: 4238)	//	nonstandard extension used : class rvalue used as lvalue

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC OLE automation classes
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__06928108_78EA_11D1_A147_0080AD1A7DB4__INCLUDED_)
