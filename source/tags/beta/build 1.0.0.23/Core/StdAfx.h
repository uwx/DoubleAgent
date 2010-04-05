#if !defined(AFX_STDAFX_H__D54ADD78_2521_43BF_BAE2_E086CD4B2845__INCLUDED_)
#define AFX_STDAFX_H__D54ADD78_2521_43BF_BAE2_E086CD4B2845__INCLUDED_
#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE
#define	_AFX_NO_CTL3D_SUPPORT
#pragma warning (disable : 4786 4812 4068 4312 4407 4995)

#if	(_MSC_VER >= 1400)
#ifndef	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_SECURE_NO_DEPRECATE
#endif
#define	_CRT_NON_CONFORMING_SWPRINTFS
#ifndef	_WIN64
#define _USE_32BIT_TIME_T
#endif
#ifdef	_WIN32_WINNT
#undef	_WIN32_WINNT
#endif
#endif

#ifndef	_UNICODE
#define	_UNICODE
#endif
#ifndef	UNICODE
#define	UNICODE
#endif

#ifdef	_VISTA
#ifndef	_WIN32_WINNT
#define	_WIN32_WINNT 0x0700
#endif
#ifndef	WINVER
#define	WINVER 0x0600
#endif
#ifndef	_WIN32_IE
#define	_WIN32_IE 0x0700
#endif
#else
#ifndef	_WIN32_WINNT
#define	_WIN32_WINNT 0x0501
#endif
#endif
#ifndef	WINVER
#define	WINVER 0x0501
#endif
#ifndef	_WIN32_IE
#define	_WIN32_IE 0x0600
#endif

#include <windowsx.h>
#include <intsafe.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxctl.h>         // MFC support for ActiveX Controls
#include <afxdisp.h>        // MFC Automation classes
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxmt.h>			// MFC multasking
#include <afxtemplfixed.h>	// MFC templates

#include <comdef.h>
#include <math.h>
#include "Log.h"
#include "OsVer.h"
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
#include "AfxTemplEx.h"
#include "DaGuid.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D54ADD78_2521_43BF_BAE2_E086CD4B2845__INCLUDED_)
