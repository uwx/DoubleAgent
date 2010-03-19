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

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define	_ATL_OLEDB_CONFORMANCE_TESTS

#include <afxwin.h>
#include <afxdisp.h>
#include <afxcoll.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
using namespace ATL;

#include <comdef.h>
#include "Log.h"
#include "OsVer.h"
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
