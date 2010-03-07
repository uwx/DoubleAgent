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
#include "Sapi4Voices.h"
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#include "AgentFile.h"
#include "StringArrayEx.h"
#include "GuidStr.h"
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

_COM_SMARTPTR_TYPEDEF (ITTSEnum, IID_ITTSEnum);

//////////////////////////////////////////////////////////////////////

CSapi4VoiceInfo::CSapi4VoiceInfo ()
:	mModeId (GUID_NULL),
	mEngineId (GUID_NULL),
	mLangId (MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL)),
	mSpeakerGender (GENDER_NEUTRAL),
	mSpeakerAge (TTSAGE_ADULT)
{
}

CSapi4VoiceInfo::~CSapi4VoiceInfo ()
{
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CSapi4Voices, CPtrArray)

CSapi4Voices::CSapi4Voices ()
:	mLogLevelDebug (LogVerbose)
{
#ifdef	_DEBUG_VOICES
	const_cast <UINT&> (mLogLevelDebug) = _DEBUG_VOICES;
#endif
	Enumerate ();
}

CSapi4Voices::~CSapi4Voices ()
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

bool CSapi4Voices::IsSapi4Installed ()
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
				IUnknownPtr	lUnknown;

				if	(SUCCEEDED (CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_SERVER, __uuidof (IUnknown), (void **) &lUnknown)))
				{
					lInstalled = true;
				}
				SafeFreeSafePtr (lUnknown);

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

