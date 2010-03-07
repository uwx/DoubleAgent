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
#include "Sapi5Voices.h"
#include "Sapi5Voice.h"
#include "Sapi5Err.h"
#include "AgentFile.h"
#include "MallocPtr.h"
#include "StringArrayEx.h"
#include "UiState.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef	_DEBUG
#define	_DEBUG_VOICES		(GetProfileDebugInt(_T("LogVoices"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_TTS_MATCH	(GetProfileDebugInt(_T("LogVoiceMatch"),LogVerbose,true)&0xFFFF)
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IEnumSpObjectTokens, __uuidof(IEnumSpObjectTokens));
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpObjectTokenCategory, __uuidof(ISpObjectTokenCategory));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

//////////////////////////////////////////////////////////////////////

CSapi5VoiceInfo::CSapi5VoiceInfo ()
:	mLangId (MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL)),
	mSpeakerGender (GENDER_NEUTRAL),
	mSpeakerAge (TTSAGE_ADULT)
{
}

CSapi5VoiceInfo::~CSapi5VoiceInfo ()
{
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CSapi5Voices, CPtrArray)

CSapi5Voices::CSapi5Voices ()
:	mLogLevelDebug (LogVerbose)
{
#ifdef	_DEBUG_VOICES
	const_cast <UINT&> (mLogLevelDebug) = _DEBUG_VOICES;
#endif
	Enumerate ();
}

CSapi5Voices::~CSapi5Voices ()
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

bool CSapi5Voices::IsSapi5Installed ()
{
	static bool	lInstalled = false;
	static bool	lFirstCheck = true;

	if	(lFirstCheck)
	{
		try
		{
			CoInitialize (NULL);

			try
			{
				ISpObjectTokenCategoryPtr	lTokenCategory;

				if	(SUCCEEDED (SpGetCategoryFromId (SPCAT_VOICES, &lTokenCategory)))
				{
					lInstalled = true;
				}
				SafeFreeSafePtr (lTokenCategory);

				lFirstCheck = false;
			}
			catch AnyExceptionSilent

			CoUninitialize ();
		}
		catch AnyExceptionSilent
	}

	return lInstalled;
}

//////////////////////////////////////////////////////////////////////

