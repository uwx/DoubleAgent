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
#ifdef	_DEBUG
#include "Registry.h"
#endif

#ifdef	_DEBUG
#define	_DEBUG_VOICES		(GetProfileDebugInt(_T("LogVoices"),LogVerbose,true)&0xFFFF)
#define	_DEBUG_TTS_MATCH	(GetProfileDebugInt(_T("LogVoiceMatch"),LogVerbose,true)&0xFFFF)
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSEnum, IID_ITTSEnum);

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DLL_OBJECT(CSapi4VoiceIndexArray)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceInfoArray)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceMatchRanks)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceInfo)

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

IMPLEMENT_DLL_OBJECT(CSapi4Voices)

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

CSapi4Voices * CSapi4Voices::CreateInstance ()
{
	return new CSapi4Voices;
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
				lVoiceInfo->mVoiceName = CAtlString (lModeInfo.szSpeaker).AllocSysString ();
				lVoiceInfo->mSpeakerGender = lModeInfo.wGender;
				lVoiceInfo->mSpeakerAge = lModeInfo.wAge;
				lVoiceInfo->mManufacturer = CAtlString (lModeInfo.szMfgName).AllocSysString ();
				lVoiceInfo->mProduct = CAtlString (lModeInfo.szProductName).AllocSysString ();

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

	for	(lNdx = 0; lNdx < (INT_PTR)GetCount(); lNdx++)
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

CSapi4VoiceInfo * CSapi4Voices::GetVoiceName (LPCTSTR pVoiceName)
{
	return operator () (FindVoiceName (pVoiceName));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, int * pMatchRank)
{
	tPtr <CSapi4VoiceIndexArray const>	lIndexArray;
	tPtr <CSapi4VoiceMatchRanks const>	lMatchRanks;

	if	(lIndexArray = FindVoices (pAgentFileTts, pUseDefaults, lMatchRanks.Free()))
	{
#ifdef	_DEBUG_TTS_MATCH
		LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voice [%u] %4.4d [%ls] [%ls] [%s] [%s]"), pUseDefaults, (*lMatchRanks) [0], (BSTR)GetAt((*lIndexArray) [0])->mVoiceName, (BSTR)GetAt((*lIndexArray) [0])->mProduct, (CString)CGuidStr(GetAt((*lIndexArray) [0])->mModeId), (CString)CGuidStr(GetAt((*lIndexArray) [0])->mEngineId));
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

CSapi4VoiceInfo * CSapi4Voices::GetVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, int * pMatchRank)
{
	return operator () (FindVoice (pAgentFileTts, pUseDefaults, pMatchRank));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapi4VoiceIndexArray const * CSapi4Voices::FindVoices (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks)
{
	tPtr <CSapi4VoiceIndexArray>	lIndexArray = new CSapi4VoiceIndexArray;
	tPtr <CSapi4VoiceMatchRanks>	lMatchRanks = new CSapi4VoiceMatchRanks;

	try
	{
		CAtlTypeArray <LANGID>		lLanguageIds;
		INT_PTR						lLanguageNdx;
		INT_PTR						lVoiceNdx;
		static const int			lLanguageWeight = 1000;
		static const int			lGenderWeight = 10000;
		static const int			lGuidWeight = 200;
		static const int			lAgeWeight = 100;
		static const int			lOrderWeight = 1;

#ifdef	_DEBUG_TTS_MATCH
		if	(IsEqualGUID (pAgentFileTts.mMode, GUID_NULL))
		{
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voices [%u] Language [%4.4X] Gender [%u] Age [%u]"), pUseDefaults, pAgentFileTts.mLanguage, pAgentFileTts.mGender, pAgentFileTts.mAge);
		}
		else
		{
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voices [%u] GUID [%s] Language [%4.4X] Gender [%u] Age [%u]"), pUseDefaults, (CString)CGuidStr(pAgentFileTts.mMode), pAgentFileTts.mLanguage, pAgentFileTts.mGender, pAgentFileTts.mAge);
		}
		LogLanguageMatchList (_DEBUG_TTS_MATCH, pAgentFileTts.mLanguage, pUseDefaults, NULL, _T("  "));
#endif
		MakeLanguageMatchList (pAgentFileTts.mLanguage, lLanguageIds, pUseDefaults);

		for	(lVoiceNdx = 0; lVoiceNdx < (INT_PTR)GetCount(); lVoiceNdx++)
		{
			CSapi4VoiceInfo *	lVoiceInfo = GetAt (lVoiceNdx);
			int					lPartMatch = 0;
			int					lCurrMatch = 0;

#ifdef	_DEBUG_TTS_MATCH
			CAtlString	lMatchLog;
			lMatchLog.Format (_T("%-20.20ls"), (BSTR)lVoiceInfo->mProduct);
#endif
			
			lLanguageNdx = FindLanguageMatch (lVoiceInfo->mLangId, lLanguageIds);
			if	(lLanguageNdx >= 0)
			{
				lCurrMatch += lPartMatch = (int)(lLanguageIds.GetCount()-lLanguageNdx) * lLanguageWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Language [%4.4X (%d)]"), CAtlString((LPCTSTR)lMatchLog), lLanguageIds [lLanguageNdx], lPartMatch);
#endif
			}

			if	(
					(lLanguageNdx >= 0)
				&&	(!IsEqualGUID (pAgentFileTts.mMode, GUID_NULL))
				&&	(IsEqualGUID (pAgentFileTts.mMode, lVoiceInfo->mModeId))
				)
			{
				lCurrMatch += lPartMatch = lGenderWeight + lGuidWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s GUID [%s (%d)]"), CAtlString((LPCTSTR)lMatchLog), (CString)CGuidStr(lVoiceInfo->mModeId), lPartMatch);
#endif
			}
			else
			if	(
					(lCurrMatch > 0)
				&&	(pAgentFileTts.mGender != GENDER_NEUTRAL)
				)
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
					lMatchLog.Format (_T("%s Gender [%u (%d)]"), CAtlString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerGender, lPartMatch);
				}
#endif
			}

			if	(
					(lCurrMatch > 0)
				&&	(pAgentFileTts.mAge != 0)
				&&	(lVoiceInfo->mSpeakerAge != 0)
				)
			{
				lCurrMatch += lPartMatch = -abs((int)pAgentFileTts.mAge-(int)lVoiceInfo->mSpeakerAge) * lAgeWeight;
#ifdef	_DEBUG_TTS_MATCH
				lMatchLog.Format (_T("%s Age [%u (%d)]"), CAtlString((LPCTSTR)lMatchLog), lVoiceInfo->mSpeakerAge, lPartMatch);
#endif
			}

#ifdef	_DEBUG_TTS_MATCH
			LogMessage (_DEBUG_TTS_MATCH|LogHighVolume, _T("  %3d [%6.6d] %s Name [%ls]"), lVoiceNdx, lCurrMatch, lMatchLog, (BSTR)GetAt(lVoiceNdx)->mVoiceName);
#endif
			if	(lCurrMatch > 0)
			{
				lIndexArray->InsertAt (lMatchRanks->AddSortedQS (lCurrMatch + (((int)GetCount()-lVoiceNdx)*lOrderWeight), CSapi4VoiceMatchRanks::SortDescending, false), lVoiceNdx);
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

CSapi4VoiceInfoArray const * CSapi4Voices::GetVoices (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks)
{
	tPtr <CSapi4VoiceInfoArray>			lInfoArray;
	tPtr <CSapi4VoiceIndexArray const>	lIndexArray;
	INT_PTR								lNdx;
	
	if	(
			(lIndexArray = FindVoices (pAgentFileTts, pUseDefaults, pMatchRanks))
		&&	(lInfoArray = new CSapi4VoiceInfoArray)
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

bool CSapi4Voices::RemoveVoice (INT_PTR pVoiceNdx)
{
	if	(
			(pVoiceNdx >= 0)
		&&	(pVoiceNdx < (INT_PTR)GetCount())
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

void CSapi4Voices::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
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
				lTitle = _T("Sapi4 Voices");
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

void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo & pVoiceInfo, LPCTSTR pTitle, LPCTSTR pIndent)
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
			CAtlString		lTitle (pTitle);
			CAtlString		lFeatures;
			CAtlString		lInterfaces;

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
