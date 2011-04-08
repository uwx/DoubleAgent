/////////////////////////////////////////////////////////////////////////////
//	Double Agent - Copyright 2009-2011 Cinnamon Software Inc.
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
#ifdef	_M_CEE
#include "InitGuid.h"
#include "Sapi4Err.h"
#else
#include "Sapi4Voice.h"
#include "Sapi4Err.h"
#include "AgentFile.h"
#include "StringArrayEx.h"
#include "GuidStr.h"
#ifdef	_DEBUG
#include "Registry.h"
#endif
#endif

#ifndef	_M_CEE
#ifdef	_DEBUG
#define	_DEBUG_VOICES		(GetProfileDebugInt(_T("LogVoices"),LogVerbose,true)&0xFFFF|LogTime)
#define	_DEBUG_TTS_MATCH	(GetProfileDebugInt(_T("LogVoiceMatch"),LogVerbose,true)&0xFFFF|LogTime)
#endif
#endif

//////////////////////////////////////////////////////////////////////

_COM_SMARTPTR_TYPEDEF (ITTSEnum, IID_ITTSEnum);

//////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
namespace DoubleAgent {
#else
IMPLEMENT_DLL_OBJECT(CSapi4Voices)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceIndexArray)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceInfoArray)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceMatchRanks)
IMPLEMENT_DLL_OBJECT(CSapi4VoiceInfo)
#endif
//////////////////////////////////////////////////////////////////////

CSapi4VoiceInfo::CSapi4VoiceInfo ()
{
#ifdef	_M_CEE
	LangId = MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL);
	SpeakerGender = GENDER_NEUTRAL;
	SpeakerAge = TTSAGE_ADULT;
	ModeId = Guid::Empty;
	EngineId = Guid::Empty;
#else
	mLangId = MAKELANGID (LANG_NEUTRAL, SUBLANG_NEUTRAL);
	mSpeakerGender = GENDER_NEUTRAL;
	mSpeakerAge = TTSAGE_ADULT;
	mModeId = GUID_NULL;
	mEngineId = GUID_NULL;
#endif
}

CSapi4VoiceInfo::~CSapi4VoiceInfo ()
{
}

//////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
System::String^ CSapi4VoiceInfo::ToString()
{
	return String::Format ("\"{0}\" [{1}] [{2:X4} {2:D4}] [{3:D}]", VoiceName, ModeId.ToString()->ToUpper(), LangId, SpeakerGender);
}
#endif

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

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
#ifdef	_M_CEE
	Clear ();
#else
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
#endif
}

#ifndef	_M_CEE
CSapi4Voices * CSapi4Voices::CreateInstance ()
{
	return new CSapi4Voices;
}
#endif

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

				if	(SUCCEEDED (CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_SERVER, __uuidof (IUnknown), (void**) &lUnknown)))
				{
					lInstalled = true;
				}
				lUnknown = NULL;

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
#ifdef	_M_CEE
	CSapi4VoiceInfo^	lVoiceInfo;
#else
	CSapi4VoiceInfo*	lVoiceInfo;
#endif

#ifdef	_M_CEE
	Clear ();
#else
	DeleteAll ();
