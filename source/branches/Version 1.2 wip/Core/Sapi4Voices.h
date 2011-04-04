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
#pragma once

#ifdef	_M_CEE
#ifndef	_DACORE_IMPEXP
#define	_DACORE_IMPEXP
#endif
#endif

#ifdef	_M_CEE
using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
#else
#include "DaCoreExp.h"
#include "SapiLanguageMatch.h"
#endif

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
#ifdef	_M_CEE
namespace DoubleAgent {
#else
#pragma warning (push)
#pragma warning (disable: 4251)
#endif
/////////////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CSapi4VoiceInfo Sapi4VoiceInfo
public ref class CSapi4VoiceInfo
#else
class _DACORE_IMPEXP CSapi4VoiceInfo
#endif
{
public:
	CSapi4VoiceInfo ();
	virtual ~CSapi4VoiceInfo ();

public:
#ifdef	_M_CEE
	System::String^	VoiceName;
	System::Guid	ModeId;
	System::Guid	EngineId;
	LANGID			LangId;
	UInt16			SpeakerGender;
	UInt16			SpeakerAge;
	System::String^	Product;
	System::String^	Manufacturer;
#else
	tBstrPtr		mVoiceName;
	GUID			mModeId;
	GUID			mEngineId;
	LANGID			mLangId;
	UINT			mSpeakerGender;
	UINT			mSpeakerAge;
	tBstrPtr		mProduct;
	tBstrPtr		mManufacturer;
#endif	
	
#ifdef	_M_CEE
public:
    virtual System::String^ ToString() override;
#else
	DECLARE_DLL_OBJECT(CSapi4VoiceInfo)
#endif	
};

//////////////////////////////////////////////////////////////////////

#ifndef	_M_CEE
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

	static int __cdecl SortDescending (const void* pElem1, const void* pElem2)
	{
		return -ATL::CElementTraits<int>::CompareElementsOrdered (*(int *)pElem1, *(int*)pElem2);
	}
};
#endif

//////////////////////////////////////////////////////////////////////

#ifdef	_M_CEE
#define CSapi4Voices Sapi4Voices
public ref class CSapi4Voices : public System::Collections::ObjectModel::KeyedCollection <System::Guid, CSapi4VoiceInfo^>
#else
class CSapi4Voices : public CAtlOwnPtrArray <CSapi4VoiceInfo>, protected CSapiLanguageMatch
#endif
{
#ifdef	_M_CEE
public:
	CSapi4Voices();
	virtual ~CSapi4Voices();
#else
	DECLARE_DLL_OBJECT_EX(CSapi4Voices, _DACORE_IMPEXP)
protected:
	CSapi4Voices();
public:
	_DACORE_IMPEXP virtual ~CSapi4Voices();
	_DACORE_IMPEXP static CSapi4Voices * CreateInstance ();
#endif	

// Attributes
	const UINT	mLogLevelDebug;

// Operations
#ifdef	_M_CEE
	static bool IsSapi4Installed ();
	void Enumerate ();
#else
	_DACORE_IMPEXP static bool IsSapi4Installed ();
	_DACORE_IMPEXP void Enumerate ();

	_DACORE_IMPEXP INT_PTR FindModeId (const GUID& pModeId);
	_DACORE_IMPEXP CSapi4VoiceInfo* GetModeId (const GUID& pModeId);

	_DACORE_IMPEXP INT_PTR FindVoiceName (LPCTSTR pVoiceName);
	_DACORE_IMPEXP CSapi4VoiceInfo* GetVoiceName (LPCTSTR pVoiceName);

	_DACORE_IMPEXP INT_PTR FindVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);
	_DACORE_IMPEXP CSapi4VoiceInfo* GetVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, int * pMatchRank = NULL);

	_DACORE_IMPEXP CSapi4VoiceIndexArray const * FindVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks = NULL);
	_DACORE_IMPEXP CSapi4VoiceInfoArray const * GetVoices (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, CSapi4VoiceMatchRanks const ** pMatchRanks = NULL);

	_DACORE_IMPEXP bool RemoveVoice (INT_PTR pVoiceNdx);
	_DACORE_IMPEXP bool RemoveVoice (const CSapi4VoiceInfo* pVoiceInfo);

	_DACORE_IMPEXP bool VoiceSupportsLanguage (CSapi4VoiceInfo* pVoiceInfo, LANGID pLangId, bool pUseDefaults);
#endif	

#ifdef	_M_CEE
	void Log (UINT pLogLevel);
	void Log (UINT pLogLevel, System::String^ pTitle);
	void Log (UINT pLogLevel, System::String^ pTitle, System::String^ pIndent);
	static void LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo);
	static void LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo, System::String^ pTitle);
	static void LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo^ pVoiceInfo, System::String^ pTitle, System::String^ pIndent);
#else
	_DACORE_IMPEXP void Log (UINT pLogLevel, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
	_DACORE_IMPEXP static void LogVoiceInfo (UINT pLogLevel, CSapi4VoiceInfo& pVoiceInfo, LPCTSTR pTitle = NULL, LPCTSTR pIndent = NULL);
#endif	

// Implementation
protected:
#ifdef	_M_CEE
	virtual System::Guid GetKeyForItem (CSapi4VoiceInfo^ pItem) override;

	void Add (CSapi4VoiceInfo^ pItem) new {__super::Add (pItem);}
	void Insert (Int32 pNdx, CSapi4VoiceInfo^ pItem) new {__super::Insert (pNdx, pItem);}
	void Remove (System::Guid pKey) new {__super::Remove (pKey);}
	void RemoveAt (Int32 pNdx) new {__super::RemoveAt (pNdx);}
	void Clear () new {__super::Clear ();}

	static void LogModeInfo (UINT pLogLevel, LPVOID pModeInfo);
	static void LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, System::String^ pTitle);
#else
	_DACORE_IMPEXP static void LogModeInfo (UINT pLogLevel, LPVOID pModeInfo, LPCTSTR pTitle = NULL);
#endif	
};

/////////////////////////////////////////////////////////////////////////////
#ifdef	_M_CEE
} // namespace DoubleAgent
#else
#pragma warning (pop)
#endif
/////////////////////////////////////////////////////////////////////////////
