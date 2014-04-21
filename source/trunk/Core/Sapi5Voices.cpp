/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2014 Cinnamon Software Inc.
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
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_VOICES		(GetProfileDebugInt(_T("LogVoices"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_TTS_MATCH	(GetProfileDebugInt(_T("LogVoiceMatch"),LogVerbose,true)&0xFFFF|LogTime)
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (IEnumSpObjectTokens, __uuidof(IEnumSpObjectTokens));
_COM_SMARTPTR_TYPEDEF (ISpObjectToken, __uuidof(ISpObjectToken));
_COM_SMARTPTR_TYPEDEF (ISpObjectTokenCategory, __uuidof(ISpObjectTokenCategory));
_COM_SMARTPTR_TYPEDEF (ISpDataKey, __uuidof(ISpDataKey));

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi5VoiceIndexArray)
IMPLEMENT_DLL_OBJECT(CSapi5VoiceInfoArray)
IMPLEMENT_DLL_OBJECT(CSapi5VoiceMatchRanks)
IMPLEMENT_DLL_OBJECT(CSapi5VoiceInfo)

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

IMPLEMENT_DLL_OBJECT(CSapi5Voices)

CSapi5Voices::CSapi5Voices ()
:	mLogLevelDebug (LogVerbose|LogTime)
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

CSapi5Voices * CSapi5Voices::CreateInstance ()
{
	return new CSapi5Voices;
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
			CSapi5VoiceInfo *	lVoiceInfo;

			if	(LogIsActive (mLogLevelDebug))
			{
				CSapi5Voice::LogToken (mLogLevelDebug, lToken.GetInterfacePtr ());
			}

			if	(lVoiceInfo = UniqueVoiceFromToken (lToken))
			{
				Add (lVoiceInfo);
			}
		}
	}
}