#endif

	if	(
			(SUCCEEDED (LogComErr (LogVerbose|LogTime, CoCreateInstance (CLSID_TTSEnumerator, NULL, CLSCTX_INPROC_SERVER, IID_ITTSEnum, (void**) &lEnum))))
		&&	(lEnum != NULL)
		)
	{
#ifdef	_M_CEE
		Guid	lKnownEngine1 ("{E0725551-286F-11D0-8E73-00A0C9083363}");
#else
		GUID	lKnownEngine1 = CGuidStr::Parse (_T("{E0725551-286F-11D0-8E73-00A0C9083363}"));
#endif

		while (lEnum->Next (1, &lModeInfo, NULL) == S_OK)
		{
			if	(LogIsActive (mLogLevelDebug))
			{
				LogModeInfo (mLogLevelDebug, &lModeInfo);
			}

#ifdef	_M_CEE
			if	(lVoiceInfo = gcnew CSapi4VoiceInfo)
			{
				lVoiceInfo->EngineId = Guid (lModeInfo.gEngineID.Data1, lModeInfo.gEngineID.Data2, lModeInfo.gEngineID.Data3, lModeInfo.gEngineID.Data4[0], lModeInfo.gEngineID.Data4[1], lModeInfo.gEngineID.Data4[2], lModeInfo.gEngineID.Data4[3], lModeInfo.gEngineID.Data4[4], lModeInfo.gEngineID.Data4[5], lModeInfo.gEngineID.Data4[6], lModeInfo.gEngineID.Data4[7]);
				lVoiceInfo->ModeId = Guid (lModeInfo.gModeID.Data1, lModeInfo.gModeID.Data2, lModeInfo.gModeID.Data3, lModeInfo.gModeID.Data4[0], lModeInfo.gModeID.Data4[1], lModeInfo.gModeID.Data4[2], lModeInfo.gModeID.Data4[3], lModeInfo.gModeID.Data4[4], lModeInfo.gModeID.Data4[5], lModeInfo.gModeID.Data4[6], lModeInfo.gModeID.Data4[7]);
				lVoiceInfo->LangId = lModeInfo.language.LanguageID;
				lVoiceInfo->SpeakerGender = lModeInfo.wGender;
				lVoiceInfo->SpeakerAge = lModeInfo.wAge;
				lVoiceInfo->Manufacturer = gcnew String (lModeInfo.szMfgName);
				lVoiceInfo->Product = gcnew String (lModeInfo.szProductName);
				if	(lVoiceInfo->EngineId.Equals (lKnownEngine1))
				{
					lVoiceInfo->VoiceName = gcnew String (lModeInfo.szModeName);
				}
				else
				{
					lVoiceInfo->VoiceName = gcnew String (lModeInfo.szSpeaker);
				}
#else
			if	(lVoiceInfo = new CSapi4VoiceInfo)
			{
				lVoiceInfo->mEngineId = lModeInfo.gEngineID;
				lVoiceInfo->mModeId = lModeInfo.gModeID;
				lVoiceInfo->mLangId = lModeInfo.language.LanguageID;
				lVoiceInfo->mSpeakerGender = lModeInfo.wGender;
				lVoiceInfo->mSpeakerAge = lModeInfo.wAge;
				lVoiceInfo->mManufacturer = CAtlString (lModeInfo.szMfgName).AllocSysString ();
				lVoiceInfo->mProduct = CAtlString (lModeInfo.szProductName).AllocSysString ();
				if	(IsEqualGUID (lModeInfo.gEngineID, lKnownEngine1))
				{
					lVoiceInfo->mVoiceName = CAtlString (lModeInfo.szModeName).AllocSysString ();
				}
				else
				{
					lVoiceInfo->mVoiceName = CAtlString (lModeInfo.szSpeaker).AllocSysString ();
				}
#endif
				Add (lVoiceInfo);
			}
			lModeInfo.Clear ();
		}
	}
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////
#ifndef	_M_CEE
//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindModeId (const GUID& pModeId)
{
	INT_PTR				lNdx;
	CSapi4VoiceInfo*	lVoiceInfo;

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

CSapi4VoiceInfo* CSapi4Voices::GetModeId (const GUID& pModeId)
{
	return operator () (FindModeId (pModeId));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindVoiceName (LPCTSTR pVoiceName)
{
	INT_PTR				lNdx;
	CSapi4VoiceInfo*	lVoiceInfo;

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

CSapi4VoiceInfo* CSapi4Voices::GetVoiceName (LPCTSTR pVoiceName)
{
	return operator () (FindVoiceName (pVoiceName));
}

//////////////////////////////////////////////////////////////////////

INT_PTR CSapi4Voices::FindVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank)
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

CSapi4VoiceInfo* CSapi4Voices::GetVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank)
{
	return operator () (FindVoice (pAgentFileTts, pUseDefaults, pMatchRank));
}

//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

CSapi4VoiceIndexArray const * CSapi4Voices::FindVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks)
{
	tPtr <CSapi4VoiceIndexArray>	lIndexArray = new CSapi4VoiceIndexArray;
	tPtr <CSapi4VoiceMatchRanks>	lMatchRanks = new CSapi4VoiceMatchRanks;

	try
	{
		CAtlTypeArray <LANGID>		lLanguageIds;
		INT_PTR						lLanguageNdx;
		INT_PTR						lVoiceNdx;
		static const int			lLanguageWeight = 1000;
		static const int			lGenderWeight = 1000;
		static const int			lGuidWeight = 300;
		static const int			lAgeWeight = 100;
		static const int			lOrderWeight = 1;

#ifdef	_DEBUG_TTS_MATCH
		if	(IsEqualGUID (pAgentFileTts.Mode, GUID_NULL))
		{
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voices [%u] Language [%4.4X] Gender [%u] Age [%u]"), pUseDefaults, pAgentFileTts.Language, pAgentFileTts.Gender, pAgentFileTts.Age);
		}
		else
		{
			LogMessage (_DEBUG_TTS_MATCH, _T("FindSapi4Voices [%u] GUID [%s] Language [%4.4X] Gender [%u] Age [%u]"), pUseDefaults, (CString)CGuidStr(pAgentFileTts.Mode), pAgentFileTts.Language, pAgentFileTts.Gender, pAgentFileTts.Age);
		}
		LogLanguageMatchList (_DEBUG_TTS_MATCH, pAgentFileTts.Language, pUseDefaults, NULL, _T("  "));
#endif
		MakeLanguageMatchList (pAgentFileTts.Language, lLanguageIds, pUseDefaults);

		for	(lVoiceNdx = 0; lVoiceNdx < (INT_PTR)GetCount(); lVoiceNdx++)
		{
			CSapi4VoiceInfo*	lVoiceInfo = GetAt (lVoiceNdx);
			int					lPartMatch = 0;
			int					lCurrMatch = 0;

#ifdef	_DEBUG_TTS_MATCH
			CAtlString	lMatchLog;
			lMatchLog.Format (_T("%-40.40ls"), (CString)CGuidStr(lVoiceInfo->mModeId));
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
					(lLanguageNdx >= 0)
				&&	(!IsEqualGUID (pAgentFileTts.Mode, GUID_NULL))
				&&	(IsEqualGUID (pAgentFileTts.Mode, lVoiceInfo->mModeId))
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

CSapi4VoiceInfoArray const * CSapi4Voices::GetVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks)
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

bool CSapi4Voices::RemoveVoice (const CSapi4VoiceInfo* pVoiceInfo)
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

bool CSapi4Voices::VoiceSupportsLanguage (CSapi4VoiceInfo* pVoiceInfo, LANGID pLangId, bool pUseDefaults)
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
#else	// _M_CEE
//////////////////////////////////////////////////////////////////////

System::Guid CSapi4Voices::GetKeyForItem (CSapi4VoiceInfo^ pItem)
{
	return pItem->ModeId;
}

//////////////////////////////////////////////////////////////////////
#endif	// _M_CEE
//////////////////////////////////////////////////////////////////////
#pragma page()
//////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
void CSapi4Voices::Log (UINT pLogLevel)
{
	Log (pLogLevel, nullptr);
}

void CSapi4Voices::Log (UINT pLogLevel, System::String^ pTitle)
{
	Log (pLogLevel, pTitle, nullptr);
}

void CSapi4Voices::Log (UINT pLogLevel, System::String^ pTitle, System::String^ pIndent)
#else
void CSapi4Voices::Log (UINT pLogLevel, LPCTSTR pTitle, LPCTSTR pIndent)
#endif
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
#ifdef	_M_CEE
#else
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
#endif
		}
		catch AnyExceptionDebug
	}
}

//////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo)
{
	LogVoiceInfo (pLogLevel, pVoiceInfo, nullptr);
}

void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo, System::String^ pTitle)
{
	LogVoiceInfo (pLogLevel, pVoiceInfo, pTitle, nullptr);
}

void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo, System::String^ pTitle, System::String^ pIndent)
#else
void CSapi4Voices::LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo& pVoiceInfo, LPCTSTR pTitle, LPCTSTR pIndent)
#endif
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
#ifdef	_M_CEE
#else
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
#endif
		}
		catch AnyExceptionDebug
	}
}


//////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
void CSapi4Voices::LogModeInfo (UINT pLogLevel, LPVOID pModeInfo)
{
	LogModeInfo (pLogLevel, pModeInfo, nullptr);
}

void CSapi4Voices::LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, System::String^ pTitle)
#else
void CSapi4Voices::LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, LPCTSTR pTitle)
#endif
{
	if	(LogIsActive (pLogLevel))
	{
		try
		{
#ifdef	_M_CEE
#else
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
#endif
		}
		catch AnyExceptionSilent
	}
}

//////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace DoubleAgent
#endif
/////////////////////////////////////////////////////////////////////////////
