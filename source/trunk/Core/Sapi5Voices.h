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
#pragma once
#include "DaCoreExp.h"
#include "SapiLanguageMatch.h"

/////////////////////////////////////////////////////////////////////////////

#ifndef	GENDER_NEUTRAL
#define	GENDER_NEUTRAL		0
#define	GENDER_FEMALE		1
#define	GENDER_MALE			2
#endif

#ifndef	TTSAGE_BABY
#define	TTSAGE_BABY			1
#define	TTSAGE_TODDLER		3
#define	TTSAGE_CHILD		6
#define	TTSAGE_ADOLESCENT	14
#define	TTSAGE_ADULT		30
#define	TTSAGE_ELDERLY		70
#endif

/////////////////////////////////////////////////////////////////////////////
#pragma warning (push)
#pragma warning (disable: 4251)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi5VoiceInfo
{
	DECLARE_DLL_OBJECT(CSapi5VoiceInfo)
public:
	CSapi5VoiceInfo ();
	virtual ~CSapi5VoiceInfo ();

// Attributes
	tBstrPtr	mVoiceName;
	tBstrPtr	mVoiceIdShort;
	tBstrPtr	mVoiceIdLong;
	LANGID		mLangId;
	UINT		mSpeakerGender;
	UINT		mSpeakerAge;
	tBstrPtr	mProduct;
	tBstrPtr	mManufacturer;
};

/////////////////////////////////////////////////////////////////////////////

class CSapi5VoiceIndexArray : public CAtlTypeArray <INT_PTR>
{
	DECLARE_DLL_OBJECT(CSapi5VoiceIndexArray)
public:
	CSapi5VoiceIndexArray() {}
	virtual ~CSapi5VoiceIndexArray() {}
};

class CSapi5VoiceInfoArray : public CAtlPtrTypeArray <CSapi5VoiceInfo>
{
	DECLARE_DLL_OBJECT(CSapi5VoiceInfoArray)
public:
	CSapi5VoiceInfoArray() {}
	virtual ~CSapi5VoiceInfoArray() {}
};

class CSapi5VoiceMatchRanks : public CAtlTypeArray <int>
{
	DECLARE_DLL_OBJECT(CSapi5VoiceMatchRanks)
public:
	CSapi5VoiceMatchRanks() {}
	virtual ~CSapi5VoiceMatchRanks() {}

	static int __cdecl SortDescending (const void* pElem1, const void* pElem2)
	{
		return -ATL::CElementTraits<int>::CompareElementsOrdered (*(int *)pElem1, *(int*)pElem2);
	}
};

/////////////////////////////////////////////////////////////////////////////

class CSapi5Voices : public CAtlOwnPtrArray <CSapi5VoiceInfo>, protected CSapiLanguageMatch
{
	DECLARE_DLL_OBJECT_EX(CSapi5Voices, _DACORE_IMPEXP)
protected:
	CSapi5Voices();
public:
	_DACORE_IMPEXP virtual ~CSapi5Voices();
	_DACORE_IMPEXP static CSapi5Voices * CreateInstance ();

// Attributes
public:
	const UINT	mLogLevelDebug;

// Operations
public:
	_DACORE_IMPEXP static bool IsSapi5Installed ();
	_DACORE_IMPEXP void Enumerate ();

	_DACORE_IMPEXP INT_PTR FindVoiceId (LPCTSTR pVoiceId);
	_DACORE_IMPEXP CSapi5VoiceInfo * GetVoiceId (LPCTSTR pVoiceId);

	_DACORE_IMPEXP INT_PTR FindVoiceName (LPCTSTR pVoiceName);
	_DACORE_IMPEXP CSapi5VoiceInfo * GetVoiceName (LPCTSTR pVoiceName);

	_DACORE_IMPEXP INT_PTR FindVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);
	_DACORE_IMPEXP CSapi5VoiceInfo * GetVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);

	_DACORE_IMPEXP CSapi5VoiceIndexArray const * FindVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi5VoiceMatchRanks const ** pMatchRanks = NULL);
	_DACORE_IMPEXP CSapi5VoiceInfoArray const * GetVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi5VoiceMatchRanks const ** pMatchRanks = NULL);

	_DACORE_IMPEXP bool RemoveVoice (INT_PTR pVoiceNdx);
	_DACORE_IMPEXP bool RemoveVoice (const CSapi5VoiceInfo * pVoiceInfo);

	_DACORE_IMPEXP INT_PTR DefaultVoiceNdx ();
	_DACORE_IMPEXP bool VoiceSupportsLanguage (CSapi5VoiceInfo * pVoiceInfo, LANGID pLangId, bool pUseDefaults);

	_DACORE_IMPEXP void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	_DACORE_IMPEXP static void LogVoiceInfo (UINT pLogLevel, CSapi5VoiceInfo& pVoiceInfo, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);

// Implementation
protected:
	CSapi5VoiceInfo * UniqueVoiceFromToken (struct ISpObjectToken * pVoiceToken);
public:
	static _DACORE_IMPEXP CSapi5VoiceInfo * VoiceFromToken (struct ISpObjectToken * pVoiceToken);
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