CSapi5VoiceInfo * CSapi5Voices::UniqueVoiceFromToken (ISpObjectToken * pVoiceToken)
{
	CSapi5VoiceInfo *	lVoiceInfo;
	ISpDataKeyPtr		lAttributes;
	tMallocPtr <WCHAR>	lSapiStr;

	if	(lVoiceInfo = VoiceFromToken (pVoiceToken))
	{
		if	(SUCCEEDED (pVoiceToken->GetStringValue (L"VoiceName", lSapiStr.Free ())))
		{
			CAtlString	lAltName (lSapiStr);
			CAtlString	lVoiceName (lVoiceInfo->mVoiceName);

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

		if	(SUCCEEDED (pVoiceToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
		{
			if	(SUCCEEDED (lAttributes->GetStringValue (L"Name", lSapiStr.Free ())))
			{
				CAtlString	lAltName (lSapiStr);
				CAtlString	lVoiceName (lVoiceInfo->mVoiceName);

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

			if	(SUCCEEDED (lAttributes->GetStringValue (L"Vendor", lSapiStr.Free ())))
			{
				CAtlString	lAltName (lVoiceInfo->mVoiceName);
				CAtlString	lManufacturer (lSapiStr);

				if	(
						(lAltName.GetLength () > lManufacturer.GetLength ())
					&&	(lAltName.Left (lManufacturer.GetLength ()).CompareNoCase (lManufacturer) == 0)
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
		}
	}
	return lVoiceInfo;
}

CSapi5VoiceInfo * CSapi5Voices::VoiceFromToken (ISpObjectToken * pVoiceToken)
{
	CSapi5VoiceInfo *	lVoiceInfo;
	ISpDataKeyPtr		lAttributes;
	tMallocPtr <WCHAR>	lSapiStr;
	DWORD				lSapiValue;

	if	(lVoiceInfo = new CSapi5VoiceInfo)
	{
		if	(SUCCEEDED (LogSapi5Err (LogNormal|LogTime, SpGetDescription (pVoiceToken, lSapiStr.Free (), NULL))))
		{
			lVoiceInfo->mVoiceName = _bstr_t (lSapiStr).Detach();
			lVoiceInfo->mProduct = tBstrPtr (lVoiceInfo->mVoiceName).Detach();
		}
		if	(SUCCEEDED (pVoiceToken->GetId (lSapiStr.Free ())))
		{
			lVoiceInfo->mVoiceIdLong = _bstr_t (lSapiStr).Detach ();
			lVoiceInfo->mVoiceIdShort = CSapi5Voice::ShortVoiceId (lVoiceInfo->mVoiceIdLong).Detach ();
		}

		if	(SUCCEEDED (pVoiceToken->OpenKey (SPTOKENKEY_ATTRIBUTES, &lAttributes)))
		{
			CAtlStringArray	lLangIdStrs;
			LPTSTR			lLangIdStrEnd;

			if	(SUCCEEDED (lAttributes->GetDWORD (L"Gender", &lSapiValue)))
			{
				lVoiceInfo->mSpeakerGender = lSapiValue;
			}
			else
			if	(SUCCEEDED (lAttributes->GetStringValue (L"Gender", lSapiStr.Free ())))
			{
				CAtlString	lGenderName (lSapiStr);

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
				&&	(MakeStringArray (CAtlString (lSapiStr), lLangIdStrs, _T(" ,;")) > 0)
				&&	(lSapiValue = _tcstoul (lLangIdStrs [0], &lLangIdStrEnd, 16))
				&&	(*lLangIdStrEnd == 0)
				&&	(IsValidLocale (MAKELCID ((LANGID)lSapiValue, SORT_DEFAULT), LCID_SUPPORTED))
				)
			{
				lVoiceInfo->mLangId = (LANGID)lSapiValue;
			}

			if	(SUCCEEDED (lAttributes->GetStringValue (L"Vendor", lSapiStr.Free ())))
			{
				lVoiceInfo->mManufacturer = _bstr_t (lSapiStr).Detach();
			}

			if	(SUCCEEDED (lAttributes->GetStringValue (L"Age", lSapiStr.Free ())))
			{
				CAtlString	lAgeStr (lSapiStr);

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
	}

	return lVoiceInfo;
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi5Voices::FindVoiceId (LPCTSTR pVoiceId)
{
	INT_PTR				lNdx;
	CSapi5VoiceInfo *	lVoiceInfo;

	for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
	{
		if	(
				(lVoiceInfo = (operator [] (lNdx)))
			&&	(
					(CAtlString (lVoiceInfo->mVoiceIdLong).CompareNoCase (pVoiceId) == 0)
				||	(CAtlString (lVoiceInfo->mVoiceIdShort).CompareNoCase (pVoiceId) == 0)
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

	for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
	{
		if	(
				(lVoiceInfo = (operator [] (lNdx)))
			&&	(
					(CAtlString (lVoiceInfo->mVoiceName).CompareNoCase (pVoiceName) == 0)
				||	(CAtlString (lVoiceInfo->mProduct).CompareNoCase (pVoiceName) == 0)
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

INT_PTR CSapi5Voices::FindVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank)
{
	tPtr <CSapi5VoiceIndexArray const>	lIndexArray;
	tPtr <CSapi5VoiceMatchRanks const>	lMatchRanks;

	if	(lIndexArray = FindVoices (pAgentFileTts, pUseDefaults, lMatchRanks.Free()))
	{
#ifdef	_DEBUG_TTS_MATCH
		LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi5Voice [%u] %4.4d [%ls] [%ls] [%ls] [%ls]"), pUseDefaults, (*lMatchRanks) [0], (BSTR)GetAt((*lIndexArray) [0])->mVoiceName, (BSTR)GetAt((*lIndexArray) [0])->mProduct, (BSTR)GetAt((*lIndexArray) [0])->mVoiceIdShort, (BSTR)GetAt((*lIndexArray) [0])->mVoiceIdLong);
		LogMessage (_DEBUG_TTS_MATCH, _T(""));
#endif
		if	(pMatchRank)
		{
			(*pMatchRank) = (*lMatchRanks) [0];
		}
		return (*lIndexArray) [0];
	}
	else
	if	(pMatchRank)
	{
		(*pMatchRank) = 0;
	}
	return -1;
}

CSapi5VoiceInfo * CSapi5Voices::GetVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank)
{
	return operator () (FindVoice (pAgentFileTts, pUseDefaults, pMatchRank));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapi5VoiceIndexArray const * CSapi5Voices::FindVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi5VoiceMatchRanks const ** pMatchRanks)
{
	tPtr <CSapi5VoiceIndexArray>	lIndexArray = new CSapi5VoiceIndexArray;
	tPtr <CSapi5VoiceMatchRanks>	lMatchRanks = new CSapi5VoiceMatchRanks;

	try
	{
		CAtlTypeArray <LANGID>		lLanguageIds;
		INT_PTR						lLanguageNdx;
		INT_PTR						lVoiceNdx;
		static const int			lLanguageWeight = 1000;
		static const int			lGenderWeight = 1000;
		static const int			lAgeWeight = 100;
		static const int			lOrderWeight = 1;

#ifdef	_DEBUG_TTS_MATCH
		LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi5Voices [%u] Language [%4.4X] Gender [%u] Age [%u]"), pUseDefaults, pAgentFileTts.Language, pAgentFileTts.Gender, pAgentFileTts.Age);
		LogLanguageMatchList (_DEBUG_TTS_MATCH, pAgentFileTts.Language, pUseDefaults, NULL, _T("  "));
#endif
		MakeLanguageMatchList (pAgentFileTts.Language, lLanguageIds, pUseDefaults);

		for	(lVoiceNdx = 0; lVoiceNdx < (INT_PTR)GetCount(); lVoiceNdx++)
		{
			CSapi5VoiceInfo *	lVoiceInfo = GetAt (lVoiceNdx);
			int					lPartMatch = 0;
			int					lCurrMatch = 0;

#ifdef	_DEBUG_TTS_MATCH
			CAtlString	lMatchLog;
			lMatchLog.Format (_T("%-20.20ls"), (BSTR)lVoiceInfo->mProduct);
#endif

			lLanguageNdx = FindLanguageMatch (lVoiceInfo->mLangId, lLanguageIds);
			if	(lLanguageNdx >= 0)
			{
				lCurrMatch += lPartMatch = ((int)(lLanguageIds.GetCount()-lLanguageNdx) * lLanguageWeight) + ((lLanguageIds [lLanguageNdx] == lVoiceInfo->mLangId) ? lLanguageWeight : 0);
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Language [%4.4X %4.4X (%d %d)]"), CAtlString((LPCTSTR)lMatchLog), lVoiceInfo->mLangId, lLanguageIds [lLanguageNdx], lLanguageNdx, lPartMatch);
#endif
			}

			if	(
					(lCurrMatch > 0)
				&&	(pAgentFileTts.Gender != GENDER_NEUTRAL)
				)
			{
				if	(lVoiceInfo->mSpeakerGender == pAgentFileTts.Gender)
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
					lMatchLog.Format (_T("%s Gender [%u (%d)]"), CAtlString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerGender, lPartMatch);
				}
#endif
			}

			if	(
					(lCurrMatch > 0)
				&&	(pAgentFileTts.Age != 0)
				&&	(lVoiceInfo->mSpeakerAge != 0)
				)
			{
				lCurrMatch += lPartMatch = -abs((int)pAgentFileTts.Age-(int)lVoiceInfo->mSpeakerAge) * lAgeWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Age [%u (%d)]"), CAtlString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerAge, lPartMatch);
#endif
			}

#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH|LogHighVolume, _T("  %3d [%6.6d] %s Name [%ls]"), lVoiceNdx, lCurrMatch, lMatchLog, (BSTR)GetAt(lVoiceNdx)->mVoiceName);
#endif
			if	(lCurrMatch > 0)
			{
				lIndexArray->InsertAt (lMatchRanks->AddSortedQS (lCurrMatch + (((int)GetCount()-(int)lVoiceNdx)*lOrderWeight), CSapi5VoiceMatchRanks::SortDescending, false), lVoiceNdx);
			}
		}

#ifdef	_DEBUG_TTS_MATCH
		LogMessage (_DEBUG_TTS_MATCH, _T("  Voices [%u] [%s]"), pUseDefaults, FormatArray (*lIndexArray, _T("%6d")));
		LogMessage (_DEBUG_TTS_MATCH, _T("  Ranks  [%u] [%s]"), pUseDefaults, FormatArray (*lMatchRanks, _T("%6.6d")));
#endif
	}
	catch AnyExceptionDebug

	if	(lIndexArray->GetCount () <= 0)
	{
		lIndexArray = NULL;
		lMatchRanks = NULL;
	}
	if	(pMatchRanks)
	{
		(*pMatchRanks) = lMatchRanks.Detach ();
	}
	return lIndexArray.Detach ();
}

//////////////////////////////////////////////////////////////////////

CSapi5VoiceInfoArray const * CSapi5Voices::GetVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi5VoiceMatchRanks const ** pMatchRanks)
{
	tPtr <CSapi5VoiceInfoArray>			lInfoArray;
	tPtr <CSapi5VoiceIndexArray const>	lIndexArray;
	INT_PTR								lNdx;

	if	(
			(lIndexArray = FindVoices (pAgentFileTts, pUseDefaults, pMatchRanks))
		&&	(lInfoArray = new CSapi5VoiceInfoArray)
		)
	{
		for	(lNdx = 0; lNdx < (INT_PTR)lIndexArray->GetCount(); lNdx++)
		{
			lInfoArray->Add (operator () ((*lIndexArray) [lNdx]));
		}
	}
	return lInfoArray.Detach();
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Voices::RemoveVoice (INT_PTR pVoiceNdx)
{
	if	(
			(pVoiceNdx >= 0)
		&&	(pVoiceNdx < (INT_PTR)GetCount())
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
		lRet = FindVoiceId (CAtlString (lSapiStr));
	}
	return lRet;
}

//////////////////////////////////////////////////////////////////////

bool CSapi5Voices::VoiceSupportsLanguage (CSapi5VoiceInfo * pVoiceInfo, LANGID pLangId, bool pUseDefaults)
{
	bool					lRet = false;
	CAtlTypeArray <LANGID>	lLanguageIds;

	if	(pVoiceInfo)
	{
		MakeLanguageMatchList (pLangId, lLanguageIds, pUseDefaults);

		if	(FindLanguageMatch (pVoiceInfo->mLangId, lLanguageIds) >= 0)
		{
			lRet = true;
		}
	}
	return lRet;
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
			CAtlString	lTitle (pTitle);
			CAtlString	lIndent (pIndent);
			INT_PTR		lNdx;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("SAPI5 Voices");
			}
			LogMessage (pLogLevel, _T("%s%s [%d]"), lIndent, lTitle, GetCount());

			lIndent += _T("  ");
			for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
			{
				lTitle.Format (_T("Voice %d"), lNdx);
				LogVoiceInfo (pLogLevel|LogHighVolume, *operator[](lNdx), lTitle, lIndent);
			}
		}
		catch AnyExceptionDebug
	}
}

void CSapi5Voices::LogVoiceInfo (UINT pLogLevel, CSapi5VoiceInfo& pVoiceInfo, LPCTSTR pTitle, LPCTSTR pIndent)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			CAtlString	lTitle (pTitle);
			CAtlString	lIndent (pIndent);

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
