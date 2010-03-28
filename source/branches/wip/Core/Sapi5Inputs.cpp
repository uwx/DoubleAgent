/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2010 Cinnamon Software Inc.
/////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Double Agent.

    Double Agent is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Double Agent is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Double Agent.  If not, see <http://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <sapi.h>
#include <sphelper.h>
#include "Sapi5Inputs.h"
#include "Sapi5Input.h"
#include "Sapi5Err.h"
#include "MallocPtr.h"
#include "StringArrayEx.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef	_DEBUG
#define	_DEBUG_INPUTS		(GetProfileDebugInt(_T("LogInputs"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_INPUT_MATCH	(GetProfileDebugInt(_T("LogInputMatch"),LogVerbose,true)&0xFFFF)
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IEnumSpObjectTokens, __uuidof(IEnumSpObjectTokens));
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpObjectTokenCategory, __uuidof(ISpObjectTokenCategory));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

//////////////////////////////////////////////////////////////////////

CSapi5InputInfo::CSapi5InputInfo ()
:	mLangId (MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL)),
	mLangIdCount (0)
{
}

CSapi5InputInfo::~CSapi5InputInfo ()
{
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CSapi5Inputs, CPtrArray)

CSapi5Inputs::CSapi5Inputs ()
:	mLogLevelDebug (LogVerbose)
{
#ifdef	_DEBUG_INPUTS
	const_cast <UINT&> (mLogLevelDebug) = _DEBUG_INPUTS;
#endif
	Enumerate ();
}

CSapi5Inputs::~CSapi5Inputs ()
{
	try
	{
		DeleteAll ();
	}
	catch AnyExceptionSilent
	try
	{
		RemoveAll ();
	}
	catch AnyExceptionSilent
}

//////////////////////////////////////////////////////////////////////

