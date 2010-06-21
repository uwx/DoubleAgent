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
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static inline System::String^ FormatArguments (array<System::Type^>^ pArguments)
{
	System::Text::StringBuilder^	lFormat = gcnew System::Text::StringBuilder;
	int								lArgumentNdx;

	if	(pArguments)
	{
		lFormat->Append ("<");
		for	(lArgumentNdx = 0; lArgumentNdx < pArguments->Length; lArgumentNdx++)
		{
			lFormat->Append (pArguments [lArgumentNdx]->Name);
			if	(lArgumentNdx < pArguments->Length-1)
			{
				lFormat->Append (",");
			}
		}
		lFormat->Append (">");
	}
	return lFormat->ToString ();
}

static inline System::String^ FormatMethodArguments (System::Reflection::MethodBase^ pMethod)
{
	array<System::Type^>^	lArguments;

	if	(
			(pMethod)
		&&	(pMethod->IsGenericMethodDefinition)
		&&	(lArguments = pMethod->GetGenericArguments ())
		)
	{
		return FormatArguments (lArguments);
	}
	return System::String::Empty;
}

static inline System::String^ FormatTypeArguments (System::Type^ pType)
{
	array<System::Type^>^	lArguments;

	if	(
			(pType)
		&&	(pType->IsGenericTypeDefinition)
		&&	(lArguments = pType->GetGenericArguments ())
		)
	{
		return FormatArguments (lArguments);
	}
	return String::Empty;
}

/////////////////////////////////////////////////////////////////////////////
#pragma page()
/////////////////////////////////////////////////////////////////////////////

static inline tBstrPtr StringBSTR (String^ pString)
{
	return static_cast<BSTR>(System::Runtime::InteropServices::Marshal::StringToBSTR(pString).ToPointer());
}

#define	_B(x) StringBSTR(x)

/////////////////////////////////////////////////////////////////////////////

static inline tBstrPtr _BT (System::Type^ pType)
{
	if	(pType)
	{
		if	(
				(pType->IsGenericParameter)
			&&	(!System::String::IsNullOrEmpty (pType->Name))
			)
		{
			return _B(pType->Name);
		}
		else
		if	(!System::String::IsNullOrEmpty (pType->FullName))
		{
			return _B(pType->FullName+FormatTypeArguments(pType));
		}
		else
		if	(!System::String::IsNullOrEmpty (pType->Namespace))
		{
			return _B(pType->Namespace+"."+pType->Name+FormatTypeArguments(pType));
		}
		else
		{
			return _B(pType->Name);
		}
	}
	return _B("<null>");
}

static inline tBstrPtr _BMT (System::Reflection::MemberInfo^ pMember)
{
	if	(pMember)
	{
		return _BT(pMember->DeclaringType);
	}
	else
	{
		return _B("<null>");
	}
}

static inline tBstrPtr _BM (System::Reflection::MemberInfo^ pMember)
{
	if	(pMember)
	{
		return _B(pMember->Name);
	}
	else
	{
		return _B("<null>");
	}
}

static inline tBstrPtr _BM (System::Reflection::ParameterInfo^ pParameter)
{
	if	(pParameter)
	{
		return _B(pParameter->Name);
	}
	else
	{
		return _B("<null>");
	}
}

static inline tBstrPtr _BM (System::Reflection::MethodInfo^ pMethod)
{
	if	(pMethod)
	{
		if	(pMethod->IsGenericMethodDefinition)
		{
			return _B(pMethod->Name+FormatMethodArguments(pMethod));
		}
		else
		{
			return _B(pMethod->Name);
		}
	}
	else
	{
		return _B("<null>");
	}
}

/////////////////////////////////////////////////////////////////////////////

#include "AssemblyDlg.h"
