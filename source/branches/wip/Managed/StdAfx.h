#pragma once
#pragma warning (disable : 4068)
#ifdef	_DEBUG
#pragma warning (disable : 4945)
#endif

#ifndef STRICT
#define STRICT
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_SECURE_NO_DEPRECATE
#endif
#ifndef	_CRT_NON_CONFORMING_SWPRINTFS
#define	_CRT_NON_CONFORMING_SWPRINTFS
#endif

#include "TargetVer.h"

#if	defined (_M_CEE_PURE) || defined (_M_CEE_SAFE)
#include <tchar.h>
#include "Log.h"
#include "ExceptionMacros.h"
#else	// _M_CEE_XXXX
#pragma managed(push,off)
#include <tchar.h>
#include <comdef.h>
#include <windows.h>
#include <oleauto.h>
#pragma managed(pop)

#include <vcclr.h>
#include <msclr\marshal.h>
#include <msclr\marshal_windows.h>
using namespace msclr::interop;

#pragma managed(push,off)
#include "Log.h"
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
#pragma managed(pop)
#endif	// _M_CEE_XXXX

#ifdef	ReportEvent
#undef	ReportEvent
#endif
#ifdef	GetTempPath
#undef	GetTempPath
#endif
#ifdef	SearchPath
#undef	SearchPath
#endif

#include "DaVersion.h"

/////////////////////////////////////////////////////////////////////////////
