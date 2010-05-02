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

#pragma managed(push,off)
#include "TargetVer.h"
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
#include "DaGuid.h"
#pragma managed(pop)

#ifdef	ReportEvent
#undef	ReportEvent
#endif
#ifdef	GetTempPath
#undef	GetTempPath
#endif
#ifdef	SearchPath
#undef	SearchPath
#endif

/////////////////////////////////////////////////////////////////////////////