void CSapi5Voices::Enumerate ()
{
	IEnumSpObjectTokensPtr	lEnum;
	ISpObjectTokenPtr		lToken;

	DeleteAll ();

	if	(
			(SUCCEEDED (SpEnumTokens (SPCAT_VOICES, L"", L"", &lEnum)))
		&&	(lEnum != NULL)
		)
	{
		while (lEnum->Next (1, &lToken, NULL) == S_OK)
		{
			ISpDataKeyPtr		lAttributes;
			tMallocPtr <WCHAR>	lSapiStr;
			DWORD				lSapiValue;
			CSapi5VoiceInfo *	lVoiceInfo;

			if	(LogIsActive (mLogLevelDebug))
			{
				LogVoiceToken (mLogLevelDebug, lToken.GetInterfacePtr ());
			}

			if	(lVoiceInfo = new CSapi5VoiceInfo)
			{
				if	(SUCCEEDED (LogSapi5Err (LogNormal, SpGetDescription (lToken, lSapiStr.Free (), NULL))))
				{
					lVoiceInfo->mVoiceName = _bstr_t (lSapiStr).Detach();
					lVoiceInfo->mProduct = tBstrPtr (lVoiceInfo->mVoiceName).Detach();
				}
				if	(SUCCEEDED (lToken->GetId (lSapiStr.Free ())))
				{
					lVoiceInfo->mVoiceIdLong = _bstr_t (lSapiStr).Detach ();
					lVoiceInfo->mVoiceIdShort = CSapi5Voice::ShortVoiceId (lVoiceInfo->mVoiceIdLong).Detach ();
				}

				if	(SUCCEEDED (lToken->GetStringValue (L"VoiceName", lSapiStr.Free ())))
				{
					CString	lAltName (lSapiStr);
					CString	lVoiceName (lVoiceInfo->mVoiceName);

					if	(
							(lAltName.GetLength () > 0)
						&&	(lAltName.GetLength () < lVoiceName.GetLength ())
						)
					{
						lAltName.MakeUpper ();
						lVoiceName.MakeUpper ();

						if	(
								(lVoiceName.Find (lAltName) >= 0)
							&&	(FindVoiceName (lAltName) < 0)
							)
						{
							lVoiceInfo->mVoiceName = _bstr_t (lSapiStr).Detach();
						}
					}
				}

				if	(SUCCEEDED (lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
				{
					CStringArray	lLangIdStrs;
					LPTSTR			lLangIdStrEnd;

					if	(SUCCEEDED (lAttributes->GetStringValue (L"Name", lSapiStr.Free ())))
					{
						CString	lAltName (lSapiStr);
						CString	lVoiceName (lVoiceInfo->mVoiceName);

						if	(
								(lAltName.GetLength () > 0)
							&&	(lAltName.GetLength () < lVoiceName.GetLength ())
							)
						{
							lAltName.MakeUpper ();
							lVoiceName.MakeUpper ();

							if	(
									(lVoiceName.Find (lAltName) >= 0)
								&&	(FindVoiceName (lAltName) < 0)
								)
							{
								lVoiceInfo->mVoiceName = _bstr_t (lSapiStr).Detach();
							}
						}
					}

					if	(SUCCEEDED (lAttributes->GetDWORD (L"Gender", &lSapiValue)))
					{
						lVoiceInfo->mSpeakerGender = lSapiValue;
					}
					else
					if	(SUCCEEDED (lAttributes->GetStringValue (L"Gender", lSapiStr.Free ())))
					{
						CString	lGenderName (lSapiStr);

						if	(lGenderName.CompareNoCase (_T("Male")) == 0)
						{
							lVoiceInfo->mSpeakerGender = GENDER_MALE;
						}
						else
						if	(lGenderName.CompareNoCase (_T("Female")) == 0)
						{
							lVoiceInfo->mSpeakerGender = GENDER_FEMALE;
						}
					}

					if	(SUCCEEDED (lAttributes->GetDWORD (L"Language", &lSapiValue)))
					{
						if	(IsValidLocale (MAKELCID ((LANGID)lSapiValue, SORT_DEFAULT), LCID_SUPPORTED))
						{
							lVoiceInfo->mLangId = (LANGID)lSapiValue;
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
						lVoiceInfo->mLangId = (LANGID)lSapiValue;
					}

					if	(SUCCEEDED (lAttributes->GetStringValue (L"Vendor", lSapiStr.Free ())))
					{
						CString	lAltName (lVoiceInfo->mVoiceName);
						CString	lManufacturer (lSapiStr);

						lVoiceInfo->mManufacturer = _bstr_t (lSapiStr).Detach();

						if	(
								(lAltName.GetLength () > lManufacturer.GetLength ())
							&&	(lAltName.Left(lManufacturer.GetLength ()).CompareNoCase (lManufacturer) == 0)
							)
						{
							lAltName.Delete (0, lManufacturer.GetLength ());
							lAltName.TrimLeft ();

							if	(FindVoiceName (lAltName) < 0)
							{
								lVoiceInfo->mVoiceName = lAltName.AllocSysString();
							}
						}
					}

					if	(SUCCEEDED (lAttributes->GetStringValue (L"Age", lSapiStr.Free ())))
					{
						CString	lAgeStr (lSapiStr);

						if	(lAgeStr.CompareNoCase (_T("Baby")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_BABY;
						}
						else
						if	(lAgeStr.CompareNoCase (_T("Toddler")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_TODDLER;
						}
						else
						if	(lAgeStr.CompareNoCase (_T("Child")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_CHILD;
						}
						else
						if	(lAgeStr.CompareNoCase (_T("Adult")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_ADULT;
						}
						else
						if	(lAgeStr.CompareNoCase (_T("Adolescent")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_ADOLESCENT;
						}
						else
						if	(lAgeStr.CompareNoCase (_T("Elderly")) == 0)
						{
							lVoiceInfo->mSpeakerAge = TTSAGE_ELDERLY;
						}
					}
				}

				Add (lVoiceInfo);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Voices::FindVoiceId (LPCTSTR pVoiceId)
{
	INT_PTR				lNdx;
	CSapi5VoiceInfo *	lVoiceInfo;

	for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
	{
		if	(
				(lVoiceInfo = (operator [] (lNdx)))
			&&	(
					(CString (lVoiceInfo->mVoiceIdLong).CompareNoCase (pVoiceId) == 0)
				||	(CString (lVoiceInfo->mVoiceIdShort).CompareNoCase (pVoiceId) == 0)
				)
			)
		{
			return lNdx;
		}
	}

	return -1;
}

CSapi5VoiceInfo * CSapi5Voices::GetVoiceId (LPCTSTR pVoiceId)
{
	return operator () (FindVoiceId (pVoiceId));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Voices::FindVoiceName (LPCTSTR pVoiceName)
{
	INT_PTR				lNdx;
	CSapi5VoiceInfo *	lVoiceInfo;

	for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
	{
		if	(
				(lVoiceInfo = (operator [] (lNdx)))
			&&	(
					(CString (lVoiceInfo->mVoiceName).CompareNoCase (pVoiceName) == 0)
				||	(CString (lVoiceInfo->mProduct).CompareNoCase (pVoiceName) == 0)
				)
			)
		{
			return lNdx;
		}
	}

	return -1;
}

CSapi5VoiceInfo * CSapi5Voices::GetVoiceName (LPCTSTR pVoiceName)
{
	return operator () (FindVoiceName (pVoiceName));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Voices::FindVoice (const CAgentFileTts & pAgentFileTts, bool pUseDefaults, INT_PTR pStartAfter)
{
	INT_PTR	lRet = -1;

	try
	{
		CArrayEx <LANGID, LANGID>	lLanguageIds;
		INT_PTR						lLanguageNdx;
		CSapi5VoiceInfo *			lVoiceInfo;
		INT_PTR						lVoiceNdx;
		INT_PTR						lBestNdx = -1;
		int							lPartMatch = 0;
		int							lCurrMatch;
		int							lBestMatch = -1;
		static const int			lLanguageWeight = 1000;
		static const int			lGenderWeight = 200;
		static const int			lAgeWeight = 2;

#ifdef	_DEBUG_TTS_MATCH
		LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi5Voice Language [%4.4X] Gender [%u] Age [%u] Defaults [%u] From [%d]"), pAgentFileTts.mLanguage, pAgentFileTts.mGender, pAgentFileTts.mAge, pUseDefaults, pStartAfter+1);
#endif
		MakeLanguageMatchList (pAgentFileTts.mLanguage, lLanguageIds, pUseDefaults);

		for	(lVoiceNdx = max(pStartAfter+1,0); lVoiceNdx <= GetUpperBound (); lVoiceNdx++)
		{
			lVoiceInfo = GetAt (lVoiceNdx);
			lCurrMatch = 0;

#ifdef	_DEBUG_TTS_MATCH
			CString	lMatchLog;
			lMatchLog.Format (_T("%-20.20ls"), (BSTR)lVoiceInfo->mProduct);
#endif

			lLanguageNdx = lLanguageIds.Find (lVoiceInfo->mLangId);
			if	(lLanguageNdx >= 0)
			{
				lCurrMatch += lPartMatch = (int)(lLanguageIds.GetSize ()-lLanguageNdx) * lLanguageWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Language [%4.4X (%d)]"), CString((LPCTSTR)lMatchLog), lVoiceInfo->mLangId, lPartMatch);
#endif
			}
			else
			if	(SUBLANGID (lVoiceInfo->mLangId) != SUBLANG_NEUTRAL)
			{
				lLanguageNdx = lLanguageIds.Find (PRIMARYLANGID (lVoiceInfo->mLangId));
				if	(lLanguageNdx >= 0)
				{
					lCurrMatch += lPartMatch = (int)(lLanguageIds.GetSize ()-lLanguageNdx) * lLanguageWeight;
#ifdef	_DEBUG_TTS_MATCH
					lMatchLog.Format (_T("%s Language [%4.4X (%d)]"), CString((LPCTSTR)lMatchLog), lVoiceInfo->mLangId, lPartMatch);
#endif
				}
			}

			if	(pAgentFileTts.mGender != GENDER_NEUTRAL)
			{
				if	(lVoiceInfo->mSpeakerGender == pAgentFileTts.mGender)
				{
					lCurrMatch += lPartMatch = lGenderWeight;
				}
				else
				if	(lVoiceInfo->mSpeakerGender == GENDER_NEUTRAL)
				{
					lCurrMatch += lPartMatch = lGenderWeight/2;
				}
#ifdef	_DEBUG_TTS_MATCH
				else
				{
					lPartMatch = 0;
				}
				if	(lPartMatch)
				{
					lMatchLog.Format (_T("%s Gender [%u (%d)]"), CString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerGender, lPartMatch);
				}
#endif
			}

			if	(
					(pAgentFileTts.mAge != 0)
				&&	(lVoiceInfo->mSpeakerAge != 0)
				)
			{
				lCurrMatch += lPartMatch = -abs((int)pAgentFileTts.mAge-(int)lVoiceInfo->mSpeakerAge) * lAgeWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Age [%u (%d)]"), CString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerAge, lPartMatch);
#endif
			}

#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH|LogHighVolume, _T("  %4.4d %s"), lCurrMatch, lMatchLog);
#endif
			if	(lCurrMatch > lBestMatch)
			{
				lBestMatch = lCurrMatch;
				lBestNdx = lVoiceNdx;
			}
		}

		if	(lBestNdx >= 0)
		{
#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi5Voice [%ls] [%ls] [%ls] [%ls]"), (BSTR)GetAt(lBestNdx)->mVoiceName, (BSTR)GetAt(lBestNdx)->mProduct, (BSTR)GetAt(lBestNdx)->mVoiceIdShort, (BSTR)GetAt(lBestNdx)->mVoiceIdLong);
			LogMessage (_DEBUG_TTS_MATCH, _T(""));
#endif
			lRet = lBestNdx;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

CSapi5VoiceInfo * CSapi5Voices::GetVoice (const CAgentFileTts & pAgentFileTts, bool pUseDefaults, INT_PTR pStartAfter)
{
	return operator () (FindVoice (pAgentFileTts, pUseDefaults, pStartAfter));
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Voices::RemoveVoice (INT_PTR pVoiceNdx)
{
	if	(
			(pVoiceNdx >= 0)
		&&	(pVoiceNdx <= GetUpperBound ())
		)
	{
		if	(LogIsActive (mLogLevelDebug))
		{
			LogMessage (mLogLevelDebug, _T("Remove Voice [%ls] [%ls]"), GetAt(pVoiceNdx)->mVoiceIdShort, GetAt(pVoiceNdx)->mVoiceIdLong);
		}
		DeleteAt (pVoiceNdx);
		return true;
	}
	return false;
}

bool CSapi5Voices::RemoveVoice (const CSapi5VoiceInfo * pVoiceInfo)
{
	if	(pVoiceInfo)
	{
		return RemoveVoice (Find (pVoiceInfo));
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Voices::DefaultVoiceNdx ()
{
	INT_PTR				lRet = -1;
	ISpObjectTokenPtr	lToken;
	tMallocPtr <WCHAR>	lSapiStr;

	if	(
			(SUCCEEDED (SpGetDefaultTokenFromCategoryId (SPCAT_VOICES, &lToken, FALSE)))
		&&	(SUCCEEDED (lToken->GetId (lSapiStr.Free ())))
		)
	{
		lRet = FindVoiceId (CString (lSapiStr));
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Voices::VoiceSupportsLanguage (CSapi5VoiceInfo * pVoiceInfo, LANGID pLangId, bool pUseDefaults)
{
	bool						lRet = false;
	CArrayEx <LANGID, LANGID>	lLanguageIds;

	if	(pVoiceInfo)
	{
		MakeLanguageMatchList (pLangId, lLanguageIds, pUseDefaults);

		if	(lLanguageIds.Find (pVoiceInfo->mLangId) >= 0)
		{
			lRet = true;
		}
	}
	return lRet;
}

void CSapi5Voices::MakeLanguageMatchList (LANGID pLanguageId, CArrayEx <LANGID, LANGID> & pLanguageIds, bool pUseDefaults)
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
#ifdef	_DEBUG_TTS_MATCH
	LogMessage (_DEBUG_TTS_MATCH, _T("  LanguageMatches for [%4.4X] are [%s] defaults [%4.4X] [%4.4X] [%4.4X] [%4.4X]"), pLanguageId, FormatArray (pLanguageIds, _T("%4.4X")), GetUserDefaultUILanguage (), GetSystemDefaultUILanguage (), GetUserDefaultLangID (), GetSystemDefaultLangID ());
#endif
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

void CSapi5Voices::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
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
				lTitle = _T("SAPI5 Voices");
			}
			LogMessage (pLogLevel, _T("%s%s [%d]"), lIndent, lTitle, GetSize ());

			lIndent += _T("  ");
			for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
			{
				lTitle.Format (_T("Voice %d"), lNdx);
				LogVoiceInfo (pLogLevel|LogHighVolume, *operator[](lNdx), lTitle, lIndent);
			}
		}
		catch AnyExceptionDebug
	}
}

void CSapi5Voices::LogVoiceInfo (UINT pLogLevel, CSapi5VoiceInfo & pVoiceInfo, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CString	lTitle (pTitle);
			CString	lIndent (pIndent);

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Voice");
			}

			LogMessage (pLogLevel, _T("%s%s"), lIndent, lTitle);
			LogMessage (pLogLevel, _T("%s  Name         [%ls]"), lIndent, (BSTR)pVoiceInfo.mVoiceName);
			LogMessage (pLogLevel, _T("%s  Language     [%4.4X]"), lIndent, pVoiceInfo.mLangId);
			LogMessage (pLogLevel, _T("%s  Gender       [%u]"), lIndent, pVoiceInfo.mSpeakerGender);
			LogMessage (pLogLevel, _T("%s  Age          [%u]"), lIndent, pVoiceInfo.mSpeakerAge);
			LogMessage (pLogLevel, _T("%s  ShortId      [%ls]"), lIndent, (BSTR)pVoiceInfo.mVoiceIdShort);
			LogMessage (pLogLevel, _T("%s  LongId       [%ls]"), lIndent, (BSTR)pVoiceInfo.mVoiceIdLong);
			LogMessage (pLogLevel, _T("%s  Manufacturer [%ls]"), lIndent, (BSTR)pVoiceInfo.mManufacturer);
			LogMessage (pLogLevel, _T("%s  Product      [%ls]"), lIndent, (BSTR)pVoiceInfo.mProduct);
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CSapi5Voices::LogVoiceToken (UINT pLogLevel, void * pVoiceToken, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			ISpObjectToken *	lToken = (ISpObjectToken *) pVoiceToken;
			CString				lTitle (pTitle);
			tMallocPtr <WCHAR>	lVoiceName;
			tMallocPtr <WCHAR>	lVoiceId;
			ISpDataKeyPtr		lAttributes;
			tMallocPtr <WCHAR>	lGenderName;
			tMallocPtr <WCHAR>	lLanguageIdName;
			DWORD				lGenderCode;
			DWORD				lLanguageId;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Sapi5Voice");
			}
			LogMessage (pLogLevel, _T("%s [%p]"), lTitle, lToken);

			if	(lToken)
			{
				try
				{
					if	(SUCCEEDED (LogSapi5Err (LogNormal, SpGetDescription (lToken, lVoiceName.Free (), NULL))))
					{
						LogMessage (pLogLevel, _T("  Name     [%ls]"), (LPWSTR)lVoiceName);
					}
					if	(SUCCEEDED (LogSapi5Err (LogNormal, lToken->GetId (lVoiceId.Free ()))))
					{
						LogMessage (pLogLevel, _T("  Id       [%ls]"), (LPWSTR)lVoiceId);
					}

					if	(SUCCEEDED (lToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
					{
						if	(SUCCEEDED (lAttributes->GetDWORD (L"Gender", &lGenderCode)))
						{
							LogMessage (pLogLevel, _T("  Gender   [%u]"), (LPWSTR)lGenderCode);
						}
						if	(SUCCEEDED (lAttributes->GetStringValue (L"Gender", lGenderName.Free ())))
						{
							LogMessage (pLogLevel, _T("  Gender   [%ls]"), (LPWSTR)lGenderName);
						}

						if	(SUCCEEDED (lAttributes->GetDWORD (L"Language", &lLanguageId)))
						{
							LogMessage (pLogLevel, _T("  Language [%4.4X]"), lLanguageId);
						}
						if	(SUCCEEDED (lAttributes->GetStringValue (L"Language", lLanguageIdName.Free ())))
						{
							LogMessage (pLogLevel, _T("  Language [%ls]"), (LPWSTR)lLanguageIdName);
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
