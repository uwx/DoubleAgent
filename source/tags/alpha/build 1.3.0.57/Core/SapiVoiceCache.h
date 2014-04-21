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
#include "AtlCollEx.h"

class CSapiVoice;
class CSapi5Voice;
class CSapi5Voices;
#ifndef	_WIN64
class CSapi4Voice;
class CSapi4Voices;
#endif

//////////////////////////////////////////////////////////////////////

class CSapiVoiceClient
{
public:
	CSapiVoiceClient () {}
	virtual ~CSapiVoiceClient () {}
};

//////////////////////////////////////////////////////////////////////

class CSapiVoiceCache
{
	DECLARE_DLL_OBJECT_EX(CSapiVoiceCache, _DACORE_IMPEXP)
public:
	_DACORE_IMPEXP CSapiVoiceCache ();
	_DACORE_IMPEXP virtual ~CSapiVoiceCache ();

// Attributes
public:
	_DACORE_IMPEXP static CSapiVoiceCache * GetStaticInstance ();

	_DACORE_IMPEXP UINT GetSapiVersionRestriction () const;
	_DACORE_IMPEXP UINT GetSapiVersionPreference () const;
	_DACORE_IMPEXP bool RestrictSapiVersion (UINT pSapiVersion);
	_DACORE_IMPEXP bool PreferSapiVersion (UINT pSapiVersion);

#ifndef	_WIN64
	_DACORE_IMPEXP CSapi4Voices * GetSapi4Voices ();
#endif
	_DACORE_IMPEXP CSapi5Voices * GetSapi5Voices ();

// Operations
public:
	_DACORE_IMPEXP CSapiVoice* GetAgentVoice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, bool pCached = true);
	_DACORE_IMPEXP CSapiVoice* GetAgentVoice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);

	_DACORE_IMPEXP bool CacheVoice (CSapiVoice* pVoice, CSapiVoiceClient * pClient);
	_DACORE_IMPEXP bool UncacheVoice (CSapiVoice* pVoice);
	_DACORE_IMPEXP bool AddVoiceClient (CSapiVoice* pVoice, CSapiVoiceClient * pClient);
	_DACORE_IMPEXP bool RemoveVoiceClient (CSapiVoice* pVoice, CSapiVoiceClient * pClient, bool pDeleteUnusedVoice = true);

	_DACORE_IMPEXP CSapiVoice* GetCachedVoice (INT_PTR pVoiceNdx);
	_DACORE_IMPEXP CSapiVoice* FindCachedVoice (LPCTSTR pVoiceId);
	_DACORE_IMPEXP bool GetVoiceClients (CSapiVoice* pVoice, CAtlPtrTypeArray <CSapiVoiceClient>& pClients);

	_DACORE_IMPEXP void Terminate (DWORD pWaitForCompletion = 0);
	_DACORE_IMPEXP bool DeleteUnusedVoices ();
	_DACORE_IMPEXP void DeleteAllVoices ();
	_DACORE_IMPEXP static void TerminateStaticInstance (DWORD pWaitForCompletion = 5000);
	_DACORE_IMPEXP static void CleanupStaticInstance ();

// Implementation
protected:
	CSapi5Voice* GetAgentSapi5Voice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapi5Voice* GetAgentSapi5Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);
#ifndef	_WIN64
	CSapi4Voice* GetAgentSapi4Voice (const class CAgentFileTts& pAgentFileTts, bool pUseDefaults, bool pCached = true);
	CSapi4Voice* GetAgentSapi4Voice (LPCTSTR pVoiceName, LANGID pLangID, bool pUseDefaults, bool pCached = true);
#endif

protected:
	CComAutoCriticalSection												mCritSec;
#ifndef	_WIN64
	UINT																mSapiVersionRestriction;
	UINT																mSapiVersionPreference;
	tPtr <CSapi4Voices>													mSapi4Voices;
#endif
	tPtr <CSapi5Voices>													mSapi5Voices;
	CAtlOwnPtrArray <CSapiVoice>										mCachedVoices;
	CAtlOwnPtrMap <CSapiVoice*, CAtlPtrTypeArray <CSapiVoiceClient> >	mVoiceClients;
	bool																mVoiceDeletePending;
};

//////////////////////////////////////////////////////////////////////
