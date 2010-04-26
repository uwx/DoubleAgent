// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning (disable : 4068)

#ifndef	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_SECURE_NO_DEPRECATE
#endif
#ifndef	_CRT_NON_CONFORMING_SWPRINTFS
#define	_CRT_NON_CONFORMING_SWPRINTFS
#endif

#pragma managed(push,off)
#include <tchar.h>
#include <comdef.h>
#include <oleauto.h>
#pragma managed(pop)

#ifdef	ReportEvent
#undef	ReportEvent
#endif
#ifdef	GetTempPath
#undef	GetTempPath
#endif

#include <msclr\marshal.h>
#include <msclr\marshal_windows.h>
using namespace msclr::interop;

using namespace System;
using namespace System::Text;

#pragma managed(push,off)
#include "Log.h"
#include "ExceptionMacros.h"
#include "HelperTemplates.h"
#pragma managed(pop)

/////////////////////////////////////////////////////////////////////////////

static inline tBstrPtr StringBSTR (String^ pString)
{
	return static_cast<BSTR>(System::Runtime::InteropServices::Marshal::StringToBSTR(pString).ToPointer());
}

#define	_B(x) StringBSTR(x)
#define	_BT(x) (x?StringBSTR(x->FullName):tBstrPtr())
#define	_BM(x) (x?StringBSTR(x->Name):tBstrPtr())
#define	_BF(x) (x?StringBSTR(x->Name):tBstrPtr())
#define	_BP(x) (x?StringBSTR(x->Name):tBstrPtr())
#define	_BMT(x) (((x)&&(x->DeclaringType))?StringBSTR(x->DeclaringType->FullName):tBstrPtr())
#define	_BFT(x) (((x)&&(x->DeclaringType))?StringBSTR(x->DeclaringType->FullName):tBstrPtr())
#define	_BPT(x) (((x)&&(x->DeclaringType))?StringBSTR(x->DeclaringType->FullName):tBstrPtr())

/////////////////////////////////////////////////////////////////////////////

#include "AssemblyDlg.h"
