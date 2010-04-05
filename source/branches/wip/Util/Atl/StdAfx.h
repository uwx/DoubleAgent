#pragma once

#pragma warning (disable : 4005 4786 4812 4068 4312 4407 4995)
#ifndef STRICT
#define STRICT
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef OEMRESOURCE
#define OEMRESOURCE
#endif
#ifndef	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_SECURE_NO_DEPRECATE
#endif
#ifndef	_CRT_NON_CONFORMING_SWPRINTFS
#define	_CRT_NON_CONFORMING_SWPRINTFS
#endif
#ifndef	_WIN64
#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif
#endif

#include "TargetVer.h"
#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define	_ATL_OLEDB_CONFORMANCE_TESTS

#include <atlbase.h>
#include <atltypes.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atlsync.h>
using namespace ATL;

#include <windowsx.h>
#ifdef	SubclassWindow
#undef	SubclassWindow
#endif
#include <comdef.h>
#include <math.h>
#include <shlwapi.h>
#include <shlobj.h>
#include "Log.h"
#include "OsVer.h"
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
#include "HandleTemplates.h"
#include "AtlUtil.h"
#include "AtlCollEx.h"