void CSapi4Voices::Enumerate ()
{
	ITTSEnumPtr			lEnum;
	tS <TTSMODEINFO>	lModeInfo;
	CSapi4VoiceInfo *	lVoiceInfo;

	DeleteAll ();

	if	(
			(SUCCEEDED (LogComErr (LogVerbose, CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_ITTSEnum, (void **) &lEnum))))
		&&	(lEnum != NULL)
		)
	{
		while (lEnum->Next (1, &lModeInfo, NULL) == S_OK)
		{
			if	(LogIsActive (mLogLevelDebug))
			{
				LogModeInfo (mLogLevelDebug, &lModeInfo);
			}

			if	(lVoiceInfo = new CSapi4VoiceInfo)
			{
				lVoiceInfo->mEngineId = lModeInfo.gEngineID;
				lVoiceInfo->mModeId = lModeInfo.gModeID;
				lVoiceInfo->mLangId = lModeInfo.language.LanguageID;
				lVoiceInfo->mVoiceName = CString (lModeInfo.szSpeaker).AllocSysString ();
				lVoiceInfo->mSpeakerGender = lModeInfo.wGender;
				lVoiceInfo->mSpeakerAge = lModeInfo.wAge;
				lVoiceInfo->mManufacturer = CString (lModeInfo.szMfgName).AllocSysString ();
				lVoiceInfo->mProduct = CString (lModeInfo.szProductName).AllocSysString ();

				Add (lVoiceInfo);
			}
			lModeInfo.Clear ();
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindModeId (const GUID & pModeId)
{
	INT_PTR				lNdx;
	CSapi4VoiceInfo *	lVoiceInfo;

	for	(lNdx = 0; lNdx <= GetUpperBound (); lNdx++)
	{
		if	(
				(lVoiceInfo = (operator [] (lNdx)))
			&&	(IsEqualGUID (lVoiceInfo->mModeId, pModeId))
			)
		{
			return lNdx;
		}
	}

	return -1;
}

CSapi4VoiceInfo * CSapi4Voices::GetModeId (const GUID & pModeId)
{
	return operator () (FindModeId (pModeId));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindVoiceName (LPCTSTR pVoiceName)
{
	INT_PTR				lNdx;
	CSapi4VoiceInfo *	lVoiceInfo;

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

CSapi4VoiceInfo * CSapi4Voices::GetVoiceName (LPCTSTR pVoiceName)
{
	return operator () (FindVoiceName (pVoiceName));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindVoice (const CAgentFileTts & pAgentFileTts, bool pUseDefaults, INT_PTR pStartAfter)
{
	INT_PTR	lRet = -1;

	try
	{
		CArrayEx <LANGID, LANGID>	lLanguageIds;
		INT_PTR						lLanguageNdx;
		CSapi4VoiceInfo *			lVoiceInfo;
		INT_PTR						lVoiceNdx;
		INT_PTR						lBestNdx = -1;
		int							lPartMatch = 0;
		int							lCurrMatch;
		int							lBestMatch = -1;
		static const int			lLanguageWeight = 1000;
		static const int			lGenderWeight = 200;
		static const int			lAgeWeight = 2;

		if	(!IsEqualGUID (pAgentFileTts.mMode, GUID_NULL))
		{
#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voice [%s] From [%d]"), (CString)CGuidStr(pAgentFileTts.mMode), pStartAfter+1);
#endif
			for	(lVoiceNdx = max(pStartAfter+1,0); lVoiceNdx <= GetUpperBound (); lVoiceNdx++)
			{
				lVoiceInfo = GetAt (lVoiceNdx);
				if	(IsEqualGUID (pAgentFileTts.mMode, lVoiceInfo->mModeId))
				{
					lBestNdx = lVoiceNdx;
					break;
				}
			}
		}

		if	(lBestNdx < 0)
		{
#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voice Language [%4.4X] Gender [%u] Age [%u] Defaults [%u] From [%d]"), pAgentFileTts.mLanguage, pAgentFileTts.mGender, pAgentFileTts.mAge, pUseDefaults, pStartAfter+1);
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
		}

		if	(lBestNdx >= 0)
		{
#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voice [%ls] [%ls] [%s] [%s]"), (BSTR)GetAt(lBestNdx)->mVoiceName, (BSTR)GetAt(lBestNdx)->mProduct, (CString)CGuidStr(GetAt(lBestNdx)->mModeId), (CString)CGuidStr(GetAt(lBestNdx)->mEngineId));
			LogMessage (_DEBUG_TTS_MATCH, _T(""));
#endif
			lRet = lBestNdx;
		}
	}
	catch AnyExceptionDebug

	return lRet;
}

CSapi4VoiceInfo * CSapi4Voices::GetVoice (const CAgentFileTts & pAgentFileTts, bool pUseDefaults, INT_PTR pStartAfter)
{
	return operator () (FindVoice (pAgentFileTts, pUseDefaults, pStartAfter));
}

//////////////////////////////////////////////////////////////////////

bool CSapi4Voices::RemoveVoice (INT_PTR pVoiceNdx)
{
	if	(
			(pVoiceNdx >= 0)
		&&	(pVoiceNdx <= GetUpperBound ())
		)
	{
		if	(LogIsActive (mLogLevelDebug))
		{
			LogMessage (mLogLevelDebug, _T("Remove Voice [%s] [%s]"), (CString)CGuidStr(GetAt(pVoiceNdx)->mModeId), (CString)CGuidStr(GetAt(pVoiceNdx)->mEngineId));
		}
		DeleteAt (pVoiceNdx);
		return true;
	}
	return false;
}

bool CSapi4Voices::RemoveVoice (const CSapi4VoiceInfo * pVoiceInfo)
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

bool CSapi4Voices::VoiceSupportsLanguage (CSapi4VoiceInfo * pVoiceInfo, LANGID pLangId, bool pUseDefaults)
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

void CSapi4Voices::MakeLanguageMatchList (LANGID pLanguageId, CArrayEx <LANGID, LANGID> & pLanguageIds, bool pUseDefaults)
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

void CSapi4Voices::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
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
				lTitle = _T("Sapi4 Voices");
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

void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo & pVoiceInfo, LPCTSTR pTitle, LPCTSTR pIndent)
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
			LogMessage (pLogLevel, _T("%s  ModeId       [%s]"), lIndent, (CString)CGuidStr(pVoiceInfo.mModeId));
			LogMessage (pLogLevel, _T("%s  EngineId     [%s]"), lIndent, (CString)CGuidStr(pVoiceInfo.mEngineId));
			LogMessage (pLogLevel, _T("%s  Manufacturer [%ls]"), lIndent, (BSTR)pVoiceInfo.mManufacturer);
			LogMessage (pLogLevel, _T("%s  Product      [%ls]"), lIndent, (BSTR)pVoiceInfo.mProduct);
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

void CSapi4Voices::LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, LPCTSTR pTitle)
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
			PTTSMODEINFO	lModeInfo = (PTTSMODEINFO) pModeInfo;
			CString			lTitle (pTitle);
			CString			lFeatures;
			CString			lInterfaces;

			if	(lTitle.IsEmpty ())
			{
				lTitle = _T("Sapi4TtsMode");
			}
			LogMessage (pLogLevel, _T("%s [%p]"), lTitle, lModeInfo);

			if	(lModeInfo)
			{
				if	(lModeInfo->dwFeatures & TTSFEATURE_ANYWORD)
				{
					lFeatures += _T("ANYWORD ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_VOLUME)
				{
					lFeatures += _T("VOLUME ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_SPEED)
				{
					lFeatures += _T("SPEED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_PITCH)
				{
					lFeatures += _T("PITCH ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_TAGGED)
				{
					lFeatures += _T("TAGGED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_IPAUNICODE)
				{
					lFeatures += _T("IPAUNICODE ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_VISUAL)
				{
					lFeatures += _T("VISUAL ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_WORDPOSITION)
				{
					lFeatures += _T("WORDPOSITION ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_PCOPTIMIZED)
				{
					lFeatures += _T("PCOPTIMIZED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_PHONEOPTIMIZED)
				{
					lFeatures += _T("PHONEOPTIMIZED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_FIXEDAUDIO)
				{
					lFeatures += _T("FIXEDAUDIO ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_SINGLEINSTANCE)
				{
					lFeatures += _T("SINGLEINSTANCE ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_THREADSAFE)
				{
					lFeatures += _T("THREADSAFE ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_IPATEXTDATA)
				{
					lFeatures += _T("IPATEXTDATA ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_PREFERRED)
				{
					lFeatures += _T("PREFERRED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_TRANSPLANTED)
				{
					lFeatures += _T("TRANSPLANTED ");
				}
				if	(lModeInfo->dwFeatures & TTSFEATURE_SAPI4)
				{
					lFeatures += _T("SAPI4 ");
				}
				lFeatures.TrimRight ();

				if	(lModeInfo->dwInterfaces & TTSI_ILEXPRONOUNCE)
				{
					lInterfaces += _T("ILEXPRONOUNCE ");
				}
				if	(lModeInfo->dwInterfaces & TTSI_ILEXPRONOUNCE2)
				{
					lInterfaces += _T("ILEXPRONOUNCE2 ");
				}
				if	(lModeInfo->dwInterfaces & TTSI_ITTSATTRIBUTES)
				{
					lInterfaces += _T("ITTSATTRIBUTES ");
				}
				if	(lModeInfo->dwInterfaces & TTSI_ITTSCENTRAL)
				{
					lInterfaces += _T("ITTSCENTRAL ");
				}
				if	(lModeInfo->dwInterfaces & TTSI_ITTSDIALOGS)
				{
					lInterfaces += _T("ITTSDIALOGS ");
				}
				if	(lModeInfo->dwInterfaces & TTSI_IATTRIBUTES)
				{
					lInterfaces += _T("IATTRIBUTES ");
				}
				lInterfaces.TrimRight ();

				LogMessage (pLogLevel|LogHighVolume, _T("  Engine Id    [%s]"), (CString) CGuidStr (lModeInfo->gEngineID));
				LogMessage (pLogLevel|LogHighVolume, _T("  Mode Id      [%s]"), (CString) CGuidStr (lModeInfo->gModeID));
				LogMessage (pLogLevel|LogHighVolume, _T("  Mode Name    [%s]"), lModeInfo->szModeName);
				LogMessage (pLogLevel|LogHighVolume, _T("  Language     [%4.4X %s]"), lModeInfo->language.LanguageID, lModeInfo->language.szDialect);
				LogMessage (pLogLevel|LogHighVolume, _T("  Speaker      [%s]"), lModeInfo->szSpeaker);
				LogMessage (pLogLevel|LogHighVolume, _T("  Style        [%s]"), lModeInfo->szStyle);
				LogMessage (pLogLevel|LogHighVolume, _T("  Gender       [%hu]"), lModeInfo->wGender);
				LogMessage (pLogLevel|LogHighVolume, _T("  Age          [%hu]"), lModeInfo->wAge);
				LogMessage (pLogLevel|LogHighVolume, _T("  Manufacturer [%s]"), lModeInfo->szMfgName);
				LogMessage (pLogLevel|LogHighVolume, _T("  Product      [%s]"), lModeInfo->szProductName);
				LogMessage (pLogLevel|LogHighVolume, _T("  Features     [%8.8X] [%s]"), lModeInfo->dwFeatures, lFeatures);
				LogMessage (pLogLevel|LogHighVolume, _T("  Interfaces   [%8.8X] [%s]"), lModeInfo->dwInterfaces, lInterfaces);
				LogMessage (pLogLevel, _T(""));
			}
		}
		catch AnyExceptionSilent
	}
}
