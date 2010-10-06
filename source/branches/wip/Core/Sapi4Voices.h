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
#pragma warning (disable: 4251 4275)
/////////////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi4VoiceInfo
{
	DECLARE_DLL_OBJECT(CSapi4VoiceInfo)
public:
	CSapi4VoiceInfo ();
	virtual ~CSapi4VoiceInfo ();

// Attributes
	tBstrPtr	mVoiceName;
	GUID		mModeId;
	GUID		mEngineId;
	LANGID		mLangId;
	UINT		mSpeakerGender;
	UINT		mSpeakerAge;
	tBstrPtr	mProduct;
	tBstrPtr	mManufacturer;
};

//////////////////////////////////////////////////////////////////////

class CSapi4VoiceIndexArray : public CAtlTypeArray <INT_PTR>  
{
	DECLARE_DLL_OBJECT(CSapi4VoiceIndexArray)
public:
	CSapi4VoiceIndexArray() {}
	virtual ~CSapi4VoiceIndexArray() {}
};

class CSapi4VoiceInfoArray : public CAtlPtrTypeArray <CSapi4VoiceInfo>
{
	DECLARE_DLL_OBJECT(CSapi4VoiceInfoArray)
public:
	CSapi4VoiceInfoArray() {}
	virtual ~CSapi4VoiceInfoArray() {}
};

class CSapi4VoiceMatchRanks : public CAtlTypeArray <int>
{
	DECLARE_DLL_OBJECT(CSapi4VoiceMatchRanks)
public:
	CSapi4VoiceMatchRanks() {}
	virtual ~CSapi4VoiceMatchRanks() {}

	static int __cdecl SortDescending (const void * pElem1, const void * pElem2)
	{
		return -ATL::CElementTraits<int>::CompareElementsOrdered (*(int *)pElem1, *(int*)pElem2);
	}
};

//////////////////////////////////////////////////////////////////////

class _DACORE_IMPEXP CSapi4Voices : public CAtlOwnPtrArray <CSapi4VoiceInfo>, protected CSapiLanguageMatch
{
	DECLARE_DLL_OBJECT(CSapi4Voices)
protected:
	CSapi4Voices();
public:
	virtual ~CSapi4Voices();
	static CSapi4Voices * CreateInstance ();

// Attributes
	const UINT	mLogLevelDebug;

// Operations
	static bool IsSapi4Installed ();
	void Enumerate ();

	INT_PTR FindModeId (const GUID & pModeId);
	CSapi4VoiceInfo * GetModeId (const GUID & pModeId);

	INT_PTR FindVoiceName (LPCTSTR pVoiceName);
	CSapi4VoiceInfo * GetVoiceName (LPCTSTR pVoiceName);

	INT_PTR FindVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);
	CSapi4VoiceInfo * GetVoice (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);

	CSapi4VoiceIndexArray const * FindVoices (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks = NULL);
	CSapi4VoiceInfoArray const * GetVoices (const struct CAgentFileTts & pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks = NULL);

	bool RemoveVoice (INT_PTR pVoiceNdx);
	bool RemoveVoice (const CSapi4VoiceInfo * pVoiceInfo);

	bool VoiceSupportsLanguage (CSapi4VoiceInfo * pVoiceInfo, LANGID pLangId, bool pUseDefaults);

	void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	static void LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo & pVoiceInfo, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);

// Implementation
protected:
	static void LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, LPCTSTR pTitle = NULL);
};

#pragma warning (pop)
/////////////////////////////////////////////////////////////////////////////