void CSapi5Inputs::Enumerate ()
{
	IEnumSpObjectTokensPtr	lEnum;
	ISpObjectTokenPtr		lToken;

	DeleteAll ();

	if	(
			(SUCCEEDED (SpEnumTokens (SPCAT_RECOGNIZERS, L"", L"", &lEnum)))
		&&	(lEnum != NULL)
		)
	{
		while (lEnum->Next (1, &lToken, NULL) == S_OK)
		{
			ISpDataKeyPtr		lAttributes;
			tMallocPtr <WCHAR>	lSapiStr;
			DWORD				lSapiValue;
			CSapi5InputInfo *	lInputInfo;

			if	(LogIsActive (mLogLevelDebug))
			{
				LogInputToken (mLogLevelDebug, lToken.GetInterfacePtr ());
			}

			if	(lInputInfo = new CSapi5InputInfo)
			{
				if	(SUCCEEDED (LogSapi5Err (LogNormal, SpGetDescription (lToken, lSapiStr.Free (), NULL))))
				{
					lInputInfo->mEngineName = _bstr_t (lSapiStr).Detach();
				}
				if	(SUCCEEDED (lToken->GetId (lSapiStr.Free ())))
				{
					lInputInfo->mEngineIdLong = _bstr_t (lSapiStr).Detach ();
					lInputInfo->mEngineIdShort = CSapi5Input::ShortEngineId (lInputInfo->mEngineIdLong).Detach ();
				}

				if	(SUCCEEDED (lToken->GetStringValue (SPTOKENVALUE_CLSID, lSapiStr.Free ())))
				{
					lInputInfo->mClassId = _bstr_t (lSapiStr).Detach();
				}

				if	(SUCCEEDED (lToken->GetStringValue (L"RecoExtension", lSapiStr.Free ())))
				{
					lInputInfo->mRecoExtension = _bstr_t (lSapiStr).Detach();
				}

				if	(SUCCEEDED (lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
				{
					CStringArray		lLangIdStrs;
					LPTSTR				lLangIdStrEnd;
					CTypeArray <LANGID>	lLocales;
					INT_PTR				lLocaleNdx;

					if	(SUCCEEDED (lAttributes->GetDWORD (L"Language", &lSapiValue)))
					{
						if	(IsValidLocale (MAKELCID ((LANGID)lSapiValue, SORT_DEFAULT), LCID_SUPPORTED))
						{
							lInputInfo->mLangId = (LANGID)lSapiValue;
							lLocales.AddUnique ((LANGID)lSapiValue);
						}
					}
					else
					if	(
							(SUCCEEDED (lAttributes->GetStringValue (L"Language", lSapiStr.Free ())))
						&&	(MakeStringArray (CString (lSapiStr), lLangIdStrs, _T(" ,;")) > 0)
						&&	(lSapiValue = _tcstoul (lLangIdStrs [0], &lLangIdStrEnd, 16))
						&&	(*lLangIdStrEnd == 0)
						&&	(IsValidLocale (MAKELCID ((LANGID)lSapiValue, SORT_DEFAULT), LCID_SUPPORTED))
						)
					{
						lInputInfo->mLangId = (LANGID)lSapiValue;
						lLocales.AddUnique ((LANGID)lSapiValue);
					}

					if	(
							(SUCCEEDED (lAttributes->GetStringValue (L"SupportedLocales", lSapiStr.Free ())))
						&&	(MakeStringArray (CString (lSapiStr), lLangIdStrs, _T(" ,;")) > 0)
						)
					{
						for	(lLocaleNdx = 0; lLocaleNdx <= lLangIdStrs.GetUpperBound(); lLocaleNdx++)
						{
							if	(
									(lSapiValue = _tcstoul (lLangIdStrs [lLocaleNdx], &lLangIdStrEnd, 16))
								&&	(*lLangIdStrEnd == 0)
								)
							{
								lLocales.AddUnique ((LANGID)lSapiValue);
							}
						}
					}
					if	(
							(
								(lLocales.GetSize() > 1)
							||	(
									(lLocales.GetSize() > 0)
								&&	(lLocales [0] != lInputInfo->mLangId)
								)
							)
						&&	(lInputInfo->mLangIdSupported = new LANGID [lLocales.GetSize()])
						)
					{
						lInputInfo->mLangIdCount = (UINT)lLocales.GetSize();
						for	(lLocaleNdx = 0; lLocaleNdx <= lLocales.GetUpperBound(); lLocaleNdx++)
						{
							lInputInfo->mLangIdSupported [lLocaleNdx] = lLocales [lLocaleNdx];
						}
					}

					if	(SUCCEEDED (lAttributes->GetStringValue (L"Vendor", lSapiStr.Free ())))
					{
						lInputInfo->mManufacturer = _bstr_t (lSapiStr).Detach();
					}
				}

				Add (lInputInfo);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Inputs::FindEngineId (LPCTSTR pEngineId)
{
	INT_PTR				lNdx;
	CSapi5InputInfo *	lInputInfo;

	for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
	{
		if	(
				(lInputInfo = (operator [] (lNdx)))
			&&	(
					(CString (lInputInfo->mEngineIdLong).CompareNoCase (pEngineId) == 0)
				||	(CString (lInputInfo->mEngineIdShort).CompareNoCase (pEngineId) == 0)
				)
			)
		{
			return lNdx;
		}
	}

	return -1;
}

CSapi5InputInfo * CSapi5Inputs::GetEngineId (LPCTSTR pEngineId)
{
	return operator () (FindEngineId (pEngineId));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Inputs::FindEngineName (LPCTSTR pEngineName)
{
	INT_PTR				lNdx;
	CSapi5InputInfo *	lInputInfo;

	for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
	{
		if	(
				(lInputInfo = (operator [] (lNdx)))
			&&	(CString (lInputInfo->mEngineName).CompareNoCase (pEngineName) == 0)
			)
		{
			return lNdx;
		}
	}

	return -1;
}

CSapi5InputInfo * CSapi5Inputs::GetEngineName (LPCTSTR pEngineName)
{
	return operator () (FindEngineName (pEngineName));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Inputs::FindInput (LANGID pLangId, bool pUseDefaults, INT_PTR pStartAfter)
{
	INT_PTR	lRet = -1;

	try
	{
		CTypeArray <LANGID>			lLanguageIds;
		INT_PTR						lLanguageNdx;
		INT_PTR						lLocaleNdx;
		CSapi5InputInfo *			lInputInfo;
		INT_PTR						lInputNdx;
		INT_PTR						lBestLanguage = INT_MAX;
		INT_PTR						lBestNdx = -1;

#ifdef	_DEBUG_INPUT_MATCH
		LogMessage (_DEBUG_INPUT_MATCH, _T("FindInput Language [%4.4X] Defaults [%u] From [%d]"), pLangId, pUseDefaults, pStartAfter);
#endif
		MakeLanguageMatchList (pLangId, lLanguageIds, pUseDefaults);

		for	(lInputNdx = max(pStartAfter+1,0); lInputNdx <= GetUpperBound (); lInputNdx++)
		{
			lInputInfo = GetAt (lInputNdx);

#ifdef	_DEBUG_INPUT_MATCH
			CString	lMatchLog;
			lMatchLog.Format (_T("%-60.60ls"), (BSTR)lInputInfo->mEngineName);
#endif

			lLanguageNdx = lLanguageIds.Find (lInputInfo->mLangId);
			if	(lLanguageNdx >= 0)
			{
#ifdef	_DEBUG_INPUT_MATCH
				lMatchLog.Format (_T("%s Language [%4.4X] [%d]"), CString((LPCTSTR)lMatchLog), lInputInfo->mLangId, lLanguageNdx);
#endif
				if	(lLanguageNdx < lBestLanguage)
				{
					lBestLanguage = lLanguageNdx;
					lBestNdx = lInputNdx;
				}
			}

			if	(
					(lLanguageNdx < 0)
				&&	(lInputInfo->mLangIdCount > 0)
				)
			{
				for	(lLocaleNdx = 0; lLocaleNdx < (INT_PTR)lInputInfo->mLangIdCount; lLocaleNdx++)
				{
					lLanguageNdx = lLanguageIds.Find (lInputInfo->mLangIdSupported [lLocaleNdx]);
					if	(lLanguageNdx >= 0)
					{
#ifdef	_DEBUG_INPUT_MATCH
						lMatchLog.Format (_T("%s Language [%4.4X] [%d]"), CString((LPCTSTR)lMatchLog), lInputInfo->mLangIdSupported [lLocaleNdx], lLanguageNdx);
#endif
						if	(lLanguageNdx < lBestLanguage)
						{
							lBestLanguage = lLanguageNdx;
							lBestNdx = lInputNdx;
						}
						break;
					}
				}
			}

			if	(
					(lLanguageNdx < 0)
				&&	(SUBLANGID (lInputInfo->mLangId) != SUBLANG_NEUTRAL)
				)
			{
				lLanguageNdx = lLanguageIds.Find (PRIMARYLANGID (lInputInfo->mLangId));
				if	(lLanguageNdx >= 0)
				{
#ifdef	_DEBUG_INPUT_MATCH
					lMatchLog.Format (_T("%s Language [%4.4X] [%d]"), CString((LPCTSTR)lMatchLog), PRIMARYLANGID (lInputInfo->mLangId), lLanguageNdx);
#endif
					if	(lLanguageNdx < lBestLanguage)
					{
						lBestLanguage = lLanguageNdx;
						lBestNdx = lInputNdx;
					}
				}
			}

#ifdef	_DEBUG_INPUT_MATCH
			LogMessage (_DEBUG_INPUT_MATCH|LogHighVolume, _T("  %4.4d %s"), lBestNdx, lMatchLog);
#endif
			if	(lBestLanguage == 0)
			{
				break;
			}
		}

		if	(lBestNdx >= 0)
		{
#ifdef	_DEBUG_INPUT_MATCH
			LogMessage (_DEBUG_INPUT_MATCH, _T("FindInput [%ls] [%ls] [%ls] [%ls]"), (BSTR)GetAt(lBestNdx)->mEngineName, (BSTR)GetAt(lBestNdx)->mClassId, (BSTR)GetAt(lBestNdx)->mEngineIdShort, (BSTR)GetAt(lBestNdx)->mEngineIdLong);
			LogMessage (_DEBUG_INPUT_MATCH, _T(""));
#endif
			lRet = lBestNdx;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

CSapi5InputInfo * CSapi5Inputs::GetInput (LANGID pLangId, bool pUseDefaults, INT_PTR pStartAfter)
{
	return operator () (FindInput (pLangId, pUseDefaults, pStartAfter));
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Inputs::RemoveInput (INT_PTR pInputNdx)
{
	if	(
			(pInputNdx >= 0)
		&&	(pInputNdx <= GetUpperBound ())
		)
	{
		if	(LogIsActive (mLogLevelDebug))
		{
			LogMessage (mLogLevelDebug, _T("Remove Input [%ls] [%ls]"), GetAt(pInputNdx)->mEngineIdShort, GetAt(pInputNdx)->mEngineIdLong);
		}
		DeleteAt (pInputNdx);
		return true;
	}
	return false;
}

bool CSapi5Inputs::RemoveInput (const CSapi5InputInfo * pInputInfo)
{
	if	(pInputInfo)
	{
		return RemoveInput (Find (pInputInfo));
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Inputs::DefaultInputNdx ()
{
	INT_PTR				lRet = -1;
	ISpObjectTokenPtr	lToken;
	tMallocPtr <WCHAR>	lSapiStr;

	if	(
			(SUCCEEDED (SpGetDefaultTokenFromCategoryId (SPCAT_RECOGNIZERS, &lToken, FALSE)))
		&&	(SUCCEEDED (lToken->GetId (lSapiStr.Free ())))
		)
	{
		lRet = FindEngineId (CString (lSapiStr));
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Inputs::InputSupportsLanguage (CSapi5InputInfo * pInputInfo, LANGID pLangId, bool pUseDefaults)
{
	bool				lRet = false;
	CTypeArray <LANGID>	lLanguageIds;
	INT_PTR				lLocaleNdx;

	if	(pInputInfo)
	{
		MakeLanguageMatchList (pLangId, lLanguageIds, pUseDefaults);

		if	(lLanguageIds.Find (pInputInfo->mLangId) >= 0)
		{
			lRet = true;
		}
		else
		if	(pInputInfo->mLangIdCount > 0)
		{
			for	(lLocaleNdx = 0; lLocaleNdx < (INT_PTR)pInputInfo->mLangIdCount; lLocaleNdx++)
			{
				if	(lLanguageIds.Find (pInputInfo->mLangIdSupported [lLocaleNdx]) >= 0)
				{
					lRet = true;
					break;
				}
			}
		}
	}
	return lRet;
}

void CSapi5Inputs::MakeLanguageMatchList (LANGID pLanguageId, CTypeArray <LANGID> & pLanguageIds, bool pUseDefaults)
{
	pLanguageIds.RemoveAll ();

	if	(pLanguageId)
	{
		if	(IsValidLocale (MAKELCID (pLanguageId, SORT_DEFAULT), LCID_SUPPORTED))
		{
			pLanguageIds.AddUnique (pLanguageId);
		}

		if	(
				(SUBLANGID (pLanguageId) != SUBLANG_DEFAULT)
			&&	(IsValidLocale (MAKELCID (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_DEFAULT), SORT_DEFAULT), LCID_SUPPORTED))
			)
		{
			pLanguageIds.AddUnique (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_DEFAULT));
		}

		if	(
				(SUBLANGID (pLanguageId) != SUBLANG_DEFAULT)
			&&	(IsValidLocale (MAKELCID (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_SYS_DEFAULT), SORT_DEFAULT), LCID_SUPPORTED))
			)
		{
			pLanguageIds.AddUnique (MAKELANGID (PRIMARYLANGID (pLanguageId), SUBLANG_SYS_DEFAULT));
		}
	}

	if	(pUseDefaults)
	{
		pLanguageIds.AddUnique (GetUserDefaultUILanguage ());
		pLanguageIds.AddUnique (GetSystemDefaultUILanguage ());
		pLanguageIds.AddUnique (GetUserDefaultLangID ());
		pLanguageIds.AddUnique (GetSystemDefaultLangID ());
		pLanguageIds.AddUnique (MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US));
		pLanguageIds.AddUnique (MAKELANGID (LANG_ENGLISH, SUBLANG_NEUTRAL));
	}
#ifdef	_DEBUG_INPUT_MATCH
	LogMessage (_DEBUG_INPUT_MATCH, _T("  LanguageMatches for [%4.4X] are [%s] defaults [%4.4X] [%4.4X] [%4.4X] [%4.4X]"), pLanguageId, FormatArray (pLanguageIds, _T("%4.4X")), GetUserDefaultUILanguage (), GetSystemDefaultUILanguage (), GetUserDefaultLangID (), GetSystemDefaultLangID ());
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CSapi5Inputs::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);
			int		lNdx;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("SAPI5 Inputs");
			}
			LogMessage (pLogLevel, _T("%s%s [%d]"), lIndent, lTitle, GetSize ());

			lIndent += _T("  ");
			for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
			{
				lTitle.Format (_T("Input %d"), lNdx);
				LogInputInfo (pLogLevel|LogHighVolume, *operator[](lNdx), lTitle, lIndent);
			}
		}
		catch AnyExceptionDebug
	}
}

void CSapi5Inputs::LogInputInfo (UINT pLogLevel, CSapi5InputInfo & pInputInfo, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Input");
			}

			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
			LogMessage (pLogLevel, _T("%s  Name         [%ls]"), lIndent, (BSTR)pInputInfo.mEngineName);
			if	(
					(pInputInfo.mLangIdCount > 0)
				&&	(pInputInfo.mLangIdSupported.Ptr())
				)
			{
				LogMessage (pLogLevel, _T("%s  Language     [%4.4X] [%s]"), lIndent, pInputInfo.mLangId, FormatArray (pInputInfo.mLangIdSupported.Ptr(), (INT_PTR)pInputInfo.mLangIdCount, _T("%4.4X")));
			}
			else
			{
				LogMessage (pLogLevel, _T("%s  Language     [%4.4X]"), lIndent, pInputInfo.mLangId);
			}
			LogMessage (pLogLevel, _T("%s  ShortId      [%ls]"), lIndent, (BSTR)pInputInfo.mEngineIdShort);
			LogMessage (pLogLevel, _T("%s  LongId       [%ls]"), lIndent, (BSTR)pInputInfo.mEngineIdLong);
			LogMessage (pLogLevel, _T("%s  ClassId      [%ls]"), lIndent, (BSTR)pInputInfo.mClassId);
			LogMessage (pLogLevel, _T("%s  RecoExt      [%ls]"), lIndent, (BSTR)pInputInfo.mRecoExtension);
			LogMessage (pLogLevel, _T("%s  Manufacturer [%ls]"), lIndent, (BSTR)pInputInfo.mManufacturer);
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CSapi5Inputs::LogInputToken (UINT pLogLevel, void * pInputToken, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			ISpObjectToken *	lToken = (ISpObjectToken *) pInputToken;
			CString				lTitle (pTitle);
			tMallocPtr <WCHAR>	lInputName;
			tMallocPtr <WCHAR>	lInputId;
			tMallocPtr <WCHAR>	lClassId;
			ISpDataKeyPtr		lAttributes;
			tMallocPtr <WCHAR>	lLanguageIdName;
			DWORD				lLanguageId;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Sapi5Input");
			}
			LogMessage (pLogLevel, _T("%s [%p]"), lTitle, lToken);

			if	(lToken)
			{
				try
				{
					if	(SUCCEEDED (LogSapi5Err (LogNormal, SpGetDescription (lToken, lInputName.Free (), NULL))))
					{
						LogMessage (pLogLevel, _T("  Name     [%ls]"), (LPWSTR)lInputName);
					}
					if	(SUCCEEDED (LogSapi5Err (LogNormal, lToken->GetId (lInputId.Free ()))))
					{
						LogMessage (pLogLevel, _T("  Id       [%ls]"), (LPWSTR)lInputId);
					}
					if	(SUCCEEDED (lToken->GetStringValue (SPTOKENVALUE_CLSID, lClassId.Free ())))
					{
						LogMessage (pLogLevel, _T("  ClassId  [%ls]"), (LPWSTR)lClassId);
					}

					if	(SUCCEEDED (lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
					{
						if	(SUCCEEDED (lAttributes->GetDWORD (L"Language", &lLanguageId)))
						{
							LogMessage (pLogLevel, _T("  Language [%4.4X]"), lLanguageId);
						}
						if	(SUCCEEDED (lAttributes->GetStringValue (L"Language", lLanguageIdName.Free ())))
						{
							LogMessage (pLogLevel, _T("  Language [%ls]"), (LPWSTR)lLanguageIdName);
						}
						if	(SUCCEEDED (lAttributes->GetStringValue (L"SupportedLocales", lLanguageIdName.Free ())))
						{
							LogMessage (pLogLevel, _T("  Locales  [%ls]"), (LPWSTR)lLanguageIdName);
						}
					}
					LogMessage (pLogLevel, _T(""));
				}
				catch AnyExceptionSilent

#ifdef	_DEBUG_NOT
				try
				{
					ULONG				lValueNdx;
					tMallocPtr <WCHAR>	lValueName;
					DWORD				lValueNum;
					tMallocPtr <WCHAR>	lValueStr;

					for	(lValueNdx = 0; lToken->EnumValues (lValueNdx, lValueName.Free ()) == S_OK; lValueNdx++)
					{
						if	(SUCCEEDED (lToken->GetDWORD (lValueName, &lValueNum)))
						{
							LogMessage (pLogLevel, _T("  Value [%ls] [%8.8X (%u) (%d)]"), (LPWSTR)lValueName, lValueNum, lValueNum, lValueNum);
						}
						else
						if	(SUCCEEDED (lToken->GetStringValue (lValueName, lValueStr.Free ())))
						{
							LogMessage (pLogLevel, _T("  Value [%ls] [%ls]"), (LPWSTR)lValueName, (LPWSTR)lValueStr);
						}
						else
						{
							LogMessage (pLogLevel, _T("  Value [%ls]"), (LPWSTR)lValueName);
						}
					}

					if	(SUCCEEDED (lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
					{
						LogMessage (pLogLevel, _T("  Attributes"));

						for	(lValueNdx = 0; lAttributes->EnumValues (lValueNdx, lValueName.Free ()) == S_OK; lValueNdx++)
						{
							if	(SUCCEEDED (lAttributes->GetDWORD (lValueName, &lValueNum)))
							{
								LogMessage (pLogLevel, _T("    Value [%ls] [%8.8X (%u) (%d)]"), (LPWSTR)lValueName, lValueNum, lValueNum, lValueNum);
							}
							else
							if	(SUCCEEDED (lAttributes->GetStringValue (lValueName, lValueStr.Free ())))
							{
								LogMessage (pLogLevel, _T("    Value [%ls] [%ls]"), (LPWSTR)lValueName, (LPWSTR)lValueStr);
							}
							else
							{
								LogMessage (pLogLevel, _T("    Value [%ls]"), (LPWSTR)lValueName);
							}
						}
					}
					LogMessage (pLogLevel, _T(""));
				}
				catch AnyExceptionSilent
#endif
			}
		}
		catch AnyExceptionSilent
	}
}